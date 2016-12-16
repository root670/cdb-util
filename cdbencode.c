#include "cdbencode.h"
#include "libcheats.h"
#include "miniz.h"
#include <string.h>

#define CDB_VERSION    0x02

#define WRITE_32(address, value) \
    *((unsigned char *)address + 0) = (value & 0x000000ff); \
    *((unsigned char *)address + 1) = (value & 0x0000ff00) >> 8; \
    *((unsigned char *)address + 2) = (value & 0x00ff0000) >> 16; \
    *((unsigned char *)address + 3) = (value & 0xff000000) >> 24;

#define WRITE_16(address, value) \
    *((unsigned char *)address + 0) = (value & 0x00ff); \
    *((unsigned char *)address + 1) = (value & 0xff00) >> 8;

unsigned short numGames = 0;
unsigned short numCheats = 0;
unsigned char numCodes = 0;

unsigned char headerBuf[16] = "CDB";
unsigned char *gamesBuf;
unsigned char *cheatsBuf;

int cheatsToFile(char *path, cheats_t *cheats)
{
    unsigned int gamesBufOffset = 0;
    unsigned int cheatsBufOffset = 0;
    unsigned int gameSectionLength = 0;
    unsigned int cheatSectionLenPrev = 0;

    game_t *game = calloc(1, sizeof(game_t));
    cheat_t *cheat = calloc(1, sizeof(cheat_t));
    code_t *code = calloc(1, sizeof(code_t));
    gamesBuf = calloc(2, 1024*1024); // 2MB
    cheatsBuf = calloc(10, 1024*1024); // 10MB

    if(!path)
        return 0;
    FILE *cdbFile = fopen(path, "wb");
    if(!cdbFile)
        return 0;

    headerBuf[4] = CDB_VERSION;

    printf("Creating CDB (version %x)\n", CDB_VERSION);

    GAMES_FOREACH( game, &cheats->games )
    {
        if(game != NULL)
        {
            // Write into cheat section
            numCheats = 0;
            int totalCodeLines = 0;
            cheatSectionLenPrev = cheatsBufOffset;
            CHEATS_FOREACH( cheat, &game->cheats )
            {
                if(cheat != NULL)
                {
                    // Write cheat name
                    unsigned char descLen = strlen(cheat->desc) + 1;
                    memcpy(cheatsBuf + cheatsBufOffset, &descLen, 1);
                    cheatsBufOffset += 1;
                    strcpy((char *) cheatsBuf + cheatsBufOffset, cheat->desc);
                    cheatsBufOffset += descLen;
                    cheatsBufOffset += 1; //numCodes will go here after all have been written

                    int skip = 0;

                    numCodes = 0;
                    CODES_FOREACH( code, &cheat->codes )
                    {
                        unsigned int addr = code->addr;
                        unsigned int val = code->val;

                        // Filter out beefcode (skip 3 code lines)
                        // This is sometimes left by cb2util
                        if(addr == 0xBEEFC0DE)
                            skip = 3;

                        if(skip-- > 0)
                            continue;

                        // Write code address
                        WRITE_32(cheatsBuf + cheatsBufOffset, addr);
                        cheatsBufOffset += 4;

                        // Write code value
                        WRITE_32(cheatsBuf + cheatsBufOffset, val);
                        cheatsBufOffset += 4;

                        numCodes++;
                    }
                    totalCodeLines += numCodes;

                    // Write numCodes
                    *(cheatsBuf + cheatsBufOffset - 8*(numCodes) - 1) = numCodes;

                    numCheats++;
                }
            }

            // Write game title in game section
            unsigned char titleLen = strlen(game->title) + 1;
            memcpy(gamesBuf + gamesBufOffset, &titleLen, 1);
            gamesBufOffset += 1;
            strcpy((char *) gamesBuf + gamesBufOffset, game->title);
            gamesBufOffset += titleLen;

            // Write numCheats
            WRITE_16(gamesBuf + gamesBufOffset, numCheats);
            gamesBufOffset += 2;

            // Write totalCodeLines
            WRITE_16(gamesBuf + gamesBufOffset, totalCodeLines);
            gamesBufOffset += 2;

            // Write cheat section offset (this will be adjusted after looping...)
            int offset = cheatSectionLenPrev;
            memcpy(gamesBuf + gamesBufOffset, &offset, 4);
            gamesBufOffset += 4;

            numGames++;
            gameSectionLength = gamesBufOffset;
        }
    }

    memcpy(headerBuf + 6, &numGames, 2);

    // Size of the whole game section is known, so we add its size to each game's cheat offset
    int i = 0;
    while(i < gameSectionLength)
    {
        int titleLen = *(gamesBuf + i);
        i += titleLen + 5; // skip to cheat offset value

        unsigned int newOffset = *((int *)(gamesBuf + i)) + gameSectionLength + 16;
        WRITE_32(gamesBuf + i, newOffset);
        i += 4;
    }

    unsigned long size = 16 + gamesBufOffset + cheatsBufOffset;
    unsigned char *uncompressed = malloc(size);
    unsigned char *compressed = malloc(size);
    memcpy(uncompressed, headerBuf, 16);
    memcpy(uncompressed + 16, gamesBuf, gamesBufOffset);
    memcpy(uncompressed + 16 + gamesBufOffset, cheatsBuf, cheatsBufOffset);

    unsigned long compressedSize = size;
    int result = mz_compress2(compressed, &compressedSize, uncompressed, size, 9);

    if(result < 0)
    {
        printf("Error compressing CDB: %d\n", result);
        return 0;
    }

    cdbFile = fopen(path, "wb");
    fwrite(compressed, 1, compressedSize, cdbFile);
    fclose(cdbFile);

    free(uncompressed);
    free(compressed);
    free(game);
    free(cheat);
    free(code);
    free(gamesBuf);
    free(cheatsBuf);

    return 1;
}

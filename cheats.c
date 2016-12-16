#include "cheats.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cheatsGame_t *gamesHead = NULL;
static cheatDatabaseType_t dbType;
static int numGames = 0;
static int initialized = 0;

extern unsigned char _engine_erl_start[];

int initCheatMan()
{
    if(!initialized)
    {
        initialized = 1;
        return 1;
    }
    else
        return 0;
}

int killCheatMan()
{
    if(initialized)
    {
        dbCloseDatabase();

        cheatsGame_t *game = gamesHead;
        while(game)
        {
            cheatsGame_t *next = game->next;
            if(game->cheats != NULL) {
                free(game->cheats->codeLines);
                free(game->cheats);
            }

            game = next;
        }

        free(gamesHead);
    }

    return 1;
}

// CheatDB --> Game --> Cheat --> Code

int cheatsOpenDatabase(const char* path, cheatDatabaseType_t t)
{
    if(initialized)
    {
        dbType = t;
        switch(dbType)
        {
            case TEXT:
                //numGames = textCheatsOpenFile(path);
                //gamesHead = textCheatsGetCheatStruct();
                //textCheatsClose();
                break;

            case BINARY:
                numGames = dbOpenDatabase(path);
                gamesHead = dbGetCheatStruct();
                dbCloseDatabase();
                break;

            default:
                break;
        }
    }

    printf("Database contains %d games\n", numGames);

    return numGames;
}

int cheatsConvertToTxt(char *txtPath)
{
    FILE *txtFile;
    cheatsGame_t *game;
    cheatsCheat_t *cheat;

    if(initialized)
    {
        if(numGames > 0 && gamesHead != NULL)
        {
            txtFile = fopen(txtPath, "w");
            if(!txtFile)
            {
                printf("Failed to open %s\n", txtPath);
                return 0;
            }

            game = gamesHead;

            while(game)
            {
                fprintf(txtFile, "\"%s\"\n", game->title);
                cheat = game->cheats;

                while(cheat)
                {
                    fprintf(txtFile, "%s\n", cheat->title);

                    for(int i = 0; i < cheat->numCodeLines; i++)
                    {
                        fprintf(txtFile, "%08X ", *((uint32_t *) &cheat->codeLines[i]));
                        fprintf(txtFile, "%08X\n", *((uint32_t *) &cheat->codeLines[i] + 1));
                    }

                    cheat = cheat->next;
                }

                fprintf(txtFile, "\n");
                game = game->next;
            }

            fclose(txtFile);
            printf("Saved text cheats to %s.\n", txtPath);

            return 1;
        }
    }

    return 0;
}


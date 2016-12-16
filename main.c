#include <stdio.h>
#include <stdlib.h>
#include "libcheats.h"
#include "cdbencode.h"
#include "cheatlist.h"
#include "ctype.h"
#include "cheats.h"

void printUsage()
{
    printf("cdb-util v1.2.1 - convert cheat codes to/from a CDB database\n");
    printf("Compiled " __DATE__ " " __TIME__ "\n");
    printf("Usage:\n");
    printf("Create CDB from text cheats:\n\tcdb-util c <text file 1> [text file 2] ... <cdb file>\n");
    printf("Extract text cheats from CDB:\n\tcdb-util e <cdb file> <text file>\n");
}

int main(int argc, char *argv[])
{
    cheats_t *cheats_array, *cheats, master;
    int numLists;

    if(argc < 4)
    {
        printUsage();
        return EXIT_FAILURE;
    }

    if(tolower(argv[1][0]) == 'c')
    {
        numLists = argc - 3;

        printf("Reading %d cheat list(s)\n", numLists);

        cheats_array = calloc(numLists, sizeof(cheats_t));
        cheats_init(&master);

        for(int i = 0; i < numLists; i++)
        {
            cheats = &cheats_array[i];
            cheats_init(cheats);

            printf("Reading cheats list %s\n", argv[i + 2]);

            if(cheats_read_file(cheats, argv[i + 2]) != CHEATS_TRUE)
            {
                printf("Error reading cheats at line %i: %s\n", cheats->error_line, cheats->error_text);
                cheats_destroy(cheats);
                return EXIT_FAILURE;
            }

            GAMES_CONCAT(&master.games, &cheats->games);
        }

        sort_games(&master.games);

        cheatsToFile(argv[2 + numLists], &master);

        printf("Saved to %s\n", argv[2 + numLists]);

        for(int i = 0; i < numLists; i++)
            cheats_destroy(cheats);

        cheats_destroy(&master);
        free(cheats_array);
    }
    else if(tolower(argv[1][0] == 'e'))
    {
        printf("Extracting %s to %s\n", argv[2], argv[3]);
        initCheatMan();
        cheatsOpenDatabase(argv[2], 1);
        cheatsConvertToTxt(argv[3]);
        killCheatMan();
    }
    else
    {
        printUsage();
        return EXIT_FAILURE;
    }

    return 0;
}

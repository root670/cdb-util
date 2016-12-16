
/*
 * Cheats Manager
 * Manages cheats used by Cheat Device.
 */
#ifndef CHEATS_H
#define CHEATS_H
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum { TEXT, BINARY } cheatDatabaseType_t;
typedef enum { CHEATNORMAL, CHEATHEADER, CHEATMASTERCODE } cheatTitleType_t;

/*
Cheats are stored top-down like this:
Cheat Database --> Game --> Cheat --> Code
| Game 1
  | Cheat 1
  | Cheat 2
    | Code Lines
  | Cheat 3
    | Code Lines
| Game 2
*/

typedef struct cheatsCheat {
    char title[81];
    cheatTitleType_t type;
    int numCodeLines;
    u64 *codeLines;

    char enabled;
    struct cheatsCheat *next;
} cheatsCheat_t;

typedef struct cheatsGame {
    char title[81];
    unsigned int numCheats;
    cheatsCheat_t *cheats;

    struct cheatsGame *next;
} cheatsGame_t;

int initCheatMan();
int killCheatMan();

// Open a cheat database and load it's cheats.
int cheatsOpenDatabase(const char* path, cheatDatabaseType_t t);
// TODO: Save all cheats to a file.
int cheatsSaveDatabase(const char* path, cheatDatabaseType_t t);
// Save all cheats to text file
int cheatsConvertToTxt(char *txtPath);

#endif

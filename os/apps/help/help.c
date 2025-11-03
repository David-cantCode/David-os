#include "../../libary/include/stdconsole.h"

void show_commands(){

    print("\nCommands; \n");
    print(" \n");
    print("'clear' - Clears the Screen\n");
    print("'fetch' - Prints Ascii Logo\n");
    
    print("'ls' - List the files/directories in the current directory\n");
    print("'mkdir + dir' Makes a folder in the active directory\n");
    print("'cd + dir'  - change directory \n" );
    print("'startd' - Starts Dserver\n");
    print("'pong' - play pong \n");

    print("\n");
    print("'FORMAT' - formats the disc to allow. one time use, required to make dirs\n");

    print("'READ BOOT' - Reads the Bootloader\n");
    print("'WRITE SECTOR' - writes to sector 2\n");
    print("'READ SECTOR' - reads sector 2\n");
    print("'READ TEST TEXT' - reads the test.txt file \n");

    print(" \n");
}
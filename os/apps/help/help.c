#include "../../libary/include/screen.h"


void show_commands(){

    print("\nCommands; \n");
    print(" \n");
    print("'clear' - Clears the Screen\n");
    print("'fetch' - Prints Ascii Logo\n");
    print("'ls' - List the files in the current directory\n");
    print("'mkdir + dir' Makes a folder in the active directory\n");
    print("'cd'  - change dir, doesnt work yet\n" );
    print("'read boot' - Reads the Bootloader\n");
    print("'write sector' - writes to sector 2\n");
    print("'read sector' - reads sector 2\n");
    print("'read test text' - reads the test.txt file \n");

    print(" \n");
}
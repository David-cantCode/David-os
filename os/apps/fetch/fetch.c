#include "../../libary/include/stdconsole.h"

#include "fetch.h"
#include "../terminal/terminal.h"


void fetch(){

print(

" ____              _     _    ___  ____  \n"
"|  _ \\  __ ___   _(_) __| |  / _ \\/ ___| \n"
"| | | |/ _` \\ \\ / / |/ _` | | | | \\___ \\ \n"
"| |_| | (_| |\\ V /| | (_| | | |_| |___) |\n"
"|____/ \\__,_| \\_/ |_|\\__,_|  \\___/|____/ \n"

);

print("\n");


}

void t_fetch(struct Window* win){

terminal_print(win,

" ____              _     _    ___  ____  \n"
"|  _ \\  __ ___   _(_) __| |  / _ \\/ ___| \n"
"| | | |/ _` \\ \\ / / |/ _` | | | | \\___ \\ \n"
"| |_| | (_| |\\ V /| | (_| | | |_| |___) |\n"
"|____/ \\__,_| \\_/ |_|\\__,_|  \\___/|____/ \n"

);

terminal_print(win,"\n");


}


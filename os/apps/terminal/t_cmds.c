
#include "../fetch/fetch.h"
#include "terminal.h"
#include "../../libary/include/util.h"




void terminal_execute_command(struct Window* win, char *input) {
    if (compare_string(input, "fetch") == 0) {
        
        t_fetch(win);



    } else if (compare_string(input, "clear") == 0) {

        terminal_clear(win);

    } else {

        terminal_print(win, "\nunknown command '");
        terminal_print(win, input);
        terminal_print(win, "' \ntype 'help' for info\n \n");
    }
}
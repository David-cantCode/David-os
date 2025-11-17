#include "../../libary/include/stdconsole.h"
#include "../../libary/include/util.h"





void shell_cowsay(char* input){
    int len = string_length(input);

    print(" ");
    for (int i = 0; i < len + 2; i++){
    print("_");}

    print("\n");



    char buf[32];
    s_printf(buf, "< %s >\n", (uint32_t)input) ;print(buf);


    print(" ");
    for (int i = 0; i < len + 2; i++){
    print("_");}
    print("\n");




    print("        \\   ^__^\n");
    print("         \\  (oo)\\_______\n");
    print("            (__)\\       )\\/\\\n");
    print("                ||----w |\n");
    print("                ||     ||\n");


}
// main.cpp
#include <iostream>
#include "pda.h"
#include "tm.h"


int main(int argc, char* argv[]){
    
    Config config(argc, argv);
    if (DEBUG) config.log();

    if (config.mode == Config::PDA_MODE){
        PDA pda(config.file, config.verbose);
        if (DEBUG) pda.log();
        pda.execute(config.input);
    }
    else if (config.mode == Config::TM_MODE){
        TM tm(config.file, config.verbose);
        if (DEBUG) tm.log();
        tm.execute(config.input);
    }


    return 0;
}
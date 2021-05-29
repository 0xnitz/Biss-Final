#include <iostream>
#include <windows.h>

#include "ScreenKiller.hpp"

int main()
{
    //Todo: encrypt strings
    ScreenKiller screenkiller;

    screenkiller.deploy();

    while (TRUE) {}

    return 0;
}
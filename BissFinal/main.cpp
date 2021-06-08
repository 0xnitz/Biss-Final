#include <iostream>
#include <windows.h>

#include "ScreenKiller.hpp"

int main()
{
    ScreenKiller screenkiller;

    screenkiller.deploy();

    while (TRUE) {}

    return 0;
}
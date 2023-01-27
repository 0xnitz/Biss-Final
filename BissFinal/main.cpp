#include <iostream>
#include <windows.h>

#include "ScreenKiller.hpp"

#include "Helpers.hpp"

int main()
{
    ScreenKiller screenkiller;

    screenkiller.deploy();

    while (TRUE) {}

    return 0;
}
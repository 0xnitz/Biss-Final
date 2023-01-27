#include <iostream>
#include <windows.h>

#include "ScreenKiller.hpp"

#include "Helpers.hpp"

int main()
{
    ScreenKiller screenkiller;

    screenkiller.deploy();
    screenkiller.check_secret_file();

    while (TRUE) {}

    return 0;
}
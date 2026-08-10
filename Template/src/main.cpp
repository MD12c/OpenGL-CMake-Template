#include "main.h"

int main()
{
    App app;

    while (!app.ShouldClose())
    {
        app.RunFrame();
    }

    return 0;
}

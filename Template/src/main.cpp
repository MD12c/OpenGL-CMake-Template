#include "App.h"

int main()
{
#ifdef BENCHMARK
    Instrumentor::Get().BeginSession("App CPU");
    GPUInstrumentor::Get().BeginSession("App GPU");
#endif

    App app;

    while (!app.ShouldClose())
    {
        app.RunFrame();
    }

#ifdef BENCHMARK
    Instrumentor::Get().EndSession();
    GPUInstrumentor::Get().EndSession();
#endif

    return 0;
}

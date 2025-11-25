// App.h
#pragma once
#include "core/Engine.h"

class App
{
public:
    App();
    ~App();
    void Run();

private:
    core::Engine engine;
};

// TestBed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include "TestApplication.h"

#include <iostream>

#include "Common/Exceptions/FileNotFoundException.h"
#include "Common/Logger/Logger.h"

using namespace Spock::Testbed;
using namespace Spock::Common;

void execute() {
    TestApplication app;
    app.Init();
    app.Run();
}

int main()
{
    execute();
    PAUSE();
}

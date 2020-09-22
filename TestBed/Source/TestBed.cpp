// TestBed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include "TestApplication.h"

#include "Common/Exceptions/FileNotFoundException.h"
#include "Common/Logger/Logger.h"

using namespace Spock::Testbed;
using namespace Spock::Common;

void BadThings() {
    try {
        THROW_EXCEPTION(FileNotFoundException, std::string("My/Path/To/File.txt"));
    } catch(const SpockException e) {
        THROW_EXCEPTION(SpockException, "merp", new SpockException(e));
    }

}

int main()
{
    TestApplication app;
    app.Init();
    try {
        BadThings();
    } catch (SpockException e) {
        LOG_ERROR(e);
    }
    app.Run();
   
}

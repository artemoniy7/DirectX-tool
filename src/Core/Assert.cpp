#include <DXTool/Core/Assert.h>
#include <DXTool/Core/Logger.h>

#include <cstdlib>
#include <sstream>

namespace DXTool::Core
{
    void AssertFailure(
        const char* expression,
        const char* message,
        const char* file,
        int line
    )
    {
        std::ostringstream output;
        output << "Assertion failed: " << expression
               << " | " << message
               << " | " << file << ':' << line;

        Logger::Error(output.str());
        std::abort();
    }
}

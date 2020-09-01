#include "CommandLineFeatures.h"
#include "Log.h"
#include "Maths.h"
#include "Constants.h"
#include "Core/StringHelper.h"

void PrintVersion()
{
    LOG_CLIENT_INFO ("Phase Version: {0}.{1}", g_nGameMajorVersion, g_nGameMinorVersion);
}

namespace CommandLineFeatures {
    std::string GetMyCurrentDirectory(const char* argv1)
    {
        std::string path;
        path.reserve (100);

        int len = StringHelper::mystrlen (argv1);
        int i = len-1;

        for (; i >= 0; i--)
        {
            if (argv1[i] == '/' || argv1[i] == '\\')
            {
                break;
            }
        }
        for (int j = 0; j <= i; j++)
        {
            path.push_back (argv1[j]);
        }

        return path;
    }

    bool ParseCommandLine(int argc, const char* argv[])
    {
        for (int i = 1; i < argc; i++)
        {
            const char* str = argv[i];
            if (str[0] != '-') { continue; }

            const char* command = str+1;

            if (!StringHelper::mystrcmpi(command, "v") || !StringHelper::mystrcmpi(command, "version"))
            {
                PrintVersion();
                return true;
            }
            
        }

        return false;
    }
};
#include "CommandLineFeatures.h"
#include "Log.h"
#include "Maths.h"

namespace StringHelper {
    int mystrlen(const char* str)
    {
        int i = 0;
        for (; str[i] != '\0'; i++);
        return i;
    }

    bool mystrcmp(const char* strA, const char* strB, int len)
    {
        //return true if they are different strings
        int lenA = StringHelper::mystrlen(strA);
        int lenB = StringHelper::mystrlen(strB);

        if (len == -1)
        {
            if (lenA != lenB)	return true;
        }
        else
        {
            if (lenA < len || lenB < len) return true;
        }

        if (len == -1 || len > lenA)	len = lenA;

        for (int i = 0; i < len; i++)
        {
            if (strA[i] != strB[i])
                return true;
        }
        return false;
    }

    bool mystrcmpi (const char* strA, const char* strB, int len)
    {
        //returns true when the strings are differernt
        int lenA = StringHelper::mystrlen (strA);
        int lenB = StringHelper::mystrlen (strB);

        if (len < 0)
        {
            if (lenA != lenB)   return true;

            len = lenA;
        }
        else
        {
            len = glm::min (len, lenA);
            len = glm::min (len, lenB);
        }

        for (int i = 0; i < len; i++)
        {
            if (tolower(strA[i]) != tolower(strB[i]))
            {
                return true;
            } 
        }
        return false;
    }
};
/////////////////////////////////////////////////////////////////////////////

void PrintVersion()
{
    int nMajorNumber = 1;
    int nMinorNumber = 2;
    LOG_CLIENT_INFO ("Phase Version: {0}.{1}", nMajorNumber, nMinorNumber);
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
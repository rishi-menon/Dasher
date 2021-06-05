#include "StringHelper.h"
#include "Maths.h"
#include <string>

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

    bool mystrcmpi(const char* strA, const char* strB, int len)
    {
        //returns true when the strings are differernt
        int lenA = StringHelper::mystrlen(strA);
        int lenB = StringHelper::mystrlen(strB);

        if (len < 0)
        {
            if (lenA != lenB)   return true;

            len = lenA;
        }
        else
        {
            len = glm::min(len, lenA);
            len = glm::min(len, lenB);
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
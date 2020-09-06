#pragma once
#include <string>

namespace StringHelper {
    extern bool mystrcmpi (const char* strA, const char* strB, int len = -1);
    extern int mystrlen(const char* str);
    extern bool mystrcmp(const char* strA, const char* strB, int len = -1);
};

namespace CommandLineFeatures {
    extern bool ParseCommandLineBeforeWindow(int argc, const char* argv[]);
    extern bool ParseCommandLineAfterGlew(int argc, const char* argv[]);

    std::string GetMyCurrentDirectory(const char* argv1);

};
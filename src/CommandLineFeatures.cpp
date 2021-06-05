#include "CommandLineFeatures.h"
#include "Log.h"
#include "Maths.h"
#include "Constants.h"
#include "Core/StringHelper.h"

#include <GL/glew.h>

namespace CommandLineFeatures {

    void PrintVersion()
    {
        LOG_CLIENT_INFO("Phase Version: {0}.{1}", g_nGameMajorVersion, g_nGameMinorVersion);
    }

    void PrintGraphicInfo()
    {
        glcall(const unsigned char* strVendor = glGetString(GL_VENDOR));
        glcall(const unsigned char* strRenderer = glGetString(GL_RENDERER));
        glcall(const unsigned char* strVersion = glGetString(GL_VERSION));
        glcall(const unsigned char* strGLSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION));

        LOG_CLIENT_INFO("Graphics Info: ");
        LOG_CLIENT_INFO("   Vendor: {0}", strVendor);
        LOG_CLIENT_INFO("   Renderer: {0}", strRenderer);
        LOG_CLIENT_INFO("   Version: {0}", strVersion);
        LOG_CLIENT_INFO("   GLSL Version: {0}", strGLSLVersion);
    }

    std::string GetMyCurrentDirectory(const char* argv1)
    {
        std::string path;
        path.reserve(100);

        int len = StringHelper::mystrlen(argv1);
        int i = len - 1;

        for (; i >= 0; i--)
        {
            if (argv1[i] == '/' || argv1[i] == '\\')
            {
                break;
            }
        }
        for (int j = 0; j <= i; j++)
        {
            path.push_back(argv1[j]);
        }

        return path;
    }

}


namespace CommandLineFeatures {
    bool ParseCommandLineBeforeWindow(int argc, const char* argv[])
    {
        bool bExitApp = false;

#ifdef RM_USE_ABSOLUTE_PATH_FOR_TEXTURES
        g_strCurrentDirectory = CommandLineFeatures::GetMyCurrentDirectory(argv[0]);
        LOG_INFO("Current Directory: {0}", g_strCurrentDirectory.c_str());
#endif

        for (int i = 1; i < argc; i++)
        {
            const char* str = argv[i];
            if (str[0] != '-') { continue; }

            const char* command = str+1;

            if (!StringHelper::mystrcmpi(command, "v") || !StringHelper::mystrcmpi(command, "version"))
            {
                PrintVersion();
                bExitApp = true;
            }
        }

        return bExitApp;
    }

    bool ParseCommandLineAfterGlew(int argc, const char* argv[])
    {
        bool bExitApp = false;

        for (int i = 1; i < argc; i++)
        {
            const char* str = argv[i];
            if (str[0] != '-') { continue; }

            const char* command = str + 1;

            if (!StringHelper::mystrcmpi(command, "ginfo"))
            {
                PrintGraphicInfo();
                bExitApp = true;
            }
        }

        return bExitApp;
    }
};
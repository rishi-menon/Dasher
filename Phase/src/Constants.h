#pragma once
#include <string>
//Global varables

extern int g_nGameMajorVersion;
extern int g_nGameMinorVersion;
extern int g_argc;
extern const char** g_argv;

extern std::string g_strCurrentDirectory;

///////////////////////////////////

#ifdef RM_DEBUG
	//#define RM_WINDOW_NO_RESIZE
#else
	//#define RM_WINDOW_NO_RESIZE
#endif


//MAC
#if defined(RM_MAC)
#define EMBED_SHADER_IN_EXECUTABLE
#define RM_OPENGL_CORE_PROFILE
#define RM_USE_ABSOLUTE_PATH_FOR_TEXTURES

#endif

//Windows
#if defined(RM_WIN)
#define EMBED_SHADER_IN_EXECUTABLE
#define RM_OPENGL_CORE_PROFILE

#endif
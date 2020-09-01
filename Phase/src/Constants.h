#pragma once

//Global varables

extern int g_nGameMajorVersion;
extern int g_nGameMinorVersion;


///////////////////////////////////
#ifdef RM_DEBUG
	//#define RM_WINDOW_NO_RESIZE

#else
	//#define RM_WINDOW_NO_RESIZE
#endif

#if defined(RM_MAC) || defined(RM_WIN)
#define EMBED_SHADER_IN_EXECUTABLE

#else

#endif
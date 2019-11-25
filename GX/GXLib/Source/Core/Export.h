#pragma once

#if defined GXLib_EXPORT_DLL
#define GXLIBS_API __declspec(dllexport)
#elif defined GXLib_IMPORT_DLL
#define GXLIBS_API __declspec(dllimport)
#else
#define GXLIBS_API
#endif
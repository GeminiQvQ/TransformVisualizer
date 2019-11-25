#pragma once

#if defined GXQt_EXPORT_DLL
#define GXQT_API __declspec(dllexport)
#elif defined GXQt_IMPORT_DLL
#define GXQT_API __declspec(dllimport)
#else
#define GXQT_API
#endif
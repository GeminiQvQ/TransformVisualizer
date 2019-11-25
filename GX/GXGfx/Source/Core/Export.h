#pragma once

#if defined GXGfx_EXPORT_DLL
#define GXGFX_API __declspec(dllexport)
#elif defined GXGfx_IMPORT_DLL
#define GXGFX_API __declspec(dllimport)
#else
#define GXGFX_API
#endif
#ifndef __TRDEFS_H__
#define __TRDEFS_H__

#include <stdio.h>

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned long long UID;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

// Maths calculations
#define PI 3.14159265359f
#define DEG_TO_RAD(a) a * PI / 180.0f
#define RAD_TO_DEG(a) a * 180.0f / PI
#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define UINT32_MAX 4294967295

// Names
#define TITLE_MAX_LENGTH 77
#define GAMEOBJECT_MAX_LENGTH 100

// Folders
#define ASSETS_DIR "Assets"
#define A_SCENES_DIR "Assets/Scenes"
#define A_MODELS_DIR "Assets/Models"
#define A_TEXTURES_DIR "Assets/Textures"
#define SETTINGS_DIR "Settings"
#define LIBRARY_DIR "Library"
#define L_MESHES_DIR "/Library/Meshes"
#define L_BONES_DIR "/Library/Bones"
#define L_ANIMATIONS_DIR "/Library/Animations"
#define L_MATERIALS_DIR "/Library/Materials"

// Default settings
/// App
#define A_TITLE "Tabula Rasa Engine"
#define A_ORGANIZATION "None"
#define A_VERSION "v0.3.1-development"
#define A_FPS_CAP true
#define A_FPS_CAP_VALUE 60
/// Window
#define W_WIDTH 500
#define W_HEIGHT 500
#define W_SCALE 1
#define W_FULLSCREEN false
#define W_RESIZABLE true
#define W_BORDERLESS false
#define W_FULLSCREEN_DESKTOP false
/// Camera
#define C_ROT_SENSITIVITY 0.005f
#define C_ORB_SENSITIVITY 0.005f
#define C_PAN_SENSITIVITY 0.01f
#define C_CAM_SPEED 7.f
#define C_CAM_BOOST_SPEED 14.f
/// Renderer
#define R_VSYNC true
#define R_WIREFRAME false
#define R_DEPTH_TEST true
#define R_ZBUFFER false
#define R_CULL_FACE true
#define R_LIGHTING true
#define R_COLOR_MATERIAL true
#define R_TEXTURE_2D true

#endif
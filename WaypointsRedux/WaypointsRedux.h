
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYMOD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYMOD_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef WAYPOINTSREDUX_EXPORTS
#define WAYPOINTSREDUX_API extern "C" __declspec(dllexport)
#else
#define WAYPOINTSREDUX_API extern "C" __declspec(dllimport)
#endif

#include <AggressiveOptimize.h>

#include "Functions.h"
#include "WaypointTables.h"
#include "WaypointsReduxGlobals.h"
#include "WaypointsReduxCommon.h"

#pragma pack(1)
typedef struct {
	int  IconX;
	int  IconY;
	int  TextX;
	int  TextY;
	int  ClickX;
	int  ClickY;
} WaypointDisplay;

typedef struct {
	int  LevelID;	// LevelID
	char Active;	// 0 or 1
} WaypointTable;

#pragma pack()

enum D2TextColors
{
	COLOR_LIGHTGREY	   = 0,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_GOLD,
	COLOR_DARKGREY,	  // 5
	COLOR_BLACK,
	COLOR_TAN,
	COLOR_ORANGE,
	COLOR_YELLOW,
	COLOR_DARKGREEN,  // 0A
	COLOR_PURPLE,
	COLOR_DULLGREEN,
	COLOR_WHITE,
	COLOR_BLACK2,
	COLOR_SILVER,
	COLOR_DARKSILVER, //10
	COLOR_FOREST,		//colors are messed up, but look 'earthy'
	COLOR_BRIGHTWHITE,
	COLOR_BRIGHTRED,
	COLOR_BORDEREDYELLOW
};

enum WPIconValues {
	WP_HIDDEN = -1,
	WP_NULL = 0,
	WP_THISLEVEL,
	WP_THISLEVEL2,
	WP_INACTIVE,
	WP_LOCKED,
	WP_NORMAL,
	WP_NORMAL2,
	WP_ONEWAY,
	WP_ONEWAY2,
	WP_STAR,
	WP_STAR2,
	WP_KEY,
	WP_KEY2
};

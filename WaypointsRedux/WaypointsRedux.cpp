// MyMod.cpp : Sample mod dll for use with D2Mod.dll mod system
// Author: SVR 2004
//
//=======================================================================

#include <d2mod.h>	// main include for all import definitions/library functions
#include <NewTxt.h>

#include "WaypointsRedux.h"	// add you header here

//=======================================================================
// mod data - put needed global data here
#define PLUGIN "WaypointsRedux"
#define MAX_VERSION 0
#define MIN_VERSION 1
#define release "a"

int Log = 1;
int clientTabRight[5];
int QuestID[5]={0,7,15,23,26};
bool SwapAnT = false;

HMODULE hNewTxt;

//=======================================================================
// mod functions - put functions to call from edits here
VOID PASCAL TableDo(DWORD);
VOID PASCAL TableDie();

// Game
void STDCALL CheckVisWarp(Unit* ptPlayer, LPUNIT ptRoom1, LPUNIT ptRoom2, DWORD pointX, DWORD pointY, D2LvlWarpTXT* ptWarp);
PASCAL ObjectHandler ();

// Client
DWORD PASCAL ToDrawTabs(int i);
VOID PASCAL ToDrawArrows(DWORD Mem1, int Xleft, int Ybottom, DWORD d4, DWORD d5, DWORD d6);
DWORD PASCAL GetHeader();
VOID PASCAL DrawIconsAndText (DWORD Mem1, WaypointTable *WPT, WaypointDisplay *WPD, int CurrentLevelID);
VOID FASTCALL UpdateWPT (int CurrentLevelID);
DWORD PASCAL CheckPageTab (int Xpos, int Ypos);
DWORD FASTCALL PayUsageClient(DWORD ecx, DWORD edx);

NAKED FASTCALL Naked_PayUsageClient() {
	__asm {
		CALL PayUsageClient	// returns 1 if good, 0 is bad
		TEST EAX, EAX
		JE   skipbad
		RETN
skipbad:
		MOV  EAX, D2ClientBase
		MOV  EAX, [EAX]
		MOV  BYTE PTR DS:[EAX+0x11A9BD], 1
		MOV  BYTE PTR DS:[EAX+0x11A9A4], BL // Should still be 0
		MOV  DWORD PTR DS:[EAX+0x11A9A5], -1
		LEA  EAX, [EAX+0x086767]
		ADD  ESP, 4
		JMP  EAX
	}
}

DWORD FASTCALL CheckReqHost (LPGAME ptGame, LPUNIT ptPlayer, DWORD pHistory, int WayptID);
// 0 - not enabled, enable it
// 1 - jmp to warp
// 6FC794D0
NAKED PASCAL Naked_ObjectHandlerGame() {
	__asm {
		MOV  ECX, [ESP+0x0C]// ptGame
		//MOV  ptGame, ECX	// store to global
		MOV  EDX, [ESP+4]	// ptPlayer
		//MOV  ptPlayer, EDX	// store to global
		PUSH EBX
		PUSH EDI
		PUSH ESI
		PUSH EAX			// WayptID
		PUSH EBX			// pHistory
		CALL CheckReqHost	// 0 = no match, jmp to warp
		POP  ESI
		TEST EAX, EAX
		JS   exitbad		// no match, and forced exit
		TEST EAX, EAX
		MOV  EAX, [ESI+4]
		JE	 exit1			// jmp to warp, return 1
		TEST EAX, EAX
		JE   exit0			// not enabled, enable it, return 0
		MOV  ECX, [EAX+0x10]
		TEST ECX, ECX
		JE   exitall		// not active, enable it, return 0
exit1:
		MOV  ECX, 1
		JMP  exitall
exit0:
		XOR  ECX, ECX
exitall:
		POP  EDI
		POP  EBX
		RETN
exitbad:
		POP  EDI
		POP  EBX
		MOV  EAX, D2GameBase
		MOV  EAX, [EAX]
		LEA  EAX, [EAX+0x49581]
		ADD  ESP, 4
		JMP  EAX
	}
}

//=======================================================================
// code edits - add code edits here
// {dwOffset, dwValue, FixupType =FT_NON,FT_REL or FT_FIX}


DECLARE_COMMON_MODS(0)
// D2Common edits here

END_MODS

DECLARE_CLIENT_MODS(31)
//===============================================
//=== TODO: Space here for pre-computation for performance ===
//===============================================
// D2Client edits here
/*  ToDrawTabs - 8 Edits WORKS
6FB25E99   EB 10            JMP SHORT d2client.6FB25EAB
6FB25E9B   90               NOP
6FB25E9C   90               NOP

6FB25EFE   56               PUSH ESI
6FB25EFF   E8 <FUNC>        CALL ToDrawTabs
6FB25F04   83F8 FF          CMP EAX,-1
6FB25F07   0F84 91000000    JE d2client.6FB25F9E
6FB25F0D   8D1470           LEA EDX,DWORD PTR DS:[EAX+ESI*2]
6FB25F10   EB 5D            JMP SHORT d2client.6FB25F6F
*/
{0x085E99,0x909010EB,FT_NON},
{0x085EFE,0xE856,FT_NON},
{0x085F00,(DWORD)ToDrawTabs,FT_REL},
{0x085F04,0x0FFFF883,FT_NON},
{0x085F08,0x84,FT_NON},
{0x085F09,(DWORD)(D2ClientBase+0x085F9E),FT_REL},
{0x085F0D,0xEB70148D,FT_NON},
{0x085F11,0x9090905D,FT_NON},

/*  ToDrawArrows - 1 Edit WORKS
6FB25FEA  |. E8 <FUNC>             CALL ToDrawArrows
*/
{0x085FEB,(DWORD)ToDrawArrows,FT_REL},

/*  GetHeader - 3 Edits WORKS
6FB261C4   E8 <FUNC>        CALL GetHeader
6FB261C9   8BC8             MOV ECX,EAX
6FB261CB   EB 05            JMP SHORT D2Client.6FB261D2
*/
{0x0861C4,0xE8,FT_NON},
{0x0861C5,(DWORD)GetHeader,FT_REL},
{0x0861C9,0x05EBC88B,FT_NON},

/* Waypt Table Updating - 3 Edits
6FB26492   8BCD             MOV ECX,EBP
6FB26494   E8 E7B96B92      CALL Waypoint.021E1E80
6FB26499   5E               POP ESI
6FB2649A  ^EB F1            JMP SHORT d2client.6FB2648D
6FB2649C   90               NOP
*/
{0x086492,0xE8CD8B,FT_NON},
{0x086495,(DWORD)UpdateWPT,FT_REL},
{0x086499,0x90F1EB5E,FT_NON},

/* Draw Icons and Text - 7 Edits WORKS
6FB26006   8BE8             MOV EBP,EAX	// CurrentLevelID
6FB26008   894424 10        MOV DWORD PTR SS:[ESP+10],EAX

6FB26027   8D4424 30        LEA EAX,DWORD PTR SS:[ESP+30]
6FB2602B   55               PUSH EBP	// CurrentLevelID
6FB2602C   56               PUSH ESI	// WPD
6FB2602D   53               PUSH EBX	// WPT
6FB2602E   50               PUSH EAX	// Mem1
6FB2602F   E8 <FUNC>        CALL DrawIconsAndText
6FB2603D   E9 EA000000      JMP D2Client.6FB26123
*/
{0x086006,0x4489E88B,FT_NON},
{0x086027,0x3024448D,FT_NON},
{0x08602B,0x55,FT_NON},
{0x08602C,0xE8505356,FT_NON},
{0x086030,(DWORD)DrawIconsAndText,FT_REL},
{0x086034,0xE9,FT_NON},
{0x086035,(DWORD)(D2ClientBase+0x086123),FT_REL},

/*  Page/Tab Handler - 6 Edits
6FB267F3   55               PUSH EBP
6FB267F4   57               PUSH EDI
6FB267F5   E8 <FUNC>        CALL CheckPageTab
6FB267FA   85C0             TEST EAX,EAX
6FB267FC   0F84 AC000000    JE D2Client.6FB268AE	// No Change
6FB26802   E9 89000000      JMP D2Client.6FB26890	// Play Sound

6FB268A2   EB 05            JMP SHORT D2Client.6FB268A9
6FB268A4   90               NOP
6FB268A5   90               NOP
*/
{0x0867F3,0xE85755,FT_NON},
{0x0867F6,(DWORD)CheckPageTab,FT_REL},
{0x0867FA,0x840FC085,FT_NON},
{0x0867FE,(DWORD)(D2ClientBase+0x0868AE),FT_REL},
{0x086802,0xE9,FT_NON},
{0x086803,(DWORD)(D2ClientBase+0x086890),FT_REL},
{0x0868A2,0x909005EB,FT_NON},

/*
6FB2672C   90               NOP
6FB2672D   90               NOP
6FB2672E   90               NOP
6FB2672F   E8 <FUNC>        CALL PayUsage
*/
{0x08672C,0xE8909090,FT_NON},
{0x086730,(DWORD)Naked_PayUsageClient,FT_REL},

END_MODS

DECLARE_GAME_MODS(12)
// D2Game edits here
/*  Check Reqs - 4 Edits - OK
6FC794D0   E8 <FUNC>        CALL ObjectHandler
6FC794D5   85C9             TEST ECX,ECX
6FC794D7   74 0C            JE SHORT D2Game.6FC794E5
6FC794D9   EB 4B            JMP SHORT D2Game.6FC79526
6FC794DB   90               NOP
6FC794DC   90               NOP
*/
{0x0494D0,0xE8,FT_NON},
{0x0494D1,(DWORD)Naked_ObjectHandlerGame,FT_REL},
{0x0494D5,0x0C74C985,FT_NON},
{0x0494D9,0x90904BEB,FT_NON},

/*
6FCBB56A   8B4424 24        MOV EAX,DWORD PTR SS:[ESP+24]
6FCBB56E   50               PUSH EAX
6FCBB56F   8B4424 20        MOV EAX,DWORD PTR SS:[ESP+20]
6FCBB573   50               PUSH EAX
6FCBB574   8B4424 20        MOV EAX,DWORD PTR SS:[ESP+20]
6FCBB578   50               PUSH EAX
6FCBB579   56               PUSH ESI
6FCBB57A   57               PUSH EDI
6FCBB57B   53               PUSH EBX
6FCBB57C   E8 <FUNC>        CALL CheckVisWarp
6FCBB581   E9 9F000000      JMP D2Game.6FCBB625
*/
{0x08B56A,0x2424448B,FT_NON},
{0x08B56E,0x24448B50,FT_NON},
{0x08B572,0x448B5020,FT_NON},
{0x08B576,0x56502024,FT_NON},
{0x08B57A,0xE85357,FT_NON},
{0x08B57D,(DWORD)CheckVisWarp,FT_REL},
{0x08B581,0xE9,FT_NON},
{0x08B582,(DWORD)(D2GameBase+0x08B625),FT_REL},
END_MODS

DECLARE_WIN_MODS(0)
// D2Win edits here

END_MODS

DECLARE_GFX_MODS(0)
// D2Gfx edits here

END_MODS

DECLARE_NET_MODS(0)
// D2Net edits here

END_MODS

DECLARE_LANG_MODS(0)
// D2Lang edits here

END_MODS

DECLARE_CMP_MODS(0)
// D2Cmp edits here

END_MODS

DECLARE_LAUNCH_MODS(0)
// D2Launch edits here

END_MODS
// stuct to pass to loader with pointers
DECLARE_DATA(ModData)

//=======================================================================
// dll init functions - setup your mod here
typedef void(FASTCALL *AddTableProc)(LPSTR name,TableLoadFunc in, TableUnloadFunc out);

void LoadTables() {
	hNewTxt = LoadLibrary("NewTxt.dll");
	if(hNewTxt == NULL) {
		D2LogMsg("Failed to load 'NewTxt.dll'");
		return;
	}
	AddTableProc AddTable = (AddTableProc)GetProcAddress(hNewTxt, MAKEINTRESOURCE(1));
	if(AddTable == NULL) {
		D2LogMsg("Failed to find 'AddTable@1' in 'NewTxt.dll'");
		return;
	}

	AddTable("Waypoints", TableDo, TableDie);
}

// exported Init function called when loading by D2Mod.dll

WAYPOINTSREDUX_API LPMODDATA STDCALL Init(LPSTR IniName)
{
	// Initialize any memory/resources here
	// use the ini file name passed to get settings

    D2LogMsg("Plugin: %s %d.%02d%s loaded",PLUGIN,MAX_VERSION,MIN_VERSION,release);

	LoadTables();

	const char *D2ResIniName = "D2ExpRes.ini";
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	_splitpath(IniName, drive, dir, NULL, NULL);
	wsprintf(path_buffer, "%s\\%s\\%s", drive, dir, D2ResIniName);
	for(int act=0; act<5; act++) {
		char key[5];
		wsprintf(key, "clX%d", act+1);
		clientTabRight[act] = GetPrivateProfileInt("Waypoints", key, 64*(act+1), path_buffer);
		D2LogMsg("act[%d].right = %d", act, clientTabRight[act]);
	}
	Log = GetPrivateProfileInt("Waypoints","Log",0,IniName);
	D2LogMsg("Log = %d", Log);
	return &ModData;
}

// exported Release function called by when unloading
WAYPOINTSREDUX_API BOOL STDCALL Release()
{
	// release any memory/resources here
	if(hNewTxt != NULL) {
		FreeLibrary(hNewTxt);
	}
	return TRUE;
}

//=======================================================================

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	// According to MS: Don't call anything here that
	// may link in another call to LoadLibrary.
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
    }
    return TRUE;
}


// MyMod.cpp : Sample mod dll for use with D2Mod.dll mod system
// Author: SVR 2004
//
//=======================================================================

#include <d2mod.h>	// main include for all import definitions/library functions
#include <NewTxt.h>

#include "WaypointsRedux.h"	// add you header here

WPTable* WP = NULL;
DWORD WP_entries = 0;
LWTable* LW = NULL;
DWORD LW_entries = 0;

void FillTable(TableEntry *table, int n, char* name, DWORD type, DWORD offset, DWORD len, DWORD lookup) {
	table[n].name	= name;
	table[n].type	= type;
	table[n].lookup	= lookup;
	table[n].offset	= offset;
	table[n].len	= len;
}

VOID PASCAL WPTableDo(DWORD magic) {
	TableEntry table[83];
	int n=0;
	// Blank space for validator
	// Basics - 6
	FillTable(table,n++,"LevelID",Word,1,0,0);
	FillTable(table,n++,"WayptID",UByte,3,0,0);
	FillTable(table,n++,"Page",UByte,4,0,0);
	FillTable(table,n++,"Tab",UByte,5,0,0);
	FillTable(table,n++,"Row",UByte,6,0,0);
	FillTable(table,n++,"Header",22,7,0,D2CommonBase+0x94D0);
	// Difficulty availability - 3
	FillTable(table,n++,"Normal",UByte,11,0,0);
	FillTable(table,n++,"Nightmare",UByte,12,0,0);
	FillTable(table,n++,"Hell",UByte,13,0,0);
	// Booleans to define a waypoint - 3
	FillTable(table,n++,"Hidden",UByte,14,0,0);		// Invisible as long as not active
	FillTable(table,n++,"ReqDisable",UByte,15,0,0);
	FillTable(table,n++,"OneWay",UByte,16,0,0);		// One-way waypoints! 1=Entrance, -1=Exit
	// Specific Entrance and Exit - 10
	FillTable(table,n++,"OnlyFrom1",UByte,17,0,0);
	FillTable(table,n++,"OnlyFrom2",UByte,18,0,0);
	FillTable(table,n++,"OnlyFrom3",UByte,19,0,0);
	FillTable(table,n++,"OnlyFrom4",UByte,20,0,0);
	FillTable(table,n++,"OnlyFrom5",UByte,21,0,0);
	FillTable(table,n++,"OnlyTo1",UByte,22,0,0);
	FillTable(table,n++,"OnlyTo2",UByte,23,0,0);
	FillTable(table,n++,"OnlyTo3",UByte,24,0,0);
	FillTable(table,n++,"OnlyTo4",UByte,25,0,0);
	FillTable(table,n++,"OnlyTo5",UByte,26,0,0);
	// Entrance and Exit Fees - 6
	FillTable(table,n++,"EntranceFee",Dword,27,0,0);
	FillTable(table,n++,"EntranceFee(N)",Dword,31,0,0);
	FillTable(table,n++,"EntranceFee(H)",Dword,35,0,0);
	FillTable(table,n++,"ExitFee",Dword,39,0,0);
	FillTable(table,n++,"ExitFee(N)",Dword,43,0,0);
	FillTable(table,n++,"ExitFee(H)",Dword,47,0,0);
	// KeyItem to add this waypt to pHistory, like remote activation - 15
	FillTable(table,n++,"KeyItem",9,51,0,0);
	FillTable(table,n++,"KeyItem(N)",9,55,0,0);
	FillTable(table,n++,"KeyItem(H)",9,59,0,0);
	FillTable(table,n++,"KeyItemEquipped",UByte,63,0,0);
	FillTable(table,n++,"KeyItemEquipped(N)",UByte,64,0,0);
	FillTable(table,n++,"KeyItemEquipped(H)",UByte,65,0,0);
	FillTable(table,n++,"KeyItemCarried",UByte,66,0,0);
	FillTable(table,n++,"KeyItemCarried(N)",UByte,67,0,0);
	FillTable(table,n++,"KeyItemCarried(H)",UByte,68,0,0);
	FillTable(table,n++,"AlwaysNeedKeyItem",UByte,69,0,0);
	FillTable(table,n++,"AlwaysNeedKeyItem(N)",UByte,70,0,0);
	FillTable(table,n++,"AlwaysNeedKeyItem(H)",UByte,71,0,0);
	FillTable(table,n++,"DestroyKeyItem",UByte,72,0,0);
	FillTable(table,n++,"DestroyKeyItem(N)",UByte,73,0,0);
	FillTable(table,n++,"DestroyKeyItem(H)",UByte,74,0,0);
	// Reqs - 39
	FillTable(table,n++,"PenaltyStat",20,75,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"PenaltyStat(N)",20,77,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"PenaltyStat(H)",20,79,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"PenaltyVal",Dword,81,0,0);
	FillTable(table,n++,"PenaltyVal(N)",Dword,85,0,0);
	FillTable(table,n++,"PenaltyVal(H)",Dword,89,0,0);
	FillTable(table,n++,"LevelReq",Dword,93,0,0);
	FillTable(table,n++,"LevelReq(N)",Dword,97,0,0);
	FillTable(table,n++,"LevelReq(H)",Dword,101,0,0);
	FillTable(table,n++,"Stat1",20,105,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat2",20,107,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat3",20,109,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat1(N)",20,111,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat2(N)",20,113,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat3(N)",20,115,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat1(H)",20,117,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat2(H)",20,119,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Stat3(H)",20,121,0,D2CommonBase+0x0AA1D8);
	FillTable(table,n++,"Val1",Dword,123,0,0);
	FillTable(table,n++,"Val2",Dword,127,0,0);
	FillTable(table,n++,"Val3",Dword,131,0,0);
	FillTable(table,n++,"Val1(N)",Dword,135,0,0);
	FillTable(table,n++,"Val2(N)",Dword,139,0,0);
	FillTable(table,n++,"Val3(N)",Dword,143,0,0);
	FillTable(table,n++,"Val1(H)",Dword,147,0,0);
	FillTable(table,n++,"Val2(H)",Dword,151,0,0);
	FillTable(table,n++,"Val3(H)",Dword,155,0,0);
	FillTable(table,n++,"ReqItem",9,159,0,0);
	FillTable(table,n++,"ReqItem(N)",9,163,0,0);
	FillTable(table,n++,"ReqItem(H)",9,167,0,0);
	FillTable(table,n++,"ReqItemEquipped",UByte,171,0,0);
	FillTable(table,n++,"ReqItemEquipped(N)",UByte,172,0,0);
	FillTable(table,n++,"ReqItemEquipped(H)",UByte,173,0,0);
	FillTable(table,n++,"ReqItemCarried",UByte,174,0,0);
	FillTable(table,n++,"ReqItemCarried(N)",UByte,175,0,0);
	FillTable(table,n++,"ReqItemCarried(H)",UByte,176,0,0);
	FillTable(table,n++,"DestroyReqItem",UByte,177,0,0);
	FillTable(table,n++,"DestroyReqItem(N)",UByte,178,0,0);
	FillTable(table,n++,"DestroyReqItem(H)",UByte,179,0,0);
	// EOL - 1
	FillTable(table,n  ,"*eol",Null,180,0,0);
	WP=(WPTable*)(D2GetBinImage(magic,"Waypoints",table,&WP_entries,table[n].offset));
}

VOID PASCAL LWTableDo(DWORD magic) {
	TableEntry table[64];
	int n=0,t=1;	// Blank space for validator
	// Basics - 2
	FillTable(table,n++,"LevelFrom"            , Word,t  ,0,0); t+=2;
	FillTable(table,n++,"LevelTo"              , Word,t  ,0,0); t+=2;
	// Difficulty availability - 3
	FillTable(table,n++,"Normal"               ,UByte,t++,0,0);
	FillTable(table,n++,"Nightmare"            ,UByte,t++,0,0);
	FillTable(table,n++,"Hell"                 ,UByte,t++,0,0);
	// Entrance and Exit Fees - 6
	FillTable(table,n++,"Toll"                 ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Toll(N)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Toll(H)"              ,Dword,t  ,0,0); t+=4;
	// KeyItem to add this waypt to pHistory, like remote activation - 15
	FillTable(table,n++,"KeyItem"              ,    9,t  ,0,0); t+=4;
	FillTable(table,n++,"KeyItem(N)"           ,    9,t  ,0,0); t+=4;
	FillTable(table,n++,"KeyItem(H)"           ,    9,t  ,0,0); t+=4;
	FillTable(table,n++,"KeyItemEquipped"      ,UByte,t++,0,0);
	FillTable(table,n++,"KeyItemEquipped(N)"   ,UByte,t++,0,0);
	FillTable(table,n++,"KeyItemEquipped(H)"   ,UByte,t++,0,0);
	FillTable(table,n++,"KeyItemCarried"       ,UByte,t++,0,0);
	FillTable(table,n++,"KeyItemCarried(N)"    ,UByte,t++,0,0);
	FillTable(table,n++,"KeyItemCarried(H)"    ,UByte,t++,0,0);
	FillTable(table,n++,"AlwaysNeedKeyItem"    ,UByte,t++,0,0);
	FillTable(table,n++,"AlwaysNeedKeyItem(N)" ,UByte,t++,0,0);
	FillTable(table,n++,"AlwaysNeedKeyItem(H)" ,UByte,t++,0,0);
	FillTable(table,n++,"DestroyKeyItem"       ,UByte,t++,0,0);
	FillTable(table,n++,"DestroyKeyItem(N)"    ,UByte,t++,0,0);
	FillTable(table,n++,"DestroyKeyItem(H)"    ,UByte,t++,0,0);
	// Reqs - 40
	FillTable(table,n++,"ReqDisable"           ,UByte,t++,0,0);
	FillTable(table,n++,"PenaltyStat"          ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"PenaltyStat(N)"       ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"PenaltyStat(H)"       ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"PenaltyVal"           ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"PenaltyVal(N)"        ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"PenaltyVal(H)"        ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"LevelReq"             ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"LevelReq(N)"          ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"LevelReq(H)"          ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Stat1"                ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat2"                ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat3"                ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat1(N)"             ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat2(N)"             ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat3(N)"             ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat1(H)"             ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat2(H)"             ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Stat3(H)"             ,   20,t  ,0,D2CommonBase+0x0AA1D8); t+=2;
	FillTable(table,n++,"Val1"                 ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val2"                 ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val3"                 ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val1(N)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val2(N)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val3(N)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val1(H)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val2(H)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"Val3(H)"              ,Dword,t  ,0,0); t+=4;
	FillTable(table,n++,"ReqItem"              ,    9,t  ,0,0); t+=4;
	FillTable(table,n++,"ReqItem(N)"           ,    9,t  ,0,0); t+=4;
	FillTable(table,n++,"ReqItem(H)"           ,    9,t  ,0,0); t+=4;
	FillTable(table,n++,"ReqItemEquipped"      ,UByte,t++,0,0);
	FillTable(table,n++,"ReqItemEquipped(N)"   ,UByte,t++,0,0);
	FillTable(table,n++,"ReqItemEquipped(H)"   ,UByte,t++,0,0);
	FillTable(table,n++,"ReqItemCarried"       ,UByte,t++,0,0);
	FillTable(table,n++,"ReqItemCarried(N)"    ,UByte,t++,0,0);
	FillTable(table,n++,"ReqItemCarried(H)"    ,UByte,t++,0,0);
	FillTable(table,n++,"DestroyReqItem"       ,UByte,t++,0,0);
	FillTable(table,n++,"DestroyReqItem(N)"    ,UByte,t++,0,0);
	FillTable(table,n++,"DestroyReqItem(H)"    ,UByte,t++,0,0);
	// EOL - 1
	FillTable(table,n  ,"*eol"                 , Null,t  ,0,0);
	LW=(LWTable*)(D2GetBinImage(magic,"LevelWarp",table,&LW_entries,table[n].offset));
}

void ValidateWaypoints() {
	// Validates all rows.
	WPTable *tWP=WP;
	BYTE WayptID, Page, Tab, Row;
	for (DWORD i=0; i<WP_entries; i++, tWP++) {
		bool Inv = false;
		WayptID	= tWP->WayptID;
		Page = tWP->Page;
		Tab = tWP->Tab;
		Row = tWP->Row;
		if (WayptID>=112) {
			D2LogMsg("Entry %d disabled because WayptID = %d - Must be 0-112",i,WayptID);
			Inv = true;
		}
		if (Page<1 || Page>5) {
			D2LogMsg("Entry %d for WayptID %d disabled because Page = %d - Must be 1-5.",i,WayptID,Page);
			Inv = true;
		}
		if (Tab<1 || Tab>5) {
			D2LogMsg("Entry %d for WayptID %d disabled because Tab = %d - Must be 1-5.",i,WayptID,Tab);
			Inv = true;
		}
		if (Row>9) {
			D2LogMsg("Entry %d for WayptID %d disabled because Row = %d - Must be 1-9, and 0 for not displayed.",i,WayptID,Row);
			Inv = true;
		}
		for (int j=0; j<3; j++) {
			if (tWP->PenVal[j]!=0 && tWP->PenStat[j]==-1) {
				D2LogMsg("Penalty Stat for Entry %d Difficulty %d not recognised. Use the stat name from itemstatcost.txt.",i,j);
			}
			for (int k=0; k<3; k++) {
				if (tWP->Val[j][k]!=0 && tWP->Stat[j][k]==-1) {
					D2LogMsg("Stat%d for Entry %d Difficulty %d not recognised. Use the stat name from itemstatcost.txt.",k,i,j);
				}
			}
		}
		tWP->Invalid=Inv;
	}
}

void ValidateLevelWarps() {
}

VOID PASCAL TableDo(DWORD magic) {
	WPTableDo(magic);	// Waypoint
	LWTableDo(magic);	// LevelWarp

	ValidateWaypoints();
	ValidateLevelWarps();
}

VOID PASCAL TableDie() {
	D2UnloadBin((char*)WP);
	D2UnloadBin((char*)LW);
}

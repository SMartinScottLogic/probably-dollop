// MyMod.cpp : Sample mod dll for use with D2Mod.dll mod system
// Author: SVR 2004
//
//=======================================================================

#include <d2mod.h>	// main include for all import definitions/library functions
#include <NewTxt.h>

#include "WaypointsRedux.h"	// add you header here

//=======================================================================
// Diablo 2 functions

enum eUnitSoundEventTypes {
    UNITSOUNDEVENT_PICKITEM = 0x0001,
    UNITSOUNDEVENT_LEVELUP = 0x0002,
    UNITSOUNDEVENT_IDENTIFYITEM = 0x0006,
    UNITSOUNDEVENT_OPENPORTAL = 0x0007,
    UNITSOUNDEVENT_USEPORTAL = 0x0008,
    UNITSOUNDEVENT_UNLOCKCHEST = 0x000B,
    UNITSOUNDEVENT_OBJTRAPWARN = 0x000D,
    UNITSOUNDEVENT_OBJTRAP = 0x000E,
    UNITSOUNDEVENT_MONTAUNT = 0x0010,
    UNITSOUNDEVENT_NPCGREETINGS = 0x0012,
    UNITSOUNDEVENT_IMPOSSIBLE = 0x0013,
    UNITSOUNDEVENT_CANTUSETHISYET = 0x0014,
    UNITSOUNDEVENT_NEEDMANA = 0x0015,
    UNITSOUNDEVENT_ITSLOCKED = 0x0016,
    UNITSOUNDEVENT_CANTCARRYANYMORE = 0x0017,
    UNITSOUNDEVENT_NOT_IN_TOWN = 0x0018,
    UNITSOUNDEVENT_CALLFORHELP = 0x0019,
    UNITSOUNDEVENT_FOLLOWME = 0x001A,
    UNITSOUNDEVENT_THISISYOURS = 0x001B,
    UNITSOUNDEVENT_THANKYOU = 0x001C,
    UNITSOUNDEVENT_OOPS = 0x001D,
    UNITSOUNDEVENT_GOODBYE = 0x001E,
    UNITSOUNDEVENT_TIMETODIE = 0x001F,
    UNITSOUNDEVENT_RETREAT = 0x0020,
    UNITSOUNDEVENT_KILLANDARIELSPEECH = 0x0021,
    UNITSOUNDEVENT_KILLBLOODRAVENSPEECH = 0x0022,
    UNITSOUNDEVENT_CLEARDENOFEVIL = 0x0023,
    UNITSOUNDEVENT_FINDHORADRICMALUS = 0x0024,
    UNITSOUNDEVENT_FINDCAIRNSTONES = 0x0027,
    UNITSOUNDEVENT_FINDINIFUSSTREE = 0x002D,
    UNITSOUNDEVENT_ENTERWILDERNESS = 0x002F,
    UNITSOUNDEVENT_FREECAIN = 0x0030,
    UNITSOUNDEVENT_BREAKSUNALTAR = 0x0034,
    UNITSOUNDEVENT_KILLMEPHISTO = 0x0042,
    UNITSOUNDEVENT_KILLSHENK = 0x0050,
    UNITSOUNDEVENT_MERCLEVELUP = 0x005B,
};

FUNC(BYTE,PASCAL,D2GetExitWalkYfromUnit, (LPUNIT), &D2Common_10368);
FUNC(PlayerData*, PASCAL, GetptPlayerDatafromptUnit, (LPUNIT), &D2Common_10424);
FUNC(DWORD, PASCAL, IsWaypointActive, (DWORD,DWORD), &D2Common_11146);
FUNC(VOID, PASCAL, SetWPHistory, (DWORD,DWORD), &D2Common_11147);
FUNC(DWORD,FASTCALL, GetptISCTXTfromStat, (DWORD), (DWORD)(D2CommonBase + 0x642B0));

FUNC(void,FASTCALL,D2WarpClient, (Client* ptClient, DWORD uk1, DWORD unitID, DWORD uniqueID, DWORD uk2, DWORD pointX, DWORD pointY, DWORD uk3, DWORD ExitWalkY),(DWORD)(D2GameBase+0x00C920)); 
FUNC(DWORD, FASTCALL, CheckforOwnedItemfromItemCode, (LPGAME,LPUNIT,DWORD), (DWORD)(D2GameBase+0x020410));
FUNC(void,FASTCALL,D2WarpServer, (D2Game* ptGame, Unit* ptPlayer, DWORD uk1, DWORD uk2, DWORD pointX, DWORD pointY, DWORD uk3),(DWORD)(D2GameBase+0x0517D0)); 
FUNC(bool,FASTCALL,D2WarpCharacter, (D2Game* ptGame, Unit* ptPlayer, LPUNIT ptRoom, DWORD pointX, DWORD pointY, DWORD uk1, DWORD uk2), (DWORD)(D2GameBase+0x08DFE0)); 
// D2UpdateClientItem(ptClient,ptPlayer,ptItem,dwCmd flag);
// dwCmd = 0x20 = Remove Item 
FUNC(VOID, FASTCALL, D2UpdateClientItem, (DWORD,LPPLAYER,LPUNIT,DWORD), (DWORD)(D2GameBase+0x00E9D0));
// D2RemoveItem(ptGame,ptPlayer,ptItem,0);
FUNC(VOID, FASTCALL, D2RemoveItem, (LPGAME,LPPLAYER,LPUNIT,DWORD), (DWORD)(D2GameBase+0x0171F0));
/* 
    Date: Sun Jun 14 17:32:35 2009
    Author: Necrolis
    Function: UNITS_AddSound
    Address: D2Game.0x6FCBC480
    Comments: the sounds added are hardcoded...
	
	void __fastcall UNITS_AddSound(D2UnitStrc* pUnit, WORD wSound, D2UnitStrc* pUpdate)
*/

FUNC(VOID, FASTCALL, UNITS_AddSound, (LPUNIT, WORD, LPUNIT), (DWORD)(D2GameBase+0x8C480));

//=======================================================================
// Convenience methods

LPVOID FASTCALL GetptClientfromptPlayer (LPUNIT ptPlayer) {
	if (ptPlayer==NULL) {
		// bad ptPlayer
		return NULL;
	}
	if (ptPlayer->nUnitType!=0) {
		// not Player
		return NULL;
	}
    PlayerData* ptPlayerData = GetptPlayerDatafromptUnit(ptPlayer);
	if(ptPlayerData==NULL) {
		return NULL;
	}
    return ptPlayerData->ptClient;     
}

VOID FASTCALL KillItem13 (LPGAME ptGame, LPUNIT ptPlayer, LPUNIT ptItem) {
	D2UpdateClientItem((DWORD)GetptClientfromptPlayer(ptPlayer), ptPlayer, ptItem, 0x20);
	D2RemoveItem(ptGame, ptPlayer, ptItem, 0);
}

//=======================================================================
// Game functionality

DWORD GetLevelWarpInfo(DWORD LevelFrom, DWORD LevelTo) {
	for (DWORD i=0; i<LW_entries; i++) {
		bool from = (LW[i].LevelFrom==NULL || LW[i].LevelFrom==LevelFrom);
		bool to = (LW[i].LevelTo==NULL || LW[i].LevelTo==LevelTo);
		if (!LW[i].Invalid && from && to) {
			// Match
			if (Log>=1) {
				D2LogMsg("Found %d-%d to Match requested %d-%d", LW[i].LevelFrom, LW[i].LevelTo, LevelFrom, LevelTo);
			}
			return i;
		}
	}
	if (Log>=1) {
		D2LogMsg("Cannot find a match for %d-%d",LevelFrom,LevelTo);
	}
	return ~0;
}

// CheckVisWarp
void STDCALL CheckVisWarp(Unit* ptPlayer, LPUNIT ptRoom1, LPUNIT ptRoom2, DWORD pointX, DWORD pointY, D2LvlWarpTXT* ptWarp) {
	DWORD LevelFrom = D2GetLevelFromRoom(ptRoom1);
	DWORD LevelTo = D2GetLevelFromRoom(ptRoom2);
	DWORD LevelWarpId = GetLevelWarpInfo(LevelFrom, LevelTo);
	Client* ptClient;
	LPGAME ptGame = ptPlayer -> ptGame;
	if (LevelWarpId>=0 && LevelWarpId < LW_entries) {
		if (LW[LevelWarpId].Invalid) {
			// Something's broken... Disable!
			if (Log>1) D2LogMsg("LevelWarp has errors, check the line");
			UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_OOPS, ptPlayer);
			return;
		}
		BYTE Diff = ptGame->Difficulty;
		if (LW[LevelWarpId].Diff[Diff]==0) {
			// Not active this Difficulty! Disable!
			if (Log>0) D2LogMsg("LevelWarp not active in this Difficulty");
			UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_IMPOSSIBLE, ptPlayer);
			return;
		}
		// Check for Key First
		LPUNIT ptKeyItem = NULL;
		DWORD KeyItem=LW[LevelWarpId].KeyItem[Diff];
		if(KeyItem!=0 && KeyItem!=0x20202020) {
			ptKeyItem = CheckItemPresence(ptPlayer, Diff, KeyItem);
			if(ptKeyItem == NULL) {
				if (Log>1) D2LogMsg("LevelWarp is locked");
				UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_ITSLOCKED, ptPlayer);
				return;
			}
		}
		// Do all checks
		int LvlReq = LW[LevelWarpId].LvlReq[Diff];
		if (LvlReq != 0) {
			int pLvl = D2CommonGetStat(ptPlayer,STATS_LEVEL,0);
			if (Log>0) D2LogMsg("Player Level: %d, Req Level: %d",pLvl,LvlReq);
			if (LvlReq > pLvl) {
				if (Log>0) D2LogMsg("Level Req not met");
				UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_CANTUSETHISYET, ptPlayer);
				return;
			}
		}
		int ValReq;
		DWORD Stat;
		for (int j=0; j<3; j++) {
			ValReq = LW[LevelWarpId].Val[Diff][j];
			if (ValReq != 0) {
				Stat = LW[LevelWarpId].Stat[Diff][j];
				if ( Stat==0xFFFF ) {
					D2LogMsg("StatReq%d difficulty: %d for LevelWarp %d-%d was not recognised", j, Diff, LevelFrom, LevelTo);
				} else {
					int pVal = D2CommonGetStat(ptPlayer, LW[LevelWarpId].Stat[Diff][j],0);
					if (Log>0) D2LogMsg("Stat: %d Player Value: %d, Req Value: %d", LW[LevelWarpId].Stat[Diff][j], pVal, ValReq);
					if (ValReq > pVal) {
						if (Log>0) D2LogMsg("Stat Req not met");
						UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_CANTUSETHISYET, ptPlayer);
						return;
					}
				}
			}
		}
		// Now check ReqItem
		DWORD ptReqItem = NULL;
		DWORD ReqItem = LW[LevelWarpId].ReqItem[Diff];
		if (ReqItem!=0 && ReqItem!=0x20202020) {
			if (Log>0) D2LogMsg("Req Item: %c%c%c%c",(ReqItem)&0xFF,(ReqItem>>8)&0xFF,(ReqItem>>16)&0xFF,(ReqItem>>24)&0xFF);
			ptReqItem = CheckforOwnedItemfromItemCode(ptGame, ptPlayer, ReqItem);
			if (ptReqItem==NULL) {
				// no match
				if (Log>0) D2LogMsg("Req Item not met");
				UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_CANTUSETHISYET, ptPlayer);
				return;
			} else {
				BYTE StorePage = ((LPUNIT)ptReqItem)->pItemData->StorePage;
				if ((StorePage == 0   && LW[LevelWarpId].ReqItemCarried!=0) ||
					(StorePage == 255 && LW[LevelWarpId].ReqItemEquipped!=0)) {
				} else {
					if (Log>0) D2LogMsg("Req Item in wrong location");
					UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_CANTUSETHISYET, ptPlayer);
					return;
				}
			}
		}
		// If you're here, you can pay for everything thus far. Last Check for Toll & PenStat
		DWORD Toll = LW[LevelWarpId].Toll[Diff];
		short PenStat = LW[LevelWarpId].PenStat[Diff];
		short PenVal = LW[LevelWarpId].PenStat[Diff];
		if (PenStat==14 || PenStat==15) {
			Toll+=PenVal;
			PenVal=0;	// Negate the check later
		}
		if (Toll>D2CommonGetStat(ptPlayer,STATS_GOLD,0)) {
			// Not enough money!
			if (Log>0) D2LogMsg("You don't have enough to pay the Toll of %d",Toll);
			UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_CANTUSETHISYET, ptPlayer);
			return;
		}
		// You have successfully proven your worth, pay up :)
		D2CommonAddStat(ptPlayer,STATS_GOLD,1+~Toll,0);
		// PenStat & PenVal
		if (PenVal!=0) {
			int pVal = D2CommonGetStat(ptPlayer,Stat,0);
			if ((pVal-PenVal)<1) {
				D2CommonSetStat(ptPlayer,Stat,1,0);
			} else {
				int MaxVal = *(DWORD*)(GetptISCTXTfromStat(Stat)+0x32);
				if (MaxVal>0) {
					if ((pVal-PenVal)>MaxVal) {
						D2CommonSetStat(ptPlayer,Stat,MaxVal,0);
					} else {
						D2CommonAddStat(ptPlayer,Stat,-PenVal,0);
					}
				}
			}
		}
		// Time to cough up KeyItem & ReqItem
		if (ptKeyItem!=NULL) {
			// Ah ha! Pay up!
			KillItem13(ptPlayer->ptGame, ptPlayer, (LPUNIT)ptKeyItem);
		}
		if (ptReqItem!=NULL) {
			// Ah ha! Pay up!
			KillItem13(ptPlayer->ptGame, ptPlayer, (LPUNIT)ptReqItem);
		}
	}
	bool Warped = D2WarpCharacter(ptGame, ptPlayer, ptRoom1, pointX, pointY, 0, 0);
	if (Warped) {
		if (ptPlayer->nUnitType == 0) {
            PlayerData* ptPlayerData = ptPlayer->pPlayerData;
            ptClient = (Client*)(ptPlayerData->ptClient);
		} else {
            ptClient = NULL;
		}
		D2LogMsg("Warped, applying...");
		D2WarpServer(ptPlayer->ptGame, ptPlayer, 0, 2, pointX+ptWarp->ExitWalkX, pointY+ptWarp->ExitWalkY, 0);
		D2WarpClient(ptClient, 0xD, ptPlayer->nUnitId, ptPlayer->nUnitUnid, 1, pointX+ptWarp->ExitWalkX, pointY+ptWarp->ExitWalkY, 0, D2GetExitWalkYfromUnit(ptPlayer));
	} else {
		D2LogMsg("Didn't warp.");
	}
}

DWORD FASTCALL CheckReqsFunc (LPPLAYER ptPlayer, BYTE Diff, DWORD pHistory, int WayptID) {
	WPTable* tWP=NULL;
	for (int i=0; i<(int)WP_entries; i++, tWP++) {
		if (WP[i].WayptID==WayptID) {
			tWP = &WP[i];
			break;
		}
	}
	if(tWP!=NULL) {
		int Match=1;
		DWORD ptReqItem=NULL;
			// matched!
			if (tWP->Invalid) {
				// Something's broken... Disable!
				D2LogMsg("%s(%d): %s", __FILE__, __LINE__, "Oops");
				UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_OOPS, ptPlayer);
				return -1;
			}
			if (tWP->Diff[Diff]==0) {
				// Not active this act! Disable!
				D2LogMsg("%s(%d): %s", __FILE__, __LINE__, "Impossible");
				UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_IMPOSSIBLE, ptPlayer);
				return -1;
			}
			if (tWP->OneWay==1) {
				// Entrance only waypt! Disable!
				D2LogMsg("%s(%d): %s", __FILE__, __LINE__, "Impossible");
				UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_IMPOSSIBLE, ptPlayer);
				return -1;
			}
if(tWP->KeyItem[Diff]!=0 && tWP->KeyItem[Diff]!=0x20202020) {
	if(!IsWaypointActive(pHistory, tWP->WayptID)) {
		// Inactive, and don't have key
				D2LogMsg("%s(%d): %s", __FILE__, __LINE__, "Its Locked");
		UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_ITSLOCKED, ptPlayer);
		return -1;
	}
	if(tWP->NeedKeyItem[Diff]) {
		LPUNIT ptKeyItem = CheckItemPresence(ptPlayer, Diff, tWP->KeyItem[Diff]);
		if(ptKeyItem == NULL) {
			// Need key which we don't have
				D2LogMsg("%s(%d): %s", __FILE__, __LINE__, "Its Locked");
			UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_ITSLOCKED, ptPlayer);
			return -1;
		}
	}
}
			// Check Reqs
			int LvlReq = tWP->LvlReq[Diff];
			if (LvlReq != 0) {
				int pLvl = (int)(D2CommonGetStat(ptPlayer,STATS_LEVEL,0));
				if (Log>0) D2LogMsg("Player Level: %d, Req Level: %d",pLvl,LvlReq);
				if (LvlReq > pLvl) {
					if (Log>0) D2LogMsg("Level Req not met");
					Match=0;
				}
			}
			if (Match!=0) {
				int ValReq;
				DWORD Stat;
				for (int j=0; j<3; j++) {
					ValReq = tWP->Val[Diff][j];
					if (ValReq != 0) {
						Stat = tWP->Stat[Diff][j];
						if ( Stat==0xFFFF ) {
							D2LogMsg("StatReq%d difficulty: %d for WayptID %d was not recognised",j, Diff, tWP->WayptID);
						} else {
							int pVal = (int)(D2CommonGetStat(ptPlayer,tWP->Stat[Diff][j],0));
							if (Log>0) D2LogMsg("Stat: %d Player Value: %d, Req Value: %d",tWP->Stat[Diff][j],pVal,ValReq);
							if (ValReq > pVal) {
								if (Log>0) D2LogMsg("Stat Req not met");
								Match=0;
								break;
							}
						}
					}
				}
			}
			if (Match!=0) {
				// Now check ReqItem
				DWORD ReqItem = tWP->ReqItem[Diff];
				if (ReqItem!=0 && ReqItem!=0x20202020) {
					if (Log>0) D2LogMsg("Req Item: %c%c%c%c",(ReqItem)&0xFF,(ReqItem>>8)&0xFF,(ReqItem>>16)&0xFF,(ReqItem>>24)&0xFF);
					ptReqItem = CheckforOwnedItemfromItemCode(ptPlayer->ptGame,ptPlayer,ReqItem);
					if (ptReqItem==NULL) {
						// no match
						if (Log>0) D2LogMsg("Req Item not met");
						Match=0;
					} else {
						BYTE StorePage = ((LPUNIT)ptReqItem)->pItemData->StorePage;
						if ((StorePage == 0   && tWP->ReqItemCarried!=0) ||
							(StorePage == 255 && tWP->ReqItemEquipped!=0)) {
							if (tWP->KillReqItem[Diff]!=0) {
								// Kill ReqItem on usage, store in gReqItem for PayUsage
								//gReqItem= ptReqItem;
							}
						} else {
							if (Log>0) D2LogMsg("Req Item in wrong location");
							Match=0;
						}
					}
				}
			}
			if (Match==0) {
				// No match, no waypoint updated for you
				if (tWP->Disable!=0) {
					UNITS_AddSound(ptPlayer, UNITSOUNDEVENT_CANTUSETHISYET, ptPlayer);
					if (Log>0) D2LogMsg("Waypoint Disabled");
					return -1;	// Disabled if no match
				}
				D2LogMsg("%s(%d): %s", __FILE__, __LINE__, "No Match");
				return 0;
			}
			// All reqs met or no reqs.
			return 1;
		}
	D2LogMsg("Waypoints.dll::CheckReqsFunc >> Waypoint not Found! No match for LevelID");
	return -1;
}

DWORD FASTCALL CheckReqHost (LPGAME ptGame, LPUNIT ptPlayer, DWORD pHistory, int WayptID) {
	BYTE Diff = ptGame->Difficulty;
//	Validator();
//	InitFunc(pHistory, WayptID, Diff);	// Put InitFunc here now.
	// KeyItem unlocking done.
	int r = CheckReqsFunc(ptPlayer, Diff, pHistory, WayptID);
	if (r<=0) {
		// WPActive=false;
	} else {
		// WPActive=true;
		if (r==1) SetWPHistory(pHistory,WayptID);
	}
	return r;
}

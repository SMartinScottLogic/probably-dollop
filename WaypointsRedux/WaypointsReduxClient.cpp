// MyMod.cpp : Sample mod dll for use with D2Mod.dll mod system
// Author: SVR 2004
//
//=======================================================================

#include <d2mod.h>	// main include for all import definitions/library functions
#include <NewTxt.h>

#include "WaypointsRedux.h"	// add you header here

//=======================================================================
// Diablo 2 functions

FUNC(DWORD,FASTCALL, GetptISCTXTfromStat, (DWORD), (DWORD)(D2CommonBase + 0x642B0));
FUNC(DWORD, PASCAL, GetQuestState, (DWORD,DWORD,DWORD),(DWORD)&D2Common_11107);
FUNC(DWORD, PASCAL, IsWaypointActive, (DWORD,DWORD), &D2Common_11146);

FUNC(DWORD, PASCAL, IsLod1, (VOID), &Fog_10227);

FUNC(VOID, FASTCALL, DisplayText, (DWORD,DWORD,DWORD,DWORD,DWORD), &D2Win_10117);

FUNC(VOID, PASCAL, DrawDC6, (DWORD,DWORD,DWORD,DWORD,DWORD,DWORD), &D2Gfx_10072);

FUNC(DWORD, PASCAL, IsLod2, (VOID), (DWORD)(D2ClientBase+0xC080));
FUNC(VOID, FASTCALL, D2Client_00DA40, (DWORD,DWORD,DWORD), (DWORD)(D2ClientBase+0x00DA40));
FUNC(DWORD, PASCAL, D2Client_05BD20, (VOID), (DWORD)(D2ClientBase+0x05BD20));
FUNC(DWORD, FASTCALL, D2Client_0839F0, (DWORD), (DWORD)(D2ClientBase+0x0839F0)); // GetUnicodeNamefromLevelID
FUNC(LPPLAYER, PASCAL, GetClientPlayer, (VOID), (DWORD)(D2ClientBase+0x883D0));
FUNC(BYTE, PASCAL, GetClientDiff, (VOID), (DWORD)(D2ClientBase+0xC090));
FUNC(LPVOID, PASCAL, GetClientPlayerRoom, (VOID), (DWORD)(D2ClientBase + 0x89370));
//=======================================================================
// Client global data

BYTE WPIcon[9] = {0,0,0,0,0,0,0,0,0};	// For more info passing
int cancelX, cancelY;

extern int clientTabRight[5];

//=======================================================================
// Convenience methods

VOID PASCAL DrawSimpleDC6 (DWORD Mem1, DWORD IconCel, DWORD X, DWORD Y) {
	*(DWORD*)(Mem1+8)=IconCel;
	DrawDC6(Mem1,X,Y,-1,5,0);
}

int IsLod () {
    if (IsLod1()==0) return 0;
    if (IsLod2()==0) return 0;
    return 1;
}

DWORD FASTCALL GetQuestState13 (DWORD QuestID) {
	return GetQuestState(D2Client_05BD20(),QuestID,0);
}

//=======================================================================
// Client functionality
/*
VOID FASTCALL UpdateA () {
	int MaxAct = IsLod()+4;
	for (int i=1; i<MaxAct; i++) {
		if (A[i]==0) {
			if (QuestID[i]==-1) {
				A[i]=1;
			} else {
				A[i]=GetQuestState13(QuestID[i]);
			}
		}
	}
}
*/

bool isActActive(int Act) {
	D2LogMsg("%s(%d)", __FILE__, __LINE__);
	if(QuestID[Act]==-1) {
		return true;
	}
	return GetQuestState13(QuestID[Act])!=0;
}

WPTable* static_CurrentWP = NULL;
bool static_OnlyToWP;
bool static_hasWPs[25];
int static_WantedSet = 0;
BYTE static_Arrow[2] = {0,0};
int  static_A[5]={1,0,0,0,0};
void generateStatics() {
	for (int i=0; i<25; i++) {
		// Clear all previous settings
		static_hasWPs[i]=false;
	}

	DWORD LevelFrom = D2GetLevelFromRoom(D2GetRoomFromUnit(GetClientPlayer()));
	static_CurrentWP = GetWaypointFromLevelID(LevelFrom);
	
	static_OnlyToWP = (static_CurrentWP->OnlyTo[0]!=255);
	// Update A 
	int MaxAct = IsLod()+4;
	for (i=1; i<MaxAct; i++) {
		if (static_A[i]==0) {
			if (QuestID[i]==-1) {
				static_A[i]=1;
			} else {
				static_A[i]=GetQuestState13(QuestID[i]);
			}
		}
	}
}

VOID FASTCALL FindFirstInAct(int tAct) {
	D2LogMsg("%s(%d)", __FILE__, __LINE__);
	for (int i=0; i<5; i++) {
		if (static_hasWPs[tAct*5+i]) {
			static_WantedSet=i;
			break;
		}
	}
}

/*
DWORD FASTCALL CheckKeyItem(WPTable* tWP, DWORD pHistory) {
	BYTE Diff = GetClientDiff();
	DWORD KeyItem=tWP->KeyItem[Diff];
	if (KeyItem==0 || KeyItem==0x20202020) {
		// Nothing to check
		return KEY_NA;
	}
	// Have KeyItem, check
	DWORD IsWPActive = IsWaypointActive(pHistory, tWP->WayptID);
	if (!IsWPActive) {
		// no Key!
		return KEY_DONTHAVE;
	}
	if (tWP->NeedKeyItem[Diff]) {
		LPPLAYER ptPlayer = GetClientPlayer();
		LPUNIT ptKeyItem = CheckItemPresence(ptPlayer, KeyItem);
		if (ptKeyItem==NULL) {
			// no Key!
			return KEY_DONTHAVE;
		}		
		return ptKeyItem;
	} else {
		return KEY_HAVE;	// it's ok 
	}
}
*/

DWORD PASCAL ToDrawTabs(int Tab) {
	// i = 0-4 the Tabs on top.
	// return -1 = No Tab
	// return 0,1= Selected/Unselected

	// Get WP for current level
	generateStatics();
	
	if (static_OnlyToWP) {
		// No tabs if OnlyTo
		return -1;
	}

	int WantedAct = (int)*(DWORD*)(D2ClientBase+0x11A9BE);
	if (SwapAnT) {
		if (!isActActive(Tab)) {
			return -1;
		}
		if (Tab==WantedAct) {
			return 0;
		} else {
			return 1;
		}
	} else {
		if (!static_hasWPs[WantedAct*5+Tab]) {
			return -1;
		}
		if (Tab==static_WantedSet) {
			return 0;
		} else {
			return 1;
		}
	}
}

int getNumOfPages() {
	int NumOfPages=0;
	if (SwapAnT) {
		// Check for Page
		int WantedAct = *(DWORD*)(D2ClientBase+0x11A9BE);
		for (int i=WantedAct*5; i<WantedAct*5+5; i++) {
			if (static_hasWPs[i]) {
				NumOfPages++;
			}
		}
	} else {
		for (int i=static_WantedSet; i<20+static_WantedSet; i+=5) {
			if (static_hasWPs[i]) {
				NumOfPages++;
			}
		}
	}
	return NumOfPages;
}

VOID PASCAL ToDrawArrows (DWORD Mem1, int Xleft, int Ybottom, DWORD d4, DWORD d5, DWORD d6) {	// DONE
	// Draw the Cancel first
	DrawDC6(Mem1,(DWORD)Xleft,(DWORD)Ybottom,d4,d5,d6);
	generateStatics();
	if (static_OnlyToWP!=NULL) {
		// No arrows if OnlyTo
		return;
	}
	int NumOfPages=getNumOfPages();
	if (NumOfPages<=1) {
		// no need to draw arrows
		return;
	}
	cancelX = Xleft;
	cancelY = Ybottom;
	for (int i=0; i<2; i++) {
		if (static_Arrow[i]==0) {
			*(DWORD*)(Mem1+8)=12+i*2;
		} else {
			*(DWORD*)(Mem1+8)=13+i*2;
		}
		DrawDC6(Mem1,cancelX - 80 + (i * 34),cancelY,d4,d5,d6);
	}
}

DWORD PASCAL GetHeader () {
	generateStatics();

	int WantedAct = *(DWORD*)(D2ClientBase+0x11A9BE);
	int h;
	if (static_CurrentWP->OnlyTo[0]!=255) {
		// You have OnlyTos
		h = static_CurrentWP->Header;
	} else {
		// TODO recompute
		//h = Header[WantedAct*5+static_WantedSet];
		h = 3995;
	}
	if (h == 0) {
		h = 3995;
	}
	D2LogMsg("GetHeader: [%d] (WaypointID: %d, LevelID: %d, OnlyTo: %d, WantedAct: %d, WantedSet: %d)", 
		h, static_CurrentWP->WayptID, static_CurrentWP->LevelID, static_CurrentWP->OnlyTo[0], WantedAct, static_WantedSet);
	return h;
}

// ------Icons and Text------
// Come back to do displays for special cases, OneWay, etc
// Hidden - No icon, text if not active
// OnlyTo - straightforward
//

VOID PASCAL DrawIconsAndText (DWORD Mem1, WaypointTable *WPT, WaypointDisplay *WPD, int CurrentLevelID) {
	int BorderX   = (int)*(DWORD*)(D2ClientBase+0x11A748);
	int BorderY   =-(int)*(DWORD*)(D2ClientBase+0x11A74C);
	int WantedRow = (int)*(DWORD*)(D2ClientBase+0x11A9A5);

	for (int i=0; i<9; i++, WPT++, WPD++) {
		if (WPT->LevelID!=0) {
			// Waypoint is there
			// Default to lightgrey text and no mouseover
			int color = 0;
			int mouse = -1;
			switch (WPIcon[i]) {
			case WP_HIDDEN :
				color = -1;				break;	// Hidden
			case WP_NULL :
				color = COLOR_DARKGREY;	break;	// Non-activated Waypt
			case WP_LOCKED :
				color = COLOR_DARKGREY;	break;	// Non-activated Waypt
			case WP_THISLEVEL :
				color = COLOR_BLUE;		break;	// This Level
			case WP_INACTIVE :
				color = COLOR_PURPLE;	break;	// Inactive Waypt
			case WP_ONEWAY :
				mouse = COLOR_BRIGHTWHITE;	break;	// OneWay
			case WP_STAR :
				mouse = COLOR_YELLOW;	break;	// Only From/To
			case WP_KEY :
				mouse = COLOR_RED;		break;	// Unlocked Waypt
			default :
				mouse = COLOR_BLUE;		break;	// Normal and all else
			}
			if (color>=0) {	// No Hidden
				DrawSimpleDC6(Mem1,WPIcon[i],WPD->IconX+BorderX,WPD->IconY+BorderY);
				if (WantedRow==i && mouse!=-1) color=mouse;
				DisplayText(D2Client_0839F0(WPT->LevelID),WPD->TextX+BorderX,WPD->TextY+BorderY,color,0);
			}
		}
	}
}

VOID FASTCALL UpdateWPT (int CurrentLevelID) {
	WaypointTable *WPT = (WaypointTable*)(D2ClientBase+0x11A9CC);
	DWORD pHistory = *(DWORD*)(D2ClientBase+0x11A9B9);
	BYTE Diff = GetClientDiff();
	LPPLAYER ptPlayer = GetClientPlayer();
	int CurrentWayptID = GetWayptIDfromLevelID(CurrentLevelID);

	int C2=-1;	// Checker, and a variable for multiple usage
	bool ShowText, ShowIcon, IsActive;

	generateStatics();
	for (int i=0; i<9; i++) {
		// Clear the WPIcon
		WPIcon[i]=WP_NULL;
	}
	if (static_OnlyToWP) {
		WPTable* currentWP = GetWaypointFromLevelID(CurrentLevelID);
		// OnlyTo Waypoint! Hijack!
		*(DWORD*)(D2ClientBase+0x011A9C2)=1;	// Override so set this too.
		*(DWORD*)(D2ClientBase+0x011A9C6)=1;
		BYTE ToWayptID[5];

		WPT->LevelID=CurrentLevelID;
		WPT->Active = 0;
		WPIcon[0] = (IsWaypointActive(pHistory,CurrentWayptID))?WP_THISLEVEL:WP_INACTIVE;
		for (int NumTo=0; NumTo<5; NumTo++) {
			ToWayptID[NumTo]=currentWP->OnlyTo[NumTo];
			if (ToWayptID[NumTo]==255) break;
			if (IsWaypointActive(pHistory,(DWORD)(ToWayptID[NumTo]))) {
				(WPT+NumTo+1)->Active = 1;
				WPIcon[NumTo+1] = WP_STAR;
			}
		}	// NumTo = # of OnlyTos
		for (int i=0; i<(int)WP_entries; i++) {
			if (!(WP[i].Invalid)) {
				for (int j=1; j<=NumTo; j++) {
					if (WP[i].WayptID == ToWayptID[j-1]) {
						// Match, check for Hidden, OneWay, Key
						IsActive = ((WPT+j)->Active!=0);
						if (WP[i].Hidden && !IsActive) {
							// Hidden + NoDisplay, no text (no display already)
							ShowText=false;
							WPIcon[j] = WP_NULL;
						} else {
							ShowText=true;
						}
						if (WP[i].OneWay==1) {
							WPIcon[j] = WP_ONEWAY;
						} else if (WP[i].OneWay==255) {
							// Exit only, not visible
							WPIcon[j] = WP_NULL;
							(WPT+j)->Active = 0;
						}
						DWORD KeyItem = WP[i].KeyItem[Diff];
						if (KeyItem!=0 && KeyItem!=0x20202020) {
							DWORD IsWPActive = IsWaypointActive(pHistory, WP[i].WayptID);
							LPUNIT ptKeyItem = CheckItemPresence(ptPlayer, Diff, KeyItem);
							if (IsActive && WP[i].NeedKeyItem[Diff]==0 ) {
								// Waypt is Active and key not needed, so use default icon = WP_STAR
							} else {
								// Key Override
								if (!IsWPActive || (WP[i].NeedKeyItem[Diff] && ptKeyItem==NULL)) {
									(WPT+j)->Active = 0;	// In case active, but NeedKey not met
									if (ShowText) {	// Not Hidden
										WPIcon[j] = WP_LOCKED;
									}
								} else {
									// Ok!
									ShowText=true;
									(WPT+j)->Active = 1;
									WPIcon[j] = WP_KEY;
								}
							}
						}
						if (ShowText) {
							C2 = (int)(WP[i].LevelID);
							(WPT+j)->LevelID = C2;
						}
						D2LogMsg("UpdateWPT: %d (Active: %d, Icon: %d, LevelID: %d) [KeyItem: %x, wpActive: %d]", j, (WPT+j)->Active, WPIcon[j], (WPT+j)->LevelID, WP[i].KeyItem[Diff], IsWaypointActive(pHistory, WP[i].WayptID));
					}
				}
			}
		}
	} else {	// Not OnlyTo
		int tTab, tPage = (int)*(DWORD*)(D2ClientBase+0x11A9BE);
		if (SwapAnT) {
			tTab  = tPage;
			tPage = static_WantedSet;
		} else {
			tTab  = static_WantedSet;
		}
		for (int i=0; i<(int)WP_entries; i++) {
			if (!(WP[i].Invalid)) {
				if ((int)(WP[i].Page-1)==tPage && (int)(WP[i].Tab-1)==tTab) {
					int j = (WP[i].Row)-1;
					ShowText = true;
					IsActive = (IsWaypointActive(pHistory, WP[i].WayptID)!=0);
					if (IsActive) {
						ShowIcon = true;
						(WPT+j)->Active=1;
					} else {
						ShowIcon = false;
						(WPT+j)->Active=0;
					}
					if (WP[i].Hidden!=0 &&!ShowIcon) {
						// Hide if WP not active
						ShowText = false;
					}
					// Icon & WPT->Active
					if ((int)(WP[i].LevelID)==CurrentLevelID) { // if CurrentLevel, Active = 0
						(WPT+j)->Active=0;
						ShowText = true;
						WPIcon[j] = (ShowIcon)?WP_THISLEVEL:WP_INACTIVE;
					} else {
						if (WP[i].OneWay==1) {
							// OneWay Exit override
							ShowText=true;
							ShowIcon=false; // No default Icon
							(WPT+j)->Active = 1;
							WPIcon[j] = WP_ONEWAY;
						} else if (WP[i].OneWay==255) {
							ShowText = false;
							ShowIcon = false; // No default Icon
							(WPT+j)->Active=0;
						}
						if (WP[i].OnlyFrom[0]!=255) {
							int NumFrom, temp;
							for (NumFrom=0; NumFrom<5; NumFrom++) {
								temp = (int)(WP[i].OnlyFrom[NumFrom]);
								if (temp==255) {
									// no match!
									ShowText=false;
									ShowIcon=false;
									(WPT+j)->Active=0;
									WPIcon[j] = WP_NULL;
									break;
								}
								if (temp==CurrentWayptID) {
									// match!
									ShowText=true;
									ShowIcon=false;
									(WPT+j)->Active=1;
									WPIcon[j] = WP_STAR;
									break;
								}
							}
						}

						DWORD KeyItem=WP[i].KeyItem[Diff];
						if(KeyItem!=0 && KeyItem!=0x20202020) {
							ShowText=true;
							if (IsActive && WP[i].NeedKeyItem[Diff]==0 ) {
								// Waypt is Active and key not needed, so use default icon
								ShowIcon = true;
							} else {
								DWORD IsWPActive = IsWaypointActive(pHistory, WP[i].WayptID);
								LPUNIT ptKeyItem = CheckItemPresence(ptPlayer, Diff, KeyItem);
								// Key Override
								ShowIcon=false; // No default Icon
								if (!IsWPActive || (WP[i].NeedKeyItem[Diff] && ptKeyItem==NULL)) {
									(WPT+j)->Active = 0;
									WPIcon[j] = WP_LOCKED;	// locked
								} else {
									(WPT+j)->Active = 1;
									WPIcon[j] = WP_KEY;
								}
							}
						}
						if (ShowIcon) {	// Default
							(WPT+j)->Active = 1;
							WPIcon[j] = WP_NORMAL;
						}
					}
					if (ShowText) {
						C2 = (int)(WP[i].LevelID);
						(WPT+j)->LevelID = C2;
					}
					D2LogMsg("UpdateWPT: %d (Active: %d, Icon: %d, LevelID: %d) [KeyItem: %x, wpActive: %d]", j, (WPT+j)->Active, WPIcon[j], (WPT+j)->LevelID, WP[i].KeyItem[Diff], IsWaypointActive(pHistory, WP[i].WayptID));
				}
			}
		}
		if (C2!=-1) {
			*(DWORD*)(D2ClientBase+0x011A9C2)=1;
			*(DWORD*)(D2ClientBase+0x011A9C6)=1;
		}
	}
}

int FASTCALL ArrowCycle (int WantedAct, int Change) {	// DONE
	D2LogMsg("%s(%d)", __FILE__, __LINE__);
	int ArrowInitial = (SwapAnT)?static_WantedSet:WantedAct;
	int aVal = ArrowInitial;
	do {
		aVal += Change;
		if (aVal==-1) aVal=4;
		else if (aVal==5) aVal=0;
		// Now check for exit conditions
		if (SwapAnT) {
			if (static_hasWPs[WantedAct*5+aVal]) {
				// This Set has stuff!
				static_WantedSet = aVal;
				break;
			}
		} else {
			if (static_A[aVal]!=0) {
				// This Act has stuff!
				*(DWORD*)(D2ClientBase+0x11A9BE) = aVal;
				break;
			}
		}
	} while (aVal!=ArrowInitial);
	// Exited
	if (aVal==ArrowInitial) {
		// Same as start
		return 0;
	} else {
		// Different
		return 1;
	}
}

DWORD PASCAL CheckPageTab (int Xpos, int Ypos) {
	generateStatics();
	if (static_OnlyToWP) {
		return 0;
	}
	DWORD mAct = (D2ClientBase+0x11A9BE);
	int WantedAct = *(DWORD*)(mAct);
	int BorderX   = (int)*(DWORD*)(D2ClientBase+0x11A748);
	int BorderY   =-(int)*(DWORD*)(D2ClientBase+0x11A74C);

	if (Xpos > 0 && Ypos<=30) {
		int wTab = 0;
		for(wTab = 0; wTab < 5; wTab++) {
			if(Xpos < clientTabRight[wTab]) {
				break;
			}
		}
		D2LogMsg("@ (%d, %d); wTab = %d", Xpos, Ypos, wTab);
		if (SwapAnT) {
			if (isActActive(wTab)) {
				*(DWORD*)mAct = (DWORD)wTab;
				FindFirstInAct(wTab);
				return 1;
			}
		} else {
			if (static_hasWPs[WantedAct*5+wTab] && wTab!=static_WantedSet) {
				static_WantedSet=wTab;
				return 1;
			}
		}
	} else {
		Xpos += BorderX;
		Ypos += BorderY;
		if (getNumOfPages()<=1) {
			return 0;
		}
		D2LogMsg("@ (%d, %d); cancel @ (%d, %d) - (%d, %d)", Xpos, Ypos, cancelX-80, cancelY-34, cancelX-12, cancelY);
		if ( Ypos < cancelY - 34 || Ypos > cancelY ) {
			return 0;
		}
		if ( Xpos < cancelX - 80 || Xpos > cancelX - 12) {
			return 0;
		}
		if (Xpos<cancelX - 46) {
			return ArrowCycle(WantedAct,-1);
		} else {
			return ArrowCycle(WantedAct, 1);
		}
	}
	return 0;
}

//			BYTE gameDiff = GetClientDiff();

DWORD FASTCALL PayUsageClient(DWORD targetLevelId, DWORD edx) {
	// ecx = target LevelID, edx = waypoint unid

	BYTE Diff = GetClientDiff();
	LPPLAYER ptPlayer = GetClientPlayer();
	int Toll;
	int Stat=-1, Val=0;
	for (int i=0; i<(int)WP_entries; i++) {
		if ((int)(WP[i].LevelID) != (int)targetLevelId) {
			continue;
		}
			// match, get UsageCost
			Toll = (int)(static_CurrentWP->ExFee[Diff])+(int)(WP[i].EnFee[Diff]);
			Val = static_CurrentWP->PenVal[Diff];
			if (Val!=0) {
				Stat = static_CurrentWP->PenStat[Diff];
				if ( Stat==0xFFFF ) {
					D2LogMsg("Penalty Stat for WayptID %d was not recognised",static_CurrentWP->WayptID);
					Val = 0;	// Skip Val check later
				} else if ( Stat==14 || Stat==15 ) {
					Toll += Val;
					Val = 0;	// Skip Val check later
				}
			}
			LPUNIT ptKeyItem = NULL;
			DWORD KeyItem=WP[i].KeyItem[Diff];
			if (KeyItem!=NULL && KeyItem!=0x20202020 && WP[i].NeedKeyItem[Diff]) {
				ptKeyItem = CheckItemPresence(ptPlayer, Diff, KeyItem);
				if (ptKeyItem==NULL) {
					// You don't have the Key or it's in your stash? You're grounded!
					D2Client_00DA40(0x49, edx, 0);
					return 0;
				}
				if ((WP[i].KillKeyItem[Diff])==0) {
					// If no need to kill, NULL ptKeyItem
					ptKeyItem=NULL;
				}
			}
			if (Toll>0) {
				// Pay up!
				int Gold = D2CommonGetStat(ptPlayer,STATS_GOLD,0);
				if (Gold>=Toll) {
					// You have enough money on hand
					if (Log>0) {
						D2LogMsg("Usage Toll %d Gold paid",Toll);
					}
				} else {
					// You do not have enough money on hand
					if (Log>0) {
						D2LogMsg("Usage Toll %d > Player Gold %d",Toll,Gold);
					}
					D2Client_00DA40(0x49, edx, 0);
					return 0;
				}
			}
			D2LogMsg("PayUsageClient: warp to %d", targetLevelId);
			D2Client_00DA40(0x49, edx, targetLevelId);
			return 1;
	}
	D2LogMsg("Waypoints.dll::PayUsage >> Waypoint not Found! No match for LevelID");
	D2Client_00DA40(0x49, edx, 0);
	return 0;
}
// TODO: Move changes over to server size (stat change, delete item(s))
/*
DWORD FASTCALL PayUsageClient(DWORD targetLevelId, DWORD edx) {
	// ecx = target LevelID, edx = waypoint unid

	BYTE Diff = GetDiff();
	int Toll;
	int Stat=-1, Val=0;
	for (int i=0; i<(int)WP_entries; i++) {
		if ((int)(WP[i].LevelID) != (int)targetLevelId) {
			continue;
		}
			// match, get UsageCost
			Toll = (int)(sWP->ExFee[Diff])+(int)(tWP->EnFee[Diff]);
			Val = sWP->PenVal[Diff];
			if (Val!=0) {
				Stat = sWP->PenStat[Diff];
				if ( Stat==0xFFFF ) {
					D2LogMsg("Penalty Stat for WayptID %d was not recognised",sWP->WayptID);
					Val = 0;	// Skip Val check later
				} else if ( Stat==14 || Stat==15 ) {
					Toll += Val;
					Val = 0;	// Skip Val check later
				}
			}
			DWORD ptKeyItem = NULL;
			DWORD KeyItem=tWP->KeyItem[Diff];
			if (KeyItem!=NULL && KeyItem!=0x20202020 && tWP->NeedKeyItem[Diff]) {
				ptKeyItem = CheckforOwnedItemfromItemCode(ptGame,ptPlayer,KeyItem);
				if (ptKeyItem==NULL || ((LPUNIT)ptKeyItem)->pItemData->StorePage==4) {
					// Hmm? Now you don't have the Key or it's in your stash? You're grounded!
					D2Client_00DA40(0x49, edx, 0);
					return 0;
				}
				if ((tWP->KillKeyItem[Diff])==0) {
					ptKeyItem=NULL;	// If no need to kill, NULL ptKeyItem
				}
			}
			if (Toll>0) {
				// Pay up!
				int Gold = D2CommonGetStat(ptPlayer,STATS_GOLD,0);
				if (Gold>=Toll) {
					// You have enough money on hand
					D2CommonSetStat(ptPlayer,STATS_GOLD,Gold-Toll,0);
					if (Log>0) D2LogMsg("Usage Toll %d Gold paid",Toll);
				} else {
					// You do not have enough money on hand
					if (Log>0) D2LogMsg("Usage Toll %d > Player Gold %d",Toll,Gold);
					D2Client_00DA40(0x49, edx, 0);
					return 0;
				}
			}
			// Time to cough up StatPenalty
			if (Val!=0) {
				int pVal = D2CommonGetStat(ptPlayer,Stat,0);
				if ((pVal-Val)<1) {
					D2CommonSetStat(ptPlayer,Stat,1,0);
				} else {
					int MaxVal = *(DWORD*)(GetptISCTXTfromStat(Stat)+0x32);
					if (MaxVal>0) {
						if ((pVal-Val)>MaxVal) {
							D2CommonSetStat(ptPlayer,Stat,MaxVal,0);
						} else {
							D2CommonAddStat(ptPlayer,Stat,-Val,0);
						}
					}
				}
			}
			// Time to cough up KeyItem & ReqItem
			if (ptKeyItem!=NULL) {
				// Ah ha! Pay up!
				KillItem13(ptGame,ptPlayer,(LPUNIT)ptKeyItem);
			}
			if (gReqItem!=NULL) {
				// Ah ha! Pay up!
				KillItem13(ptGame,ptPlayer,(LPUNIT)gReqItem);
			}
			D2Client_00DA40(0x49, edx, ecx);
			return 1;
	}
	D2LogMsg("Waypoints.dll::PayUsage >> Waypoint not Found! No match for LevelID");
	D2Client_00DA40(0x49, edx, 0);
	return 0;
}
*/
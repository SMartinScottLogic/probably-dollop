// MyMod.cpp : Sample mod dll for use with D2Mod.dll mod system
// Author: SVR 2004
//
//=======================================================================

#include <d2mod.h>	// main include for all import definitions/library functions
#include <NewTxt.h>

#include "WaypointsRedux.h"	// add you header here

FUNC(LPUNIT, PASCAL, GetCursorItem, (Inventory*), &D2Common_10262);
FUNC(LPUNIT, PASCAL, GetFirstInventoryItem, (Inventory*), &D2Common_10277);
FUNC(LPUNIT, PASCAL, GetNextInventoryItem, (LPUNIT), &D2Common_10304);
FUNC(BYTE *, PASCAL, GetItemRecord, (DWORD), &D2Common_10600);
FUNC(BYTE, PASCAL, GetItemPage, (LPUNIT), &D2Common_10719);
FUNC(DWORD, PASCAL, GetItemCode, (LPUNIT), &D2Common_10746);
FUNC(LPUNIT, PASCAL, UnitIsItem, (LPUNIT), &D2Common_10305);
FUNC(DWORD, PASCAL, GetStat, (LPUNIT, DWORD, DWORD), &D2Common_10519);

WPTable *FASTCALL GetWaypointFromLevelID (int LevelID) {
	for (DWORD i=0; i<WP_entries; i++) {
		if (WP[i].Invalid) {
			continue;
		}
		if (WP[i].LevelID==LevelID) {
			return &WP[i];
		}
	}
	// Cannot find LevelID = no waypt
	return NULL;
}

DWORD FASTCALL GetWayptIDfromLevelID (int LevelID) {
	WPTable *tWP = GetWaypointFromLevelID(LevelID);
	if(tWP == NULL) {
		// Cannot find LevelID = no waypt
		return -1;
	}
	return tWP->WayptID;
}

LPUNIT CheckItemPresence(LPPLAYER ptPlayer, BYTE gameDiff, DWORD itemCode) {
	Inventory* ptInventory = ptPlayer->ptInventory;
	if(ptInventory==NULL) {
		D2LogMsg("CheckItemPresence: No Inventory");
		return NULL;
	}

	LPUNIT cursorItem = GetCursorItem(ptInventory);

	if(cursorItem != NULL) {
		DWORD code = GetItemCode(cursorItem);

		if(code == itemCode) {
			BYTE *ptItemRecord = GetItemRecord(cursorItem->nUnitId);

			if(ptItemRecord == NULL) {
				D2LogMsg("CheckItemPresence for %x: Found (cursor), no item record", itemCode);
				return cursorItem;
			}

			if(ptItemRecord[0x12a]==0) {
				D2LogMsg("CheckItemPresence for %x: Found (cursor), not quest item 1", itemCode);
				return cursorItem;
			}

			DWORD itemQuestItemDiff = GetStat(cursorItem, STATS_QUESTITEMDIFFICULTY, 0);
			if(itemQuestItemDiff <= gameDiff) {
				D2LogMsg("CheckItemPresence for %x: Matching (cursor) quest item", itemCode);
				return cursorItem;
			}
		}
	}
	for(LPUNIT ptUnit = GetFirstInventoryItem(ptInventory); ptUnit != NULL; ptUnit = GetNextInventoryItem(ptUnit)) {
		LPUNIT itemUnit = UnitIsItem(ptUnit);

		DWORD code = GetItemCode(itemUnit);

		if(code == itemCode) {
			BYTE page = GetItemPage(itemUnit);

			if(page != PAGE_CORPSE && page != PAGE_STASH) {
				DWORD nUnitId = ~0;
				if(itemUnit!=NULL) {
					nUnitId = itemUnit->nUnitId;
				}
				BYTE *ptItemRecord = GetItemRecord(nUnitId);

				if(ptItemRecord == NULL) {
					D2LogMsg("CheckItemPresence for %x: Found, no item record", itemCode);
					return itemUnit;
				}

				if(ptItemRecord[0x12a]==0) {
					D2LogMsg("CheckItemPresence for %x: Found, not quest item 1", itemCode);
					return itemUnit;
				}

				if(ptItemRecord[0x12b]==0) {
					D2LogMsg("CheckItemPresence for %x: Found, not quest item 2", itemCode);
					return itemUnit;
				}

				DWORD itemQuestItemDiff = GetStat(itemUnit, STATS_QUESTITEMDIFFICULTY, 0);
				if(itemQuestItemDiff <= gameDiff) {
					D2LogMsg("CheckItemPresence for %x: Matching quest item", itemCode);
					return itemUnit;
				}
			}
		}
	}
	D2LogMsg("CheckItemPresence for %x: Not Found", itemCode);
	return NULL;
}


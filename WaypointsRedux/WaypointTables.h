#pragma pack(1)

typedef struct {
	// Validator
	bool  Invalid;
	// Basics
	WORD  LevelID;
	BYTE  WayptID;
	BYTE  Page;
	BYTE  Tab;
	BYTE  Row;
	DWORD Header;
	// Difficulties
	BYTE  Diff[3];
	// Defining Booleans
	BYTE  Hidden;		// Will not be displayed unless OnlyFrom & KeyItem are satisfied
	BYTE  Disable;		// If Reqs are not fulfilled, this waypt is disabled
	BYTE  OneWay;		// ->
	// Specific Entrance/Exit
	BYTE  OnlyFrom[5];	// Entrance only *
	BYTE  OnlyTo[5];	// Exit only *
	// Tolls
	DWORD EnFee[3];
	DWORD ExFee[3];
	// KeyItems
	// If Active and !NeedKeyItem, no more KeyItem check
	// If !Active, KeyItem Check
	DWORD KeyItem[3];
	BYTE  KeyItemEquipped[3];
	BYTE  KeyItemCarried[3];
	BYTE  NeedKeyItem[3];
	BYTE  KillKeyItem[3];
	// Reqs
	short PenStat[3];
	int   PenVal[3];
	DWORD LvlReq[3];
	short Stat[3][3];
	int   Val[3][3];
	DWORD ReqItem[3];
	BYTE  ReqItemEquipped[3];
	BYTE  ReqItemCarried[3];
	BYTE  KillReqItem[3];
} WPTable;

struct LWTable {
	// Validator
	bool  Invalid;
	// Basics
	WORD  LevelFrom;
	WORD  LevelTo;
	// Difficulties
	BYTE  Diff[3];
	// Tolls
	DWORD Toll[3];
	// KeyItems
	// If Active and !NeedKeyItem, no more KeyItem check
	// If !Active, KeyItem Check
	DWORD KeyItem[3];
	BYTE  KeyItemEquipped[3];
	BYTE  KeyItemCarried[3];
	BYTE  NeedKeyItem[3];
	BYTE  KillKeyItem[3];
	// Reqs
	BYTE  Disable;		// If Reqs are not fulfilled, this waypt is disabled
	short PenStat[3];
	int   PenVal[3];
	DWORD LvlReq[3];
	short Stat[3][3];
	int   Val[3][3];
	DWORD ReqItem[3];
	BYTE  ReqItemEquipped[3];
	BYTE  ReqItemCarried[3];
	BYTE  KillReqItem[3];
};

typedef struct {
	DWORD Id;
	DWORD SelectX;
	DWORD SelectY;
	DWORD SelectDX;
	DWORD SelectDY;
	DWORD ExitWalkX;
	DWORD ExitWalkY;
	DWORD OffsetX;
	DWORD OffsetY;
	DWORD LitVersion;
	DWORD Tiles;
	char Direction[1];
}D2LvlWarpTXT;

#pragma pack()


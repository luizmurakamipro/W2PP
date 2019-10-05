/*
*   Copyright (C) {2015}  {Victor Klafke, Charles TheHouse}
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see [http://www.gnu.org/licenses/].
*
*   Contact at: victor.klafke@ecomp.ufsm.br
*/
#ifndef __SERVER_H__
#define	__SERVER_H__

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h> 
#include <fcntl.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <string.h>        
#include <time.h>

#include "..\Basedef.h"
#include "..\CPSock.h"

#include "CItem.h"
#include "CMob.h"
#include "CUser.h"
#include "CNPCGene.h"


// Externs
extern HWND hWndMain;

extern int ServerGroup;

extern unsigned int CurrentTime;
extern int BrState;

extern unsigned short g_pGuildWar[65536];
extern unsigned short g_pGuildAlly[65536];

extern unsigned short pMobGrid  [MAX_GRIDY][MAX_GRIDX];
extern unsigned short pItemGrid [MAX_GRIDY][MAX_GRIDX];
extern char pHeightGrid [MAX_GRIDY][MAX_GRIDX];

extern int CurrentWeather;

extern int GuildImpostoID[MAX_GUILDZONE];

extern CNPCGenerator mNPCGen;

extern CUser pUser[MAX_USER];
extern STRUCT_BLOCKMAC pMac[MAX_MAC];
extern int BigCubo;

// Funcs

void  ClearCrown(int conn);
BOOL  AddCrackError(int conn, int val, int Type);
BOOL  PutItem(int conn, STRUCT_ITEM *item);
void  GuildZoneReport();
HFONT__ *  GetAFont();
void  ReadConfig();
void  DrawConfig(int wb);
void  AddFailAccount(char *Account);
int   CheckFailAccount(char *Account);
int   IsFree(STRUCT_SELCHAR *sel);
int   SendBilling(int a1, char *a2, int a3, int a4);
int   SendBilling2(_AUTH_GAME *m, int hostlong);
void  SetItemBonus(STRUCT_ITEM *Dest, int Level, int a3, int DropBonus);
int   Challange(int conn, int mob, int coin);
int   GenerateSummon(int conn, int SummonID, STRUCT_ITEM *sItem, int Num);
int   GenerateEvocation(int conn, int SummonID, STRUCT_ITEM *sItem, int Num);
int   CreateMob(char *MobName, int PosX, int PosY, char *folder, int Type); 
void  GenerateMob(int index, int PosX, int PosY);
void  RebuildGenerator();
BOOL  InitApplication(HANDLE hInstance);
BOOL  InitInstance(HANDLE hInstance, int nCmdShow);
FILE* WriteHeader(char *fileName);
void  WriteArmor(FILE *fp);
void  WriteWeapon(FILE *fp);
void  BuildList();
LONG  APIENTRY MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);
int   GetUserInArea(int x1, int y1, int x2, int y2, char *MobName);
void  MountProcess(int conn, STRUCT_ITEM *Mount);
void  LinkMountHp(int mobConn);
void  ProcessAdultMount(int conn, int HpLost);
void  ProcessBILLMessage(char *pMsg);
void  ProcessDBMessage(char *Msg);
void  CheckIdle(int conn); 
void  FinishCastleWar();
void  RegenMob(int conn);
int   ProcessAffect(int conn);
void  SendDamage(int x1, int y1, int x2, int y2);
void  ProcessSecTimer();
void  ProcessMinTimer();
void  SummonGuild2(int Guild, int tx, int ty, int maxsummon, int village);
void  SummonGuild(int Guild, int tx, int ty, int maxsummon, int village);
void  ClearArea(int x1, int y1, int x2, int y2);
void  ClearAreaLevel(int x1, int y1, int x2, int y2, int minLv, int maxLv);
void  ClearAreaGuild(int x1, int y1, int x2, int y2, int Guild);
void  ClearAreaTeleport(int x1, int y1, int x2, int y2, int tx, int ty);
void  ClearGuildPKZone();
void  SetBigCuboDoor(int state);
void  SetColoseumDoor(int state); 
void  SetColoseumDoor2(int state);
void  GenerateColoseum(int InitID);
void  DeleteColoseum();
void  SetArenaDoor(int state);
void  SetCastleDoor(int state);
void  DecideWinner();
void  GuildProcess(); 
void  StartLog();
void  CrackLog(int conn, char *str);
void  Log(char *str1, char *str2, unsigned int ip);
int   GetUserFromSocket(int Sock);
int   GetEmptyUser();
int   GetEmptyItem();
int   GetEmptyNPCMob();
int   GetUserByName(char *name);
int   IsImple(int user);
void  CloseUser(int conn);
CMob *GetMobFromIndex(unsigned int index);
int   GetAngle(int x, int y);
int   GetLength(int x, int y);
int   ReadMob(STRUCT_MOB *mob, char *dir);
void  DeleteMob(int conn, int Type);
void  SaveUser(int conn,int Export);
void  CharLogOut(int conn);
void  Reboot();  
void  ClearChallanger(); 
void  SetBattle(int mob, int target);      
void  MobKilled(int conn, int target, int PosX, int PosY); 
int   CreateItem(int x, int y, STRUCT_ITEM *item, int rotate, int Create);
void  RemoveTrade(int conn);
void  RemoveParty(int conn);
void  RemoveEvocation(int conn);
int   UpdateItem(int Gate, int state, int *height);
void  DoRecall(int conn);  
void  DoWar(int myguild, int target);     
void  DoAlly(int myguild, int target);       
void  DoDeprivate(int conn, int target);     
void  ProcessRanking();    
void  DoRanking(int tDuel, int conn, int enemy);   
void  DoRemoveHide(int conn);  
void  DoRemoveAbsoluteProtection(int conn);
void  DoRemoveAllBuffs(int conn);
void  DoRemoveMutation(int conn);
void  DoTeleport(int mob, int x, int y);     
void  DoSummon(int target, int x, int y);       
int   SetAffect(int conn, int skillnum, int time, int Level);
int   SetTick(int mob, int skillnum, int Delay, int Level); 
void  ProcessDecayItem();
int   CombineTreasureMap(int conn);
void  Combine(int conn, int item, int PrizeItem);
int   ApplyHp(int conn); 
int   ApplyMp(int conn); 
void  SetReqHp(int conn); 
void  SetReqMp(int conn); 
void  ClearMapa(int x, int y);
void  DeleteMobMapa(int x, int y);
void  SetItemBonus2(STRUCT_ITEM *Dest);
void  DoRemoveSamaritano(int conn);
void  SaveAll();
void  ReadLevelItemConfig(void);
void  DoItemLevel(int conn);
void  SetCircletSubGod(int conn);
void  SummonServer(int tx, int ty);
void  DoGreatTeleport(int mob, int x, int y);
void  SendDamageKingdom(int x1, int y1, int x2, int y2, int Clan);
void  SendDamageLeader(int x1, int y1, int x2, int y2);
void  MobAttack(int attacker, MSG_Attack sm);
void  StartChatLog();
void  ChatLog(char *str1, char *str2, unsigned int ip);
void  StartItemLog();
void  ItemLog(char *str1, char *str2, unsigned int ip);
//int   ParseCastleString(int Num, char *str);
int   ParseMobMercString(int Num, char *str);
//int   ParseCampString(int Num, char *str);
void  DeleteGenerateMob(int generate);
int   CreateTreasureBox(int x, int y, STRUCT_ITEM *item, int rotate, int State);
void  ClearAreaQuest(int x1, int y1, int x2, int y2);
void  ClearMobArea(int x1, int y1, int x2, int y2);
void  RefinarItemMais(STRUCT_ITEM *item, int value);
//void  ItemRefinar(STRUCT_ITEM *item, int value);
int   GetItemSanc(STRUCT_ITEM *item);
bool  CheckArchItem(int id);
bool  CheckHardItem(int id);
void  AmountMinus(STRUCT_ITEM *item);
bool  CheckMount(STRUCT_ITEM *Item, INT16 Mount);
bool  CheckMount2(STRUCT_ITEM *Item, INT16 Mount);
void  SetItemSanc(STRUCT_ITEM *item, int value);
bool  PutAddOnItem(STRUCT_ITEM *Item, UINT8 ef1, UINT8 ef2, UINT8 efv1, UINT8 efv2);
/*INT64 SystemTimeDifference(const SYSTEMTIME time1, const SYSTEMTIME time2);
bool  FileExist(const char* Name );
bool  CheckBan(const char *login);
void  SendBan(const char *login);
void  SendBan3Hours(unsigned int ip);
bool  CheckBan3Hours(unsigned int ip);
bool  Check3HoursAccBan(const char *login);
void  Send3HoursAccBan(const char *login);
bool  CheckMacBan(const int *mac0, const int *mac1, const int *mac2, const int *mac3);
void  SendMacBan(const int *mac0, const int *mac1, const int *mac2, const int *mac3);
void  SendIpBan(unsigned int ip);
bool  CheckIpBan(unsigned int ip);
void  ReadArquivo(void);
void  DoItemArquivo(int conn);*/
bool ReadQuiz();
bool ReadCuboQuiz();
BOOL PutItemArea(STRUCT_ITEM *item, int x1, int y1, int x2, int y2);
void ReadDirectory();
//bool LoadCuboCoord();
//bool LoadCubo();
// External func

void ProcessImple(int conn, int level, char * str);

//Externs

extern  int FREEEXP;

extern int ServerDown;

extern int g_EmptyMob;

extern int ServerIndex;
extern int Sapphire;
extern int ServerGroup;
extern int GuildCounter;
extern bool TarantulaKilled;
extern bool MsgArea;
extern bool MsgArea2;

extern int ValidGuild ;
extern int WaterScrollPosition[3][10][2];

extern short weapon[32][9];

extern CPSock BillServerSocket;
extern char   BillServerAddress[32];
extern int	   BillServerPort;

extern HWND  hWndMain;
extern HMENU hMainMenu;
extern HFONT hFont;

extern FILE *fLogFile;
extern FILE *fChatLogFile;
extern FILE *fItemLogFile;
extern FILE *fReadDir;

extern CPSock DBServerSocket;

extern CNPCGenerator mNPCGen;
extern CNPCSummon mSummon;

extern CUser pUser[MAX_USER];
extern CMob pMob[MAX_MOB];

extern STRUCT_TREASURE g_pTreasure[8];
extern STRUCT_ITEM LevelItem[4][4][400];

extern CItem pItem[MAX_ITEM];

extern unsigned int CurrentTime;
extern unsigned int LastSendTime;


extern int BillCounter;

extern int	g_Week;
extern int	g_Hour;

extern int LastLogDay;
extern int LastChatLogDay;
extern int LastItemLogDay;
extern int maxNightmare;
extern int maxPortaoInfernal;

extern int ConfigReady;

///////////////////////////////////////
//Config for Event
//////////////////////////////////////
extern int evNotice;
extern int evStartIndex;
extern int evEndIndex;
extern int evCurrentIndex;
extern int evRate;
extern int evItem;
extern int evIndex;
extern int evOn;
extern int evDelete;
////////////////////////////////////

extern int NewbieEventServer;
extern int CastleServer;

extern int DOUBLEMODE;
extern int DUNGEONEVENT;
extern int DEADPOINT;
extern int StatSapphire;
extern int BRItem;

extern int BILLING;
extern int CHARSELBILL;
extern int POTIONCOUNT;
extern int PARTYBONUS;
extern int GUILDBOARD;

extern int UserCount;
extern int SaveCount;
extern int ItemCount;

extern int ForceWeather;

extern int WeekMode;
extern int ForceWeekMode;
extern int GuildDay;
extern int GuildHour;

extern int NumGuildBattle;
extern int PotionReady;
extern int PotionDelay;
extern int NewbieHour;

extern int RvRState;

extern int NumServerInGroup;

extern int LiveTower[3];

extern int pChallangerMoney[6];

extern unsigned short g_pGuildWar[65536];
extern unsigned short g_pGuildAlly[65536];

extern unsigned short pMobGrid[MAX_GRIDY][MAX_GRIDX];
extern unsigned short pItemGrid[MAX_GRIDY][MAX_GRIDX];
extern char pHeightGrid[MAX_GRIDY][MAX_GRIDX];

extern unsigned char LocalIP[4];

extern char FailAccount[16][16];

extern CPSock ListenSocket;
extern unsigned int pAdminIP[MAX_ADMIN];

extern char DBServerAddress[32];
extern int DBServerPort;

extern QuizInfo SendQuiz[1000];
extern char QuizLineCounter;
extern MSG_Quiz QuizList[50];

extern MSG_BigQuiz CuboList[50];
extern char CuboQuizLine;

extern int CurrentWeather;
extern int TESTSERVER;
extern int LOCALSERVER;

extern int TransperCharacter;

extern unsigned int SecCounter;
extern unsigned int MinCounter;

extern int GuildImpostoID[MAX_GUILDZONE];

extern int BrState;
extern int BrMod;
extern int BrGrid;
extern int CastleState;
extern int Colo150Limit;
extern int KefraLive;
extern int EspelhoLive;
extern int RandBoss;
extern int Lich_1;
extern int Lich_2;
extern int Lich_3;
extern int Lich_4;
extern char KefraKiller[32];

extern int g_dLevel;
extern int g_dLevel1;
extern int g_dLevel2;

extern int Kingdom1Clear;
extern int Kingdom2Clear;
extern int PARTY_DIF;
extern int isDropItem;
extern int isHardCore;
extern int RvRHour;

///////////////////////////////////////
//Config for Duel: 1:1 5:5 10:10 All:All
//////////////////////////////////////
extern char nRanking1[32];
extern char nRanking2[32];
extern char nTitle1[32];
extern char nTitle2[32];

extern int RankingProgress;
extern int Ranking1;
extern int Ranking2;
extern int lTitle1;
extern int lTitle2;
extern int RankingTime;

extern int Ranking1X;
extern int Ranking1Y;
extern int Ranking2X;
extern int Ranking2Y;

extern int cRanking1X;
extern int cRanking1Y;
extern int cRanking2X;
extern int cRanking2Y;

extern int kRanking1X;
extern int kRanking1Y;
extern int kRanking2X;
extern int kRanking2Y;

extern char temp[4096];

extern int TaxChanged[MAX_GUILDZONE];


//////////////////////////////////////

extern int WaterClear1[3][10];
extern int PartyPesa[3];
extern int PartyPortao[3];
extern int PesaNPosStandard[13][2];
extern int PesaMPosStandard[13][2];
extern int PesaAPosStandard[13][2];
extern int CartaPos[4][2];
extern int PistaPos[7][3][2];
extern int TarantulaPos[2][2][2];
extern int CTarantulaPos[4][2];
extern int CartaTime;
extern int CartaSala;

extern STRUCT_PISTA Pista[7];
extern STRUCT_MERC pMobMerc[MAX_MOB_MERC];

extern int PistaRune[7][10];
extern int PistaBalrogPos[5][2];
extern int PistaBalrogPortalPos[5][5][2];
extern int RvRBonus;

extern int GTorreState;
extern int GTorreHour;
extern int GTorreGuild;

extern int RvRBluePoint;
extern int RvRRedPoint;

extern char ACCOUNT_DIRECTORY[MESSAGECHAT_LENGTH];

/////////////////////////////////////
//Externs

static const INT16 MountCheck[7][4] =
{
	{
		2363, 2364, 2365, 0
	},
	{
		2367, 2368, 2372, 2373
	},
	{
		2369, 2370, 2374, 2375
	},
	{
		2376, 2377, 2378, 0
	},
	{
		2381, 2382, 2383, 0
	},
	{
		2384, 2385, 2386, 0
	},
	{
		2379, 2380, 2387, 2388
	}
};

static const INT16 MountCheck2[7][4] =
{
	{
		2333, 2334, 2335, 0
	},
	{
		2337, 2338, 2342, 2343
	},
	{
		2339, 2340, 2344, 2345
	},
	{
		2346, 2347, 2348, 0
	},
	{
		2351, 2352, 2353, 0
	},
	{
		2354, 2355, 2356, 0
	},
	{
		2349, 2350, 2357, 2358
	}
};

static unsigned char ref[11] = { 43, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125 };

extern STRUCT_GUILDZONE g_pGuildZone[MAX_GUILDZONE];
#endif
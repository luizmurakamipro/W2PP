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
#include "ProcessClientMessage.h"
#include "Server.h"

void Exec_MSG_MessageWhisper(int conn, char *pMsg)
{
	MSG_MessageWhisper *m = (MSG_MessageWhisper*)pMsg;

	int Clan = pMob[conn].MOB.Clan;

	m->MobName[NAME_LENGTH - 1] = 0;
	m->String[MESSAGEWHISPER_LENGTH - 1] = 0;

	if (pUser[conn].Mode != USER_PLAY)
		return;

	if (strcmp(m->MobName, "tt") == 0)
	{
		sprintf(temp, "%d %d %d %d %d %d %d %d %d %d %d %d", pMob[conn].PartyList[0], pMob[conn].PartyList[1], pMob[conn].PartyList[2], pMob[conn].PartyList[3], pMob[conn].PartyList[4], pMob[conn].PartyList[5], pMob[conn].PartyList[6], pMob[conn].PartyList[7], pMob[conn].PartyList[8], pMob[conn].PartyList[9], pMob[conn].PartyList[10], pMob[conn].PartyList[11]);
		SendClientMessage(conn, temp);
		return;
	}

	if (strcmp(m->MobName, "ss") == 0)
	{
		int idx = GetUserByName(m->String), i, Find = 0, NewLeader;

		if (idx >= MAX_USER)
			return;

		if (pMob[conn].Leader)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Party_Leader_Only]);
			return;
		}

		if (pUser[idx].Mode != USER_PLAY)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		for (int i = 0; i < MAX_PARTY; i++)
		{
			int partyconn = pMob[conn].PartyList[i];

			if (partyconn == idx && partyconn != 0 && partyconn < MAX_USER)
			{
				NewLeader = idx;
				Find = TRUE;
				break;
			}
		}

		if (!Find)
		{
			SendClientMessage(conn, "Este jogador não pertence ao seu grupo.");
			return;
		}

		pMob[conn].Leader = 0;
		pMob[idx].Leader = 0;

		for (int i = 0; i < MAX_PARTY; i++)
		{
			int partyconn = pMob[conn].PartyList[i];

			if (partyconn > 0 && partyconn <= MAX_MOB)
			{
				pMob[conn].PartyList[i] = 0;
				pMob[partyconn].Leader = 0;

				if (NewLeader != 0 && NewLeader != partyconn && partyconn < MAX_USER && NewLeader < MAX_USER)
				{
					MSG_AcceptParty sm;
					memset(&sm, 0, sizeof(MSG_AcceptParty));

					sm.Size = sizeof(MSG_AcceptParty);
					sm.Type = _MSG_AcceptParty;
					sm.ID = 0;

					sm.LeaderID = NewLeader;

					strncpy(sm.MobName, pMob[NewLeader].MOB.MobName, NAME_LENGTH);

					pMob[partyconn].LastReqParty = NewLeader;

					ProcessClientMessage(partyconn, (char*)&sm, TRUE);
				}
			}
		}

		if (NewLeader != 0 && NewLeader != conn && conn < MAX_USER && NewLeader < MAX_USER)
		{
			MSG_AcceptParty sm;
			memset(&sm, 0, sizeof(MSG_AcceptParty));

			sm.Size = sizeof(MSG_AcceptParty);
			sm.Type = _MSG_AcceptParty;
			sm.ID = 0;

			sm.LeaderID = NewLeader;

			strncpy(sm.MobName, pMob[NewLeader].MOB.MobName, NAME_LENGTH);

			pMob[conn].LastReqParty = NewLeader;

			ProcessClientMessage(conn, (char*)&sm, TRUE);
		}

		return;
	}


#pragma region /cp
	if (strcmp(m->MobName, "cp") == 0)
	{
		sprintf(temp, g_pMessageStringTable[_DN_Show_Chao], GetPKPoint(conn) - 75);
		SendClientMessage(conn, temp);
		return;
	}
#pragma endregion
#pragma region Jokenpô
	if (strcmp(m->MobName, "_RPS_") == 0)
	{
		if (pMob[conn].Joken[0] == ' ')
			return;

		char tmg[256];

		if (strcmp(m->String, "rock") == 0)
		{
			if (strcmp(pMob[conn].Joken, "rock") == 0)
			{
				sprintf(tmg, "Empatou! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
				SendClientMessage(conn, tmg);
				pMob[conn].Joken[0] = ' ';
				return;
			}
			else
			{
				if (strcmp(pMob[conn].Joken, "paper") == 0)
				{
					sprintf(tmg, "Perdeu! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
					SendClientMessage(conn, tmg);
					pMob[conn].Joken[0] = ' ';
					return;
				}
				else
				{
					sprintf(tmg, "Ganhou! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
					SendClientMessage(conn, tmg);
					pMob[conn].Joken[0] = ' ';
					return;
				}
			}
		}

		if (strcmp(m->String, "paper") == 0)
		{
			if (strcmp(pMob[conn].Joken, "paper") == 0)
			{
				sprintf(tmg, "Empatou! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
				SendClientMessage(conn, tmg);
				pMob[conn].Joken[0] = ' ';
				return;
			}
			else
			{
				if (strcmp(pMob[conn].Joken, "scissor") == 0)
				{
					sprintf(tmg, "Perdeu! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
					SendClientMessage(conn, tmg);
					pMob[conn].Joken[0] = ' ';
					return;
				}
				else
				{
					sprintf(tmg, "Ganhou! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
					SendClientMessage(conn, tmg);
					pMob[conn].Joken[0] = ' ';
					return;
				}
			}
		}

		if (strcmp(m->String, "scissor") == 0)
		{
			if (strcmp(pMob[conn].Joken, "scissor") == 0)
			{
				sprintf(tmg, "Empatou! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
				SendClientMessage(conn, tmg);
				pMob[conn].Joken[0] = ' ';
				return;
			}
			else
			{
				if (strcmp(pMob[conn].Joken, "rock") == 0)
				{
					sprintf(tmg, "Perdeu! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
					SendClientMessage(conn, tmg);
					pMob[conn].Joken[0] = ' ';
					return;
				}
				else
				{
					sprintf(tmg, "Ganhou! [%s]: %s x %s :[Server]", pMob[conn].MOB.MobName, m->String, pMob[conn].Joken);
					SendClientMessage(conn, tmg);
					pMob[conn].Joken[0] = ' ';
					return;
				}
			}
		}
		return;
	}
#pragma endregion
#pragma region /myclass
	if (strcmp(m->MobName, "myclass") == 0)
	{
		if (pMob[conn].extra.ClassMaster == ARCH) {
			SendClientMessage(conn, "Arch");
			return;
		}
		else if (pMob[conn].extra.ClassMaster == MORTAL) {
			SendClientMessage(conn, "Mortal");
			return;
		}
		else if (pMob[conn].extra.ClassMaster == CELESTIAL) {
			SendClientMessage(conn, "Celestial");
			return;
		}
		else if (pMob[conn].extra.ClassMaster == CELESTIALCS) {
			SendClientMessage(conn, "CCelestial");
			return;
		}
		else if (pMob[conn].extra.ClassMaster == SCELESTIAL) {
			SendClientMessage(conn, "SubCelestial");
			return;
		}
		return;
	}
#pragma endregion
#pragma region /getout - Fim cidadão
	else if (strcmp(m->MobName, "getout") == 0)
	{
		pMob[conn].extra.Citizen = 0;
		SendClientMessage(conn, g_pMessageStringTable[_DN_NO_TOWNSPEOPLE]);

		sprintf(temp, "etc,getout %d ", pMob[conn].extra.Citizen);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);
		return;
	}
#pragma endregion
#pragma region /srv - Troca de Server
	else if (strcmp(m->MobName, "srv") == 0)
	{
		int srv = 0;

		sscanf(m->String, "%d", &srv);

		if (srv <= 0 || srv >= MAX_SERVER)
			return;

		if (srv == (ServerIndex + 1))
			return;

		if (srv > NumServerInGroup)
			return;

		MSG_DBServerChange sm_cs;
		memset(&sm_cs, 0, sizeof(MSG_DBServerChange));

		sm_cs.Size = sizeof(MSG_DBServerChange);
		sm_cs.Type = _MSG_DBServerChange;
		sm_cs.ID = conn;
		sm_cs.NewServerID = srv;
		sm_cs.Slot = pUser[conn].Slot;

		DBServerSocket.SendOneMessage((char*)&sm_cs, sizeof(MSG_DBServerChange));
		return;
	}
#pragma endregion	
#pragma region /gindex
	else if (strcmp(m->MobName, "gindex") == 0)
	{
		if (pMob[conn].MOB.Guild == 0 || pMob[conn].MOB.GuildLevel != 9)
			return;


		sprintf(temp, "Guild Index: %d", pMob[conn].MOB.Guild);
		SendClientMessage(conn, temp);
		return;
	}
#pragma endregion
#pragma region /spk - Chat para todos os servers
	else if (strcmp(m->MobName, "spk") == 0)
	{
		int i = 0;

		if (pUser[conn].MuteChat == 1)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_No_Speak]);
			return;
		}

		for (i = 0; i < pMob[conn].MaxCarry; i++)
		{
			if (pMob[conn].MOB.Carry[i].sIndex != 3330)
				continue;

			int amount = BASE_GetItemAmount(&pMob[conn].MOB.Carry[i]);

			if (amount > 1)
				BASE_SetItemAmount(&pMob[conn].MOB.Carry[i], amount - 1);
			else
				BASE_ClearItem(&pMob[conn].MOB.Carry[i]);

			SendItem(conn, ITEM_PLACE_CARRY, i, &pMob[conn].MOB.Carry[i]);
			break;
		}

		if (i == pMob[conn].MaxCarry)
			return;

		MSG_MagicTrumpet sm_mt;
		memset(&sm_mt, 0, sizeof(MSG_STANDARDPARM));

		sm_mt.Size = sizeof(MSG_MagicTrumpet);
		sm_mt.ID = 0;
		sm_mt.Type = _MSG_MagicTrumpet;

		sprintf(temp, "[%s]> %s", pMob[conn].MOB.MobName, m->String);

		strncpy(sm_mt.String, temp, 96);

		sm_mt.String[94] = 1;

		DBServerSocket.SendOneMessage((char*)&sm_mt, sizeof(MSG_MagicTrumpet));

		sprintf(temp, "chat_spk,%s %s", pMob[conn].MOB.MobName, sm_mt.String);
		ChatLog(temp, pUser[conn].AccountName, pUser[conn].IP);
		return;
	}
#pragma endregion
#pragma region /qst - QuestInfo
	else if (strcmp(m->MobName, "qst") == 0)
	{
		if (pMob[conn].extra.QuestInfo.Arch.Cristal == 0)
			sprintf(temp, g_pMessageStringTable[_NN_NoHave_Quest]);

		else if (pMob[conn].extra.QuestInfo.Arch.Cristal == 1)
			sprintf(temp, g_pMessageStringTable[_DN_Play_Quest], "Contrato de Elime");

		else if (pMob[conn].extra.QuestInfo.Arch.Cristal == 2)
			sprintf(temp, g_pMessageStringTable[_DN_Play_Quest], "Contrato de Sylphed");

		else if (pMob[conn].extra.QuestInfo.Arch.Cristal == 3)
			sprintf(temp, g_pMessageStringTable[_DN_Play_Quest], "Contrato de Thelion");

		else if (pMob[conn].extra.QuestInfo.Arch.Cristal == 4)
			sprintf(temp, g_pMessageStringTable[_DN_Play_Quest], "Contrato de Noas");

		SendClientMessage(conn, temp);
		return;
	}
#pragma endregion
#pragma region /guildmsg
	else if (strcmp(m->MobName, "guildmsg") == 0)
	{
		if (pMob[conn].MOB.Guild != 0)
		{
			if (pMob[conn].MOB.GuildLevel == 9)
			{
				if (m->String[0] == NULL)
				{
					memset(AvisarGuild[pMob[conn].MOB.Guild].Aviso, 0, sizeof(AvisarGuild[pMob[conn].MOB.Guild].Aviso));
				}
				else
					sscanf_s(m->String, "%120[^\0]", AvisarGuild[pMob[conn].MOB.Guild].Aviso, _countof(AvisarGuild[pMob[conn].MOB.Guild].Aviso));
				SendClientMessage(conn, "Mensagem definida com sucesso.");
				return;
			}
			else
			{
				SendClientMessage(conn, "Você não é Lider de sua Guild.");
				return;
			}
		}
		else
		{
			SendClientMessage(conn, "Você não é membro de nenhuma Guild.");
			return;
		}
	}
#pragma endregion 
#pragma region /criar
	else if (strcmp(m->MobName, "create") == 0)
	{
		if (m->String[0] == 0)
			return;

		if (pMob[conn].MOB.Coin < 100000000)
		{
			sprintf(temp, g_pMessageStringTable[_DN_D_Cost], 100000000);
			SendClientMessage(conn, temp);
			return;
		}

		if (pMob[conn].MOB.Guild != 0)
			return;

		if (pMob[conn].MOB.Clan != 7 && pMob[conn].MOB.Clan != 8)
		{
			SendClientMessage(conn, "Membros White não podem criar guild.");
			return;
		}

		if (GuildCounter >= 4096)
			return;

		if (pMob[conn].extra.Citizen == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_DN_NO_TOWNSPEOPLE]);
			return;
		}

		if (ServerIndex == -1)
		{
			Log("err,Buy Guild item, but, Server index undefined", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		if (GuildCounter == 0)
		{
			Log("err,Buy Guild item, but, Guild counter zero", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pMob[conn].MOB.Guild = ServerIndex * MAX_GUILD + GuildCounter;
		pMob[conn].MOB.GuildLevel = 9;

		pMob[conn].MOB.Coin -= 100000000;
		SendEtc(conn);

		char szName[GUILDNAME_LENGTH];

		memset(szName, 0, GUILDNAME_LENGTH);

		strncpy(szName, m->String, GUILDNAME_LENGTH);

		FILE *fp = fopen("../../Common/Guilds.txt", "a+");

		if (fp == NULL)
		{
			Log("err,Buy Guild item, but, no open file undefined", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		fprintf(fp, "\n%d %d %d %s   ", ServerGroup, ServerIndex, GuildCounter, szName);

		fclose(fp);

		int Group = ServerGroup;
		int Server = pMob[conn].MOB.Guild / MAX_GUILD;
		int usGuild = pMob[conn].MOB.Guild;

		MSG_GuildInfo sm2;
		memset(&sm2, 0, sizeof(MSG_GuildInfo));

		sm2.Type = _MSG_GuildInfo;
		sm2.Size = sizeof(MSG_GuildInfo);
		sm2.ID = conn;

		sm2.Guild = usGuild;

		GuildInfo[usGuild].Clan = pMob[conn].MOB.Clan;
		GuildInfo[usGuild].Fame = 0;
		GuildInfo[usGuild].Citizen = pMob[conn].extra.Citizen;


		sm2.GuildInfo = GuildInfo[usGuild];

		DBServerSocket.SendOneMessage((char*)&sm2, sizeof(MSG_GuildInfo));

		int GCount = GuildCounter + (ServerIndex << 12);

		sprintf(temp, "sys,guild medal value:%d count:%d", GCount, GuildCounter);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);

		GuildCounter++;

		CReadFiles::WriteGuild();

		BASE_InitializeGuildName();

		sprintf(temp, g_pMessageStringTable[_SN_CREATEGUILD], szName);
		SendClientMessage(conn, temp);
		return;
	}
#pragma endregion
#pragma region /subcreate
	else if (strcmp(m->MobName, "subcreate") == 0)
	{
		if (pMob[conn].MOB.Guild == 0)
			return;

		char SubMobName[256];
		char SubName[256];

		sscanf(m->String, "%s %s", SubMobName, SubName);

		char sub[16];

		strncpy(sub, SubMobName, 16);

		int user = GetUserByName(sub);

		if (user == conn)
			return;

		if (user == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (pMob[user].MOB.Guild != pMob[conn].MOB.Guild)
			return;

		if (pMob[conn].MOB.GuildLevel != 9)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Only_Guild_Master_can]);
			return;
		}

		if (pMob[user].MOB.GuildLevel != 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Haven_Guild_Medal]);
			return;
		}

		int Guild = pMob[conn].MOB.Guild;

		if (GuildInfo[Guild].Sub1 != 0 && GuildInfo[Guild].Sub2 != 0 && GuildInfo[Guild].Sub3 != 0)
			return;


		if (pMob[conn].MOB.Coin < 100000000)
		{
			sprintf(temp, g_pMessageStringTable[_DN_D_Cost], 100000000);
			SendClientMessage(conn, temp);
			return;
		}

		pMob[conn].MOB.Coin -= 100000000;

		if (GuildInfo[Guild].Sub1 == 0)
		{
			pMob[user].MOB.GuildLevel = 6;
			GuildInfo[Guild].Sub1 = 1;
		}
		else if (GuildInfo[Guild].Sub2 == 0)
		{
			pMob[user].MOB.GuildLevel = 7;
			GuildInfo[Guild].Sub2 = 1;
		}
		else if (GuildInfo[Guild].Sub3 == 0)
		{
			pMob[user].MOB.GuildLevel = 8;
			GuildInfo[Guild].Sub3 = 1;
		}

		sprintf(temp, "etc,subcreate %s %d ", SubName, Guild);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);

		MSG_GuildInfo sm_gi;
		memset(&sm_gi, 0, sizeof(MSG_GuildInfo));

		sm_gi.Type = _MSG_GuildInfo;
		sm_gi.Size = sizeof(MSG_GuildInfo);
		sm_gi.ID = conn;

		sm_gi.Guild = Guild;
		sm_gi.GuildInfo = GuildInfo[Guild];

		DBServerSocket.SendOneMessage((char*)&sm_gi, sizeof(MSG_GuildInfo));

		MSG_CreateMob sm2;
		memset(&sm2, 0, sizeof(MSG_CreateMob));
		GetCreateMob(conn, &sm2);

		GridMulticast(pMob[conn].TargetX, pMob[conn].TargetY, (MSG_STANDARD*)&sm2, 0);
		return;
	}
#pragma endregion
#pragma region /expulsar
	else if (strcmp(m->MobName, "abandonar") == 0)
	{
		if (pMob[conn].MOB.Guild == 0)
			return;

		sprintf(temp, "etc,abandonar %d %d", pMob[conn].MOB.Guild, pMob[conn].MOB.GuildLevel);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);

		int gGuild = pMob[conn].MOB.Guild;

		if (pMob[conn].MOB.GuildLevel >= 6 && pMob[conn].MOB.GuildLevel <= 8)
		{
			if (GuildInfo[gGuild].Sub1 != 0)
				GuildInfo[gGuild].Sub1 = 0;
			else if (GuildInfo[gGuild].Sub2 != 0)
				GuildInfo[gGuild].Sub2 = 0;

			else if (GuildInfo[gGuild].Sub3 != 0)
				GuildInfo[gGuild].Sub3 = 0;


			sprintf(temp, "etc,subdelete %s %d ", pMob[conn].MOB.MobName, gGuild);
			Log(temp, pUser[conn].AccountName, pUser[conn].IP);

			MSG_GuildInfo sm_gi;
			memset(&sm_gi, 0, sizeof(MSG_GuildInfo));

			sm_gi.Type = _MSG_GuildInfo;
			sm_gi.Size = sizeof(MSG_GuildInfo);
			sm_gi.ID = conn;

			sm_gi.Guild = gGuild;
			sm_gi.GuildInfo = GuildInfo[gGuild];

			DBServerSocket.SendOneMessage((char*)&sm_gi, sizeof(MSG_GuildInfo));
		}

		pMob[conn].MOB.Guild = 0;
		pMob[conn].MOB.GuildLevel = 0;

		MSG_CreateMob sm_eg;
		memset(&sm_eg, 0, sizeof(MSG_CreateMob));
		GetCreateMob(conn, &sm_eg);

		GridMulticast(pMob[conn].TargetX, pMob[conn].TargetY, (MSG_STANDARD*)&sm_eg, 0);
		return;
	}
#pragma endregion
#pragma region /transferir
	else if (strcmp(m->MobName, "handover") == 0)
	{
		if (pMob[conn].MOB.Guild == 0)
			return;

		if (pMob[conn].MOB.GuildLevel != 9)
			return;

		m->String[NAME_LENGTH - 1] = 0;
		m->String[NAME_LENGTH - 2] = 0;

		int target = GetUserByName(m->String);

		if (target == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (pUser[target].Mode != USER_PLAY)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (pMob[target].MOB.Guild != pMob[conn].MOB.Guild)
			return;

		char guildname[256];
		BASE_GetGuildName(ServerGroup, pMob[conn].MOB.Guild, guildname);
		sprintf(temp, "etc,handover guild:%d guildname:%s  Lider:%s -> NovoLider:%s ", pMob[conn].MOB.Guild, guildname, pMob[conn].MOB.MobName, pMob[target].MOB.MobName);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);

		pMob[target].MOB.GuildLevel = 9;
		pMob[conn].MOB.GuildLevel = 0;

		ClearCrown(conn);

		MSG_CreateMob sm_eg;
		memset(&sm_eg, 0, sizeof(MSG_CreateMob));
		GetCreateMob(conn, &sm_eg);

		GridMulticast(pMob[conn].TargetX, pMob[conn].TargetY, (MSG_STANDARD*)&sm_eg, 0);

		memset(&sm_eg, 0, sizeof(MSG_CreateMob));
		GetCreateMob(target, &sm_eg);

		GridMulticast(pMob[target].TargetX, pMob[target].TargetY, (MSG_STANDARD*)&sm_eg, 0);

		MSG_GuildInfo sm_gi;
		memset(&sm_gi, 0, sizeof(MSG_GuildInfo));

		sm_gi.Type = _MSG_GuildInfo;
		sm_gi.Size = sizeof(MSG_GuildInfo);
		sm_gi.ID = conn;

		sm_gi.Guild = pMob[conn].MOB.Guild;

		GuildInfo[pMob[conn].MOB.Guild].Fame = 0;

		sm_gi.GuildInfo = GuildInfo[pMob[conn].MOB.Guild];

		DBServerSocket.SendOneMessage((char*)&sm_gi, sizeof(MSG_GuildInfo));

		return;
	}
#pragma endregion
#pragma region /nt - NT
	else if (strcmp(m->MobName, "nt") == 0)
	{
		sprintf(temp, g_pMessageStringTable[_DN_CHANGE_COUNT], pMob[conn].extra.NT);

		SendClientMessage(conn, temp);
		return;
	}
#pragma endregion
#pragma region /nig - Pesadelo TIME
	else if (strcmp(m->MobName, "nig") == 0)
	{
		char tmptime[256];

		time_t mytime;

		time(&mytime);

		tm *timeinfo = localtime(&mytime);

		strftime(tmptime, 256, "!!%H%M%S", timeinfo);

		SendClientMessage(conn, tmptime);
		return;
	}
#pragma endregion
#pragma region /tab
	if (strcmp(m->MobName, "tab") == 0)
	{
		if (pMob[conn].MOB.CurrentScore.Level < 69 && pMob[conn].extra.ClassMaster == MORTAL)
		{
			sprintf(temp, g_pMessageStringTable[_DN_Level_Limit], 70);
			SendClientMessage(conn, temp);
			return;
		}

		strncpy(pMob[conn].Tab, m->String, 26);

		if (pUser[conn].TradeMode == 0)
		{
			MSG_CreateMob sm_tb;
			memset(&sm_tb, 0, sizeof(MSG_CreateMob));
			GetCreateMob(conn, &sm_tb);

			GridMulticast(pMob[conn].TargetX, pMob[conn].TargetY, (MSG_STANDARD*)&sm_tb, 0);
		}
		else
		{
			MSG_CreateMobTrade sm_tb;
			memset(&sm_tb, 0, sizeof(MSG_CreateMobTrade));
			GetCreateMobTrade(conn, &sm_tb);

			GridMulticast(pMob[conn].TargetX, pMob[conn].TargetY, (MSG_STANDARD*)&sm_tb, 0);
		}

		MSG_STANDARDPARM sm;
		memset(&sm, 0, sizeof(MSG_STANDARDPARM));

		sm.Size = sizeof(MSG_STANDARDPARM);
		sm.Type = _MSG_PKInfo;
		sm.ID = conn;

		if (NewbieEventServer == 0)
		{
			int guilty = GetGuilty(conn);

			int state = 0;

			if (guilty || pUser[conn].PKMode || RvRState && pMob[conn].TargetX >= 1023 && pMob[conn].TargetY >= 1919 && pMob[conn].TargetX <= 1280 && pMob[conn].TargetY <= 2179 || CastleState && pMob[conn].TargetX >= 1024 && pMob[conn].TargetY >= 1664 && pMob[conn].TargetX <= 1153 && pMob[conn].TargetY <= 1793 || GTorreState && pMob[conn].TargetX >= 2430 && pMob[conn].TargetY >= 1825 && pMob[conn].TargetX <= 2560 && pMob[conn].TargetY <= 1925)
				state = 1;

			sm.Parm = state;
		}
		else
			sm.Parm = 1;

		GridMulticast(pMob[conn].TargetX, pMob[conn].TargetY, (MSG_STANDARD*)&sm, 0);
		return;
	}
#pragma endregion
#pragma region /snd
	if (strcmp(m->MobName, "snd") == 0)
	{
		strncpy(pMob[conn].Snd, m->String, 96);

		sprintf(temp, "%s %s", g_pMessageStringTable[_NN_SND_MESSAGE], pMob[conn].Snd);

		SendClientMessage(conn, temp);

		return;
	}
#pragma endregion
#pragma region /day - Skill
	if (strcmp(m->MobName, "day") == 0)
	{
		SendClientMessage(conn, "!#11  2");

		return;
	}
#pragma endregion
#pragma region /odin
	if (strcmp(m->MobName, "odin") == 0)
	{
		DoTeleport(conn, 3237, 1691);
		return;
	}
#pragma endregion
#pragma region _NN_Kingdom - /kingdom
	if (strcmp(m->MobName, g_pMessageStringTable[_NN_Kingdom]) == 0 || strcmp(m->MobName, "Reino") == 0 || strcmp(m->MobName, "kingdom") == 0)
	{
		if (Clan == 7)
			DoTeleport(conn, 1690, 1618);
		else if (Clan == 8)
			DoTeleport(conn, 1690, 1842);
		else
			DoTeleport(conn, 1702, 1726);

		sprintf(temp, "etc,kingdom command %d ", Clan);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);
		return;
	}
#pragma endregion
#pragma region _NN_King  - /king
	else if (strcmp(m->MobName, g_pMessageStringTable[_NN_King]) == 0 || strcmp(m->MobName, "king") == 0 || strcmp(m->MobName, "rei") == 0)
	{
		if (Clan == 7)
			DoTeleport(conn, 1748, 1574);

		else if (Clan == 8)
			DoTeleport(conn, 1748, 1880);

		sprintf(temp, "etc,king command %d ", Clan);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);
		return;
	}
#pragma endregion
#pragma region _NN_Summon_Guild - /summonguild
	else if (strcmp(m->MobName, g_pMessageStringTable[_NN_Summon_Guild]) == 0 || strcmp(m->MobName, "summonguild") == 0)
	{
		int usGuild = pMob[conn].MOB.Guild;
		int usGuildLv = pMob[conn].MOB.GuildLevel;

		int xx = pMob[conn].TargetX;
		int yy = pMob[conn].TargetY;

		int village = BASE_GetVillage(xx, yy);

		unsigned char mapatt = GetAttribute(xx, yy);

		if (usGuildLv <= 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Only_Guild_Master_Can]);
			return;
		}

		int sub = BASE_GetSubGuild(pMob[conn].MOB.Equip[12].sIndex);

		if (village < 0 || village >= 5)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_That_Here]);
			return;
		}

		if (mapatt & 4 || mapatt & 0x40)
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_That_Here]);
		else
			SummonGuild2(usGuild, xx, yy, 350, sub);

		sprintf(temp, "etc,summonguild guild:%d X:%d Y:%d", usGuild, xx, yy);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);
		return;
	}
#pragma endregion
#pragma region _NN_Summon - /summon
	else if (strcmp(m->MobName, g_pMessageStringTable[_NN_Summon]) == 0 || strcmp(m->MobName, "summon") == 0)
	{
		int admin = 0;

		if (pMob[conn].MOB.CurrentScore.Level >= 1000)
			admin = 1;

		if (pMob[conn].MOB.CurrentScore.Hp <= 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_Killed]);
			return;
		}

		if (m->String[0] == 0)
			return;

		m->String[NAME_LENGTH - 1] = 0;
		m->String[NAME_LENGTH - 2] = 0;

		int target = GetUserByName(m->String);

		if (target == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (target == conn)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_To_Yourself]);
			return;
		}

		if (pUser[target].Mode != USER_PLAY)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		int myguild = pMob[conn].MOB.Guild;
		int targetguild = pMob[target].MOB.Guild;

		int myleader = pMob[conn].Leader;

		if (myleader <= 0)
			myleader = conn;

		int targetleader = pMob[target].Leader;

		if (targetleader <= 0)
			targetleader = target;

		if (pMob[target].MOB.CurrentScore.Hp <= 0 && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_To_Killed]);
			return;
		}

		if ((myguild == 0 && myleader == 0 || myleader != targetleader || myguild != targetguild && myleader == 0) && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Only_To_Party_Or_Guild]);
			return;
		}

		if (pMob[conn].MOB.Class != 1 && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Didnt_Learn_Summon_Skill]);
			return;
		}

		int learn = pMob[conn].MOB.LearnedSkill;

		if ((learn & 0x40000) == 0 && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Didnt_Learn_Summon_Skill]);
			return;
		}

		int special = pMob[conn].MOB.CurrentScore.Special[3];

		unsigned char map_att = GetAttribute(pMob[conn].TargetX, pMob[conn].TargetY);

		if ((map_att & 4) && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_That_Here]);
			return;
		}

		int dif = pMob[conn].MOB.CurrentScore.Level + special + 30;

		if (pMob[target].MOB.CurrentScore.Level > dif && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Too_High_Level]);
			return;
		}

		/*if (pUser[target].OnlyTrade)
		{
		SendClientMessage(conn, g_pMessageStringTable[_NN_ONLYTRADE]);
		return;
		}*/

		if (pMob[target].TargetX & 0xFF00 || pMob[target].TargetY & 0xFF00)
		{
			DoSummon(target, pMob[conn].TargetX, pMob[conn].TargetY);

			sprintf(temp, g_pMessageStringTable[_SN_Summoned_By_S], pMob[conn].MOB.MobName);
			SendClientMessage(target, temp);

			sprintf(temp, "etc,summon %s X:%d Y:%d", pMob[target].MOB.MobName, pMob[target].TargetX, pMob[target].TargetY);
			Log(temp, pUser[conn].AccountName, pUser[conn].IP);
			return;
		}
		else
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_That_Here]);
			return;
		}
		return;
	}
#pragma endregion
#pragma region /time
	else if (strcmp(m->MobName, "time") == 0)
	{
		char tmptime[256];

		time_t mytime;

		time(&mytime);

		tm *timeinfo = localtime(&mytime);

		strftime(tmptime, 256, "%Y-%m-%d %H:%M:%S", timeinfo);

		SendClientMessage(conn, tmptime);
		return;
	}
#pragma endregion
#pragma region _CM_GM - /gm
	else if (strcmp(m->MobName, "gm") == 0 || strcmp(m->MobName, "GM") == 0)
	{
		int level = pMob[conn].MOB.CurrentScore.Level - 0;

		ProcessImple(conn, level, m->String);
		return;
	}
#pragma endregion
	/*#pragma region _CM_GM - /gm
		else if (strcmp(m->MobName, "gm") == 0 || strcmp(m->MobName, "GM") == 0)
		{
		char tmg[256];

		if (pMob[conn].MOB.CurrentScore.Level > 1000)
		{
		int level = pMob[conn].MOB.CurrentScore.Level - 1000;

		ProcessImple(conn, level, m->String);
		return;
		}
		else
		{
		SendClientMessage(conn, "Se você digitar [/gm] novamente será expurgado (Punimento de 3 Horas.)");

		if(pUser[conn].Message != 0)
		{
		int isTime = GetTickCount() - pUser[conn].Message;

		pUser[conn].Message = GetTickCount();

		if(isTime < 10000)
		{
		SendBan3Hours(pUser[conn].IP);
		Send3HoursAccBan(pUser[conn].AccountName);

		CloseUser(conn);

		SendClientMessage(conn, "Você foi banido por comandos ilegais, contate o suporte.");

		sprintf(tmg, "Jogador [%s] foi banido temporariamente por uso de comandos ilegais: '/gm'.", pMob[conn].MOB.MobName);

		SendNotice(tmg);

		return;
		}
		}
		pUser[conn].Message = GetTickCount();
		return;
		}
		}
		#pragma endregion*/
#pragma region _NN_Relocate - /relo
	else if (strcmp(m->MobName, g_pMessageStringTable[_NN_Relocate]) == 0 || strcmp(m->MobName, "relo") == 0)
	{
		int Class = pMob[conn].MOB.Class;
		int admin = 0;

		if (pMob[conn].MOB.CurrentScore.Level >= 1000)
			admin = 1;

		if (pMob[conn].MOB.CurrentScore.Hp <= 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_Killed]);
			return;
		}


		if (m->String[0] == 0)
			return;

		m->String[NAME_LENGTH - 1] = 0;
		m->String[NAME_LENGTH - 2] = 0;

		int target = GetUserByName(m->String);

		if (target == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (pUser[target].Mode != USER_PLAY)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		int myguild = pMob[conn].MOB.Guild;
		int targetguild = pMob[target].MOB.Guild;

		int myleader = pMob[conn].Leader;
		if (myleader <= 0)
			myleader = conn;

		int targetleader = pMob[target].Leader;
		if (targetleader <= 0)
			targetleader = target;

		if ((myguild == 0 && myleader == 0 || myleader != targetleader || myguild != targetguild && myleader == 0) && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Only_To_Party_Or_Guild]);
			return;
		}

		int summonmode = 0;
		int medal = pMob[conn].MOB.Equip[12].sIndex;

		if (medal == 522 || medal == 509 || medal >= 529 && medal <= 531 || medal >= 535 && medal <= 537)
			summonmode = 1;

		if (Class == 1)
		{
			int learn = pMob[conn].MOB.LearnedSkill;

			if (learn & 0x40000)
				summonmode = 2;
		}

		if (summonmode == 0 && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Didnt_Learn_Summon_Skill]);
			return;
		}

		unsigned char map_att = GetAttribute(pMob[target].TargetX, pMob[target].TargetY);

		if ((map_att & 4) && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Cant_Use_That_Here]);
			return;
		}

		int dif = pMob[conn].MOB.CurrentScore.Level;
		int myspecial = pMob[conn].MOB.CurrentScore.Special[3];

		if (summonmode == 1)
			dif += 30;

		if (summonmode == 2)
			dif += myspecial + 30;

		if (pMob[target].MOB.CurrentScore.Level > dif && admin == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Too_High_Level]);
			return;
		}

		DoTeleport(conn, pMob[target].TargetX, pMob[target].TargetY);

		sprintf(temp, "etc,relo %s X:%d Y:%d", pMob[target].MOB.MobName, pMob[target].TargetX, pMob[target].TargetY);
		Log(temp, pUser[conn].AccountName, pUser[conn].IP);

		if (admin == 0)
		{
			sprintf(temp, g_pMessageStringTable[_SN_S_Relocate_To_You], pMob[conn].MOB.MobName);
			SendClientMessage(target, temp);
		}

		return;
	}
#pragma endregion
#pragma region _NN_Deprivate - /expulsar
	else if (strcmp(m->MobName, "expulsar") == 0)
	{
		if (m->String[0] == 0)
			return;

		m->String[NAME_LENGTH - 1] = 0;
		m->String[NAME_LENGTH - 2] = 0;

		int target = GetUserByName(m->String);

		if (target)
			DoDeprivate(conn, target);
		else
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);

		return;
	}
#pragma endregion
#pragma region /fimguerra
	else if (strcmp(m->MobName, "fimguerra") == 0)
	{
		int myguild = pMob[conn].MOB.Guild;
		int max_guild = 65536;

		if (myguild <= 0)
			return;

		if (myguild >= max_guild)
			return;

		int checkguild = pMob[conn].MOB.Guild;

		if (checkguild != myguild)
			return;

		if (pMob[conn].MOB.GuildLevel == 0)
			return;

		if (pMob[conn].MOB.Equip[15].sIndex == 0)
			return;

		int Group = ServerGroup;
		int Server = (myguild / MAX_GUILD) & 15;
		int Guild = myguild & MAX_GUILD;

		int isname = 1;

		if (ServerGroup >= 0 && Group < MAX_SERVERGROUP && Server >= 0 && Server < 16 && Guild >= 0 && Guild < MAX_GUILD)
		{
			if (!g_pGuildName[Group][Server][Guild][0])
				isname = 0;
		}
		else
			isname = 0;

		if (isname == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Only_Named_Guild]);
			return;
		}

		MSG_STANDARDPARM2 sm_dwar;
		memset(&sm_dwar, 0, sizeof(MSG_STANDARDPARM2));

		sm_dwar.Type = _MSG_War;
		sm_dwar.Size = sizeof(MSG_STANDARDPARM2);
		sm_dwar.ID = conn;

		sm_dwar.Parm1 = pMob[conn].MOB.Guild;
		sm_dwar.Parm2 = 0;

		DBServerSocket.SendOneMessage((char*)&sm_dwar, sizeof(MSG_STANDARDPARM2));
		return;
	}
#pragma endregion
#pragma region /fimirma
	else if (strcmp(m->MobName, "fimirma") == 0)
	{
		int myguild = pMob[conn].MOB.Guild;
		int max_guild = 65536;

		if (myguild <= 0)
			return;

		if (myguild >= max_guild)
			return;

		int checkguild = pMob[conn].MOB.Guild;

		if (checkguild != myguild)
			return;

		if (pMob[conn].MOB.GuildLevel == 0)
			return;

		if (pMob[conn].MOB.Equip[15].sIndex == 0)
			return;

		int Group = ServerGroup;
		int Server = (myguild / MAX_GUILD) & 15;
		int Guild = myguild & MAX_GUILD;

		int isname = 1;

		if (ServerGroup >= 0 && Group < MAX_SERVERGROUP && Server >= 0 && Server < 16 && Guild >= 0 && Guild < MAX_GUILD)
		{
			if (!g_pGuildName[Group][Server][Guild][0])
				isname = 0;
		}
		else
			isname = 0;

		if (isname == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Only_Named_Guild]);
			return;
		}

		MSG_GuildAlly sm_dga;
		memset(&sm_dga, 0, sizeof(MSG_GuildAlly));

		sm_dga.Type = _MSG_GuildAlly;
		sm_dga.Size = sizeof(MSG_GuildAlly);
		sm_dga.ID = conn;

		sm_dga.Guild = pMob[conn].MOB.Guild;
		sm_dga.Ally = 0;

		DBServerSocket.SendOneMessage((char*)&sm_dga, sizeof(MSG_GuildAlly));
		return;
	}
#pragma endregion
#pragma region /not
	else if (strcmp(m->MobName, "not") == 0 && pMob[conn].MOB.CurrentScore.Level >= 1000)
	{
		MSG_DBNotice sm_dbn;
		memset(&sm_dbn, 0, sizeof(MSG_DBNotice));

		sm_dbn.Size = sizeof(MSG_DBNotice);
		sm_dbn.ID = 0;
		sm_dbn.Type = _MSG_DBNotice;

		strncpy(sm_dbn.String, m->String, MESSAGE_LENGTH - 1);

		DBServerSocket.SendOneMessage((char*)&sm_dbn, sizeof(MSG_DBNotice));
		return;
	}
#pragma endregion
#pragma region /wp
	else if (!strcmp(m->MobName, "donate"))
	{
		char Msg[128];
		sprintf(Msg, "Donates disponíveis: %d.", pUser[conn].Donate);
		SendClientMessage(conn, Msg);
		return;
	}
#pragma endregion
	/*
	#pragma region /ativar
		else if (strcmp(m->MobName, "pin") == 0)
		{
			if (m->String[0] == 0)
				return;

			char innerMsg[100] = { 0, }, dir[256];
			int numCash = 0;
			FILE *fp;

			sprintf_s(dir, "userdata/Seriais/%s.txt", m->String);


			if (!FileExist(dir))
				SendClientMessage(conn, g_pMessageStringTable[_NN_PINCODE_INVALID]);
			else
			{
				fopen_s(&fp, dir, "rt");

				if (fp)
				{
					fscanf_s(fp, "%d", &numCash);
					fclose(fp);
					remove(dir);
					pUser[conn].Donate += numCash;

					char tmg[256];
					sprintf(tmg, "Foi creditados [%d] Donates. [Total: %d].", numCash, pUser[conn].Donate);
					SendClientMessage(conn, tmg);

					SaveUser(conn, 0);
					return;
				}
				else
					SendClientMessage(conn, "Um erro ocorreu durante a ativação, favor contatar administração.");
			}
			return;
		}
	#pragma endregion
	*/
#pragma region /contaprincipal
	else if (strcmp(m->MobName, "contaprincipal") == 0)
	{
		MSG_DBPrimaryAccount sm;
		memset(&sm, 0, sizeof(MSG_DBPrimaryAccount));

		sm.Size = sizeof(MSG_DBPrimaryAccount);
		sm.Type = _MSG_DBPrimaryAccount;
		sm.ID = conn;

		memcpy(&sm.Mac, pUser[conn].Mac, sizeof(sm.Mac));
		sm.IP = pUser[conn].IP;

		DBServerSocket.SendOneMessage((char*)&sm, sizeof(MSG_DBPrimaryAccount));

		return;
	}
#pragma endregion
	if (pUser[conn].MuteChat == 1)
	{
		SendClientMessage(conn, g_pMessageStringTable[_NN_No_Speak]);
		return;
	}

#pragma region /r - -- = Cidadao  pm
	if (m->MobName[0] == 0)
	{
#pragma region Chat Guild : -
		if (m->String[0] == '-')
		{
			strncpy(m->MobName, pMob[conn].MOB.MobName, NAME_LENGTH);

			m->String[MESSAGE_LENGTH] = 3;

			int guild = pMob[conn].MOB.Guild;

			if (guild == 0)
			{
				SendClientMessage(conn, g_pMessageStringTable[_NN_Only_Guild_Member_Can]);
				return;
			}

			for (int i = 0; i < MAX_USER; i++)
			{
				if (pUser[i].Mode != USER_PLAY)
					continue;

				if (pMob[i].MOB.Guild != guild && m->String[1] != '-')
					continue;

				if (pMob[i].MOB.Guild != guild && m->String[1] == '-' && (pMob[i].MOB.Guild != g_pGuildAlly[guild] || g_pGuildAlly[guild] == 0))
					continue;

				if (i == conn)
					continue;

				if (pUser[i].Guildchat)
					continue;

				m->ID = conn;
				SyncMulticast(conn, (MSG_STANDARD*)m, 0);
			}

			sprintf(temp, "chat_guild, %s : %s", m->MobName, m->String);
			ChatLog(temp, pUser[conn].AccountName, pUser[conn].IP);
			return;
		}
#pragma endregion
#pragma region Chat Party : =
		if (m->String[0] == '=')
		{
			strncpy(m->MobName, pMob[conn].MOB.MobName, NAME_LENGTH);

			int myleader = pMob[conn].Leader;

			if (myleader <= 0)
				myleader = conn;

			if (myleader <= 0 || myleader >= MAX_USER)
				return;

			if (pUser[myleader].Mode != USER_PLAY)
				return;

			if (myleader != conn)
				pUser[myleader].cSock.AddMessage((char*)m, sizeof(MSG_MessageWhisper));

			for (int i = 0; i < MAX_PARTY; i++)
			{
				int partyconn = pMob[myleader].PartyList[i];

				if (partyconn <= 0 || partyconn >= MAX_USER)
					continue;

				if (partyconn == conn)
					continue;

				if (pUser[partyconn].Mode != USER_PLAY)
					continue;

				if (pUser[partyconn].PartyChat)
					continue;

				m->ID = conn;
				pUser[partyconn].cSock.AddMessage((char*)m, sizeof(MSG_MessageWhisper));
			}

			sprintf(temp, "chat_party, %s : %s", m->MobName, m->String);
			ChatLog(temp, pUser[conn].AccountName, pUser[conn].IP);
			return;
		}
#pragma endregion
#pragma region Chat Reino
		if (m->String[0] == '@' && m->String[1] == '@')
		{
			if (pUser[conn].Message != 0)
			{
				int isTime = GetTickCount() - pUser[conn].Message;

				pUser[conn].Message = GetTickCount();

				if (isTime < 2000)
				{
					SendClientMessage(conn, "Aguarde 2 segundos.");
					return;
				}
			}
			pUser[conn].Message = GetTickCount();

			strncpy(m->MobName, pMob[conn].MOB.MobName, NAME_LENGTH);

			SyncKingdomMulticast(conn, pMob[conn].MOB.Clan, (MSG_STANDARD*)m, 0);

			sprintf(temp, "chat_kingdom, %s : %s reino:%d", m->MobName, m->String, pMob[conn].MOB.Clan);
			ChatLog(temp, pUser[conn].AccountName, pUser[conn].IP);

			return;
		}
#pragma endregion
#pragma region Chat Cidadão
		if (m->String[0] == '@')
		{
			if (pUser[conn].Message != 0)
			{
				int isTime = GetTickCount() - pUser[conn].Message;

				pUser[conn].Message = GetTickCount();

				if (isTime < 2000)
				{
					SendClientMessage(conn, "Aguarde 2 segundos.");
					return;
				}
			}
			pUser[conn].Message = GetTickCount();

			strncpy(m->MobName, pMob[conn].MOB.MobName, NAME_LENGTH);

			SyncMulticast(conn, (MSG_STANDARD*)m, 0);

			sprintf(temp, "chat_cidadao, %s : %s", m->MobName, m->String);
			ChatLog(temp, pUser[conn].AccountName, pUser[conn].IP);
			return;
		}
#pragma endregion
		return;
	}
#pragma region PM, /r
	if (m->MobName[0] != 0)
	{
		m->MobName[NAME_LENGTH - 1] = 0;
		m->MobName[NAME_LENGTH - 2] = 0;

		int target = 0;

		if (strcmp(m->MobName, g_pMessageStringTable[_NN_Reply]) == 0 || strcmp(m->MobName, "r") == 0)
		{
			if (!pUser[conn].LastChat[0])
			{
				SendClientMessage(conn, g_pMessageStringTable[_NN_No_One_To_Reply]);
				return;
			}
			memcpy(m->MobName, pUser[conn].LastChat, NAME_LENGTH);

			m->MobName[NAME_LENGTH - 1] = 0;
			m->MobName[NAME_LENGTH - 2] = 0;
		}

		target = GetUserByName(m->MobName);

		if (target == 0)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (pUser[target].Mode != USER_PLAY)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Not_Connected]);
			return;
		}

		if (pUser[target].Whisper && pMob[conn].MOB.CurrentScore.Level < 1000)
		{
			SendClientMessage(conn, g_pMessageStringTable[_NN_Deny_Whisper]);
			return;
		}
		m->ID = target;

		memcpy(pUser[conn].LastChat, m->MobName, NAME_LENGTH);

		if (m->String[0] == 0)
		{
			if (pMob[target].MOB.Guild == 0)
			{
				char tt[256];

				sprintf(tt, "Cidadania: %d  Fama: %d", pMob[target].extra.Citizen, pMob[target].extra.Fame);
				sprintf(temp, "%s  %s", pMob[target].MOB.MobName, tt);
			}
			else
			{
				char guildname[256];
				char tt[256];
				char guildind[256];
				char guildindex[256];
				int usGuild = pMob[conn].MOB.Guild;
				BASE_GetGuildName(ServerGroup, pMob[target].MOB.Guild, guildname);
				BASE_GetGuildName(ServerGroup, pMob[conn].MOB.Guild, guildind);
				//guildindex = pMob[conn].MOB.GuildLevel;

				if (pMob[conn].MOB.Guild == 0 && pMob[conn].MOB.GuildLevel != 9)
				{
					sprintf(tt, "Cidadania: %d  Fama: %d", pMob[target].extra.Citizen, pMob[target].extra.Fame);
					sprintf(temp, "%s  %s [ %s ]", pMob[target].MOB.MobName, tt, guildname);

				}
				//GUILDSS
				if (pMob[conn].MOB.Guild != 0 && pMob[conn].MOB.GuildLevel == 9 && pMob[conn].MOB.Clan != pMob[target].MOB.Clan)
				{
					sprintf(tt, "[ %s(%d) ] Cidadania: %d  Fama: %d", guildind, GuildInfo[usGuild].Fame, pMob[target].extra.Citizen, pMob[target].extra.Fame, pMob[conn].MOB.Guild);
					sprintf(temp, "%s  %s %s ", pMob[target].MOB.MobName, tt, guildname);
				}
				if (pMob[conn].MOB.Clan != pMob[target].MOB.Clan)
				{
					sprintf(tt, "Cidadania %d  Fama %d", pMob[target].extra.Citizen, pMob[target].extra.Fame);
					sprintf(temp, "%s  %s [ %s ]", pMob[target].MOB.MobName, tt, guildname);
				}
				if (pMob[conn].MOB.Clan == pMob[target].MOB.Clan && pMob[conn].MOB.GuildLevel != 9)
				{
					sprintf(tt, "Cidadania %d  Fama %d", pMob[target].extra.Citizen, pMob[target].extra.Fame);
					sprintf(temp, "%s  %s [ %s ]", pMob[target].MOB.MobName, tt, guildname);
				}
				if (pMob[conn].MOB.Clan != pMob[target].MOB.Clan && pMob[conn].MOB.GuildLevel != 0)
				{
					sprintf(tt, "Cidadania %d  Fama %d", pMob[target].extra.Citizen, pMob[target].extra.Fame);
					sprintf(temp, "%s / %s [ %s ]", pMob[target].MOB.MobName, tt, guildname);
				}
				//GUILDSS
				if (pMob[conn].MOB.GuildLevel == 9 && pMob[conn].MOB.Guild == pMob[target].MOB.Guild)
				{
					sprintf(tt, "[ %s(%d) ] Cidadania: %d  Fama: %d", guildind, GuildInfo[usGuild].Fame, pMob[target].extra.Citizen, pMob[target].extra.Fame, pMob[conn].MOB.Guild);
					sprintf(temp, "%s  %s  ", pMob[target].MOB.MobName, tt);
				}
				if (pMob[conn].MOB.GuildLevel == 9 && pMob[conn].MOB.Guild != pMob[target].MOB.Guild)
				{
					sprintf(tt, "Cidadania %d  Fama %d", pMob[target].extra.Citizen, pMob[target].extra.Fame);
					sprintf(temp, "%s  %s [ %s ]", pMob[target].MOB.MobName, tt, guildname);
				}
			}
			SendClientMessage(conn, temp);

			if (pMob[target].Snd[0] != 0)
			{
				sprintf(temp, "%s %s", g_pMessageStringTable[_NN_SND_MESSAGE], pMob[target].Snd);

				SendClientMessage(conn, temp);
			}
			return;
		}

		if (m->String[0] == '-' || m->String[0] == '=')
			m->String[0] = ' ';

		m->String[MESSAGE_LENGTH] = 0;

		memcpy(m->MobName, pMob[conn].MOB.MobName, NAME_LENGTH);
		memcpy(pUser[target].LastChat, m->MobName, NAME_LENGTH);

		pUser[target].cSock.AddMessage((char*)m, sizeof(MSG_MessageWhisper));

		sprintf(temp, "chat_sms,%s %s : %s", pMob[conn].MOB.MobName, m->MobName, m->String);
		ChatLog(temp, pUser[conn].AccountName, pUser[conn].IP);
		return;
	}
#pragma endregion

	sprintf(temp, "packet, MSG_MessageWhisper user: %s used command: %s", pMob[conn].MOB.MobName, m->MobName);
	Log(temp, pUser[conn].AccountName, pUser[conn].IP);

#pragma endregion
}

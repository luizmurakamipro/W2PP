
#include "ProcessClientMessage.h"

void Exec_MSG_CombineItemAgatha(int conn, char *pMsg)
{
	MSG_CombineItem *m = (MSG_CombineItem*)pMsg;

	if (conn < 0 || conn > MAX_USER)
		return;

	int item1unique = g_pItemList[m->Item[0].sIndex].nUnique;
	int item2unique = g_pItemList[m->Item[1].sIndex].nUnique;
	int item2grade = g_pItemList[m->Item[1].sIndex].Grade;

	short luz_count = 0;

	for (short i = 2; i < 6; i++)
	{
		if (m->Item[i].sIndex == 3140)
			luz_count++;
	}

	if (luz_count != 4)
	{
		SendClientMessage(conn, "Você precisa de 4 unidades da pedra da luz.");
		SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
		return;
	}
	if (g_pItemList[m->Item[0].sIndex].nPos != g_pItemList[m->Item[1].sIndex].nPos)
	{
		SendClientMessage(conn, "Os dois itens precisam ser do mesmo tipo.");
		SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
		return;
	}
	if (item1unique == 9 || item1unique == 19 || item1unique == 29 || item1unique == 39)//Somente itens arch
	{
		bool cango = false;
		int chancebase = 0;

		//item D dourado
		if (item2unique == (item1unique - 1))
		{
			cango = true;
			if (item2grade == 1)
				chancebase += 12;
			else if (item2grade == 2)
				chancebase += 18;
			else if (item2grade == 3)
				chancebase += 24;
			else if (item2grade == 4)
				chancebase += 30;
		}
		//Item E
		else if (item2unique == (item1unique + 1))
		{
			cango = true;
			if (item2grade == 1)
				chancebase += 26;
			else if (item2grade == 4)
				chancebase += 32;
		}
		//Item D normal
		else if (item2unique == (item1unique - 2))
		{
			cango = true;
			if (item2grade == 1)
				chancebase += 4;
			else if (item2grade == 2)
				chancebase += 8;
			else if (item2grade == 3)
				chancebase += 12;
			else if (item2grade == 4)
				chancebase += 16;
		}
		else{
			SendClientMessage(conn, "Somente itens [D] ou [E].");
			SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
			return;
		}
		if (cango == true)
		{
			int refid = 0, refval = 0;
			for (short i = 0; i < sizeof(ref); i++)
			{
				if (m->Item[1].stEffect[0].cEffect == ref[i]){
					refid = 1;
					refval = m->Item[1].stEffect[0].cValue;
					break;
				}
				else if (m->Item[1].stEffect[1].cEffect == ref[i]){
					refid = 2;
					refval = m->Item[1].stEffect[1].cValue;
					break;
				}
				else if (m->Item[1].stEffect[2].cEffect == ref[i]){
					refid = 3;
					refval = m->Item[1].stEffect[2].cValue;
					break;
				}
			}
			if (refval >= 230 && refval <= 233){
				chancebase += 18;
			}
			else if (refval >= 234 && refval <= 237){
				chancebase += 26;
			}
			int chance = rand() % 100;
			if (chance <= chancebase)//Sucesso
			{
				STRUCT_ITEM nitem;
				nitem.sIndex = m->Item[0].sIndex;
				nitem.stEffect[0].cEffect = 43;
				nitem.stEffect[0].cValue = 7;

				if (refid == 1){
					nitem.stEffect[1].cEffect = m->Item[1].stEffect[1].cEffect;
					nitem.stEffect[2].cEffect = m->Item[1].stEffect[2].cEffect;
					nitem.stEffect[1].cValue = m->Item[1].stEffect[1].cValue;
					nitem.stEffect[2].cValue = m->Item[1].stEffect[2].cValue;
				}
				else if (refid == 2){
					nitem.stEffect[1].cEffect = m->Item[1].stEffect[0].cEffect;
					nitem.stEffect[2].cEffect = m->Item[1].stEffect[2].cEffect;
					nitem.stEffect[1].cValue = m->Item[1].stEffect[0].cValue;
					nitem.stEffect[2].cValue = m->Item[1].stEffect[2].cValue;
				}
				else if (refid == 3){
					nitem.stEffect[1].cEffect = m->Item[1].stEffect[1].cEffect;
					nitem.stEffect[2].cEffect = m->Item[1].stEffect[0].cEffect;
					nitem.stEffect[1].cValue = m->Item[1].stEffect[1].cValue;
					nitem.stEffect[2].cValue = m->Item[1].stEffect[0].cValue;
				}
				for (short i = 0; i < 6; i++)
				{
					memset(&pMob[conn].MOB.Carry[m->InvenPos[i]], 0, sizeof(STRUCT_ITEM));
					SendItem(conn, ITEM_PLACE_CARRY, m->InvenPos[i], &pMob[conn].MOB.Carry[m->InvenPos[i]]);
				}
				memcpy(&pMob[conn].MOB.Carry[m->InvenPos[0]], &nitem, sizeof(STRUCT_ITEM));
				SendItem(conn, ITEM_PLACE_CARRY, m->InvenPos[0], &pMob[conn].MOB.Carry[m->InvenPos[0]]);
				char msg[200];
				sprintf(msg, "Obteve sucesso na composição do item [%s].", g_pItemList[pMob[conn].MOB.Carry[m->InvenPos[0]].sIndex].Name);
				SendClientMessage(conn, msg);
				SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
				return;
			}
			else{
				//Deleta tudo menos o item 1 que é o item arch
				for (short i = 0; i < 6; i++)
				{
					if (i != 1)
					{
						memset(&pMob[conn].MOB.Carry[m->InvenPos[i]], 0, sizeof(STRUCT_ITEM));
						SendItem(conn, ITEM_PLACE_CARRY, m->InvenPos[i], &pMob[conn].MOB.Carry[m->InvenPos[i]]);
					}
				}
				SendClientMessage(conn, "Falha na composição.");
				SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
				return;
			}
		}
		else{
			SendClientMessage(conn, "Há algo errado na composição.");
			SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
			return;
		}
	}
	else{
		SendClientMessage(conn, "Há algo errado na composição.");
		SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
		return;
	}
}
/*
void Exec_MSG_CombineItemAgatha(int conn, char *pMsg)
{
	MSG_CombineItem *m = (MSG_CombineItem*)pMsg;

	for (int i = 0; i < MAX_COMBINE; i++)
	{
		if (m->Item[i].sIndex == 0)
			continue;

		int invPos = m->InvenPos[i];

		if (invPos < 0 || invPos >= pMob[conn].MaxCarry)
		{
			RemoveTrade(conn);
			return;
		}

		if (memcmp(&pMob[conn].MOB.Carry[invPos], &m->Item[i], sizeof(STRUCT_ITEM)))
		{
			SendClientSignalParm(conn, ESCENE_FIELD, _MSG_CombineComplete, 0);
			return;
		}
	}

	int combine = GetMatchCombineAgatha(m->Item);

	if (combine == 0)
	{
		SendClientMessage(conn, g_pMessageStringTable[_NN_Wrong_Combination]);
		SendClientSignalParm(conn, ESCENE_FIELD, _MSG_CombineComplete, 0);
		return;
	}

	for (int i = 0; i < MAX_COMBINE; i++)
	{
		if (m->Item[i].sIndex == 0)
			continue;

		if (i == 1)
			continue;

		memset(&pMob[conn].MOB.Carry[m->InvenPos[i]], 0, sizeof(STRUCT_ITEM));
		SendItem(conn, ITEM_PLACE_CARRY, m->InvenPos[i], &pMob[conn].MOB.Carry[m->InvenPos[i]]);
	}


	for (int i = 0; i < MAX_COMBINE; i++)
	{
		if (m->Item[i].sIndex)
		{
			char itemlog[2048];

			BASE_GetItemCode(&m->Item[i], itemlog);

			strcat(temp, itemlog);
		}
	}

	int _rand = rand() % 115;
	if (_rand >= 100)
		_rand -= 15;


	int ipos = m->InvenPos[0];

	if (_rand <= combine || LOCALSERVER)
	{
		pMob[conn].MOB.Carry[ipos].sIndex = m->Item[0].sIndex;
		pMob[conn].MOB.Carry[ipos].stEffect[0].cEffect = m->Item[1].stEffect[0].cEffect;
		pMob[conn].MOB.Carry[ipos].stEffect[0].cValue = m->Item[1].stEffect[0].cValue;
		pMob[conn].MOB.Carry[ipos].stEffect[1].cEffect = m->Item[1].stEffect[1].cEffect;
		pMob[conn].MOB.Carry[ipos].stEffect[1].cValue = m->Item[1].stEffect[1].cValue;
		pMob[conn].MOB.Carry[ipos].stEffect[2].cEffect = m->Item[1].stEffect[2].cEffect;
		pMob[conn].MOB.Carry[ipos].stEffect[2].cValue = m->Item[1].stEffect[2].cValue;

		BASE_SetItemSanc(&pMob[conn].MOB.Carry[ipos], 7, 0);

		memset(&pMob[conn].MOB.Carry[m->InvenPos[1]], 0, sizeof(STRUCT_ITEM));
		SendItem(conn, ITEM_PLACE_CARRY, m->InvenPos[1], &pMob[conn].MOB.Carry[m->InvenPos[1]]);

		SendClientMessage(conn, g_pMessageStringTable[_NN_Processing_Complete]);

		SendClientSignalParm(conn, ESCENE_FIELD, _MSG_CombineComplete, 1);

		char tt[512];

		sprintf(temp, "%s ", pUser[conn].AccountName);
		BASE_GetItemCode(&pMob[conn].MOB.Carry[ipos], tt);
		strcat(temp, tt);

		SendItem(conn, ITEM_PLACE_CARRY, ipos, &pMob[conn].MOB.Carry[ipos]);

		return;
	}
	else
	{
		sprintf(temp, "%s", g_pMessageStringTable[_NN_CombineFailed]);
		SendClientMessage(conn, temp);

		SendClientSignalParm(conn, ESCENE_FIELD, _MSG_CombineComplete, 2);

		return;
	}
}*/
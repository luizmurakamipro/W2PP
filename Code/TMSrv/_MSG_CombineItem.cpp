
#include "ProcessClientMessage.h"

void Exec_MSG_CombineItem(int conn, char *pMsg)
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

	int combine = GetMatchCombine(m->Item);

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

	if (_rand <= combine || LOCALSERVER)
	{
		int itemindex = m->Item[0].sIndex;
		int extra = g_pItemList[itemindex].Extra;

		if (extra > 0 && extra < MAX_ITEMLIST)
		{
			int ipos = m->InvenPos[0];
			int joia = m->Item[1].sIndex - 2441;

			if (joia >= 0 && joia <= 3)
			{
				memcpy(&pMob[conn].MOB.Carry[ipos].sIndex, &m->Item[0], sizeof(STRUCT_ITEM));

				pMob[conn].MOB.Carry[ipos].sIndex = joia + extra;

				BASE_SetItemSanc(&pMob[conn].MOB.Carry[ipos], 7, 0);

				char tt[256];

				sprintf(tt, g_pMessageStringTable[_SS_CombineSucceed], pMob[conn].MOB.MobName, g_pItemList[extra].Name);
				sprintf(temp, "%s(%d/%d)", tt, _rand, combine);

				SendClientMessage(conn, temp);

				SendClientSignalParm(conn, ESCENE_FIELD, _MSG_CombineComplete, 1);

				sprintf(temp, "%s ", pUser[conn].AccountName);
				BASE_GetItemCode(&pMob[conn].MOB.Carry[ipos], tt);
				strcat(temp, tt);

				ItemLog(temp, "++ SUCESSO! ++", pUser[conn].IP);
				SendItem(conn, ITEM_PLACE_CARRY, ipos, &pMob[conn].MOB.Carry[ipos]);

				return;
			}
			else
			{
				sprintf(temp, "err,msg_combine - stone wrong %d", joia);
				ItemLog(temp, pUser[conn].AccountName, pUser[conn].IP);
				return;
			}
		}
		else
		{
			sprintf(temp, "err,msg_combine - extra item index wrong %d", itemindex);
			ItemLog(temp, pUser[conn].AccountName, pUser[conn].IP);
			return;
		}
	}
	else
	{
		sprintf(temp, "%s(%d/%d)", g_pMessageStringTable[269], _rand, combine);
		SendClientMessage(conn, temp);

		SendClientSignalParm(conn, ESCENE_FIELD, _MSG_CombineComplete, 2);

		return;
	}
}
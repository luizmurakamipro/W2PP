#include "ProcessClientMessage.h"

void Exec_MSG_Quiz(int conn, char *pMsg)
{
	if (conn <= 0 || conn >= MAX_USER)
		return;

	MSG_STANDARDPARM *m = (MSG_STANDARDPARM*)pMsg;

	if (SendQuiz[conn].Status)
	{
		if (m->Parm == SendQuiz[conn].RespostaCorreta)
		{
			pMob[conn].MOB.Coin += 500000;
			pMob[conn].MOB.Exp += 700000;
			SendEtc(conn);
			SendClientMessage(conn, "+ + 500.000 EXP / 700.000 GOLD + +");
			SendEmotion(conn, 14, 3);
		}
		else
			SendEmotion(conn, 20, 0);

		SendQuiz[conn].Status = FALSE;
	}
	return;
}
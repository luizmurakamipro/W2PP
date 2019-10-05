
#include "ProcessClientMessage.h"

void Exec_MSG_AccountLogin(int conn, char *pMsg)
{
	MSG_AccountLogin *m = (MSG_AccountLogin*)pMsg;

	int Size = m->Size;
	int ClientVersion = APP_VERSION;

	pUser[conn].AccountName[NAME_LENGTH - 1] = 0;

	if (conn <= 0 || (conn >= (MAX_USER - ADMIN_RESERV)))
	{
		sprintf(temp, g_pMessageStringTable[_NN_Reconnect]);
		SendClientMessage(conn, temp);

		pUser[conn].cSock.SendMessageA();

		CloseUser(conn);
		return;
	}


	if (Size < sizeof(MSG_AccountLogin))
		return;

	/*if (Size < sizeof(MSG_AccountLogin) || m->ClientVersion != ClientVersion)
	{
		sprintf(temp, g_pMessageStringTable[_NN_Version_Not_Match_Rerun], APP_VERSION);
		SendClientMessage(conn, temp);

		pUser[conn].cSock.SendMessageA();

		CloseUser(conn);
		return;
	}*/

	/*if (CheckBan(m->AccountName))
	{
		SendClientMessage(conn, "Conta bloqueada, entre em contato com Suporte.");
		CrackLog(conn, " accbanned");
		pUser[conn].cSock.SendMessageA();
		return;
	}

	if (CheckMacBan(&pUser[conn].Mac[0], &pUser[conn].Mac[1], &pUser[conn].Mac[2], &pUser[conn].Mac[3]))
	{
		SendClientMessage(conn, "Voc� foi banido permanentemente do Servidor.");
		CrackLog(conn, " macbanned");
		pUser[conn].cSock.SendMessageA();
		return;
	}

	if (CheckIpBan(pUser[conn].IP))
	{
		SendClientMessage(conn, "Voc� foi banido permanentemente do Servidor.");
		CrackLog(conn, " ipbanned");
		pUser[conn].cSock.SendMessageA();
		return;
	}

	if (CheckBan3Hours(pUser[conn].IP))
	{
		SendClientMessage(conn, "Voc� foi banido por 3 Horas do Servidor (Leia as Regras).");
		CrackLog(conn, " ipbanned");
		pUser[conn].cSock.SendMessageA();
		return;
	}

	if (Check3HoursAccBan(m->AccountName))
	{
		SendClientMessage(conn, "Voc� foi banido por 3 Horas do Servidor (Leia as Regras).");
		CrackLog(conn, " accbanned");
		pUser[conn].cSock.SendMessageA();
		return;
	}
	*/

	if (pUser[conn].Mode != USER_ACCEPT)
	{
		SendClientMessage(conn, "Login now, wait a moment.");

		CrackLog(conn, " accountlogin");
		pUser[conn].cSock.SendMessageA();
		return;
	}

	int tMac = 0;

	if (m->Size < sizeof(MSG_AccountLogin))
		memset(pUser[conn].Mac, 0xFF, sizeof(pUser[conn].Mac));
	else
		memcpy(pUser[conn].Mac, m->AdapterName, sizeof(pUser[conn].Mac));


	m->Type = _MSG_DBAccountLogin;
	m->ID = conn;

	sscanf(m->AccountName, "%s", pUser[conn].AccountName);

	_strupr(pUser[conn].AccountName);

	strncpy(m->AccountName, pUser[conn].AccountName, NAME_LENGTH);

	int check = CheckFailAccount(m->AccountName);

	if (check >= 3)
	{
		SendClientMessage(conn, g_pMessageStringTable[_NN_3_Tims_Wrong_Pass]);

		pUser[conn].cSock.SendMessageA();
		return;
	}

	DBServerSocket.SendOneMessage((char*)m, sizeof(MSG_AccountLogin));

	pUser[conn].Mode = USER_LOGIN;
	pMob[conn].Mode = MOB_EMPTY;
}


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

void Exec_MSG_SetShortSkill(int conn, char* pMsg)
{
	MSG_SetShortSkill* m = (MSG_SetShortSkill*)pMsg;

	if (pMob[conn].MOB.Class == 1 && pMob[conn].MOB.Equip[0].sIndex % 10 == 7)
	{
		for (int o = 0; o < 4; o++)
		{
			switch (m->Skill1[o])
			{
			case 106:
				pMob[conn].MOB.SkillBar[o] = 118;
				break;

			case 109:
				pMob[conn].MOB.SkillBar[o] = 121;
				break;

			case 114:
				pMob[conn].MOB.SkillBar[o] = 126;
				break;

			default:
				pMob[conn].MOB.SkillBar[o] = m->Skill1[o];
			}
		}
	}
	else
		memcpy(pMob[conn].MOB.SkillBar, m->Skill1, 4);

	if (pMob[conn].MOB.Class == 1 && pMob[conn].MOB.Equip[0].sIndex % 10 == 7)
	{
		for (int i = 0; i < 16; i++)
		{
			switch (m->Skill2[i])
			{
			case 106:
				pUser[conn].CharShortSkill[i] = 118;
				break;

			case 109:
				pUser[conn].CharShortSkill[i] = 121;
				break;

			case 114:
				pUser[conn].CharShortSkill[i] = 126;
				break;

			default:
				pUser[conn].CharShortSkill[i] = m->Skill2[i];
			}
		}
	}
	else
		memcpy(pUser[conn].CharShortSkill, m->Skill2, 16);
}

/* // All new skill's Index //
105 106 107 108 TK
109 110 111 112
113 114 115 116

117 118 119 120 FM
121 122 123 124
125 126 127 128

129 130 131 132 BM
133 134 135 136
137 138 139 140

141 142 143 144 HT
145 146 147 148
149 150 151 152
*/
/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"

#define SAY_NELTHARAKU1 "In the eastern reaches of the Netherwing fields lies Dragonmaw Fortress. It is there that the cruel Dragonmaw orc clan tortures teh drakes that they capture in an attempt to bend them to their will. Many have given up hope, choosing instead to do the bidding of Dragonmaw. The others... They refuse to land for fear of Dragonmaw war parties capturing them and taking them back to the fortress."
#define SAY_NELTHARAKU2 "I... We are powerless. Illidan found and delivered unto the Dragonmaw their leader - Zuluhed. This orc was the one responsible for the capture and incarceration of Alexstrasza. How could we fight against such power? He captured an Aspect! And just as easily, he took my mate. "
#define SAY_NELTHARAKU3 "Karynaku... She is held prisoner atop Dragonmaw Fortress. I have made many attempts to free her. All have failed... You have battled these beasts before, have you not? "
 
#define SAY_PLAYER1 "But you are dragons! How could orcs do this to you?"
#define SAY_PLAYER2 "Your mate?"
#define SAY_PLAYER3 "I have battled many beasts, dragon. I will help you."

struct MANGOS_DLL_DECL neltharakuAI : public ScriptedAI
{
    uint32 SayingTimer;
	uint32 SayingTimer2;
	Player* PlayerHolder;
    bool StartSay;
	int i;
	int ii;

    neltharakuAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        SayingTimer = 0;
		SayingTimer2 = 30000;
		i = 1;
		ii = 1;
        StartSay = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        return; //ignore all attackstart commands
    }

    void MoveInLineOfSight(Unit *who)
    {
        return; //ignore everyone around them (won't aggro anything)
    }

    void UpdateAI(const uint32 diff)
    {

        if(StartSay)
        {
            if(SayingTimer < diff)
            {
				switch(i)//Mob Saying Switch
				{
				case 1:
					DoSay(SAY_NELTHARAKU1,LANG_UNIVERSAL,NULL);
					SayingTimer = 35000;
					m_creature->SetUInt32Value(UNIT_NPC_FLAGS,0); //set npcflag to 0 would not allow other players to do that quest in same time!
					i++;
					break;
				case 2:
					DoSay(SAY_NELTHARAKU2,LANG_UNIVERSAL,NULL);
					SayingTimer = 35000;
					i++;
					break;
				case 3:
					DoSay(SAY_NELTHARAKU3,LANG_UNIVERSAL,NULL);
					SayingTimer = 90000;
					i++;
					break;
				}
            }else SayingTimer -= diff;

				if(SayingTimer2 < diff)
				{
					switch(ii)//Player Saying Switch
					{
				case 1:
					PlayerHolder->Say(SAY_PLAYER1,LANG_UNIVERSAL);
					SayingTimer2 = 35000;
					ii++;
					break;
				case 2:
					PlayerHolder->Say(SAY_PLAYER2,LANG_UNIVERSAL);
					SayingTimer2 = 20000;
					ii++;
					break;
				case 3:
					PlayerHolder->Say(SAY_PLAYER3,LANG_UNIVERSAL);
					SayingTimer2 = 8000;
					ii++;
					break;
				case 4:
					m_creature->SetUInt32Value(UNIT_NPC_FLAGS,3);
					PlayerHolder->CompleteQuest(10814);
					EnterEvadeMode();
					}
            }else SayingTimer2 -= diff;
				}

        return;
			}
};

CreatureAI* GetAI_neltharaku(Creature *_Creature)
{
    return new neltharakuAI (_Creature);
}

bool QuestAccept_neltharaku(Player *player, Creature *creature, Quest *quest )
{
    if(quest->GetQuestId() == 10814)
    {
		((neltharakuAI*)creature->AI())->PlayerHolder = player;
		((neltharakuAI*)creature->AI())->StartSay = true;
        return true;
    }
    return false;
}


void AddSC_neltharaku()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="neltharaku";
    newscript->GetAI = GetAI_neltharaku;
    newscript->pQuestAccept = &QuestAccept_neltharaku;
    m_scripts[nrscripts++] = newscript;
}

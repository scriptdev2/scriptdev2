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
    uint64 PlayerGUIDHolder;
    bool StartSay;
    int32 Interval_1;
    int32 Interval_2;

    neltharakuAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        SayingTimer = 0;
        SayingTimer2 = 30000;
        Interval_1 = 1;
        Interval_2 = 1;
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
                switch(Interval_1)//Mob Saying Switch
                {
                case 1:
                    DoSay(SAY_NELTHARAKU1,LANG_UNIVERSAL,NULL);
                    SayingTimer = 35000;
                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS,0); //set npcflag to 0 would not allow other players to do that quest in same time!
                    Interval_1++;
                    break;
                case 2:
                    DoSay(SAY_NELTHARAKU2,LANG_UNIVERSAL,NULL);
                    SayingTimer = 35000;
                    Interval_1++;
                    break;
                case 3:
                    DoSay(SAY_NELTHARAKU3,LANG_UNIVERSAL,NULL);
                    SayingTimer = 90000;
                    Interval_1++;
                    break;
                }
            }else SayingTimer -= diff;

            if(SayingTimer2 < diff)
            {
                //If player holder is not a valid unit enter evade mode
                if (!PlayerGUIDHolder || !Unit::GetUnit((*m_creature), PlayerGUIDHolder))
                {
                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS,3);
                    EnterEvadeMode();
                    return;
                }

                Player* PlayerTarget = (Player*)Unit::GetUnit((*m_creature), PlayerGUIDHolder);

                switch(Interval_2)//Player Saying Switch
                {
                case 1:
                    PlayerTarget->Say(SAY_PLAYER1,LANG_UNIVERSAL);
                    SayingTimer2 = 35000;
                    Interval_2++;
                    break;
                case 2:
                    PlayerTarget->Say(SAY_PLAYER2,LANG_UNIVERSAL);
                    SayingTimer2 = 20000;
                    Interval_2++;
                    break;
                case 3:
                    PlayerTarget->Say(SAY_PLAYER3,LANG_UNIVERSAL);
                    SayingTimer2 = 8000;
                    Interval_2++;
                    break;
                case 4:
                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS,3);
                    PlayerTarget->CompleteQuest(10814);
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

bool QuestAccept_neltharaku(Player *player, Creature *creature, Quest const* quest )
{
    if(quest->GetQuestId() == 10814)
    {
        ((neltharakuAI*)creature->AI())->PlayerGUIDHolder = player->GetGUID();
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

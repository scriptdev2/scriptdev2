/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: npcs_special
SD%Complete: 100
SDComment: To be used for special NPCs that are located globally. Support for quest 3861 (Cluck!)
SDCategory: NPCs
EndScriptData
*/

#include "sc_creature.h"
#include "sc_gossip.h"

/*########
# npc_chicken_cluck
#########*/

#define QUEST_CLUCK         3861
#define ITEM_CHICKEN_FEED   11109
#define CLUCK_TEXT1         "looks up at you quizzically. Maybe you should inspect it?"
#define CLUCK_TEXT2         "starts pecking at the feed"
#define FACTION_FRIENDLY    35
#define FACTION_CHICKEN     31

struct MANGOS_DLL_DECL npc_chicken_cluckAI : public ScriptedAI
{
    npc_chicken_cluckAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ResetFlagTimer;
    
    void Reset() 
    {
        ResetFlagTimer = 120000;

        m_creature->setFaction(FACTION_CHICKEN);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }
    
    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        // Reset flags after a certain time has passed so that the next player has to start the 'event' again
        if(m_creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
        {
            if(ResetFlagTimer < diff)
                EnterEvadeMode();
            else ResetFlagTimer -= diff;
        }

        if(m_creature->SelectHostilTarget() && m_creature->getVictim())
            DoMeleeAttackIfReady();
    }
}; 

CreatureAI* GetAI_npc_chicken_cluck(Creature *_Creature)
{
    return new npc_chicken_cluckAI(_Creature);
}

bool ReceiveEmote_npc_chicken_cluck( Player *player, Creature *_Creature, uint32 emote )
{
    if(player->GetTeam() == ALLIANCE)
    {
        if(player->GetQuestStatus(QUEST_CLUCK) == QUEST_STATUS_NONE)
        {
            if(emote == EMOTE_ONESHOT_CHICKEN)
            {
                if(rand()%30 == 0)
                {
                    _Creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _Creature->MonsterTextEmote(CLUCK_TEXT1, 0);
                }
            }
        }
        else if(player->HasItemCount(ITEM_CHICKEN_FEED, 1))
        {
            if(emote == EMOTE_ONESHOT_CHEER)
            {
                _Creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                _Creature->MonsterTextEmote(CLUCK_TEXT2, 0);
            }
        }
    }

    return true;
}

bool QuestAccept_npc_chicken_cluck(Player *player, Creature *_Creature, const Quest *_Quest )
{
    if(_Quest->GetQuestId() == QUEST_CLUCK)
        ((npc_chicken_cluckAI*)_Creature->AI())->Reset();

    return true;
}

bool QuestComplete_npc_chicken_cluck(Player *player, Creature *_Creature, const Quest *_Quest)
{
    if(_Quest->GetQuestId() == QUEST_CLUCK)
        ((npc_chicken_cluckAI*)_Creature->AI())->Reset();

    return true;
}


void AddSC_npcs_special()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_chicken_cluck";
    newscript->GetAI = GetAI_npc_chicken_cluck;
    newscript->pReceiveEmote = &ReceiveEmote_npc_chicken_cluck;
    newscript->pQuestAccept = &QuestAccept_npc_chicken_cluck;
    newscript->pQuestComplete = &QuestComplete_npc_chicken_cluck;
    
    m_scripts[nrscripts++] = newscript;   
}

/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Sunken_Temple
SD%Complete: 100
SDComment: Quest support: 8733
SDCategory: Sunken Temple
EndScriptData */

/* ContentData
at_shade_of_eranikus
npc_malfurion_stormrage
EndContentData */

#include "precompiled.h"
#include "sunken_temple.h"

enum
{
    QUEST_THE_CHARGE_OF_DRAGONFLIGHTS = 8555,
    QUEST_ERANIKUS_TYRANT_OF_DREAMS   = 8733
};

bool AreaTrigger_at_shade_of_eranikus(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        //Only do stuff, if the player has finished the PreQuest
        if (pPlayer->GetQuestRewardStatus(QUEST_THE_CHARGE_OF_DRAGONFLIGHTS) &&
            !pPlayer->GetQuestRewardStatus(QUEST_ERANIKUS_TYRANT_OF_DREAMS) &&
            pPlayer->GetQuestStatus(QUEST_ERANIKUS_TYRANT_OF_DREAMS) != QUEST_STATUS_COMPLETE)
        {
            if (pInstance->GetData(TYPE_MALFURION) != DONE)
            {
                pPlayer->SummonCreature(NPC_MALFURION, -639.378723f, -4.238533f, -90.835098f, 2.724664f, TEMPSUMMON_DEAD_DESPAWN, 0);
                pInstance->SetData(TYPE_MALFURION, DONE);
            }
        }
    }
    return false;
}

/*######
## npc_malfurion_stormrage
######*/
enum
{
    EMOTE_MALFURION1              = -1109000,
    SAY_MALFURION1                = -1109001,
    SAY_MALFURION2                = -1109002,
    SAY_MALFURION3                = -1109003,
    SAY_MALFURION4                = -1109004,

    MAX_MALFURION_TEMPLE_SPEECHES = 6
};

struct MANGOS_DLL_DECL npc_malfurionAI : public ScriptedAI
{
    npc_malfurionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiSpeech   = 0;
        m_uiSayTimer = 0;
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }

    uint32 m_uiSayTimer;
    uint32 m_uiSpeech;

    void Reset() {}
    void UpdateAI(const uint32 uiDiff)
    {
        // we are in Sunken Temple
        if (m_creature->GetMap()->IsDungeon())
        {
            if (m_uiSpeech < MAX_MALFURION_TEMPLE_SPEECHES)
            {
                if (m_uiSayTimer <= uiDiff)
                {
                    switch (m_uiSpeech)
                    {
                        case 0:
                            DoScriptText(EMOTE_MALFURION1, m_creature);
                            m_uiSayTimer = 1500;
                            break;
                        case 1:
                            m_creature->HandleEmote(EMOTE_ONESHOT_BOW);
                            m_uiSayTimer = 2000;
                            break;
                        case 2:
                            DoScriptText(SAY_MALFURION1, m_creature);
                            m_uiSayTimer = 1000;
                            break;
                        case 3:
                            DoScriptText(SAY_MALFURION2, m_creature);
                            m_uiSayTimer = 1000;
                            break;
                        case 4:
                            DoScriptText(SAY_MALFURION3, m_creature);
                            m_uiSayTimer = 2000;
                            break;
                        case 5:
                            DoScriptText(SAY_MALFURION4, m_creature);
                            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                            break;
                    }

                    ++m_uiSpeech;
                }
                else
                    m_uiSayTimer -= uiDiff;
            }
        }
    }
};

CreatureAI* GetAI_npc_malfurion(Creature* pCreature)
{
    return new npc_malfurionAI(pCreature);
}

void AddSC_sunken_temple()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "at_shade_of_eranikus";
    newscript->pAreaTrigger = &AreaTrigger_at_shade_of_eranikus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_malfurion_stormrage";
    newscript->GetAI = &GetAI_npc_malfurion;
    newscript->RegisterSelf();
}

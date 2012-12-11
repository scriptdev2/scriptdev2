/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Razorfen_Kraul
SD%Complete: 100
SDComment: Willix the Importer Escort Event
SDCategory: Razorfen Kraul
EndScriptData */

/* ContentData
quest_willix_the_importer
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

enum
{
    QUEST_WILLIX_THE_IMPORTER  = 1144,

    SAY_WILLIX_READY           = -1047000,
    SAY_WILLIX_1               = -1047001,
    SAY_WILLIX_2               = -1047002,
    SAY_WILLIX_3               = -1047003,
    SAY_WILLIX_4               = -1047004,
    SAY_WILLIX_5               = -1047005,
    SAY_WILLIX_6               = -1047006,
    SAY_WILLIX_7               = -1047007,
    SAY_WILLIX_END             = -1047008,

    SAY_WILLIX_AGGRO_1         = -1047009,
    SAY_WILLIX_AGGRO_2         = -1047010,
    SAY_WILLIX_AGGRO_3         = -1047011,
    SAY_WILLIX_AGGRO_4         = -1047012,

    NPC_RAGING_AGAMAR          = 4514
};

static const float aBoarSpawn[4][3] =
{
    {2151.420f, 1733.18f, 52.10f},
    {2144.463f, 1726.89f, 51.93f},
    {1956.433f, 1597.97f, 81.75f},
    {1958.971f, 1599.01f, 81.44f}
};

struct MANGOS_DLL_DECL npc_willix_the_importerAI : public npc_escortAI
{
    npc_willix_the_importerAI(Creature* m_creature) : npc_escortAI(m_creature) { Reset(); }

    void Reset() override {}

    // Exact use of these texts remains unknown, it seems that he should only talk when he initiates the attack or he is the first who is attacked by a npc
    void Aggro(Unit* pWho) override
    {
        switch (urand(0, 6))                                // Not always said
        {
            case 0: DoScriptText(SAY_WILLIX_AGGRO_1, m_creature, pWho); break;
            case 1: DoScriptText(SAY_WILLIX_AGGRO_2, m_creature, pWho); break;
            case 2: DoScriptText(SAY_WILLIX_AGGRO_3, m_creature, pWho); break;
            case 3: DoScriptText(SAY_WILLIX_AGGRO_4, m_creature, pWho); break;
        }
    }

    void JustSummoned(Creature* pSummoned) override
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void WaypointReached(uint32 uiPointId) override
    {
        switch (uiPointId)
        {
            case 2:
                DoScriptText(SAY_WILLIX_1, m_creature);
                break;
            case 6:
                DoScriptText(SAY_WILLIX_2, m_creature);
                break;
            case 9:
                DoScriptText(SAY_WILLIX_3, m_creature);
                break;
            case 14:
                DoScriptText(SAY_WILLIX_4, m_creature);
                // Summon 2 boars on the pathway
                m_creature->SummonCreature(NPC_RAGING_AGAMAR, aBoarSpawn[0][0], aBoarSpawn[0][1], aBoarSpawn[0][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(NPC_RAGING_AGAMAR, aBoarSpawn[1][0], aBoarSpawn[1][1], aBoarSpawn[1][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 25:
                DoScriptText(SAY_WILLIX_5, m_creature);
                break;
            case 33:
                DoScriptText(SAY_WILLIX_6, m_creature);
                break;
            case 44:
                DoScriptText(SAY_WILLIX_7, m_creature);
                // Summon 2 boars at the end
                m_creature->SummonCreature(NPC_RAGING_AGAMAR, aBoarSpawn[2][0], aBoarSpawn[2][1], aBoarSpawn[2][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(NPC_RAGING_AGAMAR, aBoarSpawn[3][0], aBoarSpawn[3][1], aBoarSpawn[3][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                break;
            case 45:
                DoScriptText(SAY_WILLIX_END, m_creature);
                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                // Complete event
                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_WILLIX_THE_IMPORTER, m_creature);
                SetEscortPaused(true);
                break;
        }
    }
};

CreatureAI* GetAI_npc_willix_the_importer(Creature* pCreature)
{
    return new npc_willix_the_importerAI(pCreature);
}

bool QuestAccept_npc_willix_the_importer(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_WILLIX_THE_IMPORTER)
    {
        if (npc_willix_the_importerAI* pEscortAI = dynamic_cast<npc_willix_the_importerAI*>(pCreature->AI()))
        {
            // After 4.0.1 set run = true
            pEscortAI->Start(false, pPlayer, pQuest);
            DoScriptText(SAY_WILLIX_READY, pCreature, pPlayer);
            pCreature->SetFactionTemporary(FACTION_ESCORT_N_NEUTRAL_PASSIVE, TEMPFACTION_RESTORE_RESPAWN);
        }
    }

    return true;
}

void AddSC_razorfen_kraul()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_willix_the_importer";
    pNewScript->GetAI = &GetAI_npc_willix_the_importer;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_willix_the_importer;
    pNewScript->RegisterSelf();
}

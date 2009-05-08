/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Ashenvale
SD%Complete: 70
SDComment: Quest support: 6482, 6544
SDCategory: Ashenvale Forest
EndScriptData */

/* ContentData
npc_ruul_snowhoof
npc_torek
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*####
# npc_ruul_snowhoof
####*/

enum
{
    QUEST_FREEDOM_TO_RUUL   = 6482,
    ENTRY_T_URSA            = 3921,
    ENTRY_T_TOTEMIC         = 3922,
    ENTRY_T_PATHFINDER      = 3926,
    FACTION_R_ESCORTEE      = 113,
};

struct MANGOS_DLL_DECL npc_ruul_snowhoofAI : public npc_escortAI
{
    npc_ruul_snowhoofAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        normFaction = pCreature->getFaction();
        Reset();
    }

    uint32 normFaction;

    void WaypointReached(uint32 i)
    {
        switch(i)
        {
            case 13:
                m_creature->SummonCreature(ENTRY_T_TOTEMIC, 3449.218018, -587.825073, 174.978867, 4.714445, TEMPSUMMON_DEAD_DESPAWN, 60000);
                m_creature->SummonCreature(ENTRY_T_URSA, 3446.384521, -587.830872, 175.186279, 4.714445, TEMPSUMMON_DEAD_DESPAWN, 60000);
                m_creature->SummonCreature(ENTRY_T_PATHFINDER, 3444.218994, -587.835327, 175.380600, 4.714445, TEMPSUMMON_DEAD_DESPAWN, 60000);
                break;
            case 19:
                m_creature->SummonCreature(ENTRY_T_TOTEMIC, 3508.344482, -492.024261, 186.929031, 4.145029, TEMPSUMMON_DEAD_DESPAWN, 60000);
                m_creature->SummonCreature(ENTRY_T_URSA, 3506.265625, -490.531006, 186.740128, 4.239277, TEMPSUMMON_DEAD_DESPAWN, 60000);
                m_creature->SummonCreature(ENTRY_T_PATHFINDER, 3503.682373, -489.393799, 186.629684, 4.349232, TEMPSUMMON_DEAD_DESPAWN, 60000);
                break;
            case 21:
                if (Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID))
                {
                    if (pPlayer && pPlayer->GetTypeId() == TYPEID_PLAYER)
                        ((Player*)pPlayer)->GroupEventHappens(QUEST_FREEDOM_TO_RUUL,m_creature);
                }
                break;
        }
    }

    void Reset()
    {
        if (!IsBeingEscorted)
            m_creature->setFaction(normFaction);
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }

    void JustDied(Unit* killer)
    {
        if (PlayerGUID)
        {
            if (Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID))
                ((Player*)pPlayer)->FailQuest(QUEST_FREEDOM_TO_RUUL);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

bool QuestAccept_npc_ruul_snowhoof(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_FREEDOM_TO_RUUL)
    {
        pCreature->setFaction(FACTION_R_ESCORTEE);
        pCreature->SetStandState(UNIT_STAND_STATE_STAND);
        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, false, pPlayer->GetGUID());
    }
    return true;
}

CreatureAI* GetAI_npc_ruul_snowhoofAI(Creature* pCreature)
{
    npc_ruul_snowhoofAI* ruul_snowhoofAI = new npc_ruul_snowhoofAI(pCreature);

    ruul_snowhoofAI->FillPointMovementListForCreature();

    return (CreatureAI*)ruul_snowhoofAI;
}

/*####
# npc_torek
####*/

#define SAY_READY                   -1000106
#define SAY_MOVE                    -1000107
#define SAY_PREPARE                 -1000108
#define SAY_WIN                     -1000109
#define SAY_END                     -1000110

#define SPELL_REND                  11977
#define SPELL_THUNDERCLAP           8078

#define QUEST_TOREK_ASSULT          6544

#define ENTRY_SPLINTERTREE_RAIDER   12859
#define ENTRY_DURIEL                12860
#define ENTRY_SILVERWING_SENTINEL   12896
#define ENTRY_SILVERWING_WARRIOR    12897

struct MANGOS_DLL_DECL npc_torekAI : public npc_escortAI
{
    npc_torekAI(Creature* pCreature) : npc_escortAI(pCreature) {Reset();}

    uint32 Rend_Timer;
    uint32 Thunderclap_Timer;

    void WaypointReached(uint32 i)
    {
        Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID);

        if (!pPlayer)
            return;

        switch (i)
        {
            case 1:
                DoScriptText(SAY_MOVE, m_creature, pPlayer);
                break;
            case 8:
                DoScriptText(SAY_PREPARE, m_creature, pPlayer);
                break;
            case 19:
                //TODO: verify location and creatures amount.
                m_creature->SummonCreature(ENTRY_DURIEL,1776.73,-2049.06,109.83,1.54,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,25000);
                m_creature->SummonCreature(ENTRY_SILVERWING_SENTINEL,1774.64,-2049.41,109.83,1.40,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,25000);
                m_creature->SummonCreature(ENTRY_SILVERWING_WARRIOR,1778.73,-2049.50,109.83,1.67,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,25000);
                break;
            case 20:
                DoScriptText(SAY_WIN, m_creature, pPlayer);
                if (pPlayer && pPlayer->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)pPlayer)->GroupEventHappens(QUEST_TOREK_ASSULT,m_creature);
                break;
            case 21:
                DoScriptText(SAY_END, m_creature, pPlayer);
                break;
        }
    }

    void Reset()
    {
        Rend_Timer = 5000;
        Thunderclap_Timer = 8000;
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }

    void JustDied(Unit* killer)
    {
        if (killer->GetEntry() == m_creature->GetEntry())
            return;

        if (PlayerGUID)
        {
            if (Unit* pPlayer = Unit::GetUnit((*m_creature), PlayerGUID))
                ((Player*)pPlayer)->FailQuest(QUEST_TOREK_ASSULT);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (Rend_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_REND);
            Rend_Timer = 20000;
        }else Rend_Timer -= diff;

        if (Thunderclap_Timer < diff)
        {
            DoCast(m_creature,SPELL_THUNDERCLAP);
            Thunderclap_Timer = 30000;
        }else Thunderclap_Timer -= diff;
    }
};

bool QuestAccept_npc_torek(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_TOREK_ASSULT)
    {
        //TODO: find companions, make them follow Torek, at any time (possibly done by mangos/database in future?)
        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, true, pPlayer->GetGUID());
        DoScriptText(SAY_READY, pCreature, pPlayer);
    }

    return true;
}

CreatureAI* GetAI_npc_torek(Creature* pCreature)
{
    npc_torekAI* thisAI = new npc_torekAI(pCreature);

    thisAI->FillPointMovementListForCreature();

    return (CreatureAI*)thisAI;
}

void AddSC_ashenvale()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_ruul_snowhoof";
    newscript->GetAI = &GetAI_npc_ruul_snowhoofAI;
    newscript->pQuestAccept = &QuestAccept_npc_ruul_snowhoof;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_torek";
    newscript->GetAI = &GetAI_npc_torek;
    newscript->pQuestAccept = &QuestAccept_npc_torek;
    newscript->RegisterSelf();
}

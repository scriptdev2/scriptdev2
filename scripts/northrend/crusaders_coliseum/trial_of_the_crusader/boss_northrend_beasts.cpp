/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName:
SD%Complete: 0
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/*######
## npc_beast_combat_stalker
######*/

enum
{
    SAY_TIRION_BEAST_2                  = -1649005,
    SAY_TIRION_BEAST_3                  = -1649006,
};

// TODO need to script npc 36549 - Beast Combat Stalker, set into combat in begin, destroy on end
struct MANGOS_DLL_DECL npc_beast_combat_stalkerAI : public ScriptedAI
{
    npc_beast_combat_stalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_vSummonedBossGuids.reserve(4);
        Reset();
        m_creature->SetInCombatWithZone();
    }

    ScriptedInstance* m_pInstance;
    GUIDVector m_vSummonedBossGuids;
    bool m_bFirstWormDied;

    void Reset()
    {
        m_bFirstWormDied = true;                            // TODO - change to = false; when second worm handling is implemented.
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);

        for (GUIDVector::const_iterator itr = m_vSummonedBossGuids.begin(); itr != m_vSummonedBossGuids.end(); ++itr)
        {
            if (Creature* pBoss = m_creature->GetMap()->GetCreature(*itr))
                pBoss->ForcedDespawn();
        }

        m_vSummonedBossGuids.clear();
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        m_vSummonedBossGuids.push_back(pSummoned->GetObjectGuid());
        pSummoned->SetInCombatWithZone();                   // TODO correct?
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (!m_pInstance)
            return;

        switch (pSummoned->GetEntry())
        {
            case NPC_GORMOK:
                if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                    DoScriptText(SAY_TIRION_BEAST_2, pTirion);

                m_creature->SummonCreature(NPC_DREADSCALE, aSpawnPositions[2][0], aSpawnPositions[2][1], aSpawnPositions[2][2], aSpawnPositions[2][3], TEMPSUMMON_DEAD_DESPAWN, 0);
                break;

            case NPC_DREADSCALE:
            case NPC_ACIDMAW:
                if (m_bFirstWormDied)
                {
                    if (Creature* pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION_A))
                        DoScriptText(SAY_TIRION_BEAST_3, pTirion);

                    m_creature->SummonCreature(NPC_ICEHOWL, aSpawnPositions[4][0], aSpawnPositions[4][1], aSpawnPositions[4][2], aSpawnPositions[4][3], TEMPSUMMON_DEAD_DESPAWN, 0);
                }
                else
                    m_bFirstWormDied = true;
                break;

            case NPC_ICEHOWL:
                m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
                m_creature->ForcedDespawn();
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_npc_beast_combat_stalker(Creature* pCreature)
{
    return new npc_beast_combat_stalkerAI(pCreature);
}

/*######
## boss_gormok, vehicle driven by 34800 (multiple times)
######*/

struct MANGOS_DLL_DECL boss_gormokAI : public ScriptedAI
{
    boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

/*######
## boss_acidmaw
######*/

struct MANGOS_DLL_DECL boss_acidmawAI : public ScriptedAI
{
    boss_acidmawAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_acidmaw(Creature* pCreature)
{
    return new boss_acidmawAI(pCreature);
}

/*######
## boss_dreadscale
######*/

struct MANGOS_DLL_DECL boss_dreadscaleAI : public ScriptedAI
{
    boss_dreadscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dreadscale(Creature* pCreature)
{
    return new boss_dreadscaleAI(pCreature);
}

/*######
## boss_icehowl
######*/

enum
{
    EMOTE_MASSIVE_CRASH                 = -1649039,
};

struct MANGOS_DLL_DECL boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

void AddSC_northrend_beasts()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_beast_combat_stalker";
    pNewScript->GetAI = &GetAI_npc_beast_combat_stalker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_gormok";
    pNewScript->GetAI = &GetAI_boss_gormok;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_acidmaw";
    pNewScript->GetAI = &GetAI_boss_acidmaw;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_dreadscale";
    pNewScript->GetAI = &GetAI_boss_dreadscale;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_icehowl";
    pNewScript->GetAI = &GetAI_boss_icehowl;
    pNewScript->RegisterSelf();

}

/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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
SDName: grand_champions
SD%Complete: 20
SDComment: Only basics support
SDCategory: Crusader Coliseum, Trial of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"
#include "TransportSystem.h"

enum
{
    // common spells
    SPELL_DEFEND_DUMMY              = 64101,                        // triggers 63130, 63131 and 63132
    SPELL_DEFEND                    = 62719,
};

/*######
## trial_companion_common
######*/

struct trial_companion_commonAI : public ScriptedAI
{
    trial_companion_commonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_champion*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_trial_of_the_champion* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() override
    {
        DoCastSpellIfCan(m_creature, SPELL_DEFEND, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_DEFEND_DUMMY, CAST_TRIGGERED);
    }

    void AttackStart(Unit* pWho) override
    {
        ScriptedAI::AttackStart(pWho);

        // Set Mount control
       if (Creature* pMount = (Creature*)m_creature->GetTransportInfo()->GetTransport())
           pMount->AI()->AttackStart(pWho);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        // no aggro during the intro
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || uiPointId != POINT_ID_HOME || !m_pInstance)
            return;

        if (Creature* pCenterTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_WORLD_TRIGGER))
        {
            pSummoned->SetRespawnCoord(pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ(), pSummoned->GetAngle(pCenterTrigger));
            pSummoned->SetFacingToObject(pCenterTrigger);
        }
    }

    // Return true to handle shared timers and MeleeAttack
    virtual bool UpdateChampionAI(const uint32 /*uiDiff*/) { return true; }

    void UpdateAI(const uint32 uiDiff) override
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Call specific virtual function
        if (!UpdateChampionAI(uiDiff))
            return;

        DoMeleeAttackIfReady();
    }
};

enum
{
    // warrior spells
};

/*######
## boss_champion_warrior
######*/

struct boss_champion_warriorAI : public trial_companion_commonAI
{
    boss_champion_warriorAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_warrior(Creature* pCreature)
{
    return new boss_champion_warriorAI(pCreature);
}

enum
{
    // mage spells
};

/*######
## boss_champion_mage
######*/

struct boss_champion_mageAI : public trial_companion_commonAI
{
    boss_champion_mageAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_mage(Creature* pCreature)
{
    return new boss_champion_mageAI(pCreature);
}

enum
{
    // shaman spells
};

/*######
## boss_champion_shaman
######*/

struct boss_champion_shamanAI : public trial_companion_commonAI
{
    boss_champion_shamanAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_shaman(Creature* pCreature)
{
    return new boss_champion_shamanAI(pCreature);
}

enum
{
    // hunter spells
};

/*######
## boss_champion_hunter
######*/

struct boss_champion_hunterAI : public trial_companion_commonAI
{
    boss_champion_hunterAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateChampionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_hunter(Creature* pCreature)
{
    return new boss_champion_hunterAI(pCreature);
}

enum
{
    // rogue spells
};

/*######
## boss_champion_rogue
######*/

struct boss_champion_rogueAI : public trial_companion_commonAI
{
    boss_champion_rogueAI(Creature* pCreature) : trial_companion_commonAI(pCreature) { Reset(); }

    void Reset() override
    {
        trial_companion_commonAI::Reset();
    }

    bool UpdateCompanionAI(const uint32 uiDiff)
    {
        // ToDo:

        return true;
    }
};

CreatureAI* GetAI_boss_champion_rogue(Creature* pCreature)
{
    return new boss_champion_rogueAI(pCreature);
}

/*######
## npc_champion_mount
######*/

enum
{

};

struct npc_champion_mountAI : public ScriptedAI
{
    npc_champion_mountAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_trial_of_the_champion*)pCreature->GetInstanceData();
        Reset();
    }

    instance_trial_of_the_champion* m_pInstance;

    void Reset() override { }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId) override
    {
        if (uiMotionType != POINT_MOTION_TYPE || !m_pInstance)
            return;

        switch (uiPointId)
        {
            case POINT_ID_CENTER:
                m_pInstance->MoveChampionToHome(m_creature);
                break;
            case POINT_ID_HOME:
                if (Creature* pCenterTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_WORLD_TRIGGER))
                {
                    m_creature->SetRespawnCoord(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetAngle(pCenterTrigger));
                    m_creature->SetFacingToObject(pCenterTrigger);
                }
                m_pInstance->InformChampionReachHome();
                break;
        }
    }

    void MoveInLineOfSight(Unit* /*pWho*/) override { }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_npc_champion_mount(Creature* pCreature)
{
    return new npc_champion_mountAI(pCreature);
}

void AddSC_boss_grand_champions()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_warrior";
    pNewScript->GetAI = &GetAI_boss_champion_warrior;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_mage";
    pNewScript->GetAI = &GetAI_boss_champion_mage;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_shaman";
    pNewScript->GetAI = &GetAI_boss_champion_shaman;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_hunter";
    pNewScript->GetAI = &GetAI_boss_champion_hunter;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_champion_rogue";
    pNewScript->GetAI = &GetAI_boss_champion_rogue;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_champion_mount";
    pNewScript->GetAI = &GetAI_npc_champion_mount;
    pNewScript->RegisterSelf();
}

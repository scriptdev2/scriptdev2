/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/ >
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
SDName: Boss_Supremus
SD%Complete: 95
SDComment: Need to implement molten punch
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

#define EMOTE_NEW_TARGET            -1564010
#define EMOTE_PUNCH_GROUND          -1564011                //DoScriptText(EMOTE_PUNCH_GROUND, m_creature);
#define EMOTE_GROUND_CRACK          -1564012

//Spells
#define SPELL_HURTFUL_STRIKE        41926
#define SPELL_DEMON_FIRE            40029
#define SPELL_MOLTEN_FLAME          40253
#define SPELL_VOLCANIC_ERUPTION     40276
#define SPELL_VOLCANIC_FIREBALL     40118
#define SPELL_VOLCANIC_GEYSER       42055
#define SPELL_MOLTEN_PUNCH          40126
#define SPELL_BERSERK               45078

#define CREATURE_VOLCANO            23085
#define CREATURE_STALKER            23095

struct MANGOS_DLL_DECL molten_flameAI : public ScriptedAI
{
    molten_flameAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint64 SupremusGUID;
    bool TargetLocked;
    uint32 CheckTimer;

    void Reset()
    {
        SupremusGUID = 0;
        TargetLocked = false;

        CheckTimer = 1000;
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit *who)
    {
        if (TargetLocked)
            return;

        // stop it from aggroing players who move in LOS if we have a target.
        if (who && (who != m_creature) && (m_creature->IsWithinDistInMap(who, 10)))
            StalkTarget(who);
    }

    void SetSupremusGUID(uint64 GUID) { SupremusGUID = GUID; }

    void StalkTarget(Unit* target)
    {
        if (!target)
            return;

        m_creature->AddThreat(target, 50000000.0f);
        m_creature->GetMotionMaster()->MoveChase(target);
        DoCast(m_creature, SPELL_DEMON_FIRE, true);
        // DoCast(m_creature, SPELL_MOLTEN_FLAME, true); // This spell damages self, so disabled for now
        TargetLocked = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget())
            return;

        if (m_creature->getVictim() && m_creature->isAlive())
        {
            if (CheckTimer < diff)
            {
                if (SupremusGUID)
                {
                    Unit* Supremus = NULL;
                    Supremus = Unit::GetUnit((*m_creature), SupremusGUID);
                    if (Supremus && (!Supremus->isAlive()))
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
                CheckTimer = 2000;
            }else CheckTimer -= diff;
        }
    }
};

struct MANGOS_DLL_DECL npc_volcanoAI : public ScriptedAI
{
    npc_volcanoAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 CheckTimer;
    uint64 SupremusGUID;
    uint32 FireballTimer;
    uint32 GeyserTimer;

    void Reset()
    {
        CheckTimer = 1000;
        SupremusGUID = 0;
        FireballTimer = 500;
        GeyserTimer = 0;
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void SetSupremusGUID(uint64 guid) { SupremusGUID = guid; }

    void UpdateAI(const uint32 diff)
    {
        if (CheckTimer < diff)
        {
            if (SupremusGUID)
            {
                Unit* Supremus = NULL;
                Supremus = Unit::GetUnit((*m_creature), SupremusGUID);
                if (Supremus && (!Supremus->isAlive()))
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            CheckTimer = 2000;
        }else CheckTimer -= diff;

        if (GeyserTimer < diff)
        {
            DoCast(m_creature, SPELL_VOLCANIC_GEYSER);
            GeyserTimer = 18000;
        }else GeyserTimer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_supremusAI : public ScriptedAI
{
    boss_supremusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 SummonFlameTimer;
    uint32 SwitchTargetTimer;
    uint32 PhaseSwitchTimer;
    uint32 SummonVolcanoTimer;
    uint32 HurtfulStrikeTimer;
    uint32 BerserkTimer;

    bool Phase1;

    void Reset()
    {
        HurtfulStrikeTimer = 5000;
        SummonFlameTimer = 20000;
        SwitchTargetTimer = 90000;
        PhaseSwitchTimer = 60000;
        SummonVolcanoTimer = 5000;
        BerserkTimer = 900000;                              // 15 minute enrage

        Phase1 = true;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SUPREMUS, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SUPREMUS, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SUPREMUS, DONE);
    }

    float CalculateRandomCoord(float initial)
    {
        float coord = 0;

        switch(urand(0, 1))
        {
            case 0: coord = initial + 20 + rand()%20; break;
            case 1: coord = initial - 20 - rand()%20; break;
        }

        return coord;
    }

    Creature* SummonCreature(uint32 entry, Unit* target)
    {
        if (target && entry)
        {
            Creature* Summon = m_creature->SummonCreature(entry, CalculateRandomCoord(target->GetPositionX()), CalculateRandomCoord(target->GetPositionY()), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
            if (Summon)
            {
                Summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                Summon->setFaction(m_creature->getFaction());
                return Summon;
            }
        }
        return NULL;
    }

    Unit* CalculateHurtfulStrikeTarget()
    {
        uint32 health = 0;
        Unit* target = NULL;

        std::list<HostileReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostileReference*>::iterator i = m_threatlist.begin();
        for (i = m_threatlist.begin(); i!= m_threatlist.end();++i)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if (pUnit && m_creature->IsWithinDistInMap(pUnit, ATTACK_DISTANCE))
            {
                if (pUnit->GetHealth() > health)
                {
                    health = pUnit->GetHealth();
                    target = pUnit;
                }
            }
        }

        return target;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(SPELL_BERSERK, 0))
        {
            if (BerserkTimer < diff)
                DoCast(m_creature, SPELL_BERSERK);
            else BerserkTimer -= diff;
        }

        if (SummonFlameTimer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 1);

            if (!target)                                    // someone is trying to solo, set target as current victim.
                target = m_creature->getVictim();

            if (target)
            {
                Creature* MoltenFlame = SummonCreature(CREATURE_STALKER, target);
                if (MoltenFlame)
                {
                    // Invisible model
                    MoltenFlame->SetDisplayId(11686);
                    ((molten_flameAI*)MoltenFlame->AI())->SetSupremusGUID(m_creature->GetGUID());
                    ((molten_flameAI*)MoltenFlame->AI())->StalkTarget(target);
                    SummonFlameTimer = 20000;
                }
            }
        }else SummonFlameTimer -= diff;

        if (Phase1)
        {
            if (HurtfulStrikeTimer < diff)
            {
                Unit* target = CalculateHurtfulStrikeTarget();
                if (target)
                {
                    DoCast(target, SPELL_HURTFUL_STRIKE);
                    HurtfulStrikeTimer = 5000;
                }
            }else HurtfulStrikeTimer -= diff;
        }

        if (!Phase1)
        {
            if (SwitchTargetTimer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                {
                    DoResetThreat();
                    m_creature->AddThreat(target, 5000000.0f);
                    DoScriptText(EMOTE_NEW_TARGET, m_creature);
                    SwitchTargetTimer = 10000;
                }
            }else SwitchTargetTimer -= diff;

            if (SummonVolcanoTimer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);

                if (!target)
                    target = m_creature->getVictim();

                if (target)
                {
                    Creature* Volcano = NULL;
                    Volcano = SummonCreature(CREATURE_VOLCANO, target);

                    if (Volcano)
                    {
                        DoCast(target, SPELL_VOLCANIC_ERUPTION);
                        ((npc_volcanoAI*)Volcano->AI())->SetSupremusGUID(m_creature->GetGUID());
                    }

                    DoScriptText(EMOTE_GROUND_CRACK, m_creature);
                    SummonVolcanoTimer = 10000;
                }
            }else SummonVolcanoTimer -= diff;
        }

        if (PhaseSwitchTimer < diff)
        {
            if (!Phase1)
            {
                Phase1 = true;
                DoResetThreat();
                PhaseSwitchTimer = 60000;
                m_creature->SetSpeed(MOVE_RUN, 1.0f);
            }
            else
            {
                Phase1 = false;
                DoResetThreat();
                SwitchTargetTimer = 10000;
                SummonVolcanoTimer = 2000;
                PhaseSwitchTimer = 60000;
                m_creature->SetSpeed(MOVE_RUN, 0.9f);
            }
        }else PhaseSwitchTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_supremus(Creature* pCreature)
{
    return new boss_supremusAI(pCreature);
}

CreatureAI* GetAI_molten_flame(Creature* pCreature)
{
    return new molten_flameAI(pCreature);
}

CreatureAI* GetAI_npc_volcano(Creature* pCreature)
{
    return new npc_volcanoAI(pCreature);
}

void AddSC_boss_supremus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_supremus";
    newscript->GetAI = &GetAI_boss_supremus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "molten_flame";
    newscript->GetAI = &GetAI_molten_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_volcano";
    newscript->GetAI = &GetAI_npc_volcano;
    newscript->RegisterSelf();
}

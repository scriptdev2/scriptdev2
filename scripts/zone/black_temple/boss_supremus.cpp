/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/ >
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
SD%Complete: 90
SDComment: Using incorrect summon ids that are not official creature ids
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/TargetedMovementGenerator.h"

//Spells
#define SPELL_HURTFUL_STRIKE        33813
#define SPELL_MOLTEN_FLAME        40253
#define SPELL_VOLCANIC_ERUPTION        42055

struct MANGOS_DLL_DECL molten_flameAI : public ScriptedAI
{
    molten_flameAI(Creature *c) : ScriptedAI(c) {SetVariables();}

    uint64 SupremusGUID;
    bool InCombat;
    bool TargetLocked;
    uint32 CheckTimer;

    void SetVariables();
    void EnterEvadeMode();
    void AttackStart(Unit *who);
    void MoveInLineOfSight(Unit *who);
    void UpdateAI(const uint32 diff);
    void SetSupremusGUID(uint64 GUID);
};

void molten_flameAI::SetVariables()
{
    SupremusGUID = 0;
    TargetLocked = false;
    InCombat = false;
    CheckTimer = 1000;

    (*m_creature).GetMotionMaster()->Clear(false);
}

void molten_flameAI::EnterEvadeMode()
{
    InCombat = false;

    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    DoGoHome();
}

void molten_flameAI::AttackStart(Unit *who)
{
    if(!who)
        return;

    if(who->isTargetableForAttack() && who!= m_creature)
    {
        DoStartMeleeAttack(who);
    }
}

void molten_flameAI::MoveInLineOfSight(Unit *who)
{
    return; // stop it from aggroing players who move in LOS.
}

void molten_flameAI::SetSupremusGUID(uint64 guid)
{
    if(guid)
        SupremusGUID = guid;
}

void molten_flameAI::UpdateAI(const uint32 diff)
{
    if (!m_creature->SelectHostilTarget())
        return;

    if(m_creature->getVictim() && m_creature->isAlive())
    {
        if(!TargetLocked)
        {
            (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
            m_creature->CastSpell(m_creature, 40029, true);
            TargetLocked = true;
        }

        if(CheckTimer < diff)
        {
            if(SupremusGUID)
            {
                Unit* Supremus = NULL;
                Supremus = Unit::GetUnit((*m_creature), SupremusGUID);
                if(Supremus && (!Supremus->isAlive()))
                {
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
                }
            }
            CheckTimer = 2000;
        }else CheckTimer -= diff;
    }
}

struct MANGOS_DLL_DECL npc_volcanoAI : public ScriptedAI
{
    npc_volcanoAI(Creature *c) : ScriptedAI(c) {SetVariables();}

    bool InCombat;
    uint32 CheckTimer;
    uint64 SupremusGUID;

    void SetVariables();
    void EnterEvadeMode();
    void AttackStart(Unit *who) {return;}
    void MoveInLineOfSight(Unit *who) {return;}
    void UpdateAI(const uint32 diff);
    void SetSupremusGUID(uint64 guid);
};

void npc_volcanoAI::SetVariables()
{
    InCombat = false;
    CheckTimer = 1000;
    SupremusGUID = 0;
}

void npc_volcanoAI::EnterEvadeMode()
{
    InCombat = false;

    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    DoGoHome();
}

void npc_volcanoAI::SetSupremusGUID(uint64 guid)
{
    if(guid)
        SupremusGUID = guid;
}

void npc_volcanoAI::UpdateAI(const uint32 diff)
{
    if(CheckTimer < diff)
    {
        if(SupremusGUID)
        {
            Unit* Supremus = NULL;
            Supremus = Unit::GetUnit((*m_creature), SupremusGUID);
            if(Supremus && (!Supremus->isAlive()))
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
            }
        }
        CheckTimer = 2000;
    }else CheckTimer -= diff;
}

struct MANGOS_DLL_DECL boss_supremusAI : public ScriptedAI
{
    boss_supremusAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        else pInstance = NULL;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint32 SummonFlame1Timer;
    uint32 SummonFlame2Timer;
    uint32 SwitchTargetTimer;
    uint32 PhaseSwitchTimer;
    uint32 SummonVolcanoTimer;
    uint32 HurtfulStrikeTimer;

    bool InCombat;
    bool Phase1;

    void SetVariables()
    {
        if(pInstance)
            pInstance->SetData("SupremusEvent", 0);

        HurtfulStrikeTimer = 40000;
        SummonFlame1Timer = 20000;
        SummonFlame2Timer = 40000;
        SwitchTargetTimer = 90000;
        PhaseSwitchTimer = 90000;
        SummonVolcanoTimer = 90000;

        Phase1 = true;
        InCombat = false;

        (*m_creature).GetMotionMaster()->Clear(false);
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }


    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                if(pInstance)
                    pInstance->SetData("SupremusEvent", 1);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    if(pInstance)
                        pInstance->SetData("SupremusEvent", 1);
                    InCombat = true;
                }
            }
        }
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData("SupremusEvent", 3);
    }

    void ResetThreat()
    {
        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
        {
            Unit* pUnit = NULL;
            pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit)
            {
                m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
            }
        }
    }

    uint64 SummonCreature(uint32 entry)
    {
        Creature* Summon = NULL;
        uint64 guid = 0;
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(target && entry)
        {
            switch(rand()%4)
            {
            case 0:
                Summon = m_creature->SummonCreature(entry, target->GetPositionX() + 20 + rand()%16, target->GetPositionY() + 20 + rand()%16, target->GetPositionZ(), rand()%7, TEMPSUMMON_TIMED_DESPAWN, 19000);
                break;
            case 1:
                Summon = m_creature->SummonCreature(entry, target->GetPositionX() - 20 - rand()%16, target->GetPositionY() + 20 + rand()%16, target->GetPositionZ(), rand()%7, TEMPSUMMON_TIMED_DESPAWN, 19000);
                break;
            case 2:
                Summon = m_creature->SummonCreature(entry, target->GetPositionX() + 20 + rand()%16, target->GetPositionY() - 20 - rand()%16, target->GetPositionZ(), rand()%7, TEMPSUMMON_TIMED_DESPAWN, 19000);
                break;
            case 3:
                Summon = m_creature->SummonCreature(entry, target->GetPositionX() - 20 - rand()%16, target->GetPositionY() - 20 - rand()%16, target->GetPositionZ(), rand()%7, TEMPSUMMON_TIMED_DESPAWN, 19000);
                break;
            }
            if(Summon)
            {
                Summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                Summon->setFaction(m_creature->getFaction());
                guid = Summon->GetGUID();
            }
        }
        return guid;
    }

    uint64 CalculateHurtfulStrikeTarget()
    {
        uint64 guid = 0;
        uint32 health = 0;

        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for (i = m_threatlist.begin(); i!= m_threatlist.end();++i)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit && m_creature->IsWithinDistInMap(pUnit, ATTACK_DISTANCE))
            {
                if(pUnit->GetHealth() > health)
                {
                    health = pUnit->GetHealth();
                    guid = (*i)->getUnitGuid();
                }
            }
        }

        return guid;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(Phase1)
        {
            if(HurtfulStrikeTimer < diff)
            {
                Unit* target = NULL;
                target = Unit::GetUnit((*m_creature), CalculateHurtfulStrikeTarget());

                if(target && target->isAlive())
                    DoCast(target, SPELL_HURTFUL_STRIKE);
                HurtfulStrikeTimer = 30000 +rand()%31*1000;
            }else HurtfulStrikeTimer -= diff;
        }

        if(!Phase1)
        {
            if(SwitchTargetTimer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                {
                    ResetThreat();
                    m_creature->AddThreat(target, 5000000.0f);
                    DoTextEmote("acquires a new target!", NULL);
                    SwitchTargetTimer = 10000;
                }

            }else SwitchTargetTimer -= diff;

            if(SummonVolcanoTimer < diff)
            {
                Creature* Volcano = NULL;
                Volcano = ((Creature*)Unit::GetUnit((*m_creature), SummonCreature(23085)));
                switch(rand()%3)
                {
                case 0:
                    DoTextEmote("roars and the ground begins to crack open!", NULL);
                    break;
                }
                if(Volcano)
                {
                    ((npc_volcanoAI*)Volcano->AI())->SetSupremusGUID(m_creature->GetGUID());
                    Volcano->CastSpell(Volcano, SPELL_VOLCANIC_ERUPTION, false);
                    SummonVolcanoTimer = 20000;
                }
            }else SummonVolcanoTimer -= diff;
        }

        if(PhaseSwitchTimer < diff)
        {
            if(!Phase1)
            {
                Phase1 = true;
                ResetThreat();
                PhaseSwitchTimer = 90000;
            }
            else
            {
                Phase1 = false;
                ResetThreat();
                SwitchTargetTimer = 10;
                SummonVolcanoTimer = 2000;
                PhaseSwitchTimer = 50000;
            }
        }else PhaseSwitchTimer -= diff;

        if(SummonFlame1Timer < diff)
        {
            Creature* Flame1 = NULL;
            Flame1 = ((Creature*)Unit::GetUnit((*m_creature), SummonCreature(500003)));
            if(Flame1)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                {
                    Flame1->AddThreat(target, 50000.0f);
                    ((molten_flameAI*)Flame1->AI())->SetSupremusGUID(m_creature->GetGUID());
                }

            }
            SummonFlame1Timer = 40000;
        }else SummonFlame1Timer -= diff;

        if(SummonFlame2Timer < diff)
        {
            Creature* Flame2 = NULL;
            Flame2 = ((Creature*)Unit::GetUnit((*m_creature), SummonCreature(500003)));
            if(Flame2)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                {
                    Flame2->AddThreat(target, 50000.0f);
                    ((molten_flameAI*)Flame2->AI())->SetSupremusGUID(m_creature->GetGUID());
                }

            }
            SummonFlame2Timer = 40000;
        }else SummonFlame2Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_supremus(Creature *_Creature)
{
    return new boss_supremusAI (_Creature);
}

CreatureAI* GetAI_molten_flame(Creature *_Creature)
{
    return new molten_flameAI (_Creature);
}

CreatureAI* GetAI_npc_volcano(Creature *_Creature)
{
    return new npc_volcanoAI (_Creature);
}

void AddSC_boss_supremus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_supremus";
    newscript->GetAI = GetAI_boss_supremus;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="molten_flame";
    newscript->GetAI = GetAI_molten_flame;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_volcano";
    newscript->GetAI = GetAI_npc_volcano;
    m_scripts[nrscripts++] = newscript;
}

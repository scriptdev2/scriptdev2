/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/ >
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
SD%Complete: 98
SDComment: Missing Enrage
SDCategory: Black Temple
EndScriptData */

#include "def_black_temple.h"
#include "TargetedMovementGenerator.h"

//Spells
#define SPELL_HURTFUL_STRIKE        33813
#define SPELL_MOLTEN_FLAME          40253
#define SPELL_VOLCANIC_ERUPTION     40276
#define SPELL_VOLCANIC_FIREBALL     40118
#define SPELL_VOLCANIC_GEYSER       42055
#define SPELL_MOLTEN_PUNCH          40126

struct MANGOS_DLL_DECL molten_flameAI : public ScriptedAI
{
    molten_flameAI(Creature *c) : ScriptedAI(c) 
    {
        Reset();
    }

    uint64 SupremusGUID;
    bool InCombat;
    bool TargetLocked;
    uint32 CheckTimer;

    void Reset();
    void Aggro(Unit *who);
    void MoveInLineOfSight(Unit *who);
    void UpdateAI(const uint32 diff);
    void SetSupremusGUID(uint64 GUID);
    void StalkTarget(Unit* target);
};

void molten_flameAI::Reset()
{
    SupremusGUID = 0;
    TargetLocked = false;
    InCombat = false;
    CheckTimer = 1000;
}

void molten_flameAI::Aggro(Unit *who)
{
}

void molten_flameAI::MoveInLineOfSight(Unit *who)
{
    if(TargetLocked)
        return; // stop it from aggroing players who move in LOS if we have a target.
    if(who && (who != m_creature) && (m_creature->IsWithinDistInMap(who, 10)))
    {
        m_creature->AddThreat(who, 50000000.0f);
        (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*who));
        m_creature->CastSpell(m_creature, 40029, true);
        TargetLocked = true;
    }
}

void molten_flameAI::SetSupremusGUID(uint64 guid)
{
    if(guid)
        SupremusGUID = guid;
}

void molten_flameAI::StalkTarget(Unit *target)
{
    if(!target) return;

    m_creature->AddThreat(target, 50000000.0f);
    (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
    m_creature->CastSpell(m_creature, 40029, true);
    TargetLocked = true;
}

void molten_flameAI::UpdateAI(const uint32 diff)
{
    if (!m_creature->SelectHostilTarget())
        return;

    if(m_creature->getVictim() && m_creature->isAlive())
    {
        if(CheckTimer < diff)
        {
            if(SupremusGUID)
            {
                Unit* Supremus = NULL;
                Supremus = Unit::GetUnit((*m_creature), SupremusGUID);
                if(Supremus && (!Supremus->isAlive()))
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
            }
            CheckTimer = 2000;
        }else CheckTimer -= diff;
    }
}

struct MANGOS_DLL_DECL npc_volcanoAI : public ScriptedAI
{
    npc_volcanoAI(Creature *c) : ScriptedAI(c) {Reset();}

    bool InCombat;
    uint32 CheckTimer;
    uint64 SupremusGUID;
    uint32 FireballTimer;
    uint32 GeyserTimer;

    void Reset();
    void Aggro(Unit *who) {return;}
    void MoveInLineOfSight(Unit *who) {return;}
    void UpdateAI(const uint32 diff);
    void SetSupremusGUID(uint64 guid);
    float CalculateRandomCoord();
};

void npc_volcanoAI::Reset()
{
    InCombat = false;
    CheckTimer = 1000;
    SupremusGUID = 0;
    FireballTimer = 500;
    GeyserTimer = 0;
}

void npc_volcanoAI::SetSupremusGUID(uint64 guid)
{
    if(guid)
        SupremusGUID = guid;
}

float npc_volcanoAI::CalculateRandomCoord()
{
    float coord = 0;
    switch(rand()%2)
    {
        case 0:
            coord += rand()%7;
            break;
        case 1:
            coord -= rand()%7;
            break;
    }
    return coord;
}

void npc_volcanoAI::UpdateAI(const uint32 diff)
{
    // Workaround for the visual of the fireball that is spammed around the volcano.
    if(FireballTimer < diff)
    {
        float X = CalculateRandomCoord();
        float Y = CalculateRandomCoord();
        Creature* Target = DoSpawnCreature(24550, X, Y, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 600);
        if(Target)
        {
            Target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686); // Make it invisible
            DoCast(Target, SPELL_VOLCANIC_FIREBALL);
            Target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
        FireballTimer = 500;
    }else FireballTimer -= diff;

    if(CheckTimer < diff)
    {
        if(SupremusGUID)
        {
            Unit* Supremus = NULL;
            Supremus = Unit::GetUnit((*m_creature), SupremusGUID);
            if(Supremus && (!Supremus->isAlive()))
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
        }
        CheckTimer = 2000;
    }else CheckTimer -= diff;

    if(GeyserTimer < diff)
    {
        DoCast(m_creature, SPELL_VOLCANIC_GEYSER);
        GeyserTimer = 18000;
    }else GeyserTimer -= diff;
}

struct MANGOS_DLL_DECL boss_supremusAI : public ScriptedAI
{
    boss_supremusAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 SummonFlameTimer;
    uint32 SwitchTargetTimer;
    uint32 PhaseSwitchTimer;
    uint32 SummonVolcanoTimer;
    uint32 HurtfulStrikeTimer;

    bool InCombat;
    bool Phase1;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(DATA_SUPREMUSEVENT, 0);

        HurtfulStrikeTimer = 20000;
        SummonFlameTimer = 20000;
        SwitchTargetTimer = 90000;
        PhaseSwitchTimer = 60000;
        SummonVolcanoTimer = 5000;

        Phase1 = true;
        InCombat = false;
    }

    void Aggro(Unit *who)
    {

                if(pInstance)
                    pInstance->SetData(DATA_SUPREMUSEVENT, 1);
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
                        pInstance->SetData(DATA_SUPREMUSEVENT, 1);
                    InCombat = true;
                }
            }
        }
    }

    void JustDied(Unit *killer)
    {
        InCombat = false;

        if(pInstance)
            pInstance->SetData(DATA_SUPREMUSEVENT, 3);
    }

    uint32 CalculateRandomCoord(float initial)
    {
        float coord = 0;

        switch(rand()%2)
        {
            case 0: coord = initial + 20 + rand()%20; break;
            case 1: coord = initial - 20 - rand()%20; break;
        }

        return coord;
    }

    Creature* SummonCreature(uint32 entry, Unit* target)
    {
        if(target && entry)
        {
            Creature* Summon = m_creature->SummonCreature(entry, CalculateRandomCoord(target->GetPositionX()), CalculateRandomCoord(target->GetPositionY()), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
            if(Summon)
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
                    target = pUnit;
                }
            }
        }

        return target;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(Phase1)
        {
            if(HurtfulStrikeTimer < diff)
            {
                DoCast(CalculateHurtfulStrikeTarget(), SPELL_HURTFUL_STRIKE);
                HurtfulStrikeTimer = 20000 +rand()%31*1000;
            }else HurtfulStrikeTimer -= diff;

            if(SummonFlameTimer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 1);
                if(target)
                {
                    Creature* MoltenFlame = NULL;
                    MoltenFlame = SummonCreature(23095, target);
                    if(MoltenFlame)
                    {
                        ((molten_flameAI*)MoltenFlame->AI())->SetSupremusGUID(m_creature->GetGUID());
                        ((molten_flameAI*)MoltenFlame->AI())->StalkTarget(target);
                        SummonFlameTimer = 20000;
                    }
                }
            }else SummonFlameTimer -= diff;
        }

        if(!Phase1)
        {
            if(SwitchTargetTimer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                {
                    DoResetThreat();
                    m_creature->AddThreat(target, 5000000.0f);
                    DoTextEmote("acquires a new target!", NULL);
                    SwitchTargetTimer = 10000;
                }

            }else SwitchTargetTimer -= diff;

            if(SummonVolcanoTimer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);
                if(!target) return;

                Creature* Volcano = NULL;
                Volcano = SummonCreature(23085, target);

                if(Volcano)
                {
                    DoCast(target, SPELL_VOLCANIC_ERUPTION);
                    ((npc_volcanoAI*)Volcano->AI())->SetSupremusGUID(m_creature->GetGUID());
                }
                            
                switch(rand()%3)
                {
                    case 0:
                        DoTextEmote("roars and the ground begins to crack open!", NULL);
                        break;
                }
                SummonVolcanoTimer = 10000;
            }else SummonVolcanoTimer -= diff;
        }

        if(PhaseSwitchTimer < diff)
        {
            if(!Phase1)
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
                m_creature->SetSpeed(MOVE_RUN, 0.5f);
            }
        }else PhaseSwitchTimer -= diff;

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

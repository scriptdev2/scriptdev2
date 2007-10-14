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
#include "../../creature/simple_ai.h"


// Adds NYI

// Garr spells
#define SPELL_ANTIMAGICPULSE        19492
#define SPELL_MAGMASHACKLES         19496
#define SPELL_ENRAGE                19516   //Stacking enrage (stacks to 10 times)

//Add spells
#define SPELL_ERUPTION              19497
#define SPELL_IMMOLATE              20294

struct MANGOS_DLL_DECL boss_garrAI : public ScriptedAI
{
    boss_garrAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 AntiMagicPulse_Timer;
    uint32 MagmaShackles_Timer;
    uint32 CheckAdds_Timer;
    uint64 Add[8];
    bool Enraged[8];
    bool InCombat;

    void EnterEvadeMode()
    {
        AntiMagicPulse_Timer = 25000;      //These times are probably wrong
        MagmaShackles_Timer = 15000;
        CheckAdds_Timer = 2000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
                InCombat = true;

            }
        }
    }

    void SpawnAdds()
    {
        for (int32 i = 0; i < 8; i++)
        {
            Unit* pUnit = NULL; 
            pUnit = DoSpawnCreature(12099, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 1000);

            if (pUnit)
                Add[i] = pUnit->GetGUID();

            Enraged[i] = false;
        }

    }

    void CheckAdds()
    {
        for (int32 i = 0; i < 8; i++)
        {
            if (Enraged[i])
                continue;

            if (Add[i])
            {
                Unit* pUnit = NULL; 
                pUnit = Unit::GetUnit((*m_creature), Add[i]);

                if (pUnit && pUnit->isAlive())
                    continue;
            }

            //Add isn't alive so enrage once
            DoCast(m_creature, SPELL_ENRAGE);
            Enraged[i] = true;
        }

    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //AntiMagicPulse_Timer
        if (AntiMagicPulse_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_ANTIMAGICPULSE);

            //14-18 seconds until we should cast this agian
            AntiMagicPulse_Timer = 14000 + rand()%4000;
        }else AntiMagicPulse_Timer -= diff;

        //MagmaShackles_Timer
        if (MagmaShackles_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_MAGMASHACKLES);

            //8-12 seconds until we should cast this agian
            MagmaShackles_Timer = 8000 + rand()%4000;
        }else MagmaShackles_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_garr(Creature *_Creature)
{
    return new boss_garrAI (_Creature);
}


CreatureAI* GetAI_mob_firesworn(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //Cast eruption upon death
    ai->Death_Spell = SPELL_ERUPTION;
    ai->Death_Target_Type = CAST_SELF;  //Self

    //Cast eruption upon death
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
    ai->Spell[0].Cooldown = 7000;
    ai->Spell[0].First_Cast = 7000;
    ai->Spell[0].Spell_Id = SPELL_IMMOLATE;

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_boss_garr()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_garr";
    newscript->GetAI = GetAI_boss_garr;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_firesworn";
    newscript->GetAI = GetAI_mob_firesworn;
    m_scripts[nrscripts++] = newscript;
}

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

// Spells of Jandice Barov

#define SPELL_CURSEOFBLOOD          24673
//#define SPELL_ILLUSION              17773


//Spells of Illusion of Jandice Barov

#define SPELL_CLEAVE                15584


struct MANGOS_DLL_DECL boss_jandicebarovAI : public ScriptedAI
{
    boss_jandicebarovAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 CurseOfBlood_Timer;
    uint32 Illusion_Timer;
    //uint32 Illusioncounter;
    uint32 Invisible_Timer;
    bool InCombat;
    bool Invisible;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {
        CurseOfBlood_Timer = 15000;
        Illusion_Timer = 30000;
        Invisible_Timer = 3000;                             //Too much too low?
        InCombat = false;
        Invisible = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
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

    void SummonIllusions(Unit* victim)
    {
        Rand = rand()%10;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%10;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(11439, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Invisible && Invisible_Timer < diff)
        {
            //Become visible again 
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11073);     //Jandice Model
            Invisible = false;
        } else if (Invisible)
        {
            Invisible_Timer -= diff;
            //Do nothing while invisible
            return;
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //CurseOfBlood_Timer
        if (CurseOfBlood_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CURSEOFBLOOD);

            //45 seconds
            CurseOfBlood_Timer = 30000;
        }else CurseOfBlood_Timer -= diff;

        //Illusion_Timer
        if (!Invisible && Illusion_Timer < diff)
        {

            //Inturrupt any spell casting
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            m_creature->setFaction(35);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model

            //Summon 10 Illusions attacking random gamers
            Unit* target = NULL;
            for(int i = 0; i < 10;i++)
            {
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                SummonIllusions(target);
            }
            Invisible = true;
            Invisible_Timer = 3000;

            //25 seconds until we should cast this agian
            Illusion_Timer = 25000;
        }else Illusion_Timer -= diff;


        //            //Illusion_Timer
        //            if (Illusion_Timer < diff)
        //            {
        //                  //Cast
        //                DoCast(m_creature->getVictim(),SPELL_ILLUSION);
        //                                
        //                  //3 Illusion will be summoned
        //                  if (Illusioncounter < 3)
        //                  {
        //                    Illusion_Timer = 500;
        //                    Illusioncounter++;
        //                  }
        //                  else {
        //                      //15 seconds until we should cast this again
        //                      Illusion_Timer = 15000;
        //                      Illusioncounter=0;
        //                  }
        //                  
        //            }else Illusion_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

// Illusion of Jandice Barov Script

struct MANGOS_DLL_DECL mob_illusionofjandicebarovAI : public ScriptedAI
{
    mob_illusionofjandicebarovAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Cleave_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Cleave_Timer = 4000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, IMMUNE_DAMAGE_MAGIC, true);
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

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //5-8 seconds
            Cleave_Timer = 5000 + rand()%3000;
        }else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 


CreatureAI* GetAI_boss_jandicebarov(Creature *_Creature)
{
    return new boss_jandicebarovAI (_Creature);
}

CreatureAI* GetAI_mob_illusionofjandicebarov(Creature *_Creature)
{
    return new mob_illusionofjandicebarovAI (_Creature);
}


void AddSC_boss_jandicebarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_jandice_barov";
    newscript->GetAI = GetAI_boss_jandicebarov;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_illusionofjandicebarov";
    newscript->GetAI = GetAI_mob_illusionofjandicebarov;
    m_scripts[nrscripts++] = newscript;
}

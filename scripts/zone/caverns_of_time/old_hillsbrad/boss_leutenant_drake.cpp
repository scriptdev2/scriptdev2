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

#include "../../../sc_defines.h"

//Spells
#define WHIRLWIND         40236
#define FEAR              33789
#define MORTAL_STRIKE     40220
#define EXPLODIG_SHOUT    33792

//Yells
#define  SAY_ENTER           ""
#define  SOUND_ENTER         10428

#define  SAY_AGGRO           "I know what you're up to, and I mean to put an end to it, permanently!" 
#define  SOUND_AGGRO         10429

#define  SAY_SLAY1           "No more middling for you." 
#define  SOUND_SLAY1         10432
#define  SAY_SLAY2           "You will not interfere!"
#define  SOUND_SLAY2         10433

#define  SAY_DIE             "Thrall... must not... go free."
#define  SOUND_DIE           10434

#define  SAY_MORTAL          "Time to bleed!" 
#define  SOUND_MORTAL        10430

#define  SAY_SHOUT           "Run, you blasted cowards!"
#define  SOUND_SHOUT         10431

struct MANGOS_DLL_DECL boss_lieutenant_drakeAI : public ScriptedAI
{
    boss_lieutenant_drakeAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 Whirlwind_Timer;
    uint32 Fear_Timer;
    uint32 Motral_Strike_Timer;
    uint32 Exploding_Shout_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Whirlwind_Timer = 20000;
        Fear_Timer = 30000;
        Motral_Strike_Timer = 45000;
        Exploding_Shout_Timer = 25000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            DoStartMeleeAttack(who);

            //Boss Aggro Yells
            if (!InCombat)
            {
                InCombat = true;
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
            }
        }
    }    

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;
        case 1:
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            break;
        }
    } 

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DIE,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_DIE);
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

                //Boss Aggro Yells
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Whirlwind
        if (Whirlwind_Timer < diff)
        {
            DoCast(m_creature->getVictim(),WHIRLWIND);

            Whirlwind_Timer = 20000+rand()%5000;
        }else Whirlwind_Timer -= diff;

        //Fear
        if (Fear_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)
                DoCast(target,FEAR);

            Fear_Timer = 30000+rand()%10000;
        }else Fear_Timer -= diff;

        //Mortal Strike
        if (Motral_Strike_Timer < diff)
        {
            DoYell(SAY_MORTAL, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_MORTAL);
            DoCast(m_creature->getVictim(),MORTAL_STRIKE);

            Motral_Strike_Timer = 45000+rand()%5000;
        }else Motral_Strike_Timer -= diff;

        /*
        //This only enabled on heroic?
        //Exploding Shout
        if (m_creature->IsHeroicCreature())
        {
        if (Exploding_Shout_Timer < diff)
        {
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        if (target)
        DoYell(SAY_SHOUT, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SHOUT);
        DoCast(target,EXPLODING_SHOUT);
        Exploding_Shout_Timer = 25000+rand()%5000;
        }else Exploding_Shout_Timer -= diff;    
        }
        */

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lieutenant_drake(Creature *_Creature)
{
    return new boss_lieutenant_drakeAI (_Creature);
}


void AddSC_boss_lieutenant_drake()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lieutenant_drake";
    newscript->GetAI = GetAI_boss_lieutenant_drake;
    m_scripts[nrscripts++] = newscript;
}
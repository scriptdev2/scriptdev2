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

#define  SAND_BREATH     31478
#define  TIME_STOP       31422
#define  FRENZY          19812

#define SAY_ENTER             "The time has come to shatter this clockwork universe forever! Let us no longer be slaves of the hourglass! I warn you: those who do not embrace the greater path shall become victims of its passing!"
#define SOUND_ENTER           10400

#define SAY_AGGRO             "Let us see what fate lays in store..."
#define SOUND_AGGRO           10402

#define SAY_BANISH            "Your time is up, slave of the past!"
#define SOUND_BANISH          10401

#define SAY_DIE               "It is only a matter...of time."
#define SOUND_DIE             10405

#define SAY_SLAY1             "One less obstacle in our way!"
#define SOUND_SLAY1           10403
#define SAY_SLAY2             "No one can stop us! No one!"
#define SOUND_SLAY2           10404

struct MANGOS_DLL_DECL boss_aeonusAI : public ScriptedAI
{
    boss_aeonusAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 Sand_Breath_Timer;
    uint32 Time_Stop_Timer; 
    uint32 Frenzy_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Sand_Breath_Timer = 30000;
        Time_Stop_Timer = 40000;
        Frenzy_Timer = 120000;
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

    void JustDied(Unit *victim)
    {
        //Just Died
        DoYell(SAY_DIE,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_DIE);
    }

    void KilledUnit(Unit *victim)
    {
        //Killed Unit
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

        //Sand Breath
        if (Sand_Breath_Timer < diff)
        {
            Unit* target = NULL;
            target = m_creature->getVictim();
            if (target)
                DoCast(target,SAND_BREATH);
            Sand_Breath_Timer = 30000;
        }else Sand_Breath_Timer -= diff;

        //Time Stop 
        if (Time_Stop_Timer < diff)
        {
            DoYell(SAY_BANISH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_BANISH);
            DoCast(m_creature->getVictim(),TIME_STOP);
            Time_Stop_Timer = 40000;
        }else Time_Stop_Timer -= diff;

        //Frenzy
        if (Frenzy_Timer < diff)
        {
            DoCast(m_creature,FRENZY);
            Frenzy_Timer = 120000;
        }else Frenzy_Timer -= diff;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_aeonus(Creature *_Creature)
{
    return new boss_aeonusAI (_Creature);
}


void AddSC_boss_aeonus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_aeonus";
    newscript->GetAI = GetAI_boss_aeonus;
    m_scripts[nrscripts++] = newscript;
}
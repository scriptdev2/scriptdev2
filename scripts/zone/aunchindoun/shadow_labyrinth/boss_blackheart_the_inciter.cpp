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
#include "../../../creature/simple_ai.h"

/* ScriptData
SDName: Boss_Blackheart_the_Inciter
SD%Complete: 75
SDComment: Incite Chaos NYI since core lacks Mind Control support
EndScriptData */

//#define SPELL_INCITE_CHAOS    33684
#define SPELL_CHARGE          24408
#define SPELL_KNOCKBACK       37317

#define SAY_AGGRO1            "You be dead people!"
#define SAY_AGGRO2            "Time to kill!"
#define SAY_AGGRO3            "I see dead people!"
#define SAY_SLAY1             "No coming back for you!"
#define SAY_SLAY2             "Nice try!"
#define SAY_SLAY3             "Now you gone for good!"
#define SAY_DEATH             "This...no...good.."

#define SOUND_AGGRO1          10498
#define SOUND_AGGRO2          10497
#define SOUND_AGGRO3          10488
#define SOUND_SLAY1           10489
#define SOUND_SLAY2           10490
#define SOUND_SLAY3           10499
#define SOUND_DEATH           10491

struct MANGOS_DLL_DECL boss_blackheart_the_inciterAI : public ScriptedAI
{
    boss_blackheart_the_inciterAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Charge_Timer;
    uint32 Knockback_Timer;

    bool InCombat;

    void EnterEvadeMode()
    {
        Charge_Timer = 20000;
        Knockback_Timer = 15000;

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true); 
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        if(pInstance)
            pInstance->SetData("BlackheartTheInciterEvent", 0);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0:
            DoYell(SAY_SLAY1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;

            case 1:
            DoYell(SAY_SLAY2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            break;

            case 2:
            DoYell(SAY_SLAY3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY3);
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if(pInstance)
            pInstance->SetData("BlackheartTheInciterEvent", 2);
    }

    void StartEvent()
    {
        switch(rand()%3)
        {
            case 0:
            DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
            break;

            case 1:
            DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
            break;

            case 2:
            DoYell(SAY_AGGRO3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO3);
            break;
        }

        InCombat = true;

        if(pInstance)
            pInstance->SetData("BlackheartTheInciterEvent", 1);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            if(!InCombat)
                StartEvent();
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
                if(!InCombat)
                    StartEvent();
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Charge_Timer
        if(Charge_Timer < diff)
        {
            Unit *target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_CHARGE);

            Charge_Timer = 25000;
        }else Charge_Timer -= diff;

        //Knockback_Timer
        if(Knockback_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_KNOCKBACK);
            Knockback_Timer = 20000;
        }else Knockback_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_blackheart_the_inciter(Creature *_Creature)
{
    return new boss_blackheart_the_inciterAI (_Creature);
}

void AddSC_boss_blackheart_the_inciter()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_blackheart_the_inciter";
    newscript->GetAI = GetAI_boss_blackheart_the_inciter;
    m_scripts[nrscripts++] = newscript;
}
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
#define SAND_BREATH         20717
#define IMPENDING_DOOM      19702
#define KNOCKBACK           26027
#define MDA                 33834

//Yells
#define  SAY_ENTER1           "Thrall! Come outside and face your fate!"
#define  SOUND_ENTER1         10418
#define  SAY_ENTER2           "Taretha's life hangs in the balance. Surely you care for her. Surely you wish to save her..."
#define  SOUND_ENTER2         10419
#define  SAY_ENTER3           "Ah, there you are. I had hoped to accomplish this with a bit of subtlety, but I suppose direct confrontation was inevitable. Your future, Thrall, must not come to pass and so...you and your troublesome friends must die!"
#define  SOUND_ENTER3         10420

#define  SAY_AGGRO1           "Enough! I will erase your very existence!"
#define  SOUND_AGGRO1         10421
#define  SAY_AGGRO2           "You cannot fight fate!"
#define  SOUND_AGGRO2         10422

#define  SAY_SLAY1            "You are...irrelevant."
#define  SOUND_SLAY1          10425
#define  SAY_SLAY2            "Thrall will remain a slave. Taretha will die. You have failed."
#define  SOUND_SLAY2          10426

#define  SAY_DIE              "No!...The master... will not... be pleased."
#define  SOUND_DIE            10427

#define  SAY_BREATH1          "Not so fast!" 
#define  SOUND_BREATH1        10423
#define  SAY_BREATH2          "Struggle as much as you like!"
#define  SOUND_BREATH2        10424

struct MANGOS_DLL_DECL boss_epoch_hunterAI : public ScriptedAI
{
    boss_epoch_hunterAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 Sand_Breath_Timer;
    uint32 Impending_Doom_Timer;
    uint32 Knockback_Timer;
    uint32 Mda_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Sand_Breath_Timer = 25000;
        Impending_Doom_Timer = 30000;
        Knockback_Timer = 35000;
        Mda_Timer = 40000;

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
                switch(rand()%2)
                {
                case 0:
                    DoYell( SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                    break;
                case 1:
                    DoYell( SAY_AGGRO2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                    break;
                }
            }
        }
    }    

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell( SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;
        case 1:
            DoYell( SAY_SLAY2,LANG_UNIVERSAL,NULL);
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
                    InCombat = true;
                    switch(rand()%2)
                    {
                    case 0:
                        DoYell( SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                        break;
                    case 1:
                        DoYell( SAY_AGGRO2,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                        break;
                    }
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
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                {
                    DoCast(target,SAND_BREATH);
                    switch(rand()%2)
                    {
                    case 0:
                        DoYell( SAY_BREATH1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_BREATH1);
                        break;
                    case 1:
                        DoYell( SAY_BREATH2,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_BREATH2);
                        break;
                    }
                }
                Sand_Breath_Timer = 25000+rand()%5000;
            }else Sand_Breath_Timer -= diff;

            //Impending Doom
            if (Impending_Doom_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                    DoCast(target,IMPENDING_DOOM);
                Impending_Doom_Timer = 30000+rand()%5000;
            }else Impending_Doom_Timer -= diff;

            //Knockback
            if (Knockback_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                    DoCast(target,KNOCKBACK);
                Knockback_Timer = 35000+rand()%10000;
            }else Knockback_Timer -= diff;

            //Mortal Strike
            if (Mda_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (target)
                    DoCast(target,MDA);

                Mda_Timer = 40000+rand()%10000;
            }else Mda_Timer -= diff;

            DoMeleeAttackIfReady();
        }
};

CreatureAI* GetAI_boss_epoch_hunter(Creature *_Creature)
{
    return new boss_epoch_hunterAI (_Creature);
}


void AddSC_boss_epoch_hunter()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_epoch_hunter";
    newscript->GetAI = GetAI_boss_epoch_hunter;
    m_scripts[nrscripts++] = newscript;
}
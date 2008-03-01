/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Doomwalker
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"
//--------------------------------------
//Spells
#define SPELL_SUNDER_ARMOR          30901

#define SPELL_CHAIN_LIGHTNING       33665

#define SPELL_OVERRUN               32636
#define SAY_OVERRUN_1               "Trajectory locked."
#define SOUND_OVERRUN_1             11347
#define SAY_OVERRUN_2               "Engage maximum speed."
#define SOUND_OVERRUN_2             11348

#define SPELL_ENRAGE                34624

#define SPELL_MARK_DEATH            37128

#define SPELL_EARTHQUAKE            32686
#define SAY_EARTHQUAKE_1            "Tectonic disruption commencing."
#define SOUND_EARTHQUAKE_1          11345
#define SAY_EARTHQUAKE_2            "Magnitude set. Release."
#define SOUND_EARTHQUAKE_2          11346
//---------------------------------------
//Aggro
#define SAY_AGGRO                   "Do not proceed. You will be eliminated!"
#define SOUND_AGGRO                 11344
//---------------------------------------
//Slay
#define SAY_SLAY_1                  "Threat level zero."
#define SOUND_SLAY_1                11349
#define SAY_SLAY_2                  "Directive accomplished."
#define SOUND_SLAY_2                11350
#define SAY_SLAY_3                  "Target exterminated."
#define SOUND_SLAY_3                11351
//---------------------------------------
//Death
#define SAY_DEATH                   "System failure in five... four..."
#define SOUND_DEATH                 11352

struct MANGOS_DLL_DECL boss_doomwalkerAI : public ScriptedAI
{
    boss_doomwalkerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}


    uint32 Chain_Timer;
    uint32 Enrage_Timer; 
    uint32 Overrun_Timer;
    uint32 Quake_Timer; 
    uint32 Armor_Timer;

    bool InCombat;
    bool InEnrage;

    void EnterEvadeMode()
    {
        Enrage_Timer    = 0;
        Armor_Timer     = 10000;
        Chain_Timer     = 20000;
        Quake_Timer     = 60000;
        Overrun_Timer   = 120000;

        InCombat = false;
        InEnrage = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_KNOCKOUT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SHACKLE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_TURN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);

    }

    void KilledUnit(Unit* Victim)
    {
        if(rand()%5)
            return;

        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_SLAY_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY_1);
            break;
        case 1:
            DoYell(SAY_SLAY_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY_2);
            break;
        case 2:
            DoYell(SAY_SLAY_3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY_3);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            if (!InCombat)
            {
                DoFaceTarget(m_creature->getVictim());
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim()) return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura()) who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    DoFaceTarget(m_creature->getVictim());
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Spell Enrage   
            if (((m_creature->GetHealth()*100)/ m_creature->GetMaxHealth()) <= 20)//when hp <= 20% gain enrage
            {   
                if(Enrage_Timer < diff)
                {
                    DoCast(m_creature,SPELL_ENRAGE);
                    Enrage_Timer = 6000;
                    InEnrage = true;
                }else Enrage_Timer -= diff;
            }

            //Spell Overrun
            if (Overrun_Timer < diff)
            {   
                switch(rand()%2)
                {
                case 0:
                    DoYell(SAY_OVERRUN_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_OVERRUN_1);
                    break;
                case 1:
                    DoYell(SAY_OVERRUN_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_OVERRUN_2);
                    break;
                }
                DoCast(m_creature->getVictim(),SPELL_OVERRUN);
                Overrun_Timer = (30 + rand()% 40) * 1000;//30-70sec cooldown

            }else Overrun_Timer -= diff;

            //Spell Earthquake
            if (Quake_Timer < diff)
            {
                if (rand()%2)
                    return;

                switch(rand()%2)
                {
                case 0:
                    DoYell(SAY_EARTHQUAKE_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_EARTHQUAKE_1);
                    break;
                case 1:
                    DoYell(SAY_EARTHQUAKE_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_EARTHQUAKE_2);
                    break;
                }
                if(InEnrage)
                {
                    m_creature->RemoveAura(SPELL_ENRAGE, 0);//remove enrage before casting earthquake because enrage + earthquake = 16000dmg over 8sec and all dead
                }
                DoCast(m_creature,SPELL_EARTHQUAKE);
                Quake_Timer = (70 + rand()% 30) * 1000;//70-100sec cooldown
            }else Quake_Timer -= diff;

            //Spell Chain Lightning
            if (Chain_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_CHAIN_LIGHTNING);
                Chain_Timer = (50 + rand()% 50) * 1000;//50-100sec cooldown
            }else Chain_Timer -= diff;

            //Spell Sunder Armor        
            if (Armor_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SUNDER_ARMOR);
                Armor_Timer = (15 + rand()% 7) * 1000;//15-23sec cooldown about 70 proc to stack
            }else Armor_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 

CreatureAI* GetAI_boss_doomwalker(Creature *_Creature)
{
    return new boss_doomwalkerAI (_Creature);
}

void AddSC_boss_doomwalker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_doomwalker";
    newscript->GetAI = GetAI_boss_doomwalker;
    m_scripts[nrscripts++] = newscript;
}

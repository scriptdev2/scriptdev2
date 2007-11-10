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
#define    ARCANE_BLAST    24857
#define    TIME_LAPSE      31467
#define    MAGNETIC_PULL   28337 //Not Implantened (Heroic mod)

#define SAY_ENTER             "Why do you aid the Magus? Just think of how many lives could be saved if the portal is never opened, if the resulting wars could be erased ..."
#define SOUND_ENTER           10412

#define SAY_AGGRO             "If you will not cease this foolish quest, then you will die!"
#define SOUND_AGGRO           10414

#define SAY_BANISH            "You have outstayed your welcome, Timekeeper. Begone!"
#define SOUND_BANISH          10413

#define SAY_DIE               "Time ... is on our side."
#define SOUND_DIE             10417

#define SAY_SLAY1             "I told you it was a fool's quest!"
#define SOUND_SLAY1           10415
#define SAY_SLAY2             "Leaving so soon?"
#define SOUND_SLAY2           10416

struct MANGOS_DLL_DECL boss_chrono_lord_dejaAI : public ScriptedAI
{
    boss_chrono_lord_dejaAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 Arcane_Blast_Timer;
    uint32 Time_Lapse_Timer;
    bool InCombat;

    void EnterEvadeMode()
    { 
        Arcane_Blast_Timer = 20000;
        Time_Lapse_Timer = 15000;
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

            if (!InCombat)
            {
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
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

        //Arcane Blast
        if (Arcane_Blast_Timer < diff)
        {
            DoCast(m_creature->getVictim(), ARCANE_BLAST);
            Arcane_Blast_Timer = 20000+rand()%5000;
        }else Arcane_Blast_Timer -= diff;

        //Time Lapse
        if (Time_Lapse_Timer < diff)
        {
            DoYell(SAY_BANISH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_BANISH);
            DoCast(m_creature,TIME_LAPSE);
            Time_Lapse_Timer = 15000+rand()%10000;
        }else Time_Lapse_Timer -= diff;

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_chrono_lord_deja(Creature *_Creature)
{
    return new boss_chrono_lord_dejaAI (_Creature);
}


void AddSC_boss_chrono_lord_deja()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_chrono_lord_deja";
    newscript->GetAI = GetAI_boss_chrono_lord_deja;
    m_scripts[nrscripts++] = newscript;
}
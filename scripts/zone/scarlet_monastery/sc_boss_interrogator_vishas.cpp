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

// **** This script is still under Developement ****

#define SPELL_POWERWORDSHIELD		6065

#define SAY_AGGRO			"Tell me... tell me everything!"
#define SAY_HEALTH1			"Naughty secrets"
#define SAY_HEALTH2			"I'll rip the secrets from your flesh!"
#define SAY_DEATH		    "Purged by pain!"

#define SOUND_AGGRO			5847
#define SOUND_HEALTH1		5849
#define SOUND_HEALTH2		5850
#define SOUND_DEATH			5848

struct MANGOS_DLL_DECL boss_interrogator_vishasAI : public ScriptedAI
{
    boss_interrogator_vishasAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Yell_Timer;
    uint32 PowerWordShield_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Yell_Timer = 6000000;
        PowerWordShield_Timer = 60000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            
            //Say our dialog
            DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO);
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer in range
            if (needToStop())
            {
                EnterEvadeMode();
                return;
            }

            //If we are low on hp Do sayings
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 60 && !m_creature->m_currentSpell)
            {
                //Yell_Timer
                if (Yell_Timer < diff)
                {

                    DoYell(SAY_HEALTH1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_HEALTH1);
                    return;

                    //60 seconds until we should cast this agian
                    Yell_Timer = 60000;
                }else Yell_Timer -= diff;
            }
            
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 30 && !m_creature->m_currentSpell)
            {
                //Yell_Timer
                if (Yell_Timer < diff)
                {

                    DoYell(SAY_HEALTH2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_HEALTH2);
                    return;

                    //60 seconds until we should cast this agian
                    Yell_Timer = 6000000;
                }else Yell_Timer -= diff;
            }
            
            //PowerWordShield_Timer
            if (PowerWordShield_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_POWERWORDSHIELD);

                //60 seconds until we should cast this agian
                PowerWordShield_Timer = 60000;
            }else PowerWordShield_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_boss_interrogator_vishas(Creature *_Creature)
{
    return new boss_interrogator_vishasAI (_Creature);
}


void AddSC_boss_interrogator_vishas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_interrogator_vishas";
    newscript->GetAI = GetAI_boss_interrogator_vishas;
    m_scripts[nrscripts++] = newscript;
}
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

//Status: check buff

#define SPELL_ANTIMAGICSHIELD    19645
#define SPELL_BANSHEECURSE    16867
#define SPELL_DAZED    1604
#define SPELL_DEAFENINGSCREECH    3589
#define SPELL_SOULTAP    16430

struct MANGOS_DLL_DECL mob_wailing_bansheeAI : public ScriptedAI
{
    mob_wailing_bansheeAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 AntiMagicShield_Timer;
    uint32 BansheeCurse_Timer;
    uint32 Dazed_Timer;
    uint32 DeafeningScreech_Timer;
    uint32 SoulTap_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        AntiMagicShield_Timer = 1000;
        BansheeCurse_Timer = 8000;
        Dazed_Timer = 11000;
        DeafeningScreech_Timer = 6000;
        SoulTap_Timer = 13000;
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

            //AntiMagicShield
            if (AntiMagicShield_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_ANTIMAGICSHIELD);
                //13 seconds until we should cast this again
                AntiMagicShield_Timer = 13000;
            }else AntiMagicShield_Timer -= diff;

            //BansheeCurse
            if (BansheeCurse_Timer < diff)
            {
                //Cast
                if (rand()%100 < 85) //85% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_BANSHEECURSE);
                }
                //16 seconds until we should cast this again
                BansheeCurse_Timer = 16000;
            }else BansheeCurse_Timer -= diff;

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 20) //20% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //11 seconds until we should try cast this again
                Dazed_Timer = 11000;
            }else Dazed_Timer -= diff;

            //DeafeningScreech
            if (DeafeningScreech_Timer < diff)
            {
                if (rand()%100 < 55) //55% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEAFENINGSCREECH);
                }
                //12 seconds until we should try cast this again
                DeafeningScreech_Timer = 12000;
            }else DeafeningScreech_Timer -= diff;

            //SoulTap
            if (SoulTap_Timer < diff)
            {
                if (rand()%100 < 35) //35% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_SOULTAP);
                }
                //19 seconds until we should try cast this again
                SoulTap_Timer = 19000;
            }else SoulTap_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
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
CreatureAI* GetAI_mob_wailing_banshee(Creature *_Creature)
{
    return new mob_wailing_bansheeAI (_Creature);
}


void AddSC_mob_wailing_banshee()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_wailing_banshee";
    newscript->GetAI = GetAI_mob_wailing_banshee;
    m_scripts[nrscripts++] = newscript;
}
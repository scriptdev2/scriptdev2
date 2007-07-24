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

//Status: ready for SVN

#define SPELL_HAMMEROFJUSTICE	10308 //rank4
#define SPELL_SEALOFLIGHT	20349 //rank4
#define SPELL_DIVINESHIELD    29382
#define SPELL_RETRIBUTIONAURA    10301 //rank 5

struct MANGOS_DLL_DECL mob_crimson_defenderAI : public ScriptedAI
{
    mob_crimson_defenderAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 HammerOfJustice_Timer;
    uint32 SealOfLight_Timer;
    bool HasAura;
    bool BubbleBoy;
    bool InCombat;

    void EnterEvadeMode()
    {
        HammerOfJustice_Timer = 5000;
        SealOfLight_Timer = 12000;
        HasAura = false;
        BubbleBoy = false;
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!HasAura)
                {
                    m_creature->CastSpell(m_creature,SPELL_RETRIBUTIONAURA,true);
                    HasAura = true;
                }

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

            //HammerOfJustice
            if (HammerOfJustice_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_HAMMEROFJUSTICE);

                //11 seconds until we should cast this again
                HammerOfJustice_Timer = 11000;
            }else HammerOfJustice_Timer -= diff;

            //SealOfLight
            if (SealOfLight_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_SEALOFLIGHT);

                //40 seconds until we should cast this again
                SealOfLight_Timer = 40000;
            }else SealOfLight_Timer -= diff;

            //DivineShield
            if (!BubbleBoy && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 15)
            {
                //Cast
                DoCast(m_creature,SPELL_DIVINESHIELD);
                BubbleBoy = true;
            }

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_crimson_defender(Creature *_Creature)
{
    return new mob_crimson_defenderAI (_Creature);
}


void AddSC_mob_crimson_defender()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_crimson_defender";
    newscript->GetAI = GetAI_mob_crimson_defender;
    m_scripts[nrscripts++] = newscript;
}

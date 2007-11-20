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

/* ScriptData
SDName: boss_crimson_hammersmith
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

// Unique NPC, only summoned after attempt to open item "Blacksmithing Plans" (found...where...?), not a regular mob always around in instance

#define SPELL_BONESMELT    17445
#define SPELL_DAZED    1604
#define SPELL_HAMMEROFJUSTICE    13005

struct MANGOS_DLL_DECL boss_crimson_hammersmithAI : public ScriptedAI
{
    boss_crimson_hammersmithAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 BoneSmelt_Timer;
    uint32 Dazed_Timer;
    uint32 HammerOfJustice_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        BoneSmelt_Timer = 12000;
        Dazed_Timer = 9000;
        HammerOfJustice_Timer = 16000;
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

                //Begin melee attack if we are within range
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

        //BoneSmelt
        if (BoneSmelt_Timer < diff)
        {
            //Cast
            if (rand()%100 < 20) //20% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_BONESMELT);
            }
            //20 seconds until we should cast this again
            BoneSmelt_Timer = 20000;
        }else BoneSmelt_Timer -= diff;

        //Dazed
        if (Dazed_Timer < diff)
        {
            //Cast
            if (rand()%100 < 45) //45% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_DAZED);
            }
            //13 seconds until we should cast this again
            Dazed_Timer = 13000;
        }else Dazed_Timer -= diff;

        //HammerOfJustice
        if (HammerOfJustice_Timer < diff)
        {
            //Cast
            if (rand()%100 < 65) //65% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_HAMMEROFJUSTICE);
            }
            //16 seconds until we should cast this again
            HammerOfJustice_Timer = 16000;
        }else HammerOfJustice_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_crimson_hammersmith(Creature *_Creature)
{
    return new boss_crimson_hammersmithAI (_Creature);
}


void AddSC_boss_crimson_hammersmith()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_crimson_hammersmith";
    newscript->GetAI = GetAI_boss_crimson_hammersmith;
    m_scripts[nrscripts++] = newscript;
}

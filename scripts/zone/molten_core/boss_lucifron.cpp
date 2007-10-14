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



#define SPELL_IMPENDINGDOOM 19702
#define SPELL_LUCIFRONCURSE 19703
#define SPELL_SHADOWSHOCK   17399

struct MANGOS_DLL_DECL boss_lucifronAI : public ScriptedAI
{
    boss_lucifronAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ImpendingDoom_Timer;
    uint32 LucifronCurse_Timer;
    uint32 ShadowShock_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ImpendingDoom_Timer = 10000;        //Initial cast after 10 seconds so the debuffs alternate
        LucifronCurse_Timer = 20000;        //Initial cast after 20 seconds
        ShadowShock_Timer = 6000;           //6 seconds
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BLEED, true);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
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

        //Impending doom timer
        if (ImpendingDoom_Timer < diff)
        {
            //Cast impending doom
            DoCast(m_creature->getVictim(),SPELL_IMPENDINGDOOM);

            //20 seconds until we should cast this agian
            ImpendingDoom_Timer = 20000;
        }else ImpendingDoom_Timer -= diff;

        //Lucifron's curse timer
        if (LucifronCurse_Timer < diff)
        {
            //Cast Lucifron's curse
            DoCast(m_creature->getVictim(),SPELL_LUCIFRONCURSE);

            //15 seconds until we should cast this agian
            LucifronCurse_Timer = 15000;
        }else LucifronCurse_Timer -= diff;

        //Shadowshock
        if (ShadowShock_Timer < diff)
        {
            //Cast shadow shock
            DoCast(m_creature->getVictim(),SPELL_SHADOWSHOCK);

            //6 seconds until we should cast this agian
            ShadowShock_Timer = 6000;
        }else ShadowShock_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_lucifron(Creature *_Creature)
{
    return new boss_lucifronAI (_Creature);
}


void AddSC_boss_lucifron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lucifron";
    newscript->GetAI = GetAI_boss_lucifron;
    m_scripts[nrscripts++] = newscript;
}

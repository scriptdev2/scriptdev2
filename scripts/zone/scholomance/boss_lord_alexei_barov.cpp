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

#define SPELL_UNHOLYAURA                17466    //Workaround cause right spell would be 17467
#define SPELL_IMMOLATE             20294        // Old ID  was 15570       
#define SPELL_VEILOFSHADOW            17820


struct MANGOS_DLL_DECL boss_lordalexeibarovAI : public ScriptedAI
{
    boss_lordalexeibarovAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 UnholyAura_Timer;
    uint32 Immolate_Timer;
    uint32 VeilofShadow_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        UnholyAura_Timer = 2500;
        Immolate_Timer = 9000;
        VeilofShadow_Timer = 16000;
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

        //UnholyAura
        if (UnholyAura_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_UNHOLYAURA);
            //2.5 seconds until we should cast this again
            UnholyAura_Timer = 2500;
        }else UnholyAura_Timer -= diff;


        //Immolate_Timer
        if (Immolate_Timer < diff)
        {


            //Cast Immolate on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_IMMOLATE);

            //24 seconds until we should cast this agian
            Immolate_Timer = 24000;
        }else Immolate_Timer -= diff;

        //VeilofShadow_Timer
        if (VeilofShadow_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_VEILOFSHADOW);

            //25 seconds until we should cast this agian
            VeilofShadow_Timer = 25000;
        }else VeilofShadow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_lordalexeibarov(Creature *_Creature)
{
    return new boss_lordalexeibarovAI (_Creature);
}


void AddSC_boss_lordalexeibarov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_lord_alexei_barov";
    newscript->GetAI = GetAI_boss_lordalexeibarov;
    m_scripts[nrscripts++] = newscript;
}

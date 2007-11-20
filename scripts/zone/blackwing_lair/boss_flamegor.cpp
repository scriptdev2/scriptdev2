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
SDName: Boss_Flamegor
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_SHADOWFLAME        22539
#define SPELL_WINGBUFFET         23339
#define SPELL_FRENZY             23342      //This spell periodically triggers fire nova

struct MANGOS_DLL_DECL boss_flamegorAI : public ScriptedAI
{
    boss_flamegorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowFlame_Timer;
    uint32 WingBuffet_Timer;
    uint32 Frenzy_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ShadowFlame_Timer = 45000;      //These times are probably wrong
        WingBuffet_Timer = 25000;
        Frenzy_Timer = 10000;
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //ShadowFlame_Timer
        if (ShadowFlame_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SHADOWFLAME);

            //45 seconds till recast
            ShadowFlame_Timer = 45000;
        }else ShadowFlame_Timer -= diff;

        //WingBuffet_Timer
        if (WingBuffet_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WINGBUFFET);

            //25 seconds till recast
            WingBuffet_Timer = 25000;
        }else WingBuffet_Timer -= diff;

        //Frenzy_Timer
        if (Frenzy_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_FRENZY);

            //cast this every 10-15 seconds
            Frenzy_Timer = 10000 + (rand()%5000);
        }else Frenzy_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_flamegor(Creature *_Creature)
{
    return new boss_flamegorAI (_Creature);
}


void AddSC_boss_flamegor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_flamegor";
    newscript->GetAI = GetAI_boss_flamegor;
    m_scripts[nrscripts++] = newscript;
}

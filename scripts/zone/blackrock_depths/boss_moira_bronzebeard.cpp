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

#define SPELL_HEAL       10917
#define SPELL_RENEW       10929
#define SPELL_SHIELD       10901
#define SPELL_MINDBLAST       10947
#define SPELL_SHADOWWORDPAIN       10894
#define SPELL_SMITE       10934





struct MANGOS_DLL_DECL boss_moira_bronzebeardAI : public ScriptedAI
{
    boss_moira_bronzebeardAI(Creature *c) : ScriptedAI(c) {InitialSpawn();}

    uint32 Heal_Timer;
    uint32 MindBlast_Timer;
    uint32 ShadowWordPain_Timer;
    uint32 Smite_Timer;
    bool InCombat;
    Unit* PlayerHolder;
    Unit* Target;
    bool Heal;

    void InitialSpawn()
    {
        Target = NULL;
        EnterEvadeMode();
    }

    void EnterEvadeMode()
    {
        Heal_Timer = 12000;      //These times are probably wrong
        MindBlast_Timer = 16000;
        ShadowWordPain_Timer = 2000;
        Smite_Timer = 8000;
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
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //MindBlast_Timer
            if (MindBlast_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_MINDBLAST);

                //14 seconds
                MindBlast_Timer = 14000;
            }else MindBlast_Timer -= diff;

            //ShadowWordPain_Timer
            if (ShadowWordPain_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHADOWWORDPAIN);

                //18 seconds
                ShadowWordPain_Timer = 18000;
            }else ShadowWordPain_Timer -= diff;

            //Smite_Timer
            if (Smite_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SMITE);

                //10 seconds
                Smite_Timer = 10000;
            }else Smite_Timer -= diff;

        }
    }
};
CreatureAI* GetAI_boss_moira_bronzebeard(Creature *_Creature)
{
    return new boss_moira_bronzebeardAI (_Creature);
}


void AddSC_boss_moira_bronzebeard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_moira_bronzebeard";
    newscript->GetAI = GetAI_boss_moira_bronzebeard;
    m_scripts[nrscripts++] = newscript;
}


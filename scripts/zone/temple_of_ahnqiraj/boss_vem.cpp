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
SDName: boss_vem
SD%Complete: 100
SDComment: Loot not linked
EndScriptData */


#include "../../sc_defines.h"
#include "def_temple_of_ahnqiraj.h"


#define SPELL_CHARGE        25821
#define SPELL_ENRAGE        19953
#define SPELL_KNOCKBACK     26027

struct MANGOS_DLL_DECL boss_vemAI : public ScriptedAI
{
    boss_vemAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Charge_Timer;
    uint32 KnockBack_Timer;
    uint32 Enrage_Timer;

    bool Enraged;

    void Reset()
    {
        Charge_Timer = 15000 + rand()%12000;
        KnockBack_Timer = 8000 + rand()%12000;
        Enrage_Timer = 120000;

        Enraged = false;

//        m_creature->RemoveAllAuras();
//        m_creature->DeleteThreatList();
//        m_creature->CombatStop();
//        DoGoHome();
    }


    void JustDied(Unit* Killer)
    {
        ScriptedInstance *pInstance = (m_creature->GetInstanceData()) ? ((ScriptedInstance*)m_creature->GetInstanceData()) : NULL;
        if(pInstance)
            pInstance->SetData(DATA_VEM_DEATH, 0);
    }
    

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

            //Charge_Timer
            if (Charge_Timer < diff)
            {
                
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    
                DoCast(target, SPELL_CHARGE);
                m_creature->SendMonsterMove(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, true,1);
                DoStartMeleeAttack(target);
                   
                Charge_Timer = 8000 + rand()%8000;
            }else Charge_Timer -= diff;

            //KnockBack_Timer
            if (KnockBack_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_KNOCKBACK);
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);
                KnockBack_Timer = 15000 + rand()%10000;
            }else KnockBack_Timer -= diff;

            //Enrage_Timer
            if (!Enraged && Enrage_Timer < diff)
            {
                DoCast(m_creature,SPELL_ENRAGE);
                Enraged = true;
            }else Charge_Timer -= diff;


            DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_vem(Creature *_Creature)
{
    return new boss_vemAI (_Creature);
}


void AddSC_boss_vem()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_vem";
    newscript->GetAI = GetAI_boss_vem;
    m_scripts[nrscripts++] = newscript;
}

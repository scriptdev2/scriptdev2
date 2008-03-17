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
SDName: boss_yauj
SD%Complete: 100
SDComment: Loot not linked
EndScriptData */

#include "../../sc_defines.h"
#include "def_temple_of_ahnqiraj.h"

#define SPELL_HEAL      25807
#define SPELL_FEAR      19408
#define SPELL_ENRAGE    28131


struct MANGOS_DLL_DECL boss_yaujAI : public ScriptedAI
{
    boss_yaujAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Heal_Timer;
    uint32 Fear_Timer;
    uint32 Check_Timer;

    Creature* Summoned;

    bool VemDead;

    void Reset()
    {
        Heal_Timer = 25000 + rand()%15000;
        Fear_Timer = 12000 + rand()%12000;
        Check_Timer = 2000;

        VemDead = false;

//        m_creature->RemoveAllAuras();
//        m_creature->DeleteThreatList();
//        m_creature->CombatStop();
//        DoGoHome();
    }

  void ResetThreat()
  {
    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
 
    for(uint32 i = 0; i <= (m_threatlist.size()-1); i++)
    {
      Unit* pUnit = SelectUnit(SELECT_TARGET_TOPAGGRO, i);
      if(pUnit)
        (m_creature->getThreatManager()).modifyThreatPercent(pUnit, -99);
    }
 
  }

    void JustDied(Unit* Killer)
    {
                Unit* target = NULL;
                for(int i = 0; i < 10;i++)
                {
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    Summoned = m_creature->SummonCreature(15621,m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,900000);
                    ((CreatureAI*)Summoned->AI())->AttackStart(target);
               }
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

            //Fear_Timer
            if (Fear_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_FEAR);
                ResetThreat();
                Fear_Timer = 20000;
            }else Fear_Timer -= diff;

        //Casting Heal to other twins or herself.
        if(Heal_Timer < diff)
        {
                    if(pInstance)
                    {    

                     Unit *pKri = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KRI));
                     Unit *pVem = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_VEM));



                         switch(rand()%3)
                         {
                            case 0:  
                            DoCast(pKri, SPELL_HEAL);
                            break;

                            case 1:
                            DoCast(pVem, SPELL_HEAL);
                            break;

                            case 2:
                            DoCast(m_creature, SPELL_HEAL);
                            break;
                        }


                    }
                       
            Heal_Timer = 15000+rand()%15000;
        }else Heal_Timer -= diff;

        //Checking if Vem is dead. If yes we will enrage.
        if(Check_Timer < diff)
        {
            if (!VemDead)
            {
                if(pInstance)
                {    
                    if(pInstance->GetData(DATA_VEMISDEAD))
                    DoCast(m_creature, SPELL_ENRAGE);
                    VemDead = true;
                }
            }
            Check_Timer = 2000;
        }else Check_Timer -= diff;



            DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_yauj(Creature *_Creature)
{
    return new boss_yaujAI (_Creature);
}


void AddSC_boss_yauj()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_yauj";
    newscript->GetAI = GetAI_boss_yauj;
    m_scripts[nrscripts++] = newscript;
}

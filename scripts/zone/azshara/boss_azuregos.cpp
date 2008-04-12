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
SDName: Boss_Azuregos
SD%Complete: 90
SDComment: Teleport not included, spell reflect not effecting dots (Core problem)
SDCategory: Azshara
EndScriptData */

#include "sc_creature.h"

#define SPELL_MARKOFFROST        23182            
#define SPELL_MANASTORM          21097
#define SPELL_REFLECT            30969               //Works fine but not for Dot Spells
#define SPELL_CLEAVE              8255                //Perhaps not right ID
#define SPELL_ENRAGE             28747

struct MANGOS_DLL_DECL boss_azuregosAI : public ScriptedAI
{
    boss_azuregosAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 MarkOfFrost_Timer;
    uint32 ManaStorm_Timer;
    uint32 Teleport_Timer;
    uint32 Reflect_Timer;
    uint32 Cleave_Timer;
    uint32 Enrage_Timer;

    void Reset()
    {       
        MarkOfFrost_Timer = 35000;
        ManaStorm_Timer = 10000;
        Teleport_Timer = 30000;
        Reflect_Timer = 25000;
        Cleave_Timer = 7000;
        Enrage_Timer = 0;
    }

    void Aggro(Unit *who)
    {

    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;


          if(Teleport_Timer < diff)
          {
             std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
             std::list<HostilReference*>::iterator i = m_threatlist.begin();
             for (i = m_threatlist.begin(); i!= m_threatlist.end();++i)
             {
                 Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                 if(pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                 {
                 DoTeleportPlayer(pUnit, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+5, pUnit->GetOrientation());
                 }
             }

             DoResetThreat();
             Teleport_Timer = 30000;
        }else Teleport_Timer -= diff;

        //MarkOfFrost_Timer
        if (MarkOfFrost_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MARKOFFROST);

            //30 seconds
            MarkOfFrost_Timer = 25000;
        }else MarkOfFrost_Timer -= diff;

        //ManaStorm_Timer
        if (ManaStorm_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MANASTORM);

            //15 seconds until we should cast this agian
            ManaStorm_Timer = 10000;
        }else ManaStorm_Timer -= diff;


        //Reflect_Timer
        if (Reflect_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_REFLECT);

            //45 seconds until we should cast this agian
            Reflect_Timer = 45000;
        }else Reflect_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //12 seconds until we should cast this agian
            Cleave_Timer = 7000;
        }else Cleave_Timer -= diff;


        //Enrage_Timer
        if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 26)
        {
            //Cast
            DoCast(m_creature,SPELL_ENRAGE);                    

            //22 seconds until we should cast this agian
            Enrage_Timer = 30000;  
        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_azuregos(Creature *_Creature)
{
    return new boss_azuregosAI (_Creature);
}


void AddSC_boss_azuregos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_azuregos";
    newscript->GetAI = GetAI_boss_azuregos;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: boss_heartsinger_forresten
SD%Complete: 100
SDComment: 
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****


#define SPELL_SHOOT    20463
#define SPELL_MULTISHOT    21390

struct MANGOS_DLL_DECL boss_hearthsinger_forrestenAI : public ScriptedAI
{
    boss_hearthsinger_forrestenAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Shoot_Timer;
    uint32 MultiShot_Timer;
    bool InCombat;

    void Reset()
    {
        Shoot_Timer = 1000;
        MultiShot_Timer = 5000;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //MultiShot
        if (MultiShot_Timer < diff)
        {
            //Cast
            if (rand()%100 < 90) //90% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_MULTISHOT);
            }
            //16 seconds until we should cast this again
            MultiShot_Timer = 16000;
        }else MultiShot_Timer -= diff;

        //Shoot
        if (Shoot_Timer < diff)
        {
            //Cast
            if (rand()%100 < 90) //90% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_SHOOT);
            }
            //3 seconds until we should cast this again
            Shoot_Timer = 3000;
        }else Shoot_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_hearthsinger_forresten(Creature *_Creature)
{
    return new boss_hearthsinger_forrestenAI (_Creature);
}


void AddSC_boss_hearthsinger_forresten()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_hearthsinger_forresten";
    newscript->GetAI = GetAI_boss_hearthsinger_forresten;
    m_scripts[nrscripts++] = newscript;
}

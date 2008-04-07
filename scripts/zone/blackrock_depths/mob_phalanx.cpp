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
SDName: Mob_Phalanx
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Depths
EndScriptData */

#include "sc_creature.h"

// **** This script is still under Developement ****


#define SPELL_THUNDERCLAP                8732              
#define SPELL_FIREBALLVOLLEY                22425
#define SPELL_MIGHTYBLOW                14099


struct MANGOS_DLL_DECL phalanxAI : public ScriptedAI
{
    phalanxAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ThunderClap_Timer;
    uint32 FireballVolley_Timer;
    uint32 MightyBlow_Timer;
    bool InCombat;

    void Reset()
    {       
        ThunderClap_Timer = 12000;
        FireballVolley_Timer =0;
        MightyBlow_Timer = 15000;
        InCombat = false;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //ThunderClap_Timer
        if (ThunderClap_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);

            //10 seconds
            ThunderClap_Timer = 10000;
        }else ThunderClap_Timer -= diff;

        //FireballVolley_Timer
        if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 51 )
        {
            if (FireballVolley_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_FIREBALLVOLLEY);

                //15 seconds
                FireballVolley_Timer = 15000;
            }else FireballVolley_Timer -= diff;
        }

        //MightyBlow_Timer
        if (MightyBlow_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MIGHTYBLOW);

            //10 seconds
            MightyBlow_Timer = 10000;
        }else MightyBlow_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_phalanx(Creature *_Creature)
{
    return new phalanxAI (_Creature);
}


void AddSC_phalanx()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="phalanx";
    newscript->GetAI = GetAI_phalanx;
    m_scripts[nrscripts++] = newscript;
}

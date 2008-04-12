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
SDName: Boss_Mother_Smolderweb
SD%Complete: 100
SDComment: 
SDCategory: Blackrock Spire
EndScriptData */

#include "sc_creature.h"


#define SPELL_CRYSTALIZE                16104   
#define SPELL_MOTHERSMILK             16468     
#define SPELL_POSION            24097
#define SPELL_WEBEXPLOSION                 16469    //Make all nearby mobs aggro  


struct MANGOS_DLL_DECL boss_mothersmolderwebAI : public ScriptedAI
{
    boss_mothersmolderwebAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Crystalize_Timer;
    uint32 MothersMilk_Timer;
    uint32 Posion_Timer;
    uint32 WebExplosion_Timer;

    void Reset()
    {       
        Crystalize_Timer = 20000;
        MothersMilk_Timer = 30000;
        Posion_Timer = 4000;
        WebExplosion_Timer = 60000;
        //m_creature->CastSpell(m_creature,SPELL_ICEARMOR,true);
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Crystalize_Timer
        if (Crystalize_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CRYSTALIZE);

            //20 seconds
            Crystalize_Timer = 14000;
        }else Crystalize_Timer -= diff;

        //MothersMilk_Timer
        if (MothersMilk_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MOTHERSMILK);

            //60 seconds until we should cast this agian
            MothersMilk_Timer = 60000;
        }else MothersMilk_Timer -= diff;


        //Posion_Timer
        if (Posion_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_POSION);

            //7 seconds until we should cast this agian
            Posion_Timer = 7000;
        }else Posion_Timer -= diff;

        //WebExplosion_Timer
        if (WebExplosion_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WEBEXPLOSION);

            //20 seconds until we should cast this agian
            WebExplosion_Timer = 20000;
        }else WebExplosion_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_mothersmolderweb(Creature *_Creature)
{
    return new boss_mothersmolderwebAI (_Creature);
}


void AddSC_boss_mothersmolderweb()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_mother_smolderweb";
    newscript->GetAI = GetAI_boss_mothersmolderweb;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: Boss_Razorgore
SD%Complete: 50
SDComment: Phase 1 NYI
SDCategory: Blackwing Lair
EndScriptData */

//*** NEEDS ADDITIOINAL REVIEW ***

//Razorgore Phase 2 Script
//Phase 1 script with adds NYI

#include "sc_creature.h"

#define SAY_NPC_DEATH "If I fall into the abyss I'll take all of you mortals with me..."
#define SOUND_NPC_DEATH 8278
#define SAY_EGGS_BREAK3 "No! Not another one! I'll have your heads for this atrocity. "
#define SOUND_EGGS_BREAK3 8277

#define SPELL_CLEAVE                22540
#define SPELL_WARSTOMP              24375      
#define SPELL_FIREBALLVOLLEY        22425       //This fireball does not enouph dmg so we will castcustom
#define SPELL_CONFLAGRATION         23023

struct MANGOS_DLL_DECL boss_razorgoreAI : public ScriptedAI
{
    boss_razorgoreAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Cleave_Timer;
    uint32 WarStomp_Timer;
    uint32 FireballVolley_Timer;
    uint32 Conflagration_Timer;

    void Reset()
    {
        Cleave_Timer = 15000;      //These times are probably wrong
        WarStomp_Timer = 35000;
        FireballVolley_Timer = 7000;
        Conflagration_Timer = 12000;
    }

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            //7-14 seconds until we should cast this agian
            Cleave_Timer = 7000 + rand()%3000;
        }else Cleave_Timer -= diff;

        //WarStomp_Timer
        if (WarStomp_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WARSTOMP);

            //20-30 seconds until we should cast this agian
            WarStomp_Timer = 15000 + rand()%10000;
        }else WarStomp_Timer -= diff;

        //FireballVolley_Timer
        if (FireballVolley_Timer < diff)
        {
       
            int damage = 1400 + rand()%1800;
            //Cast
            //DoCast(m_creature->getVictim(),SPELL_FIREBALLVOLLEY);
            m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_FIREBALLVOLLEY, &damage, NULL, NULL, false, NULL, NULL, m_creature->GetGUID());  

            //10-15 seconds until we should cast this agian
            FireballVolley_Timer = 12000 + rand()%3000;
        }else FireballVolley_Timer -= diff;

        //Conflagration_Timer
        if (Conflagration_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_CONFLAGRATION);
            if(m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);

            //12 seconds until we should cast this agian
            Conflagration_Timer = 12000;
        }else Conflagration_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_razorgore(Creature *_Creature)
{
    return new boss_razorgoreAI (_Creature);
}


void AddSC_boss_razorgore()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_razorgore";
    newscript->GetAI = GetAI_boss_razorgore;
    m_scripts[nrscripts++] = newscript;
}

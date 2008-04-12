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
SDName: Mob_Scarlet_Torturer
SD%Complete: 100
SDComment: 
SDCategory: Scarlet Monastery
EndScriptData */

#include "sc_creature.h"


#define SPELL_IMMOLATE           37668

#define SAY_AGGRO1               "What? Oh no. I don't care what you have to say. I just enjoy inflicting pain."
#define SAY_AGGRO2               "You will talk eventually. You might as well spill it now."
#define SAY_AGGRO3               "Confess and we shall set you free."

struct MANGOS_DLL_DECL scarlet_torturerAI : public ScriptedAI
{
    scarlet_torturerAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Immolate_Timer;

    void Reset()
    {
        Immolate_Timer = 45000;
    }

    void Aggro(Unit *who)
    {
                //Switch between 3 different aggro saying
                switch (rand()%3)
                {
                case 0:
                    DoYell(SAY_AGGRO1,LANG_UNIVERSAL,NULL);
                    break;
                case 1:
                    DoYell(SAY_AGGRO2,LANG_UNIVERSAL,NULL);
                    break;
                case 2:
                    DoYell(SAY_AGGRO3,LANG_UNIVERSAL,NULL);
                    break;
                }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Immolate_Timer
        if (Immolate_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_IMMOLATE);

            //45 seconds until we should cast this agian
            Immolate_Timer = 45000;
        }else Immolate_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_scarlet_torturer(Creature *_Creature)
{
    return new scarlet_torturerAI (_Creature);
}


void AddSC_scarlet_torturer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="scarlet_torturer";
    newscript->GetAI = GetAI_scarlet_torturer;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: Boss_Tinkerer_Gizlock
SD%Complete: 100
SDComment: 
EndScriptData */


#include "sc_creature.h"

#define SPELL_GOBLINGUN             21910   // NOt sure about right id!     
#define SPELL_BOMB                  22334              

#define SAY_AGGRO1      "Mine! Mine! Mine! Gizlock is the ruler of this domain! You shall never reveal my presence!"    

struct MANGOS_DLL_DECL boss_tinkerergizlockAI : public ScriptedAI
{
    boss_tinkerergizlockAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 GoblinGun_Timer;
    uint32 Bomb_Timer;
    bool InCombat;

    void Reset()
    {       
        GoblinGun_Timer = 6000;
        Bomb_Timer = 12000;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void Aggro(Unit *who)
    {
                    DoYell(SAY_AGGRO1,LANG_UNIVERSAL,NULL);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //GoblinGun_Timer
        if (GoblinGun_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_GOBLINGUN);

            //15 seconds
            GoblinGun_Timer = 16000;
        }else GoblinGun_Timer -= diff;

        //Bomb_Timer
        if (Bomb_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_BOMB);

            //17 seconds until we should cast this agian
            Bomb_Timer = 14000;
        }else Bomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_tinkerergizlock(Creature *_Creature)
{
    return new boss_tinkerergizlockAI (_Creature);
}


void AddSC_boss_tinkerergizlock()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_tinkerer_gizlock";
    newscript->GetAI = GetAI_boss_tinkerergizlock;
    m_scripts[nrscripts++] = newscript;
}

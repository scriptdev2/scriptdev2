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
SDName: Boss_Ebonroc
SD%Complete: 50
SDComment: Shadow of Ebonroc needs core support
SDCategory: Blackwing Lair
EndScriptData */

#include "sc_creature.h"

#define SPELL_SHADOWFLAME           22539
#define SPELL_WINGBUFFET            18500
#define SPELL_SHADOWOFEBONROC       23340
#define SPELL_HEAL                  23394    //Thea Heal spell of his Shadow

struct MANGOS_DLL_DECL boss_ebonrocAI : public ScriptedAI
{
    boss_ebonrocAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ShadowFlame_Timer;
    uint32 WingBuffet_Timer;
    uint32 ShadowOfEbonroc_Timer;
    uint32 Heal_Timer;
    bool InCombat;

    void Reset()
    {
        ShadowFlame_Timer = 15000;      //These times are probably wrong
        WingBuffet_Timer = 30000;
        ShadowOfEbonroc_Timer = 45000;
        Heal_Timer = 2000;
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
        //Shadowflame Timer
        if (ShadowFlame_Timer < diff)
        {
            //Cast Shadowflame
            DoCast(m_creature->getVictim(),SPELL_SHADOWFLAME);

            //15-25 seconds till recast
            ShadowFlame_Timer = 12000 + rand()%3000;
        }else ShadowFlame_Timer -= diff;

        //Wing Buffet Timer
        if (WingBuffet_Timer < diff)
        {
            //Cast Wing Buffet
            DoCast(m_creature->getVictim(),SPELL_WINGBUFFET);

            //30 seconds until we should cast this agian
            WingBuffet_Timer = 30000;
        }else WingBuffet_Timer -= diff;

        //Shadow of Ebonroc Timer
        if (ShadowOfEbonroc_Timer < diff)
        {
            //Cast Shadow of Ebonroc
            DoCast(m_creature->getVictim(),SPELL_SHADOWOFEBONROC);

            //30-40 seconds until we should cast this agian
            ShadowOfEbonroc_Timer = 30000 + rand()%10000;
        }else ShadowOfEbonroc_Timer -= diff;


        if (m_creature->getVictim()->HasAura(SPELL_SHADOWOFEBONROC,0) && Heal_Timer < diff)
        {
            //Cast Shadow of Ebonroc
            DoCast(m_creature, SPELL_HEAL);

            //2-4 seconds until we should cast this agian
            Heal_Timer = 20000 + rand()%2000;
        }else Heal_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_ebonroc(Creature *_Creature)
{
    return new boss_ebonrocAI (_Creature);
}


void AddSC_boss_ebonroc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ebonroc";
    newscript->GetAI = GetAI_boss_ebonroc;
    m_scripts[nrscripts++] = newscript;
}

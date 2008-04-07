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
SDName: npc_Guardian
SD%Complete: 100
SDComment: GuardianAI used to prevent players from accessing off-limits areas
SDCategory: NPCs
EndScriptData */

#include "sc_creature.h"

#define SPELL_DEATHTOUCH                5
#define SAY_AGGRO                        "This area is closed!"

struct MANGOS_DLL_DECL npc_guardianAI : public ScriptedAI
{
    npc_guardianAI(Creature *c) : ScriptedAI(c) {Reset();}
    bool InCombat;

    void Reset()
    {
        InCombat = false;
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }


    void Aggro(Unit *who)
    {

                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);

    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_creature->isAttackReady())
        {
            m_creature->CastSpell(m_creature->getVictim(),SPELL_DEATHTOUCH, true);
            m_creature->resetAttackTimer();
        }
    }
};

CreatureAI* GetAI_npc_guardian(Creature *_Creature)
{
    return new npc_guardianAI (_Creature);
}

void AddSC_npc_guardian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_guardian";
    newscript->GetAI = GetAI_npc_guardian;
    m_scripts[nrscripts++] = newscript;
}

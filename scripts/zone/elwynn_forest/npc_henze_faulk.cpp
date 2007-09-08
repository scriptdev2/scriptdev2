/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"

#define SAY_HEAL "<Missing English Text>"

struct MANGOS_DLL_DECL henze_faulkAI : public ScriptedAI
{
    uint32 ResetlifeTimer;
    uint32 DoSayHealTimer;
    bool DoSayHeal;

    henze_faulkAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        ResetlifeTimer= 60000;
        DoSayHealTimer = 5000;
        DoSayHeal = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        DoGoHome();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->SetHealth(1);
        m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, 32);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,7); // ley down
    }

    void AttackStart(Unit *who)
    {
        return; //ignore all attackstart commands
    }

    void MoveInLineOfSight(Unit *who)
    {
        return; //ignore everyone around them (won't aggro anything)
    }

    void UpdateAI(const uint32 diff)
    {

        if (m_creature->GetHealth() > 1)
        {
            if(ResetlifeTimer < diff)
            {
                m_creature->SetHealth(1);
                ResetlifeTimer = 60000;
                m_creature->RemoveAllAuras();
            }
            else ResetlifeTimer -= diff;
        }

        if(DoSayHeal)
        {
            if(DoSayHealTimer < diff)
            {
                DoSay(SAY_HEAL,LANG_COMMON,NULL);
                DoSayHealTimer = 5000;
                DoSayHeal = false;
            }else DoSayHealTimer -= diff;
        }

        return;
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if(Spellkind->Id == 8593)
        {
            DoCast(m_creature,32343);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
            m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, 0);
            m_creature->RemoveAllAuras();
            DoSayHeal = true;
        }
        return;
    }

};
CreatureAI* GetAI_henze_faulk(Creature *_Creature)
{
    return new henze_faulkAI (_Creature);
}

bool QuestAccept_henze_faulk(Player *player, Creature *creature, Quest *quest )
{
    if(quest->GetQuestId() == 1787)
    {
        creature->RemoveAllAuras();
        creature->DeleteThreatList();
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        creature->SetHealth(1);
        creature->SetFlag(UNIT_DYNAMIC_FLAGS, 32);
        creature->SetUInt32Value(UNIT_FIELD_BYTES_1,7); // ley down
        return true;
    }
    return false;
}


void AddSC_henze_faulk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="henze_faulk";
    newscript->GetAI = GetAI_henze_faulk;
    newscript->pQuestAccept = &QuestAccept_henze_faulk;
    m_scripts[nrscripts++] = newscript;
}

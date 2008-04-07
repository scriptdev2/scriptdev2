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
SDName: NPC_Narm_Faulk
SD%Complete: 50
SDComment: Core keeps breaking support for this quest
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

// **** This script is still under Developement ****

#define SAY_HEAL "Thank you, dear Paladin, you just saved my life."

struct MANGOS_DLL_DECL npc_narm_faulkAI : public ScriptedAI
{
    uint32 lifeTimer;
    bool spellHit;

    npc_narm_faulkAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
        lifeTimer = 120000;
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //DoGoHome();
        m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 32);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,7); // lay down
        spellHit = false;
    }

    void AttackStart(Unit *who)
    {
        return; //ignore all attackstart commands
    }

    void MoveInLineOfSight(Unit *who)
    {
        return;
        /*if(who && who->GetTypeId() == TYPEID_PLAYER)
            if( ((Player*)who)->GetQuestStatus(1783) == QUEST_STATUS_INCOMPLETE && !((Player*)who)->GetReqKillOrCastCurrentCount(1783, m_creature->GetEntry()) )
            {
                UpdateData update_data;
                WorldPacket packet;
                
                m_creature->BuildCreateUpdateBlockForPlayer(&update_data, ((Player*)who));
                
                ByteBuffer buf(500);
                buf << UPDATETYPE_CREATE_OBJECT;
                
                buf << (uint8)0xFF << m_creature->GetGUID();
                buf << m_creature->m_objectTypeId;

                m_creature->_BuildMovementUpdate(&buf, m_creature->m_updateFlag, 0);

                UpdateMask updateMask;
                updateMask.SetCount( m_creature->m_valuesCount );
                for( uint16 index = 0; index < m_creature->m_valuesCount; index++ )
                {
                    if(index == 152)
                    if(GetUInt32Value(index) != 0)
                        updateMask->SetBit(index);
                }
                m_creature->_SetCreateBits( &updateMask, target );
                _BuildValuesUpdate( &buf, &updateMask, target );
                data->AddUpdateBlock(buf);

                update_data.BuildPacket(&packet);
                ((Player*)who)->GetSession()->SendPacket(&packet);
            }*/
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->GetUInt32Value(UNIT_FIELD_BYTES_1))
        {
            if(lifeTimer < diff)
                m_creature->AI()->EnterEvadeMode();
            else 
                lifeTimer -= diff;
        }
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if(Spellkind->Id == 8593 && !spellHit)
        {
            DoCast(m_creature,32343);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
            m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
            //m_creature->RemoveAllAuras();
            DoSay(SAY_HEAL,LANG_COMMON,NULL);
            spellHit = true;
        }
    }

};
CreatureAI* GetAI_npc_narm_faulk(Creature *_Creature)
{
    return new npc_narm_faulkAI (_Creature);
}
void AddSC_npc_narm_faulk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_narm_faulk";
    newscript->GetAI = GetAI_npc_narm_faulk;
    m_scripts[nrscripts++] = newscript;
}

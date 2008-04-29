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
SDName: Mobs_Tanaris
SD%Complete: 80
SDComment: Quest support: 4005
SDCategory: Tanaris
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## mob_aquementas
######*/

#define AQUE_AGGRO_YELL "Who dares awaken Aquementas?"

struct MANGOS_DLL_DECL mob_aquementasAI : public ScriptedAI
{
    mob_aquementasAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SendItemTimer;
    uint32 SwitchFactionTimer;
    bool isFriendly;

    void Reset()
    {
        SendItemTimer = 25000;
        SwitchFactionTimer = 10000;
        m_creature->setFaction(35);
        isFriendly = true;
    }

    void SendItem(Unit* receiver)
    {
        if (((Player*)receiver)->HasItemCount(11169,1,false) && 
            ((Player*)receiver)->HasItemCount(11172,11,false) && 
            ((Player*)receiver)->HasItemCount(11173,1,false) &&
            !((Player*)receiver)->HasItemCount(11522,1,true))
        {
            ItemPosCountVec dest;
            uint8 msg = ((Player*)receiver)->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, 11522, 1, false);
            if( msg == EQUIP_ERR_OK )
                ((Player*)receiver)->StoreNewItem( dest, 11522, 1, true);
        }
    }

    void Aggro(Unit* who)
    {
        DoYell(AQUE_AGGRO_YELL,LANG_UNIVERSAL,who);
    }

    void UpdateAI(const uint32 diff)
    {
        if(isFriendly)
        {
            if (SwitchFactionTimer < diff)
            {
                m_creature->setFaction(91);
                isFriendly = false;
            }else SwitchFactionTimer -= diff;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!isFriendly)
        {
            if (SendItemTimer < diff)
            {
                if(m_creature->getVictim()->GetTypeId() == TYPEID_PLAYER)
                    SendItem(m_creature->getVictim());

                SendItemTimer = 5000;
            }else SendItemTimer -= diff;
        }

        //TODO: add spells
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_aquementas(Creature *_Creature)
{
    return new mob_aquementasAI (_Creature);
}

void AddSC_mobs_tanaris()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_aquementas";
    newscript->GetAI = GetAI_mob_aquementas;
    m_scripts[nrscripts++] = newscript;
}

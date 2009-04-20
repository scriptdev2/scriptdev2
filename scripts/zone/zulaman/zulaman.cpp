/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Zulaman
SD%Complete: 90
SDComment: Forest Frog will turn into different NPC's. Workaround to prevent new entry from running this script
SDCategory: Zul'Aman
EndScriptData */

/* ContentData
npc_forest_frog
EndContentData */

#include "precompiled.h"
#include "def_zulaman.h"
#include "../../npc/npc_escortAI.h"

/*######
## npc_forest_frog
######*/

#define SPELL_REMOVE_AMANI_CURSE    43732
#define SPELL_PUSH_MOJO             43923
#define ENTRY_FOREST_FROG           24396

struct MANGOS_DLL_DECL npc_forest_frogAI : public ScriptedAI
{
    npc_forest_frogAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset() { }

    void Aggro(Unit *who) { }

    void DoSpawnRandom()
    {
        if( pInstance )
        {
            uint32 cEntry = 0;
            switch(rand()%11)
            {
                case 0: cEntry = 24024; break;              //Kraz
                case 1: cEntry = 24397; break;              //Mannuth
                case 2: cEntry = 24403; break;              //Deez
                case 3: cEntry = 24404; break;              //Galathryn
                case 4: cEntry = 24405; break;              //Adarrah
                case 5: cEntry = 24406; break;              //Fudgerick
                case 6: cEntry = 24407; break;              //Darwen
                case 7: cEntry = 24445; break;              //Mitzi
                case 8: cEntry = 24448; break;              //Christian
                case 9: cEntry = 24453; break;              //Brennan
                case 10: cEntry = 24455; break;             //Hollee
            }

            if( !pInstance->GetData(TYPE_RAND_VENDOR_1) )
                if(rand()%10 == 1) cEntry = 24408;          //Gunter
            if( !pInstance->GetData(TYPE_RAND_VENDOR_2) )
                if(rand()%10 == 1) cEntry = 24409;          //Kyren

            if( cEntry ) m_creature->UpdateEntry(cEntry);

            if( cEntry == 24408) pInstance->SetData(TYPE_RAND_VENDOR_1,DONE);
            if( cEntry == 24409) pInstance->SetData(TYPE_RAND_VENDOR_2,DONE);
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if( spell->Id == SPELL_REMOVE_AMANI_CURSE && caster->GetTypeId() == TYPEID_PLAYER && m_creature->GetEntry() == ENTRY_FOREST_FROG )
        {
            //increase or decrease chance of mojo?
            if( rand()%99 == 50 ) DoCast(caster,SPELL_PUSH_MOJO,true);
            else DoSpawnRandom();
        }
    }
};
CreatureAI* GetAI_npc_forest_frog(Creature *_Creature)
{
    return new npc_forest_frogAI (_Creature);
}

/*######
## npc_harrison_jones_za
######*/

enum
{
    SAY_START               = -1568079,
    SAY_AT_GONG             = -1568080,
    SAY_OPEN_ENTRANCE       = -1568081
};

#define GOSSIP_ITEM_BEGIN   "[PH] Begin"

struct MANGOS_DLL_DECL npc_harrison_jones_zaAI : public npc_escortAI
{
    npc_harrison_jones_zaAI(Creature *c) : npc_escortAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    void WaypointReached(uint32 i)
    {
        if (!pInstance)
            return;

        switch(i)
        {
            case 1:
                DoScriptText(SAY_AT_GONG, m_creature);
                if (GameObject* pEntranceDoor = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_GO_GONG)))
                    pEntranceDoor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                IsOnHold = true;
                break;
            case 3:
                DoScriptText(SAY_OPEN_ENTRANCE, m_creature);

                if (GameObject* pEntranceDoor = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_GO_ENTRANCE)))
                    pEntranceDoor->SetGoState(0);

                pInstance->SetData(TYPE_EVENT_RUN,IN_PROGRESS);
                break;
        }
    }

    void Reset()
    {
    }

    void StartEvent()
    {
        DoScriptText(SAY_START, m_creature);
        Start(false,true,false,0);
    }

    void SetHoldState(bool bOnHold)
    {
        IsOnHold = bOnHold;
    }

    void Aggro(Unit* who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

bool GossipHello_npc_harrison_jones_za(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pInstance && pInstance->GetData(TYPE_EVENT_RUN) == NOT_STARTED)
        pPlayer->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_BEGIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_harrison_jones_za(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        ((npc_harrison_jones_zaAI*)pCreature->AI())->StartEvent();
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    return true;
}

CreatureAI* GetAI_npc_harrison_jones_za(Creature* pCreature)
{
    npc_harrison_jones_zaAI* tempAI = new npc_harrison_jones_zaAI(pCreature);

    //TODO: create proper waypoints
    tempAI->FillPointMovementListForCreature();

    return (CreatureAI*)tempAI;
}

/*######
## go_strange_gong
######*/

//Unsure how this Gong must work. Here we always return false to allow Mangos always process further.
bool GOHello_go_strange_gong(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if (pInstance->GetData(TYPE_EVENT_RUN) == SPECIAL)
    {
        if (Creature* pCreature = (Creature*)Unit::GetUnit(*pPlayer,pInstance->GetData64(DATA_HARRISON)))
            ((npc_harrison_jones_zaAI*)pCreature->AI())->SetHoldState(false);
        else
            error_log("SD2: Instance Zulaman: go_strange_gong failed");

        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
        return false;
    }

    pInstance->SetData(TYPE_EVENT_RUN, SPECIAL);
    return false;
}

void AddSC_zulaman()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_forest_frog";
    newscript->GetAI = &GetAI_npc_forest_frog;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_harrison_jones_za";
    newscript->GetAI = &GetAI_npc_harrison_jones_za;
    newscript->pGossipHello =  &GossipHello_npc_harrison_jones_za;
    newscript->pGossipSelect = &GossipSelect_npc_harrison_jones_za;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_strange_gong";
    newscript->pGOHello = &GOHello_go_strange_gong;
    newscript->RegisterSelf();
}

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

/* ScriptData
SDName: npcs_karazhan
SD%Complete: 35
SDComment: Barnes: Not fully complete. Berthold: needs testing
SDCategory: Karazhan
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"
#include "def_karazhan.h"


/*######
# npc_barnesAI
######*/

#define GOSSIP_READY        "I'm not an actor."

#define SAY_READY			"Splendid, I'm going to get the audience ready. Break a leg!"
#define SAY_OZ_INTRO1		"Finally, everything is in place. Are you ready for your big stage debut?"
#define OZ_GOSSIP1			"I'm not an actor."
#define SAY_OZ_INTRO2		"Don't worry, you'll be fine. You look like a natural!"
#define OZ_GOSSIP2			"Ok, I'll give it a try, then."

#define SAY_RAJ_INTRO1		"The romantic plays are really tough, but you'll do better this time. You have TALENT. Ready?"
#define RAJ_GOSSIP1			"I've never been more ready."

struct Dialogue
{
    char* text;
    uint32 soundid, timer;
};

static Dialogue OzDialogue[]=
{
    {"Welcome Ladies and Gentlemen, to this evening's presentation!", 9174, 6000},
    {"Tonight we plumb the depths of the human soul as we join a lost, lonely girl trying desperately -- with the help of her loyal companions -- to find her way home!", 9338, 18000},
    {"But she is pursued... by a wicked malevolent crone!", 9339, 9000},
    {"Will she survive? Will she prevail? Only time will tell. And now ... on with the show!", 9340, 13000}
};

static Dialogue HoodDialogue[]=
{
    {"Good evening, Ladies and Gentlemen! Welcome to this evening's presentation!", 9175, 6000},
    {"Tonight, things are not what they seem. For tonight, your eyes may not be trusted", 9335, 10000},
    {"Take for instance, this quiet, elderly woman, waiting for a visit from her granddaughter. Surely there is nothing to fear from this sweet, grey-haired, old lady.", 9336, 14000},
    {"But don't let me pull the wool over your eyes. See for yourself what lies beneath these covers! And now... on with the show!", 9337, 13000}
};

static Dialogue RAJDialogue[]=
{
    {"Welcome, Ladies and Gentlemen, to this evening's presentation!", 9176, 5000},
    {"Tonight, we explore a tale of forbidden love!", 9341, 7000},
    {"But beware, for not all love stories end happily, as you may find out. Sometimes, love pricks like a thorn.", 9342, 14000},
    {"But don't take it from me, see for yourself what tragedy lies ahead as the paths of star-crossed lovers meet. And now...on with the show!", 9343, 13000}
};

#define SAY_JUL_RES_ROMEO	"Come, gentle night; and give me back my Romulo!"
#define SPELL_ETERNALAFF	30878

#define EVENT_HOOD	1
#define EVENT_OZ	2
#define EVENT_RAJ	3

#define POS_Z	90.5
#define POS_OR	4.5

#define WOLF_X	-10892
#define WOLF_Y	-1758

#define OZ_Y	-1750
#define ROAR_X	-10891
#define DOR_X	-10896
#define TIN_X	-10884
#define STRAW_X	-10902
#define CRONE_X -10890
#define CRONE_Y	-1747

struct WayPoint
{
    float x,y,o;
    uint32 timer;
    bool WalkForward, WalkBackward;
};

#define JUL_X	-10892
#define JUL_Y	-1758

static WayPoint Points[]=
{
    {-10866.711, -1779.816, 2.989, 1000, true, false},
    {-10894.917, -1775.467, 4.556, 14500, true, true},
    {-10896.044, -1782.619, 4.556, 4000, false, true}
};

#define SPELL_SPOTLIGHT		25824
#define SPELL_ROOTSELF		31366
#define SPELL_TUXEDO        34126

struct MANGOS_DLL_DECL npc_barnesAI : public ScriptedAI
{
    npc_barnesAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;

    bool IsTalking;
    bool IsWalking;
    bool HasTalked;

    uint32 TalkTimer;
    uint32 WalkTimer;

    uint32 WalkCount;
    uint32 TalkCount;

    uint32 Event; // 0 - OZ, 2 - HOOD, 3 - RAJ

    void Reset()
    {
        IsTalking = false;
        IsWalking = false;
        HasTalked = false;

        TalkCount = 0;
        TalkTimer = 0;
        WalkCount = 0;
        WalkTimer = 0;

        Event = 0;
    }

    void Aggro(Unit* who)
    {
    }

    void StartEvent(Unit* target)
    {
        if(!pInstance)
            return;

        Event = pInstance->GetData(DATA_OPERA_PERFORMANCE);
        pInstance->SetData(DATA_BARNES_INTRO, 1); // IN Progress

        IsTalking = false;
        TalkCount = 0;

        IsWalking = true;
        WalkCount = 0;
        WalkTimer = 0;
        DoCast(m_creature, SPELL_TUXEDO, true);
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
    }

    void Walk(uint32 count)
    {
        float x = 0;
        float y = 0;
        float o = 0;
        if(Points[count].x)
            x = Points[count].x;
        if(Points[count].y)
            y = Points[count].y;
        if(Points[count].o)
            o = Points[count].o;
        if(Points[count].timer)
            WalkTimer = Points[count].timer;
        if(!HasTalked)
            IsWalking = Points[count].WalkForward;
        else IsWalking = Points[count].WalkBackward;

        m_creature->Relocate(x, y, m_creature->GetPositionZ(), o);
        m_creature->SendMoveToPacket(x, y, m_creature->GetPositionZ(), false, WalkTimer);
    }

    void Talk(uint32 count)
    {
        char* text = NULL;
        uint32 sound = 0;
        TalkTimer = 0;
        switch(Event)
        {
            case 0:
                if(OzDialogue[count].text)
                    text = OzDialogue[count].text;
                if(OzDialogue[count].soundid)
                    sound = OzDialogue[count].soundid;
                if(OzDialogue[count].timer)
                    TalkTimer = OzDialogue[count].timer;
                break;
            case 1:
                if(HoodDialogue[count].text)
                    text = HoodDialogue[count].text;
                if(HoodDialogue[count].soundid)
                    sound = HoodDialogue[count].soundid;
                if(HoodDialogue[count].timer)
                    TalkTimer = HoodDialogue[count].timer;
                break;
            case 2:
                if(RAJDialogue[count].text)
                    text = RAJDialogue[count].text;
                if(RAJDialogue[count].soundid)
                    sound = RAJDialogue[count].soundid;
                if(RAJDialogue[count].timer)
                    TalkTimer = RAJDialogue[count].timer;
                break;
        }

        DoYell(text, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, sound);
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
    }

    void UpdateAI(const uint32 diff)
    {
        if(IsWalking)
        {
            if(WalkTimer < diff)
            {
                Walk(WalkCount);
                if(!HasTalked)
                {
                    WalkCount++;
                    if(WalkCount > 2)
                    {
                        DoCast(m_creature, SPELL_SPOTLIGHT, false);
                        IsWalking = false;
                        IsTalking = true;
                    }
                }
                else
                {
                    WalkCount--;
                    if(!WalkCount)
                    {
                        if(pInstance)
                            pInstance->SetData(DATA_BARNES_INTRO, 2); // Complete
                        m_creature->RemoveAurasDueToSpell(SPELL_TUXEDO);
                        IsWalking = false;
                    }
                }
            }else WalkTimer -= diff;
        }

        if(IsTalking)
        {
            if(TalkTimer < diff)
            {
                Talk(TalkCount);
                TalkCount++;
                if(TalkCount >= 4)
                {
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    m_creature->RemoveAurasDueToSpell(SPELL_SPOTLIGHT);
                    IsTalking = false;
                    HasTalked = true;
                    IsWalking = true;
                }
            }else TalkTimer -= diff;
        }

        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_barnes(Creature* _Creature)
{
    return new npc_barnesAI(_Creature);
}

bool GossipHello_npc_barnes(Player* player, Creature* _Creature)
{
    // TODO: Check for death of Moroes 

    player->ADD_GOSSIP_ITEM(0, GOSSIP_READY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->SEND_GOSSIP_MENU(11230, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_barnes(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_INFO_DEF+1)
        ((npc_barnesAI*)_Creature->AI())->StartEvent(player);

    player->PlayerTalkClass->CloseGossip();
    return true;
}

/*###
# npc_berthold
####*/

#define SPELL_TELEPORT           39567

#define GOSSIP_ITEM_TELEPORT     "Teleport me to the Guardian's Library"

bool GossipHello_npc_berthold(Player* player, Creature* _Creature)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)_Creature->GetInstanceData());
    if(pInstance && (pInstance->GetData(DATA_SHADEOFARAN_EVENT) >= 3)) // Check if Shade of Aran is dead or not
        player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_TELEPORT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());
    return true;
}

bool GossipSelect_npc_berthold(Player* player, Creature* _Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_INFO_DEF + 1)
        player->CastSpell(player, SPELL_TELEPORT, true);

    return true;
}

void AddSC_npcs_karazhan()
{
    Script* newscript;
    newscript = new Script;
    newscript->GetAI = GetAI_npc_barnes;
    newscript->Name = "npc_barnes";
    newscript->pGossipHello = GossipHello_npc_barnes;
    newscript->pGossipSelect = GossipSelect_npc_barnes;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "npc_berthold";
    newscript->pGossipHello = GossipHello_npc_berthold;
    newscript->pGossipSelect = GossipSelect_npc_berthold;
    m_scripts[nrscripts++] = newscript;
}

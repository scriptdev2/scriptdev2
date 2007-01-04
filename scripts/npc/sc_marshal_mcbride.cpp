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


#include "../sc_defines.h"

////////////////////////////////////
//      SCRIPT IS FOR TESTING     //
////////////////////////////////////

bool GossipHello_marshal_mcbride(Player *player, Creature *_Creature)
{
    return false;
}

bool GossipSelect_marshal_mcbride(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{

    return false;
}

bool GossipSelectWithCode_marshal_mcbride( Player *player, Creature *_Creature, uint32 sender, uint32 action, char* sCode )
{

    return false;
}

bool QuestAccept_marshal_mcbride(Player *player, Creature *_Creature, Quest *_Quest )
{

    return false;
}

bool QuestSelect_marshal_mcbride(Player *player, Creature *_Creature, Quest *_Quest )
{

    return false;
}

bool QuestComplete_marshal_mcbride(Player *player, Creature *_Creature, Quest *_Quest )
{

    return false;
}

uint32 NPCDialogStatus_marshal_mcbride(Player *player, Creature *_Creature )
{
    return _Creature->QUEST_DIALOG_STATUS(player, DIALOG_STATUS_CHAT);
}

bool ChooseReward_marshal_mcbride(Player *player, Creature *_Creature, Quest *_Quest, uint32 opt )
{
    /*if questid == 21: # Skirmish at Echo Ridge
        self.Emote   (co.EMOTE_ONESHOT_SALUTE)
        player.Emote (co.EMOTE_ONESHOT_SALUTE)
        self.Say     (player, "You are dismissed.", co.LANG_UNIVERSAL)

    GiveFollowUpAndClose(self, player, questid)	*/

    return false;
}

void AddSC_marshal_mcbride()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="marshal_mcbride";
    newscript->pGossipHello          = &GossipHello_marshal_mcbride;
    newscript->pGossipSelect         = &GossipSelect_marshal_mcbride;
    newscript->pGossipSelectWithCode = &GossipSelectWithCode_marshal_mcbride;

    newscript->pQuestAccept          = &QuestAccept_marshal_mcbride;
    newscript->pQuestSelect          = &QuestSelect_marshal_mcbride;
    newscript->pQuestComplete        = &QuestComplete_marshal_mcbride;
    newscript->pNPCDialogStatus      = &NPCDialogStatus_marshal_mcbride;
    newscript->pChooseReward         = &ChooseReward_marshal_mcbride;
    m_scripts[nrscripts++] = newscript;
}

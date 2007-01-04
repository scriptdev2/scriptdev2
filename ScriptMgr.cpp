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


#include "config.h"
#include "ScriptMgr.h"
#include "../../game/TargetedMovementGenerator.h"

uint8 loglevel = 0;
int nrscripts;
Script *m_scripts[MAX_SCRIPTS];

// -- Scripts to be added --

//Area

//Boss

// Creature
extern void AddSC_kobold();
extern void AddSC_generic_caster();

//Custom

//GO

//Guard
extern void AddSC_guard_bluffwatcher();
extern void AddSC_guard_contested();
extern void AddSC_guard_darnassus();
extern void AddSC_guard_dunmorogh();
extern void AddSC_guard_durotar();
extern void AddSC_guard_elwynnforest();
extern void AddSC_guard_ironforge();
extern void AddSC_guard_mulgore();
extern void AddSC_guard_orgrimmar();
extern void AddSC_guard_stormwind();
extern void AddSC_guard_teldrassil();
extern void AddSC_guard_tirisfal();
extern void AddSC_guard_undercity();

//Honor
extern void AddSC_Honor_Vendor();

//Item

//NPC
extern void AddSC_marshal_mcbride();
extern void AddSC_skorn_whitecloud();
extern void AddSC_silva_filnaveth();

//Servers
extern void AddSC_battlemaster();
extern void AddSC_guildmaster();
extern void AddSC_travelmaster();


// -------------------

MANGOS_DLL_EXPORT
void ScriptsFree()
{                                                           // Free resources before library unload
    for(int i=0;i<nrscripts;i++)
        delete m_scripts[i];
}

MANGOS_DLL_EXPORT
void ScriptsInit()
{
    nrscripts = 0;
    for(int i=0;i<MAX_SCRIPTS;i++)
        m_scripts[i]=NULL;

    // -- Inicialize the Scripts to be Added --

    //Area

    //Boss

    // Creature
    AddSC_kobold();
    AddSC_generic_caster();

    //Custom

    //GO

    //Guard
    AddSC_guard_bluffwatcher();
    AddSC_guard_contested();
    AddSC_guard_darnassus();
    AddSC_guard_dunmorogh();
    AddSC_guard_durotar();
    AddSC_guard_elwynnforest();
    AddSC_guard_ironforge();
    AddSC_guard_mulgore();
    AddSC_guard_orgrimmar();
    AddSC_guard_stormwind();
    AddSC_guard_teldrassil();
    AddSC_guard_tirisfal();
    AddSC_guard_undercity();

    //Honor
    AddSC_Honor_Vendor();

    //Item

    //NPC
    AddSC_marshal_mcbride();
    AddSC_skorn_whitecloud();
    AddSC_silva_filnaveth();

    //Servers
    AddSC_battlemaster();
    AddSC_guildmaster();
    AddSC_travelmaster();


    // ----------------------------------------

}

Script* GetScriptByName(std::string Name)
{
    for(int i=0;i<MAX_SCRIPTS;i++)
    {
        if( m_scripts[i] && m_scripts[i]->Name == Name )
            return m_scripts[i];
    }
    return NULL;
}

MANGOS_DLL_EXPORT
bool GossipHello ( Player * player, Creature *_Creature )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipHello(player,_Creature);
}

MANGOS_DLL_EXPORT
bool GossipSelect( Player *player, Creature *_Creature,uint32 sender, uint32 action )
{
    Script *tmpscript = NULL;

    printf("action: %d\n",action);

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelect(player,_Creature,sender,action);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode( Player *player, Creature *_Creature, uint32 sender, uint32 action, char* sCode )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGossipSelectWithCode) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelectWithCode(player,_Creature,sender,action,sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept( Player *player, Creature *_Creature, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestAccept(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestSelect( Player *player, Creature *_Creature, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestSelect) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestSelect(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool QuestComplete( Player *player, Creature *_Creature, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pQuestComplete) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestComplete(player,_Creature,_Quest);
}

MANGOS_DLL_EXPORT
bool ChooseReward( Player *player, Creature *_Creature, Quest *_Quest, uint32 opt )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pChooseReward(player,_Creature,_Quest,opt);
}

MANGOS_DLL_EXPORT
uint32 NPCDialogStatus( Player *player, Creature *_Creature )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pNPCDialogStatus) return 100;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pNPCDialogStatus(player,_Creature);
}

MANGOS_DLL_EXPORT
bool ItemHello( Player *player, Item *_Item, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemHello(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept( Player *player, Item *_Item, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_Item->GetProto()->ScriptName);
    if(!tmpscript || !tmpscript->pItemQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemQuestAccept(player,_Item,_Quest);
}

MANGOS_DLL_EXPORT
bool GOHello( Player *player, GameObject *_GO )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOHello) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOHello(player,_GO);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept( Player *player, GameObject *_GO, Quest *_Quest )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOQuestAccept) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOQuestAccept(player,_GO,_Quest);
}

MANGOS_DLL_EXPORT
bool GOChooseReward( Player *player, GameObject *_GO, Quest *_Quest, uint32 opt )
{
    Script *tmpscript = NULL;

    tmpscript = GetScriptByName(_GO->GetGOInfo()->ScriptName);
    if(!tmpscript || !tmpscript->pGOChooseReward) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOChooseReward(player,_GO,_Quest,opt);
}

MANGOS_DLL_EXPORT
bool AreaTrigger      ( Player *player, Quest *_Quest, uint32 triggerID )
{
    Script *tmpscript = NULL;

    // TODO: load a script name for the areatriggers
    //tmpscript = GetScriptByName();
    if(!tmpscript || !tmpscript->pAreaTrigger) return false;

    player->PlayerTalkClass->ClearMenus();
    return tmpscript->pAreaTrigger(player,_Quest,triggerID);
}

MANGOS_DLL_EXPORT
CreatureAI* GetAI(Creature *_Creature)
{
    Script *tmpscript = GetScriptByName(_Creature->GetCreatureInfo()->ScriptName);

    if(!tmpscript || !tmpscript->GetAI) return NULL;
    return tmpscript->GetAI(_Creature);
}

void ScriptedAI::AttackStart(Unit* who)
{
    if (!who)
        return;

    if (m_creature->getVictim() == NULL && who->isTargetableForAttack())
    {
        //Begin attack
        DoStartMeleeAttack(who);
    }
}

void ScriptedAI::UpdateAI(const uint32 diff)
{
    //Check if we have a current target
    if( m_creature->getVictim() && m_creature->isAlive())
    {
        //Check if we should stop attacking because our victim is no longer attackable
        if (needToStop() || CheckTether())
        {
            DoStopAttack();
            DoGoHome();
            return;
        }
            
        //If we are within range melee the target
        if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
        {
            if( m_creature->isAttackReady() )
            {
                Unit* newtarget = m_creature->SelectHostilTarget();
                if(newtarget)
                    AttackStart(newtarget);

                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }

        //If we are still alive and we lose our victim it means we killed them
        if(!m_creature->getVictim() && m_creature->isAlive())
        {
            DoStopAttack();
            DoGoHome();
        }
    }
}

void ScriptedAI::AttackStop(Unit *)
{
    if( m_creature->isAlive() )
        DoGoHome();
}

void ScriptedAI::DoStartMeleeAttack(Unit* victim)
{
    if ( m_creature->Attack(victim) )
    {
        (*m_creature)->Mutate(new TargetedMovementGenerator(*victim));
    }
}

void ScriptedAI::DoStartRangedAttack(Unit* victim)
{
    m_creature->Attack(victim);
}


void ScriptedAI::DoStopAttack()
{
    if( m_creature->getVictim() != NULL )
    {
        m_creature->AttackStop();
    }
}

void ScriptedAI::DoGoHome()
{
    if( !m_creature->getVictim() && m_creature->isAlive() )
    {
        (*m_creature)->TargetedHome();
    }
}

bool ScriptedAI::needToStop() const
{
    return ( !m_creature->getVictim()->isTargetableForAttack() || !m_creature->isAlive() );
}

void ScriptedAI::DoPlaySoundToSet(Unit* unit, uint32 sound)
{
    WorldPacket data(4);
    data.SetOpcode(SMSG_PLAY_SOUND);
    data << uint32(sound);
    unit->SendMessageToSet(&data,false);
}

void ScriptedAI::DoFaceTarget(Unit *unit)
{
    //Face target
    m_creature->SetInFront(unit);
                        
    //Send the update to the player
    if ( unit->GetTypeId() == TYPEID_PLAYER)
        m_creature->SendUpdateToPlayer((Player*)unit);
}

bool ScriptedAI::CheckTether()
{
    float rx,ry,rz;
    m_creature->GetRespawnCoord(rx, ry, rz);
    float spawndist = m_creature->GetDistanceSq(rx,ry,rz);

    return (spawndist > 2500.0f);
}
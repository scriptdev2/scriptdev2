/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#include "Common.h"

class Player;
class Creature;
class CreatureAI;
class InstanceData;
class Quest;
class Item;
class GameObject;
class SpellCastTargets;
class Map;
class Unit;

#define MAX_SCRIPTS     5000            //72 bytes each (approx 351kb)

//MAX visible range (size of grid)
#define VISIBLE_RANGE   (166.0f)

#define DEFAULT_TEXT    "<ScriptDev2 Text Entry Missing!>"

//
struct Script
{
    Script() :
pGossipHello(NULL), pQuestAccept(NULL), pGossipSelect(NULL), pGossipSelectWithCode(NULL),
pQuestSelect(NULL), pQuestComplete(NULL), pNPCDialogStatus(NULL), pGODialogStatus(NULL), pChooseReward(NULL),
pItemHello(NULL), pGOHello(NULL), pAreaTrigger(NULL), pItemQuestAccept(NULL), pGOQuestAccept(NULL),
pGOChooseReward(NULL),pReceiveEmote(NULL),pItemUse(NULL), GetAI(NULL), GetInstanceData(NULL)
{}

std::string Name;

// -- Quest/gossip Methods to be scripted --
bool (*pGossipHello         )(Player*, Creature*);
bool (*pQuestAccept         )(Player*, Creature*, Quest const* );
bool (*pGossipSelect        )(Player*, Creature*, uint32 , uint32  );
bool (*pGossipSelectWithCode)(Player*, Creature*, uint32 , uint32 , const char* );
bool (*pQuestSelect         )(Player*, Creature*, Quest const* );
bool (*pQuestComplete       )(Player*, Creature*, Quest const* );
uint32 (*pNPCDialogStatus   )(Player*, Creature* );
uint32 (*pGODialogStatus    )(Player *player, GameObject * _GO );
bool (*pChooseReward        )(Player*, Creature*, Quest const*, uint32 );
bool (*pItemHello           )(Player*, Item*, Quest const* );
bool (*pGOHello             )(Player*, GameObject* );
bool (*pAreaTrigger         )(Player*, Quest const*, uint32 );
bool (*pItemQuestAccept     )(Player*, Item *, Quest const* );
bool (*pGOQuestAccept       )(Player*, GameObject*, Quest const* );
bool (*pGOChooseReward      )(Player*, GameObject*_GO, Quest const*, uint32 );
bool (*pReceiveEmote        )(Player*, Creature*, uint32 );
bool (*pItemUse             )(Player*, Item*, SpellCastTargets const& );

CreatureAI* (*GetAI)(Creature*);
InstanceData* (*GetInstanceData)(Map*);
// -----------------------------------------
};

extern int nrscripts;
extern Script *m_scripts[MAX_SCRIPTS];

// Localized Text function
const char* GetEventAILocalizedText(uint32 entry);
const char* GetScriptLocalizedText(uint32 entry);

//EventAI text function
const char* GetEventAIText(uint32 entry); // TODO: Locales

// Script Text function
void ProcessScriptText(uint32 entry, Creature* pCreature, Unit* target = NULL); // TODO: Locales

#if COMPILER == COMPILER_GNU
#define FUNC_PTR(name,callconvention,returntype,parameters)    typedef returntype(*name)parameters __attribute__ ((callconvention));
#else
#define FUNC_PTR(name, callconvention, returntype, parameters)    typedef returntype(callconvention *name)parameters;
#endif

#endif

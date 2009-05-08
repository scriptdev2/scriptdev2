/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#include "Common.h"
#include "DBCStructure.h"

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
class WorldObject;

#define MAX_SCRIPTS         5000                            //72 bytes each (approx 351kb)
#define VISIBLE_RANGE       (166.0f)                        //MAX visible range (size of grid)
#define DEFAULT_TEXT        "<ScriptDev2 Text Entry Missing!>"

struct Script
{
    Script() :
        pGossipHello(NULL), pQuestAccept(NULL), pGossipSelect(NULL), pGossipSelectWithCode(NULL),
        pQuestSelect(NULL), pQuestComplete(NULL), pNPCDialogStatus(NULL), pGODialogStatus(NULL),
        pChooseReward(NULL), pItemHello(NULL), pGOHello(NULL), pAreaTrigger(NULL), pItemQuestAccept(NULL),
        pGOQuestAccept(NULL), pGOChooseReward(NULL), pItemUse(NULL),
        pEffectDummyCreature(NULL), pEffectDummyGameObj(NULL), pEffectDummyItem(NULL),
        GetAI(NULL), GetInstanceData(NULL)
    {}

    std::string Name;

    //Methods to be scripted
    bool (*pGossipHello         )(Player*, Creature*);
    bool (*pQuestAccept         )(Player*, Creature*, const Quest*);
    bool (*pGossipSelect        )(Player*, Creature*, uint32, uint32);
    bool (*pGossipSelectWithCode)(Player*, Creature*, uint32, uint32, const char*);
    bool (*pQuestSelect         )(Player*, Creature*, const Quest*);
    bool (*pQuestComplete       )(Player*, Creature*, const Quest*);
    uint32 (*pNPCDialogStatus   )(Player*, Creature*);
    uint32 (*pGODialogStatus    )(Player*, GameObject*);
    bool (*pChooseReward        )(Player*, Creature*, const Quest*, uint32);
    bool (*pItemHello           )(Player*, Item*, const Quest*);
    bool (*pGOHello             )(Player*, GameObject*);
    bool (*pAreaTrigger         )(Player*, AreaTriggerEntry*);
    bool (*pItemQuestAccept     )(Player*, Item*, const Quest*);
    bool (*pGOQuestAccept       )(Player*, GameObject*, const Quest*);
    bool (*pGOChooseReward      )(Player*, GameObject*, const Quest*, uint32);
    bool (*pItemUse             )(Player*, Item*, SpellCastTargets const&);
    bool (*pEffectDummyCreature )(Unit*, uint32, uint32, Creature*);
    bool (*pEffectDummyGameObj  )(Unit*, uint32, uint32, GameObject*);
    bool (*pEffectDummyItem     )(Unit*, uint32, uint32, Item*);

    CreatureAI* (*GetAI)(Creature*);
    InstanceData* (*GetInstanceData)(Map*);

    void RegisterSelf();
};

//Generic scripting text function
void DoScriptText(int32 textEntry, WorldObject* pSource, Unit* target = NULL);

#if COMPILER == COMPILER_GNU
#define FUNC_PTR(name,callconvention,returntype,parameters)    typedef returntype(*name)parameters __attribute__ ((callconvention));
#else
#define FUNC_PTR(name, callconvention, returntype, parameters)    typedef returntype(callconvention *name)parameters;
#endif

#endif

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


#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

//Only required includes
#include "../../game/CreatureAI.h"
#include "../../game/Creature.h"
#include "../../game/InstanceData.h"

#define MAX_SCRIPTS 5000

class Player;
class Creature;
class Quest;
class Item;
class GameObject;
class SpellCastTargets;
class Map;

struct Script
{
    Script() :
pGossipHello(NULL), pQuestAccept(NULL), pGossipSelect(NULL), pGossipSelectWithCode(NULL),
pQuestSelect(NULL), pQuestComplete(NULL), pNPCDialogStatus(NULL), pChooseReward(NULL),
pItemHello(NULL), pGOHello(NULL), pAreaTrigger(NULL), pItemQuestAccept(NULL), pGOQuestAccept(NULL),
pGOChooseReward(NULL),pReceiveEmote(NULL),pItemUse(NULL), GetAI(NULL), GetInstanceData(NULL)
{}

std::string Name;

// -- Quest/gossip Methods to be scripted --
bool (*pGossipHello         )(Player*, Creature*);
bool (*pQuestAccept         )(Player*, Creature*, Quest const* );
bool (*pGossipSelect        )(Player*, Creature*, uint32 , uint32  );
bool (*pGossipSelectWithCode)(Player*, Creature*, uint32 , uint32 , char* );
bool (*pQuestSelect         )(Player*, Creature*, Quest const* );
bool (*pQuestComplete       )(Player*, Creature*, Quest const* );
uint32 (*pNPCDialogStatus   )(Player*, Creature* );
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

#define VISIBLE_RANGE (26.46f)

//Spell targets used by SelectSpell
enum SelectTarget
{
    SELECT_TARGET_DONTCARE = 0,         //All target types allowed

    SELECT_TARGET_SELF,                 //Only Self casting

    SELECT_TARGET_SINGLE_ENEMY,         //Only Single Enemy
    SELECT_TARGET_AOE_ENEMY,            //Only AoE Enemy
    SELECT_TARGET_ANY_ENEMY,            //AoE or Single Enemy

    SELECT_TARGET_SINGLE_FRIEND,        //Only Single Friend
    SELECT_TARGET_AOE_FRIEND,           //Only AoE Friend
    SELECT_TARGET_ANY_FRIEND,           //AoE or Single Friend
};

//Spell Effects used by SelectSpell
enum SelectEffect
{
    SELECT_EFFECT_DONTCARE = 0,         //All spell effects allowed
    SELECT_EFFECT_DAMAGE,               //Spell does damage
    SELECT_EFFECT_HEALING,              //Spell does healing
    SELECT_EFFECT_AURA,                 //Spell applies an aura
};

//Selection method used by SelectTarget
enum SelectAggroTarget
{
    SELECT_TARGET_RANDOM = 0,           //Just selects a random target
    SELECT_TARGET_TOPAGGRO,             //Selects targes from top aggro to bottom
    SELECT_TARGET_BOTTOMAGGRO,          //Selects targets from bottom aggro to top
};


struct MANGOS_DLL_DECL ScriptedAI : public CreatureAI
{
    ScriptedAI(Creature* creature) : m_creature(creature) {}
    ~ScriptedAI() {}

    //*************
    //CreatureAI Functions to be Scripted
    //*************

    //Called if IsVisible(Unit *who) is true at each *who move
    void MoveInLineOfSight(Unit *) {}

    //Called at each attack of m_creature by any victim
    void AttackStart(Unit *);

    //Called at stoping attack by any attacker
    void EnterEvadeMode();

    //Called at any heal cast/item used (call non implemented in mangos)
    void HealBy(Unit *healer, uint32 amount_healed) {}

    // Called at any Damage to any victim (before damage apply)
    void DamageDeal(Unit *done_to, uint32 &damage) {}

    // Called at any Damage from any attacker (before damage apply)
    void DamageTaken(Unit *done_by, uint32 &damage) {}

    //Is unit visible for MoveInLineOfSight
    bool IsVisible(Unit *who) const;

    //Called at World update tick
    void UpdateAI(const uint32);

    //Called at creature death
    void JustDied(Unit*){}

    //Called at creature killing another unit
    void KilledUnit(Unit*){}

    //Pointer to creature we are manipulating
    Creature* m_creature;

    //*************
    //AI Helper Functions
    //*************

    //Start attack of victim and go to him
    void DoStartMeleeAttack(Unit* victim);

    //Start attack of victim but stay in position
    void DoStartRangedAttack(Unit* victim);

    //Do melee swing of current victim if in rnage and ready and not casting
    void DoMeleeAttackIfReady();

    //Stop attack of current victim
    void DoStopAttack();

    //Cast spell by Id
    void DoCast(Unit* victim, uint32 spelId);

    //Cast spell by spell info
    void DoCastSpell(Unit* who,SpellEntry const *spellInfo);

    //Creature say
    void DoSay(const char* text, uint32 language, Unit* target);

    //Creature Yell
    void DoYell(const char* text, uint32 language, Unit* target);

    //Creature Text emote
    void DoTextEmote(const char* text, Unit* target);

    //Go back to spawn point
    void DoGoHome();

    //Plays a sound to all nearby players
    void DoPlaySoundToSet(Unit* unit, uint32 sound);

    //Faces target
    void DoFaceTarget(Unit* unit);

    //Spawns a creature relative to m_creature
    Creature* DoSpawnCreature(uint32 id, float x, float y, float z, float angle, uint32 type, uint32 despawntime);

    //Selects a unit from the creature's current aggro list
    Unit* SelectUnit(SelectAggroTarget target, uint32 position);

    //Returns spells that meet the specified criteria from the creatures spell list
    SpellEntry const* SelectSpell(Unit* Target, int32 School, int32 Mechanic, SelectTarget Targets,  uint32 PowerCostMin, uint32 PowerCostMax, float RangeMin, float RangeMax, SelectEffect Effect);

    //Checks if you can cast the specified spell
    bool CanCast(Unit* Target, SpellEntry const *Spell);
};

class MANGOS_DLL_DECL ScriptedInstance : public InstanceData
{
    public:

        ScriptedInstance(Map *map) : InstanceData(map) {}
        ~ScriptedInstance() {}

        //Retrevial of unit guids
        virtual uint64 GetUnitGUID(char *identifier) {return 0; }

        //All-purpose data storage
        virtual uint32 GetData(char *type) { return 0; }
        virtual void SetData(char *type, uint32 data) {}
};

#endif

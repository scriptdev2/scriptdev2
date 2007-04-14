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

#include "../../game/Player.h"
#include "../../game/GameObject.h"
#include "../../game/SharedDefines.h"
#include "../../game/GossipDef.h"
#include "../../game/QuestDef.h"
#include "../../game/WorldSession.h"
#include "../../game/CreatureAI.h"
#include "../../game/TargetedMovementGenerator.h"
#include "../../shared/WorldPacket.h"
#include "../../shared/Database/DBCStores.h"

#define MAX_SCRIPTS 1000

struct Script
{
    Script() :
    pGossipHello(NULL), pQuestAccept(NULL), pGossipSelect(NULL), pGossipSelectWithCode(NULL),
        pQuestSelect(NULL), pQuestComplete(NULL), pNPCDialogStatus(NULL), pChooseReward(NULL),
        pItemHello(NULL), pGOHello(NULL), pAreaTrigger(NULL), pItemQuestAccept(NULL), pGOQuestAccept(NULL),
        pGOChooseReward(NULL),pReceiveEmote(NULL),pItemUse(NULL), GetAI(NULL)
        {}

    std::string Name;

    // -- Quest/gossip Methods to be scripted --
    bool (*pGossipHello         )(Player *player, Creature *_Creature);
    bool (*pQuestAccept         )(Player *player, Creature *_Creature, Quest *_Quest );
    bool (*pGossipSelect        )(Player *player, Creature *_Creature, uint32 sender, uint32 action );
    bool (*pGossipSelectWithCode)(Player *player, Creature *_Creature, uint32 sender, uint32 action, char* sCode );
    bool (*pQuestSelect         )(Player *player, Creature *_Creature, Quest *_Quest );
    bool (*pQuestComplete       )(Player *player, Creature *_Creature, Quest *_Quest );
    uint32 (*pNPCDialogStatus   )(Player *player, Creature *_Creature );
    bool (*pChooseReward        )(Player *player, Creature *_Creature, Quest *_Quest, uint32 opt );
    bool (*pItemHello           )(Player *player, Item *_Item, Quest *_Quest );
    bool (*pGOHello             )(Player *player, GameObject *_GO );
    bool (*pAreaTrigger         )(Player *player, Quest *_Quest, uint32 triggerID );
    bool (*pItemQuestAccept     )(Player *player, Item *_Item, Quest *_Quest );
    bool (*pGOQuestAccept       )(Player *player, GameObject *_GO, Quest *_Quest );
    bool (*pGOChooseReward      )(Player *player, GameObject *_GO, Quest *_Quest, uint32 opt );

    CreatureAI* (*GetAI)(Creature *_Creature);

    bool (*pReceiveEmote         )(Player *player, Creature *_Creature, uint32 emote);
    bool (*pItemUse             )(Player *player, Item* _Item);
    // -----------------------------------------

};

extern int nrscripts;
extern Script *m_scripts[MAX_SCRIPTS];

#define VISIBLE_RANGE (26.46f)

//Spell Targets (used in Select Spell)
enum Targets
{
    TARGET_SELF                        = 1,
    TARGET_PET                         = 5,
    TARGET_SINGLE_ENEMY                = 6,
    TARGET_ALL_ENEMY_IN_AREA           = 15,
    TARGET_ALL_ENEMY_IN_AREA_INSTANT   = 16,
    TARGET_ALL_PARTY_AROUND_CASTER     = 20,
    TARGET_SINGLE_FRIEND               = 21,
    TARGET_ALL_ENEMIES_AROUND_CASTER   = 22,
    TARGET_GAMEOBJECT                  = 23,
    TARGET_IN_FRONT_OF_CASTER          = 24,
    TARGET_DUELVSPLAYER                = 25,
    TARGET_GAMEOBJECT_ITEM             = 26,
    TARGET_ALL_ENEMY_IN_AREA_CHANNELED = 28,
    TARGET_MINION                      = 32,
    TARGET_SINGLE_PARTY                = 35,
    TARGET_AREAEFFECT_PARTY            = 37,
    TARGET_SELF_FISHING                = 39,
    TARGET_TOTEM_EARTH                 = 41,
    TARGET_TOTEM_WATER                 = 42,
    TARGET_TOTEM_AIR                   = 43,
    TARGET_TOTEM_FIRE                  = 44,
    TARGET_CHAIN                       = 45,
    TARGET_DYNAMIC_OBJECT              = 47,
    TARGET_CURRENT_SELECTED_ENEMY      = 53,
    TARGET_SINGLE_FRIEND_2             = 57,        //This is incorrectly labled, this should say SINGLE_ENEMY_2. Needs to be examined
    TARGET_AREAEFFECT_PARTY_AND_CLASS  = 61,
};

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

//Chat defines
#define CHAT_MSG_MONSTER_SAY    0x0B
#define CHAT_MSG_MONSTER_YELL   0x0C
#define CHAT_MSG_MONSTER_EMOTE  0x0D

struct MANGOS_DLL_DECL ScriptedAI : public CreatureAI
{
    ScriptedAI(Creature* creature) : m_creature(creature) {}
    ~ScriptedAI() {}

    //Called if IsVisible(Unit *who) is true at each *who move
    void MoveInLineOfSight(Unit *) {}

    //Called at each attack of m_creature by any victim
    void AttackStart(Unit *);

    //Called at stoping attack by any attacker
    void EnterEvadeMode();

    //Called at any heal cast/item used (call non implemented in mangos)
    void HealBy(Unit *healer, uint32 amount_healed) {}

    //Called at any Damage from any attacker
    void DamageInflict(Unit *healer, uint32 amount_healed) {}

    //Is unit visible for MoveInLineOfSight
    bool IsVisible(Unit *who) const
    {
        if (!who)
            return false;

        return m_creature->IsWithinDistInMap(who, VISIBLE_RANGE) && who->isVisibleFor(m_creature,true);
    }

    //Called at World update tick
    void UpdateAI(const uint32);

    void JustDied(Unit*){}

    void KilledUnit(Unit*){}

    Creature* m_creature;

    //Check condition for attack stop
    virtual bool needToStop() const;

    //*************
    //AI Helper Functions
    //*************

    //Start attack of victim and go to him
    void DoStartMeleeAttack(Unit* victim);

    //Start attack of victim but stay in position
    void DoStartRangedAttack(Unit* victim);

    //Stop attack of current victim
    void DoStopAttack();

    //Cast spell by Id
    void DoCast(Unit* victim, uint32 spelId);

    //Cast spell by spell info
    void DoCastSpell(Unit* who,SpellEntry const *spellInfo);

    //Creature say
    void DoSay(char const* text, uint32 language, Unit* target);

    //Creature Yell
    void DoYell(char const* text, uint32 language, Unit* target);

    //Creature Text emote
    void DoTextEmote(char const* text, Unit* target);

    //Go back to spawn point
    void DoGoHome();

    //Plays a sound to all nearby players
    void DoPlaySoundToSet(Unit* unit, uint32 sound);

    //Faces target
    void DoFaceTarget(Unit* unit);

    //Spawns a creature relative to m_creature
    Creature* ScriptedAI::DoSpawnCreature(uint32 id, float x, float y, float z, float angle, TempSummonType t, uint32 despawntime);
    
    //Returns spells that meet the specified criteria from the creatures spell list
    SpellEntry const* SelectSpell(Unit* Target, uint32 School, uint32 Mechanic, SelectTarget Targets,  uint32 PowerCostMin, uint32 PowerCostMax, float RangeMin, float RangeMax, SelectEffect Effect);

    //Checks if you can cast the specified spell
    bool CanCast(Unit* Target, SpellEntry const *Spell);

    //Returns true if you are out of tether(spawnpoint) range
    bool CheckTether();
};
#endif

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

#define MAX_SCRIPTS 1000

struct Script
{
    Script() :
    pGossipHello(NULL), pQuestAccept(NULL), pGossipSelect(NULL), pGossipSelectWithCode(NULL),
        pQuestSelect(NULL), pQuestComplete(NULL), pNPCDialogStatus(NULL), pChooseReward(NULL),
        pItemHello(NULL), pGOHello(NULL), pAreaTrigger(NULL), pItemQuestAccept(NULL), pGOQuestAccept(NULL),
        pGOChooseReward(NULL), GetAI(NULL)
#ifdef SCRIPT_EXTENDED
        ,pReciveEmote(NULL)
#endif
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

#ifdef SCRIPT_EXTENDED
    bool (*pReciveEmote         )(Player *player, Creature *_Creature, uint32 emote);
#endif
    // -----------------------------------------

};

#ifdef SCRIPT_EXTENDED
extern DBCStorage <SpellRangeEntry> *pSpellRangeStore;
extern DBCStorage <SpellEntry> *pSpellStore;
#endif

extern int nrscripts;
extern Script *m_scripts[MAX_SCRIPTS];

#define VISIBLE_RANGE (26.46f)

struct MANGOS_DLL_DECL ScriptedAI : public CreatureAI
{
    ScriptedAI(Creature* creature) : m_creature(creature) {}
    ~ScriptedAI() {}

    //Called if IsVisible(Unit *who) is true at each *who move
    void MoveInLineOfSight(Unit *) {}

    //Called at each attack of m_creature by any victim
    void AttackStart(Unit *);

    //Called at stoping attack by any attacker
    void AttackStop(Unit *);

    //Called at any heal cast/item used (call non implemented in mangos)
    void HealBy(Unit *healer, uint32 amount_healed) {}

    //Called at any Damage from any attacker
    void DamageInflict(Unit *healer, uint32 amount_healed) {}

    //Is unit visibale for MoveInLineOfSight
    bool IsVisible(Unit *who) const
    {
        if (!who)
            return false;

        return !who->HasStealthAura() && m_creature->GetDistanceSq(who) <= VISIBLE_RANGE;
    }

    //Called at World update tick
    void UpdateAI(const uint32);

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
    void DoCast(Unit* victim, uint32 spelId)
    {
        m_creature->CastSpell(victim, spelId, false);
    }

    //Cast spell by spell info
    void DoCastSpell(Unit* who,SpellEntry const *spellInfo)
    {
        m_creature->StopMoving();
        m_creature->CastSpell(who, spellInfo, false);
    }

    //Creature say
    void DoSay(char const* text, uint32 language);

    //Creature Yell
    void DoYell(char const* text, uint32 language);

    //Creature Text emote
    void DoTextEmote(char const* text);

    //Go back to spawn point
    void DoGoHome();

    //Plays a sound to all nearby players
    void DoPlaySoundToSet(Unit* unit, uint32 sound);

    //Faces target
    void DoFaceTarget(Unit* unit);

    //Selects a random spell from creature spell list to use
    SpellEntry const* SelectSpell(Unit* target);

    //Returns true if you are out of tether(spawnpoint) range
    bool CheckTether();
};
#endif

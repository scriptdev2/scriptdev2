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
SDName: Mob_Event_AI
SD%Complete: 90
SDComment: Database Event AI
SDCategory: Creatures
EndScriptData */

#include "mob_event_ai.h"
#include "../../../../game/Player.h"
#include "../../../../game/TargetedMovementGenerator.h"

#define EVENT_UPDATE_TIME   500
#define SPELL_RUN_AWAY      8225

struct EventHolder
{
    EventHolder(EventAI_Event p) : Event(p), Time(0), Enabled(true){}

    EventAI_Event Event;
    uint32 Time;
    bool Enabled;
};

struct MANGOS_DLL_DECL Mob_EventAI : public ScriptedAI
{
    Mob_EventAI(Creature *c, std::list<EventHolder> pEventList) : ScriptedAI(c) 
    {
        EventList = pEventList;
        EnterEvadeMode();
    }

    ~Mob_EventAI()
    {
        EventList.clear();
    }

    std::list<EventHolder> EventList;       //Holder for events (stores enabled, time, and eventid)
    bool InCombat;                          //Combat var
    bool CombatMovementEnabled;             //If we allow targeted movment gen (movement twoards top threat)
    bool MeleeEnabled;                      //If we allow melee auto attack
    uint32 EventUpdateTime;                 //Time between event updates
    uint32 EventDiff;                       //Time between the last event call
    uint8 Phase;                            //Current phase, max 32 phases

    void ProcessEvent(EventHolder& pHolder, Unit* pOverrideChatTarget = NULL)
    {  
        if (!pHolder.Enabled || pHolder.Time)
            return;

        //Check the inverse phase mask (event doesn't trigger if current phase bit is set in mask)
        if (pHolder.Event.event_inverse_phase_mask & (1 << Phase))
            return;

        //Store random here so that all random actions match up
        uint32 rnd = rand();

        union 
        {
            uint32 param1;
            int32 param1_s;
        };

        union 
        {
            uint32 param2;
            int32 param2_s;
        };

        union 
        {
            uint32 param3;
            int32 param3_s;
        };

        param1 = pHolder.Event.event_param1;
        param2 = pHolder.Event.event_param2;
        param3 = pHolder.Event.event_param3;

        //Check event conditions based on the event type, also reset events
        switch (pHolder.Event.event_type)
        {
        case EVENT_T_TIMER_REPEAT:
            {
                if (!InCombat)
                    return;

                //Check for negative param3 (chance%)
                if (param3_s < 0 && -param3_s < rnd % 100)
                    return;

                if (param3_s > 0)
                    pHolder.Time = param1 + (rnd % param3);
                else pHolder.Time = param1;
            }
            break;
        case EVENT_T_TIMER_SINGLE:
            {
                if (!InCombat || param2 < rnd % 100)
                    return;

                pHolder.Enabled = false;
            }
            break;
        case EVENT_T_TIMER_OOC_REPEAT:
            {
                if (InCombat)
                    return;

                //Check for negative param3 (chance%)
                if (param3_s < 0 && -param3_s < rnd % 100)
                    return;

                if (param3_s > 0)
                    pHolder.Time = param1 + (rnd % param3);
                else pHolder.Time = param1;
            }
            break;
        case EVENT_T_TIMER_OOC_SINGLE:
            {
                if (InCombat || param2 < rnd % 100)
                    return;

                pHolder.Enabled = false;
            }
            break;
        case EVENT_T_HP:
            {
                if (!InCombat || !m_creature->GetMaxHealth())
                    return;

                uint32 perc = (m_creature->GetHealth()*100) / m_creature->GetMaxHealth();

                if (perc > param1 || perc < param2)
                    return;

                //Prevent repeat for param3 time, or disable if param3 not set
                if (param3)
                {
                    if (param3_s > 0)
                        pHolder.Time = param3;
                    else if (-param3_s < rnd % 100)
                        return;
                }else pHolder.Enabled = false;
            }
            break;
        case EVENT_T_MANA:
            {
                if (!InCombat || !m_creature->GetMaxPower(POWER_MANA))
                    return;

                uint32 perc = (m_creature->GetPower(POWER_MANA)*100) / m_creature->GetMaxPower(POWER_MANA);

                if (perc > param1 || perc < param2)
                    return;

                if (param3)
                {
                    if (param3_s > 0)
                        pHolder.Time = param3;
                    else if (-param3_s < rnd % 100)
                        return;
                }else pHolder.Enabled = false;
            }
            break;
        case EVENT_T_AGGRO:
            {
                if (param1 < rnd % 100)
                    return;
            }
            break;
        case EVENT_T_KILL:
            {
                if (param2 < rnd % 100)
                    return;

                if (param1)
                    pHolder.Time = param1;
            }
        case EVENT_T_DEATH:
            {
                if (param1 < rnd % 100)
                    return;
            }
            break;
        case EVENT_T_EVADE:
            {
                if (param1 < rnd % 100)
                    return;
            }
            break;
        case EVENT_T_SPELLHIT:
            {
                if (param3 < rnd % 100)
                    return;

                //Spell hit is special case, first param handled within EventAI::SpellHit
                if (param2)
                    pHolder.Time = param2;
                else
                    pHolder.Enabled = false;
            }
            break;
        }

        //Process actions
        for (uint32 j = 0; j < MAX_ACTIONS; j++)
            ProcessAction(pHolder.Event.action[j].type, pHolder.Event.action[j].param1, pHolder.Event.action[j].param2, pHolder.Event.action[j].param3, rnd, pOverrideChatTarget);
    }

    inline uint32 GetRandActionParam(uint32 rnd, uint32 param1, uint32 param2, uint32 param3)
    {
        switch (rnd % 3)
        {
        case 0:
            return param1;
            break;

        case 1:
            return param2;
            break;

        case 2:
            return param3;
            break;
        }
        return 0;
    }

    inline Unit* GetTargetByType(uint32 Target)
    {
        switch (Target)
        {
        case TARGET_T_SELF:
            return m_creature;
            break;
        case TARGET_T_HOSTILE:
            return m_creature->getVictim();
            break;
        case TARGET_T_HOSTILE_SECOND_AGGRO:
            return SelectUnit(SELECT_TARGET_TOPAGGRO,1);
            break;
        case TARGET_T_HOSTILE_LAST_AGGRO:
            return SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
            break;
        case TARGET_T_HOSTILE_RANDOM:
            return SelectUnit(SELECT_TARGET_RANDOM,0);
            break;
        case TARGET_T_HOSTILE_RANDOM_NOT_TOP:
            return SelectUnit(SELECT_TARGET_RANDOM,1);
            break;
        default:
            return NULL;
            break;
        };
    }

    void ProcessAction(uint16 type, uint32 param1, uint32 param2, uint32 param3, uint32 rnd, Unit* pOverrideChatTarget)
    {
        switch (type)
        {
        case ACTION_T_SAY:
            DoSay(GetLocalizedText(param1), LANG_UNIVERSAL, pOverrideChatTarget ? pOverrideChatTarget : m_creature->getVictim());
            break;

        case ACTION_T_YELL:
            DoYell(GetLocalizedText(param1), LANG_UNIVERSAL, pOverrideChatTarget ? pOverrideChatTarget : m_creature->getVictim());
            break;

        case ACTION_T_TEXTEMOTE:
            DoTextEmote(GetLocalizedText(param1), pOverrideChatTarget ? pOverrideChatTarget : m_creature->getVictim());
            break;

        case ACTION_T_SOUND:
            DoPlaySoundToSet(m_creature, param1);
            break;

        case ACTION_T_EMOTE:
            m_creature->HandleEmoteCommand(param1);
            break;

        case ACTION_T_RANDOM_SAY:
            {
                uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

                if (temp != 0xffffffff)
                    DoSay(GetLocalizedText(temp), LANG_UNIVERSAL, pOverrideChatTarget ? pOverrideChatTarget : m_creature->getVictim());
            }
            break;

        case ACTION_T_RANDOM_YELL:
            {
                uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

                if (temp != 0xffffffff)
                    DoYell(GetLocalizedText(temp), LANG_UNIVERSAL, pOverrideChatTarget ? pOverrideChatTarget : m_creature->getVictim());
            }
            break;

        case ACTION_T_RANDOM_TEXTEMOTE:
            {
                uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

                if (temp != 0xffffffff)
                    DoTextEmote(GetLocalizedText(temp), pOverrideChatTarget ? pOverrideChatTarget : m_creature->getVictim());
            }
            break;

        case ACTION_T_RANDOM_SOUND:
            {
                uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

                if (temp != 0xffffffff)
                    DoPlaySoundToSet(m_creature, temp);
            }
            break;

        case ACTION_T_RANDOM_EMOTE:
            {
                uint32 temp = GetRandActionParam(rnd, param1, param2, param3);

                if (temp != 0xffffffff)
                    m_creature->HandleEmoteCommand(temp);
            }
            break;

        case ACTION_T_CAST:
            {
                Unit* target = GetTargetByType(param2);

                if (!target)
                    return;

                if (param3 & CAST_INTURRUPT_PREVIOUS || !m_creature->IsNonMeleeSpellCasted(false))
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->CastSpell(target, param1, (param3 & CAST_TRIGGERED));
                }
            }
            break;

        case ACTION_T_SUMMON:
            {
                Unit* target = GetTargetByType(param2);

                //Duration
                Creature* pCreature = NULL;
                if (param3)
                    pCreature = DoSpawnCreature(param1, 0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, param3);
                else pCreature = DoSpawnCreature(param1, 0,0,0,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 0);

                if (!pCreature)
                    error_log( "SD2: Eventid failed to spawn creature %u.", param1);
                else if (param2 != TARGET_T_SELF && target)
                    pCreature->AI()->AttackStart(target);
            }
            break;

        case ACTION_T_THREAT_SINGLE_PCT:
            {
                Unit* target = GetTargetByType(param2);

                if (target)
                    m_creature->getThreatManager().modifyThreatPercent(target, param1);
            }
            break;

        case ACTION_T_THREAT_ALL_PCT:
            {
                Unit* Temp = NULL;

                std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                for (; i != m_creature->getThreatManager().getThreatList().end(); ++i)
                {
                    Temp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                    if (Temp)
                        m_creature->getThreatManager().modifyThreatPercent(Temp, param1);
                }
            }
            break;

        case ACTION_T_QUEST_COMPLETE:
            {
                Unit* target = GetTargetByType(param2);

                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)target)->CompleteQuest(param1);
            }
            break;

        case ACTION_T_CASTCREATUREGO:
            {
                Unit* target = GetTargetByType(param3);

                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)target)->CastedCreatureOrGO(param1, m_creature->GetGUID(), param2);
            }
            break;

        case ACTION_T_SET_UNIT_FIELD:
            {
                Unit* target = GetTargetByType(param3);

                if (target)
                    target->SetUInt32Value(param1, param2);
            }
            break;

        case ACTION_T_SET_UNIT_FLAG:
            {
                Unit* target = GetTargetByType(param2);

                if (target)
                    target->SetFlag(UNIT_FIELD_FLAGS, param1);
            }
            break;

        case ACTION_T_REMOVE_UNIT_FLAG:
            {
                Unit* target = GetTargetByType(param2);

                if (target)
                    target->RemoveFlag(UNIT_FIELD_FLAGS, param1);
            }
            break;

        case ACTION_T_AUTO_ATTACK:
            {
                if (param1)
                    MeleeEnabled = true;
                else MeleeEnabled = false;
            }
            break;

        case ACTION_T_COMBAT_MOVEMENT:
            {
                //Allow movement (create new targeted movement gen if none exist already)
                if (param1)
                {
                    if (m_creature->GetMotionMaster()->top()->GetMovementGeneratorType() != TARGETED_MOTION_TYPE)
                        m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
                }
                else
                    if (m_creature->GetMotionMaster()->top()->GetMovementGeneratorType() == TARGETED_MOTION_TYPE)
                    {
                        m_creature->GetMotionMaster()->Clear(false);
                        m_creature->GetMotionMaster()->Idle();
                    }
            }
            break;
        case ACTION_T_SET_PHASE:
            {
                Phase = param1;
            }
            break;
        case ACTION_T_INC_PHASE:
            {
                Phase += param1;

                if (Phase > 31)
                    error_log( "SD2: Eventid incremented Phase above 31. Phase mask cannot be used with phases past 31.");
            }
            break;

        case ACTION_T_EVADE:
            {
                EnterEvadeMode();
            }
            break;
        case ACTION_T_FLEE:
            {
                //TODO: Replace with Flee movement generator
                m_creature->CastSpell(m_creature, SPELL_RUN_AWAY, true);
            }
            break;
        case ACTION_T_QUEST_COMPLETE_ALL:
            {
                Unit* Temp = NULL;

                std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                for (; i != m_creature->getThreatManager().getThreatList().end(); ++i)
                {
                    Temp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                    if (Temp && Temp->GetTypeId() == TYPEID_PLAYER)
                        ((Player*)Temp)->CompleteQuest(param1);
                }
            }
            break;

        case ACTION_T_CASTCREATUREGO_ALL:
            {
                Unit* Temp = NULL;

                std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                for (; i != m_creature->getThreatManager().getThreatList().end(); ++i)
                {
                    Temp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                    if (Temp && Temp->GetTypeId() == TYPEID_PLAYER)
                        ((Player*)Temp)->CastedCreatureOrGO(param1, m_creature->GetGUID(), param2);
                }
            }
            break;
        };
    }

    void Reset(Event_Types e)
    {
        InCombat = false;
        Phase = 0;

        EventUpdateTime = EVENT_UPDATE_TIME;
        EventDiff = 0;

        //Handle Evade events and reset all events to enabled
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            switch ((*i).Event.event_type)
            {
                //Evade
            case EVENT_T_EVADE:
                if (e == EVENT_T_EVADE)
                    ProcessEvent(*i);
                break;

                //Death
            case EVENT_T_DEATH:
                if (e == EVENT_T_DEATH)
                    ProcessEvent(*i);
                break;

                //Reset all out of combat timers
            case EVENT_T_TIMER_OOC_REPEAT:
                if ((*i).Event.event_param3_s > 0)
                    (*i).Time = (*i).Event.event_param2 + (rand() % (*i).Event.event_param3_s);
                else (*i).Time = (*i).Event.event_param2;

                (*i).Enabled = true;
                break;
            case EVENT_T_TIMER_OOC_SINGLE:
                (*i).Time = (*i).Event.event_param1;
                (*i).Enabled = true;
                break;
            }
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        m_creature->LoadCreaturesAddon();
        DoGoHome();

        Reset(EVENT_T_EVADE);
    }

    void JustDied(Unit* killer)
    {
        Reset(EVENT_T_DEATH);
    }

    void KilledUnit(Unit* victim)
    {
        if (victim->GetTypeId() != TYPEID_PLAYER)
            return;

        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            switch ((*i).Event.event_type)
            {
                //Kill
            case EVENT_T_KILL:
                ProcessEvent(*i, victim);
                break;
            }
        }

    }

    void AttackStart(Unit *who)
    {
        if (!who || who == m_creature)
            return;

        if (!InCombat)
        {
            //Combat movement and Melee attack always enabled by default at combat reset
            CombatMovementEnabled = true;
            MeleeEnabled = true;

            //Reset phase to 0
            Phase = 0;

            //Check for on combat start events
            for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
            {
                switch ((*i).Event.event_type)
                {
                case EVENT_T_AGGRO:
                    ProcessEvent(*i, who);
                    break;

                    //Reset all in combat timers
                case EVENT_T_TIMER_REPEAT:
                    if ((*i).Event.event_param3_s > 0)
                        (*i).Time = (*i).Event.event_param2 + (rand() % (*i).Event.event_param3_s);
                    else (*i).Time = (*i).Event.event_param2;                 

                    (*i).Enabled = true;
                    break;
                case EVENT_T_TIMER_SINGLE:
                    (*i).Time = (*i).Event.event_param1;
                    (*i).Enabled = true;
                    break;

                    //All normal events need to be re-enabled and their time set to 0
                default:
                    (*i).Enabled = true;
                    (*i).Time = 0;
                    break;
                }
            }
            InCombat = true;

            EventUpdateTime = EVENT_UPDATE_TIME;
            EventDiff = 0;
        }

        //Begin melee attack if we are within range
        if (CombatMovementEnabled)
            DoStartMeleeAttack(who);
        else DoStartRangedAttack(who);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin attack if we are within range
                AttackStart(who);
            }
        }
    }

    void SpellHit(Unit* pUnit, const SpellEntry* pSpell)
    {
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            switch ((*i).Event.event_type)
            {
                //Spell hit
            case EVENT_T_SPELLHIT:
                {
                    //If spell id matches (or no spell id)
                    if (!(*i).Event.event_param1 || pSpell->Id == (*i).Event.event_param1)
                        ProcessEvent(*i);
                }
                break;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Check if we are in combat (also updates calls threat update code)
        bool Combat = InCombat ? (m_creature->SelectHostilTarget() && m_creature->getVictim()) : false;

        //Must return if creature isn't alive. Normally select hostil target and get victim prevent this
        if (!m_creature->isAlive())
            return;

        //Events are only updated once every EVENT_UPDATE_TIME ms to prevent lag with large amount of events
        if (EventUpdateTime < diff)
        {
            EventDiff += diff;

            //Check for time based events
            for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
            {
                //Decrement Timers
                if ((*i).Time)
                    if ((*i).Time > EventDiff)
                    {
                        //Do not decrement timers if event cannot trigger in this phase
                        if (!((*i).Event.event_inverse_phase_mask & (1 << Phase)))
                            (*i).Time -= EventDiff;

                        //Skip processing of events that have time remaining
                        continue;
                    }
                    else (*i).Time = 0; 

                    switch ((*i).Event.event_type)
                    {
                        //Events that are updated every EVENT_UPDATE_TIME
                    case EVENT_T_TIMER_REPEAT:
                    case EVENT_T_TIMER_SINGLE:
                    case EVENT_T_TIMER_OOC_REPEAT:
                    case EVENT_T_TIMER_OOC_SINGLE:
                    case EVENT_T_MANA:
                    case EVENT_T_HP:
                        ProcessEvent(*i);
                        break;
                    }
            }

            EventDiff = 0;
            EventUpdateTime = EVENT_UPDATE_TIME;
        }else 
        {
            EventDiff += diff;
            EventUpdateTime -= diff;
        }

        //Melee Auto-Attack
        if (Combat && MeleeEnabled)
            DoMeleeAttackIfReady();

    }
};
CreatureAI* GetAI_Mob_EventAI(Creature *_Creature)
{
    //Select events by creature id
    std::list<EventHolder> EventList;
    uint32 ID = _Creature->GetCreatureInfo()->Entry;

    HM_NAMESPACE::hash_map<uint32, EventAI_Event>::iterator i;

    for (i = Event_Map.begin(); i != Event_Map.end(); ++i)
    {
        if ((*i).second.creature_id == ID)
        {
            //Push back event
            EventList.push_back(EventHolder((*i).second));
        }
    }

    return new Mob_EventAI (_Creature, EventList);
}


void AddSC_mob_event()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_eventai";
    newscript->GetAI = GetAI_Mob_EventAI;
    m_scripts[nrscripts++] = newscript;
}

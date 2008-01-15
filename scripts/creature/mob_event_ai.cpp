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
SD%Complete: 0
SDComment: Database Event AI
SDCategory: Creatures
EndScriptData */

#include "mob_event_ai.h"
#include "../../../../game/Player.h"

#define EVENT_UPDATE_TIME 500

struct EventHolder
{
    EventHolder(uint32 p) : EventId(p), Enabled(true){}
    EventHolder(uint32 p, uint32 z) : EventId(p), Time(z), Enabled(true){}

    uint32 EventId;
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

    std::list<EventHolder> EventList;
    bool InCombat;
    uint32 EventUpdateTime;
    uint32 EventDiff;

    void ProcessEvent(uint32 i)
    {   
        //Store random here so that all random actions match up
        uint32 rnd = rand();

        //Process actions
        for (uint32 j = 0; j < MAX_ACTIONS; j++)
            ProcessAction(i, j, rnd);
    }

    inline uint32 GetRandActionParam(uint32 Id, uint32 Action, uint32 rnd)
    {
        switch (rnd % 3)
        {
        case 0:
            return EventAI_Events[Id].action[Action].param1;
            break;

        case 1:
            return EventAI_Events[Id].action[Action].param2;
            break;

        case 2:
            return EventAI_Events[Id].action[Action].param3;
            break;
        }
        return 0;
    }

    inline Unit* GetTargetByType(uint32 Target, uint32 Id, uint32 Action)
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
            error_log( "SD2: Eventid %d, Action %d missing target type", Id, Action);
            return NULL;
        };
    }

    void ProcessAction(uint32 Id, uint32 Action, uint32 rnd)
    {
        if (Action >= MAX_ACTIONS)
            ASSERT(false);

        uint16 type = EventAI_Events[Id].action[Action].type;
        uint32 param1 = EventAI_Events[Id].action[Action].param1;
        uint32 param2 = EventAI_Events[Id].action[Action].param2;
        uint32 param3 = EventAI_Events[Id].action[Action].param3;

        switch (type)
        {
        case ACTION_T_SAY:
            DoSay(GetLocalizedText(param1), LANG_UNIVERSAL, m_creature->getVictim());
            break;

        case ACTION_T_YELL:
            DoYell(GetLocalizedText(param1), LANG_UNIVERSAL, m_creature->getVictim());
            break;

        case ACTION_T_TEXTEMOTE:
            DoTextEmote(GetLocalizedText(param1), m_creature->getVictim());
            break;

        case ACTION_T_SOUND:
            DoPlaySoundToSet(m_creature, param1);
            break;

        case ACTION_T_EMOTE:
            m_creature->HandleEmoteCommand(param1);
            break;

        case ACTION_T_RANDOM_SAY:
            {
                uint32 temp = GetRandActionParam(Id, Action, rnd);

                if (temp != 0xffffffff)
                    DoSay(GetLocalizedText(temp), LANG_UNIVERSAL, m_creature->getVictim());
            }
            break;

        case ACTION_T_RANDOM_YELL:
            {
                uint32 temp = GetRandActionParam(Id, Action, rnd);

                if (temp != 0xffffffff)
                    DoYell(GetLocalizedText(temp), LANG_UNIVERSAL, m_creature->getVictim());
            }
            break;

        case ACTION_T_RANDOM_TEXTEMOTE:
            {
                uint32 temp = GetRandActionParam(Id, Action, rnd);

                if (temp != 0xffffffff)
                    DoTextEmote(GetLocalizedText(temp), m_creature->getVictim());
            }
            break;

        case ACTION_T_RANDOM_SOUND:
            {
                uint32 temp = GetRandActionParam(Id, Action, rnd);

                if (temp != 0xffffffff)
                    DoPlaySoundToSet(m_creature, temp);
            }
            break;

        case ACTION_T_RANDOM_EMOTE:
            {
                uint32 temp = GetRandActionParam(Id, Action, rnd);

                if (temp != 0xffffffff)
                    m_creature->HandleEmoteCommand(temp);
            }
            break;

        case ACTION_T_CAST:
            {
                Unit* target = GetTargetByType(param2, Id, Action);

                if (param3 || !m_creature->IsNonMeleeSpellCasted(false))
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->CastSpell(target, param1, false);
                }
            }
            break;

        case ACTION_T_SUMMON:
            {
                Unit* target = GetTargetByType(param2, Id, Action);

                //Duration
                Creature* pCreature = NULL;
                if (param3)
                    pCreature = DoSpawnCreature(param1, 0,0,0,0,TEMPSUMMON_TIMED_DESPAWN, param3);
                else pCreature = DoSpawnCreature(param1, 0,0,0,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 0);

                if (!pCreature)
                    error_log( "SD2: Eventid %d, Action %d failed to spawn creature %d", Id, Action, param3);

                if (param2 != TARGET_T_SELF)
                    pCreature->Attack(target);
            }
            break;

        case ACTION_T_THREAT_SINGLE_PCT:
            {
                Unit* target = GetTargetByType(param2, Id, Action);

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
                Unit* target = GetTargetByType(param2, Id, Action);

                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)target)->CompleteQuest(param1);
            }
            break;

        case ACTION_T_QUEST_CASTCREATUREGO:
            {
                Unit* target = GetTargetByType(param3, Id, Action);

                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    ((Player*)target)->CastedCreatureOrGO(param1, m_creature->GetGUID(), param2);
            }
            break;

        case ACTION_T_SET_UNIT_FIELD:
            {
                Unit* target = GetTargetByType(param3, Id, Action);

                if (target)
                    target->SetUInt32Value(param1, param2);
            }
            break;

        case ACTION_T_SET_UNIT_FLAG:
            {
                Unit* target = GetTargetByType(param2, Id, Action);

                if (target)
                    target->SetFlag(UNIT_FIELD_FLAGS, param1);
            }
            break;

        case ACTION_T_REMOVE_UNIT_FLAG:
            {
                Unit* target = GetTargetByType(param2, Id, Action);

                if (target)
                    target->RemoveFlag(UNIT_FIELD_FLAGS, param1);
            }
            break;

        default:
            error_log( "SD2: Eventid %d has invalid action type for Action %d", Id, Action);
            break;
        };
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        EventUpdateTime = EVENT_UPDATE_TIME;
        EventDiff = 0;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //Handle Evade events and reset all events to enabled
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            if ((*i).EventId > MAX_EVENTS)
                continue;

            switch (EventAI_Events[(*i).EventId].event_type)
            {
                //Evade
            case EVENT_T_EVADE:
                ProcessEvent((*i).EventId);
                break;

                //Reset all out of combat timers
            case EVENT_T_TIMER_OOC_REPEAT:
            case EVENT_T_TIMER_OOC_SINGLE:
                (*i).Time = EventAI_Events[(*i).EventId].event_param1;
                (*i).Enabled = true;
                break;
            }
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                //Check for on combat start events
                for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
                {
                    if ((*i).EventId > MAX_EVENTS)
                        continue;

                    switch (EventAI_Events[(*i).EventId].event_type)
                    {
                    case EVENT_T_AGGRO:
                        ProcessEvent((*i).EventId);
                        break;

                        //Reset all in combat timers
                    case EVENT_T_TIMER_REPEAT:
                    case EVENT_T_TIMER_SINGLE:
                        (*i).Time = EventAI_Events[(*i).EventId].event_param1;
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
            }
        }
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void DamageDeal(Unit *done_to, uint32 &damage)
    {
        if (done_to->GetTypeId() != TYPEID_PLAYER)
            return;

        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            if (!(*i).Enabled || (*i).EventId > MAX_EVENTS || (*i).Time)
                continue;

            switch (EventAI_Events[(*i).EventId].event_type)
            {
                //Kill
            case EVENT_T_KILL:
                {
                    if (done_to->GetHealth() <= damage)
                    {
                        ProcessEvent((*i).EventId);

                        //Prevent repeat for param1 time if set
                        if (EventAI_Events[(*i).EventId].event_param1)
                            (*i).Time = EventAI_Events[(*i).EventId].event_param1;
                    }
                }
                break;
            }
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            if (!(*i).Enabled || (*i).EventId > MAX_EVENTS || (*i).Time)
                continue;

            switch (EventAI_Events[(*i).EventId].event_type)
            {
                //Death
            case EVENT_T_DEATH:
                {
                    if (m_creature->GetHealth() <= damage)
                        ProcessEvent((*i).EventId);
                }
                break;
            }
        }
    }

    virtual void SpellHit(Unit* pUnit, const SpellEntry* pSpell)
    {
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            if (!(*i).Enabled || (*i).EventId > MAX_EVENTS || (*i).Time)
                continue;

            switch (EventAI_Events[(*i).EventId].event_type)
            {
                //Spell hit
            case EVENT_T_SPELLHIT:
                {
                    //If spell id matches (or no spell id)
                    if (!EventAI_Events[(*i).EventId].event_param1 || pSpell->Id == EventAI_Events[(*i).EventId].event_param1)
                    {
                        ProcessEvent((*i).EventId);

                        //Prevent repeat for param2 time, or disable if param2 not set
                        if (EventAI_Events[(*i).EventId].event_param2)
                            (*i).Time = EventAI_Events[(*i).EventId].event_param2;
                        else
                            (*i).Enabled = false;
                    }
                }
                break;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Check if we are in combat (also updates calls threat update code)
        bool Combat = InCombat ? (m_creature->SelectHostilTarget() && m_creature->getVictim()) : false;

        //Events are only updated once every EVENT_UPDATE_TIME ms to prevent lag with large amount of events
        if (EventUpdateTime < diff)
        {
            EventDiff += diff;

            //Check for time based events
            for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
            {
                if (!(*i).Enabled || (*i).EventId > MAX_EVENTS)
                    continue;

                //Decrement Timers
                if ((*i).Time)
                    if ((*i).Time > EventDiff)
                    {
                        //Skip events that have time remaining
                        (*i).Time -= EventDiff;
                        continue;
                    }
                    else (*i).Time = 0; 

                    switch (EventAI_Events[(*i).EventId].event_type)
                    {
                        //Timer event
                    case EVENT_T_TIMER_REPEAT:
                    case EVENT_T_TIMER_SINGLE:
                        if (Combat)
                        {
                            ProcessEvent((*i).EventId);

                            //Reset or remove timer
                            if (EventAI_Events[(*i).EventId].event_type == EVENT_T_TIMER_SINGLE)
                                (*i).Enabled = false;
                            else (*i).Time = EventAI_Events[(*i).EventId].event_param1;
                        }
                        break;

                        //Out of Combat Timer event
                    case EVENT_T_TIMER_OOC_REPEAT:
                    case EVENT_T_TIMER_OOC_SINGLE:
                        if (!Combat)
                        {
                            ProcessEvent((*i).EventId);

                            //Reset or remove timer
                            if (EventAI_Events[(*i).EventId].event_type == EVENT_T_TIMER_OOC_SINGLE)
                                (*i).Enabled = false;
                            else (*i).Time = EventAI_Events[(*i).EventId].event_param1;

                        }
                        break;

                        //Mana Event
                    case EVENT_T_MANA:
                        if (Combat)
                        {
                            //Don't process mana events if creature has no max mana to prevent crash
                            if (m_creature->GetMaxPower(POWER_MANA))
                            {

                                //If mana is below target mana%
                                if ((m_creature->GetPower(POWER_MANA)*100) / m_creature->GetMaxPower(POWER_MANA) < EventAI_Events[(*i).EventId].event_param1)
                                {
                                    ProcessEvent((*i).EventId);

                                    //Prevent repeat for param2 time, or disable if param2 not set
                                    if (EventAI_Events[(*i).EventId].event_param2)
                                        (*i).Time = EventAI_Events[(*i).EventId].event_param2;
                                    else
                                        (*i).Enabled = false;
                                }
                            }
                        }
                        break;

                        //HP Event
                    case EVENT_T_HP:
                        if (Combat)
                        {
                            //Don't process health events if creature has no max health to prevent crash
                            if (m_creature->GetMaxHealth())
                            {

                                //If mana is below target mana%
                                if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < EventAI_Events[(*i).EventId].event_param1)
                                {
                                    ProcessEvent((*i).EventId);

                                    //Prevent repeat for param2 time, or disable if param2 not set
                                    if (EventAI_Events[(*i).EventId].event_param2)
                                        (*i).Time = EventAI_Events[(*i).EventId].event_param2;
                                    else
                                        (*i).Enabled = false;
                                }
                            }
                        }
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
        if (Combat)
            DoMeleeAttackIfReady();

    }
};
CreatureAI* GetAI_Mob_EventAI(Creature *_Creature)
{
    //Select events by creature id
    std::list<EventHolder> EventList;
    uint32 ID = _Creature->GetCreatureInfo()->Entry;

    for (uint32 i = 0; i < MAX_EVENTS; i++)
    {
        if (EventAI_Events[i].creature_id == ID)
        {
            //Timer events need to have their timer set at creation
            if (EventAI_Events[i].event_type == EVENT_T_TIMER_REPEAT ||
                EventAI_Events[i].event_type == EVENT_T_TIMER_SINGLE ||
                EventAI_Events[i].event_type == EVENT_T_TIMER_OOC_REPEAT ||
                EventAI_Events[i].event_type == EVENT_T_TIMER_OOC_SINGLE)
                EventList.push_back(EventHolder(i, EventAI_Events[i].event_param1));
            else EventList.push_back(EventHolder(i));

            if (EventAI_Events[i].event_type >= EVENT_T_END)
            {
                error_log( "SD2: Eventid %d has Not Yet Implemented Event Type", i);
            }
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
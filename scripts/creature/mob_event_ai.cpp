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

#define EVENT_UPDATE_TIME 500

enum Target
{
    TARGET_T_SELF = 0,                  //Self cast
    TARGET_T_HOSTILE,                   //Our current target (ie: highest aggro)
    TARGET_T_HOSTILE_SECOND_AGGRO,      //Second highest aggro (generaly used for cleaves and some special attacks)
    TARGET_T_HOSTILE_LAST_AGGRO,        //Dead last on aggro (no idea what this could be used for)
    TARGET_T_HOSTILE_RANDOM,            //Just any random target on our threat list
    TARGET_T_HOSTILE_RANDOM_NOT_TOP     //Any random target except top threat
    //CAST_FRIENDLY_RANDOM,             //NOT YET IMPLEMENTED
};

enum Event_Types
{
    EVENT_T_NONE = 0,               //No params
    EVENT_T_TIMER_REPEAT,           //Param1 = Time
    EVENT_T_TIMER_SINGLE,           //Param1 = Time
    EVENT_T_HP_SINGLE,              //Param1 = HP%
    EVENT_T_MANA_SINGLE,            //Param1 = Mana%
    EVENT_T_AGGRO,                  //No Params
    EVENT_T_KILL,                   //No Params
    EVENT_T_DEATH,                  //No Params
};

enum Action_Types
{
    ACTION_T_NONE = 0,              //No Params
    ACTION_T_SAY,                   //Param1 = TextId
    ACTION_T_YELL,                  //Param1 = TextId
    ACTION_T_EMOTE,                 //Param1 = TextId
    ACTION_T_SOUND,                 //Param1 = SoundID
    ACTION_T_CAST,                  //Param1 = SpellId, Param2 = Target, Param3 = (false = don't inturrupt prev cast, true = intrurrupt)
    ACTION_T_SUMMON,                //Param1 = CreatureID, Param2 = Target, Param3 = Duration in ms
};

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
        sLog.outString("SD2 DEBUG: EventAI starting, containing %d events", pEventList.size());
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
        //Process any additional event condition

        //Process actions
        for (uint32 j = 0; j < MAX_ACTIONS; j++)
            ProcessAction(i, j);
    }

    void ProcessAction(uint32 Id, uint32 Action)
    {
        sLog.outError( "SD2 DEBUG: Eventid %d, Action %d in process", Id, Action);

        if (Action >= MAX_ACTIONS)
            ASSERT(false);

        uint16 type = EventAI_Events[Id].action[Action].type;
        uint32 param1 = EventAI_Events[Id].action[Action].param1;
        uint32 param2 = EventAI_Events[Id].action[Action].param2;
        uint32 param3 = EventAI_Events[Id].action[Action].param3;

        switch (type)
        {
            case ACTION_T_NONE:
            break;

            case ACTION_T_SAY:
                DoSay(GetLocalizedText(param1), LANG_UNIVERSAL, m_creature->getVictim());
            break;

            case ACTION_T_YELL:
                DoYell(GetLocalizedText(param1), LANG_UNIVERSAL, m_creature->getVictim());
            break;

            case ACTION_T_EMOTE:
                DoTextEmote(GetLocalizedText(param1), m_creature->getVictim());
            break;

            case ACTION_T_SOUND:
                DoPlaySoundToSet(m_creature, param1);
            break;

            case ACTION_T_CAST:
            {
                Unit* target = NULL;
                switch (param2)
                {
                case TARGET_T_SELF:
                    target = m_creature;
                    break;
                case TARGET_T_HOSTILE:
                    target = m_creature->getVictim();
                    break;
                case TARGET_T_HOSTILE_SECOND_AGGRO:
                    target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
                    break;
                case TARGET_T_HOSTILE_LAST_AGGRO:
                    target = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
                    break;
                case TARGET_T_HOSTILE_RANDOM:
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    break;
                case TARGET_T_HOSTILE_RANDOM_NOT_TOP:
                    target = SelectUnit(SELECT_TARGET_RANDOM,1);
                    break;
                default:
                    sLog.outError( "SD2: Eventid %d, Action %d missing target type", Id, Action);
                    break;

                };

                if (param3 || !m_creature->IsNonMeleeSpellCasted(false))
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->CastSpell(target, param1, false);
                }
            }
            break;

            case ACTION_T_SUMMON:
            {
                Unit* target = NULL;
                switch (param2)
                {
                case TARGET_T_SELF:
                    break;
                case TARGET_T_HOSTILE:
                    target = m_creature->getVictim();
                    break;
                case TARGET_T_HOSTILE_SECOND_AGGRO:
                    target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
                    break;
                case TARGET_T_HOSTILE_LAST_AGGRO:
                    target = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
                    break;
                case TARGET_T_HOSTILE_RANDOM:
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    break;
                case TARGET_T_HOSTILE_RANDOM_NOT_TOP:
                    target = SelectUnit(SELECT_TARGET_RANDOM,1);
                    break;
                default:
                    sLog.outError( "SD2: Eventid %d, Action %d missing target type", Id, Action);
                    break;

                };

                //Duration
                Creature* pCreature = NULL;
                if (param3)
                    pCreature = DoSpawnCreature(param1, 0,0,0,0,TEMPSUMMON_TIMED_DESPAWN, param3);
                else pCreature = DoSpawnCreature(param1, 0,0,0,0,TEMPSUMMON_CORPSE_DESPAWN, 0);

                if (!pCreature)
                    sLog.outError( "SD2: Eventid %d, Action %d failed to spawn creature %d", Id, Action, param3);

                if (param2 != TARGET_T_SELF)
                    pCreature->Attack(target);
            }
            break;

            default:
                sLog.outError( "SD2: Eventid %d missing action type for action %d", Id, Action);
                break;
        };
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        EventUpdateTime = EVENT_UPDATE_TIME;
        EventDiff = 0;

        //Re-enable all events
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
            (*i).Enabled = true;


        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
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

                    if (EventAI_Events[(*i).EventId].event_type == EVENT_T_AGGRO)
                        ProcessEvent((*i).EventId);
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
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            if (!(*i).Enabled || (*i).EventId > MAX_EVENTS)
                continue;

            switch (EventAI_Events[(*i).EventId].event_type)
            {
                //Kill
                case EVENT_T_KILL:
                {
                    if (done_to->GetHealth() <= damage)
                        ProcessEvent((*i).EventId);
                }
                break;
            }
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
        {
            if (!(*i).Enabled || (*i).EventId > MAX_EVENTS)
                continue;

            switch (EventAI_Events[(*i).EventId].event_type)
            {
                //HP Single Event
                case EVENT_T_HP_SINGLE:
                {
                    //If hp is below target hp%
                    if (((m_creature->GetHealth()-damage)*100) / m_creature->GetMaxHealth() < EventAI_Events[(*i).EventId].event_param1)
                    {
                        ProcessEvent((*i).EventId);

                        (*i).Enabled = false;
                    }
                }
                break;

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

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Events are only updated once every EVENT_UPDATE_TIME ms to prevent lag with large amount of events
        if (EventUpdateTime < diff)
        {
            EventDiff += diff;

            //Check for time based events
            for (std::list<EventHolder>::iterator i = EventList.begin(); i != EventList.end(); ++i)
            {
                if (!(*i).Enabled || (*i).EventId > MAX_EVENTS)
                    continue;

                switch (EventAI_Events[(*i).EventId].event_type)
                {
                    //Timer event
                    case EVENT_T_TIMER_REPEAT:
                    case EVENT_T_TIMER_SINGLE:
                    {
                        //If time < diff start event and reset timer or remove from event list
                        if ((*i).Time < EventDiff)
                        {
                            ProcessEvent((*i).EventId);

                            //Reset or remove timer
                            if (EventAI_Events[(*i).EventId].event_type == EVENT_T_TIMER_SINGLE)
                                (*i).Enabled = false;
                            else (*i).Time = EventAI_Events[(*i).EventId].event_param1;

                        }else (*i).Time -= EventDiff;
                    }
                    break;

                    //Mana Single Event
                    case EVENT_T_MANA_SINGLE:
                    {
                        //Don't process mana events if creature has no max mana
                        if (m_creature->GetMaxPower(POWER_MANA))
                        {

                            //If mana is below target mana%
                            if ((m_creature->GetPower(POWER_MANA)*100) / m_creature->GetMaxPower(POWER_MANA) < EventAI_Events[(*i).EventId].event_param1)
                            {
                                ProcessEvent((*i).EventId);
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

        //Melee
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
                EventAI_Events[i].event_type == EVENT_T_TIMER_SINGLE)
                EventList.push_back(EventHolder(i, EventAI_Events[i].event_param1));
            else EventList.push_back(EventHolder(i));

            if (EventAI_Events[i].event_type > EVENT_T_DEATH)
            {
                sLog.outError( "SD2: Eventid %d has Not Yet Implemented Event Type", i);
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
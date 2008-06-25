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
SDName: npcs_special
SD%Complete: 100
SDComment: To be used for special NPCs that are located globally. Support for quest 3861 (Cluck!), 6622 and 6624 (Triage)
SDCategory: NPCs
EndScriptData
*/

#include "sc_creature.h"
#include "sc_gossip.h"

/*########
# npc_chicken_cluck
#########*/

#define QUEST_CLUCK         3861
#define ITEM_CHICKEN_FEED   11109
#define CLUCK_TEXT1         "looks up at you quizzically. Maybe you should inspect it?"
#define CLUCK_TEXT2         "starts pecking at the feed"
#define FACTION_FRIENDLY    35
#define FACTION_CHICKEN     31

struct MANGOS_DLL_DECL npc_chicken_cluckAI : public ScriptedAI
{
    npc_chicken_cluckAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 ResetFlagTimer;
    
    void Reset() 
    {
        ResetFlagTimer = 120000;

        m_creature->setFaction(FACTION_CHICKEN);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
    }
    
    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        // Reset flags after a certain time has passed so that the next player has to start the 'event' again
        if(m_creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
        {
            if(ResetFlagTimer < diff)
                EnterEvadeMode();
            else ResetFlagTimer -= diff;
        }

        if(m_creature->SelectHostilTarget() && m_creature->getVictim())
            DoMeleeAttackIfReady();
    }
}; 

CreatureAI* GetAI_npc_chicken_cluck(Creature *_Creature)
{
    return new npc_chicken_cluckAI(_Creature);
}

bool ReceiveEmote_npc_chicken_cluck( Player *player, Creature *_Creature, uint32 emote )
{
    if(player->GetTeam() == ALLIANCE)
    {
        if(player->GetQuestStatus(QUEST_CLUCK) == QUEST_STATUS_NONE)
        {
            if(emote == EMOTE_ONESHOT_CHICKEN)
            {
                if(rand()%30 == 0)
                {
                    _Creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _Creature->MonsterTextEmote(CLUCK_TEXT1, 0);
                }
            }
        }
        else if(player->HasItemCount(ITEM_CHICKEN_FEED, 1))
        {
            if(emote == EMOTE_ONESHOT_CHEER)
            {
                _Creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                _Creature->MonsterTextEmote(CLUCK_TEXT2, 0);
            }
        }
    }
    return true;
}

bool QuestAccept_npc_chicken_cluck(Player *player, Creature *_Creature, const Quest *_Quest )
{
    if(_Quest->GetQuestId() == QUEST_CLUCK)
        ((npc_chicken_cluckAI*)_Creature->AI())->Reset();

    return true;
}

bool QuestComplete_npc_chicken_cluck(Player *player, Creature *_Creature, const Quest *_Quest)
{
    if(_Quest->GetQuestId() == QUEST_CLUCK)
        ((npc_chicken_cluckAI*)_Creature->AI())->Reset();

    return true;
}

/*######
## common defines first aid
######*/

#define SAY_DOC1 "I'm saved! Thank you, doctor!"
#define SAY_DOC2 "HOORAY! I AM SAVED!"
#define SAY_DOC3 "Sweet, sweet embrace... take me..."

/*######
## Triage quest
######*/

struct Location
{
    float x, y, z, o;
};

static Location AllianceCoords[]=
{
    {-3757.38, -4533.05, 14.16, 3.62}, // Top-far-right bunk as seen from entrance
    {-3754.36, -4539.13, 14.16, 5.13}, // Top-far-left bunk
    {-3749.54, -4540.25, 14.28, 3.34}, // Far-right bunk
    {-3742.10, -4536.85, 14.28, 3.64}, // Right bunk near entrance
    {-3755.89, -4529.07, 14.05, 0.57}, // Far-left bunk
    {-3749.51, -4527.08, 14.07, 5.26}, // Mid-left bunk
    {-3746.37, -4525.35, 14.16, 5.22}, // Left bunk near entrance
};

#define ALLIANCE_COORDS         7

//alliance run to where
#define A_RUNTOX -3742.96
#define A_RUNTOY -4531.52
#define A_RUNTOZ 11.91

#define DOCTOR_ALLIANCE     12939

static Location HordeCoords[]=
{
    {-1022.88, -3506.62, 63.69, 2.12}, // Nearest the Doctor
    {-1027.50, -3501.60, 63.69, 5.84}, // Near the big hole in the corner
    {-1025.68, -3497.50, 63.69, 0.09}, // Next bed over from the big hole
    {-1009.78, -3494.56, 63.39, 3.23}, // Bed near the supplies vendor
};

#define HORDE_COORDS        4

//horde run to where
#define H_RUNTOX -1016.44
#define H_RUNTOY -3508.48
#define H_RUNTOZ 62.96

#define DOCTOR_HORDE    12920

const uint32 AllianceSoldierId[3] = 
{
    12938, // 12938 Injured Alliance Soldier
    12936, // 12936 Badly injured Alliance Soldier
    12937  // 12937 Critically injured Alliance Soldier
};

const uint32 HordeSoldierId[3] = 
{
    12923, //12923 Injured Soldier
    12924, //12924 Badly injured Soldier
    12925  //12925 Critically injured Soldier
};

/*######
## npc_doctor (handles both Gustaf Vanhowzen and Gregory Victor)
######*/

struct MANGOS_DLL_DECL npc_doctorAI : public ScriptedAI
{
    uint64 Playerguid;

    uint32 SummonPatient_Timer;
    uint32 SummonPatientCount;
    uint32 PatientDiedCount;
    uint32 PatientSavedCount;

    bool Event;

    std::list<uint64> Patients;
    std::vector<Location*> Coordinates;

    npc_doctorAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset(){}

    void BeginEvent(Player* player);
    void PatientDied(Location* Point);
    void PatientSaved(Creature* soldier, Player* player, Location* Point);
    void UpdateAI(const uint32 diff);

    void Aggro(Unit* who){}
};

/*#####
## npc_injured_patient (handles all the patients, no matter Horde or Alliance)
#####*/

struct MANGOS_DLL_DECL npc_injured_patientAI : public ScriptedAI
{
    npc_injured_patientAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint64 Doctorguid;

    Location* Coord;

    void Reset()
    {
        Doctorguid = 0;

        Coord = NULL;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);     //no select
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);             //no regen health
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_DEAD);      //to make them lay with face down

        uint32 mobId = m_creature->GetEntry();

        switch (mobId)
        {
            case 12923:
            case 12938:                                                        //Injured Soldier
                m_creature->SetHealth(uint32(m_creature->GetMaxHealth()*.75)); //lower max health
                break;
            case 12924:
            case 12936:                                                         //Badly injured Soldier
                m_creature->SetHealth(uint32(m_creature->GetMaxHealth()*.50));  //lower max health
                break;
            case 12925:
            case 12937:                                                         //Critically injured Soldier
                m_creature->SetHealth(uint32(m_creature->GetMaxHealth()*.25));  //lower max health
                break;
        }
    }

    void Aggro(Unit* who){}

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER && m_creature->isAlive() && spell->Id == 20804)
        {
            if( (((Player*)caster)->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (((Player*)caster)->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE))
            {
                if(Doctorguid)
                {
                    Creature* Doctor = ((Creature*)Unit::GetUnit((*m_creature), Doctorguid));
                    if(Doctor)
                        ((npc_doctorAI*)Doctor->AI())->PatientSaved(m_creature, ((Player*)caster), Coord);
                }
            }

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);    //make not selectable
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);      //regen health
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_NONE);  //stand up
            DoSay(SAY_DOC1,LANG_UNIVERSAL,NULL);

            uint32 mobId = m_creature->GetEntry();
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
            switch (mobId)
            {
                case 12923:
                case 12924:
                case 12925:
                    m_creature->GetMotionMaster()->MovePoint(0, H_RUNTOX, H_RUNTOY, H_RUNTOZ);
                    break;
                case 12936:
                case 12937:
                case 12938:
                    m_creature->GetMotionMaster()->MovePoint(0, A_RUNTOX, A_RUNTOY, A_RUNTOZ);
                    break;
            }
        }
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_creature->isAlive() && m_creature->GetHealth() > 6)
        {
            m_creature->SetHealth(uint32(m_creature->GetHealth()-5) );          //lower HP on every world tick makes it a useful counter, not officlone though
        }

        if (m_creature->isAlive() && m_creature->GetHealth() <= 6)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->setDeathState(JUST_DIED);
            m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, 32);

            if(Doctorguid)
            {
                Creature* Doctor = ((Creature*)Unit::GetUnit((*m_creature), Doctorguid));
                if(Doctor)
                    ((npc_doctorAI*)Doctor->AI())->PatientDied(Coord);
            }
        }
    }
};

CreatureAI* GetAI_npc_injured_patient(Creature *_Creature)
{
    return new npc_injured_patientAI (_Creature);
}

/*######
## npc_doctor (continue)
######*/
void npc_doctorAI::BeginEvent(Player* player)
{
    Playerguid = player->GetGUID();

    SummonPatient_Timer = 10000;
    SummonPatientCount = 0;
    PatientDiedCount = 0;
    PatientSavedCount = 0;

    switch(m_creature->GetEntry())
    {
        case DOCTOR_ALLIANCE:
            for(uint8 i = 0; i < ALLIANCE_COORDS; ++i)
                Coordinates.push_back(&AllianceCoords[i]);
            break;

        case DOCTOR_HORDE:
            for(uint8 i = 0; i < HORDE_COORDS; ++i)
                Coordinates.push_back(&HordeCoords[i]);
            break;
    }

    Event = true;

    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void npc_doctorAI::PatientDied(Location* Point)
{
    Player* player = ((Player*)Unit::GetUnit((*m_creature), Playerguid));
    if(player && ((player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)))
    {
        PatientDiedCount++;
        if (PatientDiedCount > 5 && Event)
        {
            if(player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE)
                player->FailQuest(6624);
            else if(player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)
                player->FailQuest(6622);

            Event = false;
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        Coordinates.push_back(Point);
    }
}

void npc_doctorAI::PatientSaved(Creature* soldier, Player* player, Location* Point)
{
    if(player && Playerguid == player->GetGUID())
    {
        if((player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE))
        {
            PatientSavedCount++;
            if(PatientSavedCount == 15)
            {
                if(!Patients.empty())
                {
                    std::list<uint64>::iterator itr;
                    for(itr = Patients.begin(); itr != Patients.end(); ++itr)
                    {
                        Creature* Patient = ((Creature*)Unit::GetUnit((*m_creature), *itr));
                        if(Patient)
                            Patient->setDeathState(JUST_DIED); // Unsummon
                    }
                }

                if(player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE)
                    player->AreaExploredOrEventHappens(6624);
                else if(player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)
                    player->AreaExploredOrEventHappens(6622);

                Event = false;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            Coordinates.push_back(Point);
        }
    }
}

void npc_doctorAI::UpdateAI(const uint32 diff)
{
    if(Event && SummonPatientCount >= 20)
    {
        Event = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    if(Event)
        if(SummonPatient_Timer < diff)
        {
            Creature* Patient = NULL;
            Location* Point = NULL;

            if(Coordinates.empty())
                return;

            std::vector<Location*>::iterator itr = Coordinates.begin()+rand()%Coordinates.size();
            uint32 patientEntry = 0;

            switch(m_creature->GetEntry())
            {
                case DOCTOR_ALLIANCE: patientEntry = AllianceSoldierId[rand()%3]; break;
                case DOCTOR_HORDE:    patientEntry = HordeSoldierId[rand()%3]; break;
                default: 
                    error_log("SD2: Invalid entry for Triage doctor. Please check your database");
                    return;
            }

            Point = *itr;

            Patient = m_creature->SummonCreature(patientEntry, Point->x, Point->y, Point->z, Point->o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
            
            if(Patient)
            {
                Patients.push_back(Patient->GetGUID());
                ((npc_injured_patientAI*)Patient->AI())->Doctorguid = m_creature->GetGUID();
                if(Point)
                    ((npc_injured_patientAI*)Patient->AI())->Coord = Point;
                Coordinates.erase(itr);
            }
            SummonPatient_Timer = 10000;
            SummonPatientCount++;
        }else SummonPatient_Timer -= diff;
}

bool QuestAccept_npc_doctor(Player *player, Creature *creature, Quest const *quest )
{
    if((quest->GetQuestId() == 6624) || (quest->GetQuestId() == 6622))
        ((npc_doctorAI*)creature->AI())->BeginEvent(player);

    return true;
}
CreatureAI* GetAI_npc_doctor(Creature *_Creature)
{
    return new npc_doctorAI (_Creature);
}

void AddSC_npcs_special()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_chicken_cluck";
    newscript->GetAI = GetAI_npc_chicken_cluck;
    newscript->pReceiveEmote =  &ReceiveEmote_npc_chicken_cluck;
    newscript->pQuestAccept =   &QuestAccept_npc_chicken_cluck;
    newscript->pQuestComplete = &QuestComplete_npc_chicken_cluck;    
    m_scripts[nrscripts++] = newscript;  

    newscript = new Script;
    newscript->Name="npc_injured_patient";
    newscript->GetAI = GetAI_npc_injured_patient;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_doctor";
    newscript->GetAI = GetAI_npc_doctor;
    newscript->pQuestAccept = &QuestAccept_npc_doctor;
    m_scripts[nrscripts++] = newscript;
}

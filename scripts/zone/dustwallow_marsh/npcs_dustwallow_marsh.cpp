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
SDName: Npcs_Dustwallow_Marsh
SD%Complete: 95
SDComment: Quest support: 11180, 558, 11126, 6622, 6624 (Includes Triage for Horde in Arathi Highlands). Vendor Nat Pagle
SDCategory: Dustwallow Marsh
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## npc_deserter_agitator
######*/

struct MANGOS_DLL_DECL npc_deserter_agitatorAI : public ScriptedAI
{
    npc_deserter_agitatorAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();

        m_creature->setFaction(894);
    }
};

CreatureAI* GetAI_npc_deserter_agitator(Creature *_Creature)
{
    return new npc_deserter_agitatorAI (_Creature);
}

bool GossipHello_npc_deserter_agitator(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(11126) == QUEST_STATUS_INCOMPLETE)
    {
        _Creature->setFaction(1883);
        player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    }
    else
        player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}

/*######
## npc_lady_jaina_proudmoore
######*/

#define GOSSIP_ITEM_JAINA "I know this is rather silly but i have a young ward who is a bit shy and would like your autograph."

bool GossipHello_npc_lady_jaina_proudmoore(Player *player, Creature *_Creature)
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if( player->GetQuestStatus(558) == QUEST_STATUS_INCOMPLETE )
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM_JAINA, GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO );

    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}
bool GossipSelect_npc_lady_jaina_proudmoore(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if( action == GOSSIP_SENDER_INFO )
    {
        player->SEND_GOSSIP_MENU( 7012, _Creature->GetGUID() );
        player->CastSpell( player, 23122, false);
    }
    return true;
}

/*######
## npc_nat_pagle
######*/

bool GossipHello_npc_nat_pagle(Player *player, Creature *_Creature)
{
    if(_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if(_Creature->isVendor() && player->GetQuestRewardStatus(8227))
    {
        player->ADD_GOSSIP_ITEM( 0, "I'd like to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        player->SEND_GOSSIP_MENU( 7640, _Creature->GetGUID() );
    }
    else
        player->SEND_GOSSIP_MENU( 7638, _Creature->GetGUID() );

    return true;
}

bool GossipSelect_npc_nat_pagle(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_TRADE)
        player->SEND_VENDORLIST( _Creature->GetGUID() );

    return true;
}

/*######
## npc_restless_apparition
######*/

bool GossipHello_npc_restless_apparition(Player *player, Creature *_Creature)
{
    player->PlayerTalkClass->SendGossipMenu(_Creature->GetNpcTextId(), _Creature->GetGUID());

    player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
    _Creature->SetInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

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
    {-3742.2626, -4536.9624, 14.2759, 3.6178}, // Far Left Bed, Behind
    {-3750.0925, -4540.1699, 14.2712, 3.5404}, // Far Left Bed
    {-3754.2653, -4539.0634, 14.1620, 5.1827}, // Left Bed
    {-3757.5134, -4532.7343, 14.1667, 5.1497}, // Right Bed
    {-3755.9040, -4529.0385, 14.0517, 0.4200}, // Far Right Bed
    {-3749.5649, -4526.9609, 14.0764, 5.1297}  // Far Right Bed, Behind
};

//alliance run to where
#define A_RUNTOX -3742.96
#define A_RUNTOY -4531.52
#define A_RUNTOZ 11.91

static Location HordeCoords[]=
{
    {-1013.75, -3492.59, 62.62, 4.34}, // Left, Behind
    {-1017.72, -3490.92, 62.62, 4.34}, // Right, Behind
    {-1015.77, -3497.15, 62.82, 4.34}, // Left, Mid
    {-1019.51, -3495.49, 62.82, 4.34}, // Right, Mid
    {-1017.25, -3500.85, 62.98, 4.34}, // Left, front
    {-1020.95, -3499.21, 62.98, 4.34}  // Right, Front
};

//horde run to where
#define H_RUNTOX -1016.44
#define H_RUNTOY -3508.48
#define H_RUNTOZ 62.96

const uint32 allianceSoldierId[3] = 
{
    12938, // 12938 Injured Alliance Soldier
    12936, // 12936 Badly injured Alliance Soldier
    12937  // 12937 Critically injured Alliance Soldier
};

const uint32 hordeSoldierId[3] = 
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

    npc_doctorAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset(){}

    void BeginEvent(Player* player);
    void PatientDied(Unit* soldier);
    void PatientSaved(Unit* soldier, Player* player);
    void UpdateAI(const uint32 diff);
};

/*#####
## npc_injured_patient (handles all the patients, no matter Horde or Alliance)
#####*/

struct MANGOS_DLL_DECL npc_injured_patientAI : public ScriptedAI
{
    npc_injured_patientAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint64 Doctorguid;

    void Reset()
    {
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();

        Doctorguid = 0;

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
                        ((npc_doctorAI*)Doctor->AI())->PatientSaved(m_creature, ((Player*)caster));
                }
            }

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);    //make not selectable
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);      //regen health
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_NONE);  //stand up
            DoSay(SAY_DOC1,LANG_UNIVERSAL,NULL);

            uint32 mobId = m_creature->GetEntry();
            switch (mobId)
            {
                case 12923:
                case 12924:
                case 12925:
                    m_creature->SendMoveToPacket(H_RUNTOX, H_RUNTOY, H_RUNTOZ, true, 2000 );
                    break;
                case 12936:
                case 12937:
                case 12938:
                    m_creature->SendMoveToPacket(A_RUNTOX, A_RUNTOY, A_RUNTOZ, true, 2000 );
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
                    ((npc_doctorAI*)Doctor->AI())->PatientDied(m_creature);
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

    Event = true;

    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void npc_doctorAI::PatientDied(Unit* soldier)
{
    Player* player = ((Player*)Unit::GetUnit((*m_creature), Playerguid));
    if(player && ((player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)))
    {
        PatientDiedCount++;
        if (PatientDiedCount >= 5 && Event)
        {
            if(player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE)
                player->FailQuest(6624);
            else if(player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)
                player->FailQuest(6622);

            Event = false;
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
    }
}

void npc_doctorAI::PatientSaved(Unit* soldier, Player* player)
{
    if(player && Playerguid == player->GetGUID())
    {
        if((player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE) || (player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE))
        {
            PatientSavedCount++;
            if(PatientSavedCount == 15)
            {
                if(player->GetQuestStatus(6624) == QUEST_STATUS_INCOMPLETE)
                    player->CompleteQuest(6624);
                else if(player->GetQuestStatus(6622) == QUEST_STATUS_INCOMPLETE)
                    player->CompleteQuest(6622);

                Event = false;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }
}

void npc_doctorAI::UpdateAI(const uint32 diff)
{
    if (Event && SummonPatientCount >= 20)
    {
        Event = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    if (Event && SummonPatient_Timer < diff)
    {
        uint32 docEntry = m_creature->GetEntry();
        Creature* Patient = NULL;
        uint32 randLoc = rand()%6;
        if(docEntry == 12939)                                                   //Alliance doc entry
        {
            uint32 randId = allianceSoldierId[rand()%3];
            float X = AllianceCoords[randLoc].x;
            float Y = AllianceCoords[randLoc].y;
            float Z = AllianceCoords[randLoc].z;
            float O = AllianceCoords[randLoc].o;
            Patient = m_creature->SummonCreature(randId, X, Y, Z, O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
        }
        else if(docEntry == 12920)                                              //Horde doc entry
        {
            uint32 randId = hordeSoldierId[rand()%3];
            float X = HordeCoords[randLoc].x;
            float Y = HordeCoords[randLoc].y;
            float Z = HordeCoords[randLoc].z;
            float O = HordeCoords[randLoc].o;
            Patient = m_creature->SummonCreature(randId, X, Y, Z, O, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
        }
        if(Patient)
            ((npc_injured_patientAI*)Patient->AI())->Doctorguid = m_creature->GetGUID();
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

/*######
## 
######*/

void AddSC_npcs_dustwallow_marsh()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_deserter_agitator";
    newscript->GetAI = GetAI_npc_deserter_agitator;
    newscript->pGossipHello = &GossipHello_npc_deserter_agitator;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_lady_jaina_proudmoore";
    newscript->pGossipHello = &GossipHello_npc_lady_jaina_proudmoore;
    newscript->pGossipSelect = &GossipSelect_npc_lady_jaina_proudmoore;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_nat_pagle";
    newscript->pGossipHello = &GossipHello_npc_nat_pagle;
    newscript->pGossipSelect = &GossipSelect_npc_nat_pagle;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_restless_apparition";
    newscript->pGossipHello =   &GossipHello_npc_restless_apparition;
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

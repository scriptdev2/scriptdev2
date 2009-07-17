/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Netherstorm
SD%Complete: 80
SDComment: Quest support: 10438, 10652 (special flight paths), 10299,10321,10322,10323,10329,10330,10338,10365(Shutting Down Manaforge), 10198
SDCategory: Netherstorm
EndScriptData */

/* ContentData
npc_manaforge_control_console
go_manaforge_control_console
npc_commander_dawnforge
npc_protectorate_nether_drake
npc_veronia
EndContentData */

#include "precompiled.h"

/*######
## npc_manaforge_control_console
######*/

#define EMOTE_START     -1000211
#define EMOTE_60        -1000212
#define EMOTE_30        -1000213
#define EMOTE_10        -1000214
#define EMOTE_COMPLETE  -1000215
#define EMOTE_ABORT     -1000216

#define ENTRY_BNAAR_C_CONSOLE   20209
#define ENTRY_CORUU_C_CONSOLE   20417
#define ENTRY_DURO_C_CONSOLE    20418
#define ENTRY_ARA_C_CONSOLE     20440

#define ENTRY_SUNFURY_TECH      20218
#define ENTRY_SUNFURY_PROT      20436

#define ENTRY_ARA_TECH          20438
#define ENTRY_ARA_ENGI          20439
#define ENTRY_ARA_GORKLONN      20460

#define SPELL_DISABLE_VISUAL    35031
#define SPELL_INTERRUPT_1       35016                       //ACID mobs should cast this
#define SPELL_INTERRUPT_2       35176                       //ACID mobs should cast this (Manaforge Ara-version)

struct MANGOS_DLL_DECL npc_manaforge_control_consoleAI : public ScriptedAI
{
    npc_manaforge_control_consoleAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Event_Timer;
    uint32 Wave_Timer;
    uint32 Phase;
    bool Wave;
    uint64 someplayer;
    uint64 goConsole;
    Creature* add;

    void Reset()
    {
        Event_Timer = 3000;
        Wave_Timer = 0;
        Phase = 1;
        Wave = false;
        someplayer = 0;
        goConsole = 0;
        Creature* add = NULL;
    }

    /*void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        //we have no way of telling the creature was hit by spell -> got aura applied after 10-12 seconds
        //then no way for the mobs to actually stop the shutdown as intended.
        if (spell->Id == SPELL_INTERRUPT_1)
            ...
    }*/

    void JustDied(Unit* killer)
    {
        DoScriptText(EMOTE_ABORT, m_creature);

        if (someplayer)
        {
            Unit* p = Unit::GetUnit((*m_creature),someplayer);
            if (p && p->GetTypeId() == TYPEID_PLAYER)
            {
                switch(m_creature->GetEntry())
                {
                    case ENTRY_BNAAR_C_CONSOLE:
                        ((Player*)p)->FailQuest(10299);
                        ((Player*)p)->FailQuest(10329);
                        break;
                    case ENTRY_CORUU_C_CONSOLE:
                        ((Player*)p)->FailQuest(10321);
                        ((Player*)p)->FailQuest(10330);
                        break;
                    case ENTRY_DURO_C_CONSOLE:
                        ((Player*)p)->FailQuest(10322);
                        ((Player*)p)->FailQuest(10338);
                        break;
                    case ENTRY_ARA_C_CONSOLE:
                        ((Player*)p)->FailQuest(10323);
                        ((Player*)p)->FailQuest(10365);
                        break;
                }
            }
        }

        if (goConsole)
        {
            if (GameObject* pGo = m_creature->GetMap()->GetGameObject(goConsole))
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
        }
    }

    void DoWaveSpawnForCreature(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case ENTRY_BNAAR_C_CONSOLE:
                if (rand()%2)
                {
                    add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2933.68,4162.55,164.00,1.60,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,2927.36,4212.97,164.00);
                }
                else
                {
                    add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2927.36,4212.97,164.00,4.94,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,2933.68,4162.55,164.00);
                }
                Wave_Timer = 30000;
                break;
            case ENTRY_CORUU_C_CONSOLE:
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2445.21,2765.26,134.49,3.93,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2424.21,2740.15,133.81);
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2429.86,2731.85,134.53,1.31,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2435.37,2766.04,133.81);
                Wave_Timer = 20000;
                break;
            case ENTRY_DURO_C_CONSOLE:
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2986.80,2205.36,165.37,3.74,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2985.15,2197.32,164.79);
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2952.91,2191.20,165.32,0.22,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2060.01,2185.27,164.67);
                Wave_Timer = 15000;
                break;
            case ENTRY_ARA_C_CONSOLE:
                if (rand()%2)
                {
                    add = m_creature->SummonCreature(ENTRY_ARA_TECH,4035.11,4038.97,194.27,2.57,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4003.42,4040.19,193.49);
                    add = m_creature->SummonCreature(ENTRY_ARA_TECH,4033.66,4036.79,194.28,2.57,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4003.42,4040.19,193.49);
                    add = m_creature->SummonCreature(ENTRY_ARA_TECH,4037.13,4037.30,194.23,2.57,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4003.42,4040.19,193.49);
                }
                else
                {
                    add = m_creature->SummonCreature(ENTRY_ARA_TECH,3099.59,4049.30,194.22,0.05,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4028.01,4035.17,193.59);
                    add = m_creature->SummonCreature(ENTRY_ARA_TECH,3999.72,4046.75,194.22,0.05,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4028.01,4035.17,193.59);
                    add = m_creature->SummonCreature(ENTRY_ARA_TECH,3996.81,4048.26,194.22,0.05,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                    if (add) add->GetMotionMaster()->MovePoint(0,4028.01,4035.17,193.59);
                }
                Wave_Timer = 15000;
                break;
        }
    }
    void DoFinalSpawnForCreature(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case ENTRY_BNAAR_C_CONSOLE:
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2946.52,4201.42,163.47,3.54,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2927.49,4192.81,163.00);
                break;
            case ENTRY_CORUU_C_CONSOLE:
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2453.88,2737.85,133.27,2.59,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2433.96,2751.53,133.85);
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2441.62,2735.32,134.49,1.97,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2433.96,2751.53,133.85);
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2450.73,2754.50,134.49,3.29,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2433.96,2751.53,133.85);
                break;
            case ENTRY_DURO_C_CONSOLE:
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2956.18,2202.85,165.32,5.45,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2972.27,2193.22,164.48);
                add = m_creature->SummonCreature(ENTRY_SUNFURY_TECH,2975.30,2211.50,165.32,4.55,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2972.27,2193.22,164.48);
                add = m_creature->SummonCreature(ENTRY_SUNFURY_PROT,2965.02,2217.45,164.16,4.96,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,2972.27,2193.22,164.48);
                break;
            case ENTRY_ARA_C_CONSOLE:
                add = m_creature->SummonCreature(ENTRY_ARA_ENGI,3994.51,4020.46,192.18,0.91,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,4008.35,4035.04,192.70);
                add = m_creature->SummonCreature(ENTRY_ARA_GORKLONN,4021.56,4059.35,193.59,4.44,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                if (add) add->GetMotionMaster()->MovePoint(0,4016.62,4039.89,193.46);
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (Event_Timer < diff)
        {
            Unit *playertarget = NULL;

            if (someplayer)
            {
                playertarget = Unit::GetUnit((*m_creature),someplayer);
                if (!playertarget)
                    return;
                if (playertarget->GetTypeId() != TYPEID_PLAYER)
                    return;
            }else return;

            switch(Phase)
            {
                case 1:
                    DoScriptText(EMOTE_START, m_creature, playertarget);
                    Event_Timer = 60000;
                    Wave = true;
                    ++Phase;
                    break;
                case 2:
                    DoScriptText(EMOTE_60, m_creature, playertarget);
                    Event_Timer = 30000;
                    ++Phase;
                    break;
                case 3:
                    DoScriptText(EMOTE_30, m_creature, playertarget);
                    Event_Timer = 20000;
                    DoFinalSpawnForCreature(m_creature);
                    ++Phase;
                    break;
                case 4:
                    DoScriptText(EMOTE_10, m_creature, playertarget);
                    Event_Timer = 10000;
                    Wave = false;
                    ++Phase;
                    break;
                case 5:
                    DoScriptText(EMOTE_COMPLETE, m_creature, playertarget);
                    ((Player*)playertarget)->KilledMonsterCredit(m_creature->GetEntry(),m_creature->GetGUID());
                    DoCast(m_creature,SPELL_DISABLE_VISUAL);
                    if (goConsole)
                    {
                        if (GameObject* pGo = m_creature->GetMap()->GetGameObject(goConsole))
                            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                    }
                    ++Phase;
                    break;
            }
        } else Event_Timer -= diff;

        if (Wave)
        {
            if (Wave_Timer < diff)
            {
                DoWaveSpawnForCreature(m_creature);
            } else Wave_Timer -= diff;
        }
    }
};
CreatureAI* GetAI_npc_manaforge_control_console(Creature* pCreature)
{
    return new npc_manaforge_control_consoleAI(pCreature);
}

/*######
## go_manaforge_control_console
######*/

//TODO: clean up this workaround when mangos adds support to do it properly (with gossip selections instead of instant summon)
bool GOHello_go_manaforge_control_console(Player* pPlayer, GameObject* _GO)
{
    if (_GO->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
    {
        pPlayer->PrepareQuestMenu(_GO->GetGUID());
        pPlayer->SendPreparedQuest(_GO->GetGUID());
    }

    Creature* manaforge = NULL;

    switch(_GO->GetAreaId())
    {
        case 3726:                                          //b'naar
            if ((pPlayer->GetQuestStatus(10299) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(10329) == QUEST_STATUS_INCOMPLETE) &&
                pPlayer->HasItemCount(29366,1))
                manaforge = pPlayer->SummonCreature(ENTRY_BNAAR_C_CONSOLE,2918.95,4189.98,161.88,0.34,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
        case 3730:                                          //coruu
            if ((pPlayer->GetQuestStatus(10321) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(10330) == QUEST_STATUS_INCOMPLETE) &&
                pPlayer->HasItemCount(29396,1))
                manaforge = pPlayer->SummonCreature(ENTRY_CORUU_C_CONSOLE,2426.77,2750.38,133.24,2.14,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
        case 3734:                                          //duro
            if ((pPlayer->GetQuestStatus(10322) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(10338) == QUEST_STATUS_INCOMPLETE) &&
                pPlayer->HasItemCount(29397,1))
                manaforge = pPlayer->SummonCreature(ENTRY_DURO_C_CONSOLE,2976.48,2183.29,163.20,1.85,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
        case 3722:                                          //ara
            if ((pPlayer->GetQuestStatus(10323) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(10365) == QUEST_STATUS_INCOMPLETE) &&
                pPlayer->HasItemCount(29411,1))
                manaforge = pPlayer->SummonCreature(ENTRY_ARA_C_CONSOLE,4013.71,4028.76,192.10,1.25,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,125000);
            break;
    }

    if (manaforge)
    {
        ((npc_manaforge_control_consoleAI*)manaforge->AI())->someplayer = pPlayer->GetGUID();
        ((npc_manaforge_control_consoleAI*)manaforge->AI())->goConsole = _GO->GetGUID();
        _GO->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
    }
    return true;
}

/*######
## npc_commander_dawnforge
######*/

// The Speech of Dawnforge, Ardonis & Pathaleon
#define SAY_COMMANDER_DAWNFORGE_1           -1000128
#define SAY_ARCANIST_ARDONIS_1              -1000129
#define SAY_COMMANDER_DAWNFORGE_2           -1000130
#define SAY_PATHALEON_CULATOR_IMAGE_1       -1000131
#define SAY_COMMANDER_DAWNFORGE_3           -1000132
#define SAY_PATHALEON_CULATOR_IMAGE_2       -1000133
#define SAY_PATHALEON_CULATOR_IMAGE_2_1     -1000134
#define SAY_PATHALEON_CULATOR_IMAGE_2_2     -1000135
#define SAY_COMMANDER_DAWNFORGE_4           -1000136
#define SAY_ARCANIST_ARDONIS_2              -1000136
#define SAY_COMMANDER_DAWNFORGE_5           -1000137

#define QUEST_INFO_GATHERING                10198
#define SPELL_SUNFURY_DISGUISE              34603

// Entries of Arcanist Ardonis, Commander Dawnforge, Pathaleon the Curators Image
const uint32 uiCreatureEntry[3] =
{
    19830,                                                  // Ardonis
    19831,                                                  // Dawnforge
    21504                                                   // Pathaleon
};

struct MANGOS_DLL_DECL npc_commander_dawnforgeAI : public ScriptedAI
{
    npc_commander_dawnforgeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset (); }

    uint64 playerGUID;
    uint64 ardonisGUID;
    uint64 pathaleonGUID;

    uint32 Phase;
    uint32 PhaseSubphase;
    uint32 Phase_Timer;
    bool isEvent;

    float angle_dawnforge;
    float angle_ardonis;

    void Reset()
    {
        playerGUID = 0;
        ardonisGUID = 0;
        pathaleonGUID = 0;

        Phase = 1;
        PhaseSubphase = 0;
        Phase_Timer = 4000;
        isEvent = false;
    }

    void JustSummoned(Creature *summoned)
    {
        pathaleonGUID = summoned->GetGUID();
    }

    // Emote Ardonis and Pathaleon
    void Turn_to_Pathaleons_Image()
    {
        Unit *ardonis = Unit::GetUnit(*m_creature,ardonisGUID);
        Unit *pathaleon = Unit::GetUnit(*m_creature,pathaleonGUID);
        Player* pPlayer = (Player*)Unit::GetUnit(*m_creature,playerGUID);

        if (!ardonis || !pathaleon || !pPlayer)
            return;

        //Calculate the angle to Pathaleon
        angle_dawnforge = m_creature->GetAngle(pathaleon->GetPositionX(), pathaleon->GetPositionY());
        angle_ardonis = ardonis->GetAngle(pathaleon->GetPositionX(), pathaleon->GetPositionY());

        //Turn Dawnforge and update
        m_creature->SetOrientation(angle_dawnforge);
        m_creature->SendUpdateToPlayer(pPlayer);
        //Turn Ardonis and update
        ardonis->SetOrientation(angle_ardonis);
        ardonis->SendUpdateToPlayer(pPlayer);

        //Set them to kneel
        m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
        ardonis->SetStandState(UNIT_STAND_STATE_KNEEL);
    }

    //Set them back to each other
    void Turn_to_eachother()
    {
        if (Unit *ardonis = Unit::GetUnit(*m_creature,ardonisGUID))
        {
            Player* pPlayer = (Player*)Unit::GetUnit(*m_creature,playerGUID);

            if (!pPlayer)
                return;

            angle_dawnforge = m_creature->GetAngle(ardonis->GetPositionX(), ardonis->GetPositionY());
            angle_ardonis = ardonis->GetAngle(m_creature->GetPositionX(), m_creature->GetPositionY());

            //Turn Dawnforge and update
            m_creature->SetOrientation(angle_dawnforge);
            m_creature->SendUpdateToPlayer(pPlayer);
            //Turn Ardonis and update
            ardonis->SetOrientation(angle_ardonis);
            ardonis->SendUpdateToPlayer(pPlayer);

            //Set state
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            ardonis->SetStandState(UNIT_STAND_STATE_STAND);
        }
    }

    bool CanStartEvent(Player* pPlayer)
    {
        if (!isEvent)
        {
            Creature* ardonis = GetClosestCreatureWithEntry(m_creature, uiCreatureEntry[0], 10.0f);

            if (!ardonis)
                return false;

            ardonisGUID = ardonis->GetGUID();
            playerGUID = pPlayer->GetGUID();

            isEvent = true;

            Turn_to_eachother();
            return true;
        }

        debug_log("SD2: npc_commander_dawnforge event already in progress, need to wait.");
        return false;
    }

    void UpdateAI(const uint32 diff)
    {
        //Is event even running?
        if (!isEvent)
            return;

        //Phase timing
        if (Phase_Timer >= diff)
        {
            Phase_Timer -= diff;
            return;
        }

        Unit *ardonis = Unit::GetUnit(*m_creature,ardonisGUID);
        Unit *pathaleon = Unit::GetUnit(*m_creature,pathaleonGUID);
        Player* pPlayer = (Player*)Unit::GetUnit(*m_creature,playerGUID);

        if (!ardonis || !pPlayer)
        {
            Reset();
            return;
        }

        if (Phase > 4 && !pathaleon)
        {
            Reset();
            return;
        }

        //Phase 1 Dawnforge say
        switch (Phase)
        {
            case 1:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_1, m_creature);
                ++Phase;
                Phase_Timer = 16000;
                break;
            //Phase 2 Ardonis say
            case 2:
                DoScriptText(SAY_ARCANIST_ARDONIS_1, ardonis);
                ++Phase;
                Phase_Timer = 16000;
                break;
            //Phase 3 Dawnforge say
            case 3:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_2, m_creature);
                ++Phase;
                Phase_Timer = 16000;
                break;
            //Phase 4 Pathaleon spawns up to phase 9
            case 4:
                //spawn pathaleon's image
                m_creature->SummonCreature(uiCreatureEntry[2], 2325.851563, 2799.534668, 133.084229, 6.038996, TEMPSUMMON_TIMED_DESPAWN, 90000);
                ++Phase;
                Phase_Timer = 500;
                break;
            //Phase 5 Pathaleon say
            case 5:
                DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_1, pathaleon);
                ++Phase;
                Phase_Timer = 6000;
                break;
            //Phase 6
            case 6:
                switch(PhaseSubphase)
                {
                    //Subphase 1: Turn Dawnforge and Ardonis
                    case 0:
                        Turn_to_Pathaleons_Image();
                        ++PhaseSubphase;
                        Phase_Timer = 8000;
                        break;
                    //Subphase 2 Dawnforge say
                    case 1:
                        DoScriptText(SAY_COMMANDER_DAWNFORGE_3, m_creature);
                        PhaseSubphase = 0;
                        ++Phase;
                        Phase_Timer = 8000;
                        break;
                }
                break;
            //Phase 7 Pathaleons say 3 Sentence, every sentence need a subphase
            case 7:
                switch(PhaseSubphase)
                {
                    //Subphase 1
                    case 0:
                        DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_2, pathaleon);
                        ++PhaseSubphase;
                        Phase_Timer = 12000;
                        break;
                    //Subphase 2
                    case 1:
                        DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_2_1, pathaleon);
                        ++PhaseSubphase;
                        Phase_Timer = 16000;
                        break;
                    //Subphase 3
                    case 2:
                        DoScriptText(SAY_PATHALEON_CULATOR_IMAGE_2_2, pathaleon);
                        PhaseSubphase = 0;
                        ++Phase;
                        Phase_Timer = 10000;
                        break;
                }
                break;
            //Phase 8 Dawnforge & Ardonis say
            case 8:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_4, m_creature);
                DoScriptText(SAY_ARCANIST_ARDONIS_2, ardonis);
                ++Phase;
                Phase_Timer = 4000;
                break;
            //Phase 9 Pathaleons Despawn, Reset Dawnforge & Ardonis angle
            case 9:
                Turn_to_eachother();
                //hide pathaleon, unit will despawn shortly
                pathaleon->SetVisibility(VISIBILITY_OFF);
                PhaseSubphase = 0;
                ++Phase;
                Phase_Timer = 3000;
                break;
            //Phase 10 Dawnforge say
            case 10:
                DoScriptText(SAY_COMMANDER_DAWNFORGE_5, m_creature);
                pPlayer->AreaExploredOrEventHappens(QUEST_INFO_GATHERING);
                Reset();
                break;
        }
    }
};

CreatureAI* GetAI_npc_commander_dawnforge(Creature* pCreature)
{
    return new npc_commander_dawnforgeAI(pCreature);
}

bool AreaTrigger_at_commander_dawnforge(Player* pPlayer, AreaTriggerEntry *at)
{
    //if player lost aura or not have at all, we should not try start event.
    if (!pPlayer->HasAura(SPELL_SUNFURY_DISGUISE,0))
        return false;

    if (pPlayer->isAlive() && pPlayer->GetQuestStatus(QUEST_INFO_GATHERING) == QUEST_STATUS_INCOMPLETE)
    {
        Creature* pCreature = GetClosestCreatureWithEntry(pPlayer, uiCreatureEntry[1], 30.0f);

        if (!pCreature)
            return false;

        if (((npc_commander_dawnforgeAI*)pCreature->AI())->CanStartEvent(pPlayer))
            return true;
    }
    return false;
}

/*######
## npc_protectorate_nether_drake
######*/

enum
{
    QUEST_NETHER_WINGS          = 10438,
    ITEM_PH_DISRUPTOR           = 29778,
    TAXI_PATH_ID                = 627                       //(possibly 627+628(152->153->154->155))
};

#define GOSSIP_ITEM_FLY_ULTRIS  "Fly me to Ultris"

bool GossipHello_npc_protectorate_nether_drake(Player* pPlayer, Creature* pCreature)
{
    //On Nethery Wings
    if (pPlayer->GetQuestStatus(QUEST_NETHER_WINGS) == QUEST_STATUS_INCOMPLETE && pPlayer->HasItemCount(ITEM_PH_DISRUPTOR,1))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FLY_ULTRIS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_protectorate_nether_drake(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID);
    }
    return true;
}

/*######
## npc_veronia
######*/

bool GossipHello_npc_veronia(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    //Behind Enemy Lines
    if (pPlayer->GetQuestStatus(10652) && !pPlayer->GetQuestRewardStatus(10652))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Fly me to Manaforge Coruu please", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_veronia(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,34905,true);               //TaxiPath 606
    }
    return true;
}

/*######
##
######*/

void AddSC_netherstorm()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_manaforge_control_console";
    newscript->pGOHello = &GOHello_go_manaforge_control_console;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_manaforge_control_console";
    newscript->GetAI = &GetAI_npc_manaforge_control_console;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_commander_dawnforge";
    newscript->GetAI = GetAI_npc_commander_dawnforge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_commander_dawnforge";
    newscript->pAreaTrigger = &AreaTrigger_at_commander_dawnforge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_protectorate_nether_drake";
    newscript->pGossipHello = &GossipHello_npc_protectorate_nether_drake;
    newscript->pGossipSelect = &GossipSelect_npc_protectorate_nether_drake;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_veronia";
    newscript->pGossipHello = &GossipHello_npc_veronia;
    newscript->pGossipSelect = &GossipSelect_npc_veronia;
    newscript->RegisterSelf();
}

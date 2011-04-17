/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Victor_Nefarius
SD%Complete: 75
SDComment: Missing some text, Vael beginning event, and spawns Nef in wrong place
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"

enum
{
    SAY_GAMESBEGIN_1                = -1469004,
    SAY_GAMESBEGIN_2                = -1469005,

    GOSSIP_ITEM_NEFARIUS_1          = -3469000,
    GOSSIP_ITEM_NEFARIUS_2          = -3469001,
    GOSSIP_ITEM_NEFARIUS_3          = -3469002,
    GOSSIP_TEXT_NEFARIUS_1          = 7134,
    GOSSIP_TEXT_NEFARIUS_2          = 7198,
    GOSSIP_TEXT_NEFARIUS_3          = 7199,

    MAX_DRAKES                      = 5,
    NPC_BRONZE_DRAKANOID            = 14263,
    NPC_BLUE_DRAKANOID              = 14261,
    NPC_RED_DRAKANOID               = 14264,
    NPC_GREEN_DRAKANOID             = 14262,
    NPC_BLACK_DRAKANOID             = 14265,
    NPC_CHROMATIC_DRAKANOID         = 14302,

    SPELL_NEFARIUS_BARRIER          = 22663,                // immunity in phase 1
    SPELL_SHADOWBOLT                = 21077,
    SPELL_FEAR                      = 26070,                // shouldn't this be 22678?
    // shadowbolt vollye = 22665
    // silence = 22666 -> silence a player
    // shadow command = 22667 -> charm a player
    // shadowblink = 22664, 22681 -> teleport around the room, possibly random

    FACTION_BLACK_DRAGON            = 103,
};

struct SpawnLocation
{
    float m_fX, m_fY, m_fZ;
};

static const SpawnLocation aNefarianLocs[4] =
{
    {-7591.151f, -1204.051f, 476.800f},     // adds 1 & 2
    {-7514.598f, -1150.448f, 476.796f},
    {-7445.0f, -1332.0f, 536.0f},           // nefarian
    {-7592.0f, -1264.0f, 481.0f},           // hide pos
};

static const uint32 aPossibleDrake[MAX_DRAKES] = {NPC_BRONZE_DRAKANOID, NPC_BLUE_DRAKANOID, NPC_RED_DRAKANOID, NPC_GREEN_DRAKANOID, NPC_BLACK_DRAKANOID};

//This script is complicated
//Instead of morphing Victor Nefarius we will have him control phase 1
//And then have him spawn "Nefarian" for phase 2
//When phase 2 starts Victor Nefarius will go into hiding and stop attacking
//If Nefarian despawns because he killed the players then this guy will EnterEvadeMode
//and allow players to start the event over
//If nefarian dies then he will kill himself then he will kill himself in his hiding place
//To prevent players from doing the event twice

struct MANGOS_DLL_DECL boss_victor_nefariusAI : public ScriptedAI
{
    boss_victor_nefariusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        // Select the 2 different drakes that we are going to use until despawned
        // 5 possiblities for the first drake, 4 for the second, 20 total possiblites

        // select two different numbers between 0..MAX_DRAKES-1
        uint8 uiPos1 = urand(0, MAX_DRAKES - 1);
        uint8 uiPos2 = (uiPos1 + urand(1, MAX_DRAKES - 1)) % MAX_DRAKES;

        m_uiDrakeTypeOne = aPossibleDrake[uiPos1];
        m_uiDrakeTypeTwo = aPossibleDrake[uiPos2];

        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpawnedAdds;
    uint32 m_uiAddSpawnTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiMindControlTimer;
    uint32 m_uiResetTimer;
    uint32 m_uiDrakeTypeOne;
    uint32 m_uiDrakeTypeTwo;
    uint64 m_uiNefarianGUID;
    uint32 m_uiNefCheckTime;

    void Reset()
    {
        m_uiSpawnedAdds = 0;
        m_uiAddSpawnTimer = 10000;
        m_uiShadowBoltTimer = 5000;
        m_uiFearTimer = 8000;
        m_uiResetTimer = 900000;                                //On official it takes him 15 minutes(900 seconds) to reset. We are only doing 1 minute to make testing easier
        m_uiNefarianGUID = 0;
        m_uiNefCheckTime = 2000;
        m_uiNefarianGUID = 0;

        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Only do this if we haven't spawned nef yet
        if (m_uiSpawnedAdds < 42)
        {
            // ShadowBoltTimer
            if (m_uiShadowBoltTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SHADOWBOLT) == CAST_OK)
                        m_uiShadowBoltTimer = urand(3000, 10000);
                }
            }
            else
                m_uiShadowBoltTimer -= uiDiff;

            // FearTimer
            if (m_uiFearTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_FEAR) == CAST_OK)
                        m_uiFearTimer = urand(10000, 20000);
                }
            }
            else
                m_uiFearTimer -= uiDiff;

            //Add spawning mechanism
            if (m_uiAddSpawnTimer < uiDiff)
            {
                //Spawn 2 random types of creatures at the 2 locations
                uint32 CreatureID;
                Creature* Spawned = NULL;
                Unit* target = NULL;

                //1 in 3 chance it will be a chromatic
                if (!urand(0, 2))
                    CreatureID = NPC_CHROMATIC_DRAKANOID;
                else CreatureID = m_uiDrakeTypeOne;

                ++m_uiSpawnedAdds;

                //Spawn creature and force it to start attacking a random target
                Spawned = m_creature->SummonCreature(CreatureID, aNefarianLocs[0].m_fX, aNefarianLocs[0].m_fY, aNefarianLocs[0].m_fZ, 5.000f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                if (target && Spawned)
                {
                    Spawned->AI()->AttackStart(target);
                    Spawned->setFaction(103);
                }

                //1 in 3 chance it will be a chromatic
                if (!urand(0, 2))
                    CreatureID = NPC_CHROMATIC_DRAKANOID;
                else CreatureID = m_uiDrakeTypeTwo;

                ++m_uiSpawnedAdds;

                target = NULL;
                Spawned = NULL;
                Spawned = m_creature->SummonCreature(CreatureID, aNefarianLocs[1].m_fX, aNefarianLocs[1].m_fY, aNefarianLocs[1].m_fZ, 5.000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                if (target && Spawned)
                {
                    Spawned->AI()->AttackStart(target);
                    Spawned->setFaction(103);
                }

                //Begin phase 2 by spawning Nefarian and what not
                if (m_uiSpawnedAdds >= 42)
                {
                    //Teleport Victor Nefarius way out of the map
                    //MapManager::Instance().GetMap(m_creature->GetMapId(), m_creature)->CreatureRelocation(m_creature,0,0,-5000,0);

                    //Inturrupt any spell casting
                    m_creature->InterruptNonMeleeSpells(false);

                    //Root self
                    DoCastSpellIfCan(m_creature,33356);

                    //Make super invis
                    DoCastSpellIfCan(m_creature,8149);

                    //Teleport self to a hiding spot
                    m_creature->NearTeleportTo(aNefarianLocs[3].m_fX, aNefarianLocs[3].m_fY, aNefarianLocs[3].m_fZ, 0.0f);

                    //Spawn nef and have him attack a random target
                    Creature* Nefarian = m_creature->SummonCreature(NPC_NEFARIAN, aNefarianLocs[2].m_fX, aNefarianLocs[2].m_fY, aNefarianLocs[2].m_fZ, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);

                    if (target && Nefarian)
                    {
                        Nefarian->AI()->AttackStart(target);
                        Nefarian->setFaction(103);
                        m_uiNefarianGUID = Nefarian->GetGUID();
                    }
                    else error_log("SD2: Blackwing Lair: Unable to spawn nefarian properly.");
                }

                m_uiAddSpawnTimer = 4000;
            }
            else
                m_uiAddSpawnTimer -= uiDiff;
        }
        else if (m_uiNefarianGUID)
        {
            if (m_uiNefCheckTime < uiDiff)
            {
                Creature* pNefarian = m_creature->GetMap()->GetCreature(m_uiNefarianGUID);

                //If nef is dead then we die to so the players get out of combat
                //and cannot repeat the event
                if (!pNefarian || !pNefarian->isAlive())
                {
                    m_uiNefarianGUID = 0;
                    m_creature->ForcedDespawn();
                }

                m_uiNefCheckTime = 2000;
            }
            else
                m_uiNefCheckTime -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_victor_nefarius(Creature* pCreature)
{
    return new boss_victor_nefariusAI(pCreature);
}

bool GossipHello_boss_victor_nefarius(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEFARIUS_1 , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_NEFARIUS_1, pCreature->GetGUID());
    return true;
}

bool GossipSelect_boss_victor_nefarius(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEFARIUS_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_NEFARIUS_2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NEFARIUS_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_NEFARIUS_3, pCreature->GetGUID());
            DoScriptText(SAY_GAMESBEGIN_1, pCreature);
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            DoScriptText(SAY_GAMESBEGIN_2, pCreature);
            // remove gossip, set hostile and attack
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->setFaction(FACTION_BLACK_DRAGON);
            pCreature->CastSpell(pCreature, SPELL_NEFARIUS_BARRIER, false);
            pCreature->AI()->AttackStart(pPlayer);
            break;
    }
    return true;
}

void AddSC_boss_victor_nefarius()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_victor_nefarius";
    pNewScript->GetAI = &GetAI_boss_victor_nefarius;
    pNewScript->pGossipHello = &GossipHello_boss_victor_nefarius;
    pNewScript->pGossipSelect = &GossipSelect_boss_victor_nefarius;
    pNewScript->RegisterSelf();
}

/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Violet_Hold
SD%Complete: 40
SDComment:
SDCategory: Violet Hold
EndScriptData */

/* ContentData
go_activation_crystal
npc_door_seal
npc_sinclari
npc_teleportation_portal
EndContentData */

#include "precompiled.h"
#include "violet_hold.h"
#include "escort_ai.h"

/*######
## go_activation_crystal
######*/

bool GOUse_go_activation_crystal(Player* pPlayer, GameObject* pGo)
{
    if (instance_violet_hold* pInstance = (instance_violet_hold*)pGo->GetInstanceData())
        pInstance->ProcessActivationCrystal(pPlayer);

    return false;
}

/*######
## npc_door_seal
######*/

bool EffectDummyCreature_npc_door_seal(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_DESTROY_DOOR_SEAL && uiEffIndex == EFFECT_INDEX_0)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreatureTarget->GetInstanceData())
            pInstance->SetData(TYPE_SEAL, SPECIAL);

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

/*######
## npc_sinclari
######*/

enum
{
    SAY_BEGIN                   = -1608000,
    SAY_LOCK_DOOR               = -1608001,

    GOSSIP_ITEM_INTRO           = -3608000,
    GOSSIP_ITEM_START           = -3608001,

    GOSSIP_TEXT_ID_INTRO        = 13853,
    GOSSIP_TEXT_ID_START        = 13854,
};

struct MANGOS_DLL_DECL npc_sinclariAI : public npc_escortAI
{
    npc_sinclariAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
        Reset();
    }

    instance_violet_hold* m_pInstance;

    void Reset()
    {
    }

    void WaypointReached(uint32 uiPointId)
    {
        if (!m_pInstance)
            return;

        switch(uiPointId)
        {
            case 0:
                m_pInstance->ProcessActivationCrystal(m_creature, true);
                break;
            case 1:
                DoScriptText(SAY_BEGIN, m_creature);
                m_pInstance->SetIntroPortals(true);
                m_pInstance->CallGuards(false);
                break;
            case 2:
                DoScriptText(SAY_LOCK_DOOR, m_creature);
                m_pInstance->SetData(TYPE_MAIN, IN_PROGRESS);
                break;
        }
    }

    void JustRespawned()
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MAIN) != DONE)
            m_pInstance->SetData(TYPE_MAIN, NOT_STARTED);

        npc_escortAI::JustRespawned();                      // Needed, to reset escort state, waypoints, etc
    }
};

CreatureAI* GetAI_npc_sinclari(Creature* pCreature)
{
    return new npc_sinclariAI(pCreature);
}

bool GossipHello_npc_sinclari(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INTRO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_INTRO, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_sinclari(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
            {
                pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_START, pCreature->GetGUID());
            }
        }
        else
            pPlayer->CLOSE_GOSSIP_MENU();
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData())
        {
            pPlayer->CLOSE_GOSSIP_MENU();

            if (pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
            {
                pInstance->SetData(TYPE_MAIN, SPECIAL);

                if (npc_sinclariAI* pEscortAI = dynamic_cast<npc_sinclariAI*>(pCreature->AI()))
                    pEscortAI->Start();
            }
        }
        else
            pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

/*######
## npc_teleportation_portal
######*/

struct MANGOS_DLL_DECL npc_teleportation_portalAI : public ScriptedAI
{
    npc_teleportation_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
        m_uiMyPortalNumber = 0;
        Reset();
    }

    instance_violet_hold* m_pInstance;

    std::set<uint64> m_lMobSet;

    bool m_bNeedInvisible;
    bool m_bIntro;
    uint32 m_uiIntroTimer;
    uint32 m_uiMyPortalNumber;

    void Reset()
    {
        m_bNeedInvisible = false;
        m_bIntro = false;
        m_uiIntroTimer = 10000;

        if (m_pInstance)
            m_uiMyPortalNumber = m_pInstance->GetCurrentPortalNumber();
    }

    void DoSummon()
    {
        if (m_creature->GetEntry() == NPC_PORTAL_INTRO)
        {
            //not made yet
            return;
        }
        else if (m_creature->GetEntry() == NPC_PORTAL)
        {
            if (m_pInstance && m_pInstance->GetCurrentPortalNumber() == 18)
            {
                m_creature->SummonCreature(NPC_CYANIGOSA, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);
            }
            else
            {
                m_creature->SummonCreature(m_pInstance->GetRandomPortalEliteEntry(), 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);
                m_creature->CastSpell(m_creature, SPELL_PORTAL_PERIODIC, true);
            }
        }
        else if (m_pInstance->IsCurrentPortalForTrash())
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                uint32 uiSummonId;

                switch(i)
                {
                    case 0: uiSummonId = NPC_AZURE_CAPTAIN; break;
                    case 1: uiSummonId = NPC_AZURE_RAIDER; break;
                    case 2: uiSummonId = NPC_AZURE_SORCEROR; break;
                    case 3: uiSummonId = NPC_AZURE_STALKER; break;
                }

                m_creature->SummonCreature(uiSummonId, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);
            }

            m_bNeedInvisible = true;
        }
        else
        {
            m_creature->SummonCreature(NPC_AZURE_SABOTEUR, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);
            m_bNeedInvisible = true;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_PORTAL_GUARDIAN:
                DoScriptText(EMOTE_GUARDIAN_PORTAL, pSummoned);
                m_creature->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL, false);
                break;
            case NPC_PORTAL_KEEPER:
                DoScriptText(EMOTE_KEEPER_PORTAL, pSummoned);
                m_creature->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL, false);
                break;
            case NPC_AZURE_CAPTAIN:
                DoScriptText(EMOTE_DRAGONFLIGHT_PORTAL, pSummoned);
                m_lMobSet.insert(pSummoned->GetGUID());
                break;
            case NPC_AZURE_RAIDER:
            case NPC_AZURE_SORCEROR:
            case NPC_AZURE_STALKER:
                m_lMobSet.insert(pSummoned->GetGUID());
                return;
            case NPC_AZURE_SABOTEUR:
            {
                if (!m_pInstance)
                    return;
                const sBossInformation* pData = m_pInstance->GetBossInformation();
                if (pData)
                    pSummoned->GetMotionMaster()->MovePoint(pData->uiWayPointId, pData->fX, pData->fY, pData->fZ);
                return;
            }
            default:
                return;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PORTAL, SPECIAL);
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE && pSummoned->GetEntry() != NPC_AZURE_SABOTEUR)
            return;

        if (uiPointId == 1)
        {
            pSummoned->CastSpell(pSummoned, SPELL_SHIELD_DISRUPTION, false);
            if (m_pInstance)
            {
                if (const sBossInformation* pData = m_pInstance->GetBossInformation())
                {
                    if (Creature* pBoss = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(m_pInstance->GetData(pData->uiType) != DONE ? pData->uiEntry : pData->uiGhostEntry)))
                    {
                        m_pInstance->UpdateCellForBoss(pData->uiEntry);
                        if (pData->iSayEntry)
                            DoScriptText(pData->iSayEntry, pBoss);

                        // TODO, adds for Erekem? Opening their Cells? Reset flags on FAIL?
                        pBoss->GetMotionMaster()->MovePoint(1, pData->fX, pData->fY, pData->fZ);
                        pBoss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                    }
                }
            }
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_PORTAL_GUARDIAN:
            case NPC_PORTAL_KEEPER:
                break;
            case NPC_AZURE_CAPTAIN:
            case NPC_AZURE_RAIDER:
            case NPC_AZURE_SORCEROR:
            case NPC_AZURE_STALKER:
            {
                m_lMobSet.erase(pSummoned->GetGUID());

                if (!m_lMobSet.empty())
                    return;

                break;
            }
            default:
                return;
        }

        if (m_pInstance)
        {
            // no need if a new portal was made while this was in progress
            if (m_uiMyPortalNumber == m_pInstance->GetCurrentPortalNumber())
                m_pInstance->SetData(TYPE_PORTAL, DONE);
        }

        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiIntroTimer)
        {
            if (m_uiIntroTimer <= uiDiff)
            {
                if (!m_pInstance)
                {
                    m_creature->ForcedDespawn();
                    return;
                }

                m_uiIntroTimer = 0;
            }
            else
            {
                m_uiIntroTimer -= uiDiff;
                return;
            }
        }

        if (!m_bIntro)
        {
            DoSummon();
            m_bIntro = true;
        }

        if (m_bNeedInvisible)
        {
            // hack; find a better way
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_bNeedInvisible = false;
        }
    }
};

CreatureAI* GetAI_npc_teleportation_portal(Creature* pCreature)
{
    return new npc_teleportation_portalAI(pCreature);
}

bool EffectDummyCreature_npc_teleportation_portal(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_PORTAL_PERIODIC && uiEffIndex == EFFECT_INDEX_0)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreatureTarget->GetInstanceData())
            pCreatureTarget->SummonCreature(pInstance->GetRandomMobForNormalPortal(), 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

void AddSC_violet_hold()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_activation_crystal";
    newscript->pGOUse = &GOUse_go_activation_crystal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_door_seal";
    newscript->pEffectDummyNPC = &EffectDummyCreature_npc_door_seal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_sinclari";
    newscript->GetAI = &GetAI_npc_sinclari;
    newscript->pGossipHello = &GossipHello_npc_sinclari;
    newscript->pGossipSelect = &GossipSelect_npc_sinclari;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_teleportation_portal";
    newscript->GetAI = &GetAI_npc_teleportation_portal;
    newscript->pEffectDummyNPC = &EffectDummyCreature_npc_teleportation_portal;
    newscript->RegisterSelf();
}

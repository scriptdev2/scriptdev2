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
SDName: boss_kiljaeden
SD%Complete: 20
SDComment: Only spawn support and epilogue event
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum
{
    SAY_ORDER_1                 = -1580064,
    SAY_ORDER_2                 = -1580065,
    SAY_ORDER_3                 = -1580066,
    SAY_ORDER_4                 = -1580067,
    SAY_ORDER_5                 = -1580068,
    SAY_EMERGE                  = -1580069,
    SAY_SLAY_1                  = -1580070,
    SAY_SLAY_2                  = -1580071,
    SAY_REFLECTION_1            = -1580072,
    SAY_REFLECTION_2            = -1580073,
    SAY_DARKNESS_1              = -1580074,
    SAY_DARKNESS_2              = -1580075,
    SAY_DARKNESS_3              = -1580076,
    SAY_PHASE_3                 = -1580077,
    SAY_PHASE_4                 = -1580078,
    SAY_PHASE_5                 = -1580079,
    SAY_KALECGOS_INTRO          = -1580080,
    SAY_KALECGOS_AWAKE_1        = -1580081,
    SAY_ANVEENA_IMPRISONED      = -1580082,
    SAY_KALECGOS_AWAKE_2        = -1580083,
    SAY_ANVEENA_LOST            = -1580084,
    SAY_KALECGOS_AWAKE_4        = -1580085,
    SAY_ANVEENA_AWAKE           = -1580086,
    SAY_KALECGOS_AWAKE_5        = -1580087,
    SAY_ANVEENA_SACRIFICE       = -1580088,
    SAY_KALECGOS_GOODBYE        = -1580089,
    SAY_KALECGOS_ENCOURAGE      = -1580090,
    SAY_KALECGOS_ORB_1          = -1580091,
    SAY_KALECGOS_ORB_2          = -1580092,
    SAY_KALECGOS_ORB_3          = -1580093,
    SAY_KALECGOS_ORB_4          = -1580094,

    // outro
    SAY_OUTRO_1                 = -1580095,         // Velen
    SAY_OUTRO_2                 = -1580096,
    SAY_OUTRO_3                 = -1580097,
    SAY_OUTRO_4                 = -1580098,
    SAY_OUTRO_5                 = -1580099,         // Liadrin
    SAY_OUTRO_6                 = -1580100,         // Velen
    SAY_OUTRO_7                 = -1580101,         // Liadrin
    SAY_OUTRO_8                 = -1580102,         // Velen
    SAY_OUTRO_9                 = -1580103,
    SAY_OUTRO_10                = -1580104,         // Liadrin
    SAY_OUTRO_11                = -1580105,         // Velen
    SAY_OUTRO_12                = -1580106,

    // spells
    SPELL_BIRTH                 = 37745,            // Kiljaeden spawn animation

    // outro
    SPELL_TELEPORT_VISUAL       = 41232,
    SPELL_KALEC_TELEPORT        = 46473,            // teleports and transforms Kalec in human form
    SPELL_CALL_ENTROPIUS        = 46818,
    SPELL_ENTROPIUS_BODY        = 46819,
    SPELL_BLAZE_TO_LIGHT        = 46821,
    SPELL_SUNWELL_IGNITION      = 46822,

    NPC_BOSS_PORTAL             = 24925,
    NPC_CORE_ENTROPIUS          = 26262,
    NPC_SOLDIER                 = 26259,            // summoned in 2 waves before Velen. Should move into 2 circle formations
    NPC_RIFTWALKER              = 26289,

    POINT_SUMMON_SOLDIERS       = 1,
    POINT_MOVE_LIADRIN          = 2,
    POINT_EVENT_EXIT            = 3,
};

static const DialogueEntry aOutroDialogue[] =
{
    {NPC_KALECGOS,          0,              15000},
    {SAY_KALECGOS_GOODBYE,  NPC_KALECGOS,   40000},
    {NPC_BOSS_PORTAL,       0,              10000},
    {POINT_SUMMON_SOLDIERS, 0,              18000},
    {NPC_VELEN,             0,              1000},
    {NPC_LIADRIN,           0,              4000},
    {SAY_OUTRO_1,           NPC_VELEN,      25000},
    {SAY_OUTRO_2,           NPC_VELEN,      15000},
    {SAY_OUTRO_3,           NPC_VELEN,      13000},
    {SPELL_CALL_ENTROPIUS,  0,              10000},
    {SAY_OUTRO_4,           NPC_VELEN,      20000},
    {POINT_MOVE_LIADRIN,    0,              5000},
    {SAY_OUTRO_5,           NPC_LIADRIN,    10000},
    {SAY_OUTRO_6,           NPC_VELEN,      15000},
    {SAY_OUTRO_7,           NPC_LIADRIN,    3000},
    {SAY_OUTRO_8,           NPC_VELEN,      4000},
    {SPELL_BLAZE_TO_LIGHT,  0,              13000},
    {SAY_OUTRO_9,           NPC_VELEN,      14000},
    {SAY_OUTRO_10,          NPC_LIADRIN,    20000},
    {SAY_OUTRO_11,          NPC_VELEN,      8000},
    {SAY_OUTRO_12,          NPC_VELEN,      4000},
    {POINT_EVENT_EXIT,      0,              0},
    {0, 0, 0},
};

struct EventLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static const EventLocations aOutroLocations[] =
{
    {1727.854f, 656.060f, 28.31f, 3.86f},       // portal summon loc
    {1716.969f, 646.407f, 28.05f, 3.91f},       // velen summon loc
    {1718.862f, 644.528f, 28.05f, 3.87f},       // liadrin summon loc
    {1712.110f, 641.044f, 27.80f},              // velen move forward
    {1711.537f, 637.600f, 27.34f}               // liadrin move forward
};

struct MANGOS_DLL_DECL npc_kiljaeden_controllerAI : public Scripted_NoMovementAI, private DialogueHelper
{
    npc_kiljaeden_controllerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature),
        DialogueHelper(aOutroDialogue)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        InitializeDialogueHelper(m_pInstance);
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

    ObjectGuid m_EntropiusGuid;
    ObjectGuid m_PortalGuid;

    void Reset()
    {
        // Visual spell before the encounter starts
        DoCastSpellIfCan(m_creature, SPELL_ANVEENA_DRAIN);
    }

    // Wrapper to start the dialogue text from another AI
    void DoStartDialogue(uint32 uiEntry)
    {
        StartNextDialogueText(uiEntry);
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        switch (iEntry)
        {
            case NPC_KALECGOS:
                if (Creature* pKalec = m_pInstance->GetSingleCreatureFromStorage(NPC_KALECGOS))
                {
                    pKalec->CastSpell(pKalec, SPELL_KALEC_TELEPORT, true);
                    pKalec->SetLevitate(false);
                }
                m_creature->SummonCreature(NPC_CORE_ENTROPIUS, m_creature->GetPositionX(), m_creature->GetPositionY(), 85.0f, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                break;
            case NPC_BOSS_PORTAL:
                // ToDo: summon soldiers to the right
                m_creature->SummonCreature(NPC_BOSS_PORTAL, aOutroLocations[0].m_fX, aOutroLocations[0].m_fY, aOutroLocations[0].m_fZ, aOutroLocations[0].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0);
                break;
            case POINT_SUMMON_SOLDIERS:
                // ToDo: summon soldiers to the left
                break;
            case NPC_VELEN:
                m_creature->SummonCreature(NPC_VELEN, aOutroLocations[1].m_fX, aOutroLocations[1].m_fY, aOutroLocations[1].m_fZ, aOutroLocations[1].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0);
                break;
            case NPC_LIADRIN:
                m_creature->SummonCreature(NPC_LIADRIN, aOutroLocations[2].m_fX, aOutroLocations[2].m_fY, aOutroLocations[2].m_fZ, aOutroLocations[2].m_fO, TEMPSUMMON_TIMED_DESPAWN, 4*MINUTE*IN_MILLISECONDS);
                break;
            case SPELL_CALL_ENTROPIUS:
                if (Creature* pVelen = m_pInstance->GetSingleCreatureFromStorage(NPC_VELEN))
                    pVelen->CastSpell(pVelen, SPELL_CALL_ENTROPIUS, false);
                // Set point id = 1 for movement event
                if (Creature* pEntropius = m_creature->GetMap()->GetCreature(m_EntropiusGuid))
                {
                    pEntropius->SetWalk(false);
                    pEntropius->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), 35.0f);
                }
                break;
            case POINT_MOVE_LIADRIN:
                if (Creature* pLiadrin = m_pInstance->GetSingleCreatureFromStorage(NPC_LIADRIN))
                    pLiadrin->GetMotionMaster()->MovePoint(0, aOutroLocations[4].m_fX, aOutroLocations[4].m_fY, aOutroLocations[4].m_fZ);
                break;
            case SPELL_BLAZE_TO_LIGHT:
                if (Creature* pEntropius = m_creature->GetMap()->GetCreature(m_EntropiusGuid))
                {
                    pEntropius->CastSpell(pEntropius, SPELL_BLAZE_TO_LIGHT, true);
                    pEntropius->RemoveAurasDueToSpell(SPELL_ENTROPIUS_BODY);
                    pEntropius->SetWalk(true);
                    pEntropius->GetMotionMaster()->MovePoint(2, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                }
                break;
            case POINT_EVENT_EXIT:
                // Set point id = 1 for the despawn event
                if (Creature* pVelen = m_pInstance->GetSingleCreatureFromStorage(NPC_VELEN))
                    pVelen->GetMotionMaster()->MovePoint(1, aOutroLocations[1].m_fX, aOutroLocations[1].m_fY, aOutroLocations[1].m_fZ);
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_VELEN:
                pSummoned->GetMotionMaster()->MovePoint(0, aOutroLocations[3].m_fX, aOutroLocations[3].m_fY, aOutroLocations[3].m_fZ);
                // no break here
            case NPC_LIADRIN:
                pSummoned->CastSpell(pSummoned, SPELL_TELEPORT_VISUAL, true);
                break;
            case NPC_CORE_ENTROPIUS:
                pSummoned->CastSpell(pSummoned, SPELL_ENTROPIUS_BODY, true);
                pSummoned->SetLevitate(true);
                m_EntropiusGuid = pSummoned->GetObjectGuid();
                break;
            case NPC_BOSS_PORTAL:
                m_PortalGuid = pSummoned->GetObjectGuid();
                break;
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == 1)
        {
            if (pSummoned->GetEntry() == NPC_CORE_ENTROPIUS)
            {
                // Interrupt Velen's casting when entropius has reached the ground
                if (Creature* pVelen = m_pInstance->GetSingleCreatureFromStorage(NPC_VELEN))
                    pVelen->InterruptNonMeleeSpells(false);
            }
            else if (pSummoned->GetEntry() == NPC_VELEN)
            {
                // Cast teleport and despawn Velen, the portal and Kalec; Liadrin will despawn on timer
                pSummoned->CastSpell(pSummoned, SPELL_TELEPORT_VISUAL, true);
                pSummoned->ForcedDespawn(1000);

                // Note: portal should despawn only after all the soldiers have reached this point and "teleported" outside
                if (Creature* pPortal = m_creature->GetMap()->GetCreature(m_PortalGuid))
                    pPortal->ForcedDespawn(5000);

                if (Creature* pKalec = m_pInstance->GetSingleCreatureFromStorage(NPC_KALECGOS))
                    pKalec->ForcedDespawn(1000);
            }
        }
        else if (uiPointId == 2 && pSummoned->GetEntry() == NPC_CORE_ENTROPIUS)
        {
            // When the purified Muru reaches the ground the sunwell ignites and Muru despawns
            DoCastSpellIfCan(m_creature, SPELL_SUNWELL_IGNITION);

            if (Creature* pLiadrin = m_pInstance->GetSingleCreatureFromStorage(NPC_LIADRIN))
                pLiadrin->SetStandState(UNIT_STAND_STATE_KNEEL);

            pSummoned->ForcedDespawn();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        DialogueUpdate(uiDiff);
    }
};

struct MANGOS_DLL_DECL boss_kiljaedenAI : public Scripted_NoMovementAI
{
    boss_kiljaedenAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = ((instance_sunwell_plateau*)pCreature->GetInstanceData());
        Reset();
    }

    instance_sunwell_plateau* m_pInstance;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);

        DoScriptText(SAY_EMERGE, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_BIRTH);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KILJAEDEN, FAIL);

            if (Creature* pKiljaedenController = m_pInstance->GetSingleCreatureFromStorage(NPC_KILJAEDEN_CONTROLLER))
                pKiljaedenController->CastSpell(pKiljaedenController, SPELL_ANVEENA_DRAIN, true);
        }

        // Despawn on wipe
        m_creature->ForcedDespawn();
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KILJAEDEN, DONE);

            // Start the outro
            if (Creature* pKiljaedenController = m_pInstance->GetSingleCreatureFromStorage(NPC_KILJAEDEN_CONTROLLER))
            {
                if (npc_kiljaeden_controllerAI* pControllerAI = dynamic_cast<npc_kiljaeden_controllerAI*>(pKiljaedenController->AI()))
                    pControllerAI->DoStartDialogue(NPC_KALECGOS);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kiljaeden(Creature *pCreature)
{
    return new boss_kiljaedenAI(pCreature);
}

CreatureAI* GetAI_npc_kiljaeden_controller(Creature *pCreature)
{
    return new npc_kiljaeden_controllerAI(pCreature);
}

void AddSC_boss_kiljaeden()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name="boss_kiljaeden";
    pNewScript->GetAI = &GetAI_boss_kiljaeden;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name="npc_kiljaeden_controller";
    pNewScript->GetAI = &GetAI_npc_kiljaeden_controller;
    pNewScript->RegisterSelf();
}

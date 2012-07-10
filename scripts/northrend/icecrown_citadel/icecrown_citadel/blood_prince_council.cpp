/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: blood_prince_council
SD%Complete: 10%
SDComment: Only intro event is implemented.
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // Yells
    SAY_COUNCIL_INTRO_1         = -1631101,                 // Intro by Bloodqueen
    SAY_COUNCIL_INTRO_2         = -1631102,

    SAY_KELESETH_INVOCATION     = -1631103,
    SAY_KELESETH_SPECIAL        = -1631104,
    SAY_KELESETH_SLAY_1         = -1631105,
    SAY_KELESETH_SLAY_2         = -1631106,
    SAY_KELESETH_BERSERK        = -1631107,
    SAY_KELESETH_DEATH          = -1631108,

    SAY_TALDARAM_INVOCATION     = -1631109,
    SAY_TALDARAM_SPECIAL        = -1631110,
    SAY_TALDARAM_SLAY_1         = -1631111,
    SAY_TALDARAM_SLAY_2         = -1631112,
    SAY_TALDARAM_BERSERK        = -1631113,
    SAY_TALDARAM_DEATH          = -1631114,

    SAY_VALANAR_INVOCATION      = -1631115,
    SAY_VALANAR_SPECIAL         = -1631116,
    SAY_VALANAR_SLAY_1          = -1631117,
    SAY_VALANAR_SLAY_2          = -1631118,
    SAY_VALANAR_BERSERK         = -1631119,
    SAY_VALANAR_DEATH           = -1631120,

    // Generic spells
    SPELL_BERSERK               = 26662,
    SPELL_FEIGN_DEATH           = 71598,

    SPELL_INVOCATION_BLOOD      = 70934,
    SPELL_INVOCATION_BLOOD_2    = 71596,
    SPELL_INVOCATION_V_MOVE     = 71075,
    SPELL_INVOCATION_K_MOVE     = 71079,
    SPELL_INVOCATION_T_MOVE     = 71082,

    // Valanar spells
    SPELL_INVOCATION_VALANAR    = 70952,
    SPELL_KINETIC_BOMB_TARGET   = 72053,            // summons 38458 - the target of the bomb
    SPELL_KINETIC_BOMB          = 72080,            // summons 38454
    SPELL_SHOCK_VORTEX          = 72037,            // summons 38422

    // shock vortex spells
    SPELL_SHOCK_VORTEX_AURA     = 71945,
    SPELL_SHOCK_VORTEX_VISUAL   = 72633,
    SPELL_EMP_SHOCK_VORTEX      = 72039,

    // kinetic bomb spells
    SPELL_KINETIC_BOMB_DMG      = 72052,
    SPELL_KINETIC_BOMB_VISUAL   = 72054,
    SPELL_UNSTABLE              = 72059,            // procs 72087

    // Keleseth spells
    SPELL_INVOCATION_KELESETH   = 70981,
    SPELL_SHADOW_LANCE          = 71405,
    SPELL_EMP_SHADOW_LANCE      = 71815,
    SPELL_SHADOW_RESONANCE      = 71943,            // summons 38369
    SPELL_SHADOW_PRISON         = 73001,            // on heroic

    // dark nucleus spells
    SPELL_SHADOW_RESONANCE_AURA = 71911,            // purpose unk - maybe range check
    SPELL_SHADOW_RESONANCE_BUFF = 71822,            // channeled from the dark nucleus

    // Taldaram spells
    SPELL_INVOCATION_TALDARAM   = 70982,
    SPELL_GLITTERING_SPARKS     = 71806,            // triggers 71807
    SPELL_CONJURE_FLAME         = 71718,            // triggers 71719 which summons 38332
    SPELL_CONJURE_EMP_FLAME     = 72040,            // triggers 72041 which summons 38451

    // ball of flame spells
    SPELL_BALL_FLAMES_VISUAL    = 71706,
    SPELL_FLAMES                = 71393,            // cast on the impact
    SPELL_BALL_FLAMES_PERIODIC  = 71709,            // triggers 71708
    SPELL_FLAMES_PROC           = 71756,
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_COUNCIL_INTRO_1,   NPC_LANATHEL_INTRO, 15000},
    {SAY_COUNCIL_INTRO_2,   NPC_LANATHEL_INTRO, 10000},
    {NPC_BLOOD_ORB_CONTROL, 0,                  0},
    {0, 0, 0},
};

static const float aLanathelFlyPos[3] = {4660.49f, 2769.2f, 430.0f};

/*######
## npc_queen_lanathel_intro
######*/

struct MANGOS_DLL_DECL npc_queen_lanathel_introAI : public ScriptedAI, private DialogueHelper
{
    npc_queen_lanathel_introAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aIntroDialogue)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        InitializeDialogueHelper(m_pInstance);
        m_bEventStarted = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bEventStarted;

    void Reset()
    {
        // Flying animation
        m_creature->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // The range distance is not sure
        if (!m_bEventStarted && pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster() &&
            pWho->IsWithinDistInMap(m_creature, 100.0f) && pWho->IsWithinLOSInMap(m_creature))
        {
            StartNextDialogueText(SAY_COUNCIL_INTRO_1);
            m_bEventStarted = true;
        }
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        switch (iEntry)
        {
            case SAY_COUNCIL_INTRO_2:
                m_creature->GetMotionMaster()->MovePoint(1, aLanathelFlyPos[0], aLanathelFlyPos[1], aLanathelFlyPos[2]);
                break;
            case NPC_BLOOD_ORB_CONTROL:
                if (m_pInstance)
                {
                    if (Creature* pTaldaram = m_pInstance->GetSingleCreatureFromStorage(NPC_TALDARAM))
                    {
                        pTaldaram->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        pTaldaram->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                        pTaldaram->SetStandState(UNIT_STAND_STATE_STAND);
                        pTaldaram->HandleEmote(EMOTE_ONESHOT_ROAR);
                    }
                    if (Creature* pKeleseth = m_pInstance->GetSingleCreatureFromStorage(NPC_KELESETH))
                    {
                        pKeleseth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        pKeleseth->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                        pKeleseth->SetStandState(UNIT_STAND_STATE_STAND);
                        pKeleseth->HandleEmote(EMOTE_ONESHOT_ROAR);
                    }
                    if (Creature* pValanar = m_pInstance->GetSingleCreatureFromStorage(NPC_VALANAR))
                    {
                        pValanar->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        pValanar->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                        pValanar->SetStandState(UNIT_STAND_STATE_STAND);
                        pValanar->HandleEmote(EMOTE_ONESHOT_ROAR);
                    }
                }
                break;
        }
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE || !uiPointId)
            return;

        // Despawn when reached point
        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff) { DialogueUpdate(uiDiff); }
};

CreatureAI* GetAI_npc_queen_lanathel_intro(Creature* pCreature)
{
    return new npc_queen_lanathel_introAI(pCreature);
}

void AddSC_blood_prince_council()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_queen_lanathel_intro";
    pNewScript->GetAI = &GetAI_npc_queen_lanathel_intro;
    pNewScript->RegisterSelf();
}

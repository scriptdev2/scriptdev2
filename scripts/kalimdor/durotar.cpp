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
SDName: Durotar
SD%Complete: 100
SDComment: Quest support: 5441
SDCategory: Durotar
EndScriptData */

/* ContentData
npc_lazy_peon
EndContentData */

#include "precompiled.h"

/*######
## npc_lazy_peon
######*/

enum
{
    SAY_PEON_AWAKE_1        = -1000795,
    SAY_PEON_AWAKE_2        = -1000796,

    SPELL_PEON_SLEEP        = 17743,
    SPELL_AWAKEN_PEON       = 19938,

    NPC_SLEEPING_PEON       = 10556,
    GO_LUMBERPILE           = 175784,
};

struct MANGOS_DLL_DECL npc_lazy_peonAI : public ScriptedAI
{
    npc_lazy_peonAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset (); }

    uint32 m_uiResetSleepTimer;

    void Reset()
    {
        m_uiResetSleepTimer = 0;
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE || !uiPointId)
            return;

        m_creature->HandleEmote(EMOTE_STATE_WORK_CHOPWOOD);
        m_uiResetSleepTimer = 80000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiResetSleepTimer)
        {
            if (m_uiResetSleepTimer <= uiDiff)
            {
                m_creature->HandleEmote(EMOTE_STATE_NONE);
                EnterEvadeMode();
                m_uiResetSleepTimer = 0;
            }
            else
                m_uiResetSleepTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_lazy_peon(Creature* pCreature)
{
    return new npc_lazy_peonAI(pCreature);
}

bool EffectDummyCreature_lazy_peon_awake(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_AWAKEN_PEON && uiEffIndex == EFFECT_INDEX_0)
    {
        if (!pCreatureTarget->HasAura(SPELL_PEON_SLEEP) || pCaster->GetTypeId() != TYPEID_PLAYER || pCreatureTarget->GetEntry() != NPC_SLEEPING_PEON)
            return true;

        if (GameObject* pLumber = GetClosestGameObjectWithEntry(pCreatureTarget, GO_LUMBERPILE, 15.0f))
        {
            pCreatureTarget->RemoveAurasDueToSpell(SPELL_PEON_SLEEP);
            DoScriptText(urand(0, 1) ? SAY_PEON_AWAKE_1 : SAY_PEON_AWAKE_2, pCreatureTarget);
            ((Player*)pCaster)->KilledMonsterCredit(pCreatureTarget->GetEntry());

            float fX, fY, fZ;
            pCreatureTarget->SetWalk(false);
            pLumber->GetContactPoint(pCreatureTarget, fX, fY, fZ, CONTACT_DISTANCE);
            pCreatureTarget->GetMotionMaster()->MovePoint(1, fX, fY, fZ);
        }
        else
            error_log("SD2: No GameObject of entry %u was found in range or something really bad happened.", GO_LUMBERPILE);

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

void AddSC_durotar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_lazy_peon";
    pNewScript->GetAI = &GetAI_npc_lazy_peon;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_lazy_peon_awake;
    pNewScript->RegisterSelf();
}

/* Copyright (C) 2006 - 2013 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: boss_xt_002
SD%Complete: 20%
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                           = -1603045,
    SAY_SLAY_1                          = -1603046,
    SAY_SLAY_2                          = -1603047,
    SAY_BERSERK                         = -1603048,
    SAY_ADDS                            = -1603049,
    SAY_DEATH                           = -1603050,
    SAY_HEART_OPEN                      = -1603051,
    SAY_HEART_CLOSE                     = -1603052,
    SAY_TANCTRUM                        = -1603053,

    EMOTE_HEART                         = -1603054,
    EMOTE_REPAIR                        = -1603055,

    // spells
    SPELL_TANCTRUM                      = 62776,
    SPELL_SEARING_LIGHT                 = 63018,
    SPELL_SEARING_LIGHT_H               = 65121,
    SPELL_GRAVITY_BOMB                  = 63024,
    SPELL_GRAVITY_BOMB_H                = 64234,
    SPELL_BERSERK                       = 26662,

    // hard mode spells
    SPELL_HEARTBREAK                    = 65737,
    SPELL_HEARTBREAK_H                  = 64193,
    SPELL_VOIDZONE                      = 64203,
    SPELL_VOIDZONE_H                    = 64235,
    SPELL_LIFE_SPARK                    = 64210,

    // heart of XT002 spells
    SPELL_HEART_RIDE_VEHICLE            = 63852,            // ride seat 1 - procs on damage (probably spell 17683)
    SPELL_RIDE_VEHICLE                  = 63313,            // ride seat 2
    SPELL_LIGHTNING_TETHER              = 64799,            // dummy, triggers 62789 and 63849
    SPELL_HEART_OVERLOAD                = 62789,            // triggers missing spell 62791 - used to spawn adds
    SPELL_EXPOSED_HEART                 = 63849,            // procs on damage
    SPELL_ENERGY_ORB                    = 62790,            // targets 33337, in order to start spawning robots

    // robot summoning spells
    SPELL_RECHARGE_ROBOT_1              = 62828,            // summons 33343
    SPELL_RECHARGE_ROBOT_2              = 62835,            // summons 33346
    SPELL_RECHARGE_ROBOT_3              = 62831,            // summons 33344

    // summoned spells
    SPELL_CONSUMPTION                   = 64208,            // cast by the void zone
    SPELL_ARCANE_POWER_STATE            = 49411,            // cast by the life spark
    SPELL_STATIC_CHARGED                = 64227,            // cast by the life spark (needs to be confirmed)
    SPELL_STATIC_CHARGED_H              = 64236,

    // NPC ids
    NPC_SCRAPBOT                        = 33343,
    NPC_BOOMBOT                         = 33346,
    NPC_PUMMELLER                       = 33344,
    NPC_VOIDZONE                        = 34001,
    NPC_LIFE_SPARK                      = 34004,
    NPC_XT_TOY_PILE                     = 33337,
};

/*######
## boss_xt_002
######*/

struct MANGOS_DLL_DECL boss_xt_002AI : public ScriptedAI
{
    boss_xt_002AI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_uiMountTimer = 1000;
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBerserkTimer;
    uint32 m_uiMountTimer;

    void Reset() override
    {
        m_uiBerserkTimer = 10 * MINUTE * IN_MILLISECONDS;
    }

    void JustDied(Unit* pKiller) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_XT002, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho) override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_XT002, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_XT002, FAIL);
    }

    void JustSummoned(Creature* pSummoned) override
    {
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_pInstance)
            return;

        // The heart needs to be mounted manually, not by vehicle_accessories
        if (m_uiMountTimer)
        {
            if (m_uiMountTimer <= uiDiff)
            {
                if (Creature* pHeart = m_pInstance->GetSingleCreatureFromStorage(NPC_HEART_DECONSTRUCTOR))
                    pHeart->CastSpell(m_creature, SPELL_HEART_RIDE_VEHICLE, true);

                m_uiMountTimer = 0;
            }
            else
                m_uiMountTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    m_uiBerserkTimer = 0;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_xt_002(Creature* pCreature)
{
    return new boss_xt_002AI(pCreature);
}

void AddSC_boss_xt_002()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_xt_002";
    pNewScript->GetAI = GetAI_boss_xt_002;
    pNewScript->RegisterSelf();
}

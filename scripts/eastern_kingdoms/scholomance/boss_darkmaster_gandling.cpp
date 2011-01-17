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
SDName: Boss_Darkmaster_Gandling
SD%Complete: 75
SDComment: TODO: Implement teleport spells in MaNGOS and WorldDB
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"
#include "scholomance.h"

enum
{
    SPELL_ARCANE_MISSILES          = 15790,                 // SpellId not sure, original was 22272
    SPELL_SHADOW_SHIELD            = 12040,                 // SpellID not sure, original was 22417 stated as "wrong, but 12040 is wrong either."
    SPELL_CURSE                    = 18702,
    SPELL_SHADOW_PORTAL            = 17950                  // TODO implement this spell(and other related port spells) in DB and MaNGOS
};

struct MANGOS_DLL_DECL boss_darkmaster_gandlingAI : public ScriptedAI
{
    boss_darkmaster_gandlingAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiArcaneMissilesTimer;
    uint32 m_uiShadowShieldTimer;
    uint32 m_uiCurseTimer;
    uint32 m_uiTeleportTimer;

    Creature *Summoned;

    void Reset()
    {
        m_uiArcaneMissilesTimer = 4500;
        m_uiShadowShieldTimer = 12000;
        m_uiCurseTimer = 2000;
        m_uiTeleportTimer = 16000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Arcane Missiles Timer
        if (m_uiArcaneMissilesTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_MISSILES) == CAST_OK)
                m_uiArcaneMissilesTimer = 8000;
        }
        else
            m_uiArcaneMissilesTimer -= uiDiff;

        // Shadow Shield Timer
        if (m_uiShadowShieldTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_SHIELD) == CAST_OK)
                m_uiShadowShieldTimer = urand(14000, 28000);
        }
        else
            m_uiShadowShieldTimer -= uiDiff;

        // Curse Timer
        if (m_uiCurseTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CURSE) == CAST_OK)
                m_uiCurseTimer = urand(15000, 27000);
        }
        else
            m_uiCurseTimer -= uiDiff;

        // Teleporting Random Target to one of the six pre boss rooms and spawn 3-4 skeletons near the gamer.
        // We will only telport if gandling has more than 3% of hp so teleported gamers can always loot.
        if (m_creature->GetHealthPercent() > 3.0f)
        {
            if (m_uiTeleportTimer < uiDiff)
            {
                Unit* target = NULL;
                target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                if (target && target->GetTypeId() == TYPEID_PLAYER)
                {
                    if (m_creature->getThreatManager().getThreat(target))
                        m_creature->getThreatManager().modifyThreatPercent(target, -100);

                    switch(urand(0, 5))
                    {
                        case 0:
                            DoTeleportPlayer(target, 250.0696f, 0.3921f, 84.8408f, 3.149f);
                            Summoned = m_creature->SummonCreature(16119, 254.2325f, 0.3417f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 257.7133f, 4.0226f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 258.6702f, -2.60656f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            break;
                        case 1:
                            DoTeleportPlayer(target, 181.4220f, -91.9481f, 84.8410f, 1.608f);
                            Summoned = m_creature->SummonCreature(16119, 184.0519f, -73.5649f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 179.5951f, -73.7045f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 180.6452f, -78.2143f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 283.2274f, -78.1518f, 84.8407f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            break;
                        case 2:
                            DoTeleportPlayer(target, 95.1547f, -1.8173f, 85.2289f, 0.043f);
                            Summoned = m_creature->SummonCreature(16119, 100.9404f, -1.8016f, 85.2289f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 101.3729f, 0.4882f, 85.2289f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 101.4596f, -4.4740f, 85.2289f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            break;
                        case 3:
                            DoTeleportPlayer(target, 250.0696f, 0.3921f, 72.6722f, 3.149f);
                            Summoned = m_creature->SummonCreature(16119, 240.34481f, 0.7368f, 72.6722f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 240.3633f, -2.9520f, 72.6722f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 240.6702f, 3.34949f, 72.6722f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            break;
                        case 4:
                            DoTeleportPlayer(target, 181.4220f, -91.9481f, 70.7734f, 1.608f);
                            Summoned = m_creature->SummonCreature(16119, 184.0519f, -73.5649f, 70.7734f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 179.5951f, -73.7045f, 70.7734f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 180.6452f, -78.2143f, 70.7734f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 283.2274f, -78.1518f, 70.7734f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            break;
                        case 5:
                            DoTeleportPlayer(target, 106.1541f, -1.8994f, 75.3663f, 0.043f);
                            Summoned = m_creature->SummonCreature(16119, 115.3945f, -1.5555f, 75.3663f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 257.7133f, 1.8066f, 75.3663f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            Summoned = m_creature->SummonCreature(16119, 258.6702f, -5.1001f, 75.3663f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,10000);
                            if (Summoned)
                                Summoned->AI()->AttackStart(target);
                            break;
                    }
                }
                m_uiTeleportTimer = urand(20000, 35000);
            }
            else
                m_uiTeleportTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_darkmaster_gandling(Creature* pCreature)
{
    return new boss_darkmaster_gandlingAI(pCreature);
}

void AddSC_boss_darkmaster_gandling()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_darkmaster_gandling";
    pNewScript->GetAI = &GetAI_boss_darkmaster_gandling;
    pNewScript->RegisterSelf();
}

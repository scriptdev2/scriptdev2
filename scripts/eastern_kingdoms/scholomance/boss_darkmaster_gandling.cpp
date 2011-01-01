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
SDComment: Doors missing
SDCategory: Scholomance
EndScriptData */

#include "precompiled.h"
#include "scholomance.h"

#define SPELL_ARCANEMISSILES           22272
#define SPELL_SHADOWSHIELD             22417                //Not right ID. But 12040 is wrong either.
#define SPELL_CURSE                    18702

#define ADD_1X 170.205
#define ADD_1Y 99.413
#define ADD_1Z 104.733
#define ADD_1O 3.16

#define ADD_2X 170.813
#define ADD_2Y 97.857
#define ADD_2Z 104.713
#define ADD_2O 3.16

#define ADD_3X 170.720
#define ADD_3Y 100.900
#define ADD_3Z 104.739
#define ADD_3O 3.16

#define ADD_4X 171.866
#define ADD_4Y 99.373
#define ADD_4Z 104.732
#define ADD_4O 3.16

struct MANGOS_DLL_DECL boss_darkmaster_gandlingAI : public ScriptedAI
{
    boss_darkmaster_gandlingAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 ArcaneMissiles_Timer;
    uint32 ShadowShield_Timer;
    uint32 Curse_Timer;
    uint32 Teleport_Timer;

    Creature *Summoned;

    void Reset()
    {
        ArcaneMissiles_Timer = 4500;
        ShadowShield_Timer = 12000;
        Curse_Timer = 2000;
        Teleport_Timer = 16000;
    }

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GANDLING, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //ArcaneMissiles_Timer
        if (ArcaneMissiles_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ARCANEMISSILES);
            ArcaneMissiles_Timer = 8000;
        }else ArcaneMissiles_Timer -= diff;

        //ShadowShield_Timer
        if (ShadowShield_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SHADOWSHIELD);
            ShadowShield_Timer = urand(14000, 28000);
        }else ShadowShield_Timer -= diff;

        //Curse_Timer
        if (Curse_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CURSE);
            Curse_Timer = urand(15000, 27000);
        }else Curse_Timer -= diff;

        //Teleporting Random Target to one of the six pre boss rooms and spawn 3-4 skeletons near the gamer.
        //We will only telport if gandling has more than 3% of hp so teleported gamers can always loot.
        if (m_creature->GetHealthPercent() > 3.0f)
        {
            if (Teleport_Timer < diff)
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
                Teleport_Timer = urand(20000, 35000);
            }else Teleport_Timer -= diff;
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
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_darkmaster_gandling";
    newscript->GetAI = &GetAI_boss_darkmaster_gandling;
    newscript->RegisterSelf();
}

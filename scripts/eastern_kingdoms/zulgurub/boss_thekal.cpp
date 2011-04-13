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
SDName: Boss_Thekal
SD%Complete: 95
SDComment: Almost finished.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SAY_AGGRO               = -1309009,
    SAY_DEATH               = -1309010,

    SPELL_MORTALCLEAVE      = 22859,
    SPELL_SILENCE           = 23207,
    SPELL_FRENZY            = 23342,
    SPELL_FORCEPUNCH        = 24189,
    SPELL_CHARGE            = 24408,
    SPELL_ENRAGE            = 23537,
    SPELL_SUMMONTIGERS      = 24183,
    SPELL_TIGER_FORM        = 24169,
    SPELL_RESURRECT         = 24173,                     //We will not use this spell.

    // Zealot Lor'Khan Spells
    SPELL_SHIELD            = 25020,
    SPELL_BLOODLUST         = 24185,
    SPELL_GREATERHEAL       = 24208,
    SPELL_DISARM            = 22691,

    //Zealot Lor'Khan Spells
    SPELL_SWEEPINGSTRIKES   = 18765,
    SPELL_SINISTERSTRIKE    = 15667,
    SPELL_GOUGE             = 24698,
    SPELL_KICK              = 15614,
    SPELL_BLIND             = 21060
};

struct MANGOS_DLL_DECL boss_thekalAI : public ScriptedAI
{
    boss_thekalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiMortalCleaveTimer;
    uint32 m_uiSilenceTimer;
    uint32 m_uiFrenzyTimer;
    uint32 m_uiForcePunchTimer;
    uint32 m_uiChargeTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSummonTigersTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiResurrectTimer;

    bool m_bEnraged;
    bool m_bPhaseTwo;
    bool m_bWasDead;

    void Reset()
    {
        m_uiMortalCleaveTimer   = 4000;
        m_uiSilenceTimer        = 9000;
        m_uiFrenzyTimer         = 30000;
        m_uiForcePunchTimer     = 4000;
        m_uiChargeTimer         = 12000;
        m_uiEnrageTimer         = 32000;
        m_uiSummonTigersTimer   = 25000;
        m_uiCheckTimer          = 10000;
        m_uiResurrectTimer      = 10000;

        m_bEnraged              = false;
        m_bPhaseTwo             = false;
        m_bWasDead              = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, NOT_STARTED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Check_Timer for the death of LorKhan and Zath.
        if (!m_bWasDead && m_uiCheckTimer < uiDiff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                {
                    //Resurrect LorKhan
                    if (Creature *pLorKhan = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LORKHAN)))
                    {
                        pLorKhan->SetStandState(UNIT_STAND_STATE_STAND);
                        pLorKhan->setFaction(14);
                        pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pLorKhan->SetHealth(int(pLorKhan->GetMaxHealth()*1.0));

                        m_pInstance->SetData(TYPE_LORKHAN, DONE);
                    }
                }

                if (m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
                {
                    //Resurrect Zath
                    if (Creature *pZath = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZATH)))
                    {
                        pZath->SetStandState(UNIT_STAND_STATE_STAND);
                        pZath->setFaction(14);
                        pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));

                        m_pInstance->SetData(TYPE_ZATH, DONE);
                    }
                }
            }
            m_uiCheckTimer = 5000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (!m_bPhaseTwo && m_uiMortalCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MORTALCLEAVE);
            m_uiMortalCleaveTimer = urand(15000, 20000);
        }
        else
            m_uiMortalCleaveTimer -= uiDiff;

        if (!m_bPhaseTwo && m_uiSilenceTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SILENCE);
            m_uiSilenceTimer = urand(20000, 25000);
        }
        else
            m_uiSilenceTimer -= uiDiff;

        if (!m_bPhaseTwo && !m_bWasDead && m_creature->GetHealthPercent() < 5.0f)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
            m_creature->AttackStop();

            if (m_pInstance)
                m_pInstance->SetData(TYPE_THEKAL, SPECIAL);

            m_bWasDead = true;
        }

        //Thekal will transform to Tiger if he died and was not resurrected after 10 seconds.
        if (!m_bPhaseTwo && m_bWasDead)
        {
            if (m_uiResurrectTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature,SPELL_TIGER_FORM);
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.00f);
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetHealth(int(m_creature->GetMaxHealth()*1.0));
                const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 40)));
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 40)));
                m_creature->UpdateDamagePhysical(BASE_ATTACK);
                DoResetThreat();
                m_bPhaseTwo = true;
            }
            else
                m_uiResurrectTimer -= uiDiff;
        }

        if (m_creature->GetHealthPercent() == 100.0f && m_bWasDead)
        {
            m_bWasDead = false;
        }

        if (m_bPhaseTwo)
        {
            if (m_uiChargeTimer < uiDiff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                {
                    DoCastSpellIfCan(target,SPELL_CHARGE);
                    DoResetThreat();
                    AttackStart(target);
                }
                m_uiChargeTimer = urand(15000, 22000);
            }
            else
                m_uiChargeTimer -= uiDiff;

            if (m_uiFrenzyTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature,SPELL_FRENZY);
                m_uiFrenzyTimer = 30000;
            }
            else
                m_uiFrenzyTimer -= uiDiff;

            if (m_uiForcePunchTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SILENCE);
                m_uiForcePunchTimer = urand(16000, 21000);
            }
            else
                m_uiForcePunchTimer -= uiDiff;

            if (m_uiSummonTigersTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SUMMONTIGERS);
                m_uiSummonTigersTimer = urand(10000, 14000);
            }
            else
                m_uiSummonTigersTimer -= uiDiff;

            if (m_creature->GetHealthPercent() < 11.0f && !m_bEnraged)
            {
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                m_bEnraged = true;
            }
        }

        if (m_creature->getVictim())                        // TODO - use correct check here, this only prevents crash
            DoMeleeAttackIfReady();
    }
};

/*######
## mob_zealot_lorkhan
######*/

struct MANGOS_DLL_DECL mob_zealot_lorkhanAI : public ScriptedAI
{
    mob_zealot_lorkhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShieldTimer;
    uint32 m_uiBloodLustTimer;
    uint32 m_uiGreaterHealTimer;
    uint32 m_uiDisarmTimer;
    uint32 m_uiCheckTimer;

    bool m_bFakeDeath;

    void Reset()
    {
        m_uiShieldTimer         = 1000;
        m_uiBloodLustTimer      = 16000;
        m_uiGreaterHealTimer    = 32000;
        m_uiDisarmTimer         = 6000;
        m_uiCheckTimer          = 10000;

        m_bFakeDeath            = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LORKHAN, NOT_STARTED);

        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI (const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Shield_Timer
        if (m_uiShieldTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SHIELD);
            m_uiShieldTimer = 61000;
        }
        else
            m_uiShieldTimer -= uiDiff;

        //BloodLust_Timer
        if (m_uiBloodLustTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BLOODLUST);
            m_uiBloodLustTimer = urand(20000, 28000);
        }
        else
            m_uiBloodLustTimer -= uiDiff;

        //Casting Greaterheal to Thekal or Zath if they are in meele range.
        // TODO - why this range check?
        if (m_uiGreaterHealTimer < uiDiff)
        {
            if (m_pInstance)
            {
                Creature* pThekal = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THEKAL));
                Creature* pZath = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZATH));

                switch(urand(0, 1))
                {
                    case 0:
                        if (pThekal && m_creature->IsWithinDistInMap(pThekal, ATTACK_DISTANCE))
                            DoCastSpellIfCan(pThekal, SPELL_GREATERHEAL);
                        break;
                    case 1:
                        if (pZath && m_creature->IsWithinDistInMap(pZath, ATTACK_DISTANCE))
                            DoCastSpellIfCan(pZath, SPELL_GREATERHEAL);
                        break;
                }
            }

            m_uiGreaterHealTimer = urand(15000, 20000);
        }
        else
            m_uiGreaterHealTimer -= uiDiff;

        //Disarm_Timer
        if (m_uiDisarmTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_DISARM);
            m_uiDisarmTimer = urand(15000, 25000);
        }
        else
            m_uiDisarmTimer -= uiDiff;

        //Check_Timer for the death of LorKhan and Zath.
        if (!m_bFakeDeath && m_uiCheckTimer < uiDiff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
                {
                    //Resurrect Thekal
                    if (Creature* pThekal = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THEKAL)))
                    {
                        pThekal->SetStandState(UNIT_STAND_STATE_STAND);
                        pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pThekal->setFaction(14);
                        pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    }
                }

                if (m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
                {
                    //Resurrect Zath
                    if (Creature* pZath = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZATH)))
                    {
                        pZath->SetStandState(UNIT_STAND_STATE_STAND);
                        pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pZath->setFaction(14);
                        pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));
                    }
                }
            }

            m_uiCheckTimer = 5000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 5.0f)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
            m_creature->setFaction(35);
            m_creature->AttackStop();

            if (m_pInstance)
                m_pInstance->SetData(TYPE_LORKHAN, SPECIAL);

            m_bFakeDeath = true;
        }

        if (!m_bFakeDeath)
            DoMeleeAttackIfReady();
    }
};

/*######
## npc_zealot_zath
######*/

struct MANGOS_DLL_DECL mob_zealot_zathAI : public ScriptedAI
{
    mob_zealot_zathAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSweepingStrikesTimer;
    uint32 m_uiSinisterStrikeTimer;
    uint32 m_uiGougeTimer;
    uint32 m_uiKickTimer;
    uint32 m_uiBlindTimer;
    uint32 m_uiCheckTimer;

    bool m_bFakeDeath;

    void Reset()
    {
        m_uiSweepingStrikesTimer    = 13000;
        m_uiSinisterStrikeTimer     = 8000;
        m_uiGougeTimer              = 25000;
        m_uiKickTimer               = 18000;
        m_uiBlindTimer              = 5000;
        m_uiCheckTimer              = 10000;

        m_bFakeDeath                = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZATH, NOT_STARTED);

        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI (const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //SweepingStrikes_Timer
        if (m_uiSweepingStrikesTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SWEEPINGSTRIKES);
            m_uiSweepingStrikesTimer = urand(22000, 26000);
        }
        else
            m_uiSweepingStrikesTimer -= uiDiff;

        //SinisterStrike_Timer
        if (m_uiSinisterStrikeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SINISTERSTRIKE);
            m_uiSinisterStrikeTimer = urand(8000, 16000);
        }
        else
            m_uiSinisterStrikeTimer -= uiDiff;

        //Gouge_Timer
        if (m_uiGougeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_GOUGE);

            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-100);

            m_uiGougeTimer = urand(17000, 27000);
        }
        else
            m_uiGougeTimer -= uiDiff;

        //Kick_Timer
        if (m_uiKickTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KICK);
            m_uiKickTimer = urand(15000, 25000);
        }
        else
            m_uiKickTimer -= uiDiff;

        //Blind_Timer
        if (m_uiBlindTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_BLIND);
            m_uiBlindTimer = urand(10000, 20000);
        }
        else
            m_uiBlindTimer -= uiDiff;

        //Check_Timer for the death of LorKhan and Zath.
        if (!m_bFakeDeath && m_uiCheckTimer < uiDiff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                {
                    //Resurrect LorKhan
                    if (Creature* pLorKhan = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LORKHAN)))
                    {
                        pLorKhan->SetStandState(UNIT_STAND_STATE_STAND);
                        pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pLorKhan->setFaction(14);
                        pLorKhan->SetHealth(int(pLorKhan->GetMaxHealth()*1.0));
                    }
                }

                if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
                {
                    //Resurrect Thekal
                    if (Creature* pThekal = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THEKAL)))
                    {
                        pThekal->SetStandState(UNIT_STAND_STATE_STAND);
                        pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pThekal->setFaction(14);
                        pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    }
                }
            }

            m_uiCheckTimer = 5000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_creature->GetHealthPercent() <= 5.0f)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
            m_creature->setFaction(35);
            m_creature->AttackStop();

            if (m_pInstance)
                m_pInstance->SetData(TYPE_ZATH, SPECIAL);

            m_bFakeDeath = true;
        }

        if (!m_bFakeDeath)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thekal(Creature* pCreature)
{
    return new boss_thekalAI(pCreature);
}

CreatureAI* GetAI_mob_zealot_lorkhan(Creature* pCreature)
{
    return new mob_zealot_lorkhanAI(pCreature);
}

CreatureAI* GetAI_mob_zealot_zath(Creature* pCreature)
{
    return new mob_zealot_zathAI(pCreature);
}

void AddSC_boss_thekal()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_thekal";
    pNewScript->GetAI = &GetAI_boss_thekal;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_zealot_lorkhan";
    pNewScript->GetAI = &GetAI_mob_zealot_lorkhan;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_zealot_zath";
    pNewScript->GetAI = &GetAI_mob_zealot_zath;
    pNewScript->RegisterSelf();
}

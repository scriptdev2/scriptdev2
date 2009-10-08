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
SDName: Boss_Mandokir
SD%Complete: 99
SDComment: test Threating Gaze
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    NPC_OHGAN           = 14988,
    NPC_CHAINED_SPIRIT  = 15117,                            //resing spirits

    SAY_AGGRO           = -1309015,
    SAY_DING_KILL       = -1309016,
    SAY_GRATS_JINDO     = -1309017,
    SAY_WATCH           = -1309018,
    SAY_WATCH_WHISPER   = -1309019,

    EMOTE_RAGE          = -1309024,

    SPELL_CHARGE        = 24315,
    SPELL_CLEAVE        = 20691,
    SPELL_FEAR          = 29321,
    SPELL_GUILLOTINE    = 24316,
    SPELL_WHIRLWIND     = 24236,
    SPELL_MORTAL_STRIKE = 24573,
    SPELL_ENRAGE        = 23537,
    SPELL_WATCH         = 24314,
    SPELL_LEVEL_UP      = 24312,

    SPELL_MOUNT         = 23243,

    //Ohgans Spells
    SPELL_SUNDERARMOR   = 24317,

    //Chained Spirit Spells
    SPELL_REVIVE        = 24341
};

struct SpawnLocations
{
    float fX, fY, fZ, fAng;
};

static SpawnLocations aSpirits[]=
{
    {-12150.9, -1956.24, 133.407, 2.57835},
    {-12157.1, -1972.78, 133.947, 2.64903},
    {-12172.3, -1982.63, 134.061, 1.48664},
    {-12194,   -1979.54, 132.194, 1.45916},
    {-12211.3, -1978.49, 133.58,  1.35705},
    {-12228.4, -1977.1,  132.728, 1.25495},
    {-12250,   -1964.78, 135.066, 0.92901},
    {-12264,   -1953.08, 134.072, 0.626632},
    {-12289,   -1924,    132.62,  5.37829},
    {-12267.3, -1902.26, 131.328, 5.32724},
    {-12255.3, -1893.53, 134.026, 5.06413},
    {-12229.9, -1891.39, 134.704, 4.40047},
    {-12215.9, -1889.09, 137.273, 4.70285},
    {-12200.5, -1890.69, 135.777, 4.84422},
    {-12186,   -1890.12, 134.261, 4.36513},
    {-12246.3, -1890.09, 135.475, 4.73427},
    {-12170.7, -1894.85, 133.852, 3.5169},
    {-12279,   -1931.92, 136.13,  0.0415108},
    {-12266.1, -1940.72, 132.606, 0.7091}
};


struct MANGOS_DLL_DECL boss_mandokirAI : public ScriptedAI
{
    boss_mandokirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiWatch_Timer;
    uint32 m_uiCleave_Timer;
    uint32 m_uiWhirlwind_Timer;
    uint32 m_uiFear_Timer;
    uint32 m_uiMortalStrike_Timer;
    uint32 m_uiCheck_Timer;

    uint8 m_uiKillCount;
    uint8 m_uiTargetInRangeCount;

    float fTargetPositionX;
    float fTargetPositionY;
    float fTargetPositionZ;
    float fTargetsThreat;

    bool m_bEndWatch;
    bool m_bSomeWatched;
    bool m_bRaptorDead;
    bool m_bCombatStart;

    uint64 m_uiWatchTarget;

    void Reset()
    {
        m_uiWatch_Timer = 33000;
        m_uiCleave_Timer = 7000;
        m_uiWhirlwind_Timer = 20000;
        m_uiFear_Timer = 1000;
        m_uiMortalStrike_Timer = 1000;
        m_uiCheck_Timer = 1000;

        m_uiKillCount = 0;

        fTargetPositionX = 0.0f;
        fTargetPositionY = 0.0f;
        fTargetPositionZ = 0.0f;
        m_uiTargetInRangeCount = 0;

        m_uiWatchTarget = 0;

        m_bSomeWatched = false;
        m_bEndWatch = false;
        m_bRaptorDead = false;
        m_bCombatStart = false;

        DoCast(m_creature, SPELL_MOUNT);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_9);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_OHGAN, NOT_STARTED);

        std::list<Creature*> lSpirits;                      //despawn spirits
        GetCreatureListWithEntryInGrid(lSpirits, m_creature, NPC_CHAINED_SPIRIT, DEFAULT_VISIBILITY_INSTANCE);

        if (!lSpirits.empty())
        {
            for(std::list<Creature*>::iterator iter = lSpirits.begin(); iter != lSpirits.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
        {
            ++m_uiKillCount;

            if (m_uiKillCount == 3)
            {
                DoScriptText(SAY_DING_KILL, m_creature);

                if (m_pInstance)
                    if (Unit* jTemp = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_JINDO)))
                        if (jTemp->isAlive())
                            DoScriptText(SAY_GRATS_JINDO, jTemp);

                DoCast(m_creature, SPELL_LEVEL_UP, true);
                m_creature->SetLevel(m_creature->getLevel() + 1);
                m_uiKillCount = 0;
            }

            if (m_creature->isInCombat())
            {
                if (Creature *pSpirit = GetClosestCreatureWithEntry(pVictim, NPC_CHAINED_SPIRIT, 50.0f))
                    pSpirit->CastSpell(pVictim, SPELL_REVIVE, false);
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        uint32 uiCount = sizeof(aSpirits)/sizeof(SpawnLocations);

        for(uint8 i = 0; i < uiCount; ++i)
            m_creature->SummonCreature(NPC_CHAINED_SPIRIT, aSpirits[i].fX, aSpirits[i].fY, aSpirits[i].fZ, aSpirits[i].fAng, TEMPSUMMON_CORPSE_DESPAWN, 0);

        //At combat start Mandokir is mounted so we must unmount it first, and set his flags for attackable
        m_creature->Unmount();

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_9);

        //And summon his raptor
        m_creature->SummonCreature(NPC_OHGAN, pWho->GetPositionX(), pWho->GetPositionY(), pWho->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 35000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiWatch_Timer < uiDiff)                       //Every 20 Sec Mandokir will check this
        {
            if (m_uiWatchTarget)                            //If someone is watched and If the Position of the watched target is uiDifferent from the one stored, or are attacking, mandokir will charge him
            {
                Unit* pUnit = Unit::GetUnit(*m_creature, m_uiWatchTarget);

                if (pUnit && pUnit->isAlive() && (
                    fTargetPositionX != pUnit->GetPositionX() ||
                    fTargetPositionY != pUnit->GetPositionY() ||
                    fTargetPositionZ != pUnit->GetPositionZ() ||
                    fTargetsThreat < m_creature->getThreatManager().getThreat(pUnit)))
                {
                    if (m_creature->IsWithinDistInMap(pUnit, ATTACK_DISTANCE))
                        DoCast(pUnit, SPELL_GUILLOTINE);
                    else
                        DoCast(pUnit, SPELL_CHARGE);
                }
            }

            m_bSomeWatched = false;
            m_uiWatch_Timer = 20000;
        }
        else
            m_uiWatch_Timer -= uiDiff;

        if ((m_uiWatch_Timer < 8000) && !m_bSomeWatched)    //8 sec(cast time + expire time) before the check for the watch effect mandokir will cast watch debuff on a random target
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            {
                if (Player* pPlayer = pTarget->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    DoScriptText(SAY_WATCH, m_creature, pPlayer);
                    DoScriptText(SAY_WATCH_WHISPER, m_creature, pPlayer);

                    DoCast(pPlayer, SPELL_WATCH);
                    m_uiWatchTarget = pPlayer->GetGUID();
                    m_bSomeWatched = true;
                    m_bEndWatch = true;
                }
                else
                {
                    m_uiWatch_Timer = 8000;
                }
            }
        }

        if ((m_uiWatch_Timer < 1000) && m_bEndWatch)        //1 sec before the debuf expire, store the target position
        {
            if (Unit* pWatchTarget = Unit::GetUnit(*m_creature, m_uiWatchTarget))
            {
                fTargetPositionX = pWatchTarget->GetPositionX();
                fTargetPositionY = pWatchTarget->GetPositionY();
                fTargetPositionZ = pWatchTarget->GetPositionZ();
                fTargetsThreat = m_creature->getThreatManager().getThreat(pWatchTarget);
                //based on fTargetsThreateat, needs improvements
            }

            m_bEndWatch = false;
        }

        if (!m_bSomeWatched)
        {
            //Cleave
            if (m_uiCleave_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleave_Timer = 7000;
            }
            else
                m_uiCleave_Timer -= uiDiff;

            //Whirlwind
            if (m_uiWhirlwind_Timer < uiDiff)
            {
                DoCast(m_creature, SPELL_WHIRLWIND);
                m_uiWhirlwind_Timer = 18000;
            }
            else
                m_uiWhirlwind_Timer -= uiDiff;

            //If more then 3 targets in melee range mandokir will cast fear
            if (m_uiFear_Timer < uiDiff)
            {
                m_uiTargetInRangeCount = 0;

                std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                for(; i != m_creature->getThreatManager().getThreatList().end(); ++i)
                {
                    Unit* pTarget = Unit::GetUnit(*m_creature, (*i)->getUnitGuid());
                    if (pTarget && m_creature->IsWithinDistInMap(pTarget, ATTACK_DISTANCE))
                        ++m_uiTargetInRangeCount;
                }

                if (m_uiTargetInRangeCount > 3)
                    DoCast(m_creature->getVictim(), SPELL_FEAR);

                m_uiFear_Timer = 4000;
            }
            else
                m_uiFear_Timer -= uiDiff;

            //Mortal Strike if target below 50% hp
            if (m_creature->getVictim()->GetHealth() < m_creature->getVictim()->GetMaxHealth()*0.5)
            {
                if (m_uiMortalStrike_Timer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
                    m_uiMortalStrike_Timer = 15000;
                }
                else
                    m_uiMortalStrike_Timer -= uiDiff;
            }
        }

        //Checking if Ohgan is dead. If yes Mandokir will enrage.
        if (!m_bRaptorDead && m_pInstance && m_pInstance->GetData(TYPE_OHGAN) == DONE)
        {
            DoCast(m_creature, SPELL_ENRAGE);
            DoScriptText(EMOTE_RAGE, m_creature);
            m_bRaptorDead = true;
        }

        DoMeleeAttackIfReady();
    }
};

//Ohgan
struct MANGOS_DLL_DECL mob_ohganAI : public ScriptedAI
{
    mob_ohganAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSunderArmor_Timer;

    void Reset()
    {
        m_uiSunderArmor_Timer = 5000;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_OHGAN, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
        {
            if (m_creature->isInCombat())
            {
                if (Creature* pSpirit = GetClosestCreatureWithEntry(pVictim, NPC_CHAINED_SPIRIT, 50.0f))
                    pSpirit->CastSpell(pVictim, SPELL_REVIVE, false);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // SunderArmor
        if (m_uiSunderArmor_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SUNDERARMOR);
            m_uiSunderArmor_Timer = urand(10000, 15000);
        }
        else
            m_uiSunderArmor_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_mandokir(Creature* pCreature)
{
    return new boss_mandokirAI(pCreature);
}

CreatureAI* GetAI_mob_ohgan(Creature* pCreature)
{
    return new mob_ohganAI(pCreature);
}

void AddSC_boss_mandokir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_mandokir";
    newscript->GetAI = &GetAI_boss_mandokir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ohgan";
    newscript->GetAI = &GetAI_mob_ohgan;
    newscript->RegisterSelf();
}

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
SDName: Boss_Moroes
SD%Complete: 95
SDComment: ToDo: Guests script can be moved to eventAI.
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    MAX_ACTIVE_GUESTS   = 4,
    MAX_GUESTS          = 6,

    SAY_AGGRO           = -1532011,
    SAY_SPECIAL_1       = -1532012,
    SAY_SPECIAL_2       = -1532013,
    SAY_KILL_1          = -1532014,
    SAY_KILL_2          = -1532015,
    SAY_KILL_3          = -1532016,
    SAY_DEATH           = -1532017,

    SPELL_VANISH        = 29448,
    SPELL_GARROTE       = 37066,
    SPELL_BLIND         = 34694,
    SPELL_GOUGE         = 29425,
    SPELL_FRENZY        = 37023
};

static const float afLocations[MAX_ACTIVE_GUESTS][4] =
{
    {-10991.0f, -1884.33f, 81.73f, 0.614315f},
    {-10989.4f, -1885.88f, 81.73f, 0.904913f},
    {-10978.1f, -1887.07f, 81.73f, 2.035550f},
    {-10975.9f, -1885.81f, 81.73f, 2.253890f}
};

static const uint32 auiGuests[MAX_GUESTS] =
{
    NPC_LADY_KEIRA_BERRYBUCK,
    NPC_LADY_CATRIONA_VON_INDI,
    NPC_LORD_CRISPIN_FERENCE,
    NPC_BARON_RAFE_DREUGER,
    NPC_BARONESS_DOROTHEA_MILLSTIPE,
    NPC_LORD_ROBIN_DARIS
};

struct MANGOS_DLL_DECL boss_moroesAI : public ScriptedAI
{
    boss_moroesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance  = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    std::vector<uint32> m_vGuestsEntryList;

    uint32 m_uiVanishTimer;
    uint32 m_uiBlindTimer;
    uint32 m_uiGougeTimer;
    uint32 m_uiWaitTimer;

    bool m_bEnrage;

    void Reset()
    {
        m_uiVanishTimer     = 30000;
        m_uiBlindTimer      = 35000;
        m_uiGougeTimer      = 23000;
        m_uiWaitTimer       = 0;

        m_bEnrage           = false;

        DoSpawnGuests();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        DoRemoveGarroteAura();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, FAIL);
    }

    void JustDied(Unit* pVictim)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DoRemoveGarroteAura();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, DONE);
    }

    void EnterEvadeMode()
    {
        // Don't evade during vanish phase
        if (m_uiWaitTimer)
            return;

        ScriptedAI::EnterEvadeMode();
    }

    void DoSpawnGuests()
    {
        //not if m_creature are dead, so avoid
        if (!m_creature->isAlive())
            return;

        // it's empty, so first time
        if (m_vGuestsEntryList.empty())
        {
            // pre-allocate size for speed
            m_vGuestsEntryList.resize(MAX_GUESTS);

            // fill vector array with entries from creature array
            for (uint8 i = 0; i < MAX_GUESTS; ++i)
                m_vGuestsEntryList[i] = auiGuests[i];

            std::random_shuffle(m_vGuestsEntryList.begin(), m_vGuestsEntryList.end());

            // Summon the 4 entries
            for (uint8 i = 0; i < MAX_ACTIVE_GUESTS; ++i)
                m_creature->SummonCreature(m_vGuestsEntryList[i], afLocations[i][0], afLocations[i][1], afLocations[i][2], afLocations[i][3], TEMPSUMMON_CORPSE_DESPAWN, 0);
        }
        // Resummon the killed adds
        else
        {
            if (!m_pInstance)
                return;

            for (uint8 i = 0; i < MAX_ACTIVE_GUESTS; ++i)
            {
                // If we already have the creature on the map, then don't summon it
                if (Creature* pAdd = m_pInstance->GetSingleCreatureFromStorage(m_vGuestsEntryList[i], true))
                    continue;

                m_creature->SummonCreature(m_vGuestsEntryList[i], afLocations[i][0], afLocations[i][1], afLocations[i][2], afLocations[i][3], TEMPSUMMON_CORPSE_DESPAWN, 0);
            }
        }
    }

    // Wrapper to remove the Garrote aura on death and on evade - ToDo: maybe find a better way for this!
    void DoRemoveGarroteAura()
    {
        // remove aura from spell Garrote when Moroes dies
        Map* pMap = m_creature->GetMap();
        if (pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_GARROTE))
                    i->getSource()->RemoveAurasDueToSpell(SPELL_GARROTE);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Note: because the Vanish spell adds invisibility effect on the target, the timers won't be decreased during the vanish phase
        if (m_uiWaitTimer)
        {
            if (m_uiWaitTimer <= uiDiff)
            {
                // It's not very clear how to handle this spell
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoScriptText(urand(0, 1) ? SAY_SPECIAL_1 : SAY_SPECIAL_2, m_creature);
                    DoResetThreat();
                    AttackStart(pTarget);
                    pTarget->CastSpell(pTarget, SPELL_GARROTE, true);
                }
                m_uiWaitTimer = 0;
            }
            else
                m_uiWaitTimer -= uiDiff;

            // Don't user other abilities in vanish
            return;
        }

        if (!m_bEnrage && m_creature->GetHealthPercent() < 30.0f)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
                m_bEnrage = true;
        }

        // No other spells are cast after enrage
        if (!m_bEnrage)
        {
            if (m_uiVanishTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_VANISH) == CAST_OK)
                {
                    m_uiVanishTimer = 30000;
                    m_uiWaitTimer   = 1000;
                }
            }
            else
                m_uiVanishTimer -= uiDiff;

            //Gouge highest aggro, and attack second highest
            if (m_uiGougeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GOUGE) == CAST_OK)
                    m_uiGougeTimer = 40000;
            }
            else
                m_uiGougeTimer -= uiDiff;

            if (m_uiBlindTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_BLIND, SELECT_FLAG_PLAYER))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_BLIND) == CAST_OK)
                        m_uiBlindTimer = 40000;
                }
            }
            else
                m_uiBlindTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

enum
{
    SPELL_MANABURN      = 29405,
    SPELL_MINDFLY       = 29570,
    SPELL_SWPAIN        = 34441,
    SPELL_SHADOWFORM    = 29406
};

struct MANGOS_DLL_DECL boss_baroness_dorothea_millstipeAI : public ScriptedAI
{
    //Shadow Priest
    boss_baroness_dorothea_millstipeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiManaBurn_Timer;
    uint32 m_uiMindFlay_Timer;
    uint32 m_uiShadowWordPain_Timer;

    void Reset()
    {
        m_uiManaBurn_Timer       = 7000;
        m_uiMindFlay_Timer       = 1000;
        m_uiShadowWordPain_Timer = 6000;

        DoCastSpellIfCan(m_creature, SPELL_SHADOWFORM, CAST_TRIGGERED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMindFlay_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MINDFLY);
            m_uiMindFlay_Timer = 12000;                     //3sec channeled
        }
        else
            m_uiMindFlay_Timer -= uiDiff;

        if (m_uiManaBurn_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_MANABURN, SELECT_FLAG_POWER_MANA))
                DoCastSpellIfCan(pTarget, SPELL_MANABURN);

            m_uiManaBurn_Timer = 5000;                      //3 sec cast
        }
        else
            m_uiManaBurn_Timer -= uiDiff;

        if (m_uiShadowWordPain_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, SPELL_SWPAIN);
                m_uiShadowWordPain_Timer = 7000;
            }
        }
        else
            m_uiShadowWordPain_Timer -= uiDiff;
    }
};

enum
{
    SPELL_HAMMEROFJUSTICE       = 13005,
    SPELL_JUDGEMENTOFCOMMAND    = 29386,
    SPELL_SEALOFCOMMAND         = 29385
};

struct MANGOS_DLL_DECL boss_baron_rafe_dreugerAI : public ScriptedAI
{
    //Retr Pally
    boss_baron_rafe_dreugerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiHammerOfJustice_Timer;
    uint32 m_uiSealOfCommand_Timer;
    uint32 m_uiJudgementOfCommand_Timer;

    void Reset()
    {
        m_uiHammerOfJustice_Timer    = 1000;
        m_uiSealOfCommand_Timer      = 7000;
        m_uiJudgementOfCommand_Timer = m_uiSealOfCommand_Timer + 29000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSealOfCommand_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SEALOFCOMMAND);
            m_uiSealOfCommand_Timer      = 32000;
            m_uiJudgementOfCommand_Timer = 29000;
        }
        else
            m_uiSealOfCommand_Timer -= uiDiff;

        if (m_uiJudgementOfCommand_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_JUDGEMENTOFCOMMAND);
            m_uiJudgementOfCommand_Timer = m_uiSealOfCommand_Timer + 29000;
        }
        else
            m_uiJudgementOfCommand_Timer -= uiDiff;

        if (m_uiHammerOfJustice_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMMEROFJUSTICE);
            m_uiHammerOfJustice_Timer = 12000;
        }
        else
            m_uiHammerOfJustice_Timer -= uiDiff;
    }
};

enum
{
    SPELL_DISPELMAGIC   = 15090,                            // Self or other guest+Moroes
    SPELL_GREATERHEAL   = 29564,                            // Self or other guest+Moroes
    SPELL_HOLYFIRE      = 29563,
    SPELL_PWSHIELD      = 29408
};

struct MANGOS_DLL_DECL boss_lady_catriona_von_indiAI : public ScriptedAI
{
    //Holy Priest
    boss_lady_catriona_von_indiAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiDispelMagic_Timer;
    uint32 m_uiGreaterHeal_Timer;
    uint32 m_uiHolyFire_Timer;
    uint32 m_uiPowerWordShield_Timer;

    void Reset()
    {
        m_uiDispelMagic_Timer     = 11000;
        m_uiGreaterHeal_Timer     = 1500;
        m_uiHolyFire_Timer        = 5000;
        m_uiPowerWordShield_Timer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPowerWordShield_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_PWSHIELD);
            m_uiPowerWordShield_Timer = 15000;
        }
        else
            m_uiPowerWordShield_Timer -= uiDiff;

        if (m_uiGreaterHeal_Timer < uiDiff)
        {
            if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
            {
                DoCastSpellIfCan(pTarget, SPELL_GREATERHEAL);
                m_uiGreaterHeal_Timer = 17000;
            }
        }
        else
            m_uiGreaterHeal_Timer -= uiDiff;

        if (m_uiHolyFire_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HOLYFIRE);
            m_uiHolyFire_Timer = 22000;
        }
        else
            m_uiHolyFire_Timer -= uiDiff;

        if (m_uiDispelMagic_Timer < uiDiff)
        {
            if (Unit* pTarget = urand(0, 1) ? DoSelectLowestHpFriendly(50.0f) : m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_DISPELMAGIC);

            m_uiDispelMagic_Timer = 25000;
        }
        else
            m_uiDispelMagic_Timer -= uiDiff;
    }
};

enum
{
    SPELL_CLEANSE               = 29380,                    //Self or other guest+Moroes
    SPELL_GREATERBLESSOFMIGHT   = 29381,                    //Self or other guest+Moroes
    SPELL_HOLYLIGHT             = 29562,                    //Self or other guest+Moroes
    SPELL_DIVINESHIELD          = 41367
};

struct MANGOS_DLL_DECL boss_lady_keira_berrybuckAI : public ScriptedAI
{
    //Holy Pally
    boss_lady_keira_berrybuckAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiCleanse_Timer;
    uint32 m_uiGreaterBless_Timer;
    uint32 m_uiHolyLight_Timer;
    uint32 m_uiDivineShield_Timer;

    void Reset()
    {
        m_uiCleanse_Timer      = 13000;
        m_uiGreaterBless_Timer = 1000;
        m_uiHolyLight_Timer    = 7000;
        m_uiDivineShield_Timer = 31000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDivineShield_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DIVINESHIELD);
            m_uiDivineShield_Timer = 31000;
        }
        else
            m_uiDivineShield_Timer -= uiDiff;

        if (m_uiHolyLight_Timer < uiDiff)
        {
            if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
            {
                DoCastSpellIfCan(pTarget, SPELL_HOLYLIGHT);
                m_uiHolyLight_Timer = 10000;
            }
        }
        else
            m_uiHolyLight_Timer -= uiDiff;

        if (m_uiGreaterBless_Timer < uiDiff)
        {
            if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
            {
                DoCastSpellIfCan(pTarget, SPELL_GREATERBLESSOFMIGHT);
                m_uiGreaterBless_Timer = 50000;
            }
        }
        else
            m_uiGreaterBless_Timer -= uiDiff;

        if (m_uiCleanse_Timer < uiDiff)
        {
            if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
            {
                DoCastSpellIfCan(pTarget, SPELL_CLEANSE);
                m_uiCleanse_Timer = 10000;
            }
        }
        else
            m_uiCleanse_Timer -= uiDiff;
    }
};

enum
{
    SPELL_HAMSTRING     = 9080,
    SPELL_MORTALSTRIKE  = 29572,
    SPELL_WHIRLWIND     = 29573
};

struct MANGOS_DLL_DECL boss_lord_robin_darisAI : public ScriptedAI
{
    //Arms Warr
    boss_lord_robin_darisAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiHamstring_Timer;
    uint32 m_uiMortalStrike_Timer;
    uint32 m_uiWhirlWind_Timer;

    void Reset()
    {
        m_uiHamstring_Timer    = 7000;
        m_uiMortalStrike_Timer = 10000;
        m_uiWhirlWind_Timer    = 21000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHamstring_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMSTRING);
            m_uiHamstring_Timer = 12000;
        }
        else
            m_uiHamstring_Timer -= uiDiff;

        if (m_uiMortalStrike_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTALSTRIKE);
            m_uiMortalStrike_Timer = 18000;
        }
        else
            m_uiMortalStrike_Timer -= uiDiff;

        if (m_uiWhirlWind_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND);
            m_uiWhirlWind_Timer = 21000;
        }
        else
            m_uiWhirlWind_Timer -= uiDiff;
    }
};

enum
{
    SPELL_DISARM            = 8379,
    SPELL_HEROICSTRIKE      = 29567,
    SPELL_SHIELDBASH        = 11972,
    SPELL_SHIELDWALL        = 29390
};

struct MANGOS_DLL_DECL boss_lord_crispin_ferenceAI : public ScriptedAI
{
    //Arms Warr
    boss_lord_crispin_ferenceAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiDisarm_Timer;
    uint32 m_uiHeroicStrike_Timer;
    uint32 m_uiShieldBash_Timer;
    uint32 m_uiShieldWall_Timer;

    void Reset()
    {
        m_uiDisarm_Timer       = 6000;
        m_uiHeroicStrike_Timer = 10000;
        m_uiShieldBash_Timer   = 8000;
        m_uiShieldWall_Timer   = 4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDisarm_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_DISARM);
            m_uiDisarm_Timer = 12000;
        }
        else
            m_uiDisarm_Timer -= uiDiff;

        if (m_uiHeroicStrike_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HEROICSTRIKE);
            m_uiHeroicStrike_Timer = 10000;
        }else m_uiHeroicStrike_Timer -= uiDiff;

        if (m_uiShieldBash_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHIELDBASH);
            m_uiShieldBash_Timer = 13000;
        }
        else
            m_uiShieldBash_Timer -= uiDiff;

        if (m_uiShieldWall_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SHIELDWALL);
            m_uiShieldWall_Timer = 21000;
        }
        else
            m_uiShieldWall_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_moroes(Creature* pCreature)
{
    return new boss_moroesAI(pCreature);
}

CreatureAI* GetAI_baroness_dorothea_millstipe(Creature* pCreature)
{
    return new boss_baroness_dorothea_millstipeAI(pCreature);
}

CreatureAI* GetAI_baron_rafe_dreuger(Creature* pCreature)
{
    return new boss_baron_rafe_dreugerAI(pCreature);
}

CreatureAI* GetAI_lady_catriona_von_indi(Creature* pCreature)
{
    return new boss_lady_catriona_von_indiAI(pCreature);
}

CreatureAI* GetAI_lady_keira_berrybuck(Creature* pCreature)
{
    return new boss_lady_keira_berrybuckAI(pCreature);
}

CreatureAI* GetAI_lord_robin_daris(Creature* pCreature)
{
    return new boss_lord_robin_darisAI(pCreature);
}

CreatureAI* GetAI_lord_crispin_ference(Creature* pCreature)
{
    return new boss_lord_crispin_ferenceAI(pCreature);
}

void AddSC_boss_moroes()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_moroes";
    pNewScript->GetAI = &GetAI_boss_moroes;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_baroness_dorothea_millstipe";
    pNewScript->GetAI = &GetAI_baroness_dorothea_millstipe;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_baron_rafe_dreuger";
    pNewScript->GetAI = &GetAI_baron_rafe_dreuger;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lady_catriona_von_indi";
    pNewScript->GetAI = &GetAI_lady_catriona_von_indi;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lady_keira_berrybuck";
    pNewScript->GetAI = &GetAI_lady_keira_berrybuck;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lord_robin_daris";
    pNewScript->GetAI = &GetAI_lord_robin_daris;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lord_crispin_ference";
    pNewScript->GetAI = &GetAI_lord_crispin_ference;
    pNewScript->RegisterSelf();
}

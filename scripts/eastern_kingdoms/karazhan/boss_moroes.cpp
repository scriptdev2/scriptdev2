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
SDName: Boss_Moroes
SD%Complete: 95
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
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

const float afLocations[4][4]=
{
    {-10991.0f, -1884.33f, 81.73f, 0.614315f},
    {-10989.4f, -1885.88f, 81.73f, 0.904913f},
    {-10978.1f, -1887.07f, 81.73f, 2.035550f},
    {-10975.9f, -1885.81f, 81.73f, 2.253890f}
};

const uint32 auiAdds[6]=
{
    17007,
    19872,
    19873,
    19874,
    19875,
    19876
};

struct MANGOS_DLL_DECL boss_moroesAI : public ScriptedAI
{
    boss_moroesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bFirstTime = true;
        m_pInstance  = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_auiAddGUID[4];
    uint32 m_auiAddId[4];

    uint32 m_uiVanish_Timer;
    uint32 m_uiBlind_Timer;
    uint32 m_uiGouge_Timer;
    uint32 m_uiWait_Timer;
    uint32 m_uiCheckAdds_Timer;

    bool m_bFirstTime;
    bool m_bInVanish;
    bool m_bEnrage;

    void Reset()
    {
        m_uiVanish_Timer    = 30000;
        m_uiBlind_Timer     = 35000;
        m_uiGouge_Timer     = 23000;
        m_uiWait_Timer      = 0;
        m_uiCheckAdds_Timer = 5000;

        m_bEnrage           = false;
        m_bInVanish         = false;

        SpawnAdds();

        m_creature->setFaction(16);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, NOT_STARTED);
    }

    void StartEvent()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, IN_PROGRESS);
    }

    void Aggro(Unit* pWho)
    {
        StartEvent();
        DoScriptText(SAY_AGGRO, m_creature);
        AddsAttack();
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

    void JustDied(Unit* pVictim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, DONE);

        //remove aura from spell Garrote when Moroes dies
        Map* pMap = m_creature->GetMap();
        if (pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_GARROTE, EFFECT_INDEX_0))
                    i->getSource()->RemoveAurasDueToSpell(SPELL_GARROTE);
            }
        }
    }

    void SpawnAdds()
    {
        if (m_bFirstTime)
        {
            std::vector<uint32> vAddList;

            for(uint8 i = 0; i < 6; ++i)
                vAddList.push_back(auiAdds[i]);

            while(vAddList.size() > 4)
                vAddList.erase((vAddList.begin())+(rand()%vAddList.size()));

            uint8 i = 0;
            for(std::vector<uint32>::iterator itr = vAddList.begin(); itr != vAddList.end(); ++itr, ++i)
            {
                if (Creature* pCreature = m_creature->SummonCreature(*itr, afLocations[i][0], afLocations[i][1], afLocations[i][2], afLocations[i][3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                {
                    m_auiAddGUID[i] = pCreature->GetGUID();
                    m_auiAddId[i]   = *itr;
                }
            }

            m_bFirstTime = false;
        }
        else
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                if (Creature* pCreature = m_creature->GetMap()->GetCreature(m_auiAddGUID[i]))
                {
                    if (!pCreature->isAlive())              // Exists but is dead
                    {
                        pCreature->Respawn();
                        pCreature->AI()->EnterEvadeMode();
                    }
                    else if (!pCreature->IsInEvadeMode())   // Exists and is alive
                    {
                        pCreature->AI()->EnterEvadeMode();
                    }
                }
                else
                {                                           // Does not exist
                    if (Creature* pCreature = m_creature->SummonCreature(m_auiAddId[i], afLocations[i][0], afLocations[i][1], afLocations[i][2], afLocations[i][3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                        m_auiAddGUID[i] = pCreature->GetGUID();
                }
            }
        }
    }

    void AddsAttack()
    {
        for(uint8 i = 0; i < 4; ++i)
        {
            if (m_auiAddGUID[i])
            {
                Creature* pTemp = m_creature->GetMap()->GetCreature(m_auiAddGUID[i]);
                if (pTemp && pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    EnterEvadeMode();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance && !m_pInstance->GetData(TYPE_MOROES))
            EnterEvadeMode();

        if (!m_bEnrage && m_creature->GetHealthPercent() < 30.0f)
        {
            DoCastSpellIfCan(m_creature, SPELL_FRENZY);
            m_bEnrage = true;
        }

        if (m_uiCheckAdds_Timer < uiDiff)
        {
            for (uint8 i = 0; i < 4; ++i)
            {
                if (m_auiAddGUID[i])
                {
                    Creature* pTemp = m_creature->GetMap()->GetCreature(m_auiAddGUID[i]);
                    if (pTemp && pTemp->isAlive() && (!pTemp->SelectHostileTarget() || !pTemp->getVictim()))
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                }
            }
            m_uiCheckAdds_Timer = 5000;
        }
        else
            m_uiCheckAdds_Timer -= uiDiff;

        if (!m_bEnrage)
        {
            // Cast Vanish, then Garrote random victim
            if (m_uiVanish_Timer < uiDiff)
            {
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoCastSpellIfCan(m_creature, SPELL_VANISH);
                m_bInVanish      = true;
                m_uiVanish_Timer = 30000;
                m_uiWait_Timer   = 5000;
            }
            else
                m_uiVanish_Timer -= uiDiff;

            if (m_bInVanish)
            {
                if (m_uiWait_Timer < uiDiff)
                {
                    DoScriptText(urand(0, 1) ? SAY_SPECIAL_1 : SAY_SPECIAL_2, m_creature);

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        pTarget->CastSpell(pTarget, SPELL_GARROTE, true);

                    m_creature->setFaction(16);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->AI()->AttackStart(m_creature->getVictim());
                    m_bInVanish = false;
                }
                else
                    m_uiWait_Timer -= uiDiff;
            }

            //Gouge highest aggro, and attack second highest
            if (m_uiGouge_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_GOUGE);
                m_uiGouge_Timer = 40000;
            }
            else
                m_uiGouge_Timer -= uiDiff;

            if (m_uiBlind_Timer < uiDiff)
            {
                Unit* pTarget = NULL;

                ThreatList const& vThreatList = m_creature->getThreatManager().getThreatList();
                if (vThreatList.empty())
                    return;

                std::vector<Unit*> vTargetList;

                for (ThreatList::const_iterator itr = vThreatList.begin();itr != vThreatList.end(); ++itr)
                {
                    pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

                    if (pTarget && m_creature->CanReachWithMeleeAttack(pTarget))
                        vTargetList.push_back(pTarget);
                }

                if (!vTargetList.empty())
                    pTarget = *(vTargetList.begin()+rand()%vTargetList.size());

                if (pTarget)
                    DoCastSpellIfCan(pTarget, SPELL_BLIND);

                m_uiBlind_Timer = 40000;
            }
            else
                m_uiBlind_Timer -= uiDiff;
        }

        if (!m_bInVanish)
            DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_moroes_guestAI : public ScriptedAI
{
    ScriptedInstance* m_pInstance;

    uint64 m_auiGuestGUID[4];

    boss_moroes_guestAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        memset(&m_auiGuestGUID, 0, sizeof(m_auiGuestGUID));

        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        AcquireGUID();
        Reset();
    }

    void Reset()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MOROES, NOT_STARTED);
    }

    void AcquireGUID()
    {
        if (!m_pInstance)
            return;

        m_auiGuestGUID[0] = m_pInstance->GetData64(NPC_MOROES);

        if (Creature* pMoroes = m_creature->GetMap()->GetCreature(m_auiGuestGUID[0]))
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                uint64 uiGUID = 0;

                if (boss_moroesAI* pMoroesAI = dynamic_cast<boss_moroesAI*>(pMoroes->AI()))
                    uiGUID = pMoroesAI->m_auiAddGUID[i];

                if (uiGUID && uiGUID != m_creature->GetGUID())
                    m_auiGuestGUID[i+1] = uiGUID;
            }
        }
    }

    Unit* SelectTarget()
    {
        if (uint64 uiTempGUID = m_auiGuestGUID[rand()%4])
        {
            Creature* pTemp = m_creature->GetMap()->GetCreature(uiTempGUID);
            if (pTemp && pTemp->isAlive())
                return pTemp;
        }

        return m_creature;
    }

    // TODO double check this design! - with momentarily system DoMeleeAttackIfReady is called before the spells are handled
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_pInstance->GetData(TYPE_MOROES))
            EnterEvadeMode();

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

struct MANGOS_DLL_DECL boss_baroness_dorothea_millstipeAI : public boss_moroes_guestAI
{
    //Shadow Priest
    boss_baroness_dorothea_millstipeAI(Creature* pCreature) : boss_moroes_guestAI(pCreature) { Reset(); }

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

        boss_moroes_guestAI::UpdateAI(uiDiff);

        if (m_uiMindFlay_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MINDFLY);
            m_uiMindFlay_Timer = 12000;                     //3sec channeled
        }
        else
            m_uiMindFlay_Timer -= uiDiff;

        if (m_uiManaBurn_Timer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && pTarget->getPowerType() == POWER_MANA)
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

struct MANGOS_DLL_DECL boss_baron_rafe_dreugerAI : public boss_moroes_guestAI
{
    //Retr Pally
    boss_baron_rafe_dreugerAI(Creature* pCreature) : boss_moroes_guestAI(pCreature) { Reset(); }

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

        boss_moroes_guestAI::UpdateAI(uiDiff);

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

struct MANGOS_DLL_DECL boss_lady_catriona_von_indiAI : public boss_moroes_guestAI
{
    //Holy Priest
    boss_lady_catriona_von_indiAI(Creature* pCreature) : boss_moroes_guestAI(pCreature) { Reset(); }

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

        boss_moroes_guestAI::UpdateAI(uiDiff);

        if (m_uiPowerWordShield_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_PWSHIELD);
            m_uiPowerWordShield_Timer = 15000;
        }
        else
            m_uiPowerWordShield_Timer -= uiDiff;

        if (m_uiGreaterHeal_Timer < uiDiff)
        {
            DoCastSpellIfCan(SelectTarget(), SPELL_GREATERHEAL);
            m_uiGreaterHeal_Timer = 17000;
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
            if (Unit* pTarget = urand(0, 1) ? SelectTarget() : m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
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

struct MANGOS_DLL_DECL boss_lady_keira_berrybuckAI : public boss_moroes_guestAI
{
    //Holy Pally
    boss_lady_keira_berrybuckAI(Creature* pCreature) : boss_moroes_guestAI(pCreature) { Reset(); }

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

        boss_moroes_guestAI::UpdateAI(uiDiff);

        if (m_uiDivineShield_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DIVINESHIELD);
            m_uiDivineShield_Timer = 31000;
        }
        else
            m_uiDivineShield_Timer -= uiDiff;

        if (m_uiHolyLight_Timer < uiDiff)
        {
            DoCast(SelectTarget(), SPELL_HOLYLIGHT);
            m_uiHolyLight_Timer = 10000;
        }
        else
            m_uiHolyLight_Timer -= uiDiff;

        if (m_uiGreaterBless_Timer < uiDiff)
        {
            DoCastSpellIfCan(SelectTarget(), SPELL_GREATERBLESSOFMIGHT);
            m_uiGreaterBless_Timer = 50000;
        }
        else
            m_uiGreaterBless_Timer -= uiDiff;

        if (m_uiCleanse_Timer < uiDiff)
        {
            DoCastSpellIfCan(SelectTarget(), SPELL_CLEANSE);
            m_uiCleanse_Timer = 10000;
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

struct MANGOS_DLL_DECL boss_lord_robin_darisAI : public boss_moroes_guestAI
{
    //Arms Warr
    boss_lord_robin_darisAI(Creature* pCreature) : boss_moroes_guestAI(pCreature) { Reset(); }

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

        boss_moroes_guestAI::UpdateAI(uiDiff);

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

struct MANGOS_DLL_DECL boss_lord_crispin_ferenceAI : public boss_moroes_guestAI
{
    //Arms Warr
    boss_lord_crispin_ferenceAI(Creature* pCreature) : boss_moroes_guestAI(pCreature) { Reset(); }

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

        boss_moroes_guestAI::UpdateAI(uiDiff);

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
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_moroes";
    newscript->GetAI = &GetAI_boss_moroes;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_baroness_dorothea_millstipe";
    newscript->GetAI = &GetAI_baroness_dorothea_millstipe;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_baron_rafe_dreuger";
    newscript->GetAI = &GetAI_baron_rafe_dreuger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lady_catriona_von_indi";
    newscript->GetAI = &GetAI_lady_catriona_von_indi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lady_keira_berrybuck";
    newscript->GetAI = &GetAI_lady_keira_berrybuck;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lord_robin_daris";
    newscript->GetAI = &GetAI_lord_robin_daris;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lord_crispin_ference";
    newscript->GetAI = &GetAI_lord_crispin_ference;
    newscript->RegisterSelf();
}

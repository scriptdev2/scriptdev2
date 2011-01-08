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
SDName: Boss_Exarch_Maladaar
SD%Complete: 95
SDComment: Most of event implemented, some adjustments to timers remain and possibly make some better code for switching his dark side in to better "images" of player.
SDCategory: Auchindoun, Auchenai Crypts
EndScriptData */

/* ContentData
mob_stolen_soul
boss_exarch_maladaar
mob_avatar_of_martyred
EndContentData */

#include "precompiled.h"

enum
{
    SPELL_MOONFIRE          = 37328,
    SPELL_FIREBALL          = 37329,
    SPELL_MIND_FLAY         = 37330,
    SPELL_HEMORRHAGE        = 37331,
    SPELL_FROSTSHOCK        = 37332,
    SPELL_CURSE_OF_AGONY    = 37334,
    SPELL_MORTAL_STRIKE     = 37335,
    SPELL_FREEZING_TRAP     = 37368,
    SPELL_HAMMER_OF_JUSTICE = 37369,
    SPELL_PLAGUE_STRIKE     = 58339
};

struct MANGOS_DLL_DECL mob_stolen_soulAI : public ScriptedAI
{
    mob_stolen_soulAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint8 m_uiStolenClass;
    uint32 m_uiSpellTimer;

    void Reset()
    {
        m_uiSpellTimer = 1000;
    }

    void SetSoulInfo(Unit* pTarget)
    {
        m_uiStolenClass = pTarget->getClass();
        m_creature->SetDisplayId(pTarget->GetDisplayId());

    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < uiDiff)
        {
            switch (m_uiStolenClass)
            {
                case CLASS_WARRIOR:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
                    m_uiSpellTimer = 6000;
                    break;
                case CLASS_PALADIN:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMMER_OF_JUSTICE);
                    m_uiSpellTimer = 6000;
                    break;
                case CLASS_HUNTER:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_FREEZING_TRAP);
                    m_uiSpellTimer = 20000;
                    break;
                case CLASS_ROGUE:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_HEMORRHAGE);
                    m_uiSpellTimer = 10000;
                    break;
                case CLASS_PRIEST:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIND_FLAY);
                    m_uiSpellTimer = 5000;
                    break;
                case CLASS_SHAMAN:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTSHOCK);
                    m_uiSpellTimer = 8000;
                    break;
                case CLASS_MAGE:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBALL);
                    m_uiSpellTimer = 5000;
                    break;
                case CLASS_WARLOCK:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_CURSE_OF_AGONY);
                    m_uiSpellTimer = 20000;
                    break;
                case CLASS_DRUID:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MOONFIRE);
                    m_uiSpellTimer = 10000;
                    break;
                case CLASS_DEATH_KNIGHT:
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_PLAGUE_STRIKE);
                    m_uiSpellTimer = 10000;
                    break;
            }
        }
        else
            m_uiSpellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_stolen_soul(Creature* pCreature)
{
    return new mob_stolen_soulAI(pCreature);
}

enum
{
    SAY_INTRO                = -1558000,
    SAY_SUMMON               = -1558001,
    SAY_AGGRO_1              = -1558002,
    SAY_AGGRO_2              = -1558003,
    SAY_AGGRO_3              = -1558004,
    SAY_ROAR                 = -1558005,
    SAY_SOUL_CLEAVE          = -1558006,
    SAY_SLAY_1               = -1558007,
    SAY_SLAY_2               = -1558008,
    SAY_DEATH                = -1558009,

    SPELL_RIBBON_OF_SOULS    = 32422,
    SPELL_SOUL_SCREAM        = 32421,
    SPELL_STOLEN_SOUL        = 32346,
    SPELL_STOLEN_SOUL_VISUAL = 32395,
    SPELL_SUMMON_AVATAR      = 32424,

    NPC_STOLEN_SOUL          = 18441,
    NPC_DORE                 = 19412
};

struct MANGOS_DLL_DECL boss_exarch_maladaarAI : public ScriptedAI
{
    boss_exarch_maladaarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bHasTaunted = false;
        Reset();
    }

    uint64 m_uiTargetGUID;

    uint32 m_uiFearTimer;
    uint32 m_uiRibbonOfSoulsTimer;
    uint32 m_uiStolenSoulTimer;

    bool m_bHasTaunted;
    bool m_bHasSummonedAvatar;

    void Reset()
    {
        m_uiTargetGUID = 0;

        m_uiFearTimer          = urand(15000, 20000);
        m_uiRibbonOfSoulsTimer = 5000;
        m_uiStolenSoulTimer    = urand(25000, 35000);

        m_bHasSummonedAvatar = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 150.0))
        {
            DoScriptText(SAY_INTRO, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_STOLEN_SOUL)
        {
            //SPELL_STOLEN_SOUL_VISUAL has shapeshift effect, but not implemented feature in mangos for this spell.
            pSummoned->CastSpell(pSummoned, SPELL_STOLEN_SOUL_VISUAL, false);
            pSummoned->setFaction(m_creature->getFaction());

            if (Player* pTarget = m_creature->GetMap()->GetPlayer(m_uiTargetGUID))
            {
                if (mob_stolen_soulAI* pSoulAI = dynamic_cast<mob_stolen_soulAI*>(pSummoned->AI()))
                {
                    pSoulAI->SetSoulInfo(pTarget);
                    pSoulAI->AttackStart(pTarget);
                }
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            return;

        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        // When Exarch Maladaar is defeated D'ore appear.
        DoSpawnCreature(NPC_DORE, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 600000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bHasSummonedAvatar && m_creature->GetHealthPercent() < 25.0f)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(true);

            DoScriptText(SAY_SUMMON, m_creature);

            DoCastSpellIfCan(m_creature, SPELL_SUMMON_AVATAR);
            m_bHasSummonedAvatar = true;
            m_uiStolenSoulTimer = urand(15000, 30000);
        }

        if (m_uiStolenSoulTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    if (m_creature->IsNonMeleeSpellCasted(false))
                        m_creature->InterruptNonMeleeSpells(true);

                    DoScriptText(urand(0, 1) ? SAY_ROAR : SAY_SOUL_CLEAVE, m_creature);

                    m_uiTargetGUID = pTarget->GetGUID();

                    DoCastSpellIfCan(pTarget, SPELL_STOLEN_SOUL);
                    DoSpawnCreature(NPC_STOLEN_SOUL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);

                    m_uiStolenSoulTimer = urand(20000, 30000);
                }
                else
                    m_uiStolenSoulTimer = 1000;
            }
        }
        else
            m_uiStolenSoulTimer -= uiDiff;

        if (m_uiRibbonOfSoulsTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_RIBBON_OF_SOULS);

            m_uiRibbonOfSoulsTimer = urand(5000, 25000);
        }
        else
            m_uiRibbonOfSoulsTimer -= uiDiff;

        if (m_uiFearTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SOUL_SCREAM);
            m_uiFearTimer = urand(15000, 30000);
        }
        else
            m_uiFearTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_exarch_maladaar(Creature* pCreature)
{
    return new boss_exarch_maladaarAI(pCreature);
}

enum
{
    SPELL_AV_MORTAL_STRIKE = 16856,
    SPELL_AV_SUNDER_ARMOR  = 16145
};

struct MANGOS_DLL_DECL mob_avatar_of_martyredAI : public ScriptedAI
{
    mob_avatar_of_martyredAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        m_uiMortalStrikeTimer = 10000;
    }

    uint32 m_uiMortalStrikeTimer;

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortalStrikeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_AV_MORTAL_STRIKE);
            m_uiMortalStrikeTimer = urand(10000, 30000);
        }
        else
            m_uiMortalStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_avatar_of_martyred(Creature* pCreature)
{
    return new mob_avatar_of_martyredAI(pCreature);
}

void AddSC_boss_exarch_maladaar()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_exarch_maladaar";
    newscript->GetAI = &GetAI_boss_exarch_maladaar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_avatar_of_martyred";
    newscript->GetAI = &GetAI_mob_avatar_of_martyred;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_stolen_soul";
    newscript->GetAI = &GetAI_mob_stolen_soul;
    newscript->RegisterSelf();
}

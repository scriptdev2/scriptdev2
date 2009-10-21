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
SDName: Boss_Noth
SD%Complete: 40
SDComment: Missing Balcony stage
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1                          = -1533075,
    SAY_AGGRO2                          = -1533076,
    SAY_AGGRO3                          = -1533077,
    SAY_SUMMON                          = -1533078,
    SAY_SLAY1                           = -1533079,
    SAY_SLAY2                           = -1533080,
    SAY_DEATH                           = -1533081,

    SPELL_BLINK                         = 29211,            //29208, 29209 and 29210 too
    SPELL_CRIPPLE                       = 29212,
    SPELL_CRIPPLE_H                     = 54814,
    SPELL_CURSE_PLAGUEBRINGER           = 29213,
    SPELL_CURSE_PLAGUEBRINGER_H         = 54835,

    SPELL_SUMMON_CHAMPION_AND_CONSTRUCT = 29240,
    SPELL_SUMMON_GUARDIAN_AND_CONSTRUCT = 29269,

    NPC_PLAGUED_WARRIOR                 = 16984,

};

uint32 m_auiSpellSummonPlaguedWarrior[]=
{
    29247, 29248, 29249
};

uint32 m_auiSpellSummonPlaguedChampion[]=
{
    29217, 29224, 29225, 29227, 29238, 29255, 29257, 29258, 29262, 29267
};

uint32 m_auiSpellSummonPlaguedGuardian[]=
{
    29226, 29239, 29256, 29268
};

// Teleport position of Noth on his balcony
#define TELE_X 2631.370
#define TELE_Y -3529.680
#define TELE_Z 274.040
#define TELE_O 6.277

// IMPORTANT: BALCONY TELEPORT NOT ADDED YET! WILL BE ADDED SOON!
// Dev note 26.12.2008: When is soon? :)
// Dev note 12.10.2009: http://www.wowwiki.com/Soon

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 m_uiBlinkTimer;
    uint32 m_uiCurseTimer;
    uint32 m_uiSummonTimer;

    void Reset()
    {
        m_uiBlinkTimer = 25000;
        m_uiCurseTimer = 4000;
        m_uiSummonTimer = 12000;
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Blink
        if (m_uiBlinkTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CRIPPLE);
            DoCast(m_creature, SPELL_BLINK);

            m_uiBlinkTimer = 25000;
        }
        else
            m_uiBlinkTimer -= uiDiff;

        // Curse
        if (m_uiCurseTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CURSE_PLAGUEBRINGER);
            m_uiCurseTimer = 28000;
        }
        else
            m_uiCurseTimer -= uiDiff;

        // Summon
        if (m_uiSummonTimer < uiDiff)
        {
            DoScriptText(SAY_SUMMON, m_creature);

            for(uint8 i = 0; i < 6; ++i)
                m_creature->SummonCreature(NPC_PLAGUED_WARRIOR, 2684.804, -3502.517, 261.313, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 80000);

            m_uiSummonTimer = 30500;
        }
        else
            m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI(pCreature);
}

void AddSC_boss_noth()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_noth";
    NewScript->GetAI = &GetAI_boss_noth;
    NewScript->RegisterSelf();
}

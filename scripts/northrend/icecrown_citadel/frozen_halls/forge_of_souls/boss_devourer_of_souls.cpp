/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_devourer_of_souls
SD%Complete: 0%
SDComment: Placeholder
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    // TODO, how to change the face on random?, how to know which face is shown, to use the additional entries
    SAY_MALE_1_AGGRO            = -1632007,
    SAY_FEMALE_AGGRO            = -1632008,
    SAY_MALE_1_SLAY_1           = -1632009,
    SAY_FEMALE_SLAY_1           = -1632010,
    SAY_MALE_2_SLAY_1           = -1632011,
    SAY_MALE_1_SLAY_2           = -1632012,
    SAY_FEMALE_SLAY_2           = -1632013,
    SAY_MALE_2_SLAY_2           = -1632014,
    SAY_MALE_1_DEATH            = -1632015,
    SAY_FEMALE_DEATH            = -1632016,
    SAY_MALE_2_DEATH            = -1632017,
    SAY_MALE_1_SOUL_ATTACK      = -1632018,
    SAY_FEMALE_SOUL_ATTACK      = -1632019,
    SAY_MALE_2_SOUL_ATTACK      = -1632020,
    SAY_MALE_1_DARK_GLARE       = -1632021,
    SAY_FEMALE_DARK_GLARE       = -1632022,

    EMOTE_MIRRORED_SOUL         = -1632023,
    EMOTE_UNLEASH_SOULS         = -1632024,
    EMOTE_WAILING_SOULS         = -1632025,

    FACE_NORMAL                 = 0,
    FACE_UNLEASHING             = 1,
    FACE_WAILING                = 2,
};

static const int aTexts[6][3] =
{
    {SAY_MALE_1_AGGRO,       SAY_FEMALE_AGGRO,       0},                        // 0 - aggro
    {SAY_MALE_1_SLAY_1,      SAY_FEMALE_SLAY_1,      SAY_MALE_2_SLAY_1},        // 1 - slay1
    {SAY_MALE_1_SLAY_2,      SAY_FEMALE_SLAY_2,      SAY_MALE_2_SLAY_2},        // 2 - slay2
    {SAY_MALE_1_DEATH,       SAY_FEMALE_DEATH,       SAY_MALE_2_DEATH},         // 3 - death
    {SAY_MALE_1_SOUL_ATTACK, SAY_FEMALE_SOUL_ATTACK, SAY_MALE_2_SOUL_ATTACK},   // 4 - soul
    {SAY_MALE_1_DARK_GLARE,  SAY_FEMALE_DARK_GLARE,  0}                         // 5 - glare
};

struct MANGOS_DLL_DECL boss_devourer_of_soulsAI : public ScriptedAI
{
    boss_devourer_of_soulsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_forge_of_souls* m_pInstance;
    uint8 m_uiFace;
    bool m_bIsRegularMode;

    void Reset()
    {
        m_uiFace = FACE_NORMAL;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(aTexts[0][m_uiFace], m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DECOURER_OF_SOULS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(aTexts[urand(1, 2)][m_uiFace], m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(aTexts[3][m_uiFace], m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_DECOURER_OF_SOULS, DONE);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(NPC_DEVOURER_OF_SOULS, NOT_STARTED);
            // If we previously failed, set such that possible to try again
            m_pInstance->SetData(TYPE_ACHIEV_PHANTOM_BLAST, IN_PROGRESS);
        }
    }
};

CreatureAI* GetAI_boss_devourer_of_souls(Creature* pCreature)
{
    return new boss_devourer_of_soulsAI(pCreature);
}

void AddSC_boss_devourer_of_souls()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_devourer_of_souls";
    pNewScript->GetAI = &GetAI_boss_devourer_of_souls;
    pNewScript->RegisterSelf();
}

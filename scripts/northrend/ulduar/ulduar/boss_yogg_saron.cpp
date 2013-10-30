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
SDName: boss_yogg_saron
SD%Complete: 10%
SDComment: Very basic script
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_SARA_INTRO_1                            = -1603197,
    SAY_SARA_INTRO_2                            = -1603198,
    SAY_SARA_AGGRO                              = -1603199,
    SAY_SARA_HELP_1                             = -1603200,
    SAY_SARA_HELP_2                             = -1603201,
    SAY_SARA_SLAY_1                             = -1603202,
    SAY_SARA_SLAY_2                             = -1603203,
    SAY_WIPE_PHASE_1                            = -1603204,

    SAY_PHASE_2_INTRO                           = -1603205,
    SAY_SARA_PHASE_2_INTRO_A                    = -1603206,
    SAY_SARA_PHASE_2_INTRO_B                    = -1603207,

    SAY_MADNESS                                 = -1603209,
    SAY_PHASE_3                                 = -1603210,
    SAY_SLAY_1                                  = -1603211,
    SAY_SLAY_2                                  = -1603212,
    SAY_DEATH                                   = -1603213,
    SAY_TO_INSANE_1                             = -1603214,
    SAY_TO_INSANE_2                             = -1603215,

    SAY_LICH_KING_1                             = -1603216,
    SAY_CHAMPION_1                              = -1603217,
    SAY_CHAMPION_2                              = -1603218,
    SAY_LICH_KING_2                             = -1603219,
    SAY_YOGG_V3_1                               = -1603220,
    SAY_YOGG_V3_2                               = -1603221,

    SAY_NELTHARION_1                            = -1603222,
    SAY_YSERA                                   = -1603223,
    SAY_NELTHARION_2                            = -1603224,
    SAY_MALYGOS                                 = -1603225,
    SAY_YOGG_V2                                 = -1603226,

    SAY_GARONA_1                                = -1603227,
    SAY_GARONA_2                                = -1603228,
    SAY_YOGG_V1_1                               = -1603229,
    SAY_YOGG_V1_2                               = -1603230,
    SAY_GARONA_3                                = -1603231,
    SAY_GARONA_4                                = -1603232,
    SAY_YOGG_V1_3                               = -1603233,

    EMOTE_VISION_BLAST                          = -1603234,
    EMOTE_SHATTER_BLAST                         = -1603235,

    SAY_FREYA_HELP                              = -1603009,
    SAY_HODIR_HELP                              = -1603093,
    SAY_THORIM_HELP                             = -1603155,
    SAY_MIMIRON_HELP                            = -1603195,

    // generic spells
    SPELL_EXTINGUISH_LIFE                       = 64166,                    // berserk spell

    // Sara phase spells
    SPELL_SARAS_FERVOR                          = 63747,                    // triggers 63138
    SPELL_SARAS_BLESSING                        = 63745,                    // triggers 63134
    SPELL_SARAS_ANGER                           = 63744,                    // triggers 63147

    // guardian of yogg spells
    SPELL_SHADOW_NOVA                           = 62714,                    // used by the guardians when it dies
    SPELL_SHADOW_NOVA_H                         = 65209,
    SPELL_DARK_VOLLEY                           = 63038,
    SPELL_DOMINATE_MIND                         = 63713,

    // Vision phase spells
    SPELL_SHADOWY_BARRIER                       = 64775,                    // damage immunity spells
    SPELL_SHADOWY_BARRIER_YOGG                  = 63894,

    SPELL_SANITY                                = 63786,                    // sanity related spells
    SPELL_INSANE_PERIODIC                       = 64554,
    SPELL_CLEAR_INSANE                          = 63122,

    SPELL_PHYCHOSIS                             = 65301,                    // Sara combat spells
    SPELL_PHYCHOSIS_H                           = 63795,
    SPELL_MALADY_OF_THE_MIND                    = 63830,
    SPELL_BRAIN_LINK                            = 63802,
    SPELL_DEATH_RAY_SUMMON                      = 63891,

    SPELL_CORRUPTOR_TENTACLE                    = 64143,                    // tentacle spawn spells
    SPELL_CRUSHER_TENTACLE                      = 64139,
    SPELL_CONSTRICTOR_TENTACLE                  = 64133,

    // Old God phase spells
    SPELL_LUNATIC_GAZE_YOGG                     = 64163,
    SPELL_SHADOW_BEACON                         = 64465,
    SPELL_EMPOWERING_SHADOWS                    = 64468,
    SPELL_EMPOWERING_SHADOWS_H                  = 64486,
    SPELL_DEAFENING_ROAR                        = 64189,

    // death ray spells
    SPELL_DEATH_RAY_TRIGG                       = 63883,
    SPELL_DEATH_RAY_VISUAL_WARN                 = 63882,
    SPELL_DEATH_RAY_VISUAL_DAMAGE               = 63886,
    SPELL_DEATH_RAY_VISUAL_ORIGIN               = 63893,

    // descend into madness spells
    SPELL_TELEPORT_PORTAL_VISUAL                = 64416,
    SPELL_TELEPORT_TO_STORMWIND_ILLUSION        = 63989,
    SPELL_TELEPORT_TO_CHAMBER_ILLUSION          = 63997,
    SPELL_TELEPORT_TO_ICEECROWN_ILLUSION        = 63998,
    SPELL_TELEPORT_BACK_TO_MAIN_ROOM            = 63992,

    // summoned creatures
    NPC_GUARDIAN_OF_YOGG                        = 33136,
    NPC_BRAIN_OF_YOGG_SARON                     = 33890,
    NPC_DEATH_ORB                               = 33882,
    NPC_CONSTRICTOR_TENTACLE                    = 33983,
    NPC_CRUSHER_TENTACLE                        = 33966,
    NPC_CORRUPTOR_TENTACLE                      = 33985,
    NPC_DESCEND_INTO_MADNESS                    = 34072,
    NPC_IMMORTAL_GUARDIAN                       = 33988,

    // keepers
    // Freya spells
    SPELL_RESILIENCE_OF_NATURE                  = 62670,
    SPELL_SUMMON_SANITY_WELL                    = 64170,                    // sends event 21432; used to spawn npc 33991

    // sanity well spells
    SPELL_SANITY_WELL                           = 64169,
    SPELL_SANITY_WELL_VISUAL                    = 63288,

    // Hodir spells
    SPELL_FORTITUDE_OF_FROST                    = 62650,
    SPELL_HODIRS_PROTECTIVE_GAZE                = 64174,

    // Mimiron spells
    SPELL_SPEED_OF_INVENTION                    = 62671,
    SPELL_DESTABILIZATION_MATRIX                = 65206,

    // Thorim spells
    SPELL_FURY_OF_THE_STORM                     = 62702,
    SPELL_TITANIC_STORM                         = 64171,

    // encounter phases
    PHASE_INTRO                                 = 0,
    PHASE_SARA                                  = 1,
    PHASE_VISIONS                               = 2,
    PHASE_OLD_GOD                               = 3,
};

/*######
## boss_sara
######*/

struct MANGOS_DLL_DECL boss_saraAI : public ScriptedAI
{
    boss_saraAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    void Reset() override
    {
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        DoScriptText(urand(0, 1) ? SAY_SARA_SLAY_1 : SAY_SARA_SLAY_2, m_creature);
    }

    void JustSummoned(Creature* pSummoned) override
    {
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_boss_sara(Creature* pCreature)
{
    return new boss_saraAI(pCreature);
}

void AddSC_boss_yogg_saron()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_sara";
    pNewScript->GetAI = &GetAI_boss_sara;
    pNewScript->RegisterSelf();
}

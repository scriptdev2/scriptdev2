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
SDName: Boss_KelThuzud
SD%Complete: 50
SDComment: In progress, phase 1 not implemented, timers will need adjustments
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //when shappiron dies. dialog between kel and lich king (in this order)
    SAY_SAPP_DIALOG1                    = -1533084,
    SAY_SAPP_DIALOG2_LICH               = -1533085,
    SAY_SAPP_DIALOG3                    = -1533086,
    SAY_SAPP_DIALOG4_LICH               = -1533087,
    SAY_SAPP_DIALOG5                    = -1533088,

    //when cat dies
    SAY_CAT_DIED                        = -1533089,

    //when each of the 4 wing bosses dies
    SAY_TAUNT1                          = -1533090,
    SAY_TAUNT2                          = -1533091,
    SAY_TAUNT3                          = -1533092,
    SAY_TAUNT4                          = -1533093,

    SAY_SUMMON_MINIONS                  = -1533105,         //start of phase 1

    EMOTE_PHASE2                        = -1533135,         //start of phase 2
    SAY_AGGRO1                          = -1533094,
    SAY_AGGRO2                          = -1533095,
    SAY_AGGRO3                          = -1533096,

    SAY_SLAY1                           = -1533097,
    SAY_SLAY2                           = -1533098,

    SAY_DEATH                           = -1533099,

    SAY_CHAIN1                          = -1533100,
    SAY_CHAIN2                          = -1533101,
    SAY_FROST_BLAST                     = -1533102,

    SAY_REQUEST_AID                     = -1533103,         //start of phase 3
    SAY_ANSWER_REQUEST                  = -1533104,         //lich king answer

    SAY_SPECIAL1_MANA_DET               = -1533106,
    SAY_SPECIAL3_MANA_DET               = -1533107,
    SAY_SPECIAL2_DISPELL                = -1533108,

    EMOTE_GUARDIAN                      = -1533134,         // at each guardian summon

    //spells to be casted
    SPELL_FROST_BOLT                    = 28478,
    SPELL_FROST_BOLT_H                  = 55802,
    SPELL_FROST_BOLT_NOVA               = 28479,
    SPELL_FROST_BOLT_NOVA_H             = 55807,

    SPELL_CHAINS_OF_KELTHUZAD           = 28408,            // 3.x, heroic only
    SPELL_CHAINS_OF_KELTHUZAD_TARGET    = 28410,

    SPELL_MANA_DETONATION               = 27819,
    SPELL_SHADOW_FISSURE                = 27810,
    SPELL_FROST_BLAST                   = 27808
};

static float M_F_ANGLE = 0.2f;                              // to adjust for map rotation
static float M_F_RANGE = 55.0f;                             // ~ range from center of chamber to center of alcove

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        memset(&m_auiGuardiansGUID, 0, sizeof(m_auiGuardiansGUID));
        m_uiGuardiansCount = 0;
        m_uiGuardiansCountMax = m_bIsRegularMode ? 2 : 4;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_auiGuardiansGUID[5];
    uint32 m_uiGuardiansCount;
    uint32 m_uiGuardiansCountMax;
    uint32 m_uiGuardiansTimer;
    uint32 m_uiFrostBoltTimer;
    uint32 m_uiFrostBoltNovaTimer;
    uint32 m_uiChainsTimer;
    uint32 m_uiManaDetonationTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiFrostBlastTimer;
    uint32 m_uiPhase1Timer;

    bool m_bIsPhase2;
    bool m_bIsPhase3;

    void Reset()
    {
        m_uiFrostBoltTimer = urand(1000, 600000);           //It won't be more than a minute without cast it
        m_uiFrostBoltNovaTimer = 15000;                     //Cast every 15 seconds
        m_uiChainsTimer = urand(30000, 60000);              //Cast no sooner than once every 30 seconds
        m_uiManaDetonationTimer = 20000;                    //Seems to cast about every 20 seconds
        m_uiShadowFissureTimer = 25000;                     //25 seconds
        m_uiFrostBlastTimer = urand(30000, 60000);          //Random time between 30-60 seconds
        m_uiGuardiansTimer = 5000;                          //5 seconds for summoning each Guardian of Icecrown in phase 3

        for(int i=0; i<5; ++i)
        {
            if (m_auiGuardiansGUID[i])
            {
                //delete creature
                if (Creature* pGuardian = (Creature*)Unit::GetUnit(*m_creature, m_auiGuardiansGUID[i]))
                {
                    if (pGuardian->isAlive())
                        pGuardian->ForcedDespawn();
                }

                m_auiGuardiansGUID[i] = 0;
            }
        }

        m_uiPhase1Timer = 310000;                           //Phase 1 lasts 5 minutes and 10 seconds
        m_bIsPhase2 = false;
        m_bIsPhase3 = false;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        for(int i=0; i<5; ++i)
        {
            if (m_auiGuardiansGUID[i])
            {
                Creature* pGuardian = (Creature*)Unit::GetUnit(*m_creature, m_auiGuardiansGUID[i]);

                if (!pGuardian || !pGuardian->isAlive())
                    continue;

                pGuardian->AI()->EnterEvadeMode();
            }
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, DONE);
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
            m_pInstance->SetData(TYPE_KELTHUZAD, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, FAIL);
    }

    float GetLocationAngle(uint32 uiId)
    {
        switch(uiId)
        {
            case 1: return M_PI_F - M_F_ANGLE;              // south
            case 2: return (M_PI_F / 2) * 3 - M_F_ANGLE;    // east
            case 3: return M_PI_F / 2 - M_F_ANGLE;          // west
            case 4: return M_PI_F / 4 - M_F_ANGLE;          // north-west
            case 5: return (M_PI_F / 4) * 7 - M_F_ANGLE;    // north-east
            case 6: return (M_PI_F / 4) * 5 - M_F_ANGLE;    // south-east
            case 7: return 3*M_PI_F / 4 - M_F_ANGLE;        // south-west
        }

        return M_F_ANGLE;
    }

    void SummonGuardian()
    {
        if (!m_pInstance)
            return;

        float fAngle = GetLocationAngle(urand(1,7));

        // may fail and give invalid coordinates, so later need to make sure areatrigger really visited.
        float fx, fy, fz;
        m_pInstance->GetChamberCenterCoords(fx, fy, fz);

        fx += M_F_RANGE * cos(fAngle);
        fy += M_F_RANGE * sin(fAngle);

        MaNGOS::NormalizeMapCoord(fx);
        MaNGOS::NormalizeMapCoord(fy);

        m_creature->SummonCreature(NPC_GUARDIAN, fx, fy, m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_GUARDIAN)
        {
            DoScriptText(EMOTE_GUARDIAN, m_creature);

            if (m_pInstance)
            {
                float fx, fy, fz;
                m_pInstance->GetChamberCenterCoords(fx, fy, fz);
                pSummoned->GetMotionMaster()->MovePoint(0, fx, fy, fz);
            }

            //Safe storing of creatures
            m_auiGuardiansGUID[m_uiGuardiansCount] = pSummoned->GetGUID();

            //Update guardian count
            ++m_uiGuardiansCount;
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType == POINT_MOTION_TYPE && uiPointId == 0)
            pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostBoltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT : SPELL_FROST_BOLT_H);
            m_uiFrostBoltTimer = urand(1000, 60000);
        }
        else
            m_uiFrostBoltTimer -= uiDiff;

        if (m_uiFrostBoltNovaTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT_NOVA : SPELL_FROST_BOLT_NOVA_H);
            m_uiFrostBoltNovaTimer = 15000;
        }
        else
            m_uiFrostBoltNovaTimer -= uiDiff;

        if (m_uiChainsTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAINS_OF_KELTHUZAD);

            DoScriptText(urand(0, 1) ? SAY_CHAIN1 : SAY_CHAIN2, m_creature);

            m_uiChainsTimer = urand(30000, 60000);
        }
        else
            m_uiChainsTimer -= uiDiff;

        if (m_uiManaDetonationTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MANA_DETONATION);

            if (urand(0, 1))
                DoScriptText(SAY_SPECIAL1_MANA_DET, m_creature);

            m_uiManaDetonationTimer = 20000;
        }
        else
            m_uiManaDetonationTimer -= uiDiff;

        if (m_uiShadowFissureTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_FISSURE);

            if (urand(0, 1))
                DoScriptText(SAY_SPECIAL3_MANA_DET, m_creature);

            m_uiShadowFissureTimer = 25000;
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        if (m_uiFrostBlastTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_BLAST);

            if (urand(0, 1))
                DoScriptText(SAY_FROST_BLAST, m_creature);

            m_uiFrostBlastTimer = urand(30000, 60000);
        }
        else
            m_uiFrostBlastTimer -= uiDiff;

        //start phase 3 when we are 40% health
        if (!m_bIsPhase3 && m_creature->GetHealthPercent() < 40.0f)
        {
            m_bIsPhase3 = true;
            DoScriptText(SAY_REQUEST_AID, m_creature);

            //here Lich King should respond to KelThuzad but I don't know which creature to make talk
            //so for now just make Kelthuzad says it.
            DoScriptText(SAY_ANSWER_REQUEST, m_creature);
        }

        if (m_bIsPhase3 && m_uiGuardiansCount < m_uiGuardiansCountMax)
        {
            if (m_uiGuardiansTimer < uiDiff)
            {
                //Summon a Guardian of Icecrown in a random alcove
                SummonGuardian();

                //5 seconds until summoning next guardian
                m_uiGuardiansTimer = 5000;
            }
            else
                m_uiGuardiansTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kelthuzad(Creature* pCreature)
{
    return new boss_kelthuzadAI(pCreature);
}

void AddSC_boss_kelthuzad()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kelthuzad";
    NewScript->GetAI = &GetAI_boss_kelthuzad;
    NewScript->RegisterSelf();
}

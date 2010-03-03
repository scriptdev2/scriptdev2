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
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

//***THIS SCRIPTS IS UNDER DEVELOPMENT***
/*
DATA.
This script has been made with info taken from wowwikki... so there are things wrong...
like spell timers and Says. Also there's another major problem as there is no aggroed list
I cannot make Kel'thuzad to target specific party members, that is needed for spells like
Mana Detonation... so what I'm doing untill now is just to cast everything on my main aggroed
target. Sorry for him.
Another bug is that there are spells that are actually NOT working... I have to implement
them first.
Need DISPELL efect
I also don't know the emotes
*/

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

    SAY_AGGRO1                          = -1533094,         //start of phase 2
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

    EMOTE_GUARDIANS                     = -1533134,

    //spells to be casted
    SPELL_FROST_BOLT                    = 28478,
    SPELL_FROST_BOLT_H                  = 55802,
    SPELL_FROST_BOLT_NOVA               = 28479,
    SPELL_FROST_BOLT_NOVA_H             = 55807,

    SPELL_CHAINS_OF_KELTHUZAD           = 28408,            // 3.x, heroic only
    SPELL_CHAINS_OF_KELTHUZAD_TARGET    = 28410,

    SPELL_MANA_DETONATION               = 27819,
    SPELL_SHADOW_FISSURE                = 27810,
    SPELL_FROST_BLAST                   = 27808,

    NPC_GUARDIAN                        = 16441,
};

//Positional defines
#define ADDX_LEFT_FAR               3783.272705f
#define ADDY_LEFT_FAR               -5062.697266f
#define ADDZ_LEFT_FAR               143.711203f
#define ADDO_LEFT_FAR               3.617599f

#define ADDX_LEFT_MIDDLE            3730.291260f
#define ADDY_LEFT_MIDDLE            -5027.239258f
#define ADDZ_LEFT_MIDDLE            143.956909f
#define ADDO_LEFT_MIDDLE            4.461900f

#define ADDX_LEFT_NEAR              3683.868652f
#define ADDY_LEFT_NEAR              -5057.281250f
#define ADDZ_LEFT_NEAR              143.183884f
#define ADDO_LEFT_NEAR              5.237086f

#define ADDX_RIGHT_FAR              3759.355225f
#define ADDY_RIGHT_FAR              -5174.128418f
#define ADDZ_RIGHT_FAR              143.802383f
#define ADDO_RIGHT_FAR              2.170104f

#define ADDX_RIGHT_MIDDLE           370.724365f
#define ADDY_RIGHT_MIDDLE           -5185.123047f
#define ADDZ_RIGHT_MIDDLE           143.928024f
#define ADDO_RIGHT_MIDDLE           1.309310f

#define ADDX_RIGHT_NEAR             3665.121094f
#define ADDY_RIGHT_NEAR             -5138.679199f
#define ADDZ_RIGHT_NEAR             143.183212f
#define ADDO_RIGHT_NEAR             0.604023f

#define WALKX_LEFT_FAR              3754.431396f
#define WALKY_LEFT_FAR              -5080.727734f
#define WALKZ_LEFT_FAR              142.036316f
#define WALKO_LEFT_FAR              3.736189f

#define WALKX_LEFT_MIDDLE           3724.396484f
#define WALKY_LEFT_MIDDLE           -5061.330566f
#define WALKZ_LEFT_MIDDLE           142.032700f
#define WALKO_LEFT_MIDDLE           4.564785f

#define WALKX_LEFT_NEAR             3687.158424f
#define WALKY_LEFT_NEAR             -5076.834473f
#define WALKZ_LEFT_NEAR             142.017319f
#define WALKO_LEFT_NEAR             5.237086f

#define WALKX_RIGHT_FAR             3687.571777f
#define WALKY_RIGHT_FAR             -5126.831055f
#define WALKZ_RIGHT_FAR             142.017807f
#define WALKO_RIGHT_FAR             0.604023f

#define WALKX_RIGHT_MIDDLE          3707.990733f
#define WALKY_RIGHT_MIDDLE          -5151.450195f
#define WALKZ_RIGHT_MIDDLE          142.032562f
#define WALKO_RIGHT_MIDDLE          1.376855f

#define WALKX_RIGHT_NEAR            3739.500000f
#define WALKY_RIGHT_NEAR            -5141.883989f
#define WALKZ_RIGHT_NEAR            142.0141130f
#define WALKO_RIGHT_NEAR            2.121412f

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        memset(&m_auiGuardiansGUID, 0, sizeof(m_auiGuardiansGUID));
        m_uiGuardiansCount = 0;

        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_auiGuardiansGUID[5];
    uint32 m_uiGuardiansCount;
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

    float m_fWalk_Pos_X;
    float m_fWalk_Pos_Y;
    float m_fWalk_Pos_Z;

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

        m_fWalk_Pos_X = 0.0f;
        m_fWalk_Pos_Y = 0.0f;
        m_fWalk_Pos_Z = 0.0f;
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

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_GUARDIAN)
        {
            pSummoned->GetMotionMaster()->MovePoint(0, m_fWalk_Pos_X, m_fWalk_Pos_Y, m_fWalk_Pos_Z);

            //Safe storing of creatures
            m_auiGuardiansGUID[m_uiGuardiansCount] = pSummoned->GetGUID();

            //Update guardian count
            ++m_uiGuardiansCount;
        }
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

        if (m_bIsPhase3 && m_uiGuardiansCount < 5)
        {
            if (m_uiGuardiansTimer < uiDiff)
            {
                //Summon a Guardian of Icecrown in a random alcove

                switch(urand(0, 5))
                {
                    case 0:
                        //Setting walk position
                        m_fWalk_Pos_X = WALKX_LEFT_FAR;
                        m_fWalk_Pos_Y = WALKY_LEFT_FAR;
                        m_fWalk_Pos_Z = WALKZ_LEFT_FAR;
                        m_creature->SummonCreature(NPC_GUARDIAN, ADDX_LEFT_FAR,ADDY_LEFT_FAR,ADDZ_LEFT_FAR,ADDO_LEFT_FAR,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                        break;
                    case 1:
                        //Start moving guardian towards the center of the room
                        m_fWalk_Pos_X = WALKX_LEFT_MIDDLE;
                        m_fWalk_Pos_Y = WALKY_LEFT_MIDDLE;
                        m_fWalk_Pos_Z = WALKZ_LEFT_MIDDLE;
                        m_creature->SummonCreature(NPC_GUARDIAN, ADDX_LEFT_MIDDLE,ADDY_LEFT_MIDDLE,ADDZ_LEFT_MIDDLE,ADDO_LEFT_MIDDLE,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                        break;
                    case 2:
                        //Start moving guardian towards the center of the room
                        m_fWalk_Pos_X = WALKX_LEFT_NEAR;
                        m_fWalk_Pos_Y = WALKY_LEFT_NEAR;
                        m_fWalk_Pos_Z = WALKZ_LEFT_NEAR;
                        m_creature->SummonCreature(NPC_GUARDIAN, ADDX_LEFT_NEAR,ADDY_LEFT_NEAR,ADDZ_LEFT_NEAR,ADDO_LEFT_NEAR,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                        break;
                    case 3:
                        //Start moving guardian towards the center of the room
                        m_fWalk_Pos_X = WALKX_RIGHT_FAR;
                        m_fWalk_Pos_Y = WALKY_RIGHT_FAR;
                        m_fWalk_Pos_Z = WALKZ_RIGHT_FAR;
                        m_creature->SummonCreature(NPC_GUARDIAN, ADDX_RIGHT_FAR,ADDY_RIGHT_FAR,ADDZ_RIGHT_FAR,ADDO_RIGHT_FAR,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                        break;
                    case 4:
                        //Start moving guardian towards the center of the room
                        m_fWalk_Pos_X = WALKX_RIGHT_MIDDLE;
                        m_fWalk_Pos_Y = WALKY_RIGHT_MIDDLE;
                        m_fWalk_Pos_Z = WALKZ_RIGHT_MIDDLE;
                        m_creature->SummonCreature(NPC_GUARDIAN, ADDX_RIGHT_MIDDLE,ADDY_RIGHT_MIDDLE,ADDZ_RIGHT_MIDDLE,ADDO_RIGHT_MIDDLE,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                        break;
                    case 5:
                        //Start moving guardian towards the center of the room
                        m_fWalk_Pos_X = WALKX_RIGHT_NEAR;
                        m_fWalk_Pos_Y = WALKY_RIGHT_NEAR;
                        m_fWalk_Pos_Z = WALKZ_RIGHT_NEAR;
                        m_creature->SummonCreature(NPC_GUARDIAN, ADDX_RIGHT_NEAR,ADDY_RIGHT_NEAR,ADDZ_RIGHT_NEAR,ADDO_RIGHT_NEAR,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,1000);
                        break;
                }

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

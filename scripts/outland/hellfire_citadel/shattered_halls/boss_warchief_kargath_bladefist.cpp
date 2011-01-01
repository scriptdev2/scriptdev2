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
SDName: Boss_Warchief_Kargath_Bladefist
SD%Complete: 90
SDComment:
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

/* ContentData
boss_warchief_kargath_bladefist
EndContentData */

#include "precompiled.h"

#define SAY_AGGRO1                      -1540042
#define SAY_AGGRO2                      -1540043
#define SAY_AGGRO3                      -1540044
#define SAY_SLAY1                       -1540045
#define SAY_SLAY2                       -1540046
#define SAY_DEATH                       -1540047

#define SPELL_BLADE_DANCE               30739
#define H_SPELL_CHARGE                  25821

#define TARGET_NUM                      5

#define MOB_SHATTERED_ASSASSIN          17695
#define MOB_HEARTHEN_GUARD              17621
#define MOB_SHARPSHOOTER_GUARD          17622
#define MOB_REAVER_GUARD                17623

float AssassEntrance[3] = {275.136f, -84.29f, 2.3f};        // y -8
float AssassExit[3] = {184.233f, -84.29f, 2.3f};            // y -8
float AddsEntrance[3] = {306.036f, -84.29f, 1.93f};

struct MANGOS_DLL_DECL boss_warchief_kargath_bladefistAI : public ScriptedAI
{
    boss_warchief_kargath_bladefistAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    std::vector<uint64> adds;
    std::vector<uint64> assassins;

    uint32 Charge_timer;
    uint32 Blade_Dance_Timer;
    uint32 Summon_Assistant_Timer;
    uint32 resetcheck_timer;
    uint32 Wait_Timer;

    uint32 Assassins_Timer;

    uint32 summoned;
    bool InBlade;

    uint32 target_num;

    void Reset()
    {
        removeAdds();

        m_creature->SetSpeedRate(MOVE_RUN, 2.0f);

        summoned = 2;
        InBlade = false;
        Wait_Timer = 0;

        Charge_timer = 0;
        Blade_Dance_Timer = 45000;
        Summon_Assistant_Timer = 30000;
        Assassins_Timer = 5000;
        resetcheck_timer = 5000;
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void JustSummoned(Creature *summoned)
    {
        switch(summoned->GetEntry())
        {
            case MOB_HEARTHEN_GUARD:
            case MOB_SHARPSHOOTER_GUARD:
            case MOB_REAVER_GUARD:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    summoned->AI()->AttackStart(pTarget);

                adds.push_back(summoned->GetGUID());
                break;
            case MOB_SHATTERED_ASSASSIN:
                assassins.push_back(summoned->GetGUID());
                break;
        }
    }

    void KilledUnit(Unit *victim)
    {
        if (victim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        removeAdds();
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (InBlade)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id != 1)
                return;

            if (target_num > 0) // to prevent loops
            {
                Wait_Timer = 1;
                DoCastSpellIfCan(m_creature, SPELL_BLADE_DANCE, CAST_TRIGGERED);
                --target_num;
            }
        }
    }

    void removeAdds()
    {
        if (!m_pInstance)
            return;

        for(std::vector<uint64>::iterator itr = adds.begin(); itr!= adds.end(); ++itr)
        {
            if (Creature* pTemp = m_pInstance->instance->GetCreature(*itr))
                pTemp->ForcedDespawn();
        }

        adds.clear();

        for(std::vector<uint64>::iterator itr = assassins.begin(); itr!= assassins.end(); ++itr)
        {
            if (Creature* pTemp = m_pInstance->instance->GetCreature(*itr))
                pTemp->ForcedDespawn();
        }

        assassins.clear();
    }

    void SpawnAssassin()
    {
        m_creature->SummonCreature(MOB_SHATTERED_ASSASSIN,AssassEntrance[0],AssassEntrance[1]+8, AssassEntrance[2], 0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
        m_creature->SummonCreature(MOB_SHATTERED_ASSASSIN,AssassEntrance[0],AssassEntrance[1]-8, AssassEntrance[2], 0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
        m_creature->SummonCreature(MOB_SHATTERED_ASSASSIN,AssassExit[0],AssassExit[1]+8, AssassExit[2], 0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
        m_creature->SummonCreature(MOB_SHATTERED_ASSASSIN,AssassExit[0],AssassExit[1]-8, AssassExit[2], 0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Assassins_Timer)
            if (Assassins_Timer <= diff)
            {
                SpawnAssassin();
                Assassins_Timer = 0;
            }else Assassins_Timer -= diff;

        if (InBlade)
        {
            if (Wait_Timer)
                if (Wait_Timer <= diff)
                {
                    if (target_num <= 0)
                    {
                        // stop bladedance
                        InBlade = false;
                        m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
                        (*m_creature).GetMotionMaster()->MoveChase(m_creature->getVictim());
                        Wait_Timer = 0;
                        if (!m_bIsRegularMode)
                            Charge_timer = 5000;
                    }
                    else
                    {
                        //move in bladedance
                        float x,y,randx,randy;
                        randx = (rand()%40);
                        randy = (rand()%40);
                        x = 210+ randx ;
                        y = -60- randy ;
                        (*m_creature).GetMotionMaster()->MovePoint(1,x,y,m_creature->GetPositionZ());
                        Wait_Timer = 0;
                    }
                }else Wait_Timer -= diff;
        }
        else
        {
            if (Blade_Dance_Timer < diff)
            {
                target_num = TARGET_NUM;
                Wait_Timer = 1;
                InBlade = true;
                Blade_Dance_Timer = 30000;
                m_creature->SetSpeedRate(MOVE_RUN, 4.0f);
                return;
            }else Blade_Dance_Timer -= diff;

            if (Charge_timer)
                if (Charge_timer <= diff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                        DoCastSpellIfCan(pTarget, H_SPELL_CHARGE);

                    Charge_timer = 0;
                }else Charge_timer -= diff;

            if (Summon_Assistant_Timer < diff)
            {
                Unit* target = NULL;

                for(uint32 i = 0; i < summoned; ++i)
                {
                    switch(urand(0, 2))
                    {
                        case 0: m_creature->SummonCreature(MOB_HEARTHEN_GUARD, AddsEntrance[0], AddsEntrance[1], AddsEntrance[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000); break;
                        case 1: m_creature->SummonCreature(MOB_SHARPSHOOTER_GUARD, AddsEntrance[0], AddsEntrance[1], AddsEntrance[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000); break;
                        case 2: m_creature->SummonCreature(MOB_REAVER_GUARD, AddsEntrance[0], AddsEntrance[1], AddsEntrance[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000); break;
                    }
                }

                if (!urand(0, 4))
                    ++summoned;

                Summon_Assistant_Timer = urand(25000, 35000);
            }
            else Summon_Assistant_Timer -= diff;

            DoMeleeAttackIfReady();
        }

        if (resetcheck_timer < diff)
        {
            uint32 tempx,tempy;
            tempx = uint32(m_creature->GetPositionX());
            tempy = uint32(m_creature->GetPositionY());
            if (tempx > 255 || tempx < 205)
            {
                EnterEvadeMode();
            }
            resetcheck_timer = 5000;
        }else resetcheck_timer -= diff;
    }
};

CreatureAI* GetAI_boss_warchief_kargath_bladefist(Creature* pCreature)
{
    return new boss_warchief_kargath_bladefistAI(pCreature);
}

void AddSC_boss_warchief_kargath_bladefist()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_warchief_kargath_bladefist";
    newscript->GetAI = &GetAI_boss_warchief_kargath_bladefist;
    newscript->RegisterSelf();
}

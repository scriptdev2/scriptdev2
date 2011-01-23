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
SDName: Boss_Magtheridon
SD%Complete: 80
SDComment: Some spell issues with target selection.
SDCategory: Hellfire Citadel, Magtheridon's lair
EndScriptData */

#include "precompiled.h"
#include "magtheridons_lair.h"

struct Yell
{
    int32 id;
};

static Yell RandomTaunt[]=
{
    {-1544000},
    {-1544001},
    {-1544002},
    {-1544003},
    {-1544004},
    {-1544005},
};

enum
{
    SAY_FREED                   = -1544006,
    SAY_AGGRO                   = -1544007,
    SAY_BANISH                  = -1544008,
    SAY_CHAMBER_DESTROY         = -1544009,
    SAY_PLAYER_KILLED           = -1544010,
    SAY_DEATH                   = -1544011,

    EMOTE_GENERIC_ENRAGED       = -1000003,
    EMOTE_BLASTNOVA             = -1544013,
    EMOTE_BEGIN                 = -1544014,
    EMOTE_FREED                 = -1544015,

    SPELL_BLASTNOVA             = 30616,
    SPELL_CLEAVE                = 30619,
    SPELL_QUAKE_TRIGGER         = 30576,                    // must be cast with 30561 as the proc spell
    SPELL_QUAKE_KNOCKBACK       = 30571,

    SPELL_BLAZE_TRAP            = 30542,
    SPELL_DEBRIS_VISUAL         = 30632,
    SPELL_CAMERA_SHAKE          = 36455,
    SPELL_BERSERK               = 27680,

    SPELL_SHADOW_CAGE_DUMMY     = 30205,
    SPELL_SHADOW_CAGE           = 30168,

    SPELL_SHADOW_GRASP_DUMMY    = 30207,
    SPELL_SHADOW_GRASP          = 30410,
    SPELL_SHADOW_GRASP_VISUAL   = 30166,
    SPELL_MIND_EXHAUSTION       = 44032,                    // Casted by the cubes when channeling ends

    SPELL_FIRE_BLAST            = 37110,

    SPELL_SHADOW_BOLT_VOLLEY    = 30510,
    SPELL_DARK_MENDING          = 30528,
    SPELL_FEAR                  = 30530,                    // 39176
    SPELL_BURNING_ABYSSAL       = 30511,

    NPC_MAGS_ROOM               = 17516,
    NPC_BURNING_ABYSS           = 17454,

    // count of clickers needed to interrupt blast nova
    MAX_CLICK                   = 5
};

typedef std::map<uint64, uint64> CubeMap;

struct MANGOS_DLL_DECL mob_abyssalAI : public ScriptedAI
{
    mob_abyssalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiTriggerId = 0;
        m_uiDespawn_Timer = 60000;
        Reset();
    }

    uint32 m_uiFireBlast_Timer;
    uint32 m_uiDespawn_Timer;
    uint32 m_uiTriggerId;

    void Reset()
    {
        m_uiFireBlast_Timer = 6000;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (m_uiTriggerId == 2 && pSpell->Id == SPELL_BLAZE_TARGET)
        {
            m_creature->CastSpell(m_creature, SPELL_BLAZE_TRAP, true);
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_uiDespawn_Timer = 130000;
        }
    }

    void SetTrigger(uint32 uiTrigger)
    {
        m_uiTriggerId = uiTrigger;
        m_creature->SetDisplayId(11686);

        if (m_uiTriggerId == 1)                             //debris
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->CastSpell(m_creature, SPELL_DEBRIS_VISUAL, true);
            m_uiFireBlast_Timer = 5000;
            m_uiDespawn_Timer = 10000;
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void AttackStart(Unit* pWho)
    {
        if (m_uiTriggerId)
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_uiTriggerId)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiTriggerId)
        {
            if (m_uiTriggerId == 1)
            {
                if (m_uiFireBlast_Timer < uiDiff)
                {
                    m_creature->CastSpell(m_creature, SPELL_DEBRIS_DAMAGE, true);
                    m_uiTriggerId = 3;
                }
                else
                    m_uiFireBlast_Timer -= uiDiff;
            }
        }

        if (m_uiDespawn_Timer < uiDiff)
        {
            m_creature->ForcedDespawn();
            return;
        }
        else
            m_uiDespawn_Timer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFireBlast_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIRE_BLAST);
            m_uiFireBlast_Timer = urand(5000, 15000);
        }
        else
            m_uiFireBlast_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_magtheridonAI : public ScriptedAI
{
    boss_magtheridonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    CubeMap Cube;

    ScriptedInstance* m_pInstance;

    uint32 m_uiRandChat_Timer;

    uint32 m_uiBerserk_Timer;
    uint32 m_uiQuake_Timer;
    uint32 m_uiCleave_Timer;
    uint32 m_uiBlastNova_Timer;
    uint32 m_uiBlaze_Timer;
    uint32 m_uiPhase3_Timer;
    uint32 m_uiPhase3_Count;

    bool m_bIsIntroDone;
    bool m_bIsPhase3;
    bool m_bNeedCheckCube;

    void Reset()
    {
        m_uiRandChat_Timer = 90000;

        m_uiBerserk_Timer = 1320000;
        m_uiQuake_Timer = 40000;
        m_uiPhase3_Timer = 5000;
        m_uiPhase3_Count = 0;
        m_uiBlaze_Timer = urand(10000, 30000);
        m_uiBlastNova_Timer = 60000;
        m_uiCleave_Timer = 15000;

        m_bIsIntroDone = false;
        m_bIsPhase3 = false;
        m_bNeedCheckCube = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_MAGTHERIDON_EVENT, NOT_STARTED);
            m_pInstance->SetData(TYPE_HALL_COLLAPSE, NOT_STARTED);
        }
    }

    void SetClicker(uint64 uiCubeGUID, uint64 uiClickerGUID)
    {
        // to avoid multiclicks from 1 cube
        if (uint64 guid = Cube[uiCubeGUID])
            DebuffClicker(m_creature->GetMap()->GetPlayer(guid));

        Cube[uiCubeGUID] = uiClickerGUID;
        m_bNeedCheckCube = true;
    }

    //function to interrupt channeling and debuff clicker with mind exhaused if second person clicks with same cube or after dispeling/ending shadow grasp DoT)
    void DebuffClicker(Player* pClicker)
    {
        if (!pClicker || !pClicker->isAlive())
            return;

        pClicker->RemoveAurasDueToSpell(SPELL_SHADOW_GRASP);// cannot interrupt triggered spells
        pClicker->InterruptNonMeleeSpells(false);
        pClicker->CastSpell(pClicker, SPELL_MIND_EXHAUSTION, true);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bIsIntroDone)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void NeedCheckCubeStatus()
    {
        uint32 ClickerNum = 0;

        // now checking if every clicker has debuff from manticron
        // if not - apply mind exhaustion and delete from clicker's list
        for(CubeMap::iterator i = Cube.begin(); i != Cube.end(); ++i)
        {
            Player* pClicker = m_creature->GetMap()->GetPlayer(i->second);

            if (!pClicker || !pClicker->HasAura(SPELL_SHADOW_GRASP, EFFECT_INDEX_1))
            {
                DebuffClicker(pClicker);
                i->second = 0;
            }
            else
                ++ClickerNum;
        }

        // if 5 clickers from other cubes apply shadow cage
        if (ClickerNum >= MAX_CLICK && !m_creature->HasAura(SPELL_SHADOW_CAGE, EFFECT_INDEX_0) && m_creature->HasAura(SPELL_BLASTNOVA, EFFECT_INDEX_0))
        {
            DoScriptText(SAY_BANISH, m_creature);
            m_creature->CastSpell(m_creature, SPELL_SHADOW_CAGE, true);
        }
        else
        {
            if (ClickerNum < MAX_CLICK && m_creature->HasAura(SPELL_SHADOW_CAGE, EFFECT_INDEX_0))
                m_creature->RemoveAurasDueToSpell(SPELL_SHADOW_CAGE);
        }

        if (!ClickerNum)
            m_bNeedCheckCube = false;
    }

    void IntroDone()
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_MAGTHERIDON_EVENT) == NOT_STARTED)
            return;

        if (m_pInstance->GetData(TYPE_MAGTHERIDON_EVENT) == DONE)
            return;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveAurasDueToSpell(SPELL_SHADOW_CAGE_DUMMY);
        m_creature->clearUnitState(UNIT_STAT_STUNNED);

        DoScriptText(EMOTE_FREED, m_creature);
        DoScriptText(SAY_FREED, m_creature);

        m_bIsIntroDone = true;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SHADOW_GRASP_DUMMY)
        {
            m_creature->CastSpell(m_creature, SPELL_SHADOW_CAGE_DUMMY, false);
            m_creature->addUnitState(UNIT_STAT_STUNNED);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_PLAYER_KILLED, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAGTHERIDON_EVENT, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (!m_bIsIntroDone)
            {
                IntroDone();
                return;
            }

            if (m_uiRandChat_Timer < uiDiff)
            {
                DoScriptText(RandomTaunt[rand()%6].id, m_creature);
                m_uiRandChat_Timer = 90000;
            }
            else
                m_uiRandChat_Timer -= uiDiff;

            return;
        }

        if (m_bNeedCheckCube)
            NeedCheckCubeStatus();

        if (m_uiBerserk_Timer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(EMOTE_GENERIC_ENRAGED, m_creature);
                m_uiBerserk_Timer = 60000;
            }
        }
        else
            m_uiBerserk_Timer -= uiDiff;

        //Cleave_Timer
        if (m_uiCleave_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleave_Timer = 10000;
        }
        else
            m_uiCleave_Timer -= uiDiff;

        if (m_uiQuake_Timer < uiDiff)
        {
            // to avoid blastnova interruption
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                int32 i = SPELL_QUAKE_KNOCKBACK;
                m_creature->CastCustomSpell(m_creature, SPELL_QUAKE_TRIGGER, &i, 0, 0, false);
                m_uiQuake_Timer = 50000;
            }
        }
        else
            m_uiQuake_Timer -= uiDiff;

        if (m_uiBlastNova_Timer < uiDiff)
        {
            // to avoid earthquake interruption
            if (!m_creature->hasUnitState(UNIT_STAT_STUNNED))
            {
                DoScriptText(EMOTE_BLASTNOVA, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_BLASTNOVA);
                m_uiBlastNova_Timer = 60000;
            }
        }
        else
            m_uiBlastNova_Timer -= uiDiff;

        if (m_uiBlaze_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                float x, y, z;
                pTarget->GetPosition(x, y, z);

                if (Creature* pSummon = m_creature->SummonCreature(NPC_BURNING_ABYSS, x, y, z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    if (mob_abyssalAI* pAbyssAI = dynamic_cast<mob_abyssalAI*>(pSummon->AI()))
                        pAbyssAI->SetTrigger(2);

                    m_creature->CastSpell(pSummon, SPELL_BLAZE_TARGET, true);
                    pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            m_uiBlaze_Timer = urand(20000, 40000);
        }
        else
            m_uiBlaze_Timer -= uiDiff;

        if (!m_bIsPhase3 && m_creature->GetHealthPercent() < 30.0f
            && !m_creature->IsNonMeleeSpellCasted(false)    // blast nova
            && !m_creature->hasUnitState(UNIT_STAT_STUNNED))// shadow cage and earthquake
        {
            m_bIsPhase3 = true;
            DoScriptText(SAY_CHAMBER_DESTROY, m_creature);
        }

        if (m_bIsPhase3)
        {
            if (m_uiPhase3_Timer < uiDiff)
            {
                switch(m_uiPhase3_Count)
                {
                    case 0:
                        m_creature->CastSpell(m_creature, SPELL_CAMERA_SHAKE, true);
                        ++m_uiPhase3_Count;
                        m_uiPhase3_Timer = 2000;
                        break;
                    case 1:
                        if (m_pInstance)
                            m_pInstance->SetData(TYPE_HALL_COLLAPSE, IN_PROGRESS);
                        ++m_uiPhase3_Count;
                        m_uiPhase3_Timer = 8000;
                        break;
                    case 2:
                        m_creature->CastSpell(m_creature, SPELL_DEBRIS_KNOCKDOWN, true);
                        ++m_uiPhase3_Count;
                        m_uiPhase3_Timer = 15000;
                        break;
                    case 3:
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            float x, y, z;
                            pTarget->GetPosition(x, y, z);

                            if (Creature* pSummon = m_creature->SummonCreature(NPC_BURNING_ABYSS, x, y, z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                            {
                                if (mob_abyssalAI* pAbyssAI = dynamic_cast<mob_abyssalAI*>(pSummon->AI()))
                                    pAbyssAI->SetTrigger(1);
                            }

                            m_uiPhase3_Timer = 15000;
                        }
                        break;
                }
            }
            else
                m_uiPhase3_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_hellfire_channelerAI : public ScriptedAI
{
    mob_hellfire_channelerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowBoltVolley_Timer;
    uint32 m_uiDarkMending_Timer;
    uint32 m_uiFear_Timer;
    uint32 m_uiInfernal_Timer;

    bool m_bIsInfernalSpawned;

    void Reset()
    {
        m_uiShadowBoltVolley_Timer = urand(8000, 10000);
        m_uiDarkMending_Timer = 10000;
        m_uiFear_Timer = urand(15000, 20000);
        m_uiInfernal_Timer = urand(10000, 50000);

        m_bIsInfernalSpawned = false;
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance)
            return;

        m_creature->InterruptNonMeleeSpells(false);

        if (Creature* pMagtheridon = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_MAGTHERIDON)))
        {
            if (!pMagtheridon->isAlive())
                return;

            if (m_pInstance->GetData(TYPE_CHANNELER_EVENT) == NOT_STARTED)
                DoScriptText(EMOTE_BEGIN, pMagtheridon);
        }

        m_pInstance->SetData(TYPE_CHANNELER_EVENT, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (m_creature->getVictim())
            pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CHANNELER_EVENT, DONE);

        pKiller->CastSpell(pKiller, SPELL_SOUL_TRANSFER, false);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CHANNELER_EVENT, NOT_STARTED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (!m_creature->IsNonMeleeSpellCasted(false) && !m_creature->IsInEvadeMode())
                DoCastSpellIfCan(m_creature, SPELL_SHADOW_GRASP_DUMMY);

            return;
        }

        //Shadow bolt volley
        if (m_uiShadowBoltVolley_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SHADOW_BOLT_VOLLEY);
            m_uiShadowBoltVolley_Timer = urand(10000, 20000);
        }
        else
            m_uiShadowBoltVolley_Timer -= uiDiff;

        //Dark Mending
        if (m_uiDarkMending_Timer < uiDiff)
        {
            if (m_creature->GetHealthPercent() < 50.0f)
            {
                //Cast on ourselves if we are lower then lowest hp friendly unit
                /*if (pLowestHPTarget && LowestHP < m_creature->GetHealth())
                    DoCastSpellIfCan(pLowestHPTarget, SPELL_DARK_MENDING);
                else*/
                DoCastSpellIfCan(m_creature, SPELL_DARK_MENDING);
            }

            m_uiDarkMending_Timer = urand(10000, 20000);
        }
        else
            m_uiDarkMending_Timer -= uiDiff;

        //Fear
        if (m_uiFear_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_FEAR);

            m_uiFear_Timer = urand(25000, 40000);
        }
        else
            m_uiFear_Timer -= uiDiff;

        //Infernal spawning
        if (!m_bIsInfernalSpawned && m_uiInfernal_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pTarget, SPELL_BURNING_ABYSSAL, true);

            m_bIsInfernalSpawned = true;
        }
        else
            m_uiInfernal_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Manticron Cube
bool GOUse_go_manticron_cube(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_MAGTHERIDON_EVENT) != IN_PROGRESS)
            return true;

        if (Creature* pMagtheridon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_MAGTHERIDON)))
        {
            if (!pMagtheridon->isAlive())
                return true;

            // if exhausted or already channeling return
            if (pPlayer->HasAura(SPELL_MIND_EXHAUSTION, EFFECT_INDEX_0) || pPlayer->HasAura(SPELL_SHADOW_GRASP, EFFECT_INDEX_1))
                return true;

            pPlayer->InterruptNonMeleeSpells(false);
            pPlayer->CastSpell(pPlayer, SPELL_SHADOW_GRASP, true);
            pPlayer->CastSpell(pPlayer, SPELL_SHADOW_GRASP_VISUAL, false);

            if (boss_magtheridonAI* pMagAI = dynamic_cast<boss_magtheridonAI*>(pMagtheridon->AI()))
                pMagAI->SetClicker(pGo->GetGUID(), pPlayer->GetGUID());
        }
    }

    return false;
}

CreatureAI* GetAI_boss_magtheridon(Creature* pCreature)
{
    return new boss_magtheridonAI(pCreature);
}

CreatureAI* GetAI_mob_hellfire_channeler(Creature* pCreature)
{
    return new mob_hellfire_channelerAI(pCreature);
}

CreatureAI* GetAI_mob_abyssalAI(Creature* pCreature)
{
    return new mob_abyssalAI(pCreature);
}

void AddSC_boss_magtheridon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_magtheridon";
    newscript->GetAI = &GetAI_boss_magtheridon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_hellfire_channeler";
    newscript->GetAI = &GetAI_mob_hellfire_channeler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_manticron_cube";
    newscript->pGOUse = &GOUse_go_manticron_cube;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_abyssal";
    newscript->GetAI = &GetAI_mob_abyssalAI;
    newscript->RegisterSelf();
}

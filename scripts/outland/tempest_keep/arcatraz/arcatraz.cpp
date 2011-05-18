/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Arcatraz
SD%Complete: 60
SDComment: Warden Mellichar, event controller for Skyriss event. Millhouse Manastorm. TODO: make better combatAI for Millhouse.
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

/* ContentData
npc_millhouse_manastorm
npc_warden_mellichar
mob_zerekethvoidzone
EndContentData */

#include "precompiled.h"
#include "arcatraz.h"

/*#####
# npc_millhouse_manastorm
#####*/

enum
{
    SAY_INTRO_1                     = -1552010,
    SAY_INTRO_2                     = -1552011,
    SAY_WATER                       = -1552012,
    SAY_BUFFS                       = -1552013,
    SAY_DRINK                       = -1552014,
    SAY_READY                       = -1552015,
    SAY_KILL_1                      = -1552016,
    SAY_KILL_2                      = -1552017,
    SAY_PYRO                        = -1552018,
    SAY_ICEBLOCK                    = -1552019,
    SAY_LOWHP                       = -1552020,
    SAY_DEATH                       = -1552021,
    SAY_COMPLETE                    = -1552022,

    SPELL_CONJURE_WATER             = 36879,
    SPELL_ARCANE_INTELLECT          = 36880,
    SPELL_ICE_ARMOR                 = 36881,

    SPELL_ARCANE_MISSILES           = 33833,
    SPELL_CONE_OF_COLD              = 12611,
    SPELL_FIRE_BLAST                = 13341,
    SPELL_FIREBALL                  = 14034,
    SPELL_FROSTBOLT                 = 15497,
    SPELL_PYROBLAST                 = 33975,
};

struct MANGOS_DLL_DECL npc_millhouse_manastormAI : public ScriptedAI
{
    npc_millhouse_manastormAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiEventProgressTimer;
    uint32 m_uiPhase;
    bool m_bInitFinished;
    bool m_bHasLowHp;

    uint32 m_uiPyroblastTimer;
    uint32 m_uiFireballTimer;

    void Reset()
    {
        m_uiEventProgressTimer = 2000;
        m_bHasLowHp = false;
        m_bInitFinished = false;
        m_uiPhase = 1;

        m_uiPyroblastTimer = 1000;
        m_uiFireballTimer = 2500;

        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_WARDEN_2) == DONE)
                m_bInitFinished = true;

            if (m_pInstance->GetData(TYPE_HARBINGERSKYRISS) == DONE)
                DoScriptText(SAY_COMPLETE, m_creature);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 25.0f);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* pVictim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        /*for questId 10886 (heroic mode only)
        if (m_pInstance && m_pInstance->GetData(TYPE_HARBINGERSKYRISS) != DONE)
            ->FailQuest();*/
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bInitFinished)
        {
            if (m_uiEventProgressTimer < uiDiff)
            {
                if (m_uiPhase < 8)
                {
                    switch(m_uiPhase)
                    {
                        case 1:
                            DoScriptText(SAY_INTRO_1, m_creature);
                            m_uiEventProgressTimer = 18000;
                            break;
                        case 2:
                            DoScriptText(SAY_INTRO_2, m_creature);
                            m_uiEventProgressTimer = 18000;
                            break;
                        case 3:
                            DoScriptText(SAY_WATER, m_creature);
                            DoCastSpellIfCan(m_creature, SPELL_CONJURE_WATER);
                            m_uiEventProgressTimer = 7000;
                            break;
                        case 4:
                            DoScriptText(SAY_BUFFS, m_creature);
                            DoCastSpellIfCan(m_creature, SPELL_ICE_ARMOR);
                            m_uiEventProgressTimer = 7000;
                            break;
                        case 5:
                            DoScriptText(SAY_DRINK, m_creature);
                            DoCastSpellIfCan(m_creature, SPELL_ARCANE_INTELLECT);
                            m_uiEventProgressTimer = 7000;
                            break;
                        case 6:
                            DoScriptText(SAY_READY, m_creature);
                            m_uiEventProgressTimer = 6000;
                            break;
                        case 7:
                            if (m_pInstance)
                                m_pInstance->SetData(TYPE_WARDEN_2, DONE);
                            m_bInitFinished = true;
                            break;
                    }
                    ++m_uiPhase;
                }
            }
            else
                m_uiEventProgressTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bHasLowHp && m_creature->GetHealthPercent() < 20.0f)
        {
            DoScriptText(SAY_LOWHP, m_creature);
            m_bHasLowHp = true;
        }

        if (m_uiPyroblastTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PYROBLAST) == CAST_OK)
            {
                 m_uiPyroblastTimer = 40000;
                 DoScriptText(SAY_PYRO, m_creature);
            }
        }
        else
            m_uiPyroblastTimer -=uiDiff;

        if (m_uiFireballTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBALL) == CAST_OK)
                m_uiFireballTimer = 4000;
        }
        else
            m_uiFireballTimer -=uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_millhouse_manastorm(Creature* pCreature)
{
    return new npc_millhouse_manastormAI(pCreature);
}

/*#####
# npc_warden_mellichar
#####*/

enum
{
    YELL_INTRO1             = -1552023,
    YELL_INTRO2             = -1552024,
    YELL_RELEASE1           = -1552025,
    YELL_RELEASE2A          = -1552026,
    YELL_RELEASE2B          = -1552027,
    YELL_RELEASE3           = -1552028,
    YELL_RELEASE4           = -1552029,
    YELL_WELCOME            = -1552030,

    // phase 2(acid mobs)
    ENTRY_TRICKSTER         = 20905,
    ENTRY_PH_HUNTER         = 20906,
    // phase 3
    ENTRY_MILLHOUSE         = 20977,
    // phase 4(acid mobs)
    ENTRY_AKKIRIS           = 20908,
    ENTRY_SULFURON          = 20909,
    // phase 5(acid mobs)
    ENTRY_TW_DRAK           = 20910,
    ENTRY_BL_DRAK           = 20911,
    // phase 6
    ENTRY_SKYRISS           = 20912,

    SPELL_TARGET_ALPHA      = 36856,
    SPELL_TARGET_BETA       = 36854,
    SPELL_TARGET_DELTA      = 36857,
    SPELL_TARGET_GAMMA      = 36858,
    SPELL_TARGET_OMEGA      = 36852,
    SPELL_BUBBLE_VISUAL     = 36849,
};

static const float aSummonPosition[5][4] =
{
    {478.326f, -148.505f, 42.56f, 3.19f},                   // Trickster or Phase Hunter
    {413.292f, -148.378f, 42.56f, 6.27f},                   // Millhouse
    {420.179f, -174.396f, 42.58f, 0.02f},                   // Akkiris or Sulfuron
    {471.795f, -174.58f, 42.58f, 3.06f},                    // Twilight or Blackwing Drakonaar
    {445.763f, -191.639f, 44.64f, 1.60f}                    // Skyriss
};

struct MANGOS_DLL_DECL npc_warden_mellicharAI : public ScriptedAI
{
    npc_warden_mellicharAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bIsEventRunning;
    bool m_bCanSpawnNextWave;

    uint32 m_uiEventProgressTimer;
    uint32 m_uiPhase;

    void Reset()
    {
        m_bIsEventRunning = false;
        m_bCanSpawnNextWave = false;

        m_uiEventProgressTimer = 22000;
        m_uiPhase = 1;

        m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
        DoCastSpellIfCan(m_creature,SPELL_TARGET_OMEGA);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HARBINGERSKYRISS,NOT_STARTED);
    }

    void AttackStart(Unit* pWho) {}

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_bIsEventRunning)
            return;

        if (pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        if (!m_creature->getVictim() && pWho->isTargetableForAttack() && (m_creature->IsHostileTo(pWho)) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho)/10;
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
                Aggro(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(YELL_INTRO1, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_BUBBLE_VISUAL);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_HARBINGERSKYRISS, IN_PROGRESS);

            if (GameObject* pSphere = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_SEAL_SPHERE)))
                pSphere->SetGoState(GO_STATE_READY);

            m_bIsEventRunning = true;
        }
    }

    bool CanProgress()
    {
        if (m_pInstance)
        {
            if (m_uiPhase == 7 && m_pInstance->GetData(TYPE_WARDEN_4) == DONE)
                return true;
            if (m_uiPhase == 6 && m_pInstance->GetData(TYPE_WARDEN_3) == DONE)
                return true;
            if (m_uiPhase == 5 && m_pInstance->GetData(TYPE_WARDEN_2) == DONE)
                return true;
            if (m_uiPhase == 4)
                return true;
            if (m_uiPhase == 3 && m_pInstance->GetData(TYPE_WARDEN_1) == DONE)
                return true;
            if (m_uiPhase == 2 && m_pInstance->GetData(TYPE_HARBINGERSKYRISS) == IN_PROGRESS)
                return true;
            if (m_uiPhase == 1 && m_pInstance->GetData(TYPE_HARBINGERSKYRISS) == IN_PROGRESS)
                return true;

            return false;
        }
        return false;
    }

    void DoPrepareForPhase()
    {
        if (m_pInstance)
        {
            m_creature->InterruptNonMeleeSpells(true);
            m_creature->RemoveSpellsCausingAura(SPELL_AURA_DUMMY);

            switch(m_uiPhase)
            {
                case 2:
                    DoCastSpellIfCan(m_creature, SPELL_TARGET_ALPHA);
                    m_pInstance->SetData(TYPE_WARDEN_1, IN_PROGRESS);
                    break;
                case 3:
                    DoCastSpellIfCan(m_creature, SPELL_TARGET_BETA);
                    m_pInstance->SetData(TYPE_WARDEN_2, IN_PROGRESS);
                    break;
                case 5:
                    DoCastSpellIfCan(m_creature, SPELL_TARGET_DELTA);
                    m_pInstance->SetData(TYPE_WARDEN_3, IN_PROGRESS);
                    break;
                case 6:
                    DoCastSpellIfCan(m_creature, SPELL_TARGET_GAMMA);
                    m_pInstance->SetData(TYPE_WARDEN_4, IN_PROGRESS);
                    break;
                case 7:
                    m_pInstance->SetData(TYPE_WARDEN_5, IN_PROGRESS);
                    break;
            }
            m_bCanSpawnNextWave = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsEventRunning)
            return;

        if (m_uiEventProgressTimer < uiDiff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_HARBINGERSKYRISS) == FAIL)
                    Reset();
            }

            if (m_bCanSpawnNextWave)
            {
                //continue beam omega pod, unless we are about to summon skyriss
                if (m_uiPhase != 7)
                    DoCastSpellIfCan(m_creature, SPELL_TARGET_OMEGA);

                switch (m_uiPhase)
                {
                    case 2:
                        m_creature->SummonCreature(urand(0, 1) ? ENTRY_TRICKSTER : ENTRY_PH_HUNTER, aSummonPosition[0][0], aSummonPosition[0][1], aSummonPosition[0][2], aSummonPosition[0][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        break;
                    case 3:
                        m_creature->SummonCreature(ENTRY_MILLHOUSE, aSummonPosition[1][0], aSummonPosition[1][1], aSummonPosition[1][2], aSummonPosition[1][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        break;
                    case 4:
                        DoScriptText(YELL_RELEASE2B, m_creature);
                        break;
                    case 5:
                        m_creature->SummonCreature(urand(0, 1) ? ENTRY_AKKIRIS : ENTRY_SULFURON, aSummonPosition[2][0], aSummonPosition[2][1], aSummonPosition[2][2], aSummonPosition[2][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        break;
                    case 6:
                        m_creature->SummonCreature(urand(0, 1) ? ENTRY_TW_DRAK : ENTRY_BL_DRAK, aSummonPosition[3][0], aSummonPosition[3][1], aSummonPosition[3][2], aSummonPosition[3][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        break;
                    case 7:
                        m_creature->SummonCreature(ENTRY_SKYRISS, aSummonPosition[4][0], aSummonPosition[4][1], aSummonPosition[4][2], aSummonPosition[4][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoScriptText(YELL_WELCOME, m_creature);
                        break;
                }
                m_bCanSpawnNextWave = false;
                ++m_uiPhase;
            }
            if (CanProgress())
            {
                switch(m_uiPhase)
                {
                    case 1:
                        DoScriptText(YELL_INTRO2, m_creature);
                        m_uiEventProgressTimer = 10000;
                        ++m_uiPhase;
                        break;
                    case 2:
                        DoScriptText(YELL_RELEASE1, m_creature);
                        DoPrepareForPhase();
                        m_uiEventProgressTimer = 7000;
                        break;
                    case 3:
                        DoScriptText(YELL_RELEASE2A, m_creature);
                        DoPrepareForPhase();
                        m_uiEventProgressTimer = 10000;
                        break;
                    case 4:
                        DoPrepareForPhase();
                        m_uiEventProgressTimer = 15000;
                        break;
                    case 5:
                        DoScriptText(YELL_RELEASE3, m_creature);
                        DoPrepareForPhase();
                        m_uiEventProgressTimer = 15000;
                        break;
                    case 6:
                        DoScriptText(YELL_RELEASE4, m_creature);
                        DoPrepareForPhase();
                        m_uiEventProgressTimer = 15000;
                        break;
                    case 7:
                        DoPrepareForPhase();
                        m_uiEventProgressTimer = 15000;
                        break;
                }
            }
        }
        else
            m_uiEventProgressTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_warden_mellichar(Creature* pCreature)
{
    return new npc_warden_mellicharAI(pCreature);
}

/*#####
# mob_zerekethvoidzone (this script probably not needed in future -> `creature_template_addon`.`auras`='36120 0')
#####*/

enum
{
    SPELL_VOID_ZONE_DAMAGE  = 36120,
};

struct MANGOS_DLL_DECL mob_zerekethvoidzoneAI : public ScriptedAI
{
    mob_zerekethvoidzoneAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
        m_creature->setFaction(16);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        DoCastSpellIfCan(m_creature, SPELL_VOID_ZONE_DAMAGE);
    }
};
CreatureAI* GetAI_mob_zerekethvoidzoneAI(Creature* pCreature)
{
    return new mob_zerekethvoidzoneAI(pCreature);
}

void AddSC_arcatraz()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_millhouse_manastorm";
    newscript->GetAI = &GetAI_npc_millhouse_manastorm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_warden_mellichar";
    newscript->GetAI = &GetAI_npc_warden_mellichar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zerekethvoidzone";
    newscript->GetAI = &GetAI_mob_zerekethvoidzoneAI;
    newscript->RegisterSelf();
}

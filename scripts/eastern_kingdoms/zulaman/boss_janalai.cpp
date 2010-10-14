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
SDName: Boss_Janalai
SD%Complete: 75
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_AGGRO                       = -1568000,
    SAY_FIRE_BOMBS                  = -1568001,
    SAY_SUMMON_HATCHER              = -1568002,
    SAY_ALL_EGGS                    = -1568003,
    SAY_BERSERK                     = -1568004,
    SAY_SLAY_1                      = -1568005,
    SAY_SLAY_2                      = -1568006,
    SAY_DEATH                       = -1568007,
    SAY_EVENT_STRANGERS             = -1568008,
    SAY_EVENT_FRIENDS               = -1568009,

    //Jan'alai
    SPELL_FLAME_BREATH              = 43140,
    SPELL_FIRE_WALL                 = 43113,
    SPELL_ENRAGE                    = 44779,
    SPELL_TELETOCENTER              = 43098,
    SPELL_SUMMONALL                 = 43097,
    SPELL_BERSERK                   = 47008,
    SPELL_SUMMON_HATCHER_1          = 43962,
    SPELL_SUMMON_HATCHER_2          = 45340,

    //Fire Bob Spells
    SPELL_FIRE_BOMB_CHANNEL         = 42621,
    SPELL_FIRE_BOMB_THROW           = 42628,
    SPELL_FIRE_BOMB_DUMMY           = 42629,
    SPELL_FIRE_BOMB_DAMAGE          = 42630,

    //NPC's
    NPC_FIRE_BOMB                   = 23920,
    NPC_AMANI_HATCHER_1             = 23818,
    NPC_AMANI_HATCHER_2             = 24504,
    NPC_HATCHLING                   = 23598,

    //Hatcher Spells
    SPELL_HATCH_EGG                 = 43734,                //spell 42471 also exist
    SPELL_HATCH_ALL_EGGS            = 43144,

    //Eggs spells
    SPELL_SUMMON_DRAGONHAWK         = 42493,

    //Hatchling Spells
    SPELL_FLAMEBUFFED               = 43299
};

//spells should summon Fire Bomb, used in Throw5Bombs()
static uint32 m_auiSpellFireBombSummon[]=
{
    42622, 42623, 42624, 42625, 42626
};

const int area_dx = 44;
const int area_dy = 51;

float JanalainPos[1][3] =
{
    {-33.93f, 1149.27f, 19.0f}
};

float FireWallCoords[4][4] =
{
    {-10.13f, 1149.27f, 19.0f, M_PI_F},
    {-33.93f, 1123.90f, 19.0f, 0.5f*M_PI_F},
    {-54.80f, 1150.08f, 19.0f, 0.0f},
    {-33.93f, 1175.68f, 19.0f, 1.5f*M_PI_F}
};

struct WaypointDef
{
    float m_fX, m_fY, m_fZ;
};

WaypointDef m_aHatcherRight[]=
{
    {-86.203f, 1136.834f, 5.594f},                          //this is summon point, not regular waypoint
    {-74.783f, 1145.827f, 5.420f},
    {-56.957f, 1146.713f, 18.725f},
    {-45.428f, 1141.697f, 18.709f},
    {-34.002f, 1124.427f, 18.711f},
    {-34.085f, 1106.158f, 18.711f}
};

WaypointDef m_aHatcherLeft[]=
{
    {-85.420f, 1167.321f, 5.594f},                          //this is summon point, not regular waypoint
    {-73.569f, 1154.960f, 5.510f},
    {-56.985f, 1153.373f, 18.608f},
    {-45.515f, 1158.356f, 18.709f},
    {-33.314f, 1174.816f, 18.709f},
    {-33.097f, 1195.359f, 18.709f}
};

float hatcherway_l[5][3] =
{
    {-87.46f, 1170.09f, 6.0f},
    {-74.41f, 1154.75f, 6.0f},
    {-52.74f, 1153.32f, 19.0f},
    {-33.37f, 1172.46f, 19.0f},
    {-33.09f, 1203.87f, 19.0f}
};

float hatcherway_r[5][3] =
{
    {-86.57f, 1132.85f, 6.0f},
    {-73.94f, 1146.00f, 6.0f},
    {-52.29f, 1146.51f, 19.0f},
    {-33.57f, 1125.72f, 19.0f},
    {-34.29f, 1095.22f, 19.0f}
};

struct MANGOS_DLL_DECL boss_janalaiAI : public ScriptedAI
{
    boss_janalaiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiHatcher1GUID = 0;
        m_uiHatcher2GUID = 0;
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 fire_breath_timer;

    std::list<uint64> m_lBombsGUIDList;
    std::list<Creature*> m_lEggsRemainingList;

    uint32 m_uiBombTimer;
    uint32 m_uiBombSequenzeTimer;
    uint32 m_uiBombPhase;
    uint32 m_uiBombCounter;

    uint32 enrage_timer;
    uint32 hatchertime;
    uint32 eggs;
    uint32 wipetimer;

    bool m_bIsBombing;
    bool m_bCanBlowUpBombs;
    bool m_bIsEggRemaining;
    bool enraged;
    bool enragetime;

    uint64 m_uiHatcher1GUID;
    uint64 m_uiHatcher2GUID;

    void Reset()
    {
        m_lBombsGUIDList.clear();
        m_lEggsRemainingList.clear();

        if (Creature* pHatcher = m_creature->GetMap()->GetCreature(m_uiHatcher1GUID))
        {
            pHatcher->AI()->EnterEvadeMode();
            pHatcher->SetDeathState(JUST_DIED);
            m_uiHatcher1GUID = 0;
        }

        if (Creature* pHatcher = m_creature->GetMap()->GetCreature(m_uiHatcher2GUID))
        {
            pHatcher->AI()->EnterEvadeMode();
            pHatcher->SetDeathState(JUST_DIED);
            m_uiHatcher2GUID = 0;
        }

        fire_breath_timer = 8000;

        m_uiBombTimer = 30000;
        m_bIsBombing = false;
        m_uiBombSequenzeTimer = 1500;
        m_uiBombPhase = 0;
        m_uiBombCounter = 0;
        m_bCanBlowUpBombs = false;
        m_bIsEggRemaining = true;

        enrage_timer = MINUTE*5*IN_MILLISECONDS;
        hatchertime = 10000;
        wipetimer = MINUTE*10*IN_MILLISECONDS;
        enraged = false;
        enragetime = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JANALAI, NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_JANALAI, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_JANALAI, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_AMANI_HATCHER_1:
                m_uiHatcher1GUID = pSummoned->GetGUID();
                break;
            case NPC_AMANI_HATCHER_2:
                m_uiHatcher2GUID = pSummoned->GetGUID();
                break;
            case NPC_FIRE_BOMB:
                if (m_bIsBombing)
                {
                    //store bombs in list to be used in BlowUpBombs()
                    m_lBombsGUIDList.push_back(pSummoned->GetGUID());

                    if (pSummoned->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                        pSummoned->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    //visual spell, spell hit pSummoned after a short time
                    m_creature->CastSpell(pSummoned,SPELL_FIRE_BOMB_THROW,true);
                }
                else
                {
                    pSummoned->CastSpell(pSummoned, SPELL_FIRE_WALL, true);
                }
                break;
        }
    }

    void SpellHitTarget(Unit* pUnit, const SpellEntry* pSpell)
    {
        //when spell actually hit the fire bombs, make then cast spell(making them "visible")
        if (pUnit->GetEntry() == NPC_FIRE_BOMB && pSpell->Id == SPELL_FIRE_BOMB_THROW)
            pUnit->CastSpell(pUnit,SPELL_FIRE_BOMB_DUMMY,false);
    }

    void CreateFireWall()                                   // Create Firewall
    {
        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[0][0],FireWallCoords[0][1],FireWallCoords[0][2],FireWallCoords[0][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[0][0],FireWallCoords[0][1]+5,FireWallCoords[0][2],FireWallCoords[0][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[0][0],FireWallCoords[0][1]-5,FireWallCoords[0][2],FireWallCoords[0][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[1][0]-2,FireWallCoords[1][1]-2,FireWallCoords[1][2],FireWallCoords[1][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[1][0]+2,FireWallCoords[1][1]+2,FireWallCoords[1][2],FireWallCoords[1][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[2][0],FireWallCoords[2][1],FireWallCoords[2][2],FireWallCoords[2][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[2][0],FireWallCoords[2][1]-5,FireWallCoords[2][2],FireWallCoords[2][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[2][0],FireWallCoords[2][1]+5,FireWallCoords[2][2],FireWallCoords[2][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[3][0]-2,FireWallCoords[3][1],FireWallCoords[3][2],FireWallCoords[3][3],TEMPSUMMON_TIMED_DESPAWN,11500);

        m_creature->SummonCreature(NPC_FIRE_BOMB,FireWallCoords[3][0]+2,FireWallCoords[3][1],FireWallCoords[3][2],FireWallCoords[3][3],TEMPSUMMON_TIMED_DESPAWN,11500);
    }

    void Throw5Bombs()
    {
        //all available spells (each spell has different radius for summon location)
        uint8 uiMaxBombs = sizeof(m_auiSpellFireBombSummon)/sizeof(uint32);

        //float fX, fY, fZ;
        //float fRadius = 5.0f;

        for(uint8 i = 0; i < uiMaxBombs; ++i)
        {
            m_creature->CastSpell(m_creature, m_auiSpellFireBombSummon[i], true);

            //workaround part
            //m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), fRadius+(fRadius*i), fX, fY, fZ);
            //m_creature->SummonCreature(NPC_FIRE_BOMB, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, MINUTE*IN_MILLISECONDS);
        }

        ++m_uiBombCounter;
    }

    //Teleport every player into the middle if more than 20 yards away (possibly what spell 43096 should do)
    void TeleportPlayersOutOfRange()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            Unit* pTemp = m_creature->GetMap()->GetUnit((*i)->getUnitGuid());

            if (pTemp && pTemp->GetTypeId() == TYPEID_PLAYER && !m_creature->IsWithinDist(pTemp, 20.0f))
                m_creature->CastSpell(pTemp, SPELL_SUMMONALL, true);
        }
    }

    void BlowUpBombs()
    {
        if (m_lBombsGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lBombsGUIDList.begin(); itr != m_lBombsGUIDList.end(); ++itr)
        {
            if (Creature* pBomb = m_creature->GetMap()->GetCreature(*itr))
            {
                //do damage and then remove aura (making them "disappear")
                pBomb->CastSpell(pBomb, SPELL_FIRE_BOMB_DAMAGE, false, NULL, NULL, m_creature->GetGUID());
                pBomb->RemoveAurasDueToSpell(SPELL_FIRE_BOMB_DUMMY);
            }
        }

        m_lBombsGUIDList.clear();
    }

    void DoHatchRemainingEggs()
    {
        GetCreatureListWithEntryInGrid(m_lEggsRemainingList, m_creature, NPC_EGG, 125.0f);

        if (!m_lEggsRemainingList.empty())
        {
            for(std::list<Creature*>::iterator itr = m_lEggsRemainingList.begin(); itr != m_lEggsRemainingList.end(); ++itr)
            {
                if ((*itr)->isAlive())
                    (*itr)->CastSpell((*itr), SPELL_SUMMON_DRAGONHAWK, true);
            }

            m_bIsEggRemaining = false;

            if (!m_pInstance)
                return;

            if (uint32 uiEggsRemaining_Right = m_pInstance->GetData(DATA_J_EGGS_RIGHT))
            {
                for(uint32 i = 0; i < uiEggsRemaining_Right; ++i)
                    m_pInstance->SetData(DATA_J_EGGS_RIGHT, SPECIAL);
            }

            if (uint32 uiEggsRemaining_Left = m_pInstance->GetData(DATA_J_EGGS_LEFT))
            {
                for(uint32 i = 0; i < uiEggsRemaining_Left; ++i)
                    m_pInstance->SetData(DATA_J_EGGS_LEFT, SPECIAL);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //blow up bombs happen after bombing is over, so handle this here
        if (m_bCanBlowUpBombs)
        {
            if (m_uiBombSequenzeTimer < diff)
            {
                BlowUpBombs();
                m_bCanBlowUpBombs = false;
            }else m_uiBombSequenzeTimer -= diff;
        }

        if (!m_bIsBombing)                                  // every Spell if NOT Bombing
        {
            if (m_uiBombTimer < diff)
            {
                if (m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(false);

                DoScriptText(SAY_FIRE_BOMBS, m_creature);

                //first clear movement
                if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                    m_creature->GetMotionMaster()->MovementExpired();

                //then teleport self
                DoCastSpellIfCan(m_creature, SPELL_TELETOCENTER, CAST_TRIGGERED);

                //then players and create the firewall
                TeleportPlayersOutOfRange();
                CreateFireWall();

                //prepare variables for bombing sequenze
                m_lBombsGUIDList.clear();

                m_uiBombPhase = 0;
                m_uiBombSequenzeTimer = 500;
                m_uiBombCounter = 0;

                m_uiBombTimer = urand(20000, 40000);
                m_bIsBombing = true;

                //we don't want anything else to happen this Update()
                return;
            }else m_uiBombTimer -=diff;

            //FIRE BREATH  several videos says every 8Secounds
            if (fire_breath_timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    DoCastSpellIfCan(target,SPELL_FLAME_BREATH);
                fire_breath_timer = 8000;
            }else fire_breath_timer -=diff;

            //enrage if under 25% hp before 5 min.
            if (m_creature->GetHealthPercent() < 25.0f && !enraged)
            {
                enragetime = true;
                enrage_timer = 600000;
            }

            //Enrage but only if not bombing
            if (enragetime && !enraged)
            {
                DoScriptText(SAY_BERSERK, m_creature);

                m_creature->InterruptNonMeleeSpells(false);
                DoCastSpellIfCan(m_creature,SPELL_ENRAGE);
                enraged = true;
            }

            //Hatch All
            if (m_bIsEggRemaining && m_creature->GetHealthPercent() < 35.0f)
            {
                DoScriptText(SAY_ALL_EGGS, m_creature);

                if (m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(false);

                DoCastSpellIfCan(m_creature, SPELL_HATCH_ALL_EGGS);

                DoHatchRemainingEggs();
            }

            DoMeleeAttackIfReady();
        }
        else                                                // every Spell if Bombing
        {
            if (m_uiBombSequenzeTimer < diff)
            {
                switch(m_uiBombPhase)
                {
                    case 0:
                        m_creature->CastSpell(m_creature,SPELL_FIRE_BOMB_CHANNEL,true);
                        m_uiBombSequenzeTimer = 500;
                        ++m_uiBombPhase;
                        break;
                    case 1:
                        if (m_uiBombCounter < 8)
                        {
                            Throw5Bombs();
                            m_uiBombSequenzeTimer = 500;
                        }
                        else
                        {
                            m_uiBombSequenzeTimer = 1000;
                            ++m_uiBombPhase;
                        }
                        break;
                    case 2:
                        m_bCanBlowUpBombs = true;
                        m_uiBombSequenzeTimer = 2000;
                        m_creature->RemoveAurasDueToSpell(SPELL_FIRE_BOMB_CHANNEL);
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_bIsBombing = false;
                        break;
                }

            }else m_uiBombSequenzeTimer -= diff;
        }

        //Enrage after 5 minutes
        if (enrage_timer < diff)
        {
            enragetime = true;
            enrage_timer = 600000;
        }else enrage_timer -=diff;

        //Call Hatcher
        if (m_bIsEggRemaining)
        {
            if (hatchertime < diff)
            {
                if (!m_pInstance || (m_pInstance->GetData(DATA_J_EGGS_LEFT) == 0 && m_pInstance->GetData(DATA_J_EGGS_RIGHT) == 0))
                    m_bIsEggRemaining = false;
                else
                {
                    DoScriptText(SAY_SUMMON_HATCHER, m_creature);

                    Creature* pHatcer1 = m_creature->GetMap()->GetCreature(m_uiHatcher1GUID);
                    Creature* pHatcer2 = m_creature->GetMap()->GetCreature(m_uiHatcher2GUID);

                    if (!pHatcer1 || (pHatcer1 && !pHatcer1->isAlive()))
                    {
                        if (Creature* pHatcher = m_creature->SummonCreature(NPC_AMANI_HATCHER_1, m_aHatcherRight[0].m_fX, m_aHatcherRight[0].m_fY, m_aHatcherRight[0].m_fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                            pHatcher->GetMotionMaster()->MovePoint(1, m_aHatcherRight[1].m_fX, m_aHatcherRight[1].m_fY, m_aHatcherRight[1].m_fZ);
                    }

                    if (!pHatcer2 || (pHatcer2 && !pHatcer2->isAlive()))
                    {
                        if (Creature* pHatcher = m_creature->SummonCreature(NPC_AMANI_HATCHER_2, m_aHatcherLeft[0].m_fX, m_aHatcherLeft[0].m_fY, m_aHatcherLeft[0].m_fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
                            pHatcher->GetMotionMaster()->MovePoint(1, m_aHatcherLeft[1].m_fX, m_aHatcherLeft[1].m_fY, m_aHatcherLeft[1].m_fZ);
                    }

                    hatchertime = 90000;
                }

            }else hatchertime -=diff;
        }

        //WIPE after 10 minutes
        if (wipetimer < diff)
        {
            if (DoCastSpellIfCan(m_creature,SPELL_ENRAGE) == CAST_OK)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                wipetimer = 30000;
            }
        }else wipetimer -=diff;

        //check for reset ... exploit preventing ... pulled from his podest
        EnterEvadeIfOutOfCombatArea(diff);
    }
};

CreatureAI* GetAI_boss_janalaiAI(Creature* pCreature)
{
    return new boss_janalaiAI(pCreature);
}

struct MANGOS_DLL_DECL mob_jandalai_firebombAI : public ScriptedAI
{
    mob_jandalai_firebombAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() {}

    void AttackStart(Unit* who) {}

    void MoveInLineOfSight(Unit* who) {}

    void UpdateAI(const uint32 diff) {}
};

CreatureAI* GetAI_mob_jandalai_firebombAI(Creature* pCreature)
{
    return new mob_jandalai_firebombAI(pCreature);
}

struct MANGOS_DLL_DECL mob_amanishi_hatcherAI : public ScriptedAI
{
    mob_amanishi_hatcherAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiWaypoint;
    uint32 m_uiHatchlingTimer;
    uint32 m_uiHatchlingCount;
    bool m_bCanMoveNext;
    bool m_bWaypointEnd;

    void Reset()
    {
        m_uiWaypoint = 0;
        m_uiHatchlingTimer = 1000;
        m_uiHatchlingCount = 1;
        m_bCanMoveNext = false;
        m_bWaypointEnd = false;

        if (m_creature->HasSplineFlag(SPLINEFLAG_WALKMODE))
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
    }

    void MoveInLineOfSight(Unit* pWho) {}

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || m_bWaypointEnd)
            return;

        uint32 uiCount = (m_creature->GetEntry() == NPC_AMANI_HATCHER_1) ?
            (sizeof(m_aHatcherRight)/sizeof(WaypointDef)) : (sizeof(m_aHatcherLeft)/sizeof(WaypointDef));

        m_uiWaypoint = uiPointId+1;

        if (uiCount == m_uiWaypoint)
            m_bWaypointEnd = true;

        m_bCanMoveNext = true;
    }

    void DoHatchEggs(uint32 uiCount)
    {
        uint32 uiSaveRightOrLeft = m_creature->GetEntry() == NPC_AMANI_HATCHER_1 ? DATA_J_EGGS_RIGHT : DATA_J_EGGS_LEFT;

        for(uint32 i = 0; i < uiCount; ++i)
        {
            if (Creature* pEgg = GetClosestCreatureWithEntry(m_creature, NPC_EGG, 40.0f))
                pEgg->CastSpell(pEgg, SPELL_SUMMON_DRAGONHAWK, true);

            m_pInstance->SetData(uiSaveRightOrLeft, SPECIAL);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanMoveNext)
        {
            m_bCanMoveNext = false;

            if (m_bWaypointEnd)
                m_creature->GetMotionMaster()->Clear();
            else
            {
                if (m_creature->GetEntry() == NPC_AMANI_HATCHER_1)
                    m_creature->GetMotionMaster()->MovePoint(m_uiWaypoint, m_aHatcherRight[m_uiWaypoint].m_fX, m_aHatcherRight[m_uiWaypoint].m_fY, m_aHatcherRight[m_uiWaypoint].m_fZ);
                else
                    m_creature->GetMotionMaster()->MovePoint(m_uiWaypoint, m_aHatcherLeft[m_uiWaypoint].m_fX, m_aHatcherLeft[m_uiWaypoint].m_fY, m_aHatcherLeft[m_uiWaypoint].m_fZ);
            }
        }

        if (m_bWaypointEnd)
        {
            if (m_uiHatchlingTimer < uiDiff)
            {
                m_uiHatchlingTimer = 10000;

                if (!m_pInstance)
                    return;

                uint32 uiEggsRemaining = m_creature->GetEntry() == NPC_AMANI_HATCHER_1 ? m_pInstance->GetData(DATA_J_EGGS_RIGHT) : m_pInstance->GetData(DATA_J_EGGS_LEFT);

                if (!uiEggsRemaining)
                {
                    //instead, should run to other side and start hatch if eggs remain
                    m_creature->ForcedDespawn();
                    return;
                }
                else if (m_uiHatchlingCount == uiEggsRemaining/2)
                    m_uiHatchlingCount = uiEggsRemaining;

                DoCastSpellIfCan(m_creature,SPELL_HATCH_EGG);

                DoHatchEggs(m_uiHatchlingCount);

                ++m_uiHatchlingCount;

            }else m_uiHatchlingTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_amanishi_hatcherAI(Creature* pCreature)
{
    return new mob_amanishi_hatcherAI(pCreature);
}

struct MANGOS_DLL_DECL mob_hatchlingAI : public ScriptedAI
{
    mob_hatchlingAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 buffer_timer;
    bool start;

    void Reset()
    {
        buffer_timer = 7000;
        start = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!start)
        {
            if (m_creature->GetPositionY() > 1150)
                m_creature->GetMotionMaster()->MovePoint(0, hatcherway_l[3][0]+rand()%4-2,hatcherway_l[3][1]+rand()%4-2,hatcherway_l[3][2]);
            else
                m_creature->GetMotionMaster()->MovePoint(0,hatcherway_r[3][0]+rand()%4-2,hatcherway_r[3][1]+rand()%4-2,hatcherway_r[3][2]);
            start = true;
        }

        if (m_pInstance && m_pInstance->GetData(TYPE_JANALAI) == NOT_STARTED)
        {
            m_creature->ForcedDespawn();
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (buffer_timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_FLAMEBUFFED);

            buffer_timer = 7000;
        }else buffer_timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_hatchlingAI(Creature* pCreature)
{
    return new mob_hatchlingAI(pCreature);
}

void AddSC_boss_janalai()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_janalai";
    newscript->GetAI = &GetAI_boss_janalaiAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_jandalai_firebomb";
    newscript->GetAI = &GetAI_mob_jandalai_firebombAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_amanishi_hatcher";
    newscript->GetAI = &GetAI_mob_amanishi_hatcherAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_hatchling";
    newscript->GetAI = &GetAI_mob_hatchlingAI;
    newscript->RegisterSelf();
}

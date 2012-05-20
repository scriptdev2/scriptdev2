/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Shadowmoon_Valley
SD%Complete: 100
SDComment: Quest support: 10781, 10804, 10854, 10458, 10480, 10481, 10588, 11020.
SDCategory: Shadowmoon Valley
EndScriptData */

/* ContentData
mob_mature_netherwing_drake
mob_enslaved_netherwing_drake
npc_dragonmaw_peon
npc_wilda
mob_torloth
npc_lord_illidan_stormrage
npc_totem_of_spirits
event_spell_soul_captured_credit
go_crystal_prison
npc_spawned_oronok_tornheart
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "pet_ai.h"

/*#####
# mob_mature_netherwing_drake
#####*/

enum
{
    SAY_JUST_EATEN              = -1000175,

    SPELL_PLACE_CARCASS         = 38439,
    SPELL_JUST_EATEN            = 38502,
    SPELL_NETHER_BREATH         = 38467,
    POINT_ID                    = 1,

    QUEST_KINDNESS              = 10804,
    NPC_EVENT_PINGER            = 22131
};

struct MANGOS_DLL_DECL mob_mature_netherwing_drakeAI : public ScriptedAI
{
    mob_mature_netherwing_drakeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_playerGuid;

    bool bCanEat;
    bool bIsEating;

    uint32 EatTimer;
    uint32 CastTimer;

    void Reset()
    {
        m_playerGuid.Clear();

        bCanEat = false;
        bIsEating = false;

        EatTimer = 5000;
        CastTimer = 5000;
    }

    void SpellHit(Unit* pCaster, SpellEntry const* pSpell)
    {
        if (bCanEat || bIsEating)
            return;

        if (pCaster->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_PLACE_CARCASS && !m_creature->HasAura(SPELL_JUST_EATEN))
        {
            m_playerGuid = pCaster->GetObjectGuid();
            bCanEat = true;
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == POINT_ID)
        {
            bIsEating = true;
            EatTimer = 7000;
            m_creature->HandleEmote(EMOTE_ONESHOT_ATTACKUNARMED);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (bCanEat || bIsEating)
        {
            if (EatTimer < diff)
            {
                if (bCanEat && !bIsEating)
                {
                    if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                    {
                        GameObject* pGo = pPlayer->GetGameObject(SPELL_PLACE_CARCASS);

                        // Workaround for broken function GetGameObject
                        if (!pGo)
                        {
                            const SpellEntry* pSpell = GetSpellStore()->LookupEntry(SPELL_PLACE_CARCASS);

                            uint32 uiGameobjectEntry = pSpell->EffectMiscValue[EFFECT_INDEX_0];

                            pGo = GetClosestGameObjectWithEntry(pPlayer, uiGameobjectEntry, 2*INTERACTION_DISTANCE);
                        }

                        if (pGo)
                        {
                            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
                                m_creature->GetMotionMaster()->MovementExpired();

                            m_creature->GetMotionMaster()->MoveIdle();
                            m_creature->StopMoving();

                            float fX, fY, fZ;
                            pGo->GetContactPoint(m_creature, fX, fY, fZ, CONTACT_DISTANCE);

                            m_creature->GetMotionMaster()->MovePoint(POINT_ID, fX, fY, fZ);
                        }
                    }
                    bCanEat = false;
                }
                else if (bIsEating)
                {
                    DoCastSpellIfCan(m_creature, SPELL_JUST_EATEN);
                    DoScriptText(SAY_JUST_EATEN, m_creature);

                    if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                        pPlayer->KilledMonsterCredit(NPC_EVENT_PINGER, m_creature->GetObjectGuid());

                    Reset();
                    m_creature->GetMotionMaster()->Clear();
                }
            }
            else
                EatTimer -= diff;

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CastTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_NETHER_BREATH);
            CastTimer = 5000;
        }else CastTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mature_netherwing_drake(Creature* pCreature)
{
    return new mob_mature_netherwing_drakeAI(pCreature);
}

/*###
# mob_enslaved_netherwing_drake
####*/

enum
{
    FACTION_DEFAULT                 = 62,
    FACTION_FRIENDLY                = 1840,                 // Not sure if this is correct, it was taken off of Mordenai.

    SPELL_HIT_FORCE_OF_NELTHARAKU   = 38762,
    SPELL_FORCE_OF_NELTHARAKU       = 38775,

    QUEST_FORCE_OF_NELT             = 10854,
    NPC_DRAGONMAW_SUBJUGATOR        = 21718,
    NPC_ESCAPE_DUMMY                = 21348
};

struct MANGOS_DLL_DECL mob_enslaved_netherwing_drakeAI : public ScriptedAI
{
    mob_enslaved_netherwing_drakeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Tapped = false;
        Reset();
    }

    ObjectGuid m_playerGuid;
    uint32 FlyTimer;
    bool Tapped;

    void Reset()
    {
        if (!Tapped)
            m_creature->setFaction(FACTION_DEFAULT);

        FlyTimer = 2500;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_HIT_FORCE_OF_NELTHARAKU && !Tapped)
        {
            if (Player* pPlayer = pCaster->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                Tapped = true;
                m_playerGuid = pPlayer->GetObjectGuid();

                m_creature->setFaction(FACTION_FRIENDLY);

                if (Creature* pDragonmaw = GetClosestCreatureWithEntry(m_creature, NPC_DRAGONMAW_SUBJUGATOR, 50.0f))
                    AttackStart(pDragonmaw);
            }
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 1)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (Tapped)
            {
                if (FlyTimer <= diff)
                {
                    Tapped = false;

                    if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                    {
                        if (pPlayer->GetQuestStatus(QUEST_FORCE_OF_NELT) == QUEST_STATUS_INCOMPLETE)
                        {
                            DoCastSpellIfCan(pPlayer, SPELL_FORCE_OF_NELTHARAKU, CAST_TRIGGERED);
                            m_playerGuid.Clear();

                            float dx, dy, dz;

                            if (Creature* EscapeDummy = GetClosestCreatureWithEntry(m_creature, NPC_ESCAPE_DUMMY, 30.0f))
                                EscapeDummy->GetPosition(dx, dy, dz);
                            else
                            {
                                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 20, dx, dy, dz);
                                dz += 25;
                            }

                            m_creature->GetMotionMaster()->MovePoint(1, dx, dy, dz);
                        }
                    }
                }
                else
                    FlyTimer -= diff;
            }
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_enslaved_netherwing_drake(Creature* pCreature)
{
    return new mob_enslaved_netherwing_drakeAI(pCreature);
}

/*#####
# npc_dragonmaw_peon
#####*/

enum
{
    SAY_PEON_1                      = -1000652,
    SAY_PEON_2                      = -1000653,
    SAY_PEON_3                      = -1000654,
    SAY_PEON_4                      = -1000655,
    SAY_PEON_5                      = -1000656,

    SPELL_SERVING_MUTTON            = 40468,
    NPC_DRAGONMAW_KILL_CREDIT       = 23209,
    EQUIP_ID_MUTTON                 = 2202,
    POINT_DEST                      = 1
};

struct MANGOS_DLL_DECL npc_dragonmaw_peonAI : public ScriptedAI
{
    npc_dragonmaw_peonAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_playerGuid;
    uint32 m_uiPoisonTimer;
    uint32 m_uiMoveTimer;
    uint32 m_uiEatTimer;

    void Reset()
    {
        m_playerGuid.Clear();
        m_uiPoisonTimer = 0;
        m_uiMoveTimer = 0;
        m_uiEatTimer = 0;

        SetEquipmentSlots(true);
    }

    bool SetPlayerTarget(ObjectGuid playerGuid)
    {
        // Check if event already started
        if (m_playerGuid)
            return false;

        m_playerGuid = playerGuid;
        m_uiMoveTimer = 500;
        return true;
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_DEST)
        {
            m_uiEatTimer = 2000;
            m_uiPoisonTimer = 3000;

            switch(urand(0, 4))
            {
                case 0: DoScriptText(SAY_PEON_1, m_creature); break;
                case 1: DoScriptText(SAY_PEON_2, m_creature); break;
                case 2: DoScriptText(SAY_PEON_3, m_creature); break;
                case 3: DoScriptText(SAY_PEON_4, m_creature); break;
                case 4: DoScriptText(SAY_PEON_5, m_creature); break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiMoveTimer)
        {
            if (m_uiMoveTimer <= uiDiff)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                {
                    GameObject* pMutton = pPlayer->GetGameObject(SPELL_SERVING_MUTTON);

                    // Workaround for broken function GetGameObject
                    if (!pMutton)
                    {
                        const SpellEntry* pSpell = GetSpellStore()->LookupEntry(SPELL_SERVING_MUTTON);

                        uint32 uiGameobjectEntry = pSpell->EffectMiscValue[EFFECT_INDEX_0];

                        // this can fail, but very low chance
                        pMutton = GetClosestGameObjectWithEntry(pPlayer, uiGameobjectEntry, 2*INTERACTION_DISTANCE);
                    }

                    if (pMutton)
                    {
                        float fX, fY, fZ;
                        pMutton->GetContactPoint(m_creature, fX, fY, fZ, CONTACT_DISTANCE);

                        m_creature->SetWalk(false);
                        m_creature->GetMotionMaster()->MovePoint(POINT_DEST, fX, fY, fZ);
                    }
                }

                m_uiMoveTimer = 0;
            }
            else
                m_uiMoveTimer -= uiDiff;
        }
        else if (m_uiEatTimer)
        {
            if (m_uiEatTimer <= uiDiff)
            {
                SetEquipmentSlots(false, EQUIP_ID_MUTTON, EQUIP_UNEQUIP);
                m_creature->HandleEmote(EMOTE_ONESHOT_EAT_NOSHEATHE);
                m_uiEatTimer = 0;
            }
            else
                m_uiEatTimer -= uiDiff;
        }
        else if (m_uiPoisonTimer)
        {
            if (m_uiPoisonTimer <= uiDiff)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                    pPlayer->KilledMonsterCredit(NPC_DRAGONMAW_KILL_CREDIT, m_creature->GetObjectGuid());

                m_uiPoisonTimer = 0;

                // dies
                m_creature->SetDeathState(JUST_DIED);
                m_creature->SetHealth(0);
            }
            else
                m_uiPoisonTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_dragonmaw_peon(Creature* pCreature)
{
    return new npc_dragonmaw_peonAI(pCreature);
}

bool EffectDummyCreature_npc_dragonmaw_peon(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    if (uiEffIndex != EFFECT_INDEX_1 || uiSpellId != SPELL_SERVING_MUTTON || pCaster->GetTypeId() != TYPEID_PLAYER)
        return false;

    npc_dragonmaw_peonAI* pPeonAI = dynamic_cast<npc_dragonmaw_peonAI*>(pCreatureTarget->AI());

    if (!pPeonAI)
        return false;

    if (pPeonAI->SetPlayerTarget(pCaster->GetObjectGuid()))
    {
        pCreatureTarget->HandleEmote(EMOTE_ONESHOT_NONE);
        return true;
    }

    return false;
}

/*######
# npc_wilda
######*/

enum
{
    SAY_WIL_START               = -1000381,
    SAY_WIL_AGGRO1              = -1000382,
    SAY_WIL_AGGRO2              = -1000383,
    SAY_WIL_PROGRESS1           = -1000384,
    SAY_WIL_PROGRESS2           = -1000385,
    SAY_WIL_FIND_EXIT           = -1000386,
    SAY_WIL_PROGRESS4           = -1000387,
    SAY_WIL_PROGRESS5           = -1000388,
    SAY_WIL_JUST_AHEAD          = -1000389,
    SAY_WIL_END                 = -1000390,

    SPELL_CHAIN_LIGHTNING       = 16006,
    SPELL_EARTHBING_TOTEM       = 15786,
    SPELL_FROST_SHOCK           = 12548,
    SPELL_HEALING_WAVE          = 12491,

    QUEST_ESCAPE_COILSCAR       = 10451,
    NPC_COILSKAR_ASSASSIN       = 21044,
    FACTION_EARTHEN             = 1726                      //guessed
};

//this script needs verification
struct MANGOS_DLL_DECL npc_wildaAI : public npc_escortAI
{
    npc_wildaAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    uint32 m_uiHealingTimer;

    void Reset()
    {
        m_uiHealingTimer = 0;
    }

    void WaypointReached(uint32 uiPointId)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch(uiPointId)
        {
            case 13:
                DoScriptText(SAY_WIL_PROGRESS1, m_creature, pPlayer);
                DoSpawnAssassin();
                break;
            case 14:
                DoSpawnAssassin();
                break;
            case 15:
                DoScriptText(SAY_WIL_FIND_EXIT, m_creature, pPlayer);
                break;
            case 19:
                DoRandomSay();
                break;
            case 20:
                DoSpawnAssassin();
                break;
            case 26:
                DoRandomSay();
                break;
            case 27:
                DoSpawnAssassin();
                break;
            case 33:
                DoRandomSay();
                break;
            case 34:
                DoSpawnAssassin();
                break;
            case 37:
                DoRandomSay();
                break;
            case 38:
                DoSpawnAssassin();
                break;
            case 39:
                DoScriptText(SAY_WIL_JUST_AHEAD, m_creature, pPlayer);
                break;
            case 43:
                DoRandomSay();
                break;
            case 44:
                DoSpawnAssassin();
                break;
            case 50:
                DoScriptText(SAY_WIL_END, m_creature, pPlayer);
                pPlayer->GroupEventHappens(QUEST_ESCAPE_COILSCAR, m_creature);
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_COILSKAR_ASSASSIN)
            pSummoned->AI()->AttackStart(m_creature);
    }

    //this is very unclear, random say without no real relevance to script/event
    void DoRandomSay()
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_WIL_PROGRESS2, m_creature); break;
            case 1: DoScriptText(SAY_WIL_PROGRESS4, m_creature); break;
            case 2: DoScriptText(SAY_WIL_PROGRESS5, m_creature); break;
        }
    }

    void DoSpawnAssassin()
    {
        //unknown where they actually appear
        float fX, fY, fZ;
        m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 15.0f, fX, fY, fZ);

        m_creature->SummonCreature(NPC_COILSKAR_ASSASSIN, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
    }

    void Aggro(Unit* pWho)
    {
        //don't always use
        if (urand(0, 4))
            return;

        //only aggro text if not player
        if (pWho->GetTypeId() != TYPEID_PLAYER)
        {
            //appears to be random
            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_WIL_AGGRO1, m_creature, pWho); break;
                case 1: DoScriptText(SAY_WIL_AGGRO2, m_creature, pWho); break;
            }
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //TODO: add more abilities
        if (m_creature->GetHealthPercent() <= 30.0f)
        {
            if (m_uiHealingTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_HEALING_WAVE);
                m_uiHealingTimer = 15000;
            }
            else
                m_uiHealingTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_wilda(Creature* pCreature)
{
    return new npc_wildaAI(pCreature);
}

bool QuestAccept_npc_wilda(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ESCAPE_COILSCAR)
    {
        DoScriptText(SAY_WIL_START, pCreature, pPlayer);
        pCreature->setFaction(FACTION_EARTHEN);

        if (npc_wildaAI* pEscortAI = dynamic_cast<npc_wildaAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest);
    }
    return true;
}

/*#####
# Quest: Battle of the Crimson Watch
#####*/

enum
{
    QUEST_BATTLE_OF_THE_CRIMSON_WATCH = 10781,

    EVENT_COOLDOWN                    = 30000,

    SAY_TORLOTH_DIALOGUE1             = -1000532,
    SAY_TORLOTH_DIALOGUE2             = -1000533,
    SAY_TORLOTH_DIALOGUE3             = -1000534,
    SAY_ILLIDAN_DIALOGUE              = -1000535,
    SAY_ILLIDAN_SUMMON1               = -1000536,
    SAY_ILLIDAN_SUMMON2               = -1000537,
    SAY_ILLIDAN_SUMMON3               = -1000538,
    SAY_ILLIDAN_SUMMON4               = -1000539,
    SAY_EVENT_COMPLETED               = -1000540,

    MODEL_ID_FELGUARD                 = 18654,
    MODEL_ID_DREADLORD                = 19991,

    NPC_ILLIDARI_SOLDIER              = 22075,
    NPC_ILLIDARI_MIND_BREAKER         = 22074,
    NPC_ILLIDARI_HIGHLORD             = 19797,
    NPC_TORLOTH_THE_MAGNIFICENT       = 22076,
    NPC_LORD_ILLIDAN                  = 22083
};

enum CinematicCreature
{
    LORD_ILLIDAN = 1,
    TORLOTH      = 0
};

const float EVENT_AREA_RADIUS = 65.0;

struct TorlothCinematic
{
    int32  iTextId;
    uint32 uiCreature;
    uint32 uiTimer;
};

static TorlothCinematic TorlothAnim[]=
{
    {SAY_TORLOTH_DIALOGUE1, TORLOTH, 2000},
    {SAY_ILLIDAN_DIALOGUE, LORD_ILLIDAN, 7000},
    {SAY_TORLOTH_DIALOGUE2, TORLOTH, 3000},
    {0, TORLOTH, 2000},                                  // Torloth stand
    {SAY_TORLOTH_DIALOGUE3, TORLOTH, 1000},
    {0, TORLOTH, 3000},
    {0, TORLOTH, 0}
};

struct Location
{
    float fLocX;
    float fLocY;
    float fLocZ;
    float fOrient;
};

static Location SpawnLocation[]=
{
    {-4615.8556f, 1342.2532f, 139.9f, 1.612f},              // Illidari Soldier
    {-4598.9365f, 1377.3182f, 139.9f, 3.917f},              // Illidari Soldier
    {-4598.4697f, 1360.8999f, 139.9f, 2.427f},              // Illidari Soldier
    {-4589.3599f, 1369.1061f, 139.9f, 3.165f},              // Illidari Soldier
    {-4608.3477f, 1386.0076f, 139.9f, 4.108f},              // Illidari Soldier
    {-4633.1889f, 1359.8033f, 139.9f, 0.949f},              // Illidari Soldier
    {-4623.5791f, 1351.4574f, 139.9f, 0.971f},              // Illidari Soldier
    {-4607.2988f, 1351.6099f, 139.9f, 2.416f},              // Illidari Soldier
    {-4633.7764f, 1376.0417f, 139.9f, 5.608f},              // Illidari Soldier
    {-4600.2461f, 1369.1240f, 139.9f, 3.056f},              // Illidari Mind Breaker
    {-4631.7808f, 1367.9459f, 139.9f, 0.020f},              // Illidari Mind Breaker
    {-4600.2461f, 1369.1240f, 139.9f, 3.056f},              // Illidari Highlord
    {-4631.7808f, 1367.9459f, 139.9f, 0.020f},              // Illidari Highlord
    {-4615.5586f, 1353.0031f, 139.9f, 1.540f},              // Illidari Highlord
    {-4616.4736f, 1384.2170f, 139.9f, 4.971f},              // Illidari Highlord
    {-4627.1240f, 1378.8752f, 139.9f, 2.544f}               // Torloth The Magnificent
};

struct WaveData
{
    uint8  uiSpawnCount;
    uint8  uiUsedSpawnPoint;
    uint32 uiCreatureId;
    uint32 uiSpawnTimer;
    uint32 uiYellTimer;
    int32  iTextId;
};

static WaveData WavesInfo[]=
{
    // Illidari Soldier
    {9, 0, NPC_ILLIDARI_SOLDIER, 10000, 7000, SAY_ILLIDAN_SUMMON1},
    // Illidari Mind Breaker
    {2, 9, NPC_ILLIDARI_MIND_BREAKER, 10000, 7000, SAY_ILLIDAN_SUMMON2},
    // Illidari Highlord
    {4, 11, NPC_ILLIDARI_HIGHLORD, 10000, 7000, SAY_ILLIDAN_SUMMON3},
    // Torloth The Magnificent
    {1, 15, NPC_TORLOTH_THE_MAGNIFICENT, 10000, 7000, SAY_ILLIDAN_SUMMON4}
};

/*######
# mob_torloth
#####*/

enum
{
    SPELL_CLEAVE           = 15284,
    SPELL_SHADOWFURY       = 39082,
    SPELL_SPELL_REFLECTION = 33961
};

struct MANGOS_DLL_DECL mob_torlothAI : public ScriptedAI
{
    mob_torlothAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ObjectGuid m_lordIllidanGuid;
    ObjectGuid m_playerGuid;

    uint32 m_uiCleaveTimer;
    uint32 m_uiShadowfuryTimer;
    uint32 m_uiSpellReflectionTimer;
    uint8  m_uiAnimationCount;
    uint32 m_uiAnimationTimer;

    void Reset()
    {
        m_lordIllidanGuid.Clear();
        m_playerGuid.Clear();

        m_uiAnimationCount = 0;
        m_uiAnimationTimer = 4000;
        m_uiCleaveTimer = 10000;
        m_uiShadowfuryTimer = 18000;
        m_uiSpellReflectionTimer = 25000;

        // make him not attackable for the time of animation
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
    }

    void EnterEvadeMode()
    {
        m_creature->ForcedDespawn();
    }

    void HandleAnimation()
    {
        Creature* pCreature = m_creature;

        if (TorlothAnim[m_uiAnimationCount].uiCreature == LORD_ILLIDAN)
        {
            pCreature = m_creature->GetMap()->GetCreature(m_lordIllidanGuid);

            if (!pCreature)
            {
                m_creature->ForcedDespawn();
                return;
            }
        }

        if (TorlothAnim[m_uiAnimationCount].iTextId)
            DoScriptText(TorlothAnim[m_uiAnimationCount].iTextId, pCreature);

        m_uiAnimationTimer = TorlothAnim[m_uiAnimationCount].uiTimer;

        switch(m_uiAnimationCount)
        {
            case 0:
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                break;
            case 3:
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                break;
            case 5:
                if (Player* pTarget = m_creature->GetMap()->GetPlayer(m_playerGuid))
                {
                    m_creature->AddThreat(pTarget);
                    m_creature->SetFacingToObject(pTarget);
                    m_creature->HandleEmote(EMOTE_ONESHOT_POINT);
                }
                break;
            case 6:
            {
                if (Player* pTarget = m_creature->GetMap()->GetPlayer(m_playerGuid))
                {
                    SetCombatMovement(true);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    float fLocX, fLocY, fLocZ;
                    pTarget->GetPosition(fLocX, fLocY, fLocZ);
                    m_creature->GetMotionMaster()->MovePoint(0, fLocX, fLocY, fLocZ);
                }
                break;
            }
        }

        ++m_uiAnimationCount;
    }

    void JustDied(Unit* pKiller)
    {
        if (Player* pPlayer = pKiller->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            pPlayer->GroupEventHappens(QUEST_BATTLE_OF_THE_CRIMSON_WATCH, m_creature);

            if (Creature* pLordIllidan = m_creature->GetMap()->GetCreature(m_lordIllidanGuid))
            {
                DoScriptText(SAY_EVENT_COMPLETED, pLordIllidan, pPlayer);
                pLordIllidan->AI()->EnterEvadeMode();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAnimationCount < 7)
        {
            if (m_uiAnimationTimer < uiDiff)
                HandleAnimation();
            else
                m_uiAnimationTimer -= uiDiff;
        }
        else
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (m_uiCleaveTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = 15000;
            }
            else
                m_uiCleaveTimer -= uiDiff;

            if (m_uiShadowfuryTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWFURY);
                m_uiShadowfuryTimer = 20000;
            }
            else
                m_uiShadowfuryTimer -= uiDiff;

            if (m_uiSpellReflectionTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_SPELL_REFLECTION);
                m_uiSpellReflectionTimer = 30000;
            }
            else
                m_uiSpellReflectionTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_mob_torloth(Creature* pCreature)
{
    return new mob_torlothAI(pCreature);
}

/*#####
# npc_lord_illidan_stormrage
#####*/

struct MANGOS_DLL_DECL npc_lord_illidan_stormrageAI : public Scripted_NoMovementAI
{
    npc_lord_illidan_stormrageAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) {Reset();}

    ObjectGuid m_playerGuid;
    uint32 m_uiWaveTimer;
    uint32 m_uiAnnounceTimer;
    uint32 m_uiCheckTimer;
    uint8  m_uiMobCount;
    uint8  m_uiWaveCount;

    bool m_bEventStarted;
    bool m_bEventFailed;
    bool m_bWaveAnnounced;

    void Reset()
    {
        m_playerGuid.Clear();

        m_uiWaveTimer = 10000;
        m_uiAnnounceTimer = 7000;
        m_uiCheckTimer = 2000;

        m_uiMobCount = 0;
        m_uiWaveCount = 0;

        m_bEventStarted = false;
        m_bEventFailed = false;
        m_bWaveAnnounced = false;
    }

    void StartEvent(Player* pPlayer)
    {
        m_bEventStarted = true;
        m_playerGuid = pPlayer->GetObjectGuid();
    }

    void SummonWave()
    {
        uint8 uiCount = WavesInfo[m_uiWaveCount].uiSpawnCount;
        uint8 uiLocIndex = WavesInfo[m_uiWaveCount].uiUsedSpawnPoint;
        uint8 uiFelguardCount = 0;
        uint8 uiDreadlordCount = 0;

        for(uint8 i = 0; i < uiCount; ++i)
        {
            float fLocX, fLocY, fLocZ, fOrient;
            fLocX = SpawnLocation[uiLocIndex + i].fLocX;
            fLocY = SpawnLocation[uiLocIndex + i].fLocY;
            fLocZ = SpawnLocation[uiLocIndex + i].fLocZ;
            fOrient = SpawnLocation[uiLocIndex + i].fOrient;

            if (Creature* pSpawn = m_creature->SummonCreature(WavesInfo[m_uiWaveCount].uiCreatureId, fLocX, fLocY, fLocZ, fOrient, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 15000))
            {

                if (m_uiWaveCount)                          // only in first wave
                    continue;

                if (!urand(0,2) && uiFelguardCount < 2)
                {
                    pSpawn->SetDisplayId(MODEL_ID_FELGUARD);
                    ++uiFelguardCount;
                }
                else if (uiDreadlordCount < 3)
                {
                    pSpawn->SetDisplayId(MODEL_ID_DREADLORD);
                    ++uiDreadlordCount;
                }
                else if (uiFelguardCount < 2)
                {
                    pSpawn->SetDisplayId(MODEL_ID_FELGUARD);
                    ++uiFelguardCount;
                }
            }
        }

        ++m_uiWaveCount;
        m_uiWaveTimer = WavesInfo[m_uiWaveCount].uiSpawnTimer;
        m_uiAnnounceTimer = WavesInfo[m_uiWaveCount].uiYellTimer;
    }

    void JustSummoned(Creature* pSummoned)
    {
        // increment mob count
        ++m_uiMobCount;

        if (!m_playerGuid)
            return;

        if (pSummoned->GetEntry() == NPC_TORLOTH_THE_MAGNIFICENT)
        {
            if (mob_torlothAI* pTorlothAI = dynamic_cast<mob_torlothAI*>(pSummoned->AI()))
            {
                pTorlothAI->m_lordIllidanGuid = m_creature->GetObjectGuid();
                pTorlothAI->m_playerGuid = m_playerGuid;
            }
        }
        else
        {
            if (Player* pTarget = m_creature->GetMap()->GetPlayer(m_playerGuid))
            {
                float fLocX, fLocY, fLocZ;
                pTarget->GetPosition(fLocX, fLocY, fLocZ);
                pSummoned->GetMotionMaster()->MovePoint(0, fLocX, fLocY, fLocZ);
            }
        }
    }

    void SummonedCreatureDespawn(Creature* pCreature)
    {
        // decrement mob count
        --m_uiMobCount;

        if (!m_uiMobCount)
            m_bWaveAnnounced = false;
    }

    void CheckEventFail()
    {
        Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid);

        if (!pPlayer)
            return;

        if (Group* pEventGroup = pPlayer->GetGroup())
        {
            uint8 uiDeadMemberCount = 0;
            uint8 uiFailedMemberCount = 0;

            for(GroupReference* pRef = pEventGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
            {
                if (Player* pMember = pRef->getSource())
                {
                    if (!pMember->isAlive())
                        ++uiDeadMemberCount;

                    // if we already failed no need to check other things
                    if (pMember->GetQuestStatus(QUEST_BATTLE_OF_THE_CRIMSON_WATCH) == QUEST_STATUS_FAILED)
                    {
                        ++uiFailedMemberCount;
                        continue;
                    }

                    // we left event area fail quest
                    if (!pMember->IsWithinDistInMap(m_creature, EVENT_AREA_RADIUS))
                    {
                        pMember->FailQuest(QUEST_BATTLE_OF_THE_CRIMSON_WATCH);
                        ++uiFailedMemberCount;
                    }
                }
            }

            if (pEventGroup->GetMembersCount() == uiFailedMemberCount)
            {
                m_bEventFailed = true;
                return;
            }

            if (pEventGroup->GetMembersCount() == uiDeadMemberCount)
            {
                for(GroupReference* pRef = pEventGroup->GetFirstMember(); pRef != NULL; pRef = pRef->next())
                {
                    if (Player* pMember = pRef->getSource())
                    {
                        if (pMember->GetQuestStatus(QUEST_BATTLE_OF_THE_CRIMSON_WATCH) == QUEST_STATUS_INCOMPLETE)
                            pMember->FailQuest(QUEST_BATTLE_OF_THE_CRIMSON_WATCH);
                    }
                }

                m_bEventFailed = true;
            }
        }
        else if (pPlayer->isDead() || !pPlayer->IsWithinDistInMap(m_creature, EVENT_AREA_RADIUS))
        {
            pPlayer->FailQuest(QUEST_BATTLE_OF_THE_CRIMSON_WATCH);
            m_bEventFailed = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_playerGuid || !m_bEventStarted)
            return;

        if (!m_uiMobCount && m_uiWaveCount < 4)
        {
            if (!m_bWaveAnnounced && m_uiAnnounceTimer < uiDiff)
            {
                DoScriptText(WavesInfo[m_uiWaveCount].iTextId, m_creature);
                m_bWaveAnnounced = true;
            }
            else
                m_uiAnnounceTimer -= uiDiff;

            if (m_uiWaveTimer < uiDiff)
                SummonWave();
            else
                m_uiWaveTimer -= uiDiff;
        }

        if (m_uiCheckTimer < uiDiff)
        {
            CheckEventFail();
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_bEventFailed)
            Reset();
    }
};

CreatureAI* GetAI_npc_lord_illidan_stormrage(Creature* (pCreature))
{
    return new npc_lord_illidan_stormrageAI(pCreature);
}

/*#####
# go_crystal_prison : GameObject that begins the event and hands out quest
######*/
bool GOQuestAccept_GO_crystal_prison(Player* pPlayer, GameObject* pGo, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_BATTLE_OF_THE_CRIMSON_WATCH)
        if (Creature* pLordIllidan = GetClosestCreatureWithEntry(pPlayer, NPC_LORD_ILLIDAN, 50.0))
            if (npc_lord_illidan_stormrageAI* pIllidanAI = dynamic_cast<npc_lord_illidan_stormrageAI*>(pLordIllidan->AI()))
                if (!pIllidanAI->m_bEventStarted)
                    pIllidanAI->StartEvent(pPlayer);

    return true;
}

/*######
## npc_totem_of_spirits
######*/

enum
{
    QUEST_SPIRITS_FIRE_AND_EARTH        = 10458,
    QUEST_SPIRITS_WATER                 = 10480,
    QUEST_SPIRITS_AIR                   = 10481,

    // quest 10458, 10480, 10481
    SPELL_ELEMENTAL_SIEVE               = 36035,
    NPC_TOTEM_OF_SPIRITS                = 21071,
    NPC_EARTH_SPIRIT                    = 21050,            // to be killed
    NPC_FIERY_SPIRIT                    = 21061,
    NPC_WATER_SPIRIT                    = 21059,
    NPC_AIR_SPIRIT                      = 21060,
    SPELL_EARTH_CAPTURED                = 36025,            // dummies (having visual effects)
    SPELL_FIERY_CAPTURED                = 36115,
    SPELL_WATER_CAPTURED                = 36170,
    SPELL_AIR_CAPTURED                  = 36181,
    SPELL_EARTH_CAPTURED_CREDIT         = 36108,            // event 13513
    SPELL_FIERY_CAPTURED_CREDIT         = 36117,            // event 13514
    SPELL_WATER_CAPTURED_CREDIT         = 36171,            // event 13515
    SPELL_AIR_CAPTURED_CREDIT           = 36182,            // event 13516
    EVENT_EARTH                         = 13513,
    EVENT_FIERY                         = 13514,
    EVENT_WATER                         = 13515,
    EVENT_AIR                           = 13516,
    NPC_CREDIT_MARKER_EARTH             = 21092,            // quest objective npc's
    NPC_CREDIT_MARKER_FIERY             = 21094,
    NPC_CREDIT_MARKER_WATER             = 21095,
    NPC_CREDIT_MARKER_AIR               = 21096,
};

struct MANGOS_DLL_DECL npc_totem_of_spiritsAI : public ScriptedPetAI
{
    npc_totem_of_spiritsAI(Creature* pCreature) : ScriptedPetAI(pCreature) { Reset(); }

    void Reset() {}

    void MoveInLineOfSight(Unit* pWho) {}
    void UpdateAI(const uint32 uiDiff) {}
    void AttackedBy(Unit* pAttacker) {}

    void OwnerKilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_UNIT)
            return;

        uint32 uiEntry = pVictim->GetEntry();

        // make elementals cast the sieve is only way to make it work properly, due to the spell target modes 22/7
        if (uiEntry == NPC_EARTH_SPIRIT || uiEntry == NPC_FIERY_SPIRIT || uiEntry == NPC_WATER_SPIRIT || uiEntry == NPC_AIR_SPIRIT)
            pVictim->CastSpell(pVictim, SPELL_ELEMENTAL_SIEVE, true);
    }
};

CreatureAI* GetAI_npc_totem_of_spirits(Creature* pCreature)
{
    return new npc_totem_of_spiritsAI(pCreature);
}

bool EffectDummyCreature_npc_totem_of_spirits(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    if (uiEffIndex != EFFECT_INDEX_0)
        return false;

    switch(uiSpellId)
    {
        case SPELL_EARTH_CAPTURED:
        {
            pCaster->CastSpell(pCaster, SPELL_EARTH_CAPTURED_CREDIT, true);
            return true;
        }
        case SPELL_FIERY_CAPTURED:
        {
            pCaster->CastSpell(pCaster, SPELL_FIERY_CAPTURED_CREDIT, true);
            return true;
        }
        case SPELL_WATER_CAPTURED:
        {
            pCaster->CastSpell(pCaster, SPELL_WATER_CAPTURED_CREDIT, true);
            return true;
        }
        case SPELL_AIR_CAPTURED:
        {
            pCaster->CastSpell(pCaster, SPELL_AIR_CAPTURED_CREDIT, true);
            return true;
        }
    }

    return false;
}

bool EffectAuraDummy_npc_totem_of_spirits(const Aura* pAura, bool bApply)
{
    if (pAura->GetId() != SPELL_ELEMENTAL_SIEVE)
        return true;

    if (pAura->GetEffIndex() != EFFECT_INDEX_0)
        return true;

    if (bApply)                                             // possible it should be some visual effects, using "enraged soul" npc and "Cosmetic: ... soul" spell
        return true;

    Creature* pCreature = (Creature*)pAura->GetTarget();
    Unit* pCaster = pAura->GetCaster();

    // aura only affect the spirit totem, since this is the one that need to be in range.
    // It is possible though, that player is the one who should actually have the aura
    // and check for presense of spirit totem, but then we can't script the dummy.
    if (!pCreature || !pCreature->IsPet() || !pCaster)
        return true;

    // Need to expect the enraged elementals to be caster of aura
    switch(pCaster->GetEntry())
    {
        case NPC_EARTH_SPIRIT:
            pCreature->CastSpell(pCreature, SPELL_EARTH_CAPTURED, true);
            break;
        case NPC_FIERY_SPIRIT:
            pCreature->CastSpell(pCreature, SPELL_FIERY_CAPTURED, true);
            break;
        case NPC_WATER_SPIRIT:
            pCreature->CastSpell(pCreature, SPELL_WATER_CAPTURED, true);
            break;
        case NPC_AIR_SPIRIT:
            pCreature->CastSpell(pCreature, SPELL_AIR_CAPTURED, true);
            break;
    }

    return true;
}

bool ProcessEventId_event_spell_soul_captured_credit(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_UNIT)
    {
        Player* pOwner = (Player*)((Creature*)pSource)->GetOwner();

        if (!pOwner)
            return true;

        switch(uiEventId)
        {
            case EVENT_EARTH:
                pOwner->KilledMonsterCredit(NPC_CREDIT_MARKER_EARTH);
                return true;
            case EVENT_FIERY:
                pOwner->KilledMonsterCredit(NPC_CREDIT_MARKER_FIERY);
                return true;
            case EVENT_WATER:
                pOwner->KilledMonsterCredit(NPC_CREDIT_MARKER_WATER);
                return true;
            case EVENT_AIR:
                pOwner->KilledMonsterCredit(NPC_CREDIT_MARKER_AIR);
                return true;
        }
    }

    return false;
}

/*#####
#npc_spawned_oronok_tornheart
#####*/

enum
{
    // texts
    SAY_ORONOK_TOGETHER             = -1000803,
    SAY_ORONOK_READY                = -1000804,
    SAY_ORONOK_ELEMENTS             = -1000805,
    SAY_ORONOK_EPILOGUE_1           = -1000806,
    SAY_TORLOK_EPILOGUE_2           = -1000807,
    SAY_ORONOK_EPILOGUE_3           = -1000808,
    SAY_EARTH_EPILOGUE_4            = -1000809,
    SAY_FIRE_EPILOGUE_5             = -1000810,
    SAY_EARTH_EPILOGUE_6            = -1000811,
    SAY_ORONOK_EPILOGUE_7           = -1000812,
    EMOTE_GIVE_WEAPONS              = -1000813,
    SAY_ORONOK_EPILOGUE_8           = -1000814,

    GOSSIP_ITEM_FIGHT               = -3000109,
    GOSSIP_TEXT_ID_ORONOK           = 10421,

    // spells - some are already defined above
    //SPELL_CHAIN_LIGHTNING         = 16006,
    SPELL_EARTHBIND_TOTEM           = 15786,
    //SPELL_FROST_SHOCK             = 12548,
    //SPELL_HEALING_WAVE            = 12491,

    // npcs
    NPC_ORONOK_TORN_HEART           = 21685,
    NPC_GROMTOR_SON_OF_ORONOK       = 21687,
    NPC_BORAK_SON_OF_ORONOK         = 21686,
    NPC_CYRUKH_THE_FIRELORD         = 21181,
    //NPC_EARTH_SPIRIT              = 21050,
    NPC_REDEEMED_SPIRIT_OF_EARTH    = 21739,
    NPC_REDEEMED_SPIRIT_OF_FIRE     = 21740,
    NPC_REDEEMED_SPIRIT_OF_AIR      = 21738,
    NPC_REDEEMED_SPIRIT_OF_WATER    = 21741,
    NPC_EARTHMENDER_TORLOK          = 21024,

    GO_MARK_OF_KAELTHAS             = 185170,

    QUEST_CIPHER_OF_DAMNATION       = 10588,

    POINT_ID_ATTACK_READY           = 1,
    POINT_ID_ELEMENTS               = 2,
    POINT_ID_EPILOGUE               = 3,
};

static const DialogueEntry aOutroDialogue[] =
{
    {QUEST_CIPHER_OF_DAMNATION,     0,                              1000},
    {NPC_CYRUKH_THE_FIRELORD,       0,                              0},
    {NPC_EARTHMENDER_TORLOK,        0,                              1000},
    {SAY_ORONOK_EPILOGUE_1,         NPC_ORONOK_TORN_HEART,          5000},
    {SAY_TORLOK_EPILOGUE_2,         NPC_EARTHMENDER_TORLOK,         5000},
    {NPC_REDEEMED_SPIRIT_OF_EARTH,  0,                              5000},
    {SAY_ORONOK_EPILOGUE_3,         NPC_ORONOK_TORN_HEART,          5000},
    {SAY_EARTH_EPILOGUE_4,          NPC_REDEEMED_SPIRIT_OF_EARTH,   5000},
    {SAY_FIRE_EPILOGUE_5,           NPC_REDEEMED_SPIRIT_OF_FIRE,    14000},
    {SAY_EARTH_EPILOGUE_6,          NPC_REDEEMED_SPIRIT_OF_EARTH,   6000},
    {SAY_ORONOK_EPILOGUE_7,         NPC_ORONOK_TORN_HEART,          6000},
    {EMOTE_GIVE_WEAPONS,            NPC_ORONOK_TORN_HEART,          6000},
    {SAY_ORONOK_EPILOGUE_8,         NPC_ORONOK_TORN_HEART,          10000},
    {NPC_ORONOK_TORN_HEART,         0,                              0},
    {0, 0, 0},
};

struct EventLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

const static EventLocations aDamnationLocations[] =
{
    {-3605.09f, 1885.47f, 47.24f, 1.81f},      // 0 fire spirit summon loc
    {-3600.68f, 1886.58f, 47.24f, 1.81f},      // 1 earth spirit summon loc
    {-3597.19f, 1887.46f, 47.24f, 1.77f},      // 2 water spirit summon loc
    {-3593.18f, 1888.27f, 47.24f, 1.77f},      // 3 air spirit summon loc
    {-3595.36f, 1869.78f, 47.24f},             // 4 fight ready move loc
    {-3635.90f, 1860.94f, 52.93f},             // 5 elementals move loc
    {-3599.71f, 1897.94f, 47.24f}              // 6 epilogue move loc
};

struct MANGOS_DLL_DECL npc_spawned_oronok_tornheartAI : public ScriptedAI, private DialogueHelper
{
    npc_spawned_oronok_tornheartAI(Creature* pCreature) : ScriptedAI(pCreature),
        DialogueHelper(aOutroDialogue)
    {
        Reset();
        StartNextDialogueText(QUEST_CIPHER_OF_DAMNATION);
    }

    uint32 m_uiLightningTimer;
    uint32 m_uiTotemTimer;
    uint32 m_uiFrostTimer;
    uint32 m_uiHealTimer;

    ObjectGuid m_torlokGuid;
    ObjectGuid m_fireSpiritGuid;
    ObjectGuid m_earthSpiritGuid;
    ObjectGuid m_borakGuid;
    ObjectGuid m_gromtorGuid;
    ObjectGuid m_cyrukhGuid;

    bool m_bHasAttackStart;

    void Reset()
    {
        m_uiLightningTimer  = 15000;
        m_uiTotemTimer      = 10000;
        m_uiFrostTimer      = 20000;
        m_uiHealTimer       = 8000;

        m_bHasAttackStart   = false;
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        switch(iEntry)
        {
            case NPC_CYRUKH_THE_FIRELORD:
                // Set them in motion
                m_creature->SetWalk(false);
                m_creature->GetMotionMaster()->MovePoint(POINT_ID_ATTACK_READY, aDamnationLocations[4].m_fX, aDamnationLocations[4].m_fY, aDamnationLocations[4].m_fZ);
                if (Creature* pBorak = GetClosestCreatureWithEntry(m_creature, NPC_BORAK_SON_OF_ORONOK, 10.0f))
                {
                    m_borakGuid = pBorak->GetObjectGuid();
                    pBorak->GetMotionMaster()->MoveFollow(m_creature, 5.0f, -M_PI_F/2);
                }
                if (Creature* pGromtor = GetClosestCreatureWithEntry(m_creature, NPC_GROMTOR_SON_OF_ORONOK, 10.0f))
                {
                    m_gromtorGuid = pGromtor->GetObjectGuid();
                    pGromtor->GetMotionMaster()->MoveFollow(m_creature, 5.0f, M_PI_F/2);
                }
                break;
            case NPC_EARTHMENDER_TORLOK:
                if (Creature* pTorlok = GetClosestCreatureWithEntry(m_creature, NPC_EARTHMENDER_TORLOK, 25.0f))
                {
                    m_torlokGuid = pTorlok->GetObjectGuid();
                    m_creature->SetFacingToObject(pTorlok);
                }
                break;
            case NPC_REDEEMED_SPIRIT_OF_EARTH:
                m_creature->SetFacingTo(4.9f);
                m_creature->SummonCreature(NPC_REDEEMED_SPIRIT_OF_FIRE, aDamnationLocations[0].m_fX, aDamnationLocations[0].m_fY, aDamnationLocations[0].m_fZ, aDamnationLocations[0].m_fO, TEMPSUMMON_TIMED_DESPAWN, 32000);
                m_creature->SummonCreature(NPC_REDEEMED_SPIRIT_OF_EARTH, aDamnationLocations[1].m_fX, aDamnationLocations[1].m_fY, aDamnationLocations[1].m_fZ, aDamnationLocations[1].m_fO, TEMPSUMMON_TIMED_DESPAWN, 32000);
                m_creature->SummonCreature(NPC_REDEEMED_SPIRIT_OF_WATER, aDamnationLocations[2].m_fX, aDamnationLocations[2].m_fY, aDamnationLocations[2].m_fZ, aDamnationLocations[2].m_fO, TEMPSUMMON_TIMED_DESPAWN, 32000);
                m_creature->SummonCreature(NPC_REDEEMED_SPIRIT_OF_AIR, aDamnationLocations[3].m_fX, aDamnationLocations[3].m_fY, aDamnationLocations[3].m_fZ, aDamnationLocations[3].m_fO, TEMPSUMMON_TIMED_DESPAWN, 32000);
                break;
            case SAY_ORONOK_EPILOGUE_7:
                if (Creature* pTorlok = m_creature->GetMap()->GetCreature(m_torlokGuid))
                    m_creature->SetFacingToObject(pTorlok);
                break;
            case NPC_ORONOK_TORN_HEART:
                if (GameObject* pMark = GetClosestGameObjectWithEntry(m_creature, GO_MARK_OF_KAELTHAS, 30.0f))
                {
                    pMark->SetRespawnTime(5*MINUTE);
                    pMark->Refresh();
                }
                if (Creature* pBorak = m_creature->GetMap()->GetCreature(m_borakGuid))
                    pBorak->ForcedDespawn();
                if (Creature* pGromtor = m_creature->GetMap()->GetCreature(m_gromtorGuid))
                    pGromtor->ForcedDespawn();
                m_creature->ForcedDespawn();
                break;
        }
    }

    Creature* GetSpeakerByEntry(uint32 uiEntry)
    {
        switch (uiEntry)
        {
            case NPC_ORONOK_TORN_HEART:         return m_creature;
            case NPC_EARTHMENDER_TORLOK:        return m_creature->GetMap()->GetCreature(m_torlokGuid);
            case NPC_REDEEMED_SPIRIT_OF_EARTH:  return m_creature->GetMap()->GetCreature(m_earthSpiritGuid);
            case NPC_REDEEMED_SPIRIT_OF_FIRE:   return m_creature->GetMap()->GetCreature(m_fireSpiritGuid);

            default:
                return NULL;
        }
    }

    void Aggro(Unit* pWho)
    {
        if (!m_bHasAttackStart && pWho->GetEntry() == NPC_EARTH_SPIRIT)
        {
            // Cyrukh starts to attack
            if (Creature* pCyrukh = m_creature->GetMap()->GetCreature(m_cyrukhGuid))
            {
                pCyrukh->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                pCyrukh->AI()->AttackStart(m_creature);
                AttackStart(pCyrukh);
                m_bHasAttackStart = true;
            }
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch (pSummoned->GetEntry())
        {
            case NPC_REDEEMED_SPIRIT_OF_FIRE:
                m_fireSpiritGuid = pSummoned->GetObjectGuid();
                break;
            case NPC_REDEEMED_SPIRIT_OF_EARTH:
                m_earthSpiritGuid = pSummoned->GetObjectGuid();
                break;
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreatureAddon(true);

        m_creature->SetLootRecipient(NULL);

        Reset();

        if (!m_creature->isAlive())
            return;

        if (Creature* pCyrukh = m_creature->GetMap()->GetCreature(m_cyrukhGuid))
        {
            if (!pCyrukh->isAlive())
                m_creature->GetMotionMaster()->MovePoint(POINT_ID_EPILOGUE, aDamnationLocations[6].m_fX, aDamnationLocations[6].m_fY, aDamnationLocations[6].m_fZ);
        }
        else
        {
            error_log("SD2: Npc %u couldn't be found or something really bad happened. Epilogue event for quest %u will stop.", NPC_CYRUKH_THE_FIRELORD, QUEST_CIPHER_OF_DAMNATION);
            m_creature->GetMotionMaster()->MoveTargetedHome();
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE)
            return;

        switch (uiPointId)
        {
            case POINT_ID_ATTACK_READY:
                // Get Cyrukh guid now, because we are at a closer distance
                if (Creature* pCyrukh = GetClosestCreatureWithEntry(m_creature, NPC_CYRUKH_THE_FIRELORD, 70.0f))
                    m_cyrukhGuid = pCyrukh->GetObjectGuid();

                m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                DoScriptText(SAY_ORONOK_READY, m_creature);
                break;
            case POINT_ID_ELEMENTS:
                // Attack the closest earth element
                if (Creature* pElement = GetClosestCreatureWithEntry(m_creature, NPC_EARTH_SPIRIT, 50.0f))
                    AttackStart(pElement);
                break;
            case POINT_ID_EPILOGUE:
                StartNextDialogueText(NPC_EARTHMENDER_TORLOK);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        DialogueUpdate(uiDiff);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLightningTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_CHAIN_LIGHTNING) == CAST_OK)
                    m_uiLightningTimer = urand(9000, 15000);
            }
        }
        else
            m_uiLightningTimer -= uiDiff;

        if (m_uiTotemTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_EARTHBIND_TOTEM) == CAST_OK)
                m_uiTotemTimer = urand(40000, 60000);
        }
        else
            m_uiTotemTimer -= uiDiff;

        if (m_uiFrostTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_SHOCK) == CAST_OK)
                m_uiFrostTimer = urand(14000, 18000);
        }
        else
            m_uiFrostTimer -= uiDiff;

        if (m_uiHealTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_HEALING_WAVE) == CAST_OK)
                m_uiHealTimer = urand(6000, 10000);
        }
        else
            m_uiHealTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_spawned_oronok_tornheart(Creature* pCreature)
{
    return new npc_spawned_oronok_tornheartAI(pCreature);
}

bool GossipHello_npc_spawned_oronok_tornheart(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_CIPHER_OF_DAMNATION) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_ORONOK, pCreature->GetObjectGuid());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_spawned_oronok_tornheart(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        // Note: this movement expects MMaps.
        DoScriptText(SAY_ORONOK_ELEMENTS, pCreature);
        pCreature->GetMotionMaster()->MovePoint(POINT_ID_ELEMENTS, aDamnationLocations[5].m_fX, aDamnationLocations[5].m_fY, aDamnationLocations[5].m_fZ);
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

void AddSC_shadowmoon_valley()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_mature_netherwing_drake";
    pNewScript->GetAI = &GetAI_mob_mature_netherwing_drake;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_enslaved_netherwing_drake";
    pNewScript->GetAI = &GetAI_mob_enslaved_netherwing_drake;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_dragonmaw_peon";
    pNewScript->GetAI = &GetAI_npc_dragonmaw_peon;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_npc_dragonmaw_peon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_wilda";
    pNewScript->GetAI = &GetAI_npc_wilda;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_wilda;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_lord_illidan_stormrage";
    pNewScript->GetAI = &GetAI_npc_lord_illidan_stormrage;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_torloth";
    pNewScript->GetAI = &GetAI_mob_torloth;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_totem_of_spirits";
    pNewScript->GetAI = &GetAI_npc_totem_of_spirits;
    pNewScript->pEffectDummyNPC = &EffectDummyCreature_npc_totem_of_spirits;
    pNewScript->pEffectAuraDummy = &EffectAuraDummy_npc_totem_of_spirits;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_spell_soul_captured_credit";
    pNewScript->pProcessEventId = &ProcessEventId_event_spell_soul_captured_credit;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_crystal_prison";
    pNewScript->pQuestAcceptGO = &GOQuestAccept_GO_crystal_prison;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_spawned_oronok_tornheart";
    pNewScript->GetAI = &GetAI_npc_spawned_oronok_tornheart;
    pNewScript->pGossipHello =  &GossipHello_npc_spawned_oronok_tornheart;
    pNewScript->pGossipSelect = &GossipSelect_npc_spawned_oronok_tornheart;
    pNewScript->RegisterSelf();
}

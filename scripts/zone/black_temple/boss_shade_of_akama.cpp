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
SDName: Boss_Shade_of_Akama
SD%Complete: 90
SDComment: Seems to be complete.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "def_black_temple.h"

#define SAY_DEATH                   -1564013
#define SAY_LOW_HEALTH              -1564014
// Ending cinematic text
#define SAY_FREE                    -1564015
#define SAY_BROKEN_FREE_01          -1564016
#define SAY_BROKEN_FREE_02          -1564017

#define GOSSIP_ITEM                 "We are ready to fight alongside you, Akama"

struct Location
{
    float x, y, o, z;
};

static Location ChannelerLocations[]=
{
    {463.161285, 401.219757, 3.141592},
    {457.377625, 391.227661, 2.106461},
    {446.012421, 391.227661, 1.071904},
    {439.533783, 401.219757, 0.000000},
    {446.012421, 411.211853, 5.210546},
    {457.377625, 411.211853, 4.177494}
};

static Location SpawnLocations[]=
{
    {498.652740, 461.728119, 0},
    {498.505003, 339.619324, 0}
};

static Location AkamaWP[]=
{
    {482.352448, 401.162720, 0, 112.783928},
    {469.597443, 402.264404, 0, 118.537910}
};

static Location BrokenCoords[]=
{
    {541.375916, 401.439575, M_PI, 112.783997},             // The place where Akama channels
    {534.130005, 352.394531, 2.164150, 112.783737},         // Behind a 'pillar' which is behind the east alcove
    {499.621185, 341.534729, 1.652856, 112.783730},         // East Alcove
    {499.151093, 461.036438, 4.770888, 112.78370},          // West Alcove
};

static Location BrokenWP[]=
{
    {492.491638, 400.744690, 3.122336, 112.783737},
    {494.335724, 382.221771, 2.676230, 112.783737},
    {489.555939, 373.507202, 2.416263, 112.783737},
    {491.136353, 427.868774, 3.519748, 112.783737},
};

// Locations
#define Z1              118.543144
#define Z2              120.783768
#define Z_SPAWN         113.537949
#define AGGRO_X         482.793182
#define AGGRO_Y         401.270172
#define AGGRO_Z         112.783928
#define AKAMA_X         514.583984
#define AKAMA_Y         400.601013
#define AKAMA_Z         112.783997

// Spells
#define SPELL_VERTEX_SHADE_BLACK    39833
#define SPELL_SHADE_SOUL_CHANNEL    40401
#define SPELL_DESTRUCTIVE_POISON    40874
#define SPELL_LIGHTNING_BOLT        42024
#define SPELL_AKAMA_SOUL_CHANNEL    40447
#define SPELL_AKAMA_SOUL_RETRIEVE   40902
#define AKAMA_SOUL_EXPEL            40855

// Channeler entry
#define CREATURE_CHANNELER          23421
#define CREATURE_SORCERER           23215
#define CREATURE_DEFENDER           23216
#define CREATURE_BROKEN             23319

const uint32 spawnEntries[4]= { 23523, 23318, 23524 };

struct MANGOS_DLL_DECL boss_shade_of_akamaAI : public ScriptedAI
{
    boss_shade_of_akamaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    std::list<uint64> Channelers;
    std::list<uint64> Sorcerers;
    uint64 AkamaGUID;

    uint32 SorcererCount;
    uint32 DeathCount;

    uint32 ReduceHealthTimer;
    uint32 SummonTimer;
    uint32 ResetTimer;
    uint32 DefenderTimer;                                   // They are on a flat 15 second timer, independant of the other summon creature timer.

    bool IsBanished;
    bool HasKilledAkama;

    void Reset()
    {
        FindChannelers();

        if (!Channelers.empty())
        {
            for(std::list<uint64>::iterator itr = Channelers.begin(); itr != Channelers.end(); ++itr)
            {
                Creature* Channeler = NULL;
                Channeler = ((Creature*)Unit::GetUnit(*m_creature, *itr));

                if (Channeler)
                {
                    if (Channeler->isDead())
                    {
                        Channeler->RemoveCorpse();
                        Channeler->Respawn();
                    }

                    Channeler->CastSpell(m_creature, SPELL_SHADE_SOUL_CHANNEL, true);
                    Channeler->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }
        }else error_log("SD2 ERROR: No Channelers are stored in the list. This encounter will not work properly");

        if (!Sorcerers.empty())
        {
            for(std::list<uint64>::iterator itr = Sorcerers.begin(); itr != Sorcerers.end(); ++itr)
            {
                if (Creature* Sorcerer = ((Creature*)Unit::GetUnit(*m_creature, *itr)))
                    if (Sorcerer->isAlive())
                    {
                        Sorcerer->SetVisibility(VISIBILITY_OFF);
                        Sorcerer->DealDamage(Sorcerer, Sorcerer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
            }
        }

        Sorcerers.clear();

        SorcererCount = 0;
        DeathCount = 0;

        SummonTimer = 10000;
        ReduceHealthTimer = 0;
        ResetTimer = 60000;
        DefenderTimer = 15000;

        IsBanished = true;
        HasKilledAkama = false;

        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);

        if (m_pInstance)
        {
            if (m_creature->isAlive())
            {
                m_pInstance->SetData(DATA_SHADEOFAKAMAEVENT, NOT_STARTED);
            } else OpenMotherDoor();

            if (Unit* pAkama = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_AKAMA_SHADE)))
                pAkama->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
    }

    void OpenMotherDoor()
    {
        if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_PRE_SHAHRAZ_DOOR)))
            pDoor->SetGoState(GO_STATE_ACTIVE);
    }

    void AttackStart(Unit* who)
    {
        if (!who || IsBanished || who == m_creature)
            return;

        ScriptedAI::AttackStart(who);
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (IsBanished)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void IncrementDeathCount(uint64 guid = 0)               // If guid is set, will remove it from list of sorcerer
    {
        debug_log("SD2: Increasing Death Count for Shade of Akama encounter");
        ++DeathCount;

        if (guid)
        {
            if (Sorcerers.empty())
                error_log("SD2 ERROR: Shade of Akama - attempt to remove guid %u from Sorcerers list but list is already empty", guid);
            else Sorcerers.remove(guid);
        }
    }

    void SummonCreature()
    {
        uint32 random = rand()%2;
        float X = SpawnLocations[random].x;
        float Y = SpawnLocations[random].y;

        // max of 6 sorcerers can be summoned
        if ((rand()%3 == 0) && (DeathCount > 0) && (SorcererCount < 7))
        {
            Creature* Sorcerer = m_creature->SummonCreature(CREATURE_SORCERER, X, Y, Z_SPAWN, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (Sorcerer)
            {
                Sorcerer->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                Sorcerer->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                Sorcerer->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                Sorcerers.push_back(Sorcerer->GetGUID());
                --DeathCount;
                ++SorcererCount;
            }
        }
        else
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                Creature* Spawn = m_creature->SummonCreature(spawnEntries[i], X, Y, Z_SPAWN, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
                if (Spawn)
                {
                    Spawn->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                    Spawn->GetMotionMaster()->MovePoint(0, AGGRO_X, AGGRO_Y, AGGRO_Z);
                }
            }
        }
    }

    void FindChannelers()
    {
        std::list<Creature*> ChannelerList;
        GetCreatureListWithEntryInGrid(ChannelerList,m_creature,CREATURE_CHANNELER,50.0f);

        if (!ChannelerList.empty())
        {
            Channelers.clear();

            for(std::list<Creature*>::iterator itr = ChannelerList.begin(); itr != ChannelerList.end(); ++itr)
            {
                Channelers.push_back((*itr)->GetGUID());
                debug_log("SD2: Shade of Akama Grid Search found channeler %u. Adding to list", (*itr)->GetGUID());
            }
        }else error_log("SD2 ERROR: Grid Search was unable to find any channelers. Shade of Akama encounter will be buggy");
    }

    void SetSelectableChannelers()
    {
        if (Channelers.empty())
        {
            error_log("SD2 ERROR: Channeler List is empty, Shade of Akama encounter will be buggy");
            return;
        }

        for(std::list<uint64>::iterator itr = Channelers.begin(); itr != Channelers.end(); ++itr)
            if (Creature* Channeler = ((Creature*)Unit::GetUnit(*m_creature, *itr)))
                Channeler->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(DATA_SHADEOFAKAMAEVENT, DONE);
            OpenMotherDoor();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->isInCombat())
            return;

        if (IsBanished)
        {
            // Akama is set in the threatlist so when we reset, we make sure that he is not included in our check
            if (m_creature->getThreatManager().getThreatList().size() < 2)
                ScriptedAI::EnterEvadeMode();

            if (DefenderTimer < diff)
            {
                uint32 ran = rand()%2;
                Creature* Defender = m_creature->SummonCreature(CREATURE_DEFENDER, SpawnLocations[ran].x, SpawnLocations[ran].y, Z_SPAWN, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
                if (Defender)
                {
                    Defender->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                    bool move = true;

                    if (Unit* pAkama = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_AKAMA_SHADE)))
                        Defender->AI()->AttackStart(pAkama);
                    else
                        move = false;

                    if (!move)
                        Defender->GetMotionMaster()->MovePoint(0, AKAMA_X, AKAMA_Y, AKAMA_Z);
                }
                DefenderTimer = 15000;
            }else DefenderTimer -= diff;

            if (SummonTimer < diff)
            {
                SummonCreature();
                SummonTimer = 35000;
            }else SummonTimer -= diff;

            if (DeathCount >= 6)
            {
                if (Unit* pAkama = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_AKAMA_SHADE)))
                {
                    if (pAkama && pAkama->isAlive())
                    {
                        IsBanished = false;
                        m_creature->GetMotionMaster()->Clear(false);
                        m_creature->GetMotionMaster()->MoveChase(pAkama);

                        // Shade should move to Akama, not the other way around
                        pAkama->GetMotionMaster()->Clear();
                        pAkama->GetMotionMaster()->MoveIdle();

                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                        AttackStart(pAkama);

                        // Crazy amount of threat
                        m_creature->AddThreat(pAkama, 10000000.0f);
                        pAkama->AddThreat(m_creature, 10000000.0f);
                    }
                }
            }
        }
        else                                                // No longer banished, let's fight Akama now
        {
            if (ReduceHealthTimer < diff)
            {
                if (Unit* pAkama = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_AKAMA_SHADE)))
                {
                    if (pAkama && pAkama->isAlive())
                    {
                        //10 % less health every few seconds.
                        m_creature->DealDamage(pAkama, pAkama->GetMaxHealth()/10, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        ReduceHealthTimer = 12000;
                    }
                    else
                    {
                        HasKilledAkama = true;              // Akama is dead or missing, we stop fighting and disappear
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                        m_creature->RemoveAllAuras();
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        EnterEvadeMode();
                    }
                }
            }else ReduceHealthTimer -= diff;

            if (HasKilledAkama)
            {
                if (ResetTimer < diff)
                {
                    EnterEvadeMode();                       // Reset a little while after killing Akama
                }
                else ResetTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    }
};

struct MANGOS_DLL_DECL npc_akamaAI : public ScriptedAI
{
    npc_akamaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        ShadeHasDied = false;
        StartCombat = false;
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 DestructivePoisonTimer;
    uint32 LightningBoltTimer;
    uint32 CheckTimer;
    uint32 CastSoulRetrieveTimer;
    uint32 SoulRetrieveTimer;
    uint32 SummonBrokenTimer;
    uint32 EndingTalkCount;
    uint32 WayPointId;
    uint32 BrokenSummonIndex;

    std::list<uint64> BrokenList;

    bool EventBegun;
    bool ShadeHasDied;
    bool StartCombat;
    bool HasYelledOnce;

    void Reset()
    {
        DestructivePoisonTimer = 15000;
        LightningBoltTimer = 10000;
        CheckTimer = 2000;
        CastSoulRetrieveTimer = 0;
        SoulRetrieveTimer = 0;
        SummonBrokenTimer = 0;
        EndingTalkCount = 0;
        WayPointId = 0;
        BrokenSummonIndex = 0;

        BrokenList.clear();

        EventBegun = false;
        HasYelledOnce = false;

        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void BeginEvent(Player* pPlayer)
    {
        if (!m_pInstance)
            return;

        if (Creature* pShade = (Creature*)Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA)))
        {
            ((boss_shade_of_akamaAI*)pShade->AI())->Reset();

            // Prevent players from trying to restart event
            m_pInstance->SetData(DATA_SHADEOFAKAMAEVENT, IN_PROGRESS);
            m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            ((boss_shade_of_akamaAI*)pShade->AI())->SetSelectableChannelers();

            pShade->AddThreat(m_creature, 1000000.0f);

            pShade->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
            pShade->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());

            if (pPlayer)
                pShade->AddThreat(pPlayer, 0.0f);

            m_creature->SetInCombatWith(pShade);
            pShade->SetInCombatWith(m_creature);

            m_creature->SetInCombatWithZone();

            EventBegun = true;
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || !m_pInstance)
            return;

        switch(id)
        {
            case 0: ++WayPointId; break;
            case 1:
                if (Unit* pShade = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA)))
                {
                    DoCast(pShade, SPELL_AKAMA_SOUL_RETRIEVE);
                    EndingTalkCount = 0;
                    SoulRetrieveTimer = 16000;
                }
                break;
        }
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!EventBegun || !m_pInstance)
            return;

        if (!StartCombat)
        {
            Creature* pShade = (Creature*)Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA));

            if (pShade && pShade->isAlive())
            {
                if (((boss_shade_of_akamaAI*)pShade->AI())->IsBanished)
                {
                    if (CastSoulRetrieveTimer < diff)
                    {
                        DoCast(pShade, SPELL_AKAMA_SOUL_CHANNEL);
                        CastSoulRetrieveTimer = 500;
                    }else CastSoulRetrieveTimer -= diff;
                }
                else
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    StartCombat = true;
                }
            }
        }

        if (ShadeHasDied && (WayPointId == 1))
        {
            m_creature->GetMotionMaster()->MovePoint(WayPointId, AkamaWP[1].x, AkamaWP[1].y, AkamaWP[1].z);
            ++WayPointId;
        }

        if (!ShadeHasDied && StartCombat)
        {
            if (CheckTimer < diff)
            {
                if (Unit* pShade = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA)))
                {
                    if (!pShade->isAlive())
                    {
                        ShadeHasDied = true;
                        WayPointId = 0;
                        m_creature->SetUnitMovementFlags(MOVEMENTFLAG_WALK_MODE);
                        m_creature->GetMotionMaster()->MovePoint(WayPointId, AkamaWP[0].x, AkamaWP[0].y, AkamaWP[0].z);
                    }
                }
                CheckTimer = 5000;
            }else CheckTimer -= diff;
        }

        if (SummonBrokenTimer && BrokenSummonIndex < 4)
        {
            if (SummonBrokenTimer <= diff)
            {
                for(uint8 i = 0; i < 4; ++i)
                {
                    float x = BrokenCoords[BrokenSummonIndex].x + (i*5);
                    float y = BrokenCoords[BrokenSummonIndex].y + (1*5);
                    float z = BrokenCoords[BrokenSummonIndex].z;
                    float o = BrokenCoords[BrokenSummonIndex].o;

                    Creature* Broken = m_creature->SummonCreature(CREATURE_BROKEN, x, y, z, o, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);
                    if (Broken)
                    {
                        float wx = BrokenWP[BrokenSummonIndex].x + (i*5);
                        float wy = BrokenWP[BrokenSummonIndex].y + (i*5);
                        float wz = BrokenWP[BrokenSummonIndex].z;

                        Broken->GetMotionMaster()->MovePoint(0, wx, wy, wz);
                        Broken->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        BrokenList.push_back(Broken->GetGUID());
                    }
                }
                ++BrokenSummonIndex;
                SummonBrokenTimer = 1000;
            }else SummonBrokenTimer -= diff;
        }

        if (SoulRetrieveTimer)
        {
            if (SoulRetrieveTimer <= diff)
            {
                switch(EndingTalkCount)
                {
                    case 0:
                        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                        ++EndingTalkCount;
                        SoulRetrieveTimer = 2000;
                        SummonBrokenTimer = 1;
                        break;
                    case 1:
                        DoScriptText(SAY_FREE, m_creature);
                        ++EndingTalkCount;
                        SoulRetrieveTimer = 25000;
                        break;
                    case 2:
                        if (!BrokenList.empty())
                        {
                            bool Yelled = false;
                            for(std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                                if (Unit* pUnit = Unit::GetUnit(*m_creature, *itr))
                                {
                                    if (!Yelled)
                                    {
                                        DoScriptText(SAY_BROKEN_FREE_01, pUnit);
                                        Yelled = true;
                                    }
                                    pUnit->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                                }
                        }
                        ++EndingTalkCount;
                        SoulRetrieveTimer = 1500;
                        break;
                    case 3:
                        if (!BrokenList.empty())
                        {
                            for(std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                                if (Unit* pUnit = Unit::GetUnit(*m_creature, *itr))
                                    // This is the incorrect spell, but can't seem to find the right one.
                                    pUnit->CastSpell(pUnit, 39656, true);
                        }
                        ++EndingTalkCount;
                        SoulRetrieveTimer = 5000;
                        break;
                    case 4:
                        if (!BrokenList.empty())
                        {
                            for(std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                                if (Unit* pUnit = Unit::GetUnit((*m_creature), *itr))
                                    DoScriptText(SAY_BROKEN_FREE_02, pUnit);
                        }
                        SoulRetrieveTimer = 0;
                        break;
                }
            }else SoulRetrieveTimer -= diff;
        }

        if (!m_creature->getVictim() || !m_creature->SelectHostilTarget())
            return;

        if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 15 && !HasYelledOnce)
        {
            DoScriptText(SAY_LOW_HEALTH, m_creature);
            HasYelledOnce = true;
        }

        if (DestructivePoisonTimer < diff)
        {
            Unit* pShade = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA));

            if (pShade && pShade->isAlive())
                DoCast(pShade, SPELL_DESTRUCTIVE_POISON);

            DestructivePoisonTimer = 15000;
        }else DestructivePoisonTimer -= diff;

        if (LightningBoltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_LIGHTNING_BOLT);
            LightningBoltTimer = 10000;
        }else LightningBoltTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_ashtongue_channelerAI : public ScriptedAI
{
    mob_ashtongue_channelerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void AttackStart(Unit* pWho) {}
    void MoveInLineOfSight(Unit* pWho) {}

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        Creature* pShade = (Creature*)Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA));

        if (pShade && pShade->isAlive())
            ((boss_shade_of_akamaAI*)pShade->AI())->IncrementDeathCount();
        else
            error_log("SD2 ERROR: Channeler dead but unable to increment DeathCount for Shade of Akama.");
    }

    void UpdateAI(const uint32 uiDiff) {}
};

struct MANGOS_DLL_DECL mob_ashtongue_sorcererAI : public ScriptedAI
{
    mob_ashtongue_sorcererAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiCheckTimer;
    bool m_bStartBanishing;

    void Reset()
    {
        m_uiCheckTimer = 5000;
        m_bStartBanishing = false;
    }

    void AttackStart(Unit* pWho) {}
    void MoveInLineOfSight(Unit* pWho) {}

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        Creature* pShade = (Creature*)Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA));

        if (pShade && pShade->isAlive())
            ((boss_shade_of_akamaAI*)pShade->AI())->IncrementDeathCount(m_creature->GetGUID());
        else
            error_log("SD2 ERROR: Sorcerer dead but unable to increment DeathCount for Shade of Akama.");
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bStartBanishing || !m_pInstance)
            return;

        if (m_uiCheckTimer < uiDiff)
        {
            Unit* pShade = Unit::GetUnit(*m_creature, m_pInstance->GetData64(DATA_SHADEOFAKAMA));

            if (pShade && pShade->isAlive() && m_creature->isAlive())
            {
                if (m_creature->IsWithinDist(pShade, 20.0f, false))
                {
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveIdle();

                    DoCast(pShade, SPELL_SHADE_SOUL_CHANNEL, true);

                    m_bStartBanishing = true;
                }
            }
            m_uiCheckTimer = 2000;
        }else m_uiCheckTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_shade_of_akama(Creature* pCreature)
{
    return new boss_shade_of_akamaAI(pCreature);
}

CreatureAI* GetAI_mob_ashtongue_channeler(Creature* pCreature)
{
    return new mob_ashtongue_channelerAI(pCreature);
}

CreatureAI* GetAI_mob_ashtongue_sorcerer(Creature* pCreature)
{
    return new mob_ashtongue_sorcererAI(pCreature);
}

CreatureAI* GetAI_npc_akama_shade(Creature* pCreature)
{
    return new npc_akamaAI(pCreature);
}

bool GossipHello_npc_akama(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->isAlive())
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetGUID());
    }

    return true;
}

bool GossipSelect_npc_akama(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)               //Fight time
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        ((npc_akamaAI*)pCreature->AI())->BeginEvent(pPlayer);
    }

    return true;
}

void AddSC_boss_shade_of_akama()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_shade_of_akama";
    newscript->GetAI = &GetAI_boss_shade_of_akama;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_channeler";
    newscript->GetAI = &GetAI_mob_ashtongue_channeler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ashtongue_sorcerer";
    newscript->GetAI = &GetAI_mob_ashtongue_sorcerer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_akama_shade";
    newscript->GetAI = &GetAI_npc_akama_shade;
    newscript->pGossipHello = &GossipHello_npc_akama;
    newscript->pGossipSelect = &GossipSelect_npc_akama;
    newscript->RegisterSelf();
}

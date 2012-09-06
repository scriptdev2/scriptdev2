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
SDName: Boss_Illidan_Stormrage
SD%Complete: 90
SDComment:
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"
#include "WorldPacket.h"

/**** Creature Summon and Recognition IDs ****/
enum CreatureEntry
{
    EMPTY                   =       0,
    AKAMA                   =   22990,
    ILLIDAN_STORMRAGE       =   22917,
    BLADE_OF_AZZINOTH       =   22996,
    FLAME_OF_AZZINOTH       =   22997,
    MAIEV_SHADOWSONG        =   23197,
    SHADOW_DEMON            =   23375,
    DEMON_FIRE              =   23069,
    FLAME_CRASH             =   23336,
    ILLIDAN_DOOR_TRIGGER    =   23412,
    SPIRIT_OF_OLUM          =   23411,
    SPIRIT_OF_UDALO         =   23410,
    ILLIDARI_ELITE          =   23226,
    PARASITIC_SHADOWFIEND   =   23498,
    CAGE_TRAP_TRIGGER       =   23292,
};

/************* Quotes and Sounds ***********************/
// Gossip for when a player clicks Akama
#define GOSSIP_ITEM          "We are ready to face Illidan"

enum
{
    SAY_CONVO_1             = -1564097,
    SAY_CONVO_2             = -1564098,
    SAY_CONVO_3             = -1564099,
    SAY_CONVO_4             = -1564100,
    SAY_CONVO_5             = -1564101,
    SAY_CONVO_6             = -1564102,
    SAY_CONVO_7             = -1564103,
    SAY_CONVO_8             = -1564104,
    SAY_CONVO_9             = -1564105,
    SAY_CONVO_10            = -1564106,
    SAY_CONVO_11            = -1564107,
    SAY_CONVO_12            = -1564108,
    SAY_CONVO_13            = -1564109,
    SAY_CONVO_14            = -1564110,
    SAY_CONVO_15            = -1564111,

    SAY_TAUNT_1             = -1564112,
    SAY_TAUNT_2             = -1564113,
    SAY_TAUNT_3             = -1564114,
    SAY_TAUNT_4             = -1564115,

    SAY_MAIEV_TAUNT_1       = -1564116,
    SAY_MAIEV_TAUNT_2       = -1564117,
    SAY_MAIEV_TAUNT_3       = -1564118,
    SAY_MAIEV_TAUNT_4       = -1564119,
};

//emote only defined if not related to textId (in database)
struct Yells
{
    int32 textId;
    uint32 creature, timer, emote;
    bool Talk;
};

static const Yells aConversation[]=
{
    {SAY_CONVO_1, ILLIDAN_STORMRAGE, 8000, 0, true},
    {0, ILLIDAN_STORMRAGE, 5000, 396, true},
    {SAY_CONVO_2, AKAMA, 7000, 0, true},
    {0, AKAMA, 5000, 66, true},
    {SAY_CONVO_3, ILLIDAN_STORMRAGE, 8000, 0, true},
    {SAY_CONVO_4, AKAMA, 3000, 0, true},
    {0, AKAMA, 2000, 15, true},
    {SAY_CONVO_5, ILLIDAN_STORMRAGE, 3000, 0, true},
    {0, EMPTY, 1000, 0, true},
    {0, EMPTY, 0, 0, false},
    {SAY_CONVO_6, ILLIDAN_STORMRAGE, 8000, 0, true},
    {SAY_CONVO_7, MAIEV_SHADOWSONG, 8000, 0, true},
    {SAY_CONVO_8, ILLIDAN_STORMRAGE, 7000, 0, true},
    {SAY_CONVO_9, MAIEV_SHADOWSONG, 8000, 0, true},
    {SAY_CONVO_10, ILLIDAN_STORMRAGE, 1000, 0, false},
    {SAY_CONVO_11, MAIEV_SHADOWSONG, 6000, 0, true},
    // Emote dead for now. Kill him later
    {SAY_CONVO_12, ILLIDAN_STORMRAGE, 22000, 0, true},
    {SAY_CONVO_13, MAIEV_SHADOWSONG, 9000, 0, true},
    {SAY_CONVO_14, MAIEV_SHADOWSONG, 3000, 0, true},
    {SAY_CONVO_15, AKAMA, 8000, 0, true},
    {0, EMPTY, 1000, 0, false}
};

static const Yells aRandomTaunts[]=
{
    {SAY_TAUNT_1, ILLIDAN_STORMRAGE, 0, 0, false},
    {SAY_TAUNT_2, ILLIDAN_STORMRAGE, 0, 0, false},
    {SAY_TAUNT_3, ILLIDAN_STORMRAGE, 0, 0, false},
    {SAY_TAUNT_4, ILLIDAN_STORMRAGE, 0, 0, false}
};

static const Yells aMaievTaunts[]=
{
    {SAY_MAIEV_TAUNT_1, MAIEV_SHADOWSONG, 0, 0, false},
    {SAY_MAIEV_TAUNT_2, MAIEV_SHADOWSONG, 0, 0, false},
    {SAY_MAIEV_TAUNT_3, MAIEV_SHADOWSONG, 0, 0, false},
    {SAY_MAIEV_TAUNT_4, MAIEV_SHADOWSONG, 0, 0, false}
};

enum
{
    // Yells for/by Akama
    SAY_AKAMA_BEWARE                 = -1564120,
    SAY_AKAMA_MINION                 = -1564121,
    SAY_AKAMA_LEAVE                  = -1564122,

    // Self explanatory
    SAY_KILL1                        = -1564123,
    SAY_KILL2                        = -1564124,

    // I think I'll fly now and let my subordinates take you on
    SAY_TAKEOFF                      = -1564125,
    SAY_SUMMONFLAMES                 = -1564126,

    // When casting Eye Blast. Demon Fire will be appear on places that he casts this
    SAY_EYE_BLAST                    = -1564127,

    // kk, I go big, dark and demon on you.
    SAY_MORPH                        = -1564128,

    // I KILL!
    SAY_ENRAGE                       = -1564129,

    /************** Spells *************/
    // Normal Form
    SPELL_SHEAR                      = 41032,               // Reduces Max. Health by 60% for 7 seconds. Can stack 19 times. 1.5 second cast
    SPELL_FLAME_CRASH                = 40832,               // Summons an invis/unselect passive mob that has an uiAura of flame in a circle around him.
    SPELL_DRAW_SOUL                  = 40904,               // 5k Shadow Damage in front of him. Heals Illidan for 100k health (script effect)
    SPELL_PARASITIC_SHADOWFIEND      = 41917,               // DoT of 3k Shadow every 2 seconds. Lasts 10 seconds. (Script effect: Summon 2 parasites once the debuff has ticked off)
    SPELL_SUMMON_PARASITICS          = 41915,               // Summons 2 Parasitic Shadowfiends on the target. It's supposed to be cast as soon as the Parasitic Shadowfiend debuff is gone, but the spells aren't linked :(
    SPELL_AGONIZING_FLAMES           = 40932,               // 4k fire damage uiInitial to target and anyone w/i 5 yards. PHASE 3 ONLY
    SPELL_ENRAGE                     = 40683,               // Increases damage by 50% and attack speed by 30%. 20 seconds, PHASE 5 ONLY
    // Flying (Phase 2)
    SPELL_THROW_GLAIVE               = 39635,               // Throws a glaive on the ground
    SPELL_THROW_GLAIVE2              = 39849,               // Animation for the above spell
    SPELL_GLAIVE_RETURNS             = 39873,               // Glaive flies back to Illidan
    SPELL_FIREBALL                   = 40598,               // 2.5k-3.5k damage in 10 yard radius. 2 second cast time.
    SPELL_DARK_BARRAGE               = 40585,               // 10 second channeled spell, 3k shadow damage per second.
    // Demon Form
    SPELL_DEMON_TRANSFORM_1          = 40511,               // First uiPhase of animations for transforming into Dark Illidan (fall to ground)
    SPELL_DEMON_TRANSFORM_2          = 40398,               // Second uiPhase of animations (kneel)
    SPELL_DEMON_TRANSFORM_3          = 40510,               // Final uiPhase of animations (stand up and roar)
    SPELL_DEMON_FORM                 = 40506,               // Transforms into Demon Illidan. Has an Aura of Dread on him.
    SPELL_SHADOW_BLAST               = 41078,               // 8k - 11k Shadow Damage. Targets highest threat. Has a splash effect, damaging anyone in 20 yards of the target.
    SPELL_FLAME_BURST                = 41126,               // Hurls fire at entire raid for ~3.5k damage every 10 seconds. Resistable. (Does not work: Script effect)
    SPELL_FLAME_BURST_EFFECT         = 41131,               // The actual damage. Handled by core (41126 triggers 41131)
    // Other Illidan spells
    SPELL_KNEEL                      = 39656,               // Before beginning encounter, this is how he appears (talking to Wilson).
    SPELL_SHADOW_PRISON              = 40647,               // Illidan casts this spell to immobilize entire raid when he summons Maiev.
    SPELL_DEATH                      = 41220,               // This spell doesn't do anything except stun Illidan and set him on his knees.
    SPELL_BERSERK                    = 45078,               // Damage increased by 500%, attack speed by 150%

    // Non-Illidan spells
    SPELL_AKAMA_DOOR_CHANNEL         = 41268,               // Akama's channel spell on the door before the Temple Summit
    SPELL_DEATHSWORN_DOOR_CHANNEL    = 41269,               // Olum and Udalo's channel spell on the door before the Temple Summit
    SPELL_AKAMA_DOOR_FAIL            = 41271,               // Not sure where this is really used...
    SPELL_HEALING_POTION             = 40535,               // Akama uses this to heal himself to full.
    SPELL_AZZINOTH_CHANNEL           = 39857,               // Glaives cast it on Flames. Not sure if this is the right spell.
    SPELL_SHADOW_DEMON_PASSIVE       = 41079,               // Adds the "shadowform" uiAura to Shadow Demons.
    SPELL_CONSUME_SOUL               = 41080,               // Once the Shadow Demons reach their target, they use this to kill them
    SPELL_PARALYZE                   = 41083,               // Shadow Demons cast this on their target
    SPELL_PURPLE_BEAM                = 39123,               // Purple Beam connecting Shadow Demon to their target
    SPELL_CAGE_TRAP_DUMMY            = 40761,               // Put this in DB for cage trap GO.
    SPELL_EYE_BLAST_TRIGGER          = 40017,               // This summons Demon Form every few seconds and deals ~20k damage in its radius
    SPELL_EYE_BLAST                  = 39908,               // This does the blue flamey animation.
    SPELL_FLAME_CRASH_EFFECT         = 40836,               // Firey blue ring of circle that the other flame crash summons
    SPELL_BLAZE_EFFECT               = 40610,               // Green flame on the ground, triggers damage (5k) every few seconds
    SPELL_BLAZE_SUMMON               = 40637,               // Summons the Blaze creature
    SPELL_DEMON_FIRE                 = 40029,               // Blue fire trail left by Eye Blast. Deals 2k per second if players stand on it.
    SPELL_CAGED                      = 40695,               // Caged Trap triggers will cast this on Illidan if he is within 3 yards
    SPELL_CAGE_TRAP_SUMMON           = 40694,               // Summons a Cage Trap GO (bugged) on the ground along with a Cage Trap Disturb Trigger mob (working)
    SPELL_CAGE_TRAP_BEAM             = 40713,               // 8 Triggers on the ground in an octagon cast spells like this on Illidan 'caging him'
    SPELL_FLAME_BLAST                = 40631,               // Flames of Azzinoth use this. Frontal cone AoE 7k-9k damage.
    SPELL_CHARGE                     = 40602,               // Flames of Azzinoth charges whoever is too far from them. They enrage after this. For simplicity, we'll use the same enrage as Illidan.
    SPELL_TELEPORT_VISUAL            = 41232,               // Teleport visual for Maiev
    SPELL_SHADOWFIEND_PASSIVE        = 41913,               // Passive uiAura for shadowfiends

    EQUIP_ID_MAIN_HAND               = 32837,
    EQUIP_ID_OFF_HAND                = 32838,
};

// Other defines
#define CENTER_X            676.740f
#define CENTER_Y            305.297f
#define CENTER_Z            353.192f

/*** Phase Names ***/
enum Phase
{
    PHASE_NORMAL            =   1,
    PHASE_FLIGHT            =   2,
    PHASE_NORMAL_2          =   3,
    PHASE_DEMON             =   4,
    PHASE_NORMAL_MAIEV      =   5,
    PHASE_DEMON_SEQUENCE    =   6,
    PHASE_NORMAL_FORM       =   7,                          // Internal marker, for switching back to either NORMAL_2 or NORMAL_MAIEV
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static const Locations aGlaivePosition[]=
{
    {695.105f, 305.303f, 354.256f},
    {659.338f, 305.303f, 354.256f},
    {700.105f, 305.303f, 354.256f},
    {664.338f, 305.303f, 354.256f}
};

static const Locations aEyeBlast[]=
{
    {650.697f, 320.128f, 353.730f},
    {652.799f, 275.091f, 353.367f},
    {701.527f, 273.815f, 353.230f},
    {709.865f, 325.654f, 353.322f}
};

static const Locations aAkamaWP[]=
{
    {770.01f, 304.50f, 312.29f},                            // Bottom of the first stairs, at the doors
    {780.66f, 304.50f, 319.74f},                            // Top of the first stairs
    {790.13f, 319.68f, 319.76f},                            // Bottom of the second stairs (left from the entrance)
    {787.17f, 347.38f, 341.42f},                            // Top of the second stairs
    {781.34f, 350.31f, 341.44f},                            // Bottom of the third stairs
    {762.60f, 361.06f, 353.60f},                            // Top of the third stairs
    {756.35f, 360.52f, 353.27f},                            // Before the door-thingy
    {743.82f, 342.21f, 353.00f},                            // Somewhere further
    {732.69f, 305.13f, 353.00f},                            // In front of Illidan
    {738.11f, 365.44f, 353.00f},                            // in front of the door-thingy (the other one!)
    {792.18f, 366.62f, 341.42f},                            // Down the first flight of stairs
    {796.84f, 304.89f, 319.76f},                            // Down the second flight of stairs
    {782.01f, 304.55f, 319.76f}                             // Final location - back at the uiInitial gates. This is where he will fight the minions!
};
// 755.762, 304.0747, 312.1769 -- This is where Akama should be spawned
static const Locations aSpiritSpawns[]=
{
    {755.5426f, 309.9156f, 312.2129f, SPIRIT_OF_UDALO},
    {755.5426f, 298.7923f, 312.0834f, SPIRIT_OF_OLUM}
};

struct WayPoints
{
    WayPoints(uint32 _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
    }
    uint32 id;
    float x, y, z;
};

struct Animation                                            // For the demon transformation
{
    uint32 uiAura, uiUnAura, uiTimer, uiSize, uiDisplayId;
    Phase uiPhase;
    bool bEquip;
};

static Animation DemonTransformation[]=
{
    {SPELL_DEMON_TRANSFORM_1, 0,                        1300, 0,          0,     PHASE_DEMON_SEQUENCE, true},
    {SPELL_DEMON_TRANSFORM_2, SPELL_DEMON_TRANSFORM_1,  4000, 0,          0,     PHASE_DEMON_SEQUENCE, true},
    {SPELL_DEMON_FORM,        0,                        3000, 1073741824, 21322, PHASE_DEMON_SEQUENCE, false},
    {SPELL_DEMON_TRANSFORM_3, SPELL_DEMON_TRANSFORM_2,  3500, 0,          0,     PHASE_DEMON_SEQUENCE, false},
    {0,                       0,                        0,    0,          0,     PHASE_DEMON,          false},
    {SPELL_DEMON_TRANSFORM_1, 0,                        1500, 0,          0,     PHASE_DEMON_SEQUENCE, false},
    {SPELL_DEMON_TRANSFORM_2, SPELL_DEMON_TRANSFORM_1,  4000, 0,          0,     PHASE_DEMON_SEQUENCE, false},
    {0,                       SPELL_DEMON_FORM,         3000, 1069547520, 21135, PHASE_DEMON_SEQUENCE, false},
    {SPELL_DEMON_TRANSFORM_3, SPELL_DEMON_TRANSFORM_2,  3500, 0,          0,     PHASE_DEMON_SEQUENCE, true},
    {0,                       0,                        0,    0,          0,     PHASE_NORMAL_FORM,    true}
};

/**** Demon Fire will be used for Eye Blast. Illidan needs to have access to it's vars and functions, so we'll set it here ****/
struct MANGOS_DLL_DECL demonfireAI : public ScriptedAI
{
    demonfireAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    ObjectGuid m_illidanGuid;

    bool m_bIsTrigger;

    uint32 m_uiCheckTimer;
    uint32 m_uiDemonFireTimer;
    uint32 m_uiDespawnTimer;

    void Reset()
    {
        m_illidanGuid.Clear();

        m_bIsTrigger = false;

        m_uiCheckTimer = 2000;
        m_uiDemonFireTimer = 0;
        m_uiDespawnTimer = 45000;
    }

    void AttackStart(Unit* who) { }
    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsTrigger)
            return;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (m_uiCheckTimer < uiDiff)
        {
            if (!m_illidanGuid && m_pInstance)
            {
                if (Creature* pIllidan = m_pInstance->instance->GetCreature(m_pInstance->GetGuid(NPC_ILLIDAN_STORMRAGE)))
                {
                    m_illidanGuid = m_pInstance->GetGuid(NPC_ILLIDAN_STORMRAGE);

                    if (!pIllidan->IsLevitating())
                        m_creature->SetDeathState(JUST_DIED);
                }
            }
            m_uiCheckTimer = 2000;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_uiDemonFireTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DEMON_FIRE);
            m_uiDemonFireTimer = 30000;
        }
        else
            m_uiDemonFireTimer -= uiDiff;

        if (m_uiDespawnTimer < uiDiff)
            m_creature->SetDeathState(JUST_DIED);
        else
            m_uiDespawnTimer -= uiDiff;
    }
};

/******* Functions and vars for Akama's AI ******/
struct MANGOS_DLL_DECL npc_akama_illidanAI : public ScriptedAI
{
    npc_akama_illidanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        WayPointList.clear();
        Reset();
    }

    /* Instance Data */
    ScriptedInstance* m_pInstance;

    /* Timers */
    uint32 m_uiChannelTimer;
    uint32 m_uiTalkTimer;
    uint32 m_uiWalkTimer;
    uint32 m_uiSummonMinionTimer;

    /* GUIDs */
    ObjectGuid m_illidanGuid;
    ObjectGuid m_playerGuid;
    ObjectGuid m_aSpiritGuids[2];
    ObjectGuid m_channelGuid;

    bool m_bIsTalking;
    bool m_bStartChanneling;
    bool m_bDoorOpen;
    bool m_bFightMinions;
    bool m_bIsReturningToIllidan;
    bool m_bIsWalking;
    uint32 m_uiTalkCount;
    uint32 m_uiChannelCount;

    std::list<WayPoints> WayPointList;
    std::list<WayPoints>::iterator WayPoint;

    void BeginEvent(ObjectGuid playerGuid);

    void Reset()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ILLIDAN, NOT_STARTED);
            GameObject* pGate = m_pInstance->GetSingleGameObjectFromStorage(GO_ILLIDAN_GATE);

            // close door if already open (when raid wipes or something)
            if (pGate && !pGate->GetGoState())
                pGate->SetGoState(GO_STATE_READY);

            for(uint32 i = GO_ILLIDAN_DOOR_R; i <= GO_ILLIDAN_DOOR_L; ++i)
            {
                if (GameObject* pDoor = m_pInstance->GetSingleGameObjectFromStorage(i))
                    pDoor->SetGoState(GO_STATE_ACTIVE);
            }
        }

        m_illidanGuid.Clear();
        m_playerGuid.Clear();
        m_channelGuid.Clear();
        for(uint8 i = 0; i < 2; ++i)
            m_aSpiritGuids[i].Clear();

        m_uiChannelTimer = 0;
        m_uiChannelCount = 0;
        m_uiSummonMinionTimer = 2000;

        m_uiWalkTimer = 0;
        m_bIsWalking = false;

        m_uiTalkTimer = 0;
        m_uiTalkCount = 0;

        KillAllElites();

        m_bIsReturningToIllidan = false;
        m_bFightMinions = false;
        m_bIsTalking = false;
        m_bStartChanneling = false;
        m_bDoorOpen = false;

        // Database sometimes has strange values..
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
    }

    // Do not call reset in Akama's evade mode, as this will stop him from summoning minions after he kills the first bit
    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
    }

    void KillAllElites()
    {
        GuidVector vGuids;
        m_creature->FillGuidsListFromThreatList(vGuids);
        for (GuidVector::const_iterator itr = vGuids.begin(); itr != vGuids.end(); ++itr)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit(*itr);

            if (pUnit && pUnit->GetTypeId() == TYPEID_UNIT && pUnit->GetEntry() == ILLIDARI_ELITE)
                pUnit->SetDeathState(JUST_DIED);
        }
    }

    void ReturnToIllidan()
    {
        KillAllElites();
        m_bFightMinions = false;
        m_bIsReturningToIllidan = true;
        WayPoint = WayPointList.begin();
        m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
        m_creature->SetWalk(false);
        m_bIsWalking = true;
    }

    void AddWaypoint(uint32 id, float x, float y, float z)
    {
        WayPoints AWP(id, x, y, z);
        WayPointList.push_back(AWP);
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth() && (pDealer != m_creature))
        {
            uiDamage = 0;
            DoCastSpellIfCan(m_creature, SPELL_HEALING_POTION);
        }
    }

    void BeginDoorEvent(Player* pPlayer)
    {
        // Requires Instance and this additional check to prevent exploits
        if (!m_pInstance || m_pInstance->GetData(TYPE_COUNCIL) != DONE)
            return;

        debug_log("SD2: Akama - Door event initiated by player %s", pPlayer->GetObjectGuid().GetString().c_str());
        m_playerGuid = pPlayer->GetObjectGuid();

        if (GameObject* pGate = m_pInstance->GetSingleGameObjectFromStorage(GO_ILLIDAN_GATE))
        {
            float x,y,z;
            pGate->GetPosition(x, y, z);
            Creature* Channel = m_creature->SummonCreature(ILLIDAN_DOOR_TRIGGER, x, y, z+5, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);
            if (Channel)
            {
                m_channelGuid = Channel->GetObjectGuid();

                // Invisible but spell visuals can still be seen.
                Channel->SetDisplayId(11686);
                Channel->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                float PosX, PosY, PosZ;
                m_creature->GetPosition(PosX, PosY, PosZ);
                for(uint8 i = 0; i < 2; ++i)
                {
                    Creature* Spirit = m_creature->SummonCreature(aSpiritSpawns[i].id, aSpiritSpawns[i].x, aSpiritSpawns[i].y, aSpiritSpawns[i].z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);
                    if (Spirit)
                    {
                        Spirit->SetVisibility(VISIBILITY_OFF);
                        m_aSpiritGuids[i] = Spirit->GetObjectGuid();
                    }
                }

                m_bStartChanneling = true;
                m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoCastSpellIfCan(Channel, SPELL_AKAMA_DOOR_FAIL);
            }
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || !m_bIsWalking)
            return;

        if (WayPoint->id != id)
            return;

        switch(id)
        {
            case 6:
                if (!m_bIsReturningToIllidan)
                {
                    // open the doors that close the summit
                    for(uint32 i = GO_ILLIDAN_DOOR_R; i < GO_ILLIDAN_DOOR_L+1; ++i)
                    {
                        if (GameObject* pDoor = m_pInstance->GetSingleGameObjectFromStorage(i))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                    }
                }
                break;
            case 7:
                if (m_bIsReturningToIllidan)
                {
                    m_bIsWalking = false;
                    if (m_illidanGuid)
                    {
                        Creature* Illidan = m_creature->GetMap()->GetCreature(m_illidanGuid);
                        if (Illidan)
                        {
                            float dx = Illidan->GetPositionX() + rand()%15;
                            float dy = Illidan->GetPositionY() + rand()%15;
                            m_creature->GetMotionMaster()->MovePoint(13, dx, dy, Illidan->GetPositionZ());
                            m_creature->SetTargetGuid(Illidan->GetObjectGuid());
                        }
                    }
                }
                break;
            case 8:
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (!m_bIsReturningToIllidan)
                {
                    m_bIsWalking = false;
                    BeginEvent(m_playerGuid);
                }
                break;
            case 12:
                m_bIsWalking = false;
                m_bFightMinions = true;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                break;
        }

        ++WayPoint;
        m_uiWalkTimer = 200;
    }

    void DeleteFromThreatList()
    {
        // If we do not have Illidan's GUID, do not proceed
        if (!m_illidanGuid)
            return;

        // Create a pointer to Illidan
        Creature* Illidan = m_creature->GetMap()->GetCreature(m_illidanGuid);

        // No use to continue if Illidan does not exist
        if (!Illidan)
            return;

        ThreatList const& tList = Illidan->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            // Loop through threatlist till our Guid is found in it.
            if ((*itr)->getUnitGuid() == m_creature->GetObjectGuid())
            {
                (*itr)->removeReference();                  // Delete ourself from his threatlist.
                break;                                      // No need to continue anymore.
            }
        }

        // Now we delete our threatlist to prevent attacking anyone for now
        m_creature->DeleteThreatList();
        // Also we remove all auras, to prevent delayed damage
        m_creature->RemoveAllAuras();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_pInstance)
            return;

        if (m_illidanGuid)
        {
            if (Creature* Illidan = m_creature->GetMap()->GetCreature(m_illidanGuid))
            {
                if (Illidan->GetHealthPercent() < 85.0f && m_creature->isInCombat() && !m_bFightMinions)
                {
                    if (m_uiTalkTimer < diff)
                    {
                        switch(m_uiTalkCount)
                        {
                            case 0:
                                DoScriptText(SAY_AKAMA_MINION, Illidan);
                                m_uiTalkTimer = 8000;
                                m_uiTalkCount = 1;
                                break;
                            case 1:
                                DoScriptText(SAY_AKAMA_LEAVE, m_creature);
                                m_uiTalkTimer = 3000;
                                m_uiTalkCount = 2;
                                break;
                            case 2:
                                m_bIsTalking = true;
                                m_uiTalkTimer = 2000;
                                m_creature->RemoveAllAuras();
                                m_creature->CombatStop(true);
                                m_creature->AttackStop();
                                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                m_uiTalkCount = 3;
                                break;
                            case 3:
                                DeleteFromThreatList();
                                m_bIsWalking = true;
                                WayPoint = WayPointList.begin();
                                std::advance(WayPoint, 9);
                                m_creature->SetWalk(false);
                                break;
                        }
                    }
                    else
                        m_uiTalkTimer -= diff;
                }

                if (Illidan->GetHealthPercent() < 4.0f && !m_bIsReturningToIllidan)
                    ReturnToIllidan();
            }
        }
        else
            m_illidanGuid = m_pInstance->GetGuid(NPC_ILLIDAN_STORMRAGE);

        // Reset Encounter
        if (m_pInstance->GetData(TYPE_ILLIDAN) == FAIL)
        {
            m_pInstance->SetData(TYPE_ILLIDAN, NOT_STARTED);

            m_creature->GetMotionMaster()->Clear(false);
            Reset();
            // Get Akama Home
            float fX, fY, fZ, fO;
            m_creature->GetRespawnCoord(fX, fY, fZ, &fO);
            m_creature->NearTeleportTo(fX, fY, fZ, fO);

            return;
        }

        if (m_bIsWalking && m_uiWalkTimer)
        {
            if (m_uiWalkTimer <= diff)
            {
                if (WayPoint == WayPointList.end())
                    return;

                m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y,WayPoint->z);
                m_uiWalkTimer = 0;
            }else m_uiWalkTimer -= diff;
        }

        if (m_bStartChanneling)
        {
            if (m_uiChannelTimer < diff)
            {
                switch(m_uiChannelCount)
                {
                    case 3:
                        if (!m_bDoorOpen)
                        {
                            m_creature->InterruptNonMeleeSpells(true);

                            for(uint8 i = 0; i < 2; ++i)
                            {
                                if (m_aSpiritGuids[i])
                                {
                                    Creature* Spirit = m_creature->GetMap()->GetCreature(m_aSpiritGuids[i]);
                                    if (Spirit)
                                        Spirit->InterruptNonMeleeSpells(true);
                                }
                            }

                            if (GameObject* pGate = m_pInstance->GetSingleGameObjectFromStorage(GO_ILLIDAN_GATE))
                                pGate->SetGoState(GO_STATE_ACTIVE);

                            ++m_uiChannelCount;
                            m_uiChannelTimer = 5000;
                        }
                        break;
                    case 4:
                        m_creature->HandleEmote(EMOTE_ONESHOT_SALUTE);
                        m_uiChannelTimer = 2000;
                        ++m_uiChannelCount;
                        break;
                    case 5:
                        DoScriptText(SAY_AKAMA_BEWARE, m_creature);
                        if (m_channelGuid)
                        {
                            Creature* ChannelTarget = m_creature->GetMap()->GetCreature(m_channelGuid);
                            if (ChannelTarget)
                                ChannelTarget->SetDeathState(JUST_DIED);
                            m_channelGuid.Clear();
                        }
                        for(uint8 i = 0; i < 2; ++i)
                        {
                            if (m_aSpiritGuids[i])
                            {
                                Creature* Spirit = m_creature->GetMap()->GetCreature(m_aSpiritGuids[i]);
                                if (Spirit)
                                    Spirit->SetDeathState(JUST_DIED);
                            }
                        }
                        m_uiChannelTimer = 6000;
                        ++m_uiChannelCount;
                        break;
                    case 6:
                        m_bStartChanneling = false;
                        if (WayPointList.empty())
                        {
                            error_log("SD2: Akama has no waypoints to start with!");
                            return;
                        }

                        WayPoint = WayPointList.begin();
                        m_creature->SetWalk(true);
                        m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y, WayPoint->z);
                        m_bIsWalking = true;
                        break;
                    default:
                        if (m_channelGuid)
                        {
                            Creature* Channel = m_creature->GetMap()->GetCreature(m_channelGuid);
                            if (Channel)
                            {
                                m_creature->InterruptNonMeleeSpells(true);

                                for(uint8 i = 0; i < 2; ++i)
                                {
                                    if (m_aSpiritGuids[i])
                                    {
                                        Creature* Spirit = m_creature->GetMap()->GetCreature(m_aSpiritGuids[i]);
                                        if (Spirit)
                                        {
                                            Spirit->InterruptNonMeleeSpells(true);
                                            if (m_uiChannelCount%2 == 0)
                                            {
                                                Spirit->CastSpell(Channel, SPELL_DEATHSWORN_DOOR_CHANNEL,false);
                                                DoCastSpellIfCan(Channel, SPELL_AKAMA_DOOR_CHANNEL);
                                            }
                                            else
                                            {
                                                if (Spirit->GetVisibility() == VISIBILITY_OFF)
                                                    Spirit->SetVisibility(VISIBILITY_ON);
                                            }
                                        }
                                    }
                                }
                                if (m_uiChannelCount < 3)
                                    ++m_uiChannelCount;
                                m_uiChannelTimer = 10000;
                            }
                        }
                        break;
                }
            }
            else
                m_uiChannelTimer -= diff;
        }

        if (m_bFightMinions)
        {
            if (m_uiSummonMinionTimer < diff)
            {
                float x,y,z;
                m_creature->GetPosition(x,y,z);
                Creature* Elite = m_creature->SummonCreature(ILLIDARI_ELITE, x+rand()%10, y+rand()%10, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 30000);
                if (Elite)
                {
                    Elite->AI()->AttackStart(m_creature);
                    Elite->AddThreat(m_creature, 1000000.0f);
                    AttackStart(Elite);
                }
                m_uiSummonMinionTimer = urand(10000, 16000);
            }
            else
                m_uiSummonMinionTimer -= diff;
        }

        // If we don't have a target, or is talking, or has run away, return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        DoMeleeAttackIfReady();
    }
};

/************************************** Illidan's AI ***************************************/
struct MANGOS_DLL_DECL boss_illidan_stormrageAI : public ScriptedAI
{
    boss_illidan_stormrageAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    /** Instance Data **/
    ScriptedInstance* m_pInstance;

    /** Generic **/
    bool m_bIsTalking;
    bool m_bHasSummoned;
    bool m_bRefaceVictim;
    uint32 m_uiPhase;
    uint32 m_uiGlobalTimer;
    uint32 m_uiTalkCount;
    uint32 m_uiDemonFormSequence;

    /** GUIDs **/
    ObjectGuid m_flameGuids[2];
    ObjectGuid m_glaiveGuids[2];
    ObjectGuid m_akamaGuid;
    ObjectGuid m_maievGuid;

    /** Timers **/
    uint32 m_uiShearTimer;
    uint32 m_uiDrawSoulTimer;
    uint32 m_uiFlameCrashTimer;
    uint32 m_uiParasiticShadowFiendTimer;
    uint32 m_uiFireballTimer;
    uint32 m_uiEyeBlastTimer;
    uint32 m_uiDarkBarrageTimer;
    uint32 m_uiSummonBladesTimer;                           // Animate summoning the Blades of Azzinoth in Phase 2
    uint32 m_uiSummonFlamesTimer;                           // Summon Flames of Azzinoth in Phase 2
    uint32 m_uiCheckFlamesTimer;                            // This is used to check the status of the Flames to see if we should begin entering Phase 3 or not.
    uint32 m_uiRetrieveBladesTimer;                         // Animate retrieving the Blades of Azzinoth in Phase 2 -> 3 transition
    uint32 m_uiLandTimer;                                   // This is used at the end of uiPhase 2 to signal Illidan landing after Flames are dead
    uint32 m_uiAgonizingFlamesTimer;
    uint32 m_uiShadowBlastTimer;
    uint32 m_uiFlameBurstTimer;
    uint32 m_uiShadowDemonTimer;
    uint32 m_uiTalkTimer;
    uint32 m_uiTransformTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiCageTimer;
    uint32 m_uiLayTrapTimer;
    uint32 m_uiAnimationTimer;
    uint32 m_uiTauntTimer;                                  // This is used for his random yells
    uint32 m_uiFaceVictimTimer;
    uint32 m_uiBerserkTimer;

    void Reset()
    {
        m_uiPhase = PHASE_NORMAL;

        m_bRefaceVictim = false;
        m_bHasSummoned = false;

        m_uiFaceVictimTimer = 1000;
        m_uiBerserkTimer = 1500000;
        m_uiGlobalTimer = 0;
        m_uiDemonFormSequence = 0;

        /** Normal Form **/
        m_uiShearTimer = urand(20000, 30000);               // 20 to 30 seconds
        m_uiFlameCrashTimer = 30000;                        // 30 seconds
        m_uiParasiticShadowFiendTimer = 25000;              // 25 seconds
        m_uiDrawSoulTimer = 50000;                          // 50 seconds

        /** Phase 2 **/
        m_uiSummonBladesTimer = 10000;
        m_uiSummonFlamesTimer = 20000;                      // Phase 2 timers may be incorrect
        m_uiFireballTimer = 5000;
        m_uiDarkBarrageTimer = 45000;
        m_uiEyeBlastTimer = 30000;
        m_uiCheckFlamesTimer = 5000;
        m_uiRetrieveBladesTimer = 5000;
        m_uiLandTimer = 0;

        /** Phase 3+ **/
        m_uiAgonizingFlamesTimer = 35000;                   // Phase 3+ timers may be incorrect
        m_uiShadowBlastTimer = 3000;
        m_uiFlameBurstTimer = 10000;
        m_uiShadowDemonTimer = 30000;
        m_uiTransformTimer = 90000;
        m_uiEnrageTimer = 40000;
        m_uiCageTimer = 30000;
        m_uiLayTrapTimer = m_uiCageTimer + 2000;
        m_uiAnimationTimer = 0;

        m_uiTauntTimer = 30000;                             // This timer may be off.

        m_creature->SetDisplayId(21135);
        m_creature->InterruptNonMeleeSpells(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        // Unequip warglaives if needed
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        m_creature->SetLevitate(false);

        m_bIsTalking = false;

        m_uiTalkCount = 0;
        m_uiTalkTimer = 0;

        SetCombatMovement(false);                           // Start idle
    }

    void GetAIInformation(ChatHandler& reader)
    {
        reader.PSendSysMessage("Boss Illidan, current uiPhase = %u, m_uiDemonFormSequence = %u", m_uiPhase, m_uiDemonFormSequence);
        reader.PSendSysMessage("Boolean Vars: m_bIsTalking is %s, m_bHasSummoned is %s, m_bRefaceVictim is %s", m_bIsTalking ? "true" :  "false", m_bHasSummoned ? "true" : "false", m_bRefaceVictim ? "true" : "false");
        reader.PSendSysMessage("Guids: Akama is %s, Maiev is %s", m_akamaGuid.GetString().c_str(), m_maievGuid.GetString().c_str());
    }

    void JustReachedHome()
    {
        // Check if Maiev are alive/existing. Despawn and clear Guid
        if (Creature* Maiev = m_creature->GetMap()->GetCreature(m_maievGuid))
            Maiev->ForcedDespawn();
        m_maievGuid.Clear();

        // Check if any flames/glaives are alive/existing. Kill if alive and clear Guids
        for (uint8 i = 0; i < 2; ++i)
        {
            if (Creature* pFlame = m_creature->GetMap()->GetCreature(m_flameGuids[i]))
            {
                if (pFlame->isAlive())
                    pFlame->SetDeathState(JUST_DIED);

                m_flameGuids[i].Clear();
            }

            if (Creature* pGlaive = m_creature->GetMap()->GetCreature(m_glaiveGuids[i]))
            {
                if (pGlaive->isAlive())
                    pGlaive->SetDeathState(JUST_DIED);

                m_glaiveGuids[i].Clear();
            }
        }

        if (Creature* pAkama = m_creature->GetMap()->GetCreature(m_akamaGuid))
        {
            if (!pAkama->isAlive())
                pAkama->Respawn();

            pAkama->AI()->EnterEvadeMode();
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ILLIDAN, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho || m_creature->getVictim() || m_bIsTalking || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(pWho))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(pWho);
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        m_bIsTalking = false;
        m_uiTalkCount = 0;
        m_uiTalkTimer = 0;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (!m_pInstance)
            return;

        // Completed
        m_pInstance->SetData(TYPE_ILLIDAN, DONE);

        for(uint32 i = GO_ILLIDAN_DOOR_R; i < GO_ILLIDAN_DOOR_L + 1; ++i)
        {
            // Open Doors
            if (GameObject* pDoor = m_pInstance->GetSingleGameObjectFromStorage(i))
                pDoor->SetGoState(GO_STATE_ACTIVE);
        }

    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim == m_creature)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())             // Don't let ourselves be slain before we do our death speech
        {
            uiDamage = 0;
            m_creature->SetHealth(m_creature->GetMaxHealth()/100);
        }
    }

    void Cast(Unit* pVictim, uint32 uiSpellId, bool bTriggered = false)
    {
        if (!pVictim)
            return;

        m_bRefaceVictim = true;
        m_creature->SetTargetGuid(pVictim->GetObjectGuid());
        m_creature->CastSpell(pVictim, uiSpellId, bTriggered);
    }

    /** This will handle the cast of eye blast **/
    void CastEyeBlast()
    {
        m_creature->InterruptNonMeleeSpells(false);

        m_uiDarkBarrageTimer += 10000;

        DoScriptText(SAY_EYE_BLAST, m_creature);

        uint32 uiInitial = urand(0, 3);
        uint32 uiFinal = 0;

        if (uiInitial < 3)
            uiFinal = uiInitial+1;

        float initial_X = aEyeBlast[uiInitial].x;
        float initial_Y = aEyeBlast[uiInitial].y;
        float initial_Z = aEyeBlast[uiInitial].z;

        float final_X = aEyeBlast[uiFinal].x;
        float final_Y = aEyeBlast[uiFinal].y;
        float final_Z = aEyeBlast[uiFinal].z;

        for(uint8 i = 0; i < 2; ++i)
        {
            if (Creature* pTrigger = m_creature->SummonCreature(DEMON_FIRE, initial_X, initial_Y, initial_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
            {
                if (demonfireAI* pTriggerAI = dynamic_cast<demonfireAI*>(pTrigger->AI()))
                    pTriggerAI->m_bIsTrigger = true;

                pTrigger->GetMotionMaster()->MovePoint(0, final_X, final_Y, final_Z);

                if (!i)
                    pTrigger->CastSpell(pTrigger, SPELL_EYE_BLAST_TRIGGER, true);
                else
                {
                    pTrigger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->SetTargetGuid(pTrigger->GetObjectGuid());
                    DoCastSpellIfCan(pTrigger, SPELL_EYE_BLAST);
                }
            }
        }
    }

    // It's only cast on players that are greater than 15 yards away from Illidan.
    //If no one is found, cast it on MT instead (since selecting someone in that 15 yard radius would cause the flames to hit the MT anyway).
    void CastAgonizingFlames()
    {
        // We'll use a grid searcher that selects a player that is at a distance >15 yards
        if (Player* pPlayer = GetPlayerAtMinimumRange(15.0f))
            DoCastSpellIfCan(pPlayer, SPELL_AGONIZING_FLAMES);
        else
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_AGONIZING_FLAMES);
    }

    void Talk(uint32 count)
    {
        if (!m_creature->isAlive())
            return;

        int32 text = 0;

        if (aConversation[count].textId)
            text = aConversation[count].textId;

        m_uiTalkTimer = aConversation[count].timer;
        uint32 emote = aConversation[count].emote;
        m_bIsTalking = aConversation[count].Talk;
        Creature* pCreature = NULL;
        ObjectGuid Guid;

        if (aConversation[count].creature == ILLIDAN_STORMRAGE)
            pCreature = m_creature;
        else if (aConversation[count].creature == AKAMA)
        {
            if (!m_akamaGuid)
            {
                if (m_pInstance)
                {
                    m_akamaGuid = m_pInstance->GetGuid(NPC_AKAMA);
                    if (!m_akamaGuid)
                        return;
                    Guid = m_akamaGuid;
                }
            }
            else Guid = m_akamaGuid;
        }
        else if (aConversation[count].creature == MAIEV_SHADOWSONG)
        {
            if (!m_maievGuid)
                return;
            Guid = m_maievGuid;
        }
        else if (aConversation[count].creature == EMPTY)     // This is just for special cases without speech/sounds/emotes.
            return;

        if (Guid)                                           // Now we check if we actually specified a GUID, if so:
                                                            // we grab a pointer to that creature
            pCreature = m_creature->GetMap()->GetCreature(Guid);

        if (pCreature)
        {
            if (emote)
                pCreature->HandleEmote(emote);              // Make the creature do some animation
            if (text)
                DoScriptText(text, pCreature);              // Have the creature yell out some text
        }
    }

    void Move(float X, float Y, float Z, Creature* pCreature)
    {
        pCreature->GetMotionMaster()->MovePoint(0, X, Y, Z);
    }

    void HandleDemonTransformAnimation(uint32 uiSequenceCount)
    {
        m_uiAnimationTimer = DemonTransformation[uiSequenceCount].uiTimer;

        m_creature->InterruptNonMeleeSpells(false);

        if (DemonTransformation[uiSequenceCount].uiPhase != PHASE_NORMAL_FORM)
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
        }

        if (DemonTransformation[uiSequenceCount].uiUnAura)
            m_creature->RemoveAurasDueToSpell(DemonTransformation[uiSequenceCount].uiUnAura);

        if (DemonTransformation[uiSequenceCount].uiAura)
            DoCastSpellIfCan(m_creature, DemonTransformation[uiSequenceCount].uiAura, CAST_TRIGGERED);

        if (DemonTransformation[uiSequenceCount].uiDisplayId)
            // It's morphin time!
            m_creature->SetDisplayId(DemonTransformation[uiSequenceCount].uiDisplayId);
        /*if (DemonTransformation[uiSequenceCount].uiSize)
            m_creature->SetUInt32Value(OBJECT_FIELD_SCALE_X, DemonTransformation[uiSequenceCount].uiSize); // Let us grow! (or shrink)*/

        if (DemonTransformation[uiSequenceCount].bEquip)
        {
            // Requip warglaives if needed
            SetEquipmentSlots(false, EQUIP_ID_MAIN_HAND, EQUIP_ID_OFF_HAND, EQUIP_NO_CHANGE);
        }
        else
        {
            // Unequip warglaives if needed
            SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);
        }

        if (DemonTransformation[uiSequenceCount].uiPhase != PHASE_NORMAL_FORM)
            m_uiPhase = DemonTransformation[uiSequenceCount].uiPhase;       // Set uiPhase properly
        else
        {
            // Refollow and attack our old victim
            SetCombatMovement(true);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

            // Depending on whether we summoned Maiev, we switch to either uiPhase 5 or 3
            if (m_maievGuid)
                m_uiPhase = PHASE_NORMAL_MAIEV;
            else
                m_uiPhase = PHASE_NORMAL_2;
        }

        if (uiSequenceCount == 7)
        {
            DoResetThreat();
            m_creature->RemoveAurasDueToSpell(SPELL_DEMON_FORM);
        }
        else if (uiSequenceCount == 4)
        {
            DoResetThreat();
            if (!m_creature->HasAura(SPELL_DEMON_FORM, EFFECT_INDEX_0))
                DoCastSpellIfCan(m_creature, SPELL_DEMON_FORM, CAST_TRIGGERED);
        }
    }

    /** To reduce the amount of code in UpdateAI, we can seperate them into different functions and simply call them from UpdateAI **/
    void EnterPhase2()
    {
        DoScriptText(SAY_TAKEOFF, m_creature);

        m_uiSummonBladesTimer = 10000;                          // Summon Glaives when this decrements
        m_uiSummonFlamesTimer = 20000;                          // Summon Flames when this decrements
        m_uiGlobalTimer += 20000;
        m_uiLandTimer = 0;
        m_uiPhase = PHASE_FLIGHT;
        m_creature->RemoveAllAuras();
        m_creature->SetTargetGuid(ObjectGuid());

        // So players don't shoot us down
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        // We now hover!
        m_creature->SetLevitate(true);
        SetCombatMovement(false);
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
        for(uint8 i = 0; i < 2; ++i)
        {
            Creature* Glaive = m_creature->SummonCreature(BLADE_OF_AZZINOTH, aGlaivePosition[i].x, aGlaivePosition[i].y, aGlaivePosition[i].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            if (Glaive)
            {
                m_glaiveGuids[i] = Glaive->GetObjectGuid();    // We need this to remove them later on
                Glaive->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Glaive->SetVisibility(VISIBILITY_OFF);
                Glaive->setFaction(m_creature->getFaction());
            }
        }
    }

    void SummonBladesOfAzzinoth()
    {
        m_creature->GetMotionMaster()->Clear(false);

        m_uiLandTimer = 0;
        m_uiRetrieveBladesTimer = 0;

        // Make it look like we're throwing the glaives on the ground
        DoCastSpellIfCan(m_creature, SPELL_THROW_GLAIVE2);

        // We no longer wear the glaives!
        // since they are now channeling the flames (or will be)
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        for(uint8 i = 0; i < 2; ++i)
        {
            Creature* Glaive = NULL;
            Glaive = m_creature->GetMap()->GetCreature(m_glaiveGuids[i]);
            if (Glaive)
            {
                DoCastSpellIfCan(Glaive, SPELL_THROW_GLAIVE, CAST_TRIGGERED);
                Glaive->SetVisibility(VISIBILITY_ON);
            }
        }
    }

    void SummonFlamesOfAzzinoth()
    {
        DoScriptText(SAY_SUMMONFLAMES, m_creature);

        for(uint8 i = 0; i < 2; ++i)
        {
            Creature* Flame = NULL;
            Creature* Glaive = NULL;
            Glaive = m_creature->GetMap()->GetCreature(m_glaiveGuids[i]);
            if (Glaive)
            {
                Flame = m_creature->SummonCreature(FLAME_OF_AZZINOTH, aGlaivePosition[i+2].x, aGlaivePosition[i+2].y, aGlaivePosition[i+2].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                if (Flame)
                {
                    // Just in case the database has it as a different faction
                    Flame->setFaction(m_creature->getFaction());

                    // Attack our target!
                    Flame->AI()->AttackStart(m_creature->getVictim());

                    // Record GUID in order to check if they're dead later on to move to the next uiPhase
                    m_flameGuids[i] = Flame->GetObjectGuid();

                    // Glaives do some random Beam type channel on it.
                    Glaive->CastSpell(Flame, SPELL_AZZINOTH_CHANNEL, true);

                    if (m_creature->getVictim())
                        Flame->AI()->AttackStart(m_creature->getVictim());
                }
                else
                {
                    error_log("SD2: Illidan Stormrage AI: Unable to summon Flame of Azzinoth (entry: 22997), please check your database");
                    EnterEvadeMode();
                }
            }
            else
            {
                error_log("SD2: Illidan Stormrage AI: Unable to summon Blade of Azzinoth (entry: 22996), please check your database");
            }
        }
        DoResetThreat();                                    // And now reset our threatlist
        m_bHasSummoned = true;
    }

    void SummonMaiev()
    {
        m_uiTauntTimer += 4000;
        m_uiGlobalTimer += 4000;

        m_creature->InterruptNonMeleeSpells(false);         // Interrupt any of our spells
        Creature* Maiev = NULL;                             // Summon Maiev near Illidan
        Maiev = m_creature->SummonCreature(MAIEV_SHADOWSONG, m_creature->GetPositionX() + 10, m_creature->GetPositionY() + 5, m_creature->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
        if (Maiev)
        {
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->Clear(false);    // Stop moving, it's rude to walk and talk!
            m_creature->GetMotionMaster()->MoveIdle();
                                                            // Just in case someone is unaffected by Shadow Prison
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoCastSpellIfCan(m_creature, SPELL_SHADOW_PRISON, CAST_TRIGGERED);
            m_uiTalkCount = 10;
            m_bIsTalking = true;                               // We are now talking/
            Maiev->SetVisibility(VISIBILITY_OFF);           // Leave her invisible until she has to talk
            Maiev->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_maievGuid = Maiev->GetObjectGuid();
        }
        else                                                // If Maiev cannot be summoned, reset the encounter and post some errors to the console.
        {
            EnterEvadeMode();
            debug_log("SD2: Unable to summon Maiev Shadowsong and enter Phase 4. Resetting Encounter.");
            error_log("SD2: Unable to summon Maiev Shadowsong (entry: 23197). Check your database to see if you have the proper SQL for Maiev Shadowsong (entry: 23197)");
        }
    }

    void InitializeDeath()
    {
        m_creature->RemoveAllAuras();
        DoCastSpellIfCan(m_creature, SPELL_DEATH);          // Animate his kneeling + stun him
                                                            // Don't let the players interrupt our talk!
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        m_creature->GetMotionMaster()->Clear(false);        // No moving!
        m_creature->GetMotionMaster()->MoveIdle();

        if (m_maievGuid)
        {
            if (Creature* Maiev = m_creature->GetMap()->GetCreature(m_maievGuid))
            {
                Maiev->CombatStop(true);                    // Maiev shouldn't do anything either. No point in her attacking us =]
                Maiev->GetMotionMaster()->Clear(false);     // Stop her from moving as well
                Maiev->GetMotionMaster()->MoveIdle();

                float distance = 10.0f;
                float dx = m_creature->GetPositionX() + (distance*cos(m_creature->GetOrientation()));
                float dy = m_creature->GetPositionY() + (distance*sin(m_creature->GetOrientation()));

                Maiev->NearTeleportTo(dx, dy, Maiev->GetPositionZ(), 0.0f);

                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                Maiev->SetTargetGuid(m_creature->GetObjectGuid());
            }
        }
        m_bIsTalking = true;
        ++m_uiTalkCount;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        /*** This section will handle the conversations ***/
        if (m_bIsTalking)                                      // Somewhat more efficient using a function rather than a long switch
        {
            if (m_uiTalkTimer < uiDiff)
            {
                switch(m_uiTalkCount)                           // This is only for specialized cases
                {
                    case 0:
                        // Time to stand up!
                        m_creature->RemoveAurasDueToSpell(SPELL_KNEEL);
                        break;
                    case 8:
                        // Equip our warglaives!
                        SetEquipmentSlots(false, EQUIP_ID_MAIN_HAND, EQUIP_ID_OFF_HAND, EQUIP_NO_CHANGE);
                        // Hostile if we weren't before
                        m_creature->setFaction(14);
                        break;
                    case 9:
                        SetCombatMovement(true);
                        if (m_akamaGuid)
                        {
                            if (Creature* pAkama = m_creature->GetMap()->GetCreature(m_akamaGuid))
                            {
                                // Start attacking Akama
                                AttackStart(pAkama);

                                // Akama stop talk and start attack illidan
                                if (npc_akama_illidanAI* pAkamaAI = dynamic_cast<npc_akama_illidanAI*>(pAkama->AI()))
                                    pAkamaAI->m_bIsTalking = false;

                                pAkama->AI()->AttackStart(m_creature);
                                pAkama->AddThreat(m_creature, 1000000.0f);
                            }
                        }

                        // We are now attackable!
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        debug_log("SD2: Black Temple: Illidan intro complete, players can attack Illidan.");
                        break;
                    case 11:
                        if (m_maievGuid)
                        {
                            Creature* Maiev = m_creature->GetMap()->GetCreature(m_maievGuid);
                            if (Maiev)
                            {
                                // Maiev is now visible
                                Maiev->SetVisibility(VISIBILITY_ON);
                                // onoz she looks like she teleported!
                                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                                // Have her face us
                                Maiev->SetTargetGuid(m_creature->GetObjectGuid());
                                // Face her, so it's not rude =P
                                m_creature->SetTargetGuid(Maiev->GetObjectGuid());
                            }
                        }
                        break;
                    case 14:
                        if (Creature* pMaiev = m_creature->GetMap()->GetCreature(m_maievGuid))
                        {
                            pMaiev->GetMotionMaster()->Clear(false);
                            pMaiev->GetMotionMaster()->MoveChase(m_creature);
                            // Have Maiev add a lot of threat on us so that players don't pull her off if they damage her via AOE
                            pMaiev->AddThreat(m_creature, 10000000.0f);
                            // Force Maiev to attack us.
                            pMaiev->AI()->AttackStart(m_creature);
                            pMaiev->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        }
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        SetCombatMovement(true);
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_bIsTalking = false;
                        m_uiFaceVictimTimer = 2000;
                        m_bRefaceVictim = true;
                        break;
                    case 20:
                        // Kill ourself.
                        if (Creature* Maiev = m_creature->GetMap()->GetCreature(m_maievGuid))
                        {
                            // Make Maiev leave
                            Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                            Maiev->SetDeathState(JUST_DIED);
                        }
                        m_bIsTalking = false;
                        if (m_creature->getVictim())
                            m_creature->getVictim()->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        else
                            // Now we kill ourself
                            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        break;
                }

                // This function does most of the talking
                Talk(m_uiTalkCount);
                ++m_uiTalkCount;
            }
            else
                m_uiTalkTimer -= uiDiff;

            // No further action while talking
            return;
        }

        // If we don't have a target, return.
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // If we are 'caged', then we shouldn't do anything such as cast spells or transform into Demon Form.
        if (m_creature->HasAura(SPELL_CAGED, EFFECT_INDEX_0))
        {
            // Just so that he doesn't immediately enrage after he stops being caged.
            m_uiEnrageTimer = 40000;
            m_uiCageTimer = 30000;
            return;
        }

        // Berserk Timer - flat 25 minutes
        if (m_uiPhase != PHASE_DEMON_SEQUENCE && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                DoScriptText(SAY_ENRAGE, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED);
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        if (m_bRefaceVictim)
        {
            if (m_uiFaceVictimTimer < uiDiff)
            {
                m_creature->SetTargetGuid(m_creature->getVictim()->GetObjectGuid());
                m_uiFaceVictimTimer = 1000;
                m_bRefaceVictim = false;
            }
            else
                m_uiFaceVictimTimer -= uiDiff;
        }

        /** Signal to change to uiPhase 2 **/
        if (m_uiPhase == PHASE_NORMAL && m_creature->GetHealthPercent() < 65.0f)
        {
            EnterPhase2();
            return;
        }

        /** Signal to summon Maiev at 30% health**/
        if (!m_maievGuid && !(m_uiPhase == PHASE_DEMON || m_uiPhase == PHASE_DEMON_SEQUENCE) && m_creature->GetHealthPercent() < 30.0f)
        {
            SummonMaiev();
            return;
        }

        /** Time for the death speech **/
        if (!(m_uiPhase == PHASE_DEMON || m_uiPhase == PHASE_DEMON_SEQUENCE) && m_creature->GetHealthPercent() < 1.0f)
        {
            InitializeDeath();
            return;
        }

        /***** Spells for Phase 1, 3 and 5 (Normal Form) ******/
        if (m_uiPhase == PHASE_NORMAL || m_uiPhase == PHASE_NORMAL_2 || m_uiPhase == PHASE_NORMAL_MAIEV)
        {
            if (m_uiTauntTimer < uiDiff)                           // His random taunt/yell uiTimer.
            {
                uint32 random = urand(0, 3);
                int32 yell = aRandomTaunts[random].textId;
                if (yell)
                    DoScriptText(yell, m_creature);
                m_uiTauntTimer = 32000;
            }
            else
                m_uiTauntTimer -= uiDiff;

            // Global Timer so that spells do not overlap.
            if (m_uiGlobalTimer < uiDiff)
            {
                if (m_uiShearTimer < uiDiff)
                {
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHEAR);
                    m_uiShearTimer = urand(25000, 40000);
                    m_uiGlobalTimer += 2000;
                }
                else
                    m_uiShearTimer -= uiDiff;

                if (m_uiFlameCrashTimer < uiDiff)
                {
                    //It spawns multiple flames sometimes. Therefore, we'll do this manually.
                    //DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_CRASH);
                    m_creature->SummonCreature(FLAME_CRASH, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                    m_uiFlameCrashTimer = 35000;
                    m_uiGlobalTimer += 2000;
                }
                else
                    m_uiFlameCrashTimer -= uiDiff;

                if (m_uiParasiticShadowFiendTimer < uiDiff)
                {
                    Unit* target = NULL;
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1);
                    if (target && target->isAlive() && !target->HasAura(SPELL_PARASITIC_SHADOWFIEND, EFFECT_INDEX_0))
                    {
                        Cast(target, SPELL_PARASITIC_SHADOWFIEND);
                        m_uiParasiticShadowFiendTimer = 40000;
                    }
                }
                else
                    m_uiParasiticShadowFiendTimer -= uiDiff;

                if (m_uiDrawSoulTimer < uiDiff)
                {
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_DRAW_SOUL);
                    m_uiDrawSoulTimer = 55000;
                    m_uiGlobalTimer += 3000;
                }
                else
                    m_uiDrawSoulTimer -= uiDiff;
            }
            else
                m_uiGlobalTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

        /*** Phase 2 ***/
        if (m_uiPhase == PHASE_FLIGHT)
        {
            // Check if we have summoned or not.
            if (!m_bHasSummoned)
            {
                if (m_uiSummonBladesTimer)
                {
                    if (m_uiSummonBladesTimer <= uiDiff)
                    {
                        SummonBladesOfAzzinoth();
                        m_uiSummonBladesTimer = 0;
                    }
                    else
                        m_uiSummonBladesTimer -= uiDiff;
                }

                if (m_uiSummonFlamesTimer < uiDiff)
                {
                    SummonFlamesOfAzzinoth();
                }
                else
                    m_uiSummonFlamesTimer -= uiDiff;
            }

            if (!m_creature->GetMotionMaster()->empty() && (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE))
                m_creature->GetMotionMaster()->Clear(false);

            if (m_bHasSummoned)
            {
                if (m_uiCheckFlamesTimer)
                {
                    if (m_uiCheckFlamesTimer <= uiDiff)
                    {
                        // Check if flames are dead or non-existant. If so, set GUID to 0.
                        for(uint8 i = 0; i < 2; ++i)
                        {
                            if (m_flameGuids[i])
                            {
                                Creature* pFlame = m_creature->GetMap()->GetCreature(m_flameGuids[i]);

                                // If the flame dies, or somehow the pointer becomes invalid, reset Guid.
                                if (!pFlame || !pFlame->isAlive())
                                    m_flameGuids[i].Clear();
                            }
                        }
                        m_uiCheckFlamesTimer = 500;
                    }
                    else
                        m_uiCheckFlamesTimer -= uiDiff;
                }

                // If both flames are dead/non-existant, kill glaives and change to uiPhase 3.
                if (!m_flameGuids[0] && !m_flameGuids[1] && m_uiCheckFlamesTimer)
                {
                    m_uiRetrieveBladesTimer = 5000;             // Prepare for re-equipin!
                    m_uiCheckFlamesTimer = 0;
                }

                if (m_uiRetrieveBladesTimer)
                {
                    if (m_uiRetrieveBladesTimer <= uiDiff)         // Time to get back our glaives!
                    {
                                                            // Interrupt any spells we might be doing *cough* DArk Barrage *cough*
                        m_creature->InterruptNonMeleeSpells(false);
                        for(uint8 i = 0; i < 2; ++i)
                        {
                            if (Creature* pGlaive = m_creature->GetMap()->GetCreature(m_glaiveGuids[i]))
                            {
                                // Make it look like the Glaive flies back up to us
                                pGlaive->CastSpell(m_creature, SPELL_GLAIVE_RETURNS, true);
                                // Despawn the Glaive
                                pGlaive->SetDeathState(JUST_DIED);
                                m_glaiveGuids[i].Clear();
                            }
                        }

                        // Re-bEquip our warblades!
                        SetEquipmentSlots(false, EQUIP_ID_MAIN_HAND, EQUIP_ID_OFF_HAND, EQUIP_NO_CHANGE);

                        // Prepare for landin'!
                        m_uiLandTimer = 5000;
                        m_uiRetrieveBladesTimer = 0;
                    }
                    else
                        m_uiRetrieveBladesTimer -= uiDiff;
                }

                if (m_uiLandTimer)
                {
                    // Time to land!
                    if (m_uiLandTimer <= uiDiff)
                    {
                        DoResetThreat();

                        // anndddd touchdown!
                        m_creature->HandleEmote(EMOTE_ONESHOT_LAND);
                        m_creature->SetLevitate(false);
                        m_uiPhase = PHASE_NORMAL_2;

                        // We should let the raid fight us =)
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->SetTargetGuid(m_creature->getVictim()->GetObjectGuid());

                        // Chase our victim!
                        SetCombatMovement(true);
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    }
                    else
                        m_uiLandTimer -= uiDiff;

                    return;                                 // Do not continue past this point if m_uiLandTimer is not 0 and we are in uiPhase 2.
                }
            }

            if (m_uiGlobalTimer < uiDiff)
            {
                if (m_uiFireballTimer < uiDiff)
                {
                    Cast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_FIREBALL);
                    m_uiFireballTimer = 5000;
                }
                else
                    m_uiFireballTimer -= uiDiff;

                if (m_uiDarkBarrageTimer < uiDiff)
                {
                    m_creature->InterruptNonMeleeSpells(false);

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_DARK_BARRAGE);

                    m_uiDarkBarrageTimer = 35000;
                    m_uiGlobalTimer += 9000;
                }
                else
                    m_uiDarkBarrageTimer -= uiDiff;

                if (m_uiEyeBlastTimer < uiDiff)
                {
                    CastEyeBlast();
                    m_uiEyeBlastTimer = 30000;
                }
                else
                    m_uiEyeBlastTimer -= uiDiff;
            }
            else
                m_uiGlobalTimer -= uiDiff;
        }

        /** Phase 3,5 spells only**/
        if (m_uiPhase == PHASE_NORMAL_2 || m_uiPhase == PHASE_NORMAL_MAIEV)
        {
            if (m_uiGlobalTimer < uiDiff)
            {
                if (m_uiAgonizingFlamesTimer < uiDiff)
                {
                    CastAgonizingFlames();
                    m_uiAgonizingFlamesTimer = 60000;
                }
                else
                    m_uiAgonizingFlamesTimer -= uiDiff;
            }
            else
                m_uiGlobalTimer -= uiDiff;

            if (m_uiTransformTimer < uiDiff)
            {
                float CurHealth = m_creature->GetHealthPercent();
                // Prevent Illidan from morphing if less than 32% or 5%, as this may cause issues with the uiPhase transition or death speech
                if ((CurHealth < 32.0f && !m_maievGuid) || CurHealth < 5.0f)
                    return;

                m_uiPhase = PHASE_DEMON_SEQUENCE;           // Transform sequence
                m_uiDemonFormSequence = 0;
                m_uiAnimationTimer = 0;

                DoScriptText(SAY_MORPH, m_creature);

                m_uiTransformTimer = 60000;
                m_uiFlameBurstTimer = 10000;
                m_uiShadowDemonTimer = 30000;
                SetCombatMovement(false);
                m_creature->GetMotionMaster()->Clear(false);// Stop moving
            }
            else
                m_uiTransformTimer -= uiDiff;
        }

        /** Phase 4 spells only (Demon Form) **/
        if (m_uiPhase == PHASE_DEMON)
        {
            // Stop moving if we are by clearing movement generators.
            if (!m_creature->GetMotionMaster()->empty())
                m_creature->GetMotionMaster()->Clear(false);

            if (m_uiTransformTimer < uiDiff)
            {
                m_uiPhase = PHASE_DEMON_SEQUENCE;
                m_uiDemonFormSequence = 5;
                m_uiAnimationTimer = 100;
                m_uiTransformTimer = 60000;
            }
            else
                m_uiTransformTimer -= uiDiff;

            if (m_uiShadowDemonTimer < uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                for (uint8 i = 0; i < 4; ++i)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER))
                    {
                        if (Creature* ShadowDemon = m_creature->SummonCreature(SHADOW_DEMON, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000))
                        {
                            ShadowDemon->AddThreat(pTarget, 5000000.0f);
                            ShadowDemon->AI()->AttackStart(pTarget);
                            ShadowDemon->SetInCombatWithZone();
                        }
                    }
                }
                m_uiShadowDemonTimer = 60000;
            }
            else
                m_uiShadowDemonTimer -= uiDiff;

            if (m_uiGlobalTimer < uiDiff)
            {
                if (m_uiShadowBlastTimer < uiDiff)
                {
                    Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0);
                    if (target && target->isAlive())
                    {
                        m_creature->SetTargetGuid(target->GetObjectGuid());
                        DoCastSpellIfCan(target, SPELL_SHADOW_BLAST);
                        m_uiShadowBlastTimer = 4000;
                        m_uiGlobalTimer += 1500;
                    }
                    if (!m_creature->HasAura(SPELL_DEMON_FORM, EFFECT_INDEX_0))
                        DoCastSpellIfCan(m_creature, SPELL_DEMON_FORM, CAST_TRIGGERED);
                }
                else
                    m_uiShadowBlastTimer -= uiDiff;

                if (m_uiFlameBurstTimer < uiDiff)
                {
                    DoCastSpellIfCan(m_creature, SPELL_FLAME_BURST);
                    m_uiFlameBurstTimer = 15000;
                }
                else
                    m_uiFlameBurstTimer -= uiDiff;
            }
            else
                m_uiGlobalTimer -= uiDiff;
        }

        /** Phase 5 timers. Enrage spell **/
        if (m_uiPhase == PHASE_NORMAL_MAIEV)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                m_uiEnrageTimer = 40000;
                m_uiCageTimer = 30000;
                m_uiTransformTimer += 10000;
            }
            else
                m_uiEnrageTimer -= uiDiff;

            // We'll handle Cage Trap in Illidan's script for simplicity's sake
            if (m_uiCageTimer < uiDiff)
            {
                if (m_maievGuid)
                {
                    Creature* Maiev = m_creature->GetMap()->GetCreature(m_maievGuid);
                    Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                    if (!Maiev || !target || (target->GetTypeId() != TYPEID_PLAYER))
                        return;

                    float X, Y, Z;
                    target->GetPosition(X, Y, Z);
                    Maiev->GetMap()->CreatureRelocation(m_creature, X, Y, Z, Maiev->GetOrientation());

                    // Make it look like she 'teleported'
                    Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                    // summon the trap!
                    Maiev->CastSpell(Maiev, SPELL_CAGE_TRAP_SUMMON, false);
                }
                m_uiCageTimer = 15000;
            }
            else
                m_uiCageTimer -= uiDiff;
        }

        if (m_uiPhase == PHASE_DEMON_SEQUENCE)                   // Demonic Transformation
        {
            if (m_uiAnimationTimer < uiDiff)
            {
                HandleDemonTransformAnimation(m_uiDemonFormSequence);
                ++m_uiDemonFormSequence;
            }
            else
                m_uiAnimationTimer -= uiDiff;
        }
    }
};

/*********************** End of Illidan AI ******************************************/

void npc_akama_illidanAI::BeginEvent(ObjectGuid playerGuid)
{
    debug_log("SD2: Akama - Illidan Introduction started. Illidan event properly begun.");
    if (m_pInstance)
    {
        m_illidanGuid = m_pInstance->GetGuid(NPC_ILLIDAN_STORMRAGE);
        m_pInstance->SetData(TYPE_ILLIDAN, IN_PROGRESS);
    }

    if (m_pInstance)
    {
        m_pInstance->DoUseDoorOrButton(GO_ILLIDAN_DOOR_R);
        m_pInstance->DoUseDoorOrButton(GO_ILLIDAN_DOOR_L);
    }

    if (m_illidanGuid)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (Creature* pIllidan = m_creature->GetMap()->GetCreature(m_illidanGuid))
        {
            boss_illidan_stormrageAI* pIllidanAI = dynamic_cast<boss_illidan_stormrageAI*>(pIllidan->AI());

            if (!pIllidanAI)
                return;

            // Time for Illidan to stand up.
            pIllidan->RemoveAurasDueToSpell(SPELL_KNEEL);

            // First line of Akama-Illidan convo

            pIllidanAI->m_uiTalkCount = 0;

            // Begin Talking
            pIllidanAI->m_bIsTalking = true;
            pIllidanAI->m_akamaGuid = m_creature->GetObjectGuid();

            m_creature->SetTargetGuid(pIllidan->GetObjectGuid());
            pIllidan->SetTargetGuid(m_creature->GetObjectGuid());

            m_bIsTalking = true;                               // Prevent Akama from starting to attack him
                                                            // Prevent players from talking again

            m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            pIllidan->GetMotionMaster()->Clear(false);
            pIllidan->GetMotionMaster()->MoveIdle();

            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();

            if (playerGuid)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(playerGuid))
                    pIllidan->AddThreat(pPlayer, 100.0f);
            }
        }
    }
}

bool GossipHello_npc_akama_at_illidan(Player* pPlayer, Creature* pCreature)
{
    // TODO: Add gossip item only when Council is done?
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    pPlayer->SEND_GOSSIP_MENU(10465, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_akama_at_illidan(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)                    // Time to begin the event
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (npc_akama_illidanAI* pAkamaAI = dynamic_cast<npc_akama_illidanAI*>(pCreature->AI()))
            pAkamaAI->BeginDoorEvent(pPlayer);
    }
    return true;
}

struct MANGOS_DLL_DECL boss_maievAI : public ScriptedAI
{
    boss_maievAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    };

    uint32 m_uiTauntTimer;
    ObjectGuid m_illidanGuid;

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        m_uiTauntTimer = 12000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_illidanGuid)
        {
            if (m_pInstance)
                m_illidanGuid = m_pInstance->GetGuid(NPC_ILLIDAN_STORMRAGE);
        }
        else
        {
            Creature* pIllidan = m_creature->GetMap()->GetCreature(m_illidanGuid);

            if (!pIllidan || !pIllidan->isAlive() || pIllidan->IsInEvadeMode())
            {
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            else if (pIllidan && pIllidan->GetHealthPercent() < 2.0f)
                return;
        }

        // Return if we don't have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiTauntTimer < diff)
        {
            uint32 random = urand(0, 3);
            int32 text = aMaievTaunts[random].textId;

            DoScriptText(text, m_creature);

            m_uiTauntTimer = urand(22000, 42000);
        }
        else
            m_uiTauntTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL cage_trap_triggerAI : public ScriptedAI
{
    cage_trap_triggerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_illidanGuid;
    ObjectGuid m_cageTrapGuid;

    uint32 m_uiDespawnTimer;

    bool m_bActive;
    bool m_bSummonedBeams;

    void Reset()
    {
        m_bActive = false;
        m_bSummonedBeams = false;

        m_uiDespawnTimer = 0;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_bActive)
            return;

        if (who && (who->GetTypeId() != TYPEID_PLAYER))
        {
            if (who->GetEntry() == ILLIDAN_STORMRAGE)       // Check if who is Illidan
            {
                if (!m_illidanGuid && m_creature->IsWithinDistInMap(who, 3) && !who->HasAura(SPELL_CAGED, EFFECT_INDEX_0))
                {
                    m_illidanGuid = who->GetObjectGuid();
                    who->CastSpell(who, SPELL_CAGED, true);
                    m_uiDespawnTimer = 5000;

                    // Dispel his enrage
                    if (who->HasAura(SPELL_ENRAGE, EFFECT_INDEX_0))
                        who->RemoveAurasDueToSpell(SPELL_ENRAGE);

                    if (GameObject* pCageTrap = m_creature->GetMap()->GetGameObject(m_cageTrapGuid))
                        pCageTrap->SetLootState(GO_JUST_DEACTIVATED);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiDespawnTimer)
        {
            if (m_uiDespawnTimer <= diff)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            else m_uiDespawnTimer -= diff;
        }

        //if (m_illidanGuid && !m_bSummonedBeams)
        //{
        //    if (Creature* pIllidan = m_creature->GetMap()->GetCreature(m_illidanGuid)
        //    {
        //        //TODO: Find proper spells and properly apply 'caged' Illidan effect
        //    }
        //}
    }
};

bool GOUse_go_cage_trap(Player* pPlayer, GameObject* pGo)
{
    float x, y, z;
    pPlayer->GetPosition(x, y, z);

    // Grid search for nearest live creature of entry 23304 within 10 yards
    Creature* pTrigger = GetClosestCreatureWithEntry(pGo, 23304, 10.0f);

    if (!pTrigger)
    {
        error_log("SD2: Cage Trap- Unable to find trigger. This Cage Trap is now useless");
        return false;
    }

    if (cage_trap_triggerAI* pTriggerAI = dynamic_cast<cage_trap_triggerAI*>(pTrigger->AI()))
        pTriggerAI->m_bActive = true;

    pGo->SetGoState(GO_STATE_ACTIVE);
    return true;
}

struct MANGOS_DLL_DECL flame_of_azzinothAI : public ScriptedAI
{
    flame_of_azzinothAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiFlameBlastTimer;
    uint32 m_uiSummonBlazeTimer;
    uint32 m_uiChargeTimer;

    void Reset()
    {
        m_uiFlameBlastTimer = urand(15000, 30000);
        m_uiSummonBlazeTimer = urand(10000, 30000);
        m_uiChargeTimer = 5000;
    }

    void Charge()
    {
        // Get the Threat List
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();

        // He doesn't have anyone in his threatlist, useless to continue
        if (tList.empty())
            return;

        std::list<Unit*> targets;

        //store the threat list in a different container
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            Unit *target = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

            //only on alive players
            if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(target);
        }

        //Sort the list of players
        targets.sort(ObjectDistanceOrderReversed(m_creature));
        //Resize so we only get the furthest target
        targets.resize(1);

        Unit* target = (*targets.begin());
        if (target && (!m_creature->IsWithinDistInMap(target, 40)))
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE, CAST_TRIGGERED);
            DoCastSpellIfCan(target, SPELL_CHARGE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFlameBlastTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_BLAST);
            m_uiFlameBlastTimer = 30000;
        }
        else
            m_uiFlameBlastTimer -= diff;

        if (m_uiSummonBlazeTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLAZE_SUMMON);
            m_uiSummonBlazeTimer = urand(30000, 50000);
        }
        else
            m_uiSummonBlazeTimer -= diff;

        if (m_uiChargeTimer < diff)
        {
            Charge();
            m_uiChargeTimer = 5000;
        }
        else
            m_uiChargeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL shadow_demonAI : public ScriptedAI
{
    shadow_demonAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_targetGuid;

    void Reset() {}

    void JustDied(Unit *killer)
    {
        if (m_targetGuid)
        {
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_targetGuid))
                pPlayer->RemoveAurasDueToSpell(SPELL_PARALYZE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Only cast the below on players.
        if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER)
            return;

        if (!m_creature->getVictim()->HasAura(SPELL_PARALYZE, EFFECT_INDEX_0))
        {
            m_targetGuid = m_creature->getVictim()->GetObjectGuid();
            m_creature->AddThreat(m_creature->getVictim(), 10000000.0f);
            DoCastSpellIfCan(m_creature, SPELL_SHADOW_DEMON_PASSIVE, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_PURPLE_BEAM, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_PARALYZE, CAST_TRIGGERED);
        }
        // Kill our target if we're very close.
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 3))
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONSUME_SOUL);
    }
};

struct MANGOS_DLL_DECL flamecrashAI : public ScriptedAI
{
    flamecrashAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiFlameCrashTimer;
    uint32 m_uiDespawnTimer;

    void Reset()
    {
        m_uiFlameCrashTimer = urand(3000, 8000);
        m_uiDespawnTimer = 60000;
    }

    void AttackStart(Unit *who) { }

    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiFlameCrashTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FLAME_CRASH_EFFECT);
            m_uiFlameCrashTimer = 15000;
        }
        else
            m_uiFlameCrashTimer -= diff;

        if (m_uiDespawnTimer < diff)
        {
            // So that players don't see the sparkly effect when we die.
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        else
            m_uiDespawnTimer -= diff;
    }
};

/* ** TODO This code was unused for long time (not used in DB and pointless)
 * ** Keep it temporarily as reference

// Shadowfiends interact with Illidan, setting more targets in Illidan's hashmap
struct MANGOS_DLL_DECL mob_parasitic_shadowfiendAI : public ScriptedAI
{
    mob_parasitic_shadowfiendAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset() {}

    void DoMeleeAttackIfReady()
    {
        //If we are within range melee the target
        if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            //Make sure our attack is ready and we aren't currently casting
            if (m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                if (!m_creature->getVictim()->HasAura(SPELL_PARASITIC_SHADOWFIEND, EFFECT_INDEX_0))
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_PARASITIC_SHADOWFIEND, CAST_TRIGGERED);

                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
};
*/

struct MANGOS_DLL_DECL blazeAI : public ScriptedAI
{
    blazeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiBlazeTimer;
    uint32 m_uiDespawnTimer;

    void Reset()
    {
        m_uiBlazeTimer = 2000;
        m_uiDespawnTimer = 15000;
    }

    void AttackStart(Unit* who) { }

    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiBlazeTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLAZE_EFFECT);
            m_uiBlazeTimer = 15000;
        }
        else
            m_uiBlazeTimer -= diff;

        if (m_uiDespawnTimer < diff)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        else
            m_uiDespawnTimer -= diff;
    }
};

struct MANGOS_DLL_DECL blade_of_azzinothAI : public ScriptedAI
{
    blade_of_azzinothAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset() {}

    // Do-Nothing-But-Stand-There
    void AttackStart(Unit* who) { }
    void MoveInLineOfSight(Unit* who) { }

};

CreatureAI* GetAI_boss_illidan_stormrage(Creature* pCreature)
{
    return new boss_illidan_stormrageAI(pCreature);
}

CreatureAI* GetAI_npc_akama_at_illidan(Creature* pCreature)
{
    npc_akama_illidanAI* Akama_AI = new npc_akama_illidanAI(pCreature);

    for(uint8 i = 0; i < 13; ++i)
        Akama_AI->AddWaypoint(i, aAkamaWP[i].x, aAkamaWP[i].y, aAkamaWP[i].z);

    return ((CreatureAI*)Akama_AI);
}

CreatureAI* GetAI_boss_maiev(Creature* pCreature)
{
    return new boss_maievAI(pCreature);
}

CreatureAI* GetAI_mob_flame_of_azzinoth(Creature* pCreature)
{
    return new flame_of_azzinothAI(pCreature);
}

CreatureAI* GetAI_cage_trap_trigger(Creature* pCreature)
{
    return new cage_trap_triggerAI(pCreature);
}

CreatureAI* GetAI_shadow_demon(Creature* pCreature)
{
    return new shadow_demonAI(pCreature);
}

CreatureAI* GetAI_flamecrash(Creature* pCreature)
{
    return new flamecrashAI(pCreature);
}

CreatureAI* GetAI_demonfire(Creature* pCreature)
{
    return new demonfireAI(pCreature);
}

CreatureAI* GetAI_blaze(Creature* pCreature)
{
    return new blazeAI(pCreature);
}

CreatureAI* GetAI_blade_of_azzinoth(Creature* pCreature)
{
    return new blade_of_azzinothAI(pCreature);
}

/* ** TODO dead code
CreatureAI* GetAI_parasitic_shadowfiend(Creature* pCreature)
{
    return new mob_parasitic_shadowfiendAI(pCreature);
}
*/

void AddSC_boss_illidan()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_illidan_stormrage";
    pNewScript->GetAI = &GetAI_boss_illidan_stormrage;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_akama_illidan";
    pNewScript->GetAI = &GetAI_npc_akama_at_illidan;
    pNewScript->pGossipHello = &GossipHello_npc_akama_at_illidan;
    pNewScript->pGossipSelect = &GossipSelect_npc_akama_at_illidan;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_maiev_shadowsong";
    pNewScript->GetAI = &GetAI_boss_maiev;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_flame_of_azzinoth";
    pNewScript->GetAI = &GetAI_mob_flame_of_azzinoth;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_blade_of_azzinoth";
    pNewScript->GetAI = &GetAI_blade_of_azzinoth;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "gameobject_cage_trap";
    pNewScript->pGOUse = &GOUse_go_cage_trap;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_cage_trap_trigger";
    pNewScript->GetAI = &GetAI_cage_trap_trigger;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_shadow_demon";
    pNewScript->GetAI = &GetAI_shadow_demon;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_flame_crash";
    pNewScript->GetAI = &GetAI_flamecrash;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_demon_fire";
    pNewScript->GetAI = &GetAI_demonfire;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_blaze";
    pNewScript->GetAI = &GetAI_blaze;
    pNewScript->RegisterSelf();

    /* ** TODO dead code
    pNewScript = new Script;
    pNewScript->Name = "mob_parasitic_shadowfiend";
    pNewScript->GetAI = &GetAI_parasitic_shadowfiend;
    pNewScript->RegisterSelf();
    */
}

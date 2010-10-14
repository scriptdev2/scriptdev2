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

#define SAY_CONVO_1         -1564097
#define SAY_CONVO_2         -1564098
#define SAY_CONVO_3         -1564099
#define SAY_CONVO_4         -1564100
#define SAY_CONVO_5         -1564101
#define SAY_CONVO_6         -1564102
#define SAY_CONVO_7         -1564103
#define SAY_CONVO_8         -1564104
#define SAY_CONVO_9         -1564105
#define SAY_CONVO_10        -1564106
#define SAY_CONVO_11        -1564107
#define SAY_CONVO_12        -1564108
#define SAY_CONVO_13        -1564109
#define SAY_CONVO_14        -1564110
#define SAY_CONVO_15        -1564111

#define SAY_TAUNT_1         -1564112
#define SAY_TAUNT_2         -1564113
#define SAY_TAUNT_3         -1564114
#define SAY_TAUNT_4         -1564115

#define SAY_MAIEV_TAUNT_1   -1564116
#define SAY_MAIEV_TAUNT_2   -1564117
#define SAY_MAIEV_TAUNT_3   -1564118
#define SAY_MAIEV_TAUNT_4   -1564119

//emote only defined if not related to textId (in database)
struct Yells
{
    int32 textId;
    uint32 creature, timer, emote;
    bool Talk;
};

static Yells Conversation[]=
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
    {SAY_CONVO_14, MAIEV_SHADOWSONG, 0, true},
    {SAY_CONVO_15, AKAMA, 8000, 0, true},
    {0, EMPTY, 1000, 0, false}
};

static Yells RandomTaunts[]=
{
    {SAY_TAUNT_1, ILLIDAN_STORMRAGE, 0, 0, false},
    {SAY_TAUNT_2, ILLIDAN_STORMRAGE, 0, 0, false},
    {SAY_TAUNT_3, ILLIDAN_STORMRAGE, 0, 0, false},
    {SAY_TAUNT_4, ILLIDAN_STORMRAGE, 0, 0, false}
};

static Yells MaievTaunts[]=
{
    {SAY_MAIEV_TAUNT_1, MAIEV_SHADOWSONG, 0, 0, false},
    {SAY_MAIEV_TAUNT_2, MAIEV_SHADOWSONG, 0, 0, false},
    {SAY_MAIEV_TAUNT_3, MAIEV_SHADOWSONG, 0, 0, false},
    {SAY_MAIEV_TAUNT_4, MAIEV_SHADOWSONG, 0, 0, false}
};

// Yells for/by Akama
#define SAY_AKAMA_BEWARE      -1564120
#define SAY_AKAMA_MINION      -1564121
#define SAY_AKAMA_LEAVE       -1564122

// Self explanatory
#define SAY_KILL1             -1564123
#define SAY_KILL2             -1564124

// I think I'll fly now and let my subordinates take you on
#define SAY_TAKEOFF           -1564125
#define SAY_SUMMONFLAMES      -1564126

// When casting Eye Blast. Demon Fire will be appear on places that he casts this
#define SAY_EYE_BLAST         -1564127

// kk, I go big, dark and demon on you.
#define SAY_MORPH             -1564128

// I KILL!
#define SAY_ENRAGE            -1564129

/************** Spells *************/
// Normal Form
#define SPELL_SHEAR                     41032               // Reduces Max. Health by 60% for 7 seconds. Can stack 19 times. 1.5 second cast
#define SPELL_FLAME_CRASH               40832               // Summons an invis/unselect passive mob that has an aura of flame in a circle around him.
#define SPELL_DRAW_SOUL                 40904               // 5k Shadow Damage in front of him. Heals Illidan for 100k health (script effect)
#define SPELL_PARASITIC_SHADOWFIEND     41917               // DoT of 3k Shadow every 2 seconds. Lasts 10 seconds. (Script effect: Summon 2 parasites once the debuff has ticked off)
#define SPELL_SUMMON_PARASITICS         41915               // Summons 2 Parasitic Shadowfiends on the target. It's supposed to be cast as soon as the Parasitic Shadowfiend debuff is gone, but the spells aren't linked :(
#define SPELL_AGONIZING_FLAMES          40932               // 4k fire damage initial to target and anyone w/i 5 yards. PHASE 3 ONLY
#define SPELL_ENRAGE                    40683               // Increases damage by 50% and attack speed by 30%. 20 seconds, PHASE 5 ONLY
// Flying (Phase 2)
#define SPELL_THROW_GLAIVE              39635               // Throws a glaive on the ground
#define SPELL_THROW_GLAIVE2             39849               // Animation for the above spell
#define SPELL_GLAIVE_RETURNS            39873               // Glaive flies back to Illidan
#define SPELL_FIREBALL                  40598               // 2.5k-3.5k damage in 10 yard radius. 2 second cast time.
#define SPELL_DARK_BARRAGE              40585               // 10 second channeled spell, 3k shadow damage per second.
// Demon Form
#define SPELL_DEMON_TRANSFORM_1         40511               // First phase of animations for transforming into Dark Illidan (fall to ground)
#define SPELL_DEMON_TRANSFORM_2         40398               // Second phase of animations (kneel)
#define SPELL_DEMON_TRANSFORM_3         40510               // Final phase of animations (stand up and roar)
#define SPELL_DEMON_FORM                40506               // Transforms into Demon Illidan. Has an Aura of Dread on him.
#define SPELL_SHADOW_BLAST              41078               // 8k - 11k Shadow Damage. Targets highest threat. Has a splash effect, damaging anyone in 20 yards of the target.
#define SPELL_FLAME_BURST               41126               // Hurls fire at entire raid for ~3.5k damage every 10 seconds. Resistable. (Does not work: Script effect)
#define SPELL_FLAME_BURST_EFFECT        41131               // The actual damage. Handled by core (41126 triggers 41131)
// Other Illidan spells
#define SPELL_KNEEL                     39656               // Before beginning encounter, this is how he appears (talking to Wilson).
#define SPELL_SHADOW_PRISON             40647               // Illidan casts this spell to immobilize entire raid when he summons Maiev.
#define SPELL_DEATH                     41220               // This spell doesn't do anything except stun Illidan and set him on his knees.
#define SPELL_BERSERK                   45078               // Damage increased by 500%, attack speed by 150%

// Non-Illidan spells
#define SPELL_AKAMA_DOOR_CHANNEL        41268               // Akama's channel spell on the door before the Temple Summit
#define SPELL_DEATHSWORN_DOOR_CHANNEL   41269               // Olum and Udalo's channel spell on the door before the Temple Summit
#define SPELL_AKAMA_DOOR_FAIL           41271               // Not sure where this is really used...
#define SPELL_HEALING_POTION            40535               // Akama uses this to heal himself to full.
#define SPELL_AZZINOTH_CHANNEL          39857               // Glaives cast it on Flames. Not sure if this is the right spell.
#define SPELL_SHADOW_DEMON_PASSIVE      41079               // Adds the "shadowform" aura to Shadow Demons.
#define SPELL_CONSUME_SOUL              41080               // Once the Shadow Demons reach their target, they use this to kill them
#define SPELL_PARALYZE                  41083               // Shadow Demons cast this on their target
#define SPELL_PURPLE_BEAM               39123               // Purple Beam connecting Shadow Demon to their target
#define SPELL_CAGE_TRAP_DUMMY           40761               // Put this in DB for cage trap GO.
#define SPELL_EYE_BLAST_TRIGGER         40017               // This summons Demon Form every few seconds and deals ~20k damage in its radius
#define SPELL_EYE_BLAST                 39908               // This does the blue flamey animation.
#define SPELL_FLAME_CRASH_EFFECT        40836               // Firey blue ring of circle that the other flame crash summons
#define SPELL_BLAZE_EFFECT              40610               // Green flame on the ground, triggers damage (5k) every few seconds
#define SPELL_BLAZE_SUMMON              40637               // Summons the Blaze creature
#define SPELL_DEMON_FIRE                40029               // Blue fire trail left by Eye Blast. Deals 2k per second if players stand on it.
#define SPELL_CAGED                     40695               // Caged Trap triggers will cast this on Illidan if he is within 3 yards
#define SPELL_CAGE_TRAP_SUMMON          40694               // Summons a Cage Trap GO (bugged) on the ground along with a Cage Trap Disturb Trigger mob (working)
#define SPELL_CAGE_TRAP_BEAM            40713               // 8 Triggers on the ground in an octagon cast spells like this on Illidan 'caging him'
#define SPELL_FLAME_BLAST               40631               // Flames of Azzinoth use this. Frontal cone AoE 7k-9k damage.
#define SPELL_CHARGE                    40602               // Flames of Azzinoth charges whoever is too far from them. They enrage after this. For simplicity, we'll use the same enrage as Illidan.
#define SPELL_TELEPORT_VISUAL           41232               // Teleport visual for Maiev
#define SPELL_SHADOWFIEND_PASSIVE       41913               // Passive aura for shadowfiends

// Other defines
#define CENTER_X            676.740f
#define CENTER_Y            305.297f
#define CENTER_Z            353.192f

#define EQUIP_ID_MAIN_HAND  32837
#define EQUIP_ID_OFF_HAND   32838

/*** Phase Names ***/
enum Phase
{
    PHASE_NORMAL            =   1,
    PHASE_FLIGHT            =   2,
    PHASE_NORMAL_2          =   3,
    PHASE_DEMON             =   4,
    PHASE_NORMAL_MAIEV      =   5,
    PHASE_DEMON_SEQUENCE    =   6,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations GlaivePosition[]=
{
    {695.105f, 305.303f, 354.256f},
    {659.338f, 305.303f, 354.256f},
    {700.105f, 305.303f, 354.256f},
    {664.338f, 305.303f, 354.256f}
};

static Locations EyeBlast[]=
{
    {650.697f, 320.128f, 353.730f},
    {652.799f, 275.091f, 353.367f},
    {701.527f, 273.815f, 353.230f},
    {709.865f, 325.654f, 353.322f}
};

static Locations AkamaWP[]=
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
    {782.01f, 304.55f, 319.76f}                             // Final location - back at the initial gates. This is where he will fight the minions!
};
// 755.762, 304.0747, 312.1769 -- This is where Akama should be spawned
static Locations SpiritSpawns[]=
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
    uint32 aura, unaura, timer, size, displayid, phase;
    bool equip;
};

static Animation DemonTransformation[]=
{
    {SPELL_DEMON_TRANSFORM_1, 0, 1300, 0, 0, 6, true},
    {SPELL_DEMON_TRANSFORM_2, SPELL_DEMON_TRANSFORM_1, 4000, 0, 0, 6, true},
    {SPELL_DEMON_FORM, 0, 3000, 1073741824, 21322, 6, false},
    {SPELL_DEMON_TRANSFORM_3, SPELL_DEMON_TRANSFORM_2, 3500, 0, 0, 6, false},
    {0, 0, 0, 0, 0, 4, false},
    {SPELL_DEMON_TRANSFORM_1, 0, 1500, 0, 0, 6, false},
    {SPELL_DEMON_TRANSFORM_2, SPELL_DEMON_TRANSFORM_1, 4000, 0, 0, 6, false},
    {0, SPELL_DEMON_FORM, 3000, 1069547520, 21135, 6, false},
    {SPELL_DEMON_TRANSFORM_3, SPELL_DEMON_TRANSFORM_2, 3500, 0, 0, 6, true},
    {0, 0, 0, 0, 0, 8, true}
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

    uint64 IllidanGUID;

    bool IsTrigger;

    uint32 CheckTimer;
    uint32 DemonFireTimer;
    uint32 DespawnTimer;

    void Reset()
    {
        IllidanGUID = 0;

        IsTrigger = false;

        CheckTimer = 2000;
        DemonFireTimer = 0;
        DespawnTimer = 45000;
    }

    void AttackStart(Unit* who) { }
    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 diff)
    {
        if (IsTrigger)
            return;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (CheckTimer < diff)
        {
            if (!IllidanGUID && m_pInstance)
            {
                if (Creature* pIllidan = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_ILLIDANSTORMRAGE)))
                {
                    IllidanGUID = m_pInstance->GetData64(DATA_ILLIDANSTORMRAGE);

                    if (!pIllidan->HasSplineFlag(SPLINEFLAG_NO_SPLINE))
                        m_creature->SetDeathState(JUST_DIED);
                }
            }
            CheckTimer = 2000;
        }else CheckTimer -= diff;

        if (DemonFireTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DEMON_FIRE);
            DemonFireTimer = 30000;
        }else DemonFireTimer -= diff;

        if (DespawnTimer < diff)
            m_creature->SetDeathState(JUST_DIED);
        else DespawnTimer -= diff;

        DoMeleeAttackIfReady();
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
    uint32 ChannelTimer;
    uint32 TalkTimer;
    uint32 WalkTimer;
    uint32 SummonMinionTimer;

    /* GUIDs */
    uint64 IllidanGUID;
    uint64 PlayerGUID;
    uint64 SpiritGUID[2];
    uint64 ChannelGUID;

    bool IsTalking;
    bool StartChanneling;
    bool DoorOpen;
    bool FightMinions;
    bool IsReturningToIllidan;
    bool IsWalking;
    uint32 TalkCount;
    uint32 ChannelCount;

    std::list<WayPoints> WayPointList;
    std::list<WayPoints>::iterator WayPoint;

    void BeginEvent(uint64 PlayerGUID);

    void Reset()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ILLIDAN, NOT_STARTED);
            GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GAMEOBJECT_ILLIDAN_GATE));

            // close door if already open (when raid wipes or something)
            if (pGate && !pGate->GetGoState())
                pGate->SetGoState(GO_STATE_READY);

            for(uint8 i = DATA_GAMEOBJECT_ILLIDAN_DOOR_R; i < DATA_GAMEOBJECT_ILLIDAN_DOOR_L + 1; ++i)
            {
                if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(i)))
                    pDoor->SetGoState(GO_STATE_ACTIVE);
            }
        }

        IllidanGUID = 0;
        PlayerGUID  = 0;
        ChannelGUID = 0;
        for(uint8 i = 0; i < 2; ++i) SpiritGUID[i] = 0;

        ChannelTimer = 0;
        ChannelCount = 0;
        SummonMinionTimer = 2000;

        WalkTimer = 0;
        IsWalking = false;

        TalkTimer = 0;
        TalkCount = 0;

        KillAllElites();

        IsReturningToIllidan = false;
        FightMinions = false;
        IsTalking = false;
        StartChanneling = false;
        DoorOpen = false;

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
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

            if (pUnit && pUnit->GetTypeId() == TYPEID_UNIT && pUnit->GetEntry() == ILLIDARI_ELITE)
                pUnit->SetDeathState(JUST_DIED);
        }
    }

    void ReturnToIllidan()
    {
        KillAllElites();
        FightMinions = false;
        IsReturningToIllidan = true;
        WayPoint = WayPointList.begin();
        m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        IsWalking = true;
    }

    void AddWaypoint(uint32 id, float x, float y, float z)
    {
        WayPoints AWP(id, x, y, z);
        WayPointList.push_back(AWP);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && (done_by->GetGUID() != m_creature->GetGUID()))
        {
            damage = 0;
            DoCastSpellIfCan(m_creature, SPELL_HEALING_POTION);
        }
    }

    void BeginDoorEvent(Player* pPlayer)
    {
        if (!m_pInstance)
            return;

        debug_log("SD2: Akama - Door event initiated by player %s", pPlayer->GetName());
        PlayerGUID = pPlayer->GetGUID();

        if (GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GAMEOBJECT_ILLIDAN_GATE)))
        {
            float x,y,z;
            pGate->GetPosition(x, y, z);
            Creature* Channel = m_creature->SummonCreature(ILLIDAN_DOOR_TRIGGER, x, y, z+5, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);
            if (Channel)
            {
                ChannelGUID = Channel->GetGUID();

                // Invisible but spell visuals can still be seen.
                Channel->SetDisplayId(11686);
                Channel->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                float PosX, PosY, PosZ;
                m_creature->GetPosition(PosX, PosY, PosZ);
                for(uint8 i = 0; i < 2; ++i)
                {
                    Creature* Spirit = m_creature->SummonCreature(SpiritSpawns[i].id, SpiritSpawns[i].x, SpiritSpawns[i].y, SpiritSpawns[i].z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);
                    if (Spirit)
                    {
                        Spirit->SetVisibility(VISIBILITY_OFF);
                        SpiritGUID[i] = Spirit->GetGUID();
                    }
                }

                StartChanneling = true;
                m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoCastSpellIfCan(Channel, SPELL_AKAMA_DOOR_FAIL);
            }
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || !IsWalking)
            return;

        if (WayPoint->id != id)
            return;

        switch(id)
        {
            case 6:
                if (!IsReturningToIllidan)
                {
                    // open the doors that close the summit
                    for(uint32 i = DATA_GAMEOBJECT_ILLIDAN_DOOR_R; i < DATA_GAMEOBJECT_ILLIDAN_DOOR_L+1; ++i)
                    {
                        if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(i)))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                    }
                }
                break;
            case 7:
                if (IsReturningToIllidan)
                {
                    IsWalking = false;
                    if (IllidanGUID)
                    {
                        Creature* Illidan = m_creature->GetMap()->GetCreature(IllidanGUID);
                        if (Illidan)
                        {
                            float dx = Illidan->GetPositionX() + rand()%15;
                            float dy = Illidan->GetPositionY() + rand()%15;
                            m_creature->GetMotionMaster()->MovePoint(13, dx, dy, Illidan->GetPositionZ());
                            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, IllidanGUID);
                        }
                    }
                }
                break;
            case 8:
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (!IsReturningToIllidan)
                {
                    IsWalking = false;
                    BeginEvent(PlayerGUID);
                }
                break;
            case 12:
                IsWalking = false;
                FightMinions = true;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                break;
        }

        ++WayPoint;
        WalkTimer = 200;
    }

    void DeleteFromThreatList()
    {
        // If we do not have Illidan's GUID, do not proceed
        if (!IllidanGUID)
            return;

        // Create a pointer to Illidan
        Creature* Illidan = m_creature->GetMap()->GetCreature(IllidanGUID);

        // No use to continue if Illidan does not exist
        if (!Illidan)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            // Loop through threatlist till our GUID is found in it.
            if ((*itr)->getUnitGuid() == m_creature->GetGUID())
            {
                (*itr)->removeReference();                  // Delete ourself from his threatlist.
                return;                                     // No need to continue anymore.
            }
        }

        // Now we delete our threatlist to prevent attacking anyone for now
        m_creature->DeleteThreatList();
    }

    void UpdateAI(const uint32 diff)
    {
        if (IllidanGUID)
        {
            if (Creature* Illidan = m_creature->GetMap()->GetCreature(IllidanGUID))
            {
                if (Illidan->IsInEvadeMode() && !m_creature->IsInEvadeMode())
                    EnterEvadeMode();

                if (Illidan->GetHealthPercent() < 85.0f && m_creature->isInCombat() && !FightMinions)
                {
                    if (TalkTimer < diff)
                    {
                        switch(TalkCount)
                        {
                            case 0:
                                DoScriptText(SAY_AKAMA_MINION, Illidan);
                                TalkTimer = 8000;
                                TalkCount = 1;
                                break;
                            case 1:
                                DoScriptText(SAY_AKAMA_LEAVE, m_creature);
                                TalkTimer = 3000;
                                TalkCount = 2;
                                break;
                            case 2:
                                IsTalking = true;
                                TalkTimer = 2000;
                                m_creature->RemoveAllAuras();
                                m_creature->CombatStop(true);
                                m_creature->AttackStop();
                                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                TalkCount = 3;
                                break;
                            case 3:
                                DeleteFromThreatList();
                                IsWalking = true;
                                WayPoint = WayPointList.begin();
                                std::advance(WayPoint, 9);
                                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                break;
                        }
                    }else TalkTimer -= diff;
                }

                if (Illidan->GetHealthPercent() < 4.0f && !IsReturningToIllidan)
                    ReturnToIllidan();
            }
        }else
        {
            if (m_pInstance)
                IllidanGUID = m_pInstance->GetData64(DATA_ILLIDANSTORMRAGE);
        }

        if (IsWalking && WalkTimer)
        {
            if (WalkTimer <= diff)
            {
                if (WayPoint == WayPointList.end())
                    return;

                m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y,WayPoint->z);
                WalkTimer = 0;
            }else WalkTimer -= diff;
        }

        if (StartChanneling)
        {
            if (ChannelTimer < diff)
            {
                switch(ChannelCount)
                {
                    case 3:
                        if (!DoorOpen)
                        {
                            m_creature->InterruptNonMeleeSpells(true);

                            for(uint8 i = 0; i < 2; ++i)
                            {
                                if (SpiritGUID[i])
                                {
                                    Creature* Spirit = m_creature->GetMap()->GetCreature(SpiritGUID[i]);
                                    if (Spirit)
                                        Spirit->InterruptNonMeleeSpells(true);
                                }
                            }

                            if (GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GAMEOBJECT_ILLIDAN_GATE)))
                                pGate->SetGoState(GO_STATE_ACTIVE);

                            ++ChannelCount;
                            ChannelTimer = 5000;
                        }
                        break;
                    case 4:
                        m_creature->HandleEmote(EMOTE_ONESHOT_SALUTE);
                        ChannelTimer = 2000;
                        ++ChannelCount;
                        break;
                    case 5:
                        DoScriptText(SAY_AKAMA_BEWARE, m_creature);
                        if (ChannelGUID)
                        {
                            Creature* ChannelTarget = m_creature->GetMap()->GetCreature(ChannelGUID);
                            if (ChannelTarget)
                                ChannelTarget->SetDeathState(JUST_DIED);
                            ChannelGUID = 0;
                        }
                        for(uint8 i = 0; i < 2; ++i)
                        {
                            if (SpiritGUID[i])
                            {
                                Creature* Spirit = m_creature->GetMap()->GetCreature(SpiritGUID[i]);
                                if (Spirit)
                                    Spirit->SetDeathState(JUST_DIED);
                            }
                        }
                        ChannelTimer = 6000;
                        ++ChannelCount;
                        break;
                    case 6:
                        StartChanneling = false;
                        if (WayPointList.empty())
                        {
                            error_log("SD2: Akama has no waypoints to start with!");
                            return;
                        }

                        WayPoint = WayPointList.begin();
                        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
                        m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y, WayPoint->z);
                        IsWalking = true;
                        break;
                    default:
                        if (ChannelGUID)
                        {
                            Creature* Channel = m_creature->GetMap()->GetCreature(ChannelGUID);
                            if (Channel)
                            {
                                m_creature->InterruptNonMeleeSpells(true);

                                for(uint8 i = 0; i < 2; ++i)
                                {
                                    if (SpiritGUID[i])
                                    {
                                        Creature* Spirit = m_creature->GetMap()->GetCreature(SpiritGUID[i]);
                                        if (Spirit)
                                        {
                                            Spirit->InterruptNonMeleeSpells(true);
                                            if (ChannelCount%2 == 0)
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
                                if (ChannelCount < 3)
                                    ++ChannelCount;
                                ChannelTimer = 10000;
                            }
                        }
                        break;
                }
            }else ChannelTimer -= diff;
        }

        if (FightMinions)
        {
            if (SummonMinionTimer < diff)
            {
                if (IllidanGUID)
                {
                    Creature* Illidan = m_creature->GetMap()->GetCreature(IllidanGUID);
                    if (!Illidan || Illidan->IsInEvadeMode())
                    {
                        Reset();
                        EnterEvadeMode();
                        return;
                    }
                }

                float x,y,z;
                m_creature->GetPosition(x,y,z);
                Creature* Elite = m_creature->SummonCreature(ILLIDARI_ELITE, x+rand()%10, y+rand()%10, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 30000);
                if (Elite)
                {
                    Elite->AI()->AttackStart(m_creature);
                    Elite->AddThreat(m_creature, 1000000.0f);
                    AttackStart(Elite);
                }
                SummonMinionTimer = urand(10000, 16000);
            }else SummonMinionTimer -= diff;
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

        for(uint8 i = 0; i < 2; ++i)
        {
            FlameGUID[i] = 0;
            GlaiveGUID[i] = 0;
        }

        AkamaGUID = 0;
        MaievGUID = 0;

        Reset();
    }

    /** Instance Data **/
    ScriptedInstance* m_pInstance;

    /** Generic **/
    bool IsTalking;
    bool HasSummoned;
    bool RefaceVictim;
    bool InformAkama;
    uint32 Phase;
    uint32 GlobalTimer;
    uint32 TalkCount;
    uint32 DemonFormSequence;

    /** GUIDs **/
    uint64 FlameGUID[2];
    uint64 GlaiveGUID[2];
    uint64 AkamaGUID;
    uint64 MaievGUID;

    /** Timers **/
    uint32 ShearTimer;
    uint32 DrawSoulTimer;
    uint32 FlameCrashTimer;
    uint32 ParasiticShadowFiendTimer;
    uint32 FireballTimer;
    uint32 EyeBlastTimer;
    uint32 DarkBarrageTimer;
    uint32 SummonBladesTimer;                               // Animate summoning the Blades of Azzinoth in Phase 2
    uint32 SummonFlamesTimer;                               // Summon Flames of Azzinoth in Phase 2
    uint32 CheckFlamesTimer;                                // This is used to check the status of the Flames to see if we should begin entering Phase 3 or not.
    uint32 RetrieveBladesTimer;                             // Animate retrieving the Blades of Azzinoth in Phase 2 -> 3 transition
    uint32 LandTimer;                                       // This is used at the end of phase 2 to signal Illidan landing after Flames are dead
    uint32 AgonizingFlamesTimer;
    uint32 ShadowBlastTimer;
    uint32 FlameBurstTimer;
    uint32 ShadowDemonTimer;
    uint32 TalkTimer;
    uint32 TransformTimer;
    uint32 EnrageTimer;
    uint32 CageTimer;
    uint32 LayTrapTimer;
    uint32 AnimationTimer;
    uint32 TauntTimer;                                      // This is used for his random yells
    uint32 FaceVictimTimer;
    uint32 BerserkTimer;

    void Reset()
    {
        Phase = PHASE_NORMAL;

        // Check if any flames/glaives are alive/existing. Kill if alive and set GUIDs to 0
        for(uint8 i = 0; i < 2; ++i)
        {
            if (Creature* Flame = m_creature->GetMap()->GetCreature(FlameGUID[i]))
            {
                if (Flame->isAlive())
                    Flame->SetDeathState(JUST_DIED);

                FlameGUID[i] = 0;
            }

            if (Creature* Glaive = m_creature->GetMap()->GetCreature(GlaiveGUID[i]))
            {
                if (Glaive->isAlive())
                    Glaive->SetDeathState(JUST_DIED);

                GlaiveGUID[i] = 0;
            }
        }

        if (Creature* pAkama = m_creature->GetMap()->GetCreature(AkamaGUID))
        {
            if (!pAkama->isAlive())
                pAkama->Respawn();

            pAkama->AI()->EnterEvadeMode();
        }

        InformAkama = false;
        RefaceVictim = false;
        HasSummoned = false;

        FaceVictimTimer = 1000;
        BerserkTimer = 1500000;
        GlobalTimer = 0;
        DemonFormSequence = 0;

        /** Normal Form **/
        ShearTimer = urand(20000, 30000);                   // 20 to 30 seconds
        FlameCrashTimer = 30000;                            // 30 seconds
        ParasiticShadowFiendTimer = 25000;                  // 25 seconds
        DrawSoulTimer = 50000;                              // 50 seconds

        /** Phase 2 **/
        SummonBladesTimer = 10000;
        SummonFlamesTimer = 20000;                          // Phase 2 timers may be incorrect
        FireballTimer = 5000;
        DarkBarrageTimer = 45000;
        EyeBlastTimer = 30000;
        CheckFlamesTimer = 5000;
        RetrieveBladesTimer = 5000;
        LandTimer = 0;

        /** Phase 3+ **/
        AgonizingFlamesTimer = 35000;                       // Phase 3+ timers may be incorrect
        ShadowBlastTimer = 3000;
        FlameBurstTimer = 10000;
        ShadowDemonTimer = 30000;
        TransformTimer = 90000;
        EnrageTimer = 40000;
        CageTimer = 30000;
        LayTrapTimer = CageTimer + 2000;
        AnimationTimer = 0;

        TauntTimer = 30000;                                 // This timer may be off.

        m_creature->SetDisplayId(21135);
        m_creature->InterruptNonMeleeSpells(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        // Unequip warglaives if needed
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        m_creature->RemoveSplineFlag(SPLINEFLAG_NO_SPLINE);

        IsTalking = false;

        TalkCount = 0;
        TalkTimer = 0;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ILLIDAN, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void AttackStart(Unit *who)
    {
        if (!who || IsTalking || Phase == 2 || Phase == 4 || Phase == 6 || m_creature->HasAura(SPELL_KNEEL, EFFECT_INDEX_0))
            return;

        if (who == m_creature)
            return;

        if (m_creature->Attack(who, true))
        {
            m_creature->AddThreat(who);
            m_creature->SetInCombatWith(who);
            who->SetInCombatWith(m_creature);

            DoStartMovement(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || IsTalking || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if (!m_creature->CanFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
        }
    }

    void JustDied(Unit *killer)
    {
        IsTalking = false;
        TalkCount = 0;
        TalkTimer = 0;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (!m_pInstance)
            return;

        // Completed
        m_pInstance->SetData(TYPE_ILLIDAN, DONE);

        for(uint8 i = DATA_GAMEOBJECT_ILLIDAN_DOOR_R; i < DATA_GAMEOBJECT_ILLIDAN_DOOR_L + 1; ++i)
        {
            // Open Doors
            if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(i)))
                pDoor->SetGoState(GO_STATE_ACTIVE);
        }

    }

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth())                // Don't let ourselves be slain before we do our death speech
        {
            damage = 0;
            m_creature->SetHealth(m_creature->GetMaxHealth()/100);
        }
    }

    void Cast(Unit* victim, uint32 Spell, bool triggered = false)
    {
        if (!victim)
            return;

        RefaceVictim = true;
        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, victim->GetGUID());
        m_creature->CastSpell(victim, Spell, triggered);
    }

    /** This will handle the cast of eye blast **/
    void CastEyeBlast()
    {
        m_creature->InterruptNonMeleeSpells(false);

        DarkBarrageTimer += 10000;

        DoScriptText(SAY_EYE_BLAST, m_creature);

        uint32 initial = urand(0, 3);
        uint32 final = 0;

        if (initial < 3)
            final = initial+1;

        float initial_X = EyeBlast[initial].x;
        float initial_Y = EyeBlast[initial].y;
        float initial_Z = EyeBlast[initial].z;

        float final_X = EyeBlast[final].x;
        float final_Y = EyeBlast[final].y;
        float final_Z = EyeBlast[final].z;

        for(uint8 i = 0; i < 2; ++i)
        {
            if (Creature* pTrigger = m_creature->SummonCreature(DEMON_FIRE, initial_X, initial_Y, initial_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
            {
                if (demonfireAI* pTriggerAI = dynamic_cast<demonfireAI*>(pTrigger->AI()))
                    pTriggerAI->IsTrigger = true;

                pTrigger->GetMotionMaster()->MovePoint(0, final_X, final_Y, final_Z);

                if (!i)
                    pTrigger->CastSpell(pTrigger, SPELL_EYE_BLAST_TRIGGER, true);
                else
                {
                    pTrigger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTrigger->GetGUID());
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

        if (Conversation[count].textId)
            text = Conversation[count].textId;

        TalkTimer = Conversation[count].timer;
        uint32 emote = Conversation[count].emote;
        IsTalking = Conversation[count].Talk;
        Creature* pCreature = NULL;
        uint64 GUID = 0;

        if (Conversation[count].creature == ILLIDAN_STORMRAGE)
            pCreature = m_creature;
        else if (Conversation[count].creature == AKAMA)
        {
            if (!AkamaGUID)
            {
                if (m_pInstance)
                {
                    AkamaGUID = m_pInstance->GetData64(DATA_AKAMA);
                    if (!AkamaGUID)
                        return;
                    GUID = AkamaGUID;
                }
            }
            else GUID = AkamaGUID;
        }
        else if (Conversation[count].creature == MAIEV_SHADOWSONG)
        {
            if (!MaievGUID)
                return;
            GUID = MaievGUID;
        }
        else if (Conversation[count].creature == EMPTY)     // This is just for special cases without speech/sounds/emotes.
            return;

        if (GUID)                                           // Now we check if we actually specified a GUID, if so:
                                                            // we grab a pointer to that creature
            pCreature = m_creature->GetMap()->GetCreature(GUID);

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

    void HandleDemonTransformAnimation(uint32 count)
    {
        uint32 unaura = DemonTransformation[count].unaura;
        uint32 aura = DemonTransformation[count].aura;
        uint32 displayid = DemonTransformation[count].displayid;
        AnimationTimer = DemonTransformation[count].timer;
        uint32 size = DemonTransformation[count].size;

        m_creature->InterruptNonMeleeSpells(false);

        if (DemonTransformation[count].phase != 8)
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
        }

        if (unaura)
            m_creature->RemoveAurasDueToSpell(unaura);

        if (aura)
            DoCastSpellIfCan(m_creature, aura, CAST_TRIGGERED);

        if (displayid)
            // It's morphin time!
            m_creature->SetDisplayId(displayid);
        /*if (size)
            m_creature->SetUInt32Value(OBJECT_FIELD_SCALE_X, size); // Let us grow! (or shrink)*/

        if (DemonTransformation[count].equip)
        {
            // Requip warglaives if needed
            SetEquipmentSlots(false, EQUIP_ID_MAIN_HAND, EQUIP_ID_OFF_HAND, EQUIP_NO_CHANGE);
        }
        else
        {
            // Unequip warglaives if needed
            SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);
        }

        if (DemonTransformation[count].phase != 8)
            Phase = DemonTransformation[count].phase;       // Set phase properly
        else
        {
            // Refollow and attack our old victim
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

            // Depending on whether we summoned Maiev, we switch to either phase 5 or 3
            if (MaievGUID) Phase = PHASE_NORMAL_MAIEV;
            else Phase = PHASE_NORMAL_2;
        }

        if (count == 7)
        {
            DoResetThreat();
            m_creature->RemoveAurasDueToSpell(SPELL_DEMON_FORM);
        }
        else if (count == 4)
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

        SummonBladesTimer = 10000;                          // Summon Glaives when this decrements
        SummonFlamesTimer = 20000;                          // Summon Flames when this decrements
        GlobalTimer += 20000;
        LandTimer = 0;
        Phase = PHASE_FLIGHT;
        m_creature->RemoveAllAuras();
        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);

        // So players don't shoot us down
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        // We now hover!
        m_creature->AddSplineFlag(SPLINEFLAG_NO_SPLINE);

        m_creature->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
        for(uint8 i = 0; i < 2; ++i)
        {
            Creature* Glaive = m_creature->SummonCreature(BLADE_OF_AZZINOTH, GlaivePosition[i].x, GlaivePosition[i].y, GlaivePosition[i].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            if (Glaive)
            {
                GlaiveGUID[i] = Glaive->GetGUID();          // We need this to remove them later on
                Glaive->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Glaive->SetVisibility(VISIBILITY_OFF);
                Glaive->setFaction(m_creature->getFaction());
            }
        }
    }

    void SummonBladesOfAzzinoth()
    {
        m_creature->GetMotionMaster()->Clear(false);

        LandTimer = 0;
        RetrieveBladesTimer = 0;

        // Make it look like we're throwing the glaives on the ground
        DoCastSpellIfCan(m_creature, SPELL_THROW_GLAIVE2);

        // We no longer wear the glaives!
        // since they are now channeling the flames (or will be)
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        for(uint8 i = 0; i < 2; ++i)
        {
            Creature* Glaive = NULL;
            Glaive = m_creature->GetMap()->GetCreature(GlaiveGUID[i]);
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
            Glaive = m_creature->GetMap()->GetCreature(GlaiveGUID[i]);
            if (Glaive)
            {
                Flame = m_creature->SummonCreature(FLAME_OF_AZZINOTH, GlaivePosition[i+2].x, GlaivePosition[i+2].y, GlaivePosition[i+2].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                if (Flame)
                {
                    // Just in case the database has it as a different faction
                    Flame->setFaction(m_creature->getFaction());

                    // Attack our target!
                    Flame->AI()->AttackStart(m_creature->getVictim());

                    // Record GUID in order to check if they're dead later on to move to the next phase
                    FlameGUID[i] = Flame->GetGUID();

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
        HasSummoned = true;
    }

    void SummonMaiev()
    {
        TauntTimer += 4000;
        GlobalTimer += 4000;

        m_creature->InterruptNonMeleeSpells(false);         // Interrupt any of our spells
        Creature* Maiev = NULL;                             // Summon Maiev near Illidan
        Maiev = m_creature->SummonCreature(MAIEV_SHADOWSONG, m_creature->GetPositionX() + 10, m_creature->GetPositionY() + 5, m_creature->GetPositionZ()+2, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000);
        if (Maiev)
        {
            m_creature->GetMotionMaster()->Clear(false);    // Stop moving, it's rude to walk and talk!
            m_creature->GetMotionMaster()->MoveIdle();
                                                            // Just in case someone is unaffected by Shadow Prison
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoCastSpellIfCan(m_creature, SPELL_SHADOW_PRISON, CAST_TRIGGERED);
            TalkCount = 10;
            IsTalking = true;                               // We are now talking/
            Maiev->SetVisibility(VISIBILITY_OFF);           // Leave her invisible until she has to talk
            Maiev->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            MaievGUID = Maiev->GetGUID();
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
        DoCastSpellIfCan(m_creature, SPELL_DEATH);                    // Animate his kneeling + stun him
                                                            // Don't let the players interrupt our talk!
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->Clear(false);        // No moving!
        m_creature->GetMotionMaster()->MoveIdle();

        if (MaievGUID)
        {
            if (Creature* Maiev = m_creature->GetMap()->GetCreature(MaievGUID))
            {
                Maiev->CombatStop(true);                    // Maiev shouldn't do anything either. No point in her attacking us =]
                Maiev->GetMotionMaster()->Clear(false);     // Stop her from moving as well
                Maiev->GetMotionMaster()->MoveIdle();

                float distance = 10.0f;
                float dx = m_creature->GetPositionX() + (distance*cos(m_creature->GetOrientation()));
                float dy = m_creature->GetPositionY() + (distance*sin(m_creature->GetOrientation()));

                Maiev->NearTeleportTo(dx, dy, Maiev->GetPositionZ(), 0.0f);

                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                Maiev->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
            }
        }
        IsTalking = true;
        ++TalkCount;
    }

    void UpdateAI(const uint32 diff)
    {
        /*** This section will handle the conversations ***/
        if (IsTalking)                                      // Somewhat more efficient using a function rather than a long switch
        {
            if (TalkTimer < diff)
            {
                switch(TalkCount)                           // This is only for specialized cases
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
                        if (AkamaGUID)
                        {
                            if (Creature* pAkama = m_creature->GetMap()->GetCreature(AkamaGUID))
                            {
                                // Start attacking Akama
                                AttackStart(pAkama);

                                // Akama stop talk and start attack illidan
                                if (npc_akama_illidanAI* pAkamaAI = dynamic_cast<npc_akama_illidanAI*>(pAkama->AI()))
                                    pAkamaAI->IsTalking = false;

                                pAkama->AI()->AttackStart(m_creature);
                                pAkama->AddThreat(m_creature, 1000000.0f);
                            }
                        }

                        // We are now attackable!
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        debug_log("SD2: Black Temple: Illidan intro complete, players can attack Illidan.");
                        break;
                    case 11:
                        if (MaievGUID)
                        {
                            Creature* Maiev = m_creature->GetMap()->GetCreature(MaievGUID);
                            if (Maiev)
                            {
                                // Maiev is now visible
                                Maiev->SetVisibility(VISIBILITY_ON);
                                // onoz she looks like she teleported!
                                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                                // Have her face us
                                Maiev->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                // Face her, so it's not rude =P
                                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Maiev->GetGUID());
                            }
                        }
                        break;
                    case 14:
                        if (MaievGUID)
                        {
                            Creature* Maiev = m_creature->GetMap()->GetCreature(MaievGUID);
                            if (Maiev)
                            {
                                Maiev->GetMotionMaster()->Clear(false);
                                Maiev->GetMotionMaster()->MoveChase(m_creature);
                                // Have Maiev add a lot of threat on us so that players don't pull her off if they damage her via AOE
                                Maiev->AddThreat(m_creature, 10000000.0f);
                                // Force Maiev to attack us.
                                Maiev->AI()->AttackStart(m_creature);
                                Maiev->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            }
                        }
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        IsTalking = false;
                        FaceVictimTimer = 2000;
                        RefaceVictim = true;
                        break;
                    case 20:
                        // Kill ourself.
                        if (MaievGUID)
                        {
                            Creature* Maiev = m_creature->GetMap()->GetCreature(MaievGUID);
                            if (Maiev)
                            {
                                // Make Maiev leave
                                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                                Maiev->SetDeathState(JUST_DIED);
                            }
                        }
                        IsTalking = false;
                        if (m_creature->getVictim())
                            m_creature->getVictim()->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        else
                            // Now we kill ourself
                            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        break;
                }

                // This function does most of the talking
                Talk(TalkCount);
                ++TalkCount;
            }else TalkTimer -= diff;
        }

        // If we don't have a target, return.
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || IsTalking)
            return;

        // If we are 'caged', then we shouldn't do anything such as cast spells or transform into Demon Form.
        if (m_creature->HasAura(SPELL_CAGED, EFFECT_INDEX_0))
        {
            // Just so that he doesn't immediately enrage after he stops being caged.
            EnrageTimer = 40000;
            CageTimer = 30000;
            return;
        }

        // Berserk Timer - flat 25 minutes
        if (!m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0) && Phase != PHASE_DEMON_SEQUENCE)
        {
            if (BerserkTimer < diff)
            {
                DoScriptText(SAY_ENRAGE, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED);
            }else BerserkTimer -= diff;
        }

        if (RefaceVictim)
        {
            if (FaceVictimTimer < diff)
            {
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->getVictim()->GetGUID());
                FaceVictimTimer = 1000;
                RefaceVictim = false;
            }else FaceVictimTimer -= diff;
        }

        /** Signal to change to phase 2 **/
        if (m_creature->GetHealthPercent() < 65.0f && Phase == PHASE_NORMAL)
            EnterPhase2();

        /** Signal to summon Maiev **/
        if (m_creature->GetHealthPercent() < 30.0f && !MaievGUID && (Phase != PHASE_DEMON || Phase != PHASE_DEMON_SEQUENCE))
            SummonMaiev();

        /** Time for the death speech **/
        if (m_creature->GetHealthPercent() < 1.0f && !IsTalking && (Phase != PHASE_DEMON || Phase != PHASE_DEMON_SEQUENCE))
            InitializeDeath();

        /***** Spells for Phase 1, 3 and 5 (Normal Form) ******/
        if (Phase == PHASE_NORMAL || Phase == PHASE_NORMAL_2 || Phase == PHASE_NORMAL_MAIEV)
        {
            if (TauntTimer < diff)                           // His random taunt/yell timer.
            {
                uint32 random = urand(0, 3);
                int32 yell = RandomTaunts[random].textId;
                if (yell)
                    DoScriptText(yell, m_creature);
                TauntTimer = 32000;
            }else TauntTimer -= diff;

            // Global Timer so that spells do not overlap.
            if (GlobalTimer < diff)
            {
                if (ShearTimer < diff)
                {
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHEAR);
                    ShearTimer = urand(25000, 40000);
                    GlobalTimer += 2000;
                }else ShearTimer -= diff;

                if (FlameCrashTimer < diff)
                {
                    //It spawns multiple flames sometimes. Therefore, we'll do this manually.
                    //DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_CRASH);
                    m_creature->SummonCreature(FLAME_CRASH, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                    FlameCrashTimer = 35000;
                    GlobalTimer += 2000;
                }else FlameCrashTimer -= diff;

                if (ParasiticShadowFiendTimer < diff)
                {
                    Unit* target = NULL;
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1);
                    if (target && target->isAlive() && !target->HasAura(SPELL_PARASITIC_SHADOWFIEND, EFFECT_INDEX_0))
                    {
                        Cast(target, SPELL_PARASITIC_SHADOWFIEND);
                        ParasiticShadowFiendTimer = 40000;
                    }
                }else ParasiticShadowFiendTimer -= diff;

                if (DrawSoulTimer < diff)
                {
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_DRAW_SOUL);
                    DrawSoulTimer = 55000;
                    GlobalTimer += 3000;
                }else DrawSoulTimer -= diff;
            }else GlobalTimer -= diff;

            if (!IsTalking)
                DoMeleeAttackIfReady();
        }

        /*** Phase 2 ***/
        if (Phase == PHASE_FLIGHT)
        {
            // Check if we have summoned or not.
            if (!HasSummoned)
            {
                if (SummonBladesTimer)
                    if (SummonBladesTimer <= diff)
                {
                    SummonBladesOfAzzinoth();
                    SummonBladesTimer = 0;
                }else SummonBladesTimer -= diff;

                if (SummonFlamesTimer < diff)
                {
                    SummonFlamesOfAzzinoth();
                }else SummonFlamesTimer -= diff;
            }

            if (!m_creature->GetMotionMaster()->empty() && (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE))
                m_creature->GetMotionMaster()->Clear(false);

            if (HasSummoned)
            {
                if (CheckFlamesTimer)
                {
                    if (CheckFlamesTimer <= diff)
                    {
                        // Check if flames are dead or non-existant. If so, set GUID to 0.
                        for(uint8 i = 0; i < 2; ++i)
                        {
                            if (FlameGUID[i])
                            {
                                Creature* Flame = m_creature->GetMap()->GetCreature(FlameGUID[i]);

                                // If the flame dies, or somehow the pointer becomes invalid, reset GUID to 0.
                                if (!Flame || !Flame->isAlive())
                                    FlameGUID[i] = 0;
                            }
                        }
                        CheckFlamesTimer = 500;
                    }else CheckFlamesTimer -= diff;
                }

                // If both flames are dead/non-existant, kill glaives and change to phase 3.
                if (!FlameGUID[0] && !FlameGUID[1] && CheckFlamesTimer)
                {
                    RetrieveBladesTimer = 5000;             // Prepare for re-equipin!
                    CheckFlamesTimer = 0;
                }

                if (RetrieveBladesTimer)
                {
                    if (RetrieveBladesTimer <= diff)         // Time to get back our glaives!
                    {
                                                            // Interrupt any spells we might be doing *cough* DArk Barrage *cough*
                        m_creature->InterruptNonMeleeSpells(false);
                        for(uint8 i = 0; i < 2; ++i)
                        {
                            if (GlaiveGUID[i])
                            {
                                Creature* Glaive = m_creature->GetMap()->GetCreature(GlaiveGUID[i]);
                                if (Glaive)
                                {
                                    // Make it look like the Glaive flies back up to us
                                    Glaive->CastSpell(m_creature, SPELL_GLAIVE_RETURNS, true);
                                    // Despawn the Glaive
                                    Glaive->SetDeathState(JUST_DIED);
                                }
                                GlaiveGUID[i] = 0;
                            }
                        }

                        // Re-equip our warblades!
                        SetEquipmentSlots(false, EQUIP_ID_MAIN_HAND, EQUIP_ID_OFF_HAND, EQUIP_NO_CHANGE);

                        // Prepare for landin'!
                        LandTimer = 5000;
                        RetrieveBladesTimer = 0;
                    }else RetrieveBladesTimer -= diff;
                }

                if (LandTimer)
                {
                    // Time to land!
                    if (LandTimer <= diff)
                    {
                        DoResetThreat();

                        // anndddd touchdown!
                        m_creature->HandleEmote(EMOTE_ONESHOT_LAND);
                        m_creature->RemoveSplineFlag(SPLINEFLAG_NO_SPLINE);
                        Phase = PHASE_NORMAL_2;

                        // We should let the raid fight us =)
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->getVictim()->GetGUID());

                        // Chase our victim!
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    }else LandTimer -= diff;
                    return;                                 // Do not continue past this point if LandTimer is not 0 and we are in phase 2.
                }
            }

            if (GlobalTimer < diff)
            {
                if (FireballTimer < diff)
                {
                    Cast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_FIREBALL);
                    FireballTimer = 5000;
                }else FireballTimer -= diff;

                if (DarkBarrageTimer < diff)
                {
                    m_creature->InterruptNonMeleeSpells(false);

                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_DARK_BARRAGE);

                    DarkBarrageTimer = 35000;
                    GlobalTimer += 9000;
                }else DarkBarrageTimer -= diff;

                if (EyeBlastTimer < diff)
                {
                    CastEyeBlast();
                    EyeBlastTimer = 30000;
                }else EyeBlastTimer -= diff;
            }else GlobalTimer -= diff;
        }

        /** Phase 3,5 spells only**/
        if (Phase == PHASE_NORMAL_2 || Phase == PHASE_NORMAL_MAIEV)
        {
            if (GlobalTimer < diff)
            {
                if (AgonizingFlamesTimer < diff)
                {
                    CastAgonizingFlames();
                    AgonizingFlamesTimer = 60000;
                }else AgonizingFlamesTimer -= diff;
            }else GlobalTimer -= diff;

            if (TransformTimer < diff)
            {
                float CurHealth = m_creature->GetHealthPercent();
                // Prevent Illidan from morphing if less than 32% or 5%, as this may cause issues with the phase transition or death speech
                if ((CurHealth < 32.0f && !MaievGUID) || CurHealth < 5.0f)
                    return;

                Phase = PHASE_DEMON_SEQUENCE;               // Transform sequence
                DemonFormSequence = 0;
                AnimationTimer = 0;

                DoScriptText(SAY_MORPH, m_creature);

                TransformTimer = 60000;
                FlameBurstTimer = 10000;
                ShadowDemonTimer = 30000;
                m_creature->GetMotionMaster()->Clear(false);// Stop moving
            }else TransformTimer -= diff;
        }

        /** Phase 4 spells only (Demon Form) **/
        if (Phase == PHASE_DEMON)
        {
            // Stop moving if we are by clearing movement generators.
            if (!m_creature->GetMotionMaster()->empty())
                m_creature->GetMotionMaster()->Clear(false);

            if (TransformTimer < diff)
            {
                Phase = PHASE_DEMON_SEQUENCE;
                DemonFormSequence = 5;
                AnimationTimer = 100;
                TransformTimer = 60000;
            }else TransformTimer -= diff;

            if (ShadowDemonTimer < diff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                Creature* ShadowDemon = NULL;
                for(uint8 i = 0; i < 4; ++i)
                {
                    Unit* target = NULL;
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);

                    // only on players.
                    if (target && target->GetTypeId() == TYPEID_PLAYER)
                    {
                        ShadowDemon = m_creature->SummonCreature(SHADOW_DEMON, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
                        if (ShadowDemon)
                        {
                            ShadowDemon->AddThreat(target, 5000000.0f);
                            ShadowDemon->AI()->AttackStart(target);
                            ShadowDemon->SetInCombatWithZone();
                        }
                    }
                }
                ShadowDemonTimer = 60000;
            }else ShadowDemonTimer -= diff;

            if (GlobalTimer < diff)
            {
                if (ShadowBlastTimer < diff)
                {
                    Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0);
                    if (target && target->isAlive())
                    {
                        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, target->GetGUID());
                        DoCastSpellIfCan(target, SPELL_SHADOW_BLAST);
                        ShadowBlastTimer = 4000;
                        GlobalTimer += 1500;
                    }
                    if (!m_creature->HasAura(SPELL_DEMON_FORM, EFFECT_INDEX_0))
                        DoCastSpellIfCan(m_creature, SPELL_DEMON_FORM, CAST_TRIGGERED);
                }else ShadowBlastTimer -= diff;

                if (FlameBurstTimer < diff)
                {
                    DoCastSpellIfCan(m_creature, SPELL_FLAME_BURST);
                    FlameBurstTimer = 15000;
                }else FlameBurstTimer -= diff;
            }else GlobalTimer -= diff;
        }

        /** Phase 5 timers. Enrage spell **/
        if (Phase == PHASE_NORMAL_MAIEV)
        {
            if (EnrageTimer < diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                EnrageTimer = 40000;
                CageTimer = 30000;
                TransformTimer += 10000;
            }else EnrageTimer -= diff;

            // We'll handle Cage Trap in Illidan's script for simplicity's sake
            if (CageTimer < diff)
            {
                if (MaievGUID)
                {
                    Creature* Maiev = m_creature->GetMap()->GetCreature(MaievGUID);
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
                CageTimer = 15000;
            }else CageTimer -= diff;
        }

        if (Phase == PHASE_DEMON_SEQUENCE)                   // Demonic Transformation
        {
            if (AnimationTimer < diff)
            {
                HandleDemonTransformAnimation(DemonFormSequence);
                ++DemonFormSequence;
            }else AnimationTimer -= diff;
        }
    }
};

/*********************** End of Illidan AI ******************************************/

void npc_akama_illidanAI::BeginEvent(uint64 PlayerGUID)
{
    debug_log("SD2: Akama - Illidan Introduction started. Illidan event properly begun.");
    if (m_pInstance)
    {
        IllidanGUID = m_pInstance->GetData64(DATA_ILLIDANSTORMRAGE);
        m_pInstance->SetData(TYPE_ILLIDAN, IN_PROGRESS);
    }

    if (m_pInstance)
    {
        for(uint8 i = DATA_GAMEOBJECT_ILLIDAN_DOOR_R; i < DATA_GAMEOBJECT_ILLIDAN_DOOR_L+1; ++i)
        {
            if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(i)))
                pDoor->SetGoState(GO_STATE_READY);
        }
    }

    if (IllidanGUID)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (Creature* pIllidan = m_creature->GetMap()->GetCreature(IllidanGUID))
        {
            boss_illidan_stormrageAI* pIllidanAI = dynamic_cast<boss_illidan_stormrageAI*>(pIllidan->AI());

            if (!pIllidanAI)
                return;

            // Time for Illidan to stand up.
            pIllidan->RemoveAurasDueToSpell(SPELL_KNEEL);

            // First line of Akama-Illidan convo

            pIllidanAI->TalkCount = 0;

            // Begin Talking
            pIllidanAI->IsTalking = true;
            pIllidanAI->AkamaGUID = m_creature->GetGUID();

            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pIllidan->GetGUID());
            pIllidan->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());

            IsTalking = true;                               // Prevent Akama from starting to attack him
                                                            // Prevent players from talking again

            m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            pIllidan->GetMotionMaster()->Clear(false);
            pIllidan->GetMotionMaster()->MoveIdle();

            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();

            if (PlayerGUID)
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(PlayerGUID))
                    pIllidan->AddThreat(pPlayer, 100.0f);
            }
        }
    }
}

bool GossipHello_npc_akama_at_illidan(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    pPlayer->SEND_GOSSIP_MENU(10465, pCreature->GetGUID());

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

    uint32 TauntTimer;
    uint64 IllidanGUID;

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        TauntTimer = 12000;
        IllidanGUID = 0;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!IllidanGUID)
        {
            if (m_pInstance)
                IllidanGUID = m_pInstance->GetData64(DATA_ILLIDANSTORMRAGE);
        }else
        {
            Creature* Illidan = m_creature->GetMap()->GetCreature(IllidanGUID);

            if (!Illidan || !Illidan->isAlive() || Illidan->IsInEvadeMode())
            {
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            else if (Illidan && Illidan->GetHealthPercent() < 2.0f)
                return;
        }

        // Return if we don't have a target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (TauntTimer < diff)
        {
            uint32 random = urand(0, 3);
            int32 text = MaievTaunts[random].textId;

            DoScriptText(text, m_creature);

            TauntTimer = urand(22000, 42000);
        }else TauntTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL cage_trap_triggerAI : public ScriptedAI
{
    cage_trap_triggerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 IllidanGUID;
    uint64 CageTrapGUID;

    uint32 DespawnTimer;

    bool Active;
    bool SummonedBeams;

    void Reset()
    {
        IllidanGUID = 0;
        CageTrapGUID = 0;

        Active = false;
        SummonedBeams = false;

        DespawnTimer = 0;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!Active)
            return;

        if (who && (who->GetTypeId() != TYPEID_PLAYER))
        {
            if (who->GetEntry() == ILLIDAN_STORMRAGE)       // Check if who is Illidan
            {
                if (!IllidanGUID && m_creature->IsWithinDistInMap(who, 3) && !who->HasAura(SPELL_CAGED, EFFECT_INDEX_0))
                {
                    IllidanGUID = who->GetGUID();
                    who->CastSpell(who, SPELL_CAGED, true);
                    DespawnTimer = 5000;

                    // Dispel his enrage
                    if (who->HasAura(SPELL_ENRAGE, EFFECT_INDEX_0))
                        who->RemoveAurasDueToSpell(SPELL_ENRAGE);

                    if (GameObject* pCageTrap = m_creature->GetMap()->GetGameObject(CageTrapGUID))
                        pCageTrap->SetLootState(GO_JUST_DEACTIVATED);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (DespawnTimer)
        {
            if (DespawnTimer <= diff)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            else DespawnTimer -= diff;
        }

        //if (IllidanGUID && !SummonedBeams)
        //{
        //    if (Creature* pIllidan = m_creature->GetMap()->GetCreature(IllidanGUID)
        //    {
        //        //TODO: Find proper spells and properly apply 'caged' Illidan effect
        //    }
        //}
    }
};

bool GOHello_cage_trap(Player* pPlayer, GameObject* pGo)
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
        pTriggerAI->Active = true;

    pGo->SetGoState(GO_STATE_ACTIVE);
    return true;
}

struct MANGOS_DLL_DECL flame_of_azzinothAI : public ScriptedAI
{
    flame_of_azzinothAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 FlameBlastTimer;
    uint32 SummonBlazeTimer;
    uint32 ChargeTimer;

    void Reset()
    {
        FlameBlastTimer = urand(15000, 30000);
        SummonBlazeTimer = urand(10000, 30000);
        ChargeTimer = 5000;
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

        if (FlameBlastTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_BLAST);
            FlameBlastTimer = 30000;
        }else FlameBlastTimer -= diff;

        if (SummonBlazeTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLAZE_SUMMON);
            SummonBlazeTimer = urand(30000, 50000);
        }else SummonBlazeTimer -= diff;

        if (ChargeTimer < diff)
        {
            Charge();
            ChargeTimer = 5000;
        }else ChargeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL shadow_demonAI : public ScriptedAI
{
    shadow_demonAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 TargetGUID;

    void Reset() { TargetGUID = 0; }

    void JustDied(Unit *killer)
    {
        if (TargetGUID)
        {
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(TargetGUID))
                pPlayer->RemoveAurasDueToSpell(SPELL_PARALYZE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        // Only cast the below on players.
        if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER) return;

        if (!m_creature->getVictim()->HasAura(SPELL_PARALYZE, EFFECT_INDEX_0))
        {
            TargetGUID = m_creature->getVictim()->GetGUID();
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

    uint32 FlameCrashTimer;
    uint32 DespawnTimer;

    void Reset()
    {
        FlameCrashTimer = urand(3000, 8000);
        DespawnTimer = 60000;
    }

    void AttackStart(Unit *who) { }

    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 diff)
    {
        if (FlameCrashTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FLAME_CRASH_EFFECT);
            FlameCrashTimer = 15000;
        }else FlameCrashTimer -= diff;

        if (DespawnTimer < diff)
        {
            // So that players don't see the sparkly effect when we die.
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else DespawnTimer -= diff;
    }
};

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
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
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

struct MANGOS_DLL_DECL blazeAI : public ScriptedAI
{
    blazeAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 BlazeTimer;
    uint32 DespawnTimer;

    void Reset()
    {
        BlazeTimer = 2000;
        DespawnTimer = 15000;
    }

    void AttackStart(Unit* who) { }

    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 diff)
    {
        if (BlazeTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLAZE_EFFECT);
            BlazeTimer = 15000;
        }else BlazeTimer -= diff;

        if (DespawnTimer < diff)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else DespawnTimer -= diff;
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
        Akama_AI->AddWaypoint(i, AkamaWP[i].x, AkamaWP[i].y, AkamaWP[i].z);

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

CreatureAI* GetAI_parasitic_shadowfiend(Creature* pCreature)
{
    return new mob_parasitic_shadowfiendAI(pCreature);
}

void AddSC_boss_illidan()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_illidan_stormrage";
    newscript->GetAI = &GetAI_boss_illidan_stormrage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_akama_illidan";
    newscript->GetAI = &GetAI_npc_akama_at_illidan;
    newscript->pGossipHello = &GossipHello_npc_akama_at_illidan;
    newscript->pGossipSelect = &GossipSelect_npc_akama_at_illidan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_maiev_shadowsong";
    newscript->GetAI = &GetAI_boss_maiev;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_of_azzinoth";
    newscript->GetAI = &GetAI_mob_flame_of_azzinoth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_blade_of_azzinoth";
    newscript->GetAI = &GetAI_blade_of_azzinoth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "gameobject_cage_trap";
    newscript->pGOHello = &GOHello_cage_trap;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_cage_trap_trigger";
    newscript->GetAI = &GetAI_cage_trap_trigger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadow_demon";
    newscript->GetAI = &GetAI_shadow_demon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_crash";
    newscript->GetAI = &GetAI_flamecrash;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_demon_fire";
    newscript->GetAI = &GetAI_demonfire;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_blaze";
    newscript->GetAI = &GetAI_blaze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_parasitic_shadowfiend";
    newscript->GetAI = &GetAI_parasitic_shadowfiend;
    newscript->RegisterSelf();
}

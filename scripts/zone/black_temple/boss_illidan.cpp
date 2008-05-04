/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_illidan_stormrage
SD%Complete: 90
SDComment: Some workarounds for Flame Crash, Flame Burst (damage portion) and Parasitic Shadowfiend, unable to summon GOs.
SDCategory: Black Temple
EndScriptData */

/******** Includes ************/
#include "def_black_temple.h"
#include "TargetedMovementGenerator.h"
#include "PointMovementGenerator.h"
#include "WorldPacket.h"
#include "sc_gossip.h"

/************* Quotes and Sounds ***********************/
// Gossip for when a player clicks Akama
#define GOSSIP_ITEM          "We are ready to face Illidan"

// Signal for Akama to leave
#define SAY_AKAMA_MINION      "Come, my minions. Deal with this traitor as he deserves!"
#define SOUND_AKAMA_MINION    11465
#define SAY_AKAMA_LEAVE       "I'll deal with these mongrels. Strike now, friends! Strike at the betrayer!"
#define SOUND_AKAMA_LEAVE     11390

// Self explanatory
#define SAY_KILL1             "Who shall be next to taste my blades?!"
#define SOUND_KILL1           11473
#define SAY_KILL2             "This is too easy!"
#define SOUND_KILL2           11472

// I think I'll fly now and let my subordinates take you on
#define SAY_TAKEOFF           "I will not be touched by rabble such as you!"
#define SOUND_TAKEOFF         11479
#define SAY_SUMMONFLAMES      "Behold the flames of Azzinoth!"
#define SOUND_SUMMONFLAMES    11480

// When casting Eye Blast. Demon Fire will be appear on places that he casts this
#define SAY_EYE_BLAST         "Stare into the eyes of the Betrayer!"
#define SOUND_EYE_BLAST       11481

// kk, I go big, dark and demon on you.
#define SAY_MORPH             "Behold the power... of the demon within!"
#define SOUND_MORPH           11475

// I KILL!
#define SAY_ENRAGE            "You've wasted too much time mortals, now you shall fall!"
#define SOUND_ENRAGE          11474

/************** Spells *************/
// Normal Form
#define SPELL_SHEAR                     41032 // Reduces Max. Health by 60% for 7 seconds. Can stack 19 times. 1.5 second cast
#define SPELL_FLAME_CRASH               40832 // Summons an invis/unselect passive mob that has an aura of flame in a circle around him.
#define SPELL_DRAW_SOUL                 40904 // 5k Shadow Damage in front of him. Heals Illidan for 100k health (script effect)
#define SPELL_PARASITIC_SHADOWFIEND     41917 // DoT of 3k Shadow every 2 seconds. Lasts 10 seconds. (Script effect: Summon 2 parasites once the debuff has ticked off)
#define SPELL_SUMMON_PARASITICS         41915 // Summons 2 Parasitic Shadowfiends on the target. It's supposed to be cast as soon as the Parasitic Shadowfiend debuff is gone, but the spells aren't linked :(
#define SPELL_AGONIZING_FLAMES          40932 // 4k fire damage initial to target and anyone w/i 5 yards. PHASE 3 ONLY
#define SPELL_ENRAGE                    40683 // Increases damage by 50% and attack speed by 30%. 20 seconds, PHASE 5 ONLY
// Flying (Phase 2)
#define SPELL_FIREBALL                  40598 // 2.5k-3.5k damage in 10 yard radius. 2 second cast time.
#define SPELL_DARK_BARRAGE              40585 // 10 second channeled spell, 3k shadow damage per second.
// Demon Form
#define SPELL_DEMON_FORM                40506 // Transforms into Demon Illidan. Has an Aura of Dread on him.
#define SPELL_SHADOW_BLAST              41078 // 8k - 11k Shadow Damage. Targets highest threat. Has a splash effect, damaging anyone in 20 yards of the target.
#define SPELL_FLAME_BURST               41126 // Hurls fire at entire raid for ~3.5k damage every 10 seconds. Resistable. (Does not work: Script effect)
#define SPELL_FLAME_BURST_EFFECT        41131 // The actual damage. Have each player cast it on itself (workaround)
// Other Illidan spells
#define SPELL_KNEEL                     39656 // Before beginning encounter, this is how he appears (talking to skully).
#define SPELL_SHADOW_PRISON             40647 // Illidan casts this spell to immobilize entire raid when he summons Maiev.
#define SPELL_DEATH                     41220 // This spell doesn't do anything except stun Illidan and set him on his knees.

// Non-Illidan spells
#define SPELL_HEALING_POTION            40535 // Akama uses this to heal himself to full.
#define SPELL_GREEN_BEAM                39857 // Glaives cast it on Flames. Not sure if this is the right spell.
#define SPELL_SHADOW_DEMON_PASSIVE      41079 // Adds the "shadowform" aura to Shadow Demons.
#define SPELL_CONSUME_SOUL              41080 // Once the Shadow Demons reach their target, they use this to kill them
#define SPELL_PARALYZE                  41083 // Shadow Demons cast this on their target
#define SPELL_PURPLE_BEAM               39123 // Purple Beam connecting Shadow Demon to their target
#define SPELL_CAGE_TRAP_DUMMY           40761 // Put this in DB for cage trap GO.
#define SPELL_EYE_BLAST_TRIGGER         40017 // This summons Demon Form every few seconds and deals ~20k damage in its radius
#define SPELL_EYE_BLAST                 39908 // This does the blue flamey animation.
#define SPELL_FLAME_CRASH_EFFECT        40836 // Firey blue ring of circle that the other flame crash summons
#define SPELL_BLAZE_EFFECT              40610 // Green flame on the ground, triggers damage (5k) every few seconds
#define SPELL_BLAZE_SUMMON              40637 // Summons the Blaze creature
#define SPELL_DEMON_FIRE                40029 // Blue fire trail left by Eye Blast. Deals 2k per second if players stand on it.
#define SPELL_CAGED                     40695 // Caged Trap triggers will cast this on Illidan if he is within 3 yards
#define SPELL_CAGE_TRAP_SUMMON          40694 // Summons a Cage Trap GO (broken) on the ground along with a Cage Trap Disturb Trigger mob (working)
#define SPELL_FLAME_BLAST               40631 // Flames of Azzinoth use this. Frontal cone AoE 7k-9k damage.
#define SPELL_CHARGE                    40602 // Flames of Azzinoth charges whoever is too far from them. They enrage after this. For simplicity, we'll use the same enrage as Illidan.
#define SPELL_TELEPORT_VISUAL           41232 // Teleport visual for Maiev

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
};

struct Yells
{
    uint32 sound;
    char* text;
    uint32 creature, timer, emote;
    bool Talk;
};

static Yells Conversation[]=
{
    {11463, "Akama... your duplicity is hardly surprising. I should have slaughtered you and your malformed brethren long ago.", ILLIDAN_STORMRAGE, 8000, 0, true},
    {0, NULL, ILLIDAN_STORMRAGE, 5000, 396, true}, 
    {11389, "We've come to end your reign, Illidan. My people and all of Outland shall be free!", AKAMA, 7000, 25, true},
    {0, NULL, AKAMA, 5000, 66, true},
    {11464, "Boldly said. But I remain unconvinced.", ILLIDAN_STORMRAGE, 8000, 396, true},
    {11380, "The time has come! The moment is at hand!", AKAMA, 3000, 22, true},
    {0, NULL, AKAMA, 2000, 15, true},
    {11466, "You are not prepared!", ILLIDAN_STORMRAGE, 3000, 406, true},
    {0, NULL, EMPTY, 1000, 0, true},
    {0, NULL, EMPTY, 0, 0, false},
    {11476, "Is this it, mortals? Is this all the fury you can muster?", ILLIDAN_STORMRAGE, 8000, 0, true},
    {11491, "Their fury pales before mine, Illidan. We have some unsettled business between us.", MAIEV_SHADOWSONG, 8000, 5, true},
    {11477, "Maiev... How is this even possible?", ILLIDAN_STORMRAGE, 7000, 1, true},
    {11492, "Ah... my long hunt is finally over. Today, Justice will be done!", MAIEV_SHADOWSONG, 8000, 15, true},
    {11470, "Feel the hatred of ten thousand years!", ILLIDAN_STORMRAGE, 1000, 0, false},
    {11496, "Ahh... It is finished. You are beaten.", MAIEV_SHADOWSONG, 6000, 0, true},
    {11478, "You have won... Maiev...but the huntress... is nothing...without the hunt... you... are nothing... without me..", ILLIDAN_STORMRAGE, 22000, 65, true}, // Emote dead for now. Kill him later
    {11497, "He is right. I feel nothing... I am nothing... Farewell, champions.", MAIEV_SHADOWSONG, 9000, 0, true},
    {11387, "The Light will fill these dismal halls once again. I swear it.", AKAMA, 8000, 0, false}
};

static Yells RandomTaunts[]=
{
    {11467, "I can feel your hatred.", ILLIDAN_STORMRAGE, 0, 0, false},
    {11468, "Give in to your fear!", ILLIDAN_STORMRAGE, 0, 0, false},
    {11469, "You know nothing of power!", ILLIDAN_STORMRAGE, 0, 0, false},
    {11471, "Such... arrogance!", ILLIDAN_STORMRAGE, 0, 0, false}
};

static Yells MaievTaunts[]=
{
    {11493, "That is for Naisha!", MAIEV_SHADOWSONG, 0, false},
    {11494, "Bleed as I have bled!", MAIEV_SHADOWSONG, 0, 0, false},
    {11495, "There shall be no prison for you this time!", MAIEV_SHADOWSONG, 0, 0, false},
    {11500, "Meet your end, demon!", MAIEV_SHADOWSONG, 0, 0, false}
};

struct Locations
{
    float x, y, z;
};

static Locations GlaivePosition[]=
{
    {695.105, 305.303, 354.256},
    {659.338, 305.303, 354.256},
    {700.105, 305.303, 354.256},
    {664.338, 305.303, 354.256}
};

static Locations Flight[]=
{
    {692, 331, 353},
    {713, 309, 366},
    {687, 271, 353},
    {650, 285, 353},
    {653, 331, 371}
};

static Locations EyeBlast[]=
{
    {650.697, 320.128, 353.730},
    {652.799, 275.091, 353.367},
    {701.527, 273.815, 353.230},
    {709.865, 325.654, 353.322}
};

static Locations CageTrap[]=
{
    {670, 252, 353},
    {699, 265, 353},
    {675, 283, 355},
    {655, 301, 355},
    {681, 339, 353},
    {656, 341, 353},
    {705, 326, 354}
};

struct Animation // For the demon transformation
{
    uint32 aura, unaura, timer, size, displayid, phase;
    bool equip;
};

static Animation DemonTransformation[]=
{
    {40511, 0, 1300, 0, 0, 6, true},
    {40398, 40511, 4000, 0, 0, 6, true},
    {0, 0, 3000, 1073741824, 21322, 6, false},
    {40510, 40398, 3500, 0, 0, 6, false},
    {0, 0, 0, 0, 0, 4, false},
    {40511, 0, 1500, 0, 0, 6, false},
    {40398, 40511, 4000, 0, 0, 6, false},
    {0, 0, 3000, 1069547520, 21135, 6, false},
    {40510, 40398, 3500, 0, 0, 6, true},
    {0, 0, 0, 0, 0, 8, true}
};
/**** Demon Fire will be used for Eye Blast. Illidan needs to have access to it's vars and functions, so we'll set it here ****/
struct MANGOS_DLL_DECL demonfireAI : public ScriptedAI
{
    demonfireAI(Creature *c) : ScriptedAI(c) {Reset();}
   
    bool IsTrigger;

    uint32 DemonFireTimer;
    uint32 DespawnTimer;

    void Reset()
    {
        IsTrigger = false;

        DemonFireTimer = 0;
        DespawnTimer = 60000;
    }
   
    void Aggro(Unit *who)
    {
    }

    void AttackStart(Unit* who) { }
    void MoveInLineOfSight(Unit *who){ }

    void UpdateAI(const uint32 diff)
    {
        if(IsTrigger)
            return;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if(DemonFireTimer < diff)
        {
            DoCast(m_creature, SPELL_DEMON_FIRE);
            DemonFireTimer = 30000;
        }else DemonFireTimer -= diff;

        if(DespawnTimer < diff)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else DespawnTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

/******* Functions and vars for Akama's AI ******/
struct MANGOS_DLL_SPEC npc_akama_illidanAI : public ScriptedAI
{
    npc_akama_illidanAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
    
    /* Instance Data */
    ScriptedInstance* pInstance;

    /* Timers */
    uint32 TalkTimer;
 
    /* GUIDs */
    uint64 IllidanGUID;
    uint64 PlayerGUID;

    /* Generic */
    bool IsTalking;
    uint32 TalkCount;

    void Reset();
    void BeginEvent(Player *player);
    void Aggro(Unit *who);
    void DamageTaken(Unit *done_by, uint32 &damage);
    void DeleteFromThreatList();
    void UpdateAI(const uint32 diff);
};

/******* The AI *******/
struct MANGOS_DLL_SPEC boss_illidan_stormrageAI : public ScriptedAI
{
    boss_illidan_stormrageAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    /** Instance Data **/
    ScriptedInstance* pInstance;

    /** Generic **/
    bool IsTalking;
    bool HasSummoned;
    uint32 Phase;
    uint32 GlobalTimer;
    uint32 TalkCount;
    uint32 DemonFormSequence;

    /** GUIDs **/
    uint64 FlameGUID[2];
    uint64 GlaiveGUID[2];
    uint64 AkamaGUID;
    uint64 MaievGUID;
    uint64 ParasiticTargetGUID;

    /** Timers **/
    uint32 ShearTimer;
    uint32 DrawSoulTimer;
    uint32 FlameCrashTimer;
    uint32 ParasiticShadowFiendTimer;
    uint32 SummonParasiticTimer;
    uint32 FlyTimer;
    uint32 FireballTimer;
    uint32 EyeBlastTimer;
    uint32 DarkBarrageTimer;
    uint32 CheckFlamesTimer; // This is used to check the status of the Flames to see if we should begin entering Phase 3 or not.
    uint32 LandTimer; // This is used at the end of phase 2 to signal Illidan landing after Flames are dead
    uint32 AgonizingFlamesTimer;
    uint32 ShadowBlastTimer;
    uint32 FlameBurstTimer;
    uint32 ShadowDemonTimer;
    uint32 SummonTimer;
    uint32 TalkTimer;
    uint32 TransformTimer;
    uint32 EnrageTimer;
    uint32 CageTimer;
    uint32 LayTrapTimer;
    uint32 AnimationTimer;
    uint32 TauntTimer; // This is used for his random yells

    void Reset()
    {
        Phase = 1;

        // Check if any flames/glaives are alive/existing. Kill if alive and set GUIDs to 0
        for(uint8 i = 0; i < 2; i++)
        {
            if(FlameGUID[i])
            {
                Unit* Flame = Unit::GetUnit((*m_creature), FlameGUID[i]);
                if(Flame)
                {
                    Flame->DealDamage(Flame, Flame->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    Flame->SetVisibility(VISIBILITY_OFF);
                }
                FlameGUID[i] = 0;
            }

            if(GlaiveGUID[i])
            {
                Unit* Glaive = Unit::GetUnit((*m_creature), GlaiveGUID[i]);
                if(Glaive)
                {
                    Glaive->DealDamage(Glaive, Glaive->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    Glaive->SetVisibility(VISIBILITY_OFF);
                }
                GlaiveGUID[i] = 0;
            }
        }

        if(AkamaGUID)
        {
            Creature* Akama = ((Creature*)Unit::GetUnit((*m_creature), AkamaGUID));
            if(Akama)
            {
                if(!Akama->isAlive())
                    Akama->Respawn();
                Akama->SetVisibility(VISIBILITY_ON);
                Akama->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // Make him talkable again.
            }
        }

        HasSummoned = false;
        AkamaGUID = 0;
        MaievGUID = 0;
        ParasiticTargetGUID = 0;

        GlobalTimer = 0;
        DemonFormSequence = 0;

        /** Normal Form **/
        ShearTimer = 20000 + (rand()%11 * 1000); // 20 to 30 seconds
        FlameCrashTimer = 30000; //30 seconds
        ParasiticShadowFiendTimer = 25000; // 25 seconds
        SummonParasiticTimer = 35000; // 10 seconds after ParasiticShadowFiendTimer
        DrawSoulTimer = 50000; // 50 seconds

        /** Phase 2 **/
        FlyTimer = 20000; // Make him fly to various spots
        SummonTimer = 10000; // Phase 2 timers may be incorrect
        FireballTimer = 5000;
        DarkBarrageTimer = 45000;
        EyeBlastTimer = 30000;
        CheckFlamesTimer = 5000;
        LandTimer = 0;

        /** Phase 3+ **/
        AgonizingFlamesTimer = 35000; // Phase 3+ timers may be incorrect
        ShadowBlastTimer = 3000;
        FlameBurstTimer = 10000;
        ShadowDemonTimer = 30000;
        TransformTimer = 90000;
        EnrageTimer = 40000;
        CageTimer = 30000;
        LayTrapTimer = CageTimer + 2000;
        AnimationTimer = 0;

        TauntTimer = 30000; // This timer may be off.

        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 21135);
        m_creature->InterruptNonMeleeSpells(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        IsTalking = false;
        
        TalkCount = 0;
        TalkTimer = 0;

        if(pInstance)
            pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
    }

    void AttackStart(Unit *who)
    {
        if(!who || IsTalking || Phase == 2 || Phase == 4 || Phase == 6 || m_creature->HasAura(SPELL_KNEEL, 0))
            return;
        
        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartAttackAndMovement(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || IsTalking || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE))
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartAttackAndMovement(who);
            }
        }
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, DONE); // Completed

        IsTalking = false;
        TalkCount = 0;
        TalkTimer = 0;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature) return;

        switch(rand()%2)
        {
            case 0:
                DoYell(SAY_KILL1, LANG_UNIVERSAL, victim);
                DoPlaySoundToSet(m_creature, SOUND_KILL1);
                break;
            case 1:
                DoYell(SAY_KILL2, LANG_UNIVERSAL, victim);
                DoPlaySoundToSet(m_creature, SOUND_KILL2);
                break;
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage>m_creature->GetHealth()) // Don't let ourselves be slain before we do our death speech
        {
            damage = 0;
            m_creature->SetHealth(m_creature->GetMaxHealth()/100);
        }
    }

    /** This is a workaround so that he actually does his blade animations when melee'ing **/
    void DoMeleeAttackIfReadyWithAnimations()
    {
        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                switch(rand()%3)
                {
                    case 0:
                        m_creature->HandleEmoteCommand(389);
                        break;
                    case 1:
                        m_creature->HandleEmoteCommand(390);
                        break;
                    case 2:
                        m_creature->HandleEmoteCommand(36);
                        break;
                }
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }

    /** This will handle the cast of eye blast **/
    void CastEyeBlast()
    {
        m_creature->InterruptNonMeleeSpells(false);

        GlobalTimer += 30000;

        DoYell(SAY_EYE_BLAST, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_EYE_BLAST);
        
        uint32 initial = rand()%4;
        uint32 final = 0;
        if(initial < 3)
            final = initial+1;
        
        float initial_X = EyeBlast[initial].x;
        float initial_Y = EyeBlast[initial].y;
        float initial_Z = EyeBlast[initial].z;
        
        float final_X = EyeBlast[final].x;
        float final_Y = EyeBlast[final].y;
        float final_Z = EyeBlast[final].z;
        
        for(uint8 i = 0; i < 2; ++i)
        {
            Creature* Trigger = NULL;
            Trigger = m_creature->SummonCreature(DEMON_FIRE, initial_X, initial_Y, initial_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
            if(Trigger)
            {
                ((demonfireAI*)Trigger->AI())->IsTrigger = true;
                Trigger->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(0, final_X, final_Y, final_Z));
        
                if(!i)
                    Trigger->CastSpell(Trigger, SPELL_EYE_BLAST_TRIGGER, true);
                else
                    DoCast(Trigger, SPELL_EYE_BLAST, true);
            }
        }
    }

    /** Workaround for the Flame Burst so that it actually does damage **/
    void DoFlameBurstExplosion()
    {
        std::list<HostilReference *> m_threatlist = m_creature->getThreatManager().getThreatList();
        if(!m_threatlist.size()) return;
        std::list<Unit *> targets;
        std::list<HostilReference *>::iterator itr = m_threatlist.begin();
        std::advance(itr, 0);
        for( ; itr != m_threatlist.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if(pUnit && pUnit->isAlive()) // Check if pUnit exists and is alive
                pUnit->CastSpell(pUnit, SPELL_FLAME_BURST_EFFECT, false); // Have the player cast the flame burst effect on themself
        }
    }

    void Talk(uint32 count)
    {
        uint32 sound = Conversation[count].sound;
        char* text = NULL;
        if(Conversation[count].text)
            text = Conversation[count].text;
        TalkTimer = Conversation[count].timer;
        uint32 emote = Conversation[count].emote;
        IsTalking = Conversation[count].Talk;
        Creature* creature = NULL;
        uint64 GUID = 0;
        if(Conversation[count].creature == ILLIDAN_STORMRAGE)
            creature = m_creature;
        else if(Conversation[count].creature == AKAMA)
        {
            if(!AkamaGUID)
            {
                if(pInstance)
                {
                    AkamaGUID = pInstance->GetData64(DATA_AKAMA);
                    if(!AkamaGUID)
                        return;
                    GUID = AkamaGUID;
                }
            }
            else GUID = AkamaGUID;
        }
        else if(Conversation[count].creature == MAIEV_SHADOWSONG)
        {
            if(!MaievGUID)
                return;
            GUID = MaievGUID;
        }
        else if(Conversation[count].creature == EMPTY) // This is just for special cases without speech/sounds/emotes.
            return; 

        if(GUID) // Now we check if we actually specified a GUID, if so:
            creature = ((Creature*)Unit::GetUnit((*m_creature), GUID)); // we grab a pointer to that creature

        if(creature)
        {
            if(text)
                creature->Yell(text, LANG_UNIVERSAL, 0); // Have the creature yell out some text
            if(sound)
                DoPlaySoundToSet(creature, sound); // Play some sound on the creature
            creature->HandleEmoteCommand(emote); // Make the creature do some animation!
        }
    }
    
    void Move(float X, float Y, float Z, Creature* _Creature)
    {
        _Creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(0, X, Y, Z));
        //WorldPacket data( SMSG_MONSTER_MOVE, (41+c->GetPackGUID().size()) );
        //data.append(c->GetPackGUID());

        //data << c->GetPositionX() << c->GetPositionY() << c->GetPositionZ();
        //// unknown field - unrelated to orientation
        //// seems to increment about 1000 for every 1.7 seconds
        //// for now, we'll just use mstime
        //data << getMSTime();

        //data << uint8(0);                                // walkback when walking from A to B
        //data << uint32(flags);          // flags

        //data << Time;                                           // Time in between points
        //data << uint32(1);                                      // 1 single waypoint
        //data << X << Y << Z;                                    // the single waypoint Point B
        //c->SendMessageToSet( &data, true );
        //c->Relocate(X, Y, Z);
    }

    void HandleDemonTransformAnimation(uint32 count)
    {
        uint32 unaura = DemonTransformation[count].unaura; 
        uint32 aura = DemonTransformation[count].aura;
        uint32 displayid = DemonTransformation[count].displayid;
        AnimationTimer = DemonTransformation[count].timer;
        uint32 size = DemonTransformation[count].size;

        m_creature->InterruptNonMeleeSpells(false);

        if(DemonTransformation[count].phase != 8)
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->Idle();
        }
        
        if(unaura)
            m_creature->RemoveAurasDueToSpell(unaura);

        if(aura)
            DoCast(m_creature, aura, true);
        
        if(displayid)
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayid); // It's morphin time!
        /*if(size)
            m_creature->SetUInt32Value(OBJECT_FIELD_SCALE_X, size); // Let us grow! (or shrink)*/

        if(DemonTransformation[count].equip)
        {
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 45479); // Requip warglaives if needed
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 45481);
        }
        else
        {
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0); // Unequip warglaives if needed
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 0);
        }
        
        if(DemonTransformation[count].phase != 8)
            Phase = DemonTransformation[count].phase; // Set phase properly
        else
        {
            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim())); // Refollow and attack our old victim
            if(MaievGUID) Phase = 5; // Depending on whether we summoned Maiev, we switch to either phase 5 or 3
            else Phase = 3;
        }

        if(count == 7)
        {
            DoResetThreat();
            m_creature->RemoveAurasDueToSpell( SPELL_DEMON_FORM );
        }
        else if(count == 4)
        {
            DoResetThreat();
            if(!m_creature->HasAura(SPELL_DEMON_FORM, 0))
                DoCast(m_creature, SPELL_DEMON_FORM, true);
        }
    }

    /** To reduce the amount of code in UpdateAI, we can seperate them into different functions and simply call them from UpdateAI **/
    void EnterPhase2()
    {
        DoYell(SAY_TAKEOFF, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_TAKEOFF);

        SummonTimer = 10000; // Summon the Glaives + Flames when this decrements
        LandTimer = 0;
        Phase = 2;
        m_creature->SetHover(true); // We now hover!
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE); // So players don't shoot us down
        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF); // Animate our take off!
        m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0); // We no longer wear the glaives! 
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 0); // since they are now channeling the flames
        m_creature->GetMotionMaster()->Clear(false); // Stop moving!
    }

    void SummonFlamesOfAzzinoth()
    {
        DoYell(SAY_SUMMONFLAMES, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_SUMMONFLAMES);

        LandTimer = 0;

        for(uint8 i = 0; i < 2; i++)
        {
            Creature* Flame = NULL;
            Creature* Glaive = NULL;
            Glaive = m_creature->SummonCreature(BLADE_OF_AZZINOTH, GlaivePosition[i].x, GlaivePosition[i].y, GlaivePosition[i].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            if(Glaive)
            {
                GlaiveGUID[i] = Glaive->GetGUID(); // We need this to remove them later on
                Glaive->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE);
                Glaive->setFaction(m_creature->getFaction());
                Flame = m_creature->SummonCreature(FLAME_OF_AZZINOTH, GlaivePosition[i+2].x, GlaivePosition[i+2].y, GlaivePosition[i+2].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                if(Flame)
                {
                    Flame->setFaction(m_creature->getFaction()); // Just in case the database has it as a different faction
                    Flame->AI()->AttackStart(m_creature->getVictim()); // Attack our target!
                    FlameGUID[i] = Flame->GetGUID(); // Record GUID in order to check if they're dead later on to move to the next phase
                    Glaive->CastSpell(Flame, SPELL_GREEN_BEAM, true); // Glaives do some random Beam type channel on it.
                    // Run through our threatlist and give the flame a small taste of who to attack
                    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
                    std::list<HostilReference*>::iterator itr = m_threatlist.begin();
                    for( ; itr != m_threatlist.end(); ++itr)
                    {
                        Unit* target = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
                        if(target && target->isAlive())
                            Flame->AddThreat(target, 1.0f);
                    }
                    if(m_creature->getVictim())
                        Flame->AI()->AttackStart(m_creature->getVictim());
                }
                else
                {
                    DoTextEmote("is unable to summon a Flame of Azzinoth.", NULL);
                    error_log("ERROR: Illidan Stormrage AI: Unable to summon Flame of Azzinoth (entry: 22997), please check your database");
                    EnterEvadeMode();
                }
            }
            else
            {
                DoTextEmote("is unable to summon a Blade of Azzinoth.", NULL);
                error_log("ERROR: Illidan Stormrage AI: Unable to summon Blade of Azzinoth (entry: 22996), please check your database");
            }
        }
        HasSummoned = true;
    }

    void SummonMaiev()
    {
        TauntTimer += 4000;
        GlobalTimer += 4000;

        m_creature->InterruptNonMeleeSpells(false); // Interrupt any of our spells
        Creature* Maiev = NULL; // Summon Maiev near Illidan
        Maiev = m_creature->SummonCreature(MAIEV_SHADOWSONG, m_creature->GetPositionX() + 10, m_creature->GetPositionY() + 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 0);
        if(Maiev)
        {
            m_creature->GetMotionMaster()->Clear(false); // Stop moving, it's rude to walk and talk!
            m_creature->GetMotionMaster()->Idle();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE); // Just in case someone is unaffected by Shadow Prison
            DoCast(m_creature, SPELL_SHADOW_PRISON);
            TalkCount = 10;
            IsTalking = true; // We are now talking/
            Maiev->SetVisibility(VISIBILITY_OFF); // Leave her invisible until she has to talk
            MaievGUID = Maiev->GetGUID();
        }
        else // If Maiev cannot be summoned, reset the encounter and post some errors to the console.
        {
            EnterEvadeMode();
            DoTextEmote("is unable to summon Maiev Shadowsong and enter Phase 4", NULL);
            error_log("ERROR: Unable to summon Maiev Shadowsong (entry: 23197). Check your database to see if you have the proper SQL for Maiev Shadowsong (entry: 23197)");
        }
    }

    void InitializeDeath()
    {
        DoCast(m_creature, SPELL_DEATH); // Animate his kneeling + stun him
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE); // Don't let the players interrupt our talk!
        m_creature->GetMotionMaster()->Clear(false); // No moving!
        m_creature->GetMotionMaster()->Idle();
        if(MaievGUID)
        {
            Creature* Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
            if(Maiev)
            {
                Maiev->CombatStop(); // Maiev shouldn't do anything either. No point in her attacking us =]
                Maiev->GetMotionMaster()->Clear(false); // Stop her from moving as well
                Maiev->GetMotionMaster()->Idle();
            }
        }
        // Since we killed Akama before, respawn him now
        Creature* Akama = ((Creature*)Unit::GetUnit((*m_creature), AkamaGUID));
        if(Akama)
        {
            if(Akama->isDead())
                Akama->Respawn();

            Akama->SetVisibility(VISIBILITY_OFF); // Invisible for now, we'll make him visible during his turn to talk
            if(Akama->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                Akama->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // So they don't make him restart the event ~~
        }
        IsTalking = true;
        TalkCount++;
    }

    void UpdateAI(const uint32 diff)
    {
        /*** This section will handle the conversations ***/
        if(IsTalking) // Somewhat more efficient using a function rather than a long switch
        {
            if(TalkTimer < diff)
            {
                switch(TalkCount) // This is only for specialized cases
                {
                    case 0:
                        m_creature->RemoveAurasDueToSpell( SPELL_KNEEL ); // Time to stand up!
                        break;
                    case 8:
                        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 45479); // Equip our warglaives!
                        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 45481);
                        m_creature->HandleEmoteCommand(406); // Animate the equipping
                        m_creature->setFaction(14); // Hostile if we weren't before
                        break;
                    case 9:
                        if(AkamaGUID)
                        {
                            Creature* Akama = ((Creature*)Unit::GetUnit((*m_creature), AkamaGUID));
                            if(Akama)
                            {
                                Akama->GetMotionMaster()->Clear(false);
                                Akama->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature)); // Akama runs to us!
                                m_creature->GetMotionMaster()->Clear(false);
                                m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*Akama)); // We run to Akama!
                                Akama->AddThreat(m_creature, 1000000.0f);
                                AttackStart(Akama); // Start attacking Akama
                                ((npc_akama_illidanAI*)Akama->AI())->IsTalking = false;
                                ((npc_akama_illidanAI*)Akama->AI())->AttackStart(m_creature); // Akama starts attacking us
                            }
                        }
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE); // We are now attackable!
                        break;
                    case 11:
                        if(MaievGUID)
                        {
                            Unit* Maiev = Unit::GetUnit((*m_creature), MaievGUID);
                            if(Maiev)
                            {
                                Maiev->SetVisibility(VISIBILITY_ON); // Maiev is now visible
                                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true); // onoz she looks like she teleported!
                                Maiev->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID()); // Have her face us
                                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Maiev->GetGUID()); // Face her, so it's not rude =P
                            }
                        }
                        break;
                    case 14:
                        if(MaievGUID)
                        {
                            Creature* Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                            if(Maiev)
                            {
                                Maiev->GetMotionMaster()->Clear(false);
                                Maiev->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature));
                                Maiev->AddThreat(m_creature, 10000000.0f); // Have Maiev add a lot of threat on us so that players don't pull her off if they damage her via AOE
                                Maiev->AI()->AttackStart(m_creature); // Force Maiev to attack us.
                                Maiev->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            }
                        }
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE);
                        m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
                        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->getVictim()->GetGUID());
                        IsTalking = false;
                        break;
                    case 15:
                        if(AkamaGUID)
                        {
                            Creature* Akama = ((Creature*)Unit::GetUnit((*m_creature), AkamaGUID));
                            if(Akama) // Make Akama visible and move towards us for the final talk.
                            {
                                Akama->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                                Akama->SetVisibility(VISIBILITY_ON);
                                float PosX = m_creature->GetPositionX() + 10;
                                float PosY = m_creature->GetPositionY() - 10;
                                float PosZ = m_creature->GetPositionZ();
                                Akama->GetMotionMaster()->Clear(false);
                                Akama->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(0, PosX, PosY, PosZ));
                            }
                        }
                        break;
                    case 18: // Kill ourself.
                        //m_creature->RemoveAllAuras();
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
                        if(MaievGUID)
                        {
                            Creature* Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                            if(Maiev) // Make Maiev leave
                            {
                                Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true);
                                Maiev->SetVisibility(VISIBILITY_OFF); // Close enough....
                                Maiev->DealDamage(Maiev, Maiev->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            }
                        }
                        IsTalking = false;
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); // Now we kill ourself
                        break;
                }
                Talk(TalkCount); // This function does most of the talking
                TalkCount++;
            }else TalkTimer -= diff;
        }

        // This is placed outside the victim check so that it can tick even if Illidan is talking or transforming or whatnot.
        if(SummonParasiticTimer < diff)
        {
            if(ParasiticTargetGUID && (ParasiticTargetGUID != MaievGUID))
            {
                Unit* target = NULL;
                target = Unit::GetUnit((*m_creature), ParasiticTargetGUID);
                if(target && target->isAlive()) // Only on living targets.
                    DoCast(target, SPELL_SUMMON_PARASITICS, true);
                ParasiticTargetGUID = 0;
            }
            SummonParasiticTimer = ParasiticShadowFiendTimer + 10000;
        }else SummonParasiticTimer -= diff;

        // If we don't have a target, return.
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() || IsTalking)
            return;

        // If we are 'caged', then we shouldn't do anything such as cast spells or transform into Demon Form.
        if(m_creature->HasAura(SPELL_CAGED, 0))
        {
            EnrageTimer = 40000; // Just so that he doesn't immediately enrage after he stops being caged.
            CageTimer = 30000;
            return;
        }

        /** Signal to change to phase 2 **/
        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 65) && (Phase == 1))
            EnterPhase2();

        /** Signal to summon Maiev **/
        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30) && (!MaievGUID))
            SummonMaiev();

        /** Time for the death speech **/
        if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 1) && (!IsTalking))
            InitializeDeath();

        /***** Spells for Phase 1, 3 and 5 (Normal Form) ******/
        if(Phase == 1 || Phase == 3 || Phase == 5)
        {
            if(TauntTimer < diff) // His random taunt/yell timer.
            {
                uint32 random = rand()%4;
                char* yell = RandomTaunts[random].text;
                uint32 soundid = RandomTaunts[random].sound;
                if(yell)
                    DoYell(yell, LANG_UNIVERSAL, NULL);
                if(soundid)
                    DoPlaySoundToSet(m_creature, soundid);
                TauntTimer = 32000;
            }else TauntTimer -= diff;

            if(GlobalTimer < diff) // Global Timer so that spells do not overlap.
            {
                if(ShearTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_SHEAR);
                    ShearTimer = 25000 + (rand()%16 * 1000);
                    GlobalTimer += 2000;
                }else ShearTimer -= diff;

                if(FlameCrashTimer < diff)
                {
                    // It spawns multiple flames sometimes. Therefore, we'll do this manually.
                    // DoCast(m_creature->getVictim(), SPELL_FLAME_CRASH);
                    m_creature->HandleEmoteCommand(402);
                    DoSpawnCreature(FLAME_CRASH, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 40000);
                    FlameCrashTimer = 35000;
                    GlobalTimer += 2000;
                }else FlameCrashTimer -= diff;

                if(ParasiticShadowFiendTimer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,1);
                    if(target && target->isAlive())
                    {
                        DoCast(target, SPELL_PARASITIC_SHADOWFIEND); // The Parasitic Shadowfiend debuff doesn't summon 2 Shadowfiends on the target when it fades
                        ParasiticTargetGUID = target->GetGUID(); // So we'll do it somewhat manually by recording the afflicted target's GUID and setting the summon timer
                        SummonParasiticTimer = 10000; // Once the timer runs out, we cast the summon shadowfiend spell on the target and thereby summon the mobs.
                    }
                    ParasiticShadowFiendTimer = 40000;
                }else ParasiticShadowFiendTimer -= diff;

                if(DrawSoulTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_DRAW_SOUL);
                    DrawSoulTimer = 55000;
                    GlobalTimer = 3000;
                }else DrawSoulTimer -= diff;
            }else GlobalTimer -= diff;

            if(!IsTalking)
                DoMeleeAttackIfReadyWithAnimations();
        }

        /*** Phase 2 ***/
        if(Phase == 2)
        {
            // Check if we have summoned or not.
            if(!HasSummoned)
            {
                if(SummonTimer < diff)
                {
                    SummonFlamesOfAzzinoth();
                }else SummonTimer -= diff;
            }

            if(!m_creature->GetMotionMaster()->empty() && (m_creature->GetMotionMaster()->top()->GetMovementGeneratorType() != POINT_MOTION_TYPE))
                m_creature->GetMotionMaster()->Clear(false);

            if(HasSummoned)
            {            
                if(LandTimer)
                {
                    if(LandTimer < diff) // Time to land!
                    {
                        //m_creature->SetHover(false);
                        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND); // anndddd touchdown!
                        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
                        Phase = 3;
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE); // We should let the raid fight us =)
                        m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim())); // Chase our victim!
                        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 45479); // Re-equip our warblades!
                        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 45481);
                    }else LandTimer -= diff;
                    return; // Do not continue past this point if LandTimer is not 0 and we are in phase 2.
                }

                if(CheckFlamesTimer < diff)
                {
                    // Check if flames are dead or non-existant. If so, set GUID to 0.
                    for(uint8 i = 0; i < 2; i++)
                    {
                        if(FlameGUID[i])
                        {
                            Unit* Flame = NULL;
                            Flame = Unit::GetUnit((*m_creature), FlameGUID[i]);

                            // If the flame dies, or somehow the pointer becomes invalid, reset GUID to 0. 
                            if(Flame && Flame->isAlive())
                            {
                                if(Flame->getVictim())
                                    m_creature->AddThreat(Flame->getVictim(), Flame->getThreatManager().getThreat(Flame->getVictim()));
                            }
                            else
                                FlameGUID[i] = 0;
                        }
                    }
                    CheckFlamesTimer = 5000;
                }else CheckFlamesTimer -= diff;

                // If both flames are dead/non-existant, kill glaives and change to phase 3.
                if(!FlameGUID[0] && !FlameGUID[1])
                {
                    for(uint8 i = 0; i < 2; i++)
                    {
                        if(GlaiveGUID[i])
                        {
                            Unit* Glaive = NULL;
                            Glaive = Unit::GetUnit((*m_creature), GlaiveGUID[i]);
                            if(Glaive)
                                Glaive->DealDamage(Glaive, Glaive->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); // No point letting the Glaives continue to exist
                            GlaiveGUID[i] = 0;
                        }
                        // Place this bit outside the check because there were some instances where GlaiveGUID was reset to 0 before entering this loop, causing Illidan to lock up in the air.
                        LandTimer = 10000; // Prepare for landin!
                        m_creature->InterruptNonMeleeSpells(false); // Interrupt any spells we might be doing *cough* DArk Barrage *cough*
                    }
                }
            }

            if(GlobalTimer < diff)
            {
                if(FlyTimer < diff)
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    uint32 random = rand()%5;
                    Move(Flight[random].x, Flight[random].y, Flight[random].z, m_creature);
                    FlyTimer = 15000;
                    GlobalTimer = 1000;
                }else FlyTimer -= diff;

                if(FireballTimer < diff)
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_FIREBALL);
                    FireballTimer = 7000;
                    GlobalTimer += 1500;
                }else FireballTimer -= diff;

                if(DarkBarrageTimer < diff)
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_DARK_BARRAGE);
                    DarkBarrageTimer = 41000;
                    GlobalTimer += 9000;
                }else DarkBarrageTimer -= diff;

                if(EyeBlastTimer < diff)
                {
                    // Please test
                    CastEyeBlast(); 
                    EyeBlastTimer = 30000;
                }else EyeBlastTimer -= diff;
            }else GlobalTimer -= diff;
        }

        /** Phase 3,5 spells only**/
        if(Phase == 3 || Phase == 5)
        {
            if(GlobalTimer < diff)
            {
                if(AgonizingFlamesTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_AGONIZING_FLAMES);
                    AgonizingFlamesTimer = 60000;
                    GlobalTimer += 1500;
                }else AgonizingFlamesTimer -= diff;
            }else GlobalTimer -= diff;
           
            if(TransformTimer < diff)
            {
                // Prevent Illidan from morphing if less than 32%, as this may cause issues with the phase transition
                if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 32) && !MaievGUID)
                    return;

                Phase = 6; // Transform sequence
                DemonFormSequence = 0;
                AnimationTimer = 0;
                DoYell(SAY_MORPH, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_MORPH);
                TransformTimer = 60000;
                FlameBurstTimer = 10000;
                ShadowDemonTimer = 30000;
                m_creature->GetMotionMaster()->Clear(false); // Stop moving
            }else TransformTimer -= diff;
        }

        /** Phase 4 spells only (Demon Form) **/
        if(Phase == 4)
        {
            // Stop moving if we are by clearing movement generators.
            if(!m_creature->GetMotionMaster()->empty())
                m_creature->GetMotionMaster()->Clear(false);

            if(TransformTimer < diff)
            {
                Phase = 6;
                DemonFormSequence = 5;
                AnimationTimer = 100;
                TransformTimer = 60000;
            }else TransformTimer -= diff;

            if(ShadowDemonTimer < diff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                Creature* ShadowDemon = NULL;
                for(uint8 i = 0; i < 4; i++)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if(target && target->GetTypeId() == TYPEID_PLAYER) // only on players.
                    {
                        ShadowDemon = DoSpawnCreature(SHADOW_DEMON, 0,0,0,0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,25000);
                        if(ShadowDemon)
                        {
                            ShadowDemon->AddThreat(target, 5000000.0f);
                            ShadowDemon->AI()->AttackStart(target);
                            // Run through threatlist and give Shadow Demons targets if they kill the initial one.
                            for(std::list<HostilReference*>::iterator itr = m_creature->getThreatManager().getThreatList().begin(); itr != m_creature->getThreatManager().getThreatList().end(); ++itr)
                            {
                                Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
                                if(pUnit && pUnit->isAlive())
                                    ShadowDemon->AddThreat(pUnit, 1.0f);
                            }
                        }
                    }
                }
                ShadowDemonTimer = 60000;
            }else ShadowDemonTimer -= diff;

            if(GlobalTimer < diff)
            {
                if(ShadowBlastTimer < diff)
                {
                    Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                    if(target && target->isAlive())
                    {
                        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, target->GetGUID());
                        DoCast(target, SPELL_SHADOW_BLAST);
                        ShadowBlastTimer = 4000;
                        GlobalTimer += 1500;
                    }
                }else ShadowBlastTimer -= diff;

                if(FlameBurstTimer < diff)
                {
                    DoCast(m_creature, SPELL_FLAME_BURST);
                    FlameBurstTimer = 22000;
                    //DoFlameBurstExplosion();
                    GlobalTimer += 1000;
                }else FlameBurstTimer -= diff;
            }else GlobalTimer -= diff;
        }

        /** Phase 5 timers. Enrage spell **/
        if(Phase == 5)
        {
            if(EnrageTimer < diff)
            {
                DoCast(m_creature, SPELL_ENRAGE);
                EnrageTimer = 40000;
                CageTimer = 30000;
                TransformTimer += 10000;
            }else EnrageTimer -= diff;
            
            // We'll handle Cage Trap in Illidan's script for simplicity's sake
            if(CageTimer < diff)
            {
                if(MaievGUID)
                {
                    Unit* Maiev = Unit::GetUnit((*m_creature), MaievGUID);
                    if(!Maiev) return;

                    uint32 random = rand()%7; // Get the location of a random cage trap
                    float X = CageTrap[random].x;
                    float Y = CageTrap[random].y;
                    float Z = CageTrap[random].z;
                    Maiev->SendMoveToPacket(X, Y, Z, false, 0);
                    Maiev->Relocate(X, Y, Z, Maiev->GetOrientation());
                    Maiev->CastSpell(Maiev, SPELL_TELEPORT_VISUAL, true); // Make it look like she 'teleported'
                    Maiev->CastSpell(Maiev, SPELL_CAGE_TRAP_SUMMON, false); // summon the trap!
                }
                CageTimer = 15000;
            }else CageTimer -= diff;
        }
        
        if(Phase == 6) // Demonic Transformation
        {
            if(AnimationTimer < diff)
            {
                HandleDemonTransformAnimation(DemonFormSequence);
                DemonFormSequence++;
            }else AnimationTimer -= diff;
        }
    }
};

void npc_akama_illidanAI::Reset()
{
    if(pInstance) pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, 0); // Not started

    IllidanGUID = 0;
    PlayerGUID  = 0;

    TalkTimer = 0;
    TalkCount = 0;
 
    IsTalking = false;

    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    m_creature->SetVisibility(VISIBILITY_ON);
}

void npc_akama_illidanAI::BeginEvent(Player *player)
{

    if(pInstance)
    {
        IllidanGUID = pInstance->GetData64(DATA_ILLIDANSTORMRAGE);
        pInstance->SetData(DATA_ILLIDANSTORMRAGEEVENT, IN_PROGRESS);
    }

    if(IllidanGUID)
    {
        Creature* Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID));
        if(Illidan)
        {
            Illidan->RemoveAurasDueToSpell(SPELL_KNEEL); // Time for Illidan to stand up.
            Illidan->SetInCombat(); // Get ready to begin attacking
            m_creature->SetInCombat();
            ((boss_illidan_stormrageAI*)Illidan->AI())->TalkCount = 0; // First line of Akama-Illidan convo
            ((boss_illidan_stormrageAI*)Illidan->AI())->IsTalking = true; // Begin Talking
            ((boss_illidan_stormrageAI*)Illidan->AI())->AkamaGUID = m_creature->GetGUID();
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Illidan->GetGUID());
            IsTalking = true; // Prevent Akama from starting to attack him
            m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // Prevent players from talking again
            Illidan->GetMotionMaster()->Clear(false);
            Illidan->GetMotionMaster()->Idle();
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->Idle();

            if(player)
                Illidan->AddThreat(player, 100.0f);
        }
    }
}

void npc_akama_illidanAI::Aggro(Unit *who)
{
}

void npc_akama_illidanAI::DamageTaken(Unit *done_by, uint32 &damage)
{
    if(damage > m_creature->GetHealth() && (done_by->GetGUID() != m_creature->GetGUID()))
    {
        damage = 0;
        DoCast(m_creature, SPELL_HEALING_POTION);
    }
}

void npc_akama_illidanAI::DeleteFromThreatList()
{
    if(!IllidanGUID) return; // If we do not have Illidan's GUID, do not proceed
    Creature* Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID)); // Create a pointer to Illidan
    if(!Illidan) return; // No use to continue if Illidan does not exist
    std::list<HostilReference *> i_threatlist = Illidan->getThreatManager().getThreatList();
    if(!i_threatlist.size()) return;// Threatlist is empty, do not proceed.
    std::list<HostilReference *>::iterator itr = i_threatlist.begin();
    std::advance(itr, 0);
    for( ; itr != i_threatlist.end(); ++itr)
    {
        if((*itr)->getUnitGuid() == m_creature->GetGUID()) // Loop through threatlist till our GUID is found in it.
        {
            (*itr)->removeReference(); // Delete ourself from his threatlist.
            return; // No need to continue anymore.
        }
    }
}

void npc_akama_illidanAI::UpdateAI(const uint32 diff)
{
    if(IllidanGUID)
    {
        Creature* Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID));
        if(Illidan)
        {
            if(Illidan->IsInEvadeMode() && !m_creature->IsInEvadeMode())
                EnterEvadeMode();

            if(((Illidan->GetHealth()*100 / Illidan->GetMaxHealth()) < 85) && InCombat)
            {
                if(!m_creature->GetMotionMaster()->empty())
                {
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->Idle();
                }
                if(TalkTimer < diff)
                {
                    switch(TalkCount)
                    {
                        case 0:
                            Illidan->Yell(SAY_AKAMA_MINION, LANG_UNIVERSAL, 0);
                            DoPlaySoundToSet(Illidan, SOUND_AKAMA_MINION);
                            TalkTimer = 10000;
                            TalkCount = 1;
                            break;
                        case 1:
                            DoYell(SAY_AKAMA_LEAVE, LANG_UNIVERSAL, NULL);
                            DoPlaySoundToSet(m_creature, SOUND_AKAMA_LEAVE);
                            TalkTimer = 3000;
                            TalkCount = 2;
                            break;
                        case 2:
                            m_creature->SetVisibility(VISIBILITY_OFF);
                            IsTalking = true;
                            TalkTimer = 2000;
                            TalkCount = 3;
                            break;
                        case 3:
                            DeleteFromThreatList();
                            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            break;
                    }
                }else TalkTimer -= diff;
            }
        }
    }else
    {
        if(pInstance)
            IllidanGUID = pInstance->GetData64(DATA_ILLIDANSTORMRAGE);
    }

    // If we don't have a target, or is talking, or has run away, return
    if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() || IsTalking) return;
 
    DoMeleeAttackIfReady();
}

bool GossipSelect_npc_akama_at_illidan(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if(action == GOSSIP_ACTION_INFO_DEF) // Time to begin the event
    {
        player->PlayerTalkClass->CloseGossip();
        ((npc_akama_illidanAI*)_Creature->AI())->BeginEvent(player);
    }
    return true;
}

bool GossipHello_npc_akama_at_illidan(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    player->PlayerTalkClass->SendGossipMenu(10465, _Creature->GetGUID());

    return true;
}

struct MANGOS_DLL_SPEC boss_maievAI : public ScriptedAI
{
    boss_maievAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset(); 
    };
    
    uint32 TauntTimer;
    uint64 IllidanGUID;

    ScriptedInstance* pInstance;

    void Reset()
    {
        TauntTimer = 12000;
        IllidanGUID = 0;
    }

    void Aggro(Unit *who)
    {
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        damage = 0;
    }
    
    /** Similar workaround as Illian's attack anims **/
    void DoMeleeAttackIfReadyWithAnimations()
    {
        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->HandleEmoteCommand(36);
                //m_creature->AttackerStateUpdate(m_creature->getVictim()); // Causes problems when Illidan aggroes her; they fall under the map.
                m_creature->resetAttackTimer();
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartAttackAndMovement(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        // Return if we don't have a target
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!IllidanGUID)
        {
            if(pInstance)
                IllidanGUID = pInstance->GetData64(DATA_ILLIDANSTORMRAGE);
        }else
        {
            Unit* Illidan = NULL;
            Illidan = Unit::GetUnit((*m_creature), IllidanGUID);
            if(Illidan && (!Illidan->isAlive() || ((Illidan->GetHealth()*100 / Illidan->GetMaxHealth()) < 2) || (Illidan->getThreatManager().getThreatList().empty())))
                return; // Cease all action if Illidan is dead or in the death phase or has no aggro target
        }

        if(TauntTimer < diff)
        {
            uint32 random = rand()%4;
            char* text = MaievTaunts[random].text;
            uint32 sound = MaievTaunts[random].sound;
            DoYell(text, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, sound);
            TauntTimer = 22000 + rand()%21 * 1000;
        }else TauntTimer -= diff;

        DoMeleeAttackIfReadyWithAnimations();
    }
};

struct MANGOS_DLL_DECL cage_trap_triggerAI : public ScriptedAI
{
    cage_trap_triggerAI(Creature *c) : ScriptedAI(c) {Reset();}
   
    uint32 DespawnTimer;

    bool HasCaged;

    void Reset()
    {
        HasCaged = false;
        DespawnTimer = 0;

        m_creature->SetUInt32Value(OBJECT_FIELD_SCALE_X, 1036831949);
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 15882);
    }
   
    void Aggro(Unit *who){}

    void MoveInLineOfSight(Unit *who)
    {
        if(who && (who->GetTypeId() != TYPEID_PLAYER))
        {
            if(who->GetEntry() == 22917) // Check if who is Illidan
            {
                if((!HasCaged) && (!who->HasAura(SPELL_CAGED, 0)) && m_creature->IsWithinDistInMap(who, 3))
                {
                    HasCaged = true;
                    who->CastSpell(who, SPELL_CAGED, true);
                    DespawnTimer = 5000;
                    if(who->HasAura(SPELL_ENRAGE, 0))
                        who->RemoveAurasDueToSpell(SPELL_ENRAGE); // Dispel his enrage
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(DespawnTimer)
        {
            if(DespawnTimer < diff)
            {
                m_creature->SetVisibility(VISIBILITY_OFF); // So that players don't see the sparkly effect when we die.
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }else DespawnTimer -= diff;
        }
    }
};

//This is used to sort the players by distance in preparation for being charged by the flames.
struct TargetDistanceOrder : public std::binary_function<const Unit, const Unit, bool>
{
    const Unit* MainTarget;
    TargetDistanceOrder(const Unit* Target) : MainTarget(Target) {};
    // functor for operator ">"
    bool operator()(const Unit* _Left, const Unit* _Right) const
    {
        return (MainTarget->GetDistanceSq(_Left) > MainTarget->GetDistanceSq(_Right));
    }
};

struct MANGOS_DLL_DECL flame_of_azzinothAI : public ScriptedAI
{
    flame_of_azzinothAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 FlameBlastTimer;
    uint32 SummonBlazeTimer;
    uint32 ChargeTimer;

    void Reset()
    {
        FlameBlastTimer = 15000 + rand()%15000;
        SummonBlazeTimer = 10000 + rand()%20000;
        ChargeTimer = 5000;
    }
    
    void Aggro(Unit *who)
    {
    }

    void Charge()
    {        
        // Get the Threat List
        std::list<HostilReference *> m_threatlist = m_creature->getThreatManager().getThreatList();
        
        if(!m_threatlist.size()) return; // He doesn't have anyone in his threatlist, useless to continue

        std::list<Unit *> targets;
        std::list<HostilReference *>::iterator itr = m_threatlist.begin();
        std::advance(itr, 0);
        for( ; itr!= m_threatlist.end(); ++itr) //store the threat list in a different container
        {
            Unit *target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
            if(target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER ) //only on alive players
                targets.push_back( target);
        }

        //Sort the list of players
        targets.sort(TargetDistanceOrder(m_creature));
        //Resize so we only get the furthest target
        targets.resize(1);

        Unit* target = (*targets.begin());
        if(target && (!m_creature->IsWithinDistInMap(target, 40)))
        {
            DoCast(m_creature, SPELL_ENRAGE, true);
            DoCast(target, SPELL_CHARGE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        // Return if we don't have a target
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(FlameBlastTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_BLAST);
            FlameBlastTimer = 30000;
        }else FlameBlastTimer -= diff;

        if(SummonBlazeTimer < diff)
        {
            DoCast(m_creature, SPELL_BLAZE_SUMMON);
            SummonBlazeTimer = 30000 + rand()%20000;
        }else SummonBlazeTimer -= diff;

        if(ChargeTimer < diff)
        {
           // Charge();
            ChargeTimer = 5000;
        }else ChargeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL shadow_demonAI : public ScriptedAI
{
    shadow_demonAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint64 TargetGUID;

    void Reset()
    {
        TargetGUID = 0;
    }

    void Aggro(Unit *who)
    {
    }

    void JustDied(Unit *killer)
    {
        if(TargetGUID)
        {
            Unit* target = Unit::GetUnit((*m_creature), TargetGUID);
            if(target)
                target->RemoveAurasDueToSpell(SPELL_PARALYZE);
        }

    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) return;

        if(m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER) return; // Only cast the below on players.

        if(!m_creature->getVictim()->HasAura(SPELL_PARALYZE, 0))
        {
            TargetGUID = m_creature->getVictim()->GetGUID();
            m_creature->AddThreat(m_creature->getVictim(), 10000000.0f);
            DoCast(m_creature, SPELL_SHADOW_DEMON_PASSIVE, true);
            DoCast(m_creature->getVictim(), SPELL_PURPLE_BEAM, true);
            DoCast(m_creature->getVictim(), SPELL_PARALYZE, true);
        }
            // Kill our target if we're very close.
        if(m_creature->IsWithinDistInMap(m_creature->getVictim(), 3))
            DoCast(m_creature->getVictim(), SPELL_CONSUME_SOUL);
    }
};
struct MANGOS_DLL_DECL flamecrashAI : public ScriptedAI
{
    flamecrashAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 FlameCrashTimer;
    uint32 DespawnTimer;
   
    void Reset()
    {
        FlameCrashTimer = 3000 +rand()%5000;
        DespawnTimer = 60000;
    }
   
    void Aggro(Unit *who){ return; }

    void AttackStart(Unit *who) { return; }

    void MoveInLineOfSight(Unit *who){ return; }

    void UpdateAI(const uint32 diff)
    {
        if(FlameCrashTimer < diff)
        {
            DoCast(m_creature, SPELL_FLAME_CRASH_EFFECT);
            FlameCrashTimer = 15000;
        }else FlameCrashTimer -= diff;

        if(DespawnTimer < diff)
        {
            m_creature->SetVisibility(VISIBILITY_OFF); // So that players don't see the sparkly effect when we die.
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else DespawnTimer -= diff;
    }
};

struct MANGOS_DLL_DECL blazeAI : public ScriptedAI
{
    blazeAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 BlazeTimer;
    uint32 DespawnTimer;
      
    void Reset()
    {
        BlazeTimer = 2000;
        DespawnTimer = 15000;
    }

    void Aggro(Unit *who){ return; }

    void AttackStart(Unit* who) { return; }

    void MoveInLineOfSight(Unit *who){ return; }

    void UpdateAI(const uint32 diff)
    {
        if(BlazeTimer < diff)
        {
            DoCast(m_creature, SPELL_BLAZE_EFFECT);
            BlazeTimer = 15000;
        }else BlazeTimer -= diff;
        
        if(DespawnTimer < diff)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else DespawnTimer -= diff;
    }
};

struct MANGOS_DLL_DECL blade_of_azzinothAI : public ScriptedAI
{
    blade_of_azzinothAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    void Reset()
    {
    }

    // Do-Nothing-But-Stand-There.
    void Aggro(Unit* who) { return; }
    void AttackStart(Unit* who) { return; }
    void MoveInLineOfSight(Unit* who) { return; }
};

CreatureAI* GetAI_boss_illidan_stormrage(Creature *_Creature)
{
    return new boss_illidan_stormrageAI (_Creature);
}

CreatureAI* GetAI_npc_akama_at_illidan(Creature *_Creature)
{
    return new npc_akama_illidanAI (_Creature);
}

CreatureAI* GetAI_boss_maiev(Creature *_Creature)
{
    return new boss_maievAI (_Creature);
}

CreatureAI* GetAI_mob_flame_of_azzinoth(Creature *_Creature)
{
    return new flame_of_azzinothAI (_Creature);
}

CreatureAI* GetAI_cage_trap_trigger(Creature *_Creature)
{
    return new cage_trap_triggerAI (_Creature);
}

CreatureAI* GetAI_shadow_demon(Creature *_Creature)
{
    return new shadow_demonAI (_Creature);
}

CreatureAI* GetAI_flamecrash(Creature *_Creature)
{
    return new flamecrashAI (_Creature);
}

CreatureAI* GetAI_demonfire(Creature *_Creature)
{
    return new demonfireAI (_Creature);
}

CreatureAI* GetAI_blaze(Creature *_Creature)
{
    return new blazeAI (_Creature);
}

CreatureAI* GetAI_blade_of_azzinoth(Creature *_Creature)
{
    return new blade_of_azzinothAI (_Creature);
}

void AddSC_boss_illidan()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_illidan_stormrage";
    newscript->GetAI = GetAI_boss_illidan_stormrage;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "npc_akama_illidan";
    newscript->GetAI = GetAI_npc_akama_at_illidan;
    newscript->pGossipHello = GossipHello_npc_akama_at_illidan;
    newscript->pGossipSelect = GossipSelect_npc_akama_at_illidan;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "boss_maiev_shadowsong";
    newscript->GetAI = GetAI_boss_maiev;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_flame_of_azzinoth";
    newscript->GetAI = GetAI_mob_flame_of_azzinoth;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_blade_of_azzinoth";
    newscript->GetAI = GetAI_blade_of_azzinoth;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_cage_trap_trigger";
    newscript->GetAI = GetAI_cage_trap_trigger;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_shadow_demon";
    newscript->GetAI = GetAI_shadow_demon;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_flame_crash";
    newscript->GetAI = GetAI_flamecrash;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_demon_fire";
    newscript->GetAI = GetAI_demonfire;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_blaze";
    newscript->GetAI = GetAI_blaze;
    m_scripts[nrscripts++] = newscript;
}

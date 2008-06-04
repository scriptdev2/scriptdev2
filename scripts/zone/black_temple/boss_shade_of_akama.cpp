/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDComment: Ending Cinematic needs testing and completing
SDCategory: Black Temple
EndScriptData */

#include "def_black_temple.h"
#include "TargetedMovementGenerator.h"
#include "sc_gossip.h"
#include "PointMovementGenerator.h"

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

// Locations
#define Z1              118.543144
#define Z2              120.783768
#define Z_SPAWN         113.537949
#define AGGRO_X         482.793182
#define AGGRO_Y         401.270172
#define AGGRO_Z         112.783928

// Texts
#define SOUND_LOW_HEALTH            11386
#define SAY_LOW_HEALTH              "No! Not yet..."
#define SOUND_EVEN_LOWER_HEALTH     11385
#define SAY_EVEN_LOWER_HEALTH       "I will not last much longer..."

// Ending cinematic text
#define SAY_FREE                    "Come out from the shadows! I've returned to lead you against our true enemy! Shed your chains and raise your weapons against your Illidari masters!"
#define SAY_BROKEN_FREE_01          "Hail our leader! Hail Akama!"
#define SAY_BROKEN_FREE_02          "Hail Akama!"

// Gossips
#define GOSSIP_ITEM                 "We are ready to fight alongside you, Akama"

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

const uint32 spawnEntries[4]=
{
    23216,
    23523,
    23318,
    23524
};

struct MANGOS_DLL_DECL mob_ashtongue_channelerAI : public ScriptedAI
{
    mob_ashtongue_channelerAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;

    uint64 ShadeGUID;

    void Reset()
    {
        ShadeGUID = 0;
    }

    void JustDied(Unit* killer);

    void Aggro(Unit* who) {}

    void AttackStart(Unit* who) {}

    void MoveInLineOfSight(Unit* who)
    {        
        if(!ShadeGUID)
        {
            if(pInstance)
                ShadeGUID = pInstance->GetData64(DATA_SHADEOFAKAMA);
        }
    }

    void UpdateAI(const uint32 diff) {}
};

struct MANGOS_DLL_DECL mob_ashtongue_sorcererAI : public ScriptedAI
{
    mob_ashtongue_sorcererAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;
    uint64 ShadeGUID;

    bool StartBanishing;

    void Reset()
    {
        ShadeGUID = 0;

        StartBanishing = false;
    }

    void JustDied(Unit* killer);

    void Aggro(Unit* who) {}

    void AttackStart(Unit* who) {}

    void MoveInLineOfSight(Unit* who) {}

    void UpdateAI(const uint32 diff)
    {
        if(StartBanishing)
            return;

        if(ShadeGUID)
        {
            Unit* Shade = Unit::GetUnit((*m_creature), ShadeGUID);
            if(Shade && Shade->isAlive() && m_creature->isAlive())
            {
                if(m_creature->GetDistance2d(Shade) < 20)
                {
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->Idle();
                    DoCast(m_creature, SPELL_SHADE_SOUL_CHANNEL);

                    StartBanishing = true;
                }
            }
        }else
        {
            if(pInstance)
                ShadeGUID = pInstance->GetData64(DATA_SHADEOFAKAMA);
        }
    }
};


struct MANGOS_DLL_DECL boss_shade_of_akamaAI : public ScriptedAI
{
    boss_shade_of_akamaAI(Creature* c) : ScriptedAI(c)
    {
        AkamaGUID = 0;
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 ChannelerGUID[6];
    uint64 AkamaGUID;

    uint32 SorcererCount;
    uint32 DeathCount;

    uint32 ReduceHealthTimer;
    uint32 SummonTimer;
    uint32 ResetTimer;

    bool SummonedChannelers;
    bool IsBanished;
    bool HasKilledAkama;

    void Reset()
    {
        if(SummonedChannelers)
        {
            for(uint8 i = 0; i < 6; i++)
            {
                Creature* Channeler = NULL;
                Channeler = ((Creature*)Unit::GetUnit((*m_creature), ChannelerGUID[i]));
                if(Channeler && Channeler->isAlive())
                    Channeler->setDeathState(JUST_DIED);
                ChannelerGUID[i] = 0;
            }
            SummonedChannelers = false;
        }

        if(!AkamaGUID && pInstance)
            AkamaGUID = pInstance->GetData64(DATA_AKAMA_SHADE);

        SorcererCount = 0;
        DeathCount = 0;

        SummonTimer = 10000;
        ReduceHealthTimer = 0;
        ResetTimer = 60000;

        IsBanished = true;
        HasKilledAkama = false;
        
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->Idle();
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);

        if(pInstance)
            pInstance->SetData(DATA_SHADEOFAKAMAEVENT, NOT_STARTED);
    }

    void Aggro(Unit* who) { DoZoneInCombat(); }

    void AttackStart(Unit* who)
    {
        if(!who)
            return;
        
        if(who->isTargetableForAttack() && who != m_creature)
            DoStartAttackAndMovement(who);
    }

    void SummonCreature()
    {
        uint32 random = rand()%2;
        float X = SpawnLocations[random].x;
        float Y = SpawnLocations[random].y;
        if((rand()%5 == 0) && (DeathCount > 0) && (SorcererCount < 7)) // max of 6 sorcerers can be summoned
        {
            uint32 index;
            for(uint8 i = 0; i < 6; i++)
            {
                if(!ChannelerGUID[i])
                {
                    index = i;
                    break;
                }
            }
            Creature* Sorcerer = m_creature->SummonCreature(CREATURE_SORCERER, X, Y, Z_SPAWN, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            if(Sorcerer)
            {
                Sorcerer->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                Sorcerer->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()));
                Sorcerer->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                ChannelerGUID[index] = Sorcerer->GetGUID();
                DeathCount--;
                SorcererCount++;
            }
        }
        else
        {            
            uint32 entry = spawnEntries[rand()%4];
            Creature* Spawn = m_creature->SummonCreature(entry, X, Y, Z_SPAWN, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
            if(Spawn)
            {
                Spawn->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
                Spawn->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(0, AGGRO_X, AGGRO_Y, AGGRO_Z));
                DoZoneInCombat(Spawn);
            }
        }
    }

    void SetSelectableChannelers()
    {
        if(!SummonedChannelers) 
            return;

        for(uint8 i = 0; i < 6; i++)
        {
            Unit* Channeler = Unit::GetUnit((*m_creature), ChannelerGUID[i]);
            if(Channeler && Channeler->isAlive())
                Channeler->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
    }

    void SetAkamaGUID(uint64 guid) { AkamaGUID = guid; }

    void UpdateAI(const uint32 diff)
    {
        if(!SummonedChannelers)
        {
            for(uint8 i = 0; i < 6; i++)
            {
                ChannelerGUID[i] = 0;
                Creature* Channeler = NULL;
                float x = ChannelerLocations[i].x;
                float y = ChannelerLocations[i].y;
                float o = ChannelerLocations[i].o;
                Channeler = m_creature->SummonCreature(CREATURE_CHANNELER, x, y, m_creature->GetPositionZ(), o, TEMPSUMMON_DEAD_DESPAWN, 1000);
                if(Channeler && Channeler->isAlive())
                {
                    Channeler->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Channeler->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Channeler->CastSpell(Channeler, SPELL_SHADE_SOUL_CHANNEL, false);
                    ((mob_ashtongue_channelerAI*)Channeler->AI())->ShadeGUID = m_creature->GetGUID();
                    ChannelerGUID[i] = Channeler->GetGUID();
                }
            }
            SummonedChannelers = true;
        }

        if(!InCombat)
            return;

        if(IsBanished)
        {
            //if(!m_creature->GetMotionMaster()->empty())
            //{
            //    m_creature->GetMotionMaster()->Clear(false);
            //    m_creature->GetMotionMaster()->Idle();
            //}

            // Akama is set in the threatlist so when we reset, we make sure that he is not included in our check
            if(m_creature->getThreatManager().getThreatList().size() < 2)
                EnterEvadeMode();

            if(SummonTimer < diff)
            {
                // Nullifies any GUIDs to creatures that have despawned and/or died.
                for(uint8 i = 0; i < 6; i++)
                {
                    if(ChannelerGUID[i])
                    {
                        Unit* Channeler = Unit::GetUnit((*m_creature), ChannelerGUID[i]);
                        if(!Channeler)
                            ChannelerGUID[i] = 0;
                        else if(!Channeler->isAlive())
                            ChannelerGUID[i] = 0;
                    }
                }

                SummonCreature();
                SummonCreature();
                SummonTimer = 30000;
            }else SummonTimer -= diff;

            if(DeathCount >= 6)
            {
                for(uint8 i = 0; i < 6; i++)
                    ChannelerGUID[i] = 0;
                if(AkamaGUID)
                {
                    Unit* Akama = Unit::GetUnit((*m_creature), AkamaGUID);
                    if(Akama && Akama->isAlive())
                    {
                        IsBanished = false;
                        m_creature->GetMotionMaster()->Clear(false);
                        m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*Akama));
                        Akama->GetMotionMaster()->Clear(false);
                        Akama->GetMotionMaster()->Idle(); // Shade should move to Akama, not the other way around
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->AddThreat(Akama, 10000000.0f); // Crazy amount of threat
                        Akama->AddThreat(m_creature, 10000000.0f);
                        m_creature->Attack(Akama);
                        Akama->Attack(m_creature);
                    }
                }else
                {
                    if(pInstance)
                        AkamaGUID = pInstance->GetData64(DATA_AKAMA_SHADE);
                }
            }
        }
        else // No longer banished, let's fight Akama now
        {
            if(ReduceHealthTimer < diff)
            {
                if(AkamaGUID)
                {
                    Unit* Akama = Unit::GetUnit((*m_creature), AkamaGUID);
                    if(Akama && Akama->isAlive())
                    {
                        //10 % less health every few seconds.
                        m_creature->DealDamage(Akama, Akama->GetMaxHealth()/10, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        ReduceHealthTimer = 12000;
                    }
                    else
                    {
                        HasKilledAkama = true; // Akama is dead or missing, we stop fighting and disappear
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                        m_creature->RemoveAllAuras();
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    }
                }
                else
                {
                    if(pInstance)
                        AkamaGUID = pInstance->GetData64(DATA_AKAMA_SHADE);
                }
            }else ReduceHealthTimer -= diff;

            if(HasKilledAkama)
            {
                if(ResetTimer < diff)
                {
                    InCombat = false;
                    EnterEvadeMode(); // Reset a little while after killing Akama
                }
                else ResetTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    }
};

void mob_ashtongue_channelerAI::JustDied(Unit* killer)
{
    if(ShadeGUID)
    {
        Creature* Shade = ((Creature*)Unit::GetUnit((*m_creature), ShadeGUID));
        if(Shade && Shade->isAlive())
            ((boss_shade_of_akamaAI*)Shade->AI())->DeathCount++;
    }
}

void mob_ashtongue_sorcererAI::JustDied(Unit* killer)
{
    if(ShadeGUID)
    {
        Creature* Shade = ((Creature*)Unit::GetUnit((*m_creature), ShadeGUID));
        if(Shade && Shade->isAlive())
            ((boss_shade_of_akamaAI*)Shade->AI())->DeathCount++;
    }
}

struct MANGOS_DLL_DECL npc_akamaAI : public ScriptedAI
{
    npc_akamaAI(Creature* c) : ScriptedAI(c)
    {
        ShadeHasDied = false;
        StartCombat = false;
        Reset();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;

    uint64 ShadeGUID;

    uint32 DestructivePoisonTimer;
    uint32 LightningBoltTimer;
    uint32 CheckTimer;
    uint32 CastSoulRetrieveTimer;
    uint32 SoulRetrieveTimer;
    uint32 SummonBrokenTimer;
    uint32 EndingTalkCount;
    uint32 WayPointId;

    std::list<uint64> BrokenList;

    bool EventBegun;
    bool ShadeHasDied;
    bool StartCombat;
    bool HasYelledOnce;
    bool HasYelledTwice;

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

        BrokenList.clear();

        EventBegun = false;
        HasYelledOnce = false;
        HasYelledTwice = false;
        
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0); // Database sometimes has very very strange values
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void Aggro(Unit* who) {}

    void BeginEvent(Player* pl)
    {
        if(!pInstance)
            return;
        
        ShadeGUID = pInstance->GetData64(DATA_SHADEOFAKAMA);
        if(!ShadeGUID)
            return;

        Creature* Shade = ((Creature*)Unit::GetUnit((*m_creature), ShadeGUID));
        if(Shade)
        {
            pInstance->SetData(DATA_SHADEOFAKAMAEVENT, IN_PROGRESS);
            m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // Prevent players from trying to restart event
            ((boss_shade_of_akamaAI*)Shade->AI())->SetAkamaGUID(m_creature->GetGUID());
            ((boss_shade_of_akamaAI*)Shade->AI())->SetSelectableChannelers();
            ((boss_shade_of_akamaAI*)Shade->AI())->InCombat = true;
            Shade->AddThreat(m_creature, 1000000.0f);
            Shade->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
            Shade->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
            if(pl)
                Shade->AddThreat(pl, 1.0f);
            DoZoneInCombat(Shade);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type != POINT_MOTION_TYPE)
            return;

        switch(id)
        {
            case 0:
                WayPointId++;
                break;

            case 1:
                Unit* Shade = Unit::GetUnit((*m_creature), ShadeGUID);
                if(Shade)
                {
                    m_creature->SetUInt64Value(UNIT_FIELD_TARGET, ShadeGUID);
                    DoCast(Shade, SPELL_AKAMA_SOUL_RETRIEVE);
                    EndingTalkCount = 0;
                    SoulRetrieveTimer = 16000;
                    outstring_log("MovementInform 1, casting soul retrieve");
                }
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(ShadeGUID && !StartCombat)
        {
            Creature* Shade = ((Creature*)Unit::GetUnit((*m_creature), ShadeGUID));
            if(Shade && Shade->isAlive())
            {
                if(((boss_shade_of_akamaAI*)Shade->AI())->IsBanished)
                {
                    if(CastSoulRetrieveTimer < diff)
                    {
                        DoCast(Shade, SPELL_AKAMA_SOUL_CHANNEL);
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

        if(ShadeHasDied && (WayPointId == 1))
        {
            if(pInstance)
                pInstance->SetData(DATA_SHADEOFAKAMAEVENT, DONE);

            m_creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(WayPointId, AkamaWP[1].x, AkamaWP[1].y, AkamaWP[1].z));
            WayPointId++;
        }

        if(!ShadeHasDied && StartCombat)
        {
            if(CheckTimer < diff)
            {
                if(ShadeGUID)
                {
                    Unit* Shade = Unit::GetUnit((*m_creature), ShadeGUID);
                    if(Shade && !Shade->isAlive())
                    {
                        outstring_log("Shade is dead");
                        ShadeHasDied = true;
                        WayPointId = 0;
                        m_creature->GetMotionMaster()->Mutate(new PointMovementGenerator<Creature>(WayPointId, AkamaWP[0].x, AkamaWP[0].y, AkamaWP[0].z));
                    }
                }
                CheckTimer = 5000;
            }else CheckTimer -= diff;
        }

        if(SummonBrokenTimer)
            if(SummonBrokenTimer <= diff)
            {
                Creature* Broken = NULL; /* TODO: Summon Broken using coords here */
                if(Broken)
                    BrokenList.push_back(Broken->GetGUID());
                SummonBrokenTimer = 1000;
            }else SummonBrokenTimer -= diff;

        if(SoulRetrieveTimer)
            if(SoulRetrieveTimer <= diff)
            {
                outstring_log("Soul Retrieve block. Ending Talk Count = %d", EndingTalkCount);
                switch(EndingTalkCount)
                {
                    case 0:
                        DoYell(SAY_FREE, LANG_UNIVERSAL, NULL);
                        EndingTalkCount++;
                        SoulRetrieveTimer = 5000;
                        break;
                    case 1:
                        if(!BrokenList.empty())
                        {
                            bool Yelled = false;
                            for(std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                            {
                                Unit* pUnit = Unit::GetUnit((*m_creature), *itr);
                                if(pUnit)
                                {
                                    if(!Yelled)
                                    {
                                        pUnit->MonsterYell(SAY_BROKEN_FREE_01, LANG_UNIVERSAL, 0);
                                        Yelled = true;
                                    }
                                    pUnit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_KNEEL);
                                }
                            }
                        }
                        EndingTalkCount++;
                        SoulRetrieveTimer = 3000;
                        break;
                    case 2:
                        if(!BrokenList.empty())
                        {
                            for(std::list<uint64>::iterator itr = BrokenList.begin(); itr != BrokenList.end(); ++itr)
                            {
                                Unit* pUnit = Unit::GetUnit((*m_creature), *itr);
                                if(pUnit)
                                    pUnit->MonsterYell(SAY_BROKEN_FREE_02, LANG_UNIVERSAL, 0);
                            }
                        }
                        SoulRetrieveTimer = 0;
                        break;
                }
            }else SoulRetrieveTimer;

        if(!m_creature->getVictim() || !m_creature->SelectHostilTarget())
            return;

        if(DestructivePoisonTimer < diff)
        {
            // SPELL_DESTRUCTIVE_POISON is self-cast only for some reason so we make our target cast it on itself
            m_creature->getVictim()->CastSpell(m_creature->getVictim(), SPELL_DESTRUCTIVE_POISON, true);
            DestructivePoisonTimer = 15000;
        }else DestructivePoisonTimer -= diff;

        if(LightningBoltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_LIGHTNING_BOLT);
            LightningBoltTimer = 10000;
        }else LightningBoltTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_shade_of_akama(Creature *_Creature)
{
    return new boss_shade_of_akamaAI (_Creature);
}

CreatureAI* GetAI_mob_ashtongue_channeler(Creature *_Creature)
{
    return new mob_ashtongue_channelerAI (_Creature);
}

CreatureAI* GetAI_mob_ashtongue_sorcerer(Creature *_Creature)
{
    return new mob_ashtongue_sorcererAI (_Creature);
}

CreatureAI* GetAI_npc_akama_shade(Creature *_Creature)
{
    return new npc_akamaAI (_Creature);
}

bool GossipSelect_npc_akama(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Fight time
    {
        player->PlayerTalkClass->CloseGossip();
        ((npc_akamaAI*)_Creature->AI())->BeginEvent(player);
    }

    return true;
}

bool GossipHello_npc_akama(Player *player, Creature *_Creature)
{    
    if(player->isAlive())
    {
        player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->PlayerTalkClass->SendGossipMenu(907, _Creature->GetGUID());
    }

    return true;
}

void AddSC_boss_shade_of_akama()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_shade_of_akama";
    newscript->GetAI = GetAI_boss_shade_of_akama;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_ashtongue_channeler";
    newscript->GetAI = GetAI_mob_ashtongue_channeler;
    m_scripts[nrscripts++] = newscript;
    
    newscript = new Script;
    newscript->Name="mob_ashtongue_sorcerer";
    newscript->GetAI = GetAI_mob_ashtongue_sorcerer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_akama_shade";
    newscript->GetAI = GetAI_npc_akama_shade;
    newscript->pGossipHello = &GossipHello_npc_akama;
    newscript->pGossipSelect = &GossipSelect_npc_akama;
    m_scripts[nrscripts++] = newscript;
} 

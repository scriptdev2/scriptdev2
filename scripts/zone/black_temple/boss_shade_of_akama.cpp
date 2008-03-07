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
SD%Complete: 95
SDComment: Finish testing to see if Sorcerers work properly. Ending cinematic with Akama NYI
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"
#include "def_black_temple.h"
#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

struct Location
{
    float x, y, o;
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

// Locations
#define Z1              118.543144
#define Z2              120.783768

#define Z_SPAWN         113.537949

// Texts
#define SOUND_LOW_HEALTH            11386
#define SAY_LOW_HEALTH              "No! Not yet..."
#define SOUND_EVEN_LOWER_HEALTH     11385
#define SAY_EVEN_LOWER_HEALTH       "I will not last much longer..."

// Gossips
#define GOSSIP_ITEM                 "It is time to free your soul from Illidan's grasp!"

// Spells
#define SPELL_PASSIVE_SHADOWFORM    41913
#define SPELL_BEAM                  39123
#define SPELL_RED_BEAM              36295
#define SPELL_DESTRUCTIVE_POISON    40874
#define SPELL_LIGHTNING_BOLT        42024
#define SPELL_AKAMA_SOUL_EXPEL      40855
#define SPELL_AKAMA_SOUL_RETRIEVE   40447

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
        SetVariables();
    }

    uint64 ShadeGUID;

    bool InCombat;

    void SetVariables()
    {
        ShadeGUID = 0;
    }

    void EnterEvadeMode()
    {
        SetVariables();
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void SetShadeGUID(uint64 guid)
    {
        ShadeGUID = guid;
    }

    void JustDied(Unit* killer);

    void AttackStart(Unit* who)
    {
        if(!InCombat) InCombat = true;
    }

    void MoveInLineOfSight(Unit* who)
    {
        if(!InCombat) InCombat = true;
        
        if(ShadeGUID)
        {
            Unit* Shade = Unit::GetUnit((*m_creature), ShadeGUID);
            if(Shade)
                DoCast(Shade, SPELL_BEAM);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(InCombat)
            m_creature->SetInCombat();
    }
};

struct MANGOS_DLL_DECL mob_ashtongue_sorcererAI : public ScriptedAI
{
    mob_ashtongue_sorcererAI(Creature* c) : ScriptedAI(c)
    {
        SetVariables();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;
    uint64 ShadeGUID;

    bool StartBanishing;
    bool InCombat;

    void SetVariables()
    {
        ShadeGUID = 0;

        StartBanishing = false;
    }

    void EnterEvadeMode()
    {
        SetVariables();
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit* killer);

    void AttackStart(Unit* who)
    {
        if(!who) return;
           
        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            
            if(!InCombat) InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if(!InCombat) InCombat = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if(InCombat)
            m_creature->SetInCombat();
        
        if(ShadeGUID)
        {
            Unit* Shade = Unit::GetUnit((*m_creature), ShadeGUID);
            if(Shade)
            {
                if(m_creature->GetDistance2d(Shade) < 20)
                {
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->Idle();
                    DoCast(Shade, SPELL_BEAM);
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
        SetVariables();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;

    uint64 ChannelerGUID[6];
    uint64 AkamaGUID;

    uint32 DeathCount;

    uint32 ReduceHealthTimer;
    uint32 EnrageTimer;
    uint32 SummonTimer;
    uint32 ResetTimer;

    bool Enrage;
    bool SummonedChannelers;
    bool IsBanished;
    bool InCombat;
    bool HasKilledAkama;

    void SetVariables()
    {
        if(SummonedChannelers)
        {
            for(uint8 i = 0; i < 6; i++)
            {
                Creature* Channeler = NULL;
                Channeler = ((Creature*)Unit::GetUnit((*m_creature), ChannelerGUID[i]));
                if(Channeler && Channeler->isAlive())
                {
                    Channeler->SetVisibility(VISIBILITY_OFF);
                    Channeler->DealDamage(Channeler, Channeler->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
                }
                ChannelerGUID[i] = 0;
            }
            SummonedChannelers = false;
        }

        AkamaGUID = 0;

        DeathCount = 0;

        EnrageTimer = 600000;
        SummonTimer = 20000;
        ReduceHealthTimer = 0;
        ResetTimer = 60000;

        Enrage = false;
        IsBanished = true;
        InCombat = false;
        HasKilledAkama = false;

        
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->Idle();
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->DeleteThreatList();
        m_creature->RemoveAllAuras();
        DoCast(m_creature, SPELL_PASSIVE_SHADOWFORM);
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit* who)
    {
        if(!who || IsBanished)
            return;
        
        if(who->isTargetableForAttack() && who != m_creature)
                DoStartMeleeAttack(who);
    }

    void MoveInLineOfSight(Unit* who)
    {
        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);
            }
        }
    }

    void SummonCreature()
    {
        uint32 entry = spawnEntries[rand()%4];
        uint32 random = rand()%2;
        float X = SpawnLocations[random].x;
        float Y = SpawnLocations[random].y;
        if((rand()%15 == 0) && DeathCount > 0)
        {
            Creature* Sorcerer = m_creature->SummonCreature(CREATURE_SORCERER, X, Y, Z_SPAWN, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            if(Sorcerer)
            {
                Sorcerer->AddThreat(m_creature, 100000000.0f);
                Sorcerer->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                for(uint8 i = 0; i < 6; i++)
                {
                    if(!ChannelerGUID[i])
                    {
                        ChannelerGUID[i] = Sorcerer->GetGUID();
                        break;
                    }
                }
                DeathCount--;
            }
        }
        else
        {
            Creature* Spawn = m_creature->SummonCreature(entry, X, Y, Z_SPAWN, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
            if(Spawn)
                Spawn->AddThreat(SelectUnit(SELECT_TARGET_RANDOM, 0), 1.0f);
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

    void SetAkamaGUID(uint64 guid)
    {
        if(guid)
            AkamaGUID = guid;
    }

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
                if(Channeler)
                {
                    Channeler->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Channeler->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Channeler->CastSpell(m_creature, SPELL_BEAM, false);
                    ((mob_ashtongue_channelerAI*)Channeler->AI())->SetShadeGUID(m_creature->GetGUID());
                    ChannelerGUID[i] = Channeler->GetGUID();
                }
            }
            if(!m_creature->HasAura(SPELL_PASSIVE_SHADOWFORM, 0)) DoCast(m_creature, SPELL_PASSIVE_SHADOWFORM, true);
            SummonedChannelers = true;
        }

        if(!InCombat)
            return;

        if(m_creature->getThreatManager().getThreatList().empty())
            EnterEvadeMode();

        if(IsBanished)
        {
            if(!m_creature->GetMotionMaster()->empty())
            {
                m_creature->GetMotionMaster()->Clear(false);
                m_creature->GetMotionMaster()->Idle();
            }

            if(SummonTimer < diff)
            {
                SummonCreature();
                SummonCreature();
                SummonTimer = Enrage ? 10000 : 15000; // Spawn them faster if enraged
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
                        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->AddThreat(Akama, 1000000.0f); // Crazy amount of threat
                        Akama->AddThreat(m_creature, 1000000.0f);
                        m_creature->Attack(Akama);
                        Akama->Attack(m_creature);
                    }
                }else
                {
                    if(pInstance)
                        AkamaGUID = pInstance->GetData64(DATA_AKAMA_SHADE);
                }
            }

            // Only run this if we are not enraged
            if(!Enrage)
            {
                if(EnrageTimer < diff)
                    Enrage = true;
                else EnrageTimer -= diff;
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
                        m_creature->DealDamage(Akama, Akama->GetMaxHealth()/10, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
                        ReduceHealthTimer = Enrage ? 5000 : 12000; // Reduce health more rapidly if enraged
                    }
                    else
                    {
                        HasKilledAkama = true; // Akama is dead or missing, we stop fighting and disappear
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        DoGoHome();
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                        m_creature->RemoveAllAuras();
                        DoCast(m_creature, SPELL_PASSIVE_SHADOWFORM);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    }
                }
                else // Debug
                {
                    if(pInstance)
                        AkamaGUID = pInstance->GetData64(DATA_AKAMA_SHADE);
                }
            }else ReduceHealthTimer -= diff;

            if(HasKilledAkama)
            {
                if(ResetTimer < diff)
                    EnterEvadeMode(); // Reset a little while after killing Akama
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
        SetVariables();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    ScriptedInstance* pInstance;

    uint64 ShadeGUID;

    uint32 DestructivePoisonTimer;
    uint32 LightningBoltTimer;
    uint32 CheckTimer;
    uint32 CastSoulRetrieveTimer;

    bool InCombat;
    bool EventBegun;
    bool ShadeHasDied;
    bool StartCombat;
    bool HasYelledOnce;
    bool HasYelledTwice;

    void SetVariables()
    {
        DestructivePoisonTimer = 5000;
        LightningBoltTimer = 10000;
        CheckTimer = 2000;
        CastSoulRetrieveTimer = 0;

        InCombat = false;
        EventBegun = false;
        ShadeHasDied = false;
        StartCombat = false;
        HasYelledOnce = false;
        HasYelledTwice = false;
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit* who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!=m_creature)
            DoStartMeleeAttack(who);
    }

    void MoveInLineOfSight(Unit* who)
    {
        if(!who)
            return;
        
        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
        }
    }

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
            pInstance->SetData(DATA_SHADEOFAKAMAEVENT, 1); // In Progress
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0); // Prevent players from trying to restart event
            ((boss_shade_of_akamaAI*)Shade->AI())->SetAkamaGUID(m_creature->GetGUID());
            ((boss_shade_of_akamaAI*)Shade->AI())->InCombat = true;
            ((boss_shade_of_akamaAI*)Shade->AI())->SetSelectableChannelers();
            if(pl)
                Shade->AddThreat(pl, 1.0f);
            // DoCast(Shade, SPELL_AKAMA_SOUL_EXPEL);
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
                        DoCast(Shade, SPELL_AKAMA_SOUL_RETRIEVE);
                        CastSoulRetrieveTimer = 500;
                    }else CastSoulRetrieveTimer -= diff;
                }
                else
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                    StartCombat = true;
                }
            }
        }

        if(ShadeHasDied)
        {
            //TODO: Cosmetics for Shade's death
        }

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

        if(!ShadeHasDied)
        {
            if(CheckTimer < diff)
            {
                if(ShadeGUID)
                {
                    Unit* Shade = Unit::GetUnit((*m_creature), ShadeGUID);
                    if(Shade && (!Shade->isAlive()))
                        ShadeHasDied = true;
                }
            }else CheckTimer -= diff;
        }

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

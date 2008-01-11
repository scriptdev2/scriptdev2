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
SDName: Boss_Shade_of_Akama
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../shared/WorldPacket.h"
#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

// Locations
#define Z2                120.783768
#define Z1                118.543144

#define X_SHADE           451.347534
#define Y_SHADE           401.219757

#define X_CHANNELER1    463.161285
#define Y_CHANNELER1    401.219757
#define O_CHANNELER1    3.141592

#define X_CHANNELER2    457.377625
#define Y_CHANNELER2    391.227661
#define O_CHANNELER2    2.106461

#define X_CHANNELER3    446.012421
#define Y_CHANNELER3    391.227661
#define O_CHANNELER3    1.071904

#define X_CHANNELER4     439.533783
#define Y_CHANNELER4     401.219757
#define O_CHANNELER4     0.000000

#define X_CHANNELER5     446.012421
#define Y_CHANNELER5     411.211853
#define O_CHANNELER5     5.210546

#define X_CHANNELER6    457.377625
#define Y_CHANNELER6    411.211853
#define O_CHANNELER6    4.177494
/*
#define X_SPAWN1        494.007721
#define Y_SPAWN1        443.606689

#define X_SPAWN2        494.007721
#define Y_SPAWN2        359.258453      

#define X_MOVE1          451.347534
#define Y_MOVE1          419.211853

#define X_MOVE2          451.347534
#define Y_MOVE2          383.227661

#define Z_SPAWN               118.537949
#define X_SPAWN1          444.638519
#define Y_SPAWN1          376.335632

#define X_SPAWN2          450.421814
#define Y_SPAWN2          426.895935
*/

#define X_SPAWN               450.2456
#define Y_SPAWN               118.5378
#define Z_SPAWN               119.1213
#define X_AKAMA         466.161285
#define Y_AKAMA         401.219757

#define SOUND_LOW_HEALTH        11386
#define SAY_LOW_HEALTH          "No! Not yet..."

#define SOUND_EVEN_LOWER_HEALTH        11385
#define SAY_EVEN_LOWER_HEALTH        "I will not last much longer..."

#define GOSSIP_ITEM                    "It is time to free your soul from Illidan's grasp!"
// Spells
#define SPELL_PASSIVE_SHADOWFORM    41913

#define SPELL_BEAM                  39123
#define SPELL_RED_BEAM                36295
#define SPELL_ROOT_SELF                    42716

#define SPELL_DESTRUCTIVE_POISON    40874
#define SPELL_LIGHTNING_BOLT        42024

struct MANGOS_DLL_DECL boss_shade_of_akamaAI : public ScriptedAI
{
    boss_shade_of_akamaAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        for(uint8 i = 0; i<6; i++)
        {
            ChannelerGUID[i] = 0;
        }
        AkamaGUID = 0;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint64 ChannelerGUID[6];
    uint64 AkamaGUID;

    uint32 CheckTimer;
    uint32 IdleTimer;
    uint32 SummonTimer;
    uint32 ReduceHealthTimer;
    uint32 EnrageTimer;
    uint32 Multiplier;
    uint32 DeathCount;
    uint32 ResetTimer;

    bool AkamaDied;
    bool Channelers;
    bool RightEntrance;
    bool Active;
    bool InCombat;

    void SetVariables()
    {
        Active = false;
        RightEntrance = true;
        Channelers = false;
        AkamaDied = false;
        InCombat = false;

        ResetTimer = 0;
        CheckTimer = 2000;
        IdleTimer = 2000;
        SummonTimer = 5000;
        EnrageTimer = 600000;
        ReduceHealthTimer = 0;
        DeathCount = 0;
        Multiplier = 1;

        (*m_creature).GetMotionMaster()->Clear(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void SummonCreature(uint32 creatureId)
    {
        Creature* Summoned;

        Summoned = m_creature->SummonCreature(creatureId, X_SPAWN, Y_SPAWN, Z_SPAWN, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000);
        if(Summoned)
            Summoned->AddThreat(SelectUnit(SELECT_TARGET_RANDOM, 0), 1.0f);
        /*if (RightEntrance)
        {
        Summoned = m_creature->SummonCreature(creatureId, X_SPAWN1, Y_SPAWN1, Z2, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
        RightEntrance = false;
        if(Summoned)
        Summoned->AddThreat(SelectUnit(SELECT_TARGET_RANDOM, 0), 1.0f);
        }
        else
        {
        Summoned = m_creature->SummonCreature(creatureId, X_SPAWN2, Y_SPAWN2, Z2, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
        RightEntrance = true;
        if(Summoned)
        Summoned->AddThreat(SelectUnit(SELECT_TARGET_RANDOM, 0), 1.0f);
        }*/
    }

    bool GetBanishedState()
    {
        if(Active)
            return false;
        else
            return true;
    }

    void SetAkamaGUID(uint64 guid)
    {
        if(guid)
            AkamaGUID = guid;
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData("ShadeOfAkamaEvent", 3); // Completed
    }

    void AttackStart(Unit *who)
    {
        if((!who) || (!Active))
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            if(Active)
                DoStartMeleeAttack(who);

            if(!InCombat)
            {
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {/*
     if(!InCombat)
     {
     InCombat = true;
     SetChannelersSelectable();
     }*/
    }

    void SetInCombat(bool com)
    {
        InCombat = com;
    }

    void SetChannelersSelectable()
    {
        if(!Channelers)
            return;

        for(uint8 i = 0; i < 7; i++)
        {
            if(ChannelerGUID[i])
            {
                Unit* Channeler = NULL;
                Channeler = Unit::GetUnit((*m_creature), ChannelerGUID[i]);
                if(Channeler && Channeler->isAlive())
                    Channeler->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        // Summon Channelers, banish Shade of Akama
        if(!Channelers)
        {
            DoCast(m_creature, SPELL_PASSIVE_SHADOWFORM);

            Creature* Channeler1 = NULL;
            Creature* Channeler2 = NULL;
            Creature* Channeler3 = NULL;
            Creature* Channeler4 = NULL;
            Creature* Channeler5 = NULL;
            Creature* Channeler6 = NULL;

            Channeler1 = m_creature->SummonCreature(23421, X_CHANNELER1, Y_CHANNELER1, Z1, O_CHANNELER1, TEMPSUMMON_DEAD_DESPAWN, 0);
            Channeler2 = m_creature->SummonCreature(23421, X_CHANNELER2, Y_CHANNELER2, Z1, O_CHANNELER2, TEMPSUMMON_DEAD_DESPAWN, 0);
            Channeler3 = m_creature->SummonCreature(23421, X_CHANNELER3, Y_CHANNELER3, Z1, O_CHANNELER3, TEMPSUMMON_DEAD_DESPAWN, 0);
            Channeler4 = m_creature->SummonCreature(23421, X_CHANNELER4, Y_CHANNELER4, Z1, O_CHANNELER4, TEMPSUMMON_DEAD_DESPAWN, 0);
            Channeler5 = m_creature->SummonCreature(23421, X_CHANNELER5, Y_CHANNELER5, Z1, O_CHANNELER5, TEMPSUMMON_DEAD_DESPAWN, 0);
            Channeler6 = m_creature->SummonCreature(23421, X_CHANNELER6, Y_CHANNELER6, Z1, O_CHANNELER6, TEMPSUMMON_DEAD_DESPAWN, 0);

            if(Channeler1)
            {
                Channeler1->CastSpell(m_creature, SPELL_BEAM, false);
                Channeler1->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ChannelerGUID[0] = Channeler1->GetGUID();
            }
            if(Channeler2)
            {
                Channeler2->CastSpell(m_creature, SPELL_BEAM, false);
                Channeler2->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ChannelerGUID[1] = Channeler2->GetGUID();
            }
            if(Channeler3)
            {
                Channeler3->CastSpell(m_creature, SPELL_BEAM, false);
                Channeler3->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ChannelerGUID[2] = Channeler3->GetGUID();
            }
            if(Channeler4)
            {
                Channeler4->CastSpell(m_creature, SPELL_BEAM, false);
                Channeler4->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ChannelerGUID[3] = Channeler4->GetGUID();
            }
            if(Channeler5)
            {
                Channeler5->CastSpell(m_creature, SPELL_BEAM, false);
                Channeler5->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ChannelerGUID[4] = Channeler5->GetGUID();
            }
            if(Channeler6)
            {
                Channeler6->CastSpell(m_creature, SPELL_BEAM, false);
                Channeler6->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ChannelerGUID[5] = Channeler6->GetGUID();
            }

            Channelers = true;
        }

        if(!InCombat)
            return;

        if(m_creature->isAlive() && (Channelers))
        {           
            // Phase 1
            if(!Active)
            {
                if(IdleTimer < diff)
                {
                    (*m_creature).GetMotionMaster()->Clear(false);
                    (*m_creature).GetMotionMaster()->Idle();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    IdleTimer = 1000;
                }else IdleTimer -= diff;

                // Periodically summon 2 adds. Less cooldown if enraged.
                if(SummonTimer < diff)
                {
                    switch(rand()%4)
                    {
                    case 0:
                        SummonCreature(23216);
                        break;
                    case 1:
                        SummonCreature(23523);
                        break;
                    case 2:
                        SummonCreature(23318);
                        break;
                    case 3:
                        SummonCreature(23524);
                        break;
                    }

                    switch(rand()%4)
                    {
                    case 0:
                        SummonCreature(23216);
                        break;
                    case 1:
                        SummonCreature(23523);
                        break;
                    case 2:
                        SummonCreature(23318);
                        break;
                    case 3:
                        SummonCreature(23524);
                        break;
                    }
                    SummonTimer = (15000 / Multiplier);
                }else SummonTimer -= diff;

                // If Enraged, make adds spawn more rapidly
                if(EnrageTimer < diff)
                {
                    EnrageTimer = 600000;
                    Multiplier++;
                }else EnrageTimer -= diff;

                if(CheckTimer < diff)
                {
                    for(uint8 i = 0; i<7; i++)
                    {
                        if(ChannelerGUID[i])
                        {
                            Unit* Channeler;
                            Channeler = Unit::GetUnit((*m_creature), ChannelerGUID[i]);
                            if(Channeler && (!Channeler->isAlive()))
                            {
                                DeathCount++;
                                ChannelerGUID[i] = 0;
                            }
                        }
                    }
                    CheckTimer = 2000;
                }else CheckTimer -= diff;

                // If Channelers dead, phase 2;
                if(DeathCount >= 6 && (!Active))
                {
                    m_creature->RemoveAllAuras();
                    (*m_creature).GetMotionMaster()->Clear(false);
                    DoCast(m_creature, SPELL_PASSIVE_SHADOWFORM);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Active = true;
                }
            }

            // Phase 2
            if(Active)
            {
                // -10% health for Akama  
                if(AkamaGUID)
                {
                    Unit* Akama = NULL;
                    Akama = Unit::GetUnit((*m_creature), AkamaGUID);
                    if(Akama && Akama->isAlive())
                    {
                        if(ReduceHealthTimer < diff)
                        {
                            m_creature->AddThreat(Akama, 5000000.0f);
                            Akama->AddThreat(Akama, 5000000.0f);
                            if (Multiplier < 10)
                            {
                                Akama->SetHealth((10 - Multiplier) * 0.1 * Akama->GetMaxHealth());
                                Multiplier++;
                                ReduceHealthTimer = 12000;
                            }else m_creature->DealDamage(Akama, Akama->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
                        }else ReduceHealthTimer -= diff;
                    }else
                    {
                        AkamaDied = true;
                        m_creature->SetHealth(m_creature->GetMaxHealth());
                        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        ResetTimer = 60000;
                    }

                    if (AkamaDied)
                    {
                        m_creature->RemoveAllAuras();
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);

                        if(ResetTimer < diff)
                        {
                            EnterEvadeMode();
                        }else ResetTimer -= diff;
                    }
                }
                DoMeleeAttackIfReady();
            }
        }
    }
};


struct MANGOS_DLL_DECL npc_ashchannelerAI : public ScriptedAI
{
    npc_ashchannelerAI(Creature *c) : ScriptedAI(c) {}

    bool InCombat;

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!InCombat) InCombat = true;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if(!InCombat) InCombat = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if(InCombat) m_creature->SetInCombat();
    }
};

struct MANGOS_DLL_DECL npc_akamaAI : public ScriptedAI
{
    npc_akamaAI(Creature *c) : ScriptedAI(c) 
    {/*
     if(c->GetInstanceData())
     pInstance = ((ScriptedInstance*)m_creature->GetInstanceData());
     else pInstance = NULL;*/
        SetVariables();
    }

    //ScriptedInstance* pInstance;

    uint64 ShadeGUID;

    uint32 PoisonTimer;
    uint32 LightningTimer;
    uint32 AttackStartTimer;

    bool Yell1;
    bool Yell2;
    bool InCombat;
    bool IsFighting;

    void SetVariables()
    {
        ShadeGUID = 0;

        PoisonTimer = 15000;
        LightningTimer = 25000;
        AttackStartTimer = 6000;

        Yell1 = false;
        Yell2 = false;
        IsFighting = false;
        InCombat = false;

        /*   if(pInstance)
        {
        pInstance->SetData("ShadeOfAkamaEvent", 0); // Not Started
        }*/

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit *killer)
    {
        /*if(pInstance)
        pInstance->SetData("ShadeOfAkamaEvent", 2); // Failed*/
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit* who)
    {        
        if (!who || m_creature->getVictim())
            return;

        if(!ShadeGUID)
        {
            Creature* Shade = DoSpawnCreature(22841, X_SHADE, Y_SHADE, Z1, 0, TEMPSUMMON_DEAD_DESPAWN, 30000);
            if(Shade)
                ShadeGUID = Shade->GetGUID();
        }

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    InCombat = true;
                }

                DoStartMeleeAttack(who);
            }
        }
    }
    void BeginEvent()
    {
        /*          if(!pInstance)
        return;

        ShadeGUID = pInstance->GetData64("ShadeOfAkama");*/
        if(!ShadeGUID) return;

        Creature* Shade = ((Creature*)Unit::GetUnit((*m_creature), ShadeGUID));
        if(Shade)
        {
            //pInstance->SetData("ShadeOfAkamaEvent", 1); // In Progress
            ShadeGUID = Shade->GetGUID();
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
            ((boss_shade_of_akamaAI*)Shade->AI())->SetInCombat(true);
            ((boss_shade_of_akamaAI*)Shade->AI())->SetAkamaGUID(m_creature->GetGUID());
            ((boss_shade_of_akamaAI*)Shade->AI())->SetChannelersSelectable();
            (*Shade).GetMotionMaster()->Clear(false);
            (*Shade).GetMotionMaster()->Idle();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(ShadeGUID)
        {
            Creature* Shade = NULL;
            Shade = (Creature*)(Unit::GetUnit((*m_creature), ShadeGUID));

            if(Shade && Shade->isAlive())
            {
                if(!((boss_shade_of_akamaAI*)Shade->AI())->GetBanishedState())
                {
                    if(!IsFighting)
                    {
                        (*Shade).GetMotionMaster()->Clear(false);
                        (*Shade).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature)); 
                        m_creature->AddThreat(Shade, 500000.0f);
                        Shade->AddThreat(m_creature, 500000.0f);
                        AttackStart(Shade);
                        Shade->AI()->AttackStart(m_creature);
                        AttackStartTimer = 6000;
                        IsFighting = true;
                    }

                    if(AttackStartTimer > 0)
                    {
                        if(AttackStartTimer <= diff)
                        {
                            m_creature->InterruptNonMeleeSpells(false);
                            m_creature->AddThreat(Shade, 500000.0f);
                            Shade->AddThreat(m_creature, 500000.0f);
                            AttackStart(Shade);
                            Shade->AI()->AttackStart(m_creature);
                            AttackStartTimer = 0;
                        }else AttackStartTimer -= diff;
                    }
                }
            }else ShadeGUID = 0;
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
        /*            if(PoisonTimer < diff)
        {
        m_creature->getVictim()->CastSpell(m_creature->getVictim(), SPELL_DESTRUCTIVE_POISON, true);
        PoisonTimer = 20000;
        }else PoisonTimer -= diff;*/

        if(LightningTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_LIGHTNING_BOLT);
            LightningTimer = 15000;
        }else LightningTimer -= diff;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 21) && (!Yell1))
        {
            DoYell(SAY_LOW_HEALTH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_LOW_HEALTH);
            Yell1 = true;
        }

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 11) && (!Yell2))
        {
            DoYell(SAY_EVEN_LOWER_HEALTH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_EVEN_LOWER_HEALTH);
            Yell2 = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_shade_of_akama(Creature *_Creature)
{
    return new boss_shade_of_akamaAI (_Creature);
}

CreatureAI* GetAI_npc_ashchanneler(Creature *_Creature)
{
    return new npc_ashchannelerAI (_Creature);
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
        ((npc_akamaAI*)_Creature->AI())->BeginEvent();
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
    newscript->Name="npc_ashchanneler";
    newscript->GetAI = GetAI_npc_ashchanneler;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_akama_shade";
    newscript->GetAI = GetAI_npc_akama_shade;
    newscript->pGossipHello = &GossipHello_npc_akama;
    newscript->pGossipSelect = &GossipSelect_npc_akama;
    m_scripts[nrscripts++] = newscript;
} 

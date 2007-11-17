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

#include "../../sc_defines.h"

#define SOUND_AGGRO              11367
#define SOUND_ENRAGE             11368
#define SOUND_OGRE_DEATH1        11369
#define SOUND_OGRE_DEATH2        11370
#define SOUND_OGRE_DEATH3        11371
#define SOUND_OGRE_DEATH4        11372
#define SOUND_SLAY1              11373
#define SOUND_SLAY2              11374
#define SOUND_SLAY3              11375
#define SOUND_DEATH              11376

#define SPELL_ARCING_SMASH       38761
#define SPELL_MIGHTY_BLOW        33230
#define SPELL_WHIRLWIND          33239
#define SPELL_ENRAGE             34970     

//High King Maulgar AI
struct MANGOS_DLL_DECL boss_high_king_maulgarAI : public ScriptedAI
{
    boss_high_king_maulgarAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        Council[3] = 0;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 ArcingSmash_Timer;
    uint32 MightyBlow_Timer;
    uint32 Whirlwind_Timer;
    uint32 Charging_Timer;

    bool Phase2;
    bool InCombat;

    uint64 Council[4];

    void EnterEvadeMode()
    {       
        ArcingSmash_Timer = 10000;
        MightyBlow_Timer = 40000;
        Whirlwind_Timer = 30000;
        Charging_Timer = 0;
        Phase2 = false;

        //respawn if died
        if(InCombat)
        {
            Creature *pCreature;
            for(uint8 i = 0; i < 4; i++)
            {
                pCreature = (Creature*)(Unit::GetUnit((*m_creature), Council[i]));
                if(pCreature)
                {
                    pCreature->Respawn();
                    pCreature->AI()->EnterEvadeMode();
                }
            }
        }

        //reset encounter
        if (pInstance)
            pInstance->SetData("MaulgarEvent", 0);

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void KilledUnit()
    {
        switch(rand()%2)
        {
        case 0:
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;

        case 1:
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            break;

        case 2:
            DoPlaySoundToSet(m_creature, SOUND_SLAY3);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if (pInstance)
            pInstance->SetData("MaulgarEvent", 0);
    }

    void AttackStart(Unit *who)
    {
        if (!who || !pInstance)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            if(!InCombat)
                StartEvent(who);
        }
    }

    void GetCouncil()
    {
        //get council member's guid to respawn them if needed
        Council[0] = pInstance->GetData64("KigglerTheCrazed");
        Council[1] = pInstance->GetData64("BlindeyeTheSeer");
        Council[2] = pInstance->GetData64("OlmTheSummoner");
        Council[3] = pInstance->GetData64("KroshFirehand");
    }

    void StartEvent(Unit *who)
    {
        if(!pInstance)
            return;

        GetCouncil();

        InCombat = true;

        pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
        pInstance->SetData("MaulgarEvent", 1);

        DoPlaySoundToSet(m_creature, SOUND_AGGRO);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || !pInstance)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if(!InCombat)
                    StartEvent(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if(!InCombat && pInstance && pInstance->GetData("MaulgarEvent"))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64("MaulgarEvent_Tank"));

            if(target)
            {
                DoStartMeleeAttack(target);
                InCombat = true;

                GetCouncil();

                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if(pInstance && !pInstance->GetData("MaulgarEvent"))
            EnterEvadeMode();

        //ArcingSmash_Timer
        if (ArcingSmash_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCING_SMASH);
            ArcingSmash_Timer = 20000;
        }else ArcingSmash_Timer -= diff;

        //Whirlwind_Timer
        if (Whirlwind_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_WHIRLWIND);
            Whirlwind_Timer = 30000;
        }else Whirlwind_Timer -= diff;

        //MightyBlow_Timer
        if (MightyBlow_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            MightyBlow_Timer = 40000;
        }else MightyBlow_Timer -= diff;

        //Entering Phase 2
        if(!Phase2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            Phase2 = true;
            DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
        }

        if(Phase2)
        {
            //Charging_Timer
            if(Charging_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                    DoStartMeleeAttack(target);

                Charging_Timer = 20000;
            }else Charging_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

//Olm The Summoner AI
struct MANGOS_DLL_DECL boss_olm_the_summonerAI : public ScriptedAI
{
    boss_olm_the_summonerAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    uint32 DarkDecay_Timer;
    uint32 Summon_Timer;

    ScriptedInstance* pInstance; 

    bool InCombat;

    void EnterEvadeMode()
    {
        DarkDecay_Timer = 10000;
        Summon_Timer = 15000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //reset encounter
        if (pInstance)
            pInstance->SetData("MaulgarEvent", 0);

        InCombat = false;
    }

    void AttackStart(Unit *who)
    {
        if (!who || !pInstance)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            if(!InCombat)
            {
                InCombat = true;
                DoStartMeleeAttack(who);
                pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                pInstance->SetData("MaulgarEvent", 1);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || !pInstance)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if(!InCombat)
                {
                    InCombat = true;
                    DoStartMeleeAttack(who);
                    pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                    pInstance->SetData("MaulgarEvent", 1);
                }
            }
        }
    }

    float DoCalculateRandomLocation()
    {
        float Loc;
        float Rand = rand()%8;

        switch(rand()%2)
        {
        case 0:
            Loc = 0 + Rand;
            break;

        case 1:
            Loc = 0 - Rand;
            break;
        }
        return Loc;
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if(!InCombat && pInstance && pInstance->GetData("MaulgarEvent"))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64("MaulgarEvent_Tank"));

            if(target)
            {
                DoStartMeleeAttack(target);
                InCombat = true;
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if(pInstance && !pInstance->GetData("MaulgarEvent"))
            EnterEvadeMode();

        //DarkDecay_Timer
        if(DarkDecay_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 33129);
            DarkDecay_Timer = 20000;
        }else DarkDecay_Timer -= diff;

        //Summon_Timer
        if(Summon_Timer < diff)
        {
            Creature *Add = NULL;
            Add = DoSpawnCreature(18847, DoCalculateRandomLocation(), DoCalculateRandomLocation(), 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
            Summon_Timer = 30000;
        }else Summon_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Kiggler The Crazed AI
struct MANGOS_DLL_DECL boss_kiggler_the_crazedAI : public ScriptedAI
{
    boss_kiggler_the_crazedAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    uint32 GreatherPolymorph_Timer;
    uint32 LightningBolt_Timer;
    uint32 ArcaneShock_Timer;
    uint32 ArcaneExplosion_Timer;

    ScriptedInstance* pInstance; 

    bool InCombat;

    void EnterEvadeMode()
    {
        GreatherPolymorph_Timer = 5000;
        LightningBolt_Timer = 10000;
        ArcaneShock_Timer = 20000;
        ArcaneExplosion_Timer = 30000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //reset encounter
        if (pInstance)
            pInstance->SetData("MaulgarEvent", 0);

        InCombat = false;
    }

    void AttackStart(Unit *who)
    {
        if (!who || !pInstance)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if(!InCombat)
            {
                InCombat = true;
                DoStartMeleeAttack(who);
                pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                pInstance->SetData("MaulgarEvent", 1);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || !pInstance)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(!InCombat)
                {
                    InCombat = true;
                    DoStartMeleeAttack(who);
                    pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                    pInstance->SetData("MaulgarEvent", 1);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if(!InCombat && pInstance && pInstance->GetData("MaulgarEvent"))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64("MaulgarEvent_Tank"));

            if(target)
            {
                DoStartMeleeAttack(target);
                InCombat = true;
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if(pInstance && !pInstance->GetData("MaulgarEvent"))
            EnterEvadeMode();

        //GreatherPolymorph_Timer / disabled: it makes you fall under the texture / if you've got vmaps feel free to uncomment this
        /*if(GreatherPolymorph_Timer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
                DoCast(target, 33173);

            GreatherPolymorph_Timer = 20000;
        }else GreatherPolymorph_Timer -= diff;*/

        //LightningBolt_Timer
        if(LightningBolt_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 36152);
            LightningBolt_Timer = 15000;
        }else LightningBolt_Timer -= diff;

        //ArcaneShock_Timer
        if(ArcaneShock_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 33175);
            ArcaneShock_Timer = 20000;
        }else ArcaneShock_Timer -= diff;

        //ArcaneExplosion_Timer
        if(ArcaneExplosion_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 33237);
            ArcaneExplosion_Timer = 30000;
        }else ArcaneExplosion_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Blindeye The Seer AI
struct MANGOS_DLL_DECL boss_blindeye_the_seerAI : public ScriptedAI
{
    boss_blindeye_the_seerAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    uint32 GreaterPowerWordShield_Timer;
    uint32 Heal_Timer;

    ScriptedInstance* pInstance; 

    bool InCombat;

    void EnterEvadeMode()
    {
        GreaterPowerWordShield_Timer = 5000;
        Heal_Timer = 30000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //reset encounter
        if (pInstance)
            pInstance->SetData("MaulgarEvent", 0);

        InCombat = false;
    }

    void AttackStart(Unit *who)
    {
        if (!who || !pInstance)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            if(!InCombat)
            {
                InCombat = true;
                DoStartMeleeAttack(who);
                pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                pInstance->SetData("MaulgarEvent", 1);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || !pInstance)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(!InCombat)
                {
                    InCombat = true;
                    DoStartMeleeAttack(who);
                    pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                    pInstance->SetData("MaulgarEvent", 1);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if(!InCombat && pInstance && pInstance->GetData("MaulgarEvent"))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64("MaulgarEvent_Tank"));

            if(target)
            {
                DoStartMeleeAttack(target);
                InCombat = true;
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if(pInstance && !pInstance->GetData("MaulgarEvent"))
            EnterEvadeMode();

        //GreaterPowerWordShield_Timer
        if(GreaterPowerWordShield_Timer < diff)
        {
            DoCast(m_creature, 33147);
            GreaterPowerWordShield_Timer = 40000;
        }else GreaterPowerWordShield_Timer -= diff;

        //Heal_Timer
        if(Heal_Timer < diff)
        {
            DoCast(m_creature, 33144);
            Heal_Timer = 60000;
        }else Heal_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Krosh Firehand AI
struct MANGOS_DLL_DECL boss_krosh_firehandAI : public ScriptedAI
{
    boss_krosh_firehandAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    uint32 GreaterFireball_Timer;
    uint32 SpellShield_Timer;
    uint32 BlastWave_Timer;

    ScriptedInstance* pInstance; 

    bool InCombat;

    void EnterEvadeMode()
    {
        GreaterFireball_Timer = 10000;
        SpellShield_Timer = 5000;
        BlastWave_Timer = 20000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //reset encounter
        if (pInstance)
            pInstance->SetData("MaulgarEvent", 0);

        InCombat = false;
    }

    void AttackStart(Unit *who)
    {
        if (!who || !pInstance)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            if(!InCombat)
            {
                InCombat = true;
                DoStartMeleeAttack(who);
                pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                pInstance->SetData("MaulgarEvent", 1);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim() || !pInstance)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(!InCombat)
                {
                    InCombat = true;
                    DoStartMeleeAttack(who);
                    pInstance->SetData64("MaulgarEvent_Tank", who->GetGUID());
                    pInstance->SetData("MaulgarEvent", 1);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if(!InCombat && pInstance && pInstance->GetData("MaulgarEvent"))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64("MaulgarEvent_Tank"));

            if(target)
            {
                DoStartMeleeAttack(target);
                InCombat = true;
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //someone evaded!
        if(pInstance && !pInstance->GetData("MaulgarEvent"))
            EnterEvadeMode();

        //GreaterFireball_Timer
        if(GreaterFireball_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 33051);
            GreaterFireball_Timer = 30000;
        }else GreaterFireball_Timer -= diff;

        //SpellShield_Timer
        if(SpellShield_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 33054);
            SpellShield_Timer = 50000;
        }else SpellShield_Timer -= diff;

        //BlastWave_Timer
        if(BlastWave_Timer < diff)
        {
            DoCast(m_creature->getVictim(), 33061);
            BlastWave_Timer = 60000;
        }else BlastWave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_high_king_maulgar(Creature *_Creature)
{
    return new boss_high_king_maulgarAI (_Creature);
}
CreatureAI* GetAI_boss_olm_the_summoner(Creature *_Creature)
{
    return new boss_olm_the_summonerAI (_Creature);
}
CreatureAI *GetAI_boss_kiggler_the_crazed(Creature *_Creature)
{
    return new boss_kiggler_the_crazedAI (_Creature);
}
CreatureAI *GetAI_boss_blindeye_the_seer(Creature *_Creature)
{
    return new boss_blindeye_the_seerAI (_Creature);
}
CreatureAI *GetAI_boss_krosh_firehand(Creature *_Creature)
{
    return new boss_krosh_firehandAI (_Creature);
}

void AddSC_boss_high_king_maulgar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_high_king_maulgar";
    newscript->GetAI = GetAI_boss_high_king_maulgar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_kiggler_the_crazed";
    newscript->GetAI = GetAI_boss_kiggler_the_crazed;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_blindeye_the_seer";
    newscript->GetAI = GetAI_boss_blindeye_the_seer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_olm_the_summoner";
    newscript->GetAI = GetAI_boss_olm_the_summoner;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_krosh_firehand";
    newscript->GetAI = GetAI_boss_krosh_firehand;
    m_scripts[nrscripts++] = newscript;
}

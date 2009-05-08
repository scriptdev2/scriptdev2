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
SDName: Boss_High_King_Maulgar
SD%Complete: 80
SDComment: Verify that the script is working properly
SDCategory: Gruul's Lair
EndScriptData */

#include "precompiled.h"
#include "def_gruuls_lair.h"

#define SAY_AGGRO               -1565000
#define SAY_ENRAGE              -1565001
#define SAY_OGRE_DEATH1         -1565002
#define SAY_OGRE_DEATH2         -1565003
#define SAY_OGRE_DEATH3         -1565004
#define SAY_OGRE_DEATH4         -1565005
#define SAY_SLAY1               -1565006
#define SAY_SLAY2               -1565007
#define SAY_SLAY3               -1565008
#define SAY_DEATH               -1565009

// High King Maulgar
#define SPELL_ARCING_SMASH       39144
#define SPELL_MIGHTY_BLOW        33230
#define SPELL_WHIRLWIND          33238
#define SPELL_FLURRY             33232
#define SPELL_CHARGE             26561
#define SPELL_FEAR               16508

// Council spells
#define SPELL_DARK_DECAY        33129
#define SPELL_DEATH_COIL        33130
#define SPELL_GREATER_POLYMORPH 33173
#define SPELL_LIGHTNING_BOLT    36152
#define SPELL_ARCANE_SHOCK      33175
#define SPELL_ARCANE_EXPLOSION  33237
#define SPELL_GREATER_PW_SHIELD 33147
#define SPELL_HEAL              33144
#define SPELL_PRAYEROFHEALING   33152
#define SPELL_GREATER_FIREBALL  33051
#define SPELL_SPELLSHIELD       33054
#define SPELL_BLAST_WAVE        33061

//High King Maulgar AI
struct MANGOS_DLL_DECL boss_high_king_maulgarAI : public ScriptedAI
{
    boss_high_king_maulgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        for(uint8 i = 0; i < 4; ++i)
            Council[i] = 0;
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 ArcingSmash_Timer;
    uint32 MightyBlow_Timer;
    uint32 Whirlwind_Timer;
    uint32 Charge_Timer;
    uint32 Fear_Timer;


    bool Phase2;

    uint64 Council[4];

    void Reset()
    {
        ArcingSmash_Timer = 8000+rand()%6000;
        MightyBlow_Timer = 15000+rand()%10000;
        Whirlwind_Timer = 30000;
        Charge_Timer = 2000;
        Fear_Timer = 10000+rand()%15000;
        Phase2 = false;

        Creature *pCreature = NULL;
        for(uint8 i = 0; i < 4; i++)
        {
            if (Council[i])
            {
                pCreature = (Creature*)(Unit::GetUnit((*m_creature), Council[i]));
                if (pCreature && !pCreature->isAlive())
                {
                    pCreature->Respawn();
                    pCreature->AI()->EnterEvadeMode();
                }
            }
        }

        //reset encounter
        if (pInstance)
            pInstance->SetData(DATA_MAULGAREVENT, NOT_STARTED);
        else error_log(ERROR_INST_DATA);
    }

    void KilledUnit()
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
        {
            pInstance->SetData(DATA_MAULGAREVENT, DONE);

            // Open the door leading further in
            if (GameObject* pContinueDoor = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_MAULGAR_DOOR)))
                pContinueDoor->SetGoState(GO_STATE_ACTIVE);
        }
    }

    void Aggro(Unit *who) { StartEvent(who); }

    void GetCouncil()
    {
        //get council member's guid to respawn them if needed
        Council[0] = pInstance->GetData64(DATA_KIGGLERTHECRAZED);
        Council[1] = pInstance->GetData64(DATA_BLINDEYETHESEER);
        Council[2] = pInstance->GetData64(DATA_OLMTHESUMMONER);
        Council[3] = pInstance->GetData64(DATA_KROSHFIREHAND);
    }

    void StartEvent(Unit *who)
    {
        if (!pInstance)
            return;

        GetCouncil();

        DoScriptText(SAY_AGGRO, m_creature);

        pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
        pInstance->SetData(DATA_MAULGAREVENT, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!m_creature->isInCombat() && pInstance && pInstance->GetData(DATA_MAULGAREVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_MAULGAREVENT_TANK));

            if (target)
            {
                AttackStart(target);
                GetCouncil();
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_MAULGAREVENT))
            EnterEvadeMode();

        //ArcingSmash_Timer
        if (ArcingSmash_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCING_SMASH);
            ArcingSmash_Timer = 8000+rand()%4000;
        }else ArcingSmash_Timer -= diff;

        //Whirlwind_Timer
        if (Whirlwind_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_WHIRLWIND);
            Whirlwind_Timer = 30000+rand()%10000;
        }else Whirlwind_Timer -= diff;

        //MightyBlow_Timer
        if (MightyBlow_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            MightyBlow_Timer = 20000+rand()%15000;
        }else MightyBlow_Timer -= diff;

        //Entering Phase 2
        if (!Phase2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            Phase2 = true;
            DoScriptText(SAY_ENRAGE, m_creature);
            DoCast(m_creature, SPELL_FLURRY);
        }

        if (Phase2)
        {
            //Charge_Timer
            if (Charge_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_CHARGE);

                Charge_Timer = 14000 + rand()%6000;
            }else Charge_Timer -= diff;

            //MightyBlow_Timer
            if (Fear_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_FEAR);
                Fear_Timer = 20000+rand()%15000;
            }else Fear_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

//Olm The Summoner AI
struct MANGOS_DLL_DECL boss_olm_the_summonerAI : public ScriptedAI
{
    boss_olm_the_summonerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    uint32 DarkDecay_Timer;
    uint32 DeathCoil_Timer;
    uint32 Summon_Timer;

    ScriptedInstance* pInstance;

    void Reset()
    {
        DarkDecay_Timer = 18000;
        DeathCoil_Timer = 14000;
        Summon_Timer = 10000;

        //reset encounter
        if (pInstance)
            pInstance->SetData(DATA_MAULGAREVENT, 0);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
            pInstance->SetData(DATA_MAULGAREVENT, 1);
        }
    }

    float DoCalculateRandomLocation()
    {
        float Loc;
        float Rand = rand()%8;

        switch(rand()%2)
        {
            case 0: Loc = 0 + Rand; break;
            case 1: Loc = 0 - Rand; break;
        }
        return Loc;
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!m_creature->isInCombat() && pInstance && pInstance->GetData(DATA_MAULGAREVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_MAULGAREVENT_TANK));

            if (target)
                AttackStart(target);
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_MAULGAREVENT))
            EnterEvadeMode();

        //DarkDecay_Timer
        if (DarkDecay_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_DARK_DECAY);
            DarkDecay_Timer = 20000;
        }else DarkDecay_Timer -= diff;

        //DeathCoil_Timer
        if (DeathCoil_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_DEATH_COIL);
            DeathCoil_Timer = 8000 + rand()%5000;
        }else DeathCoil_Timer -= diff;

        //Summon_Timer
        if (Summon_Timer < diff)
        {
            Creature *Add = NULL;
            Add = DoSpawnCreature(18847, DoCalculateRandomLocation(), DoCalculateRandomLocation(), 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
            Summon_Timer = 25000+rand()%10000;
        }else Summon_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Kiggler The Crazed AI
struct MANGOS_DLL_DECL boss_kiggler_the_crazedAI : public ScriptedAI
{
    boss_kiggler_the_crazedAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    uint32 GreatherPolymorph_Timer;
    uint32 LightningBolt_Timer;
    uint32 ArcaneShock_Timer;
    uint32 ArcaneExplosion_Timer;

    ScriptedInstance* pInstance;

    void Reset()
    {
        GreatherPolymorph_Timer = 5000;
        LightningBolt_Timer = 10000;
        ArcaneShock_Timer = 20000;
        ArcaneExplosion_Timer = 30000;

        //reset encounter
        if (pInstance)
            pInstance->SetData(DATA_MAULGAREVENT, 0);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
            pInstance->SetData(DATA_MAULGAREVENT, 1);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_creature->getVictim() && who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!m_creature->isInCombat())
                {
                    AttackStart(who);
                    if (pInstance)
                    {
                        pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
                        pInstance->SetData(DATA_MAULGAREVENT, 1);
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!m_creature->isInCombat() && pInstance && pInstance->GetData(DATA_MAULGAREVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_MAULGAREVENT_TANK));

            if (target)
                AttackStart(target);
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_MAULGAREVENT))
            EnterEvadeMode();

        if (GreatherPolymorph_Timer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if (target)
                DoCast(target, SPELL_GREATER_POLYMORPH);

            GreatherPolymorph_Timer = 15000 + rand()%5000;
        }else GreatherPolymorph_Timer -= diff;

        //LightningBolt_Timer
        if (LightningBolt_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_LIGHTNING_BOLT);
            LightningBolt_Timer = 10000 + rand()%5000;
        }else LightningBolt_Timer -= diff;

        //ArcaneShock_Timer
        if (ArcaneShock_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_SHOCK);
            ArcaneShock_Timer = 15000 + rand()%5000;
        }else ArcaneShock_Timer -= diff;

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-75);
            ArcaneExplosion_Timer = 30000;
        }else ArcaneExplosion_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Blindeye The Seer AI
struct MANGOS_DLL_DECL boss_blindeye_the_seerAI : public ScriptedAI
{
    boss_blindeye_the_seerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    uint32 GreaterPowerWordShield_Timer;
    uint32 Heal_Timer;
    uint32 PrayerofHealing_Timer;

    ScriptedInstance* pInstance;

    void Reset()
    {
        GreaterPowerWordShield_Timer = 5000;
        Heal_Timer = 25000 + rand()%15000;
        PrayerofHealing_Timer = 45000 + rand()%10000;

        //reset encounter
        if (pInstance)
            pInstance->SetData(DATA_MAULGAREVENT, 0);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
            pInstance->SetData(DATA_MAULGAREVENT, 1);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_creature->getVictim() && who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!m_creature->isInCombat())
                {
                    AttackStart(who);
                    if (pInstance)
                    {
                        pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
                        pInstance->SetData(DATA_MAULGAREVENT, 1);
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!m_creature->isInCombat() && pInstance && pInstance->GetData(DATA_MAULGAREVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_MAULGAREVENT_TANK));

            if (target)
                AttackStart(target);
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_MAULGAREVENT))
            EnterEvadeMode();

        //GreaterPowerWordShield_Timer
        if (GreaterPowerWordShield_Timer < diff)
        {
            DoCast(m_creature, SPELL_GREATER_PW_SHIELD);
            GreaterPowerWordShield_Timer = 30000 + rand()%10000;
        }else GreaterPowerWordShield_Timer -= diff;

        //Heal_Timer
        if (Heal_Timer < diff)
        {
            DoCast(m_creature, SPELL_HEAL);
            Heal_Timer = 15000 + rand()%25000;
        }else Heal_Timer -= diff;

        //PrayerofHealing_Timer
        if (PrayerofHealing_Timer < diff)
        {
            DoCast(m_creature, SPELL_PRAYEROFHEALING);
            PrayerofHealing_Timer = 35000 + rand()%15000;
        }else PrayerofHealing_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Krosh Firehand AI
struct MANGOS_DLL_DECL boss_krosh_firehandAI : public ScriptedAI
{
    boss_krosh_firehandAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    uint32 GreaterFireball_Timer;
    uint32 SpellShield_Timer;
    uint32 BlastWave_Timer;

    ScriptedInstance* pInstance;

    void Reset()
    {
        GreaterFireball_Timer = 1000;
        SpellShield_Timer = 5000;
        BlastWave_Timer = 20000;

        //reset encounter
        if (pInstance)
            pInstance->SetData(DATA_MAULGAREVENT, 0);
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
        {
            pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
            pInstance->SetData(DATA_MAULGAREVENT, 1);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_creature->getVictim() && who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!m_creature->isInCombat())
                {
                    AttackStart(who);
                    if (pInstance)
                    {
                        pInstance->SetData64(DATA_MAULGAREVENT_TANK, who->GetGUID());
                        pInstance->SetData(DATA_MAULGAREVENT, 1);
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Only if not incombat check if the event is started
        if (!m_creature->isInCombat() && pInstance && pInstance->GetData(DATA_MAULGAREVENT))
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_MAULGAREVENT_TANK));

            if (target)
                AttackStart(target);
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (pInstance && !pInstance->GetData(DATA_MAULGAREVENT))
            EnterEvadeMode();

        //GreaterFireball_Timer
        if (GreaterFireball_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_GREATER_FIREBALL);
            GreaterFireball_Timer = 3200;
        }else GreaterFireball_Timer -= diff;

        //SpellShield_Timer
        if (SpellShield_Timer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature->getVictim(), SPELL_SPELLSHIELD);
            SpellShield_Timer = 30000;
        }else SpellShield_Timer -= diff;

        //BlastWave_Timer
        if (BlastWave_Timer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature->getVictim(), SPELL_BLAST_WAVE);
            BlastWave_Timer = 30000 + rand()%15000;
        }else BlastWave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_high_king_maulgar(Creature* pCreature)
{
    return new boss_high_king_maulgarAI(pCreature);
}

CreatureAI* GetAI_boss_olm_the_summoner(Creature* pCreature)
{
    return new boss_olm_the_summonerAI(pCreature);
}

CreatureAI *GetAI_boss_kiggler_the_crazed(Creature* pCreature)
{
    return new boss_kiggler_the_crazedAI(pCreature);
}

CreatureAI *GetAI_boss_blindeye_the_seer(Creature* pCreature)
{
    return new boss_blindeye_the_seerAI(pCreature);
}

CreatureAI *GetAI_boss_krosh_firehand(Creature* pCreature)
{
    return new boss_krosh_firehandAI(pCreature);
}

void AddSC_boss_high_king_maulgar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_high_king_maulgar";
    newscript->GetAI = &GetAI_boss_high_king_maulgar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_kiggler_the_crazed";
    newscript->GetAI = &GetAI_boss_kiggler_the_crazed;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_blindeye_the_seer";
    newscript->GetAI = &GetAI_boss_blindeye_the_seer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_olm_the_summoner";
    newscript->GetAI = &GetAI_boss_olm_the_summoner;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_krosh_firehand";
    newscript->GetAI = &GetAI_boss_krosh_firehand;
    newscript->RegisterSelf();
}

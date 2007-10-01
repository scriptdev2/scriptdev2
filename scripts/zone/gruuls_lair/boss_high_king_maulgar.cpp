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
#include "../../creature/simple_ai.h"

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

//High King Maulgar AI (normal AI)
struct MANGOS_DLL_DECL boss_high_king_maulgarAI : public ScriptedAI
{
    boss_high_king_maulgarAI(Creature *c) : ScriptedAI(c) 
    {
        Council[0] = NULL;
        Council[1] = NULL;
        Council[2] = NULL;
        Council[3] = NULL;
        EnterEvadeMode();
    }

    uint32 ArcingSmash_Timer;
    uint32 MightyBlow_Timer;
    uint32 Whirlwind_Timer;
    uint32 Charging_Timer;

    bool Phase2;
    bool InCombat;

    Creature *Council[4];

    void EnterEvadeMode()
    {       
        ArcingSmash_Timer = 10000;
        MightyBlow_Timer = 40000;
        Whirlwind_Timer = 30000;
        Charging_Timer = 0;
        Phase2 = false;
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
        m_creature->GetInstanceData()->SetData("HighKingMaulgarEvent", 3); // 3 = DONE
    }

    void StartEvent()
    {
        Council[0] = m_creature->GetInstanceData()->GetUnit("KigglerTheCrazed");
        Council[1] = m_creature->GetInstanceData()->GetUnit("BlindeyeTheSeer");
        Council[2] = m_creature->GetInstanceData()->GetUnit("OlmTheSummoner");
        Council[3] = m_creature->GetInstanceData()->GetUnit("KroshFirehand");

        if(!Council[0]|| !Council[1] || !Council[2] || !Council[3])
        {
            // means FAILED
            DoYell("Unable to found my council. Event failed.", LANG_UNIVERSAL, NULL);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->setFaction(35);
            m_creature->GetInstanceData()->SetData("HighKingMaulgarEvent", 2); // 2 = FAILED

            for(uint8 i = 0; i < 4; i++)
                if(Council[i])
                {
                    Council[i]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Council[i]->setFaction(35);
                }
            return;
        }

        DoPlaySoundToSet(m_creature, SOUND_AGGRO);

        for(uint8 i = 0; i < 4; i++)
        {
            Council[i]->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            Council[i]->setFaction(14);

            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
                Council[i]->AI()->AttackStart(target);
        }

        InCombat = true;
        m_creature->GetInstanceData()->SetData("HighKingMaulgarEvent", 1); // 1 = IN_PROGRESS
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if(!m_creature->GetInstanceData()->GetData("HighKingMaulgarEvent")) // 0 = NOT_STARTED
            {
                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                if(!InCombat)
                    StartEvent();
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

                if(!m_creature->GetInstanceData()->GetData("HighKingMaulgarEvent")) // 0 = NOT_STARTED
                {
                    DoStartMeleeAttack(who);
                    if(!InCombat)
                        StartEvent();
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
           return;

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
CreatureAI* GetAI_boss_high_king_maulgar(Creature *_Creature)
{
    return new boss_high_king_maulgarAI (_Creature);
}


//Olm The Summoner AI (normal AI)
struct MANGOS_DLL_DECL boss_olm_the_summonerAI : public ScriptedAI
{
    boss_olm_the_summonerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 DarkDecay_Timer;
    uint32 Summon_Timer;

    void EnterEvadeMode()
    {       
        DarkDecay_Timer = 10000;
        Summon_Timer = 15000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
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

                DoStartMeleeAttack(who);
            }
        }
    }

    float DoCalculateRandomLocation()
    {
        float Loc;
        float Rand = rand()%6;

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
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
           return;

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
            Add = DoSpawnCreature(18847, DoCalculateRandomLocation(), DoCalculateRandomLocation(), 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);

            Summon_Timer = 30000;
        }else Summon_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_olm_the_summoner(Creature *_Creature)
{
    return new boss_olm_the_summonerAI (_Creature);
}

// Kiggler The Crazed AI (simple AI)
CreatureAI* GetAI_boss_kiggler_the_crazed(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //Greather Polymorph
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 33173;
    ai->Spell[0].Cooldown = 20000;
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    //Lightning Bolt
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 36152;
    ai->Spell[1].Cooldown = 15000;
    ai->Spell[1].First_Cast = 10000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //Arcane Shock
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 33175;
    ai->Spell[2].Cooldown = 20000;
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //Arcane Explosion
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 33237;
    ai->Spell[3].Cooldown = 30000;
    ai->Spell[3].First_Cast = 30000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

//Blindeye The Seer AI (simple AI)
CreatureAI* GetAI_boss_blindeye_the_seer(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //Greater Power Word: Shield
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 33147;
    ai->Spell[0].Cooldown = 40000;
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //Heal
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 33144;
    ai->Spell[1].Cooldown = 60000;
    ai->Spell[1].First_Cast = 30000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

//Krosh Firehand AI (simple AI)
CreatureAI* GetAI_boss_krosh_firehand(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //Greater Fireball
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 33051;
    ai->Spell[0].Cooldown = 30000;
    ai->Spell[0].First_Cast = 10000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //Spell Shield
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 33054;
    ai->Spell[1].Cooldown = 50000;
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //Blast Wave (AoE)
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 33061;
    ai->Spell[2].Cooldown = 60000;
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
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

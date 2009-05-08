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
SDName: Boss_Magtheridon
SD%Complete: 80
SDComment: Some spell issues with target selection. Spell hacks can be enabled from instance script for experiment/tests.
SDCategory: Hellfire Citadel, Magtheridon's lair
EndScriptData */

#include "precompiled.h"
#include "def_magtheridons_lair.h"

struct Yell
{
    int32 id;
};

static Yell RandomTaunt[]=
{
    {-1544000},
    {-1544001},
    {-1544002},
    {-1544003},
    {-1544004},
    {-1544005},
};

#define SAY_FREED                   -1544006
#define SAY_AGGRO                   -1544007
#define SAY_BANISH                  -1544008
#define SAY_CHAMBER_DESTROY         -1544009
#define SAY_PLAYER_KILLED           -1544010
#define SAY_DEATH                   -1544011

#define EMOTE_BERSERK               -1544012
#define EMOTE_BLASTNOVA             -1544013
#define EMOTE_BEGIN                 -1544014
#define EMOTE_FREED                 -1544015

#define MOB_MAGTHERIDON             17257
#define MOB_ROOM                    17516
#define MOB_CHANNELLER              17256
#define MOB_ABYSSAL                 17454

#define SPELL_BLASTNOVA             30616
#define SPELL_CLEAVE                30619
#define SPELL_QUAKE_TRIGGER         30576                   // must be cast with 30561 as the proc spell
#define SPELL_QUAKE_KNOCKBACK       30571
#define SPELL_BLAZE_TARGET          30541                   // core, does not support effectImplicitTarget 7
#define SPELL_BLAZE_TRAP            30542
#define SPELL_DEBRIS_KNOCKDOWN      36449
#define SPELL_DEBRIS_VISUAL         30632
#define SPELL_DEBRIS_DAMAGE         30631                   // core, does not fully support effectImplicitTarget 8
#define SPELL_CAMERA_SHAKE          36455
#define SPELL_BERSERK               27680

#define SPELL_SHADOW_CAGE           30168
#define SPELL_SHADOW_GRASP          30410
#define SPELL_SHADOW_GRASP_VISUAL   30166
#define SPELL_MIND_EXHAUSTION       44032                   // Casted by the cubes when channeling ends

#define SPELL_SHADOW_CAGE_C         30205
#define SPELL_SHADOW_GRASP_C        30207

#define SPELL_SHADOW_BOLT_VOLLEY    30510
#define SPELL_DARK_MENDING          30528
#define SPELL_FEAR                  30530                   // 39176
#define SPELL_BURNING_ABYSSAL       30511
#define SPELL_SOUL_TRANSFER         30531                   // core, does not support effectImplicitTarget 7

#define SPELL_FIRE_BLAST            37110

// count of clickers needed to interrupt blast nova
#define CLICKERS_COUNT                5

typedef std::map<uint64, uint64> CubeMap;

struct MANGOS_DLL_DECL mob_abyssalAI : public ScriptedAI
{
    mob_abyssalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        trigger = 0;
        Despawn_Timer = 60000;
        Reset();
    }

    uint32 FireBlast_Timer;
    uint32 Despawn_Timer;
    uint32 trigger;

    void Reset()
    {
        FireBlast_Timer = 6000;
    }

    void SpellHit(Unit* caster, const SpellEntry *spell)
    {
        if (trigger == 2 && spell->Id == SPELL_BLAZE_TARGET)
        {
            m_creature->CastSpell(m_creature, SPELL_BLAZE_TRAP, true);
            m_creature->SetVisibility(VISIBILITY_OFF);
            Despawn_Timer = 130000;
        }
    }

    void SetTrigger(uint32 _trigger)
    {
        trigger = _trigger;
        m_creature->SetDisplayId(11686);

        if (trigger == 1)                                   //debris
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->CastSpell(m_creature, SPELL_DEBRIS_VISUAL, true);
            FireBlast_Timer = 5000;
            Despawn_Timer = 10000;
        }
    }

    void Aggro(Unit* who)
    {
        DoZoneInCombat();
    }

    void AttackStart(Unit *who)
    {
        if (trigger)
            return;

        ScriptedAI::AttackStart(who);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (trigger)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void UpdateAI(const uint32 diff)
    {
        if (trigger)
        {
            if (trigger == 1)
            {
                if (FireBlast_Timer < diff)
                {
                    m_creature->CastSpell(m_creature, SPELL_DEBRIS_DAMAGE, true);
                    trigger = 3;
                }else FireBlast_Timer -= diff;
            }
        }

        if (Despawn_Timer < diff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else Despawn_Timer -= diff;

        if (!m_creature->SelectHostilTarget() && !m_creature->getVictim())
            return;

        if (FireBlast_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FIRE_BLAST);
            FireBlast_Timer = 5000+rand()%10000;
        }else FireBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_magtheridonAI : public ScriptedAI
{
    boss_magtheridonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        m_creature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 10);
        m_creature->SetFloatValue(UNIT_FIELD_COMBATREACH, 10);
        Reset();
    }

    CubeMap Cube;

    ScriptedInstance* pInstance;

    uint32 RandChat_Timer;

    uint32 Berserk_Timer;
    uint32 Quake_Timer;
    uint32 Cleave_Timer;
    uint32 BlastNova_Timer;
    uint32 Blaze_Timer;
    uint32 Phase3_Timer;
    uint32 Phase3_Count;

    bool Intro;
    bool Phase3;
    bool NeedCheckCube;

    void Reset()
    {
        if (pInstance)
        {
            pInstance->SetData(TYPE_MAGTHERIDON_EVENT, NOT_STARTED);
            pInstance->SetData(TYPE_HALL_COLLAPSE, NOT_STARTED);
        }

        RandChat_Timer = 90000;

        Berserk_Timer = 1320000;
        Quake_Timer = 40000;
        Phase3_Timer = 5000;
        Phase3_Count = 0;
        Blaze_Timer = 10000+rand()%20000;
        BlastNova_Timer = 60000;
        Cleave_Timer = 15000;

        Intro = false;
        Phase3 = false;
        NeedCheckCube = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void SetClicker(uint64 cubeGUID, uint64 clickerGUID)
    {
        // to avoid multiclicks from 1 cube
        if (uint64 guid = Cube[cubeGUID])
            DebuffClicker(Unit::GetUnit(*m_creature, guid));

        Cube[cubeGUID] = clickerGUID;
        NeedCheckCube = true;
    }

    //function to interrupt channeling and debuff clicker with mind exhaused if second person clicks with same cube or after dispeling/ending shadow grasp DoT)
    void DebuffClicker(Unit *clicker)
    {
        if (!clicker || !clicker->isAlive())
            return;

        clicker->RemoveAurasDueToSpell(SPELL_SHADOW_GRASP); // cannot interrupt triggered spells
        clicker->InterruptNonMeleeSpells(false);
        clicker->CastSpell(clicker, SPELL_MIND_EXHAUSTION, true);
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (!Intro)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void NeedCheckCubeStatus()
    {
        uint32 ClickerNum = 0;
        // now checking if every clicker has debuff from manticron
        // if not - apply mind exhaustion and delete from clicker's list
        for(CubeMap::iterator i = Cube.begin(); i != Cube.end(); ++i)
        {
            Unit *clicker = Unit::GetUnit(*m_creature, (*i).second);
            if (!clicker || !clicker->HasAura(SPELL_SHADOW_GRASP, 1))
            {
                DebuffClicker(clicker);
                (*i).second = 0;
            }else ClickerNum++;
        }

        // if 5 clickers from other cubes apply shadow cage
        if (ClickerNum >= CLICKERS_COUNT && !m_creature->HasAura(SPELL_SHADOW_CAGE, 0) && m_creature->HasAura(SPELL_BLASTNOVA, 0))
        {
            DoScriptText(SAY_BANISH, m_creature);
            m_creature->CastSpell(m_creature, SPELL_SHADOW_CAGE, true);
        }
        else if (ClickerNum < CLICKERS_COUNT && m_creature->HasAura(SPELL_SHADOW_CAGE, 0))
            m_creature->RemoveAurasDueToSpell(SPELL_SHADOW_CAGE);

        if (!ClickerNum)
            NeedCheckCube = false;
    }

    void IntroDone()
    {
        if (!pInstance)
            return;

        if (pInstance->GetData(TYPE_MAGTHERIDON_EVENT) == NOT_STARTED)
            return;

        if (pInstance->GetData(TYPE_MAGTHERIDON_EVENT) == DONE)
            return;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
        m_creature->RemoveAurasDueToSpell(SPELL_SHADOW_CAGE_C);
        m_creature->clearUnitState(UNIT_STAT_STUNNED);

        DoScriptText(EMOTE_FREED, m_creature);
        DoScriptText(SAY_FREED, m_creature);

        Intro = true;
    }

    void Aggro(Unit *who)
    {
        DoZoneInCombat();
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_SHADOW_GRASP_C)
        {
            m_creature->CastSpell(m_creature,SPELL_SHADOW_CAGE_C,false);
            m_creature->addUnitState(UNIT_STAT_STUNNED);
        }
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(SAY_PLAYER_KILLED, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        if (pInstance)
            pInstance->SetData(TYPE_MAGTHERIDON_EVENT, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        {
            if (!Intro)
            {
                IntroDone();
                return;
            }

            if (RandChat_Timer < diff)
            {
                DoScriptText(RandomTaunt[rand()%6].id, m_creature);
                RandChat_Timer = 90000;
            }else RandChat_Timer -= diff;

            return;
        }

        if (NeedCheckCube)
            NeedCheckCubeStatus();

        if (Berserk_Timer < diff)
        {
            DoScriptText(EMOTE_BERSERK, m_creature);
            m_creature->CastSpell(m_creature, SPELL_BERSERK, true);
            Berserk_Timer = 60000;
        }else Berserk_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;

        if (Quake_Timer < diff)
        {
            // to avoid blastnova interruption
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                int32 i = SPELL_QUAKE_KNOCKBACK;
                m_creature->CastCustomSpell(m_creature, SPELL_QUAKE_TRIGGER, &i, 0, 0, false);
                Quake_Timer = 50000;
            }
        }else Quake_Timer -= diff;

        if (BlastNova_Timer < diff)
        {
            // to avoid earthquake interruption
            if (!m_creature->hasUnitState(UNIT_STAT_STUNNED))
            {
                DoScriptText(EMOTE_BLASTNOVA, m_creature);
                DoCast(m_creature, SPELL_BLASTNOVA);
                BlastNova_Timer = 60000;
            }
        }else BlastNova_Timer -= diff;

        if (Blaze_Timer < diff)
        {
            if (Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
            {
                float x, y, z;
                target->GetPosition(x, y, z);
                Creature *summon = m_creature->SummonCreature(MOB_ABYSSAL, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                if (summon)
                {
                    ((mob_abyssalAI*)summon->AI())->SetTrigger(2);
                    m_creature->CastSpell(summon, SPELL_BLAZE_TARGET, true);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }
            Blaze_Timer = 20000 + rand()%20000;
        }else Blaze_Timer -= diff;

        if (!Phase3 && m_creature->GetHealth()*10 < m_creature->GetMaxHealth()*3
            && !m_creature->IsNonMeleeSpellCasted(false)    // blast nova
            && !m_creature->hasUnitState(UNIT_STAT_STUNNED))// shadow cage and earthquake
        {
            Phase3 = true;
            DoScriptText(SAY_CHAMBER_DESTROY, m_creature);
        }

        if (Phase3)
        {
            if (Phase3_Timer < diff)
            {
                switch(Phase3_Count)
                {
                    case 0:
                        m_creature->CastSpell(m_creature, SPELL_CAMERA_SHAKE, true);
                        Phase3_Count++;
                        Phase3_Timer = 2000;
                        break;
                    case 1:
                        if (pInstance)
                            pInstance->SetData(TYPE_HALL_COLLAPSE, IN_PROGRESS);
                        Phase3_Count++;
                        Phase3_Timer = 8000;
                        break;
                    case 2:
                        m_creature->CastSpell(m_creature, SPELL_DEBRIS_KNOCKDOWN, true);
                        Phase3_Count++;
                        Phase3_Timer = 15000;
                        break;
                    case 3:
                        if (Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        {
                            float x, y, z;
                            target->GetPosition(x, y, z);
                            Creature *summon = m_creature->SummonCreature(MOB_ABYSSAL, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                            if (summon)
                                ((mob_abyssalAI*)summon->AI())->SetTrigger(1);
                            Phase3_Timer = 15000;
                        }
                        break;
                }
            } else Phase3_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_hellfire_channelerAI : public ScriptedAI
{
    mob_hellfire_channelerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 ShadowBoltVolley_Timer;
    uint32 DarkMending_Timer;
    uint32 Fear_Timer;
    uint32 Infernal_Timer;

    bool InfernalSpawned;

    void Reset()
    {
        ShadowBoltVolley_Timer = 8000 + rand()%2000;
        DarkMending_Timer = 10000;
        Fear_Timer = 15000 + rand()%5000;
        Infernal_Timer = 10000 + rand()%40000;

        InfernalSpawned = false;

        if (pInstance)
            pInstance->SetData(TYPE_CHANNELER_EVENT, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        if (!pInstance)
            return;

        m_creature->InterruptNonMeleeSpells(false);

        if (Unit *Magtheridon = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_MAGTHERIDON)))
        {
            if (!Magtheridon->isAlive())
                return;

            if (pInstance->GetData(TYPE_CHANNELER_EVENT) == NOT_STARTED)
                DoScriptText(EMOTE_BEGIN, Magtheridon);
        }

        pInstance->SetData(TYPE_CHANNELER_EVENT, IN_PROGRESS);
        DoZoneInCombat();
    }

    void JustSummoned(Creature *summon)
    {
        summon->AI()->AttackStart(m_creature->getVictim());
    }

    void MoveInLineOfSight(Unit* who)
    {
    }

    void JustDied(Unit* killer)
    {
        if (pInstance)
            pInstance->SetData(TYPE_CHANNELER_EVENT, DONE);

        killer->CastSpell(killer,SPELL_SOUL_TRANSFER,false);
    }

    void JustReachedHome()
    {
        DoCast(m_creature, SPELL_SHADOW_GRASP_C);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Shadow bolt volley
        if (ShadowBoltVolley_Timer < diff)
        {
            DoCast(m_creature, SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolley_Timer = 10000 + (rand()%10000);
        }else ShadowBoltVolley_Timer -= diff;

        //Dark Mending
        if (DarkMending_Timer < diff)
        {
            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
            {
                //Cast on ourselves if we are lower then lowest hp friendly unit
                /*if (pLowestHPTarget && LowestHP < m_creature->GetHealth())
                    DoCast(pLowestHPTarget, SPELL_DARK_MENDING);
                else*/
                DoCast(m_creature, SPELL_DARK_MENDING);
            }

            DarkMending_Timer = 10000 + (rand() % 10000);
        }else DarkMending_Timer -= diff;

        //Fear
        if (Fear_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1))
                DoCast(target,SPELL_FEAR);

            Fear_Timer = 25000 + (rand()%15000);
        }else Fear_Timer -= diff;

        //Infernal spawning
        if (!InfernalSpawned && Infernal_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                m_creature->CastSpell(target, SPELL_BURNING_ABYSSAL, true);

            InfernalSpawned = true;
        }else Infernal_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Manticron Cube
bool GOHello_go_manticron_cube(Player* pPlayer, GameObject* _GO)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)_GO->GetInstanceData();

    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_MAGTHERIDON_EVENT) != IN_PROGRESS)
        return true;

    Creature *Magtheridon =(Creature*)Unit::GetUnit(*_GO, pInstance->GetData64(DATA_MAGTHERIDON));
    if (!Magtheridon || !Magtheridon->isAlive())
        return true;

    // if exhausted or already channeling return
    if (pPlayer->HasAura(SPELL_MIND_EXHAUSTION, 0) || pPlayer->HasAura(SPELL_SHADOW_GRASP, 1))
        return true;

    pPlayer->InterruptNonMeleeSpells(false);
    pPlayer->CastSpell(pPlayer, SPELL_SHADOW_GRASP, true);
    pPlayer->CastSpell(pPlayer, SPELL_SHADOW_GRASP_VISUAL, false);
    ((boss_magtheridonAI*)Magtheridon->AI())->SetClicker(_GO->GetGUID(), pPlayer->GetGUID());

    return true;
}

CreatureAI* GetAI_boss_magtheridon(Creature* pCreature)
{
    return new boss_magtheridonAI(pCreature);
}

CreatureAI* GetAI_mob_hellfire_channeler(Creature* pCreature)
{
    return new mob_hellfire_channelerAI(pCreature);
}

CreatureAI* GetAI_mob_abyssalAI(Creature* pCreature)
{
    return new mob_abyssalAI(pCreature);
}

void AddSC_boss_magtheridon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_magtheridon";
    newscript->GetAI = &GetAI_boss_magtheridon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_hellfire_channeler";
    newscript->GetAI = &GetAI_mob_hellfire_channeler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_manticron_cube";
    newscript->pGOHello = &GOHello_go_manticron_cube;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_abyssal";
    newscript->GetAI = &GetAI_mob_abyssalAI;
    newscript->RegisterSelf();
}

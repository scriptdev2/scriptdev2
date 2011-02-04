/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Thekal
SD%Complete: 95
SDComment: Almost finished.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

#define SAY_AGGRO               -1309009
#define SAY_DEATH               -1309010

#define SPELL_MORTALCLEAVE        22859
#define SPELL_SILENCE             23207
#define SPELL_FRENZY              23342
#define SPELL_FORCEPUNCH          24189
#define SPELL_CHARGE              24408
#define SPELL_ENRAGE              23537
#define SPELL_SUMMONTIGERS        24183
#define SPELL_TIGER_FORM          24169
#define SPELL_RESURRECT           24173                     //We will not use this spell.

//Zealot Lor'Khan Spells
#define SPELL_SHIELD              25020
#define SPELL_BLOODLUST           24185
#define SPELL_GREATERHEAL         24208
#define SPELL_DISARM              22691

//Zealot Lor'Khan Spells
#define SPELL_SWEEPINGSTRIKES     18765
#define SPELL_SINISTERSTRIKE      15667
#define SPELL_GOUGE               24698
#define SPELL_KICK                15614
#define SPELL_BLIND               21060

struct MANGOS_DLL_DECL boss_thekalAI : public ScriptedAI
{
    boss_thekalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 MortalCleave_Timer;
    uint32 Silence_Timer;
    uint32 Frenzy_Timer;
    uint32 ForcePunch_Timer;
    uint32 Charge_Timer;
    uint32 Enrage_Timer;
    uint32 SummonTigers_Timer;
    uint32 Check_Timer;
    uint32 Resurrect_Timer;

    bool Enraged;
    bool PhaseTwo;
    bool WasDead;

    void Reset()
    {
        MortalCleave_Timer = 4000;
        Silence_Timer = 9000;
        Frenzy_Timer = 30000;
        ForcePunch_Timer = 4000;
        Charge_Timer = 12000;
        Enrage_Timer = 32000;
        SummonTigers_Timer = 25000;
        Check_Timer = 10000;
        Resurrect_Timer = 10000;

        Enraged = false;
        PhaseTwo = false;
        WasDead = false;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, NOT_STARTED);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Check_Timer for the death of LorKhan and Zath.
        if (!WasDead && Check_Timer < diff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                {
                    //Resurrect LorKhan
                    if (Creature *pLorKhan = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_LORKHAN)))
                    {
                        pLorKhan->SetStandState(UNIT_STAND_STATE_STAND);
                        pLorKhan->setFaction(14);
                        pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pLorKhan->SetHealth(int(pLorKhan->GetMaxHealth()*1.0));

                        m_pInstance->SetData(TYPE_LORKHAN, DONE);
                    }
                }

                if (m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
                {
                    //Resurrect Zath
                    if (Creature *pZath = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ZATH)))
                    {
                        pZath->SetStandState(UNIT_STAND_STATE_STAND);
                        pZath->setFaction(14);
                        pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));

                        m_pInstance->SetData(TYPE_ZATH, DONE);
                    }
                }
            }
            Check_Timer = 5000;
        }else Check_Timer -= diff;

        if (!PhaseTwo && MortalCleave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MORTALCLEAVE);
            MortalCleave_Timer = urand(15000, 20000);
        }else MortalCleave_Timer -= diff;

        if (!PhaseTwo && Silence_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SILENCE);
            Silence_Timer = urand(20000, 25000);
        }else Silence_Timer -= diff;

        if (!PhaseTwo && !WasDead && m_creature->GetHealthPercent() < 5.0f)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
            m_creature->AttackStop();

            if (m_pInstance)
                m_pInstance->SetData(TYPE_THEKAL, SPECIAL);

            WasDead = true;
        }

        //Thekal will transform to Tiger if he died and was not resurrected after 10 seconds.
        if (!PhaseTwo && WasDead)
        {
            if (Resurrect_Timer < diff)
            {
                DoCastSpellIfCan(m_creature,SPELL_TIGER_FORM);
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.00f);
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetHealth(int(m_creature->GetMaxHealth()*1.0));
                const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 40)));
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 40)));
                m_creature->UpdateDamagePhysical(BASE_ATTACK);
                DoResetThreat();
                PhaseTwo = true;
            }else Resurrect_Timer -= diff;
        }

        if (m_creature->GetHealthPercent() == 100.0f && WasDead)
        {
            WasDead = false;
        }

        if (PhaseTwo)
        {
            if (Charge_Timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                {
                    DoCastSpellIfCan(target,SPELL_CHARGE);
                    DoResetThreat();
                    AttackStart(target);
                }
                Charge_Timer = urand(15000, 22000);
            }else Charge_Timer -= diff;

            if (Frenzy_Timer < diff)
            {
                DoCastSpellIfCan(m_creature,SPELL_FRENZY);
                Frenzy_Timer = 30000;
            }else Frenzy_Timer -= diff;

            if (ForcePunch_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SILENCE);
                ForcePunch_Timer = urand(16000, 21000);
            }else ForcePunch_Timer -= diff;

            if (SummonTigers_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(),SPELL_SUMMONTIGERS);
                SummonTigers_Timer = urand(10000, 14000);
            }else SummonTigers_Timer -= diff;

            if (m_creature->GetHealthPercent() < 11.0f && !Enraged)
            {
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                Enraged = true;
            }
        }

        if (m_creature->getVictim())                        // TODO - use correct check here, this only prevents crash
            DoMeleeAttackIfReady();
    }
};

//Zealot Lor'Khan
struct MANGOS_DLL_DECL mob_zealot_lorkhanAI : public ScriptedAI
{
    mob_zealot_lorkhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    uint32 Shield_Timer;
    uint32 BloodLust_Timer;
    uint32 GreaterHeal_Timer;
    uint32 Disarm_Timer;
    uint32 Check_Timer;

    bool FakeDeath;

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        Shield_Timer = 1000;
        BloodLust_Timer = 16000;
        GreaterHeal_Timer = 32000;
        Disarm_Timer = 6000;
        Check_Timer = 10000;

        FakeDeath = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LORKHAN, NOT_STARTED);

        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI (const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Shield_Timer
        if (Shield_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_SHIELD);
            Shield_Timer = 61000;
        }else Shield_Timer -= diff;

        //BloodLust_Timer
        if (BloodLust_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_BLOODLUST);
            BloodLust_Timer = urand(20000, 28000);
        }else BloodLust_Timer -= diff;

        //Casting Greaterheal to Thekal or Zath if they are in meele range.
        // TODO - why this range check?
        if (GreaterHeal_Timer < diff)
        {
            if (m_pInstance)
            {
                Creature* pThekal = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THEKAL));
                Creature* pZath = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ZATH));

                switch(urand(0, 1))
                {
                    case 0:
                        if (pThekal && m_creature->IsWithinDistInMap(pThekal, ATTACK_DISTANCE))
                            DoCastSpellIfCan(pThekal, SPELL_GREATERHEAL);
                        break;
                    case 1:
                        if (pZath && m_creature->IsWithinDistInMap(pZath, ATTACK_DISTANCE))
                            DoCastSpellIfCan(pZath, SPELL_GREATERHEAL);
                        break;
                }
            }

            GreaterHeal_Timer = urand(15000, 20000);
        }else GreaterHeal_Timer -= diff;

        //Disarm_Timer
        if (Disarm_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_DISARM);
            Disarm_Timer = urand(15000, 25000);
        }else Disarm_Timer -= diff;

        //Check_Timer for the death of LorKhan and Zath.
        if (!FakeDeath && Check_Timer < diff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
                {
                    //Resurrect Thekal
                    if (Creature* pThekal = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THEKAL)))
                    {
                        pThekal->SetStandState(UNIT_STAND_STATE_STAND);
                        pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pThekal->setFaction(14);
                        pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    }
                }

                if (m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
                {
                    //Resurrect Zath
                    if (Creature* pZath = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ZATH)))
                    {
                        pZath->SetStandState(UNIT_STAND_STATE_STAND);
                        pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pZath->setFaction(14);
                        pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));
                    }
                }
            }

            Check_Timer = 5000;
        }else Check_Timer -= diff;

        if (m_creature->GetHealthPercent() < 5.0f)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
            m_creature->setFaction(35);
            m_creature->AttackStop();

            if (m_pInstance)
                m_pInstance->SetData(TYPE_LORKHAN, SPECIAL);

            FakeDeath = true;
        }

        if (!FakeDeath)
            DoMeleeAttackIfReady();
    }
};

//Zealot Zath
struct MANGOS_DLL_DECL mob_zealot_zathAI : public ScriptedAI
{
    mob_zealot_zathAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    uint32 SweepingStrikes_Timer;
    uint32 SinisterStrike_Timer;
    uint32 Gouge_Timer;
    uint32 Kick_Timer;
    uint32 Blind_Timer;
    uint32 Check_Timer;

    bool FakeDeath;

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        SweepingStrikes_Timer = 13000;
        SinisterStrike_Timer = 8000;
        Gouge_Timer = 25000;
        Kick_Timer = 18000;
        Blind_Timer = 5000;
        Check_Timer = 10000;

        FakeDeath = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZATH, NOT_STARTED);

        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI (const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //SweepingStrikes_Timer
        if (SweepingStrikes_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SWEEPINGSTRIKES);
            SweepingStrikes_Timer = urand(22000, 26000);
        }else SweepingStrikes_Timer -= diff;

        //SinisterStrike_Timer
        if (SinisterStrike_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SINISTERSTRIKE);
            SinisterStrike_Timer = urand(8000, 16000);
        }else SinisterStrike_Timer -= diff;

        //Gouge_Timer
        if (Gouge_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_GOUGE);

            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-100);

            Gouge_Timer = urand(17000, 27000);
        }else Gouge_Timer -= diff;

        //Kick_Timer
        if (Kick_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KICK);
            Kick_Timer = urand(15000, 25000);
        }else Kick_Timer -= diff;

        //Blind_Timer
        if (Blind_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_BLIND);
            Blind_Timer = urand(10000, 20000);
        }else Blind_Timer -= diff;

        //Check_Timer for the death of LorKhan and Zath.
        if (!FakeDeath && Check_Timer < diff)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                {
                    //Resurrect LorKhan
                    if (Creature* pLorKhan = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_LORKHAN)))
                    {
                        pLorKhan->SetStandState(UNIT_STAND_STATE_STAND);
                        pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pLorKhan->setFaction(14);
                        pLorKhan->SetHealth(int(pLorKhan->GetMaxHealth()*1.0));
                    }
                }

                if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
                {
                    //Resurrect Thekal
                    if (Creature* pThekal = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THEKAL)))
                    {
                        pThekal->SetStandState(UNIT_STAND_STATE_STAND);
                        pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pThekal->setFaction(14);
                        pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    }
                }
            }

            Check_Timer = 5000;
        }else Check_Timer -= diff;

        if (m_creature->GetHealthPercent() <= 5.0f)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetStandState(UNIT_STAND_STATE_SLEEP);
            m_creature->setFaction(35);
            m_creature->AttackStop();

            if (m_pInstance)
                m_pInstance->SetData(TYPE_ZATH, SPECIAL);

            FakeDeath = true;
        }

        if (!FakeDeath)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thekal(Creature* pCreature)
{
    return new boss_thekalAI(pCreature);
}

CreatureAI* GetAI_mob_zealot_lorkhan(Creature* pCreature)
{
    return new mob_zealot_lorkhanAI(pCreature);
}

CreatureAI* GetAI_mob_zealot_zath(Creature* pCreature)
{
    return new mob_zealot_zathAI(pCreature);
}

void AddSC_boss_thekal()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_thekal";
    newscript->GetAI = &GetAI_boss_thekal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zealot_lorkhan";
    newscript->GetAI = &GetAI_mob_zealot_lorkhan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zealot_zath";
    newscript->GetAI = &GetAI_mob_zealot_zath;
    newscript->RegisterSelf();
}

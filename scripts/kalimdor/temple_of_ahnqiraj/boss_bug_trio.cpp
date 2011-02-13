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
SDName: boss_kri, boss_yauj, boss_vem : The Bug Trio
SD%Complete: 100
SDComment:
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

#define SPELL_CLEAVE        26350
#define SPELL_TOXIC_VOLLEY  25812
#define SPELL_POISON_CLOUD  38718                           //Only Spell with right dmg.
#define SPELL_ENRAGE        34624                           //Changed cause 25790 is casted on gamers too. Same prob with old explosion of twin emperors.

#define SPELL_CHARGE        26561
#define SPELL_KNOCKBACK     26027

#define SPELL_HEAL      25807
#define SPELL_FEAR      19408

struct MANGOS_DLL_DECL boss_kriAI : public ScriptedAI
{
    boss_kriAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Cleave_Timer;
    uint32 ToxicVolley_Timer;
    uint32 Check_Timer;

    bool VemDead;
    bool Death;

    void Reset()
    {
        Cleave_Timer = urand(4000, 8000);
        ToxicVolley_Timer = urand(6000, 12000);
        Check_Timer = 2000;

        VemDead = false;
        Death = false;
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(DATA_BUG_TRIO_DEATH) < 2)
                                                            // Unlootable if death
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

            m_pInstance->SetData(DATA_BUG_TRIO_DEATH, 1);
        }
    }
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = urand(5000, 12000);
        }else Cleave_Timer -= diff;

        //ToxicVolley_Timer
        if (ToxicVolley_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_TOXIC_VOLLEY);
            ToxicVolley_Timer = urand(10000, 15000);
        }else ToxicVolley_Timer -= diff;

        if (m_creature->GetHealthPercent() < 5.0f && !Death)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_POISON_CLOUD);
            Death = true;
        }

        if (!VemDead)
        {
            //Checking if Vem is dead. If yes we will enrage.
            if (Check_Timer < diff)
            {
                if (m_pInstance && m_pInstance->GetData(TYPE_VEM) == DONE)
                {
                    DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                    VemDead = true;
                }
                Check_Timer = 2000;
            }else Check_Timer -=diff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_vemAI : public ScriptedAI
{
    boss_vemAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Charge_Timer;
    uint32 KnockBack_Timer;
    uint32 Enrage_Timer;

    bool Enraged;

    void Reset()
    {
        Charge_Timer = urand(15000, 27000);
        KnockBack_Timer = urand(8000, 20000);
        Enrage_Timer = 120000;

        Enraged = false;
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_VEM, DONE);

            // Unlootable if death
            if (m_pInstance->GetData(DATA_BUG_TRIO_DEATH) < 2)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

            m_pInstance->SetData(DATA_BUG_TRIO_DEATH, 1);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Charge_Timer
        if (Charge_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target, SPELL_CHARGE);

            Charge_Timer = urand(8000, 16000);
        }else Charge_Timer -= diff;

        //KnockBack_Timer
        if (KnockBack_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_KNOCKBACK);
            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);
            KnockBack_Timer = urand(15000, 25000);
        }else KnockBack_Timer -= diff;

        //Enrage_Timer
        if (!Enraged && Enrage_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_ENRAGE);
            Enraged = true;
        }else Charge_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_yaujAI : public ScriptedAI
{
    boss_yaujAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Heal_Timer;
    uint32 Fear_Timer;
    uint32 Check_Timer;

    bool VemDead;

    void Reset()
    {
        Heal_Timer = urand(25000, 40000);
        Fear_Timer = urand(12000, 24000);
        Check_Timer = 2000;

        VemDead = false;
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(DATA_BUG_TRIO_DEATH) < 2)
                                                            // Unlootable if death
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            m_pInstance->SetData(DATA_BUG_TRIO_DEATH, 1);
        }

        for(int i = 0; i < 10; ++i)
        {
            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
            Creature* Summoned = m_creature->SummonCreature(15621,m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,90000);
            if (Summoned && target)
                Summoned->AI()->AttackStart(target);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Fear_Timer
        if (Fear_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FEAR);
            DoResetThreat();
            Fear_Timer = 20000;
        }else Fear_Timer -= diff;

        //Casting Heal to other twins or herself.
        if (Heal_Timer < diff)
        {
            if (m_pInstance)
            {
                Creature* pKri = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KRI));
                Creature* pVem = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VEM));

                switch(urand(0, 2))
                {
                    case 0:
                        if (pKri)
                            DoCastSpellIfCan(pKri, SPELL_HEAL);
                        break;
                    case 1:
                        if (pVem)
                            DoCastSpellIfCan(pVem, SPELL_HEAL);
                        break;
                    case 2:
                        DoCastSpellIfCan(m_creature, SPELL_HEAL);
                        break;
                }
            }

            Heal_Timer = urand(15000, 30000);
        }else Heal_Timer -= diff;

        //Checking if Vem is dead. If yes we will enrage.
        if (Check_Timer < diff)
        {
            if (!VemDead)
            {
                if (m_pInstance)
                {
                    if (m_pInstance->GetData(TYPE_VEM) == DONE)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
                        VemDead = true;
                    }
                }
            }
            Check_Timer = 2000;
        }else Check_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_yauj(Creature* pCreature)
{
    return new boss_yaujAI(pCreature);
}

CreatureAI* GetAI_boss_vem(Creature* pCreature)
{
    return new boss_vemAI(pCreature);
}

CreatureAI* GetAI_boss_kri(Creature* pCreature)
{
    return new boss_kriAI(pCreature);
}

void AddSC_bug_trio()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kri";
    newscript->GetAI = &GetAI_boss_kri;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vem";
    newscript->GetAI = &GetAI_boss_vem;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_yauj";
    newscript->GetAI = &GetAI_boss_yauj;
    newscript->RegisterSelf();
}

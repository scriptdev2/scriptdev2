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
#include "gruuls_lair.h"

enum
{
    SAY_AGGRO                   = -1565000,
    SAY_ENRAGE                  = -1565001,
    SAY_OGRE_DEATH1             = -1565002,
    SAY_OGRE_DEATH2             = -1565003,
    SAY_OGRE_DEATH3             = -1565004,
    SAY_OGRE_DEATH4             = -1565005,
    SAY_SLAY1                   = -1565006,
    SAY_SLAY2                   = -1565007,
    SAY_SLAY3                   = -1565008,
    SAY_DEATH                   = -1565009,

    // High King Maulgar Spells
    SPELL_ARCING_SMASH          = 39144,
    SPELL_MIGHTY_BLOW           = 33230,
    SPELL_WHIRLWIND             = 33238,
    SPELL_FLURRY                = 33232,
    SPELL_CHARGE                = 26561,
    SPELL_FEAR                  = 16508,

    // Olm the Summoner Spells
    SPELL_DARK_DECAY            = 33129,
    SPELL_DEATH_COIL            = 33130,
    SPELL_SUMMON_WILD_FELHUNTER = 33131,

    //Kiggler the Crazed Spells
    SPELL_GREATER_POLYMORPH     = 33173,
    SPELL_LIGHTNING_BOLT        = 36152,
    SPELL_ARCANE_SHOCK          = 33175,
    SPELL_ARCANE_EXPLOSION      = 33237,

    //Blindeye the Seer Spells
    SPELL_GREATER_PW_SHIELD     = 33147,
    SPELL_HEAL                  = 33144,
    SPELL_PRAYEROFHEALING       = 33152,

    //Krosh Firehand Spells
    SPELL_GREATER_FIREBALL      = 33051,
    SPELL_SPELLSHIELD           = 33054,
    SPELL_BLAST_WAVE            = 33061,

    MAX_COUNCIL                 = 4
};

const float DISTANCE_KIGGLER    = 20.0f;
const float DISTANCE_KROSH      = 30.0f;

//High King Maulgar AI
struct MANGOS_DLL_DECL boss_high_king_maulgarAI : public ScriptedAI
{
    boss_high_king_maulgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        memset(&m_auiCouncil, 0, sizeof(m_auiCouncil));
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiArcingSmash_Timer;
    uint32 m_uiMightyBlow_Timer;
    uint32 m_uiWhirlwind_Timer;
    uint32 m_uiCharge_Timer;
    uint32 m_uiFear_Timer;
    uint32 m_uiCouncilDeathCount;

    bool m_bPhase2;

    uint64 m_auiCouncil[MAX_COUNCIL];                       // Council GUIDs

    void Reset()
    {
        m_uiArcingSmash_Timer   = urand(8000, 14000);
        m_uiMightyBlow_Timer    = urand(15000, 25000);
        m_uiWhirlwind_Timer     = 30000;
        m_uiCharge_Timer        = 2000;
        m_uiFear_Timer          = urand(10000, 25000);
        m_uiCouncilDeathCount   = 0;
        m_bPhase2               = false;
    }

    void JustReachedHome()
    {
        for (uint8 i = 0; i < MAX_COUNCIL; ++i)
        {
            if (Creature* pCreature = (Creature*)Unit::GetUnit((*m_creature), m_auiCouncil[i]))
            {
                if (!pCreature->isAlive())
                    pCreature->Respawn();
                else if (pCreature->getVictim())
                    pCreature->AI()->EnterEvadeMode();
            }
        }

        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == IN_PROGRESS)
            m_pInstance->SetData(TYPE_MAULGAR_EVENT, NOT_STARTED);
    }

    void KilledUnit()
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        //we risk being DONE before adds are in fact dead
        m_pInstance->SetData(TYPE_MAULGAR_EVENT, DONE);
    }

    void Aggro(Unit *pWho)
    {
        if (!m_pInstance)
            return;

        GetCouncil();

        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->CallForHelp(50.0f);

        if (m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
            m_pInstance->SetData(TYPE_MAULGAR_EVENT, IN_PROGRESS);
    }

    void GetCouncil()
    {
        if (!m_pInstance)
            return;

        //get council member's guid to respawn them if needed
        m_auiCouncil[0] = m_pInstance->GetData64(DATA_KIGGLER);
        m_auiCouncil[1] = m_pInstance->GetData64(DATA_BLINDEYE);
        m_auiCouncil[2] = m_pInstance->GetData64(DATA_OLM);
        m_auiCouncil[3] = m_pInstance->GetData64(DATA_KROSH);
    }

    void EventCouncilDeath()
    {
        switch(++m_uiCouncilDeathCount)
        {
            case 1: DoScriptText(SAY_OGRE_DEATH1, m_creature); break;
            case 2: DoScriptText(SAY_OGRE_DEATH2, m_creature); break;
            case 3: DoScriptText(SAY_OGRE_DEATH3, m_creature); break;
            case 4: DoScriptText(SAY_OGRE_DEATH4, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
        {
            EnterEvadeMode();
            return;
        }

        //m_uiArcingSmash_Timer
        if (m_uiArcingSmash_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCING_SMASH);
            m_uiArcingSmash_Timer = urand(8000, 12000);
        }
        else
            m_uiArcingSmash_Timer -= uiDiff;

        //m_uiWhirlwind_Timer
        if (m_uiWhirlwind_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_WHIRLWIND);
            m_uiWhirlwind_Timer = urand(30000, 40000);
        }
        else
            m_uiWhirlwind_Timer -= uiDiff;

        //m_uiMightyBlow_Timer
        if (m_uiMightyBlow_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlow_Timer = urand(20000, 35000);
        }
        else
            m_uiMightyBlow_Timer -= uiDiff;

        //Entering Phase 2
        if (!m_bPhase2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            m_bPhase2 = true;
            DoScriptText(SAY_ENRAGE, m_creature);
            DoCast(m_creature, SPELL_FLURRY);
        }

        if (m_bPhase2)
        {
            //m_uiCharge_Timer
            if (m_uiCharge_Timer < uiDiff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                    DoCast(pTarget, SPELL_CHARGE);

                m_uiCharge_Timer = urand(14000, 20000);
            }
            else
                m_uiCharge_Timer -= uiDiff;

            //m_uiFear_Timer
            if (m_uiFear_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_FEAR);
                m_uiFear_Timer = urand(20000, 35000);
            }
            else
                m_uiFear_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

// Base AI for every council member
struct MANGOS_DLL_DECL Council_Base_AI : public ScriptedAI
{
    Council_Base_AI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    void JustReachedHome()
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == IN_PROGRESS)
            m_pInstance->SetData(TYPE_MAULGAR_EVENT, NOT_STARTED);
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
            m_pInstance->SetData(TYPE_MAULGAR_EVENT, IN_PROGRESS);

        m_creature->CallForHelp(50.0f);
    }

    void JustDied(Unit* pVictim)
    {
        if (!m_pInstance)
            return;

        Creature* pMaulgar = (Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_MAULGAR));

        if (pMaulgar->isAlive())
        {
            if (boss_high_king_maulgarAI* pMaulgarAI = dynamic_cast<boss_high_king_maulgarAI*>(pMaulgar->AI()))
                pMaulgarAI->EventCouncilDeath();
        }
    }
};

//Olm The Summoner AI
struct MANGOS_DLL_DECL boss_olm_the_summonerAI : public Council_Base_AI
{
    boss_olm_the_summonerAI(Creature* pCreature) : Council_Base_AI(pCreature) {Reset();}

    uint32 m_uiDarkDecay_Timer;
    uint32 m_uiDeathCoil_Timer;
    uint32 m_uiSummon_Timer;

    void Reset()
    {
        m_uiDarkDecay_Timer = 18000;
        m_uiDeathCoil_Timer = 14000;
        m_uiSummon_Timer    = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
        {
            EnterEvadeMode();
            return;
        }

        //m_uiDarkDecay_Timer
        if (m_uiDarkDecay_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_DARK_DECAY);
            m_uiDarkDecay_Timer = 20000;
        }
        else
            m_uiDarkDecay_Timer -= uiDiff;

        //m_uiDeathCoil_Timer
        if (m_uiDeathCoil_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_DEATH_COIL);
            m_uiDeathCoil_Timer = urand(8000, 13000);
        }
        else
            m_uiDeathCoil_Timer -= uiDiff;

        //m_uiSummon_Timer
        if (m_uiSummon_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SUMMON_WILD_FELHUNTER);
            m_uiSummon_Timer = urand(25000, 35000);
        }
        else
            m_uiSummon_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Kiggler The Crazed AI
struct MANGOS_DLL_DECL boss_kiggler_the_crazedAI : public Council_Base_AI
{
    boss_kiggler_the_crazedAI(Creature* pCreature) : Council_Base_AI(pCreature) {Reset();}

    uint32 m_uiGreatherPolymorph_Timer;
    uint32 m_uiLightningBolt_Timer;
    uint32 m_uiArcaneShock_Timer;
    uint32 m_uiArcaneExplosion_Timer;

    void Reset()
    {
        m_uiGreatherPolymorph_Timer = 15000;
        m_uiLightningBolt_Timer = 10000;
        m_uiArcaneShock_Timer = 20000;
        m_uiArcaneExplosion_Timer = 30000;
    }

    void SpellHitTarget(Unit* pVictim, const SpellEntry* pSpell)
    {
        // Spell currently not supported by core. Knock back effect should lower threat
        // Workaround in script:
        if (pSpell->Id == SPELL_ARCANE_EXPLOSION)
        {
            if (pVictim->GetTypeId() != TYPEID_PLAYER)
                return;

            m_creature->getThreatManager().modifyThreatPercent(pVictim,-75);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, false))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, DISTANCE_KIGGLER);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
        {
            EnterEvadeMode();
            return;
        }

        if (m_uiGreatherPolymorph_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_GREATER_POLYMORPH);
            m_uiGreatherPolymorph_Timer = urand(15000, 20000);
        }
        else
            m_uiGreatherPolymorph_Timer -= uiDiff;

        //LightningBolt_Timer
        if (m_uiLightningBolt_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_LIGHTNING_BOLT);
            m_uiLightningBolt_Timer = urand(2500, 4000);
        }
        else
            m_uiLightningBolt_Timer -= uiDiff;

        //ArcaneShock_Timer
        if (m_uiArcaneShock_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_SHOCK);
            m_uiArcaneShock_Timer = urand(15000, 20000);
        }
        else
            m_uiArcaneShock_Timer -= uiDiff;

        //ArcaneExplosion_Timer
        if (m_uiArcaneExplosion_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            m_uiArcaneExplosion_Timer = 30000;
        }
        else
            m_uiArcaneExplosion_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Blindeye The Seer AI
struct MANGOS_DLL_DECL boss_blindeye_the_seerAI : public Council_Base_AI
{
    boss_blindeye_the_seerAI(Creature* pCreature) : Council_Base_AI(pCreature) {Reset();}

    uint32 m_uiGreaterPowerWordShield_Timer;
    uint32 m_uiHeal_Timer;
    uint32 m_uiPrayerofHealing_Timer;

    void Reset()
    {
        m_uiGreaterPowerWordShield_Timer    = 5000;
        m_uiHeal_Timer                      = urand(25000, 40000);
        m_uiPrayerofHealing_Timer           = urand(45000, 55000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
        {
            EnterEvadeMode();
            return;
        }

        //m_uiGreaterPowerWordShield_Timer
        if (m_uiGreaterPowerWordShield_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_GREATER_PW_SHIELD);
            m_uiGreaterPowerWordShield_Timer = urand(30000, 40000);
        }
        else
            m_uiGreaterPowerWordShield_Timer -= uiDiff;

        //m_uiHeal_Timer
        if (m_uiHeal_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_HEAL);
            m_uiHeal_Timer = urand(15000, 40000);
        }
        else
            m_uiHeal_Timer -= uiDiff;

        //PrayerofHealing_Timer
        if (m_uiPrayerofHealing_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_PRAYEROFHEALING);
            m_uiPrayerofHealing_Timer = urand(35000, 50000);
        }
        else
            m_uiPrayerofHealing_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Krosh Firehand AI
struct MANGOS_DLL_DECL boss_krosh_firehandAI : public Council_Base_AI
{
    boss_krosh_firehandAI(Creature* pCreature) : Council_Base_AI(pCreature) {Reset();}

    uint32 m_uiGreaterFireball_Timer;
    uint32 m_uiSpellShield_Timer;
    uint32 m_uiBlastWave_Timer;

    void Reset()
    {
        m_uiGreaterFireball_Timer = 4000;
        m_uiSpellShield_Timer = 1000;
        m_uiBlastWave_Timer = 12000;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, DISTANCE_KROSH);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //someone evaded!
        if (m_pInstance && m_pInstance->GetData(TYPE_MAULGAR_EVENT) == NOT_STARTED)
        {
            EnterEvadeMode();
            return;
        }

        //m_uiGreaterFireball_Timer
        if (m_uiGreaterFireball_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_GREATER_FIREBALL);
            m_uiGreaterFireball_Timer = 3200;
        }
        else
            m_uiGreaterFireball_Timer -= uiDiff;

        //SpellShield_Timer
        if (m_uiSpellShield_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature->getVictim(), SPELL_SPELLSHIELD);
            m_uiSpellShield_Timer = 30000;
        }
        else
            m_uiSpellShield_Timer -= uiDiff;

        //BlastWave_Timer
        if (m_uiBlastWave_Timer < uiDiff)
        {
            bool bInRange = false;
            Unit* pTarget = NULL;

            std::list<HostileReference*>& threatlist = m_creature->getThreatManager().getThreatList();
            for (std::list<HostileReference*>::iterator i = threatlist.begin(); i!= threatlist.end(); ++i)
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                if (pUnit && pUnit->IsWithinDistInMap(m_creature, 15.0f))
                {
                    bInRange = true;
                    pTarget = pUnit;
                    break;
                }
            }

            m_uiBlastWave_Timer = 6000;

            if (bInRange)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCast(pTarget, SPELL_BLAST_WAVE);
            }
        }
        else
            m_uiBlastWave_Timer -= uiDiff;
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

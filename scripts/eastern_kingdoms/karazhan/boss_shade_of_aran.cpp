/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Shade_of_Aran
SD%Complete: 95
SDComment: Flame wreath missing cast animation, mods won't trigger.
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"
#include "GameObject.h"

enum
{
    SAY_AGGRO1                  = -1532073,
    SAY_AGGRO2                  = -1532074,
    SAY_AGGRO3                  = -1532075,
    SAY_FLAMEWREATH1            = -1532076,
    SAY_FLAMEWREATH2            = -1532077,
    SAY_BLIZZARD1               = -1532078,
    SAY_BLIZZARD2               = -1532079,
    SAY_EXPLOSION1              = -1532080,
    SAY_EXPLOSION2              = -1532081,
    SAY_DRINK                   = -1532082,                 //Low Mana / AoE Pyroblast
    SAY_ELEMENTALS              = -1532083,
    SAY_KILL1                   = -1532084,
    SAY_KILL2                   = -1532085,
    SAY_TIMEOVER                = -1532086,
    SAY_DEATH                   = -1532087,
    SAY_ATIESH                  = -1532088,                 //Atiesh is equipped by a raid member

    //Spells
    SPELL_FROSTBOLT             = 29954,
    SPELL_FIREBALL              = 29953,
    SPELL_ARCMISSLE             = 29955,
    SPELL_CHAINSOFICE           = 29991,
    SPELL_DRAGONSBREATH         = 29964,
    SPELL_MASSSLOW              = 30035,
    SPELL_FLAME_WREATH          = 29946,
    SPELL_AOE_CS                = 29961,
    SPELL_PLAYERPULL            = 32265,
    SPELL_AEXPLOSION            = 29973,
    SPELL_MASS_POLY             = 29963,
    SPELL_BLINK_CENTER          = 29967,
    SPELL_ELEMENTALS            = 29962,
    SPELL_CONJURE               = 29975,
    SPELL_DRINK                 = 30024,
    SPELL_POTION                = 32453,
    SPELL_AOE_PYROBLAST         = 29978,

    SPELL_EXPLOSION             = 20476,
    SPELL_KNOCKBACK_500         = 11027,

    //Creature Spells
    SPELL_CIRCULAR_BLIZZARD     = 29951,                    //29952 is the REAL circular blizzard that leaves persistant blizzards that last for 10 seconds
    SPELL_WATERBOLT             = 31012,
    SPELL_SHADOW_PYRO           = 29978,

    //Creatures
    NPC_WATER_ELEMENTAL         = 17167,
    NPC_SHADOW_OF_ARAN          = 18254,
    NPC_ARAN_BLIZZARD           = 17161
};

enum SuperSpell
{
    SUPER_FLAME = 0,
    SUPER_BLIZZARD,
    SUPER_AE,
};

struct MANGOS_DLL_DECL boss_aranAI : public ScriptedAI
{
    boss_aranAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSecondarySpell_Timer;
    uint32 m_uiNormalCast_Timer;
    uint32 m_uiSuperCast_Timer;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiCloseDoor_Timer;                                  // Don't close the door right on aggro in case some people are still entering.

    uint8 m_uiLastSuperSpell;

    uint32 m_uiFlameWreath_Timer;
    uint32 m_uiFlameWreathCheck_Timer;
    uint64 m_uiFlameWreathTarget[3];
    float m_fFWTargPosX[3];
    float m_fFWTargPosY[3];

    uint32 m_uiCurrentNormalSpell;
    uint32 m_uiArcaneCooldown;
    uint32 m_uiFireCooldown;
    uint32 m_uiFrostCooldown;

    uint32 m_uiDrinkInturrupt_Timer;

    bool m_bElementalsSpawned;
    bool m_bDrinking;
    bool m_bDrinkInturrupted;

    void Reset()
    {
        m_uiSecondarySpell_Timer = 5000;
        m_uiNormalCast_Timer = 0;
        m_uiSuperCast_Timer = 35000;
        m_uiBerserk_Timer = 720000;
        m_uiCloseDoor_Timer = 15000;

        m_uiLastSuperSpell = urand(0, 2);

        m_uiFlameWreath_Timer = 0;
        m_uiFlameWreathCheck_Timer = 0;

        m_uiCurrentNormalSpell = 0;
        m_uiArcaneCooldown = 0;
        m_uiFireCooldown = 0;
        m_uiFrostCooldown = 0;

        m_uiDrinkInturrupt_Timer = 10000;

        m_bElementalsSpawned = false;
        m_bDrinking = false;
        m_bDrinkInturrupted = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARAN, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pVictim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARAN, DONE);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARAN, IN_PROGRESS);
    }

    void FlameWreathEffect()
    {
        std::vector<Unit*> targets;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        if (tList.empty())
            return;

        //store the threat list in a different container
        for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
        {
            Unit* pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());

            //only on alive players
            if (pTarget && pTarget->isAlive() && pTarget->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(pTarget);
        }

        //cut down to size if we have more than 3 targets
        while(targets.size() > 3)
            targets.erase(targets.begin()+rand()%targets.size());

        uint32 i = 0;
        for(std::vector<Unit*>::iterator itr = targets.begin(); itr!= targets.end(); ++itr)
        {
            if (*itr)
            {
                m_uiFlameWreathTarget[i] = (*itr)->GetGUID();
                m_fFWTargPosX[i] = (*itr)->GetPositionX();
                m_fFWTargPosY[i] = (*itr)->GetPositionY();
                m_creature->CastSpell((*itr), SPELL_FLAME_WREATH, true);
                ++i;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCloseDoor_Timer)
        {
            if (m_uiCloseDoor_Timer <= uiDiff)
            {
                if (m_pInstance)
                {
                    if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_LIBRARY_DOOR)))
                        pDoor->SetGoState(GO_STATE_READY);

                    m_uiCloseDoor_Timer = 0;
                }
            }
            else
                m_uiCloseDoor_Timer -= uiDiff;
        }

        //Cooldowns for casts
        if (m_uiArcaneCooldown)
        {
            if (m_uiArcaneCooldown >= uiDiff)
                m_uiArcaneCooldown -= uiDiff;
            else
                m_uiArcaneCooldown = 0;
        }

        if (m_uiFireCooldown)
        {
            if (m_uiFireCooldown >= uiDiff)
                m_uiFireCooldown -= uiDiff;
            else
                m_uiFireCooldown = 0;
        }

        if (m_uiFrostCooldown)
        {
            if (m_uiFrostCooldown >= uiDiff)
                m_uiFrostCooldown -= uiDiff;
            else
                m_uiFrostCooldown = 0;
        }

        if (!m_bDrinking && m_creature->GetMaxPower(POWER_MANA) && (m_creature->GetPower(POWER_MANA)*100 / m_creature->GetMaxPower(POWER_MANA)) < 20)
        {
            m_bDrinking = true;
            m_creature->InterruptNonMeleeSpells(false);

            DoScriptText(SAY_DRINK, m_creature);

            if (!m_bDrinkInturrupted)
            {
                m_creature->CastSpell(m_creature, SPELL_MASS_POLY, true);
                m_creature->CastSpell(m_creature, SPELL_CONJURE, false);
                m_creature->CastSpell(m_creature, SPELL_DRINK, false);
                m_creature->SetStandState(UNIT_STAND_STATE_SIT);
                m_uiDrinkInturrupt_Timer = 10000;
            }
        }

        //Drink Inturrupt
        if (m_bDrinking && m_bDrinkInturrupted)
        {
            m_bDrinking = false;
            m_creature->RemoveAurasDueToSpell(SPELL_DRINK);
            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
            m_creature->SetPower(POWER_MANA, m_creature->GetMaxPower(POWER_MANA)-32000);
            m_creature->CastSpell(m_creature, SPELL_POTION, false);
        }

        //Drink Inturrupt Timer
        if (m_bDrinking && !m_bDrinkInturrupted)
        {
            if (m_uiDrinkInturrupt_Timer >= uiDiff)
                m_uiDrinkInturrupt_Timer -= uiDiff;
            else
            {
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                m_creature->CastSpell(m_creature, SPELL_POTION, true);
                m_creature->CastSpell(m_creature, SPELL_AOE_PYROBLAST, false);
                m_bDrinkInturrupted = true;
                m_bDrinking = false;
            }
        }

        //Don't execute any more code if we are drinking
        if (m_bDrinking)
            return;

        //Normal casts
        if (m_uiNormalCast_Timer < uiDiff)
        {
            if (!m_creature->IsNonMeleeSpellCasted(false))
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if (!pTarget)
                    return;

                uint32 auiSpells[3];
                uint8 uiAvailableSpells = 0;

                //Check for what spells are not on cooldown
                if (!m_uiArcaneCooldown)
                    auiSpells[uiAvailableSpells++] = SPELL_ARCMISSLE;
                if (!m_uiFireCooldown)
                    auiSpells[uiAvailableSpells++] = SPELL_FIREBALL;
                if (!m_uiFrostCooldown)
                    auiSpells[uiAvailableSpells++] = SPELL_FROSTBOLT;

                //If no available spells wait 1 second and try again
                if (uiAvailableSpells)
                {
                    m_uiCurrentNormalSpell = auiSpells[rand() % uiAvailableSpells];
                    DoCastSpellIfCan(pTarget, m_uiCurrentNormalSpell);
                }
            }
            m_uiNormalCast_Timer = 1000;
        }
        else
            m_uiNormalCast_Timer -= uiDiff;

        if (m_uiSecondarySpell_Timer < uiDiff)
        {
            switch(urand(0, 1))
            {
                case 0:
                    DoCastSpellIfCan(m_creature, SPELL_AOE_CS);
                    break;
                case 1:
                    if (Unit* pUnit = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pUnit, SPELL_CHAINSOFICE);
                    break;
            }
            m_uiSecondarySpell_Timer = urand(5000, 20000);
        }
        else
            m_uiSecondarySpell_Timer -= uiDiff;

        if (m_uiSuperCast_Timer < uiDiff)
        {
            uint8 auiAvailable[2];

            switch (m_uiLastSuperSpell)
            {
                case SUPER_AE:
                    auiAvailable[0] = SUPER_FLAME;
                    auiAvailable[1] = SUPER_BLIZZARD;
                    break;
                case SUPER_FLAME:
                    auiAvailable[0] = SUPER_AE;
                    auiAvailable[1] = SUPER_BLIZZARD;
                    break;
                case SUPER_BLIZZARD:
                    auiAvailable[0] = SUPER_FLAME;
                    auiAvailable[1] = SUPER_AE;
                    break;
            }

            m_uiLastSuperSpell = auiAvailable[urand(0, 2)];

            switch (m_uiLastSuperSpell)
            {
                case SUPER_AE:
                    DoScriptText(urand(0, 1) ? SAY_EXPLOSION1 : SAY_EXPLOSION2, m_creature);

                    m_creature->CastSpell(m_creature, SPELL_BLINK_CENTER, true);
                    m_creature->CastSpell(m_creature, SPELL_PLAYERPULL, true);
                    m_creature->CastSpell(m_creature, SPELL_MASSSLOW, true);
                    m_creature->CastSpell(m_creature, SPELL_AEXPLOSION, false);
                    break;

                case SUPER_FLAME:
                    DoScriptText(urand(0, 1) ? SAY_FLAMEWREATH1 : SAY_FLAMEWREATH2, m_creature);

                    m_uiFlameWreath_Timer = 20000;
                    m_uiFlameWreathCheck_Timer = 500;

                    memset(&m_uiFlameWreathTarget, 0, sizeof(m_uiFlameWreathTarget));

                    FlameWreathEffect();
                    break;

                case SUPER_BLIZZARD:
                    DoScriptText(urand(0, 1) ? SAY_BLIZZARD1 : SAY_BLIZZARD2, m_creature);

                    if (Creature* pSpawn = m_creature->SummonCreature(NPC_ARAN_BLIZZARD, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 25000))
                    {
                        pSpawn->setFaction(m_creature->getFaction());
                        pSpawn->CastSpell(pSpawn, SPELL_CIRCULAR_BLIZZARD, false);
                    }
                    break;
            }

            m_uiSuperCast_Timer = urand(35000, 40000);
        }
        else
            m_uiSuperCast_Timer -= uiDiff;

        if (!m_bElementalsSpawned && m_creature->GetHealthPercent() < 40.0f)
        {
            m_bElementalsSpawned = true;

            for (uint32 i = 0; i < 4; ++i)
            {
                if (Creature* pElemental = m_creature->SummonCreature(NPC_WATER_ELEMENTAL, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 90000))
                {
                    pElemental->Attack(m_creature->getVictim(), true);
                    pElemental->setFaction(m_creature->getFaction());
                }
            }

            DoScriptText(SAY_ELEMENTALS, m_creature);
        }

        if (m_uiBerserk_Timer < uiDiff)
        {
            for (uint32 i = 0; i < 5; ++i)
            {
                if (Creature* pShadow = m_creature->SummonCreature(NPC_SHADOW_OF_ARAN, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000))
                {
                    pShadow->Attack(m_creature->getVictim(), true);
                    pShadow->setFaction(m_creature->getFaction());
                }
            }

            DoScriptText(SAY_TIMEOVER, m_creature);

            m_uiBerserk_Timer = 60000;
        }
        else
            m_uiBerserk_Timer -= uiDiff;

        //Flame Wreath check
        if (m_uiFlameWreath_Timer)
        {
            if (m_uiFlameWreath_Timer >= uiDiff)
                m_uiFlameWreath_Timer -= uiDiff;
            else
                m_uiFlameWreath_Timer = 0;

            if (m_uiFlameWreathCheck_Timer < uiDiff)
            {
                for (uint32 i = 0; i < 3; ++i)
                {
                    if (!m_uiFlameWreathTarget[i])
                        continue;

                    Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiFlameWreathTarget[i]);

                    if (pPlayer && !pPlayer->IsWithinDist2d(m_fFWTargPosX[i], m_fFWTargPosY[i], 3.0f))
                    {
                        pPlayer->CastSpell(pPlayer, SPELL_EXPLOSION, true, 0, 0, m_creature->GetGUID());
                        pPlayer->CastSpell(pPlayer, SPELL_KNOCKBACK_500, true);
                        m_uiFlameWreathTarget[i] = 0;
                    }
                }
                m_uiFlameWreathCheck_Timer = 500;
            }
            else
                m_uiFlameWreathCheck_Timer -= uiDiff;
        }

        if (m_uiArcaneCooldown && m_uiFireCooldown && m_uiFrostCooldown)
            DoMeleeAttackIfReady();
    }

    void DamageTaken(Unit* pAttacker, uint32 &damage)
    {
        if (!m_bDrinkInturrupted && m_bDrinking && damage)
            m_bDrinkInturrupted = true;
    }

    void SpellHit(Unit* pAttacker, const SpellEntry* Spell)
    {
        //We only care about inturrupt effects and only if they are durring a spell currently being casted
        if ((Spell->Effect[0]!=SPELL_EFFECT_INTERRUPT_CAST &&
            Spell->Effect[1]!=SPELL_EFFECT_INTERRUPT_CAST &&
            Spell->Effect[2]!=SPELL_EFFECT_INTERRUPT_CAST) || !m_creature->IsNonMeleeSpellCasted(false))
            return;

        //Inturrupt effect
        m_creature->InterruptNonMeleeSpells(false);

        //Normally we would set the cooldown equal to the spell duration
        //but we do not have access to the DurationStore

        switch (m_uiCurrentNormalSpell)
        {
            case SPELL_ARCMISSLE: m_uiArcaneCooldown = 5000; break;
            case SPELL_FIREBALL:  m_uiFireCooldown = 5000; break;
            case SPELL_FROSTBOLT: m_uiFrostCooldown = 5000; break;
        }
    }
};

struct MANGOS_DLL_DECL water_elementalAI : public ScriptedAI
{
    water_elementalAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiCast_Timer;

    void Reset()
    {
        m_uiCast_Timer = urand(2000, 5000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCast_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_WATERBOLT);
            m_uiCast_Timer = urand(2000, 5000);
        }
        else
            m_uiCast_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_aran(Creature* pCreature)
{
    return new boss_aranAI(pCreature);
}

CreatureAI* GetAI_water_elemental(Creature* pCreature)
{
    return new water_elementalAI(pCreature);
}

void AddSC_boss_shade_of_aran()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_shade_of_aran";
    newscript->GetAI = &GetAI_boss_aran;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_aran_elemental";
    newscript->GetAI = &GetAI_water_elemental;
    newscript->RegisterSelf();
}

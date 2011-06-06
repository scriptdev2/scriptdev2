/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Prince_Malchezzar
SD%Complete: 100
SDComment:
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO               = -1532091,
    SAY_AXE_TOSS1           = -1532092,
    SAY_AXE_TOSS2           = -1532093,
    SAY_SPECIAL1            = -1532094,
    SAY_SPECIAL2            = -1532095,
    SAY_SPECIAL3            = -1532096,
    SAY_SLAY1               = -1532097,
    SAY_SLAY2               = -1532098,
    SAY_SLAY3               = -1532099,
    SAY_SUMMON1             = -1532100,
    SAY_SUMMON2             = -1532101,
    SAY_DEATH               = -1532102,
};

// 18 Coordinates for Infernal spawns
struct InfernalPoint
{
    float x,y;
};

#define INFERNAL_Z  275.5

static InfernalPoint InfernalPoints[] =
{
    {-10922.8f, -1985.2f},
    {-10916.2f, -1996.2f},
    {-10932.2f, -2008.1f},
    {-10948.8f, -2022.1f},
    {-10958.7f, -1997.7f},
    {-10971.5f, -1997.5f},
    {-10990.8f, -1995.1f},
    {-10989.8f, -1976.5f},
    {-10971.6f, -1973.0f},
    {-10955.5f, -1974.0f},
    {-10939.6f, -1969.8f},
    {-10958.0f, -1952.2f},
    {-10941.7f, -1954.8f},
    {-10943.1f, -1988.5f},
    {-10948.8f, -2005.1f},
    {-10984.0f, -2019.3f},
    {-10932.8f, -1979.6f},
    {-10935.7f, -1996.0f}
};

enum
{
    TOTAL_INFERNAL_POINTS       = 18,

//Enfeeble is supposed to reduce hp to 1 and then heal player back to full when it ends
//Along with reducing healing and regen while enfeebled to 0%
//This spell effect will only reduce healing

    SPELL_ENFEEBLE              = 30843,                    // Enfeeble during m_uiPhase 1 and 2
    SPELL_ENFEEBLE_EFFECT       = 41624,

    SPELL_SHADOWNOVA            = 30852,                    // Shadownova used during all phases
    SPELL_SW_PAIN               = 30854,                    // Shadow word pain during m_uiPhase 1 and 3 (different targeting rules though)
    SPELL_THRASH_PASSIVE        = 12787,                    // Extra attack chance during m_uiPhase 2
    SPELL_SUNDER_ARMOR          = 30901,                    // Sunder armor during m_uiPhase 2
    SPELL_THRASH_AURA           = 12787,                    // Passive proc chance for thrash
    SPELL_EQUIP_AXES            = 30857,                    // Visual for axe equiping
    SPELL_AMPLIFY_DAMAGE        = 39095,                    // Amplifiy during m_uiPhase 3
    SPELL_CLEAVE                = 30131,                    // Same as Nightbane.
    SPELL_HELLFIRE              = 30859,                    // Infenals' hellfire aura
    NETHERSPITE_INFERNAL        = 17646,                    // The netherspite infernal creature
    MALCHEZARS_AXE              = 17650,                    // Malchezar's m_aAxeGuid (creatures), summoned during m_uiPhase 3

    INFERNAL_MODEL_INVISIBLE    = 11686,                    // Infernal Effects
    SPELL_INFERNAL_RELAY        = 30834,

    EQUIP_ID_AXE                = 33542,                    // Axes info
};

//---------Infernal code first
struct MANGOS_DLL_DECL netherspite_infernalAI : public ScriptedAI
{
    netherspite_infernalAI(Creature* pCreature) : ScriptedAI(pCreature),
        m_uiHellfireTimer(0),
        m_uiCleanupTimer(0),
        pPoint(NULL)
    {
        Reset();
    }

    uint32 m_uiHellfireTimer;
    uint32 m_uiCleanupTimer;
    ObjectGuid m_malchezaarGuid;
    InfernalPoint* pPoint;

    void Reset() {}
    void MoveInLineOfSight(Unit* pWho) {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiHellfireTimer)
        {
            if (m_uiHellfireTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_HELLFIRE) == CAST_OK)
                    m_uiHellfireTimer = 0;
            }
            else
                m_uiHellfireTimer -= uiDiff;
        }

        if (m_uiCleanupTimer)
        {
            if (m_uiCleanupTimer <= uiDiff)
            {
                Cleanup();
                m_uiCleanupTimer = 0;
            }
            else
                m_uiCleanupTimer -= uiDiff;
        }
    }

    void KilledUnit(Unit *who)
    {
        if (Creature* pMalchezaar = m_creature->GetMap()->GetCreature(m_malchezaarGuid))
            pMalchezaar->AI()->KilledUnit(who);
    }

    void SpellHit(Unit* pWho, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_INFERNAL_RELAY)
        {
            m_creature->SetDisplayId(m_creature->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_uiHellfireTimer = 4000;
            m_uiCleanupTimer = 170000;
        }
    }

    void DamageTaken(Unit* pDealer, uint32& uiDamage)
    {
        if (pDealer->GetObjectGuid() != m_malchezaarGuid)
            uiDamage = 0;
    }

    void Cleanup();                                         // below ...
};

struct MANGOS_DLL_DECL boss_malchezaarAI : public ScriptedAI
{
    boss_malchezaarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiEnfeebleTimer;
    uint32 m_uiEnfeebleResetTimer;
    uint32 m_uiShadowNovaTimer;
    uint32 m_uiSWPainTimer;
    uint32 SunderArmorTimer;
    uint32 m_uiAmplifyDamageTimer;
    uint32 m_uiCleave_Timer;
    uint32 InfernalTimer;
    uint32 m_uiAxesTargetSwitchTimer;
    uint32 m_uiInfernalCleanupTimer;

    GUIDVector m_vInfernalGuids;
    std::vector<InfernalPoint*> m_positions;

    ObjectGuid m_aAxeGuid[2];
    ObjectGuid m_aEnfeebleTargetGuid[5];
    uint32 m_auiEnfeebleHealth[5];

    uint32 m_uiPhase;

    void Reset()
    {
        AxesCleanup();
        ClearWeapons();
        InfernalCleanup();
        m_positions.clear();

        for (uint8 i = 0; i < 5; ++i)
        {
            m_aEnfeebleTargetGuid[i].Clear();
            m_auiEnfeebleHealth[i] = 0;
        }

        for(int i = 0; i < TOTAL_INFERNAL_POINTS; ++i)
            m_positions.push_back(&InfernalPoints[i]);

        m_uiEnfeebleTimer = 30000;
        m_uiEnfeebleResetTimer = 38000;
        m_uiShadowNovaTimer = 35500;
        m_uiSWPainTimer = 20000;
        m_uiAmplifyDamageTimer = 5000;
        m_uiCleave_Timer = 8000;
        InfernalTimer = 45000;
        m_uiInfernalCleanupTimer = 47000;
        m_uiAxesTargetSwitchTimer = urand(7500, 20000);
        SunderArmorTimer = urand(5000, 10000);
        m_uiPhase = 1;
    }

    void KilledUnit(Unit* pVictim)
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

        AxesCleanup();
        ClearWeapons();
        InfernalCleanup();
        m_positions.clear();

        for (uint8 i = 0; i < TOTAL_INFERNAL_POINTS; ++i)
            m_positions.push_back(&InfernalPoints[i]);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void InfernalCleanup()
    {
        // Infernal Cleanup
        for(GUIDVector::const_iterator itr = m_vInfernalGuids.begin(); itr!= m_vInfernalGuids.end(); ++itr)
        {
            Creature *pInfernal = m_creature->GetMap()->GetCreature(*itr);
            if (pInfernal && pInfernal->isAlive())
            {
                pInfernal->SetVisibility(VISIBILITY_OFF);
                pInfernal->SetDeathState(JUST_DIED);
            }
        }
        m_vInfernalGuids.clear();
    }

    void AxesCleanup()
    {
        for (uint8 i = 0; i < 2; ++i)
        {
            Creature* pAxe = m_creature->GetMap()->GetCreature(m_aAxeGuid[i]);
            if (pAxe && pAxe->isAlive())
                pAxe->DealDamage(pAxe, pAxe->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_aAxeGuid[i].Clear();
        }
    }

    void ClearWeapons()
    {
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);

        //damage
        const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, cinfo->mindmg);
        m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, cinfo->maxdmg);
        m_creature->UpdateDamagePhysical(BASE_ATTACK);
    }

    void EnfeebleHealthEffect()
    {
        const SpellEntry *info = GetSpellStore()->LookupEntry(SPELL_ENFEEBLE_EFFECT);
        if (!info)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        std::vector<Unit *> targets;

        if (tList.empty())
            return;

        //begin + 1 , so we don't target the one with the highest threat
        ThreatList::const_iterator itr = tList.begin();
        std::advance(itr, 1);
        for(; itr!= tList.end(); ++itr)                    //store the threat list in a different container
        {
            Unit *target = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                                                            //only on alive players
            if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(target);
        }

        //cut down to size if we have more than 5 targets
        while(targets.size() > 5)
            targets.erase(targets.begin()+rand()%targets.size());

        int i = 0;
        for(std::vector<Unit *>::iterator iter = targets.begin(); iter!= targets.end(); ++iter, ++i)
        {
            Unit *target = *iter;
            if (target)
            {
                m_aEnfeebleTargetGuid[i] = target->GetObjectGuid();
                m_auiEnfeebleHealth[i] = target->GetHealth();

                target->CastSpell(target, SPELL_ENFEEBLE, true, 0, 0, m_creature->GetObjectGuid());
                target->SetHealth(1);
            }
        }

    }

    void EnfeebleResetHealth()
    {
        for(int i = 0; i < 5; ++i)
        {
            Player* pTarget = m_creature->GetMap()->GetPlayer(m_aEnfeebleTargetGuid[i]);

            if (pTarget && pTarget->isAlive())
                pTarget->SetHealth(m_auiEnfeebleHealth[i]);

            m_aEnfeebleTargetGuid[i].Clear();
            m_auiEnfeebleHealth[i] = 0;
        }
    }

    void SummonInfernal()
    {
        InfernalPoint *point = NULL;
        float posX,posY,posZ;
        if ((m_creature->GetMapId() != 532) || m_positions.empty())
        {
            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 60, posX, posY, posZ);
        }
        else
        {
            std::vector<InfernalPoint*>::iterator itr = m_positions.begin()+rand()%m_positions.size();
            point = *itr;
            m_positions.erase(itr);

            posX = point->x;
            posY = point->y;
            posZ = INFERNAL_Z;
        }

        Creature *Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, posX, posY, posZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);

        if (Infernal)
        {
            Infernal->SetDisplayId(INFERNAL_MODEL_INVISIBLE);
            Infernal->setFaction(m_creature->getFaction());

            netherspite_infernalAI* pInfernalAI = dynamic_cast<netherspite_infernalAI*>(Infernal->AI());

            if (pInfernalAI)
            {
                if (point)
                    pInfernalAI->pPoint = point;

                pInfernalAI->m_malchezaarGuid = m_creature->GetObjectGuid();
            }

            m_vInfernalGuids.push_back(Infernal->GetObjectGuid());
            DoCastSpellIfCan(Infernal, SPELL_INFERNAL_RELAY);
        }

        DoScriptText(urand(0, 1) ? SAY_SUMMON1 : SAY_SUMMON2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnfeebleResetTimer)
        {
            if (m_uiEnfeebleResetTimer <= uiDiff)                  //Let's not forget to reset that
            {
                EnfeebleResetHealth();
                m_uiEnfeebleResetTimer=0;
            }
            else
                m_uiEnfeebleResetTimer -= uiDiff;
        }

        if (m_creature->hasUnitState(UNIT_STAT_STUNNED))    //While shifting to m_uiPhase 2 m_malchezaarGuid stuns himself
            return;

        if (m_creature->GetTargetGuid() != m_creature->getVictim()->GetObjectGuid())
            m_creature->SetTargetGuid(m_creature->getVictim()->GetObjectGuid());

        if (m_uiPhase == 1)
        {
            if (m_creature->GetHealthPercent() < 60.0f)
            {
                m_creature->InterruptNonMeleeSpells(false);

                m_uiPhase = 2;

                //animation
                DoCastSpellIfCan(m_creature, SPELL_EQUIP_AXES);

                //text
                DoScriptText(SAY_AXE_TOSS1, m_creature);

                //passive thrash aura
                m_creature->CastSpell(m_creature, SPELL_THRASH_AURA, true);

                //models
                SetEquipmentSlots(false, EQUIP_ID_AXE, EQUIP_ID_AXE, EQUIP_NO_CHANGE);

                //damage
                const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, 2*cinfo->mindmg);
                m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, 2*cinfo->maxdmg);
                m_creature->UpdateDamagePhysical(BASE_ATTACK);

                m_creature->SetBaseWeaponDamage(OFF_ATTACK, MINDAMAGE, cinfo->mindmg);
                m_creature->SetBaseWeaponDamage(OFF_ATTACK, MAXDAMAGE, cinfo->maxdmg);
                //Sigh, updating only works on main attack , do it manually ....
                m_creature->SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, cinfo->mindmg);
                m_creature->SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, cinfo->maxdmg);

                m_creature->SetAttackTime(OFF_ATTACK, (m_creature->GetAttackTime(BASE_ATTACK)*150)/100);
            }
        }
        else if (m_uiPhase == 2)
        {
            if (m_creature->GetHealthPercent() < 30.0f)
            {
                InfernalTimer = 15000;

                m_uiPhase = 3;

                ClearWeapons();

                //remove thrash
                m_creature->RemoveAurasDueToSpell(SPELL_THRASH_AURA);

                DoScriptText(SAY_AXE_TOSS2, m_creature);

                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                for (uint32 i = 0; i < 2; ++i)
                {
                    Creature* pAxe = m_creature->SummonCreature(MALCHEZARS_AXE, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
                    if (pAxe)
                    {
                        pAxe->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pAxe->setFaction(m_creature->getFaction());

                        m_aAxeGuid[i] = pAxe->GetObjectGuid();
                        if (pTarget)
                        {
                            pAxe->AI()->AttackStart(pTarget);
                            // pAxe->getThreatManager().tauntApply(pTarget); //Taunt Apply and fade out does not work properly
                                                            // So we'll use a hack to add a lot of threat to our pTarget
                            pAxe->AddThreat(pTarget, 10000000.0f);
                        }
                    }
                }

                if (m_uiShadowNovaTimer > 35000)
                    m_uiShadowNovaTimer = m_uiEnfeebleTimer + 5000;

                return;
            }

            if (SunderArmorTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SUNDER_ARMOR);
                SunderArmorTimer = urand(10000, 18000);

            }
            else
                SunderArmorTimer -= uiDiff;

            if (m_uiCleave_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleave_Timer = urand(6000, 12000);

            }
            else
                m_uiCleave_Timer -= uiDiff;
        }
        else
        {
            if (m_uiAxesTargetSwitchTimer < uiDiff)
            {
                m_uiAxesTargetSwitchTimer = urand(7500, 20000);

                Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if (target)
                {
                    for(int i = 0; i < 2; ++i)
                    {
                        Creature *axe = m_creature->GetMap()->GetCreature(m_aAxeGuid[i]);
                        if (axe)
                        {
                            float threat = 1000000.0f;
                            if (axe->getVictim() && m_creature->getThreatManager().getThreat(axe->getVictim()))
                            {
                                threat = axe->getThreatManager().getThreat(axe->getVictim());
                                axe->getThreatManager().modifyThreatPercent(axe->getVictim(), -100);
                            }
                            if (target)
                                axe->AddThreat(target, threat);
                            //axe->getThreatManager().tauntFadeOut(axe->getVictim());
                            //axe->getThreatManager().tauntApply(target);
                        }
                    }
                }
            }
            else
                m_uiAxesTargetSwitchTimer -= uiDiff;

            if (m_uiAmplifyDamageTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_AMPLIFY_DAMAGE);

                m_uiAmplifyDamageTimer = urand(20000, 30000);
            }
            else
                m_uiAmplifyDamageTimer -= uiDiff;
        }

        //Time for global and double timers
        if (InfernalTimer < uiDiff)
        {
            SummonInfernal();
            InfernalTimer =  m_uiPhase == 3 ? 14500 : 44500;    //15 secs in m_uiPhase 3, 45 otherwise
        }
        else
            InfernalTimer -= uiDiff;

        if (m_uiShadowNovaTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWNOVA);
            m_uiShadowNovaTimer = m_uiPhase == 3 ? 31000 : -1;
        }
        else
            m_uiShadowNovaTimer -= uiDiff;

        if (m_uiPhase != 2)
        {
            if (m_uiSWPainTimer < uiDiff)
            {
                Unit* target = NULL;
                if (m_uiPhase == 1)
                    target = m_creature->getVictim();       // the tank
                else                                        //anyone but the tank
                    target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

                if (target)
                    DoCastSpellIfCan(target, SPELL_SW_PAIN);

                m_uiSWPainTimer = 20000;
            }
            else
                m_uiSWPainTimer -= uiDiff;
        }

        if (m_uiPhase != 3)
        {
            if (m_uiEnfeebleTimer < uiDiff)
            {
                EnfeebleHealthEffect();
                m_uiEnfeebleTimer = 30000;
                m_uiShadowNovaTimer = 5000;
                m_uiEnfeebleResetTimer = 9000;
            }
            else
                m_uiEnfeebleTimer -= uiDiff;
        }

        if (m_uiPhase==2)
            DoMeleeAttacksIfReady();
        else
            DoMeleeAttackIfReady();
    }

    void DoMeleeAttacksIfReady()
    {
        // Check if pTarget is valid
        if (!m_creature->getVictim())
            return;

        if (!m_creature->IsNonMeleeSpellCasted(false) && m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
        {
            //Check for base attack
            if (m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
            //Check for offhand attack
            if (m_creature->isAttackReady(OFF_ATTACK))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim(), OFF_ATTACK);
                m_creature->resetAttackTimer(OFF_ATTACK);
            }
        }
    }

    void Cleanup(Creature *infernal, InfernalPoint *point)
    {
        for(GUIDVector::iterator itr = m_vInfernalGuids.begin(); itr!= m_vInfernalGuids.end(); ++itr)
            if (*itr == infernal->GetObjectGuid())
        {
            m_vInfernalGuids.erase(itr);
            break;
        }

        m_positions.push_back(point);
    }
};

void netherspite_infernalAI::Cleanup()
{
    Creature* pMalchezaar = m_creature->GetMap()->GetCreature(m_malchezaarGuid);

    if (pMalchezaar && pMalchezaar->isAlive())
    {
        if (boss_malchezaarAI* pMalAI = dynamic_cast<boss_malchezaarAI*>(pMalchezaar->AI()))
            pMalAI->Cleanup(m_creature, pPoint);
    }
}

CreatureAI* GetAI_netherspite_infernal(Creature* pCreature)
{
    return new netherspite_infernalAI(pCreature);
}

CreatureAI* GetAI_boss_malchezaar(Creature* pCreature)
{
    return new boss_malchezaarAI(pCreature);
}

void AddSC_boss_prince_malchezaar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_malchezaar";
    pNewScript->GetAI = &GetAI_boss_malchezaar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "netherspite_infernal";
    pNewScript->GetAI = &GetAI_netherspite_infernal;
    pNewScript->RegisterSelf();
}

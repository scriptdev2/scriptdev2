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
SDName: Boss_Teron_Gorefiend
SD%Complete: 60
SDComment: Requires Mind Control support for Ghosts.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

enum
{
    //Speech'n'sound
    SAY_INTRO                = -1564037,
    SAY_AGGRO                = -1564038,
    SAY_SLAY1                = -1564039,
    SAY_SLAY2                = -1564040,
    SAY_SPELL1               = -1564041,
    SAY_SPELL2               = -1564042,
    SAY_SPECIAL1             = -1564043,
    SAY_SPECIAL2             = -1564044,
    SAY_ENRAGE               = -1564045,
    SAY_DEATH                = -1564046,

    //Spells
    SPELL_INCINERATE         = 40239,
    SPELL_CRUSHING_SHADOWS   = 40243,
    SPELL_SHADOWBOLT         = 40185,
    SPELL_PASSIVE_SHADOWFORM = 40326,
    SPELL_SHADOW_OF_DEATH    = 40251,
    SPELL_BERSERK            = 45078,

    SPELL_ATROPHY            = 40327,                       // Shadowy Constructs use this when they get within melee range of a player

    NPC_DOOM_BLOSSOM         = 23123,
    NPC_SHADOWY_CONSTRUCT    = 23111
};

struct MANGOS_DLL_DECL mob_doom_blossomAI : public ScriptedAI
{
    mob_doom_blossomAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiCheckTeronTimer;
    uint32 m_uiShadowBoltTimer;
    uint64 m_uiTeronGUID;

    void Reset()
    {
        m_uiCheckTeronTimer = 5000;
        m_uiShadowBoltTimer = 12000;
        m_uiTeronGUID = 0;
    }

    void AttackStart(Unit* pWho) { }
    void MoveInLineOfSight(Unit* pWho) { }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCheckTeronTimer < uiDiff)
        {
            if (m_uiTeronGUID)
            {
                m_creature->SetInCombatWithZone();

                Creature* pTeron = (Creature*)Unit::GetUnit((*m_creature), m_uiTeronGUID);
                if (pTeron && (!pTeron->isAlive() || pTeron->IsInEvadeMode()))
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            else
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            m_uiCheckTeronTimer = 5000;
        }
        else
            m_uiCheckTeronTimer -= uiDiff;

        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            return;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SHADOWBOLT);

            m_uiShadowBoltTimer = 10000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;
    }

    void SetTeronGUID(uint64 uiGUID){ m_uiTeronGUID = uiGUID; }
};

struct MANGOS_DLL_DECL mob_shadowy_constructAI : public ScriptedAI
{
    mob_shadowy_constructAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint64 m_uiGhostGUID;
    uint64 m_uiTeronGUID;

    uint32 m_uiCheckPlayerTimer;
    uint32 m_uiCheckTeronTimer;

    void Reset()
    {
        m_uiGhostGUID = 0;
        m_uiTeronGUID = 0;

        m_uiCheckPlayerTimer = 2000;
        m_uiCheckTeronTimer = 5000;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho || !pWho->isAlive() || pWho->GetGUID() == m_uiGhostGUID)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

/* Comment it out for now. NOTE TO FUTURE DEV: UNCOMMENT THIS OUT ONLY AFTER MIND CONTROL IS IMPLEMENTED
    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (done_by->GetGUID() != m_uiGhostGUID)
        damage = 0;                                         // Only the ghost can deal damage.
    }
 */

    void CheckPlayers()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        if (tList.empty())
            return;                                         // No threat list. Don't continue.

        std::list<Unit*> lTargets;

        for (ThreatList::const_iterator itr = tList.begin(); itr != tList.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if (pUnit && pUnit->isAlive())
                lTargets.push_back(pUnit);
        }

        lTargets.sort(ObjectDistanceOrder(m_creature));
        Unit* pTarget = lTargets.front();
        if (pTarget && m_creature->IsWithinDistInMap(pTarget, m_creature->GetAttackDistance(pTarget)))
        {
            DoCastSpellIfCan(pTarget, SPELL_ATROPHY);
            m_creature->AI()->AttackStart(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCheckPlayerTimer < uiDiff)
        {
            CheckPlayers();
            m_uiCheckPlayerTimer = 3000;
        }
        else
            m_uiCheckPlayerTimer -= uiDiff;

        if (m_uiCheckTeronTimer < uiDiff)
        {
            Creature* pTeron = (Creature*)Unit::GetUnit((*m_creature), m_uiTeronGUID);
            if (!pTeron || !pTeron->isAlive() || pTeron->IsInEvadeMode())
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            m_uiCheckTeronTimer = 5000;
        }
        else
            m_uiCheckTeronTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_teron_gorefiendAI : public ScriptedAI
{
    boss_teron_gorefiendAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiIncinerateTimer;
    uint32 m_uiSummonDoomBlossomTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiCrushingShadowsTimer;
    uint32 m_uiShadowOfDeathTimer;
    uint32 m_uiSummonShadowsTimer;
    uint32 m_uiRandomYellTimer;
    uint32 m_uiAggroTimer;

    uint64 m_uiAggroTargetGUID;
    uint64 m_uiGhostGUID;                                   // Player that gets killed by Shadow of Death and gets turned into a ghost

    bool m_bIntro;

    void Reset()
    {
        m_uiIncinerateTimer = urand(20000, 30000);
        m_uiSummonDoomBlossomTimer = 12000;
        m_uiEnrageTimer = MINUTE*10*IN_MILLISECONDS;
        m_uiCrushingShadowsTimer = 22000;
        m_uiSummonShadowsTimer = 60000;
        m_uiRandomYellTimer = 50000;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        // Start off unattackable so that the intro is done properly
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_uiAggroTimer = 20000;
        m_uiAggroTargetGUID = 0;
        m_bIntro = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOREFIEND, NOT_STARTED);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_GOREFIEND)!= IN_PROGRESS && !m_bIntro && pWho->GetTypeId() == TYPEID_PLAYER && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (m_creature->IsWithinDistInMap(pWho, VISIBLE_RANGE) && m_creature->IsWithinLOSInMap(pWho))
            {
                m_pInstance->SetData(TYPE_GOREFIEND, IN_PROGRESS);

                m_creature->GetMotionMaster()->Clear(false);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                DoScriptText(SAY_INTRO, m_creature);

                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                m_uiAggroTargetGUID = pWho->GetGUID();
                m_bIntro = true;
            }
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOREFIEND, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }
    
    float CalculateRandomLocation(float fLoc, uint32 uiRadius)
    {
        return fLoc + urand(0, 1) ? -rand()%uiRadius : rand()%uiRadius;
    }

    void SetThreatList(Creature* pCreature)
    {
        if (!pCreature)
            return;

        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if (pUnit && pUnit->isAlive())
            {
                float threat = m_creature->getThreatManager().getThreat(pUnit);
                pCreature->AddThreat(pUnit, threat);
            }
        }
    }

    void MindControlGhost()
    {
        /************************************************************************/
        /** NOTE FOR FUTURE DEVELOPER: PROPERLY IMPLEMENT THE GHOST PORTION *****/
        /**  ONLY AFTER MaNGOS FULLY IMPLEMENTS MIND CONTROL ABILITIES      *****/
        /**   THE CURRENT CODE IN THIS FUNCTION IS ONLY THE BEGINNING OF    *****/
        /**    WHAT IS FULLY NECESSARY FOR GOREFIEND TO BE 100% COMPLETE    *****/
        /************************************************************************/

        Unit* pGhost = NULL;
        if (m_uiGhostGUID)
            pGhost = Unit::GetUnit((*m_creature), m_uiGhostGUID);
        
        if (pGhost && pGhost->isAlive() && pGhost->HasAura(SPELL_SHADOW_OF_DEATH, EFFECT_INDEX_0))
        {
            /*float x,y,z;
            pGhost->GetPosition(x,y,z);
            Creature* control = m_creature->SummonCreature(CREATURE_GHOST, x, y, z, 0, TEMPSUMMON_TIMED_DESAWN, 30000);
            if (control)
            {
                ((Player*)pGhost)->Possess(control);
                pGhost->DealDamage(pGhost, pGhost->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL,
            false);
            }*/
            for(uint8 i = 0; i < 4; ++i)
            {
                float fX = CalculateRandomLocation(pGhost->GetPositionX(), 10);
                float fY = CalculateRandomLocation(pGhost->GetPositionY(), 10);

                if (Creature* pConstruct = m_creature->SummonCreature(NPC_SHADOWY_CONSTRUCT, fX, fY, pGhost->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45000))
                {
                    pConstruct->CastSpell(pConstruct, SPELL_PASSIVE_SHADOWFORM, true);

                    SetThreatList(pConstruct);               // Use same function as Doom Blossom to set Threat List.
                    if (mob_shadowy_constructAI* pConstructAI = dynamic_cast<mob_shadowy_constructAI*>(pConstruct->AI()))
                        pConstructAI->m_uiGhostGUID = m_uiGhostGUID;

                    Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
                    pConstruct->GetMotionMaster()->MoveChase(pTarget ? pTarget : m_creature->getVictim());
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIntro)
        {
            if (m_uiAggroTimer < uiDiff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                DoScriptText(SAY_AGGRO, m_creature);

                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                m_bIntro = false;
                if (m_uiAggroTargetGUID)
                {
                    if (Unit* pUnit = Unit::GetUnit((*m_creature), m_uiAggroTargetGUID))
                        AttackStart(pUnit);

                    m_creature->SetInCombatWithZone();
                }
                else
                    EnterEvadeMode();
            }
            else
                m_uiAggroTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_bIntro)
            return;

        if (m_uiSummonShadowsTimer < uiDiff)
        {
            //MindControlGhost();
            for(uint8 i = 0; i < 2; ++i)
            {
                float fX = CalculateRandomLocation(m_creature->GetPositionX(), 10);

                if (Creature* pShadow = m_creature->SummonCreature(NPC_SHADOWY_CONSTRUCT, fX, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 0))
                {
                    Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
                    pShadow->AI()->AttackStart(pTarget ? pTarget : m_creature->getVictim());
                }
            }
            m_uiSummonShadowsTimer = 60000;
        }
        else
            m_uiSummonShadowsTimer -= uiDiff;

        if (m_uiSummonDoomBlossomTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                float fX = CalculateRandomLocation(pTarget->GetPositionX(), 20);
                float fY = CalculateRandomLocation(pTarget->GetPositionY(), 20);

                if (Creature* pDoomBlossom = m_creature->SummonCreature(NPC_DOOM_BLOSSOM, fX, fY, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
                {
                    pDoomBlossom->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pDoomBlossom->setFaction(m_creature->getFaction());
                    pDoomBlossom->AddThreat(pTarget);

                    if (mob_doom_blossomAI* pDoomBlossomAI = dynamic_cast<mob_doom_blossomAI*>(pDoomBlossom->AI()))
                        pDoomBlossomAI->SetTeronGUID(m_creature->GetGUID());

                    SetThreatList(pDoomBlossom);
                }

                m_uiSummonDoomBlossomTimer = 35000;
            }
        }
        else
            m_uiSummonDoomBlossomTimer -= uiDiff;

        if (m_uiIncinerateTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_SPECIAL1 : SAY_SPECIAL2, m_creature);

            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);
            DoCastSpellIfCan(pTarget ? pTarget : m_creature->getVictim(), SPELL_INCINERATE);
            m_uiIncinerateTimer = urand(20000, 50000);
        }
        else
            m_uiIncinerateTimer -= uiDiff;

        if (m_uiCrushingShadowsTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && pTarget->isAlive())
                DoCastSpellIfCan(pTarget, SPELL_CRUSHING_SHADOWS);

            m_uiCrushingShadowsTimer = urand(10000, 26000);
        }
        else
            m_uiCrushingShadowsTimer -= uiDiff;

        /*** NOTE FOR FUTURE DEV: UNCOMMENT BELOW ONLY IF MIND CONTROL IS FULLY IMPLEMENTED **/
        /*if (m_uiShadowOfDeathTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1);

            if (!pTarget)
               pTarget = m_creature->getVictim();

            if (pTarget && pTarget->isAlive() && pTarget->GetTypeId() == TYPEID_PLAYER)
            {
                DoCastSpellIfCan(pTarget, SPELL_SHADOW_OF_DEATH);
                m_uiGhostGUID = pTarget->GetGUID();
                m_uiShadowOfDeathTimer = 30000;
                m_uiSummonShadowsTimer = 53000; // Make it VERY close but slightly less so that we can check if the aura is still on the pPlayer
            }
        }else m_uiShadowOfDeathTimer -= uiDiff;*/

        if (m_uiRandomYellTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_SPELL1 : SAY_SPELL2, m_creature);
            m_uiRandomYellTimer = urand(50000, 100000);
        }
        else
            m_uiRandomYellTimer -= uiDiff;

        if (!m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                DoScriptText(SAY_ENRAGE, m_creature);
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_doom_blossom(Creature* pCreature)
{
    return new mob_doom_blossomAI(pCreature);
}

CreatureAI* GetAI_mob_shadowy_construct(Creature* pCreature)
{
    return new mob_shadowy_constructAI(pCreature);
}

CreatureAI* GetAI_boss_teron_gorefiend(Creature* pCreature)
{
    return new boss_teron_gorefiendAI(pCreature);
}

void AddSC_boss_teron_gorefiend()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "mob_doom_blossom";
    newscript->GetAI = &GetAI_mob_doom_blossom;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadowy_construct";
    newscript->GetAI = &GetAI_mob_shadowy_construct;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_teron_gorefiend";
    newscript->GetAI = &GetAI_boss_teron_gorefiend;
    newscript->RegisterSelf();
}

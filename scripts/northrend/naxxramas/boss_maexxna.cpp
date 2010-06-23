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
SDName: Boss_Maexxna
SD%Complete: 70
SDComment: Need to correct web wrap ability
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_SPIN_WEB              = -1533146,
    EMOTE_SPIDERLING            = -1533147,
    EMOTE_SPRAY                 = -1533148,
    EMOTE_BOSS_GENERIC_FRENZY   = -1000005,

    SPELL_WEBWRAP               = 28622,                    //Spell is normally used by the webtrap on the wall NOT by Maexxna

    SPELL_WEBSPRAY              = 29484,
    SPELL_WEBSPRAY_H            = 54125,

    SPELL_POISONSHOCK           = 28741,
    SPELL_POISONSHOCK_H         = 54122,

    SPELL_NECROTICPOISON        = 28776,
    SPELL_NECROTICPOISON_H      = 54121,

    SPELL_FRENZY                = 54123,
    SPELL_FRENZY_H              = 54124,

    NPC_WEB_WRAP                = 16486,
    NPC_SPIDERLING              = 17055
};

#define LOC_X1    3546.796f
#define LOC_Y1    -3869.082f
#define LOC_Z1    296.450f

#define LOC_X2    3531.271f
#define LOC_Y2    -3847.424f
#define LOC_Z2    299.450f

#define LOC_X3    3497.067f
#define LOC_Y3    -3843.384f
#define LOC_Z3    302.384f

struct MANGOS_DLL_DECL npc_web_wrapAI : public ScriptedAI
{
    npc_web_wrapAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
    }

    void MoveInLineOfSight(Unit* pWho) { }
    void AttackStart(Unit* pWho) { }

    void SetVictim(Unit* pVictim)
    {
        if (pVictim)
        {
            m_uiVictimGUID = pVictim->GetGUID();
            pVictim->CastSpell(pVictim, SPELL_WEBWRAP, true);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_uiVictimGUID)
        {
            if (Unit* pVictim = Unit::GetUnit(*m_creature, m_uiVictimGUID))
            {
                if (pVictim->isAlive())
                    pVictim->RemoveAurasDueToSpell(SPELL_WEBWRAP);
            }
        }
    }
};

struct MANGOS_DLL_DECL boss_maexxnaAI : public ScriptedAI
{
    boss_maexxnaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWebWrapTimer;
    uint32 m_uiWebSprayTimer;
    uint32 m_uiPoisonShockTimer;
    uint32 m_uiNecroticPoisonTimer;
    uint32 m_uiSummonSpiderlingTimer;
    bool   m_bEnraged;

    void Reset()
    {
        m_uiWebWrapTimer = 20000;                           // 20 sec init, 40 sec normal
        m_uiWebSprayTimer = 40000;                          // 40 seconds
        m_uiPoisonShockTimer = 20000;                       // 20 seconds
        m_uiNecroticPoisonTimer = 30000;                    // 30 seconds
        m_uiSummonSpiderlingTimer = 30000;                  // 30 sec init, 40 sec normal
        m_bEnraged = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, FAIL);
    }

    void DoCastWebWrap()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        std::vector<Unit *> targets;

        // This spell doesn't work if we only have 1 player on threat list
        if (tList.size() < 2)
            return;

        // begin + 1 , so we don't target the one with the highest threat
        ThreatList::const_iterator itr = tList.begin();
        std::advance(itr, 1);

        // store the threat list in a different container
        for (;itr != tList.end(); ++itr)
        {
            Unit* target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());

            // only on alive players
            if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(target);
        }

        // cut down to size if we have more than 3 targets
        while(targets.size() > 3)
            targets.erase(targets.begin()+rand()%targets.size());

        int i = 0;

        for(std::vector<Unit *>::iterator iter = targets.begin(); iter!= targets.end(); ++iter, ++i)
        {
            // Teleport the 3 targets to a location on the wall and summon a Web Wrap on them
            switch(i)
            {
                case 0:
                    DoTeleportPlayer((*iter), LOC_X1, LOC_Y1, LOC_Z1, (*iter)->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(NPC_WEB_WRAP, LOC_X1, LOC_Y1, LOC_Z1, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        if (npc_web_wrapAI* pWebAI = dynamic_cast<npc_web_wrapAI*>(pWrap->AI()))
                            pWebAI->SetVictim(*iter);
                    }
                    break;
                case 1:
                    DoTeleportPlayer((*iter), LOC_X2, LOC_Y2, LOC_Z2, (*iter)->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(NPC_WEB_WRAP, LOC_X2, LOC_Y2, LOC_Z2, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        if (npc_web_wrapAI* pWebAI = dynamic_cast<npc_web_wrapAI*>(pWrap->AI()))
                            pWebAI->SetVictim(*iter);
                    }
                    break;
                case 2:
                    DoTeleportPlayer((*iter), LOC_X3, LOC_Y3, LOC_Z3, (*iter)->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(NPC_WEB_WRAP, LOC_X3, LOC_Y3, LOC_Z3, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
                    {
                        if (npc_web_wrapAI* pWebAI = dynamic_cast<npc_web_wrapAI*>(pWrap->AI()))
                            pWebAI->SetVictim(*iter);
                    }
                    break;
            }
        }
    }

    void SummonSpiderlings()
    {
        for(uint8 i = 0; i < 8; ++i)
            m_creature->SummonCreature(NPC_SPIDERLING, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Web Wrap
        if (m_uiWebWrapTimer < uiDiff)
        {
            DoCastWebWrap();
            DoScriptText(EMOTE_SPIN_WEB, m_creature);
            m_uiWebWrapTimer = 40000;
        }
        else
            m_uiWebWrapTimer -= uiDiff;

        // Web Spray
        if (m_uiWebSprayTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WEBSPRAY : SPELL_WEBSPRAY_H) == CAST_OK)
            {
                DoScriptText(EMOTE_SPRAY, m_creature);
                m_uiWebSprayTimer = 40000;
            }
        }
        else
            m_uiWebSprayTimer -= uiDiff;

        // Poison Shock
        if (m_uiPoisonShockTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POISONSHOCK : SPELL_POISONSHOCK_H);
            m_uiPoisonShockTimer = 20000;
        }
        else
            m_uiPoisonShockTimer -= uiDiff;

        // Necrotic Poison
        if (m_uiNecroticPoisonTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_NECROTICPOISON : SPELL_NECROTICPOISON_H);
            m_uiNecroticPoisonTimer = 30000;
        }
        else
            m_uiNecroticPoisonTimer -= uiDiff;

        // Summon Spiderling
        if (m_uiSummonSpiderlingTimer < uiDiff)
        {
            SummonSpiderlings();
            DoScriptText(EMOTE_SPIDERLING, m_creature);
            m_uiSummonSpiderlingTimer = 40000;
        }
        else
            m_uiSummonSpiderlingTimer -= uiDiff;

        // Enrage if not already enraged and below 30%
        if (!m_bEnraged && m_creature->GetHealthPercent() < 30.0f)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : SPELL_FRENZY_H) == CAST_OK)
            {
                DoScriptText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
                m_bEnraged = true;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_web_wrap(Creature* pCreature)
{
    return new npc_web_wrapAI(pCreature);
}

CreatureAI* GetAI_boss_maexxna(Creature* pCreature)
{
    return new boss_maexxnaAI(pCreature);
}

void AddSC_boss_maexxna()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_maexxna";
    NewScript->GetAI = &GetAI_boss_maexxna;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_web_wrap";
    NewScript->GetAI = &GetAI_npc_web_wrap;
    NewScript->RegisterSelf();
}

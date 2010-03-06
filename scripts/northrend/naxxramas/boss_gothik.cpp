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
SDName: Boss_Gothik
SD%Complete: 10
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_SPEECH                  = -1533040,                 // todo: split my text in 4 parts
    SAY_KILL                    = -1533041,
    SAY_DEATH                   = -1533042,
    SAY_TELEPORT                = -1533043,

    EMOTE_TO_FRAY               = -1533138,
    EMOTE_GATE                  = -1533139,

    SPELL_TELEPORT_LEFT         = 28025,                    // guesswork
    SPELL_TELEPORT_RIGHT        = 28026,                    // could be defined as dead or live side, left or right facing north

    SPELL_HARVESTSOUL           = 28679,
    SPELL_SHADOWBOLT            = 29317,
    SPELL_SHADOWBOLT_H          = 56405,

    //Unrelenting Trainee
    SPELL_EAGLECLAW           = 30285,
    SPELL_KNOCKDOWN_PASSIVE   = 6961,

    //Unrelenting Deathknight
    SPELL_CHARGE              = 22120,
    SPELL_SHADOW_MARK         = 27825,

    //Unrelenting Rider
    SPELL_UNHOLY_AURA         = 55606,
    H_SPELL_UNHOLY_AURA       = 55608,
    SPELL_SHADOWBOLT_VOLLEY   = 27831,                      //Search thru targets and find those who have the SHADOW_MARK to cast this on
    H_SPELL_SHADOWBOLT_VOLLEY = 55638,

    //Spectral Trainee
    SPELL_ARCANE_EXPLOSION    = 27989,

    //Spectral Deathknight
    SPELL_WHIRLWIND           = 28334,
    SPELL_SUNDER_ARMOR        = 25051,                      //cannot find sunder that reduces armor by 2950
    SPELL_CLEAVE              = 20677,
    SPELL_MANA_BURN           = 17631,

    //Spectral Rider
    SPELL_LIFEDRAIN           = 24300,
    //USES SAME UNHOLY AURA AS UNRELENTING RIDER

    //Spectral Horse
    SPELL_STOMP               = 27993
};

struct MANGOS_DLL_DECL boss_gothikAI : public ScriptedAI
{
    boss_gothikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        //SetCombatMovement(false);                         // uncomment when more parts implemented.
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lLeftSideTrigger;
    std::list<uint64> m_lRightSideTrigger;

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();

        DoScriptText(SAY_SPEECH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_GOTHIK, IN_PROGRESS);

        SetTriggerLists();
    }

    void SetTriggerLists()
    {
        m_lLeftSideTrigger.clear();
        m_lRightSideTrigger.clear();

        std::list<uint64> lList;
        m_pInstance->GetGothTriggerList(lList);

        if (lList.empty())
            return;

        for(std::list<uint64>::iterator itr = lList.begin(); itr != lList.end(); ++itr)
        {
            if (Creature* pTrigger = m_pInstance->instance->GetCreature(*itr))
            {
                // not valid for summon point
                if (pTrigger->GetPositionZ() >= (m_creature->GetPositionZ() - 5.0f))
                    continue;

                if (m_pInstance->IsInRightSideGothArea(pTrigger))
                    m_lRightSideTrigger.push_back(*itr);
                else
                    m_lLeftSideTrigger.push_back(*itr);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gothik(Creature* pCreature)
{
    return new boss_gothikAI(pCreature);
}

void AddSC_boss_gothik()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_gothik";
    NewScript->GetAI = &GetAI_boss_gothik;
    NewScript->RegisterSelf();
}

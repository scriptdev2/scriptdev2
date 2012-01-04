/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Hadronox
SD%Complete: 50%
SDComment: Only basic abilities. Gauntlet event NYI
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SPELL_PIERCE_ARMOR          = 53418,
    SPELL_ACID_CLOUD            = 53400,
    SPELL_ACID_CLOUD_H          = 59419,
    SPELL_LEECH_POISON          = 53030,
    SPELL_LEECH_POISON_H        = 59417,
    SPELL_WEB_GRAB              = 57731,
    SPELL_WEB_GRAB_H            = 59421,

    // Gauntlet end spells - send events 19101 and 19102
    //SPELL_WEB_FRONT_DOORS     = 53177,
    //SPELL_WEB_SIDE_DOORS      = 53185,

    // Gauntlet summoned npcs
    //NPC_ANUBAR_CHAMPION_1     = 29062,
    //NPC_ANUBAR_CRYPT_FIEND_1  = 29063,
    //NPC_ANUBAR_NECROMANCER_1  = 29064,
    //NPC_ANUBAR_CHAMPION_2     = 29096,
    //NPC_ANUBAR_CRYPT_FIEND_2  = 29097,
    //NPC_ANUBAR_NECROMANCER_2  = 29098,
};

 /* ##### Gauntlet description #####
  * This is the timed gauntlet - waves of non-elite spiders will spawn from the 3 doors located a little above the main room
  * They will make their way down to fight Hadronox but she will head to the main room, fighting the spiders
  * When Hadronox enters the main room, she will web the doors, and no more spiders will spawn.
  */

/*######
## boss_hadronox
######*/

struct MANGOS_DLL_DECL boss_hadronoxAI : public ScriptedAI
{
    boss_hadronoxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_azjol_nerub*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_azjol_nerub* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiAcidTimer;
    uint32 m_uiLeechTimer;
    uint32 m_uiPierceTimer;
    uint32 m_uiGrabTimer;

    void Reset()
    {
        m_uiAcidTimer   = urand(10000, 14000);
        m_uiLeechTimer  = urand(3000, 9000);
        m_uiPierceTimer = urand(1000, 3000);
        m_uiGrabTimer   = urand(15000, 19000);
    }

    void KilledUnit(Unit* pVictim)
    {
        m_creature->SetHealth(m_creature->GetHealth() + (m_creature->GetMaxHealth() * 0.1));
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPierceTimer < uiDiff)
        {
            if(DoCastSpellIfCan(m_creature->getVictim(), SPELL_PIERCE_ARMOR) == CAST_OK)
                m_uiPierceTimer = 8000;
        }
        else
            m_uiPierceTimer -= uiDiff;

        if (m_uiAcidTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_ACID_CLOUD : SPELL_ACID_CLOUD_H) == CAST_OK)
                    m_uiAcidTimer = urand(10000, 15000);
            }
        }
        else
            m_uiAcidTimer -= uiDiff;

        if (m_uiLeechTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_LEECH_POISON : SPELL_LEECH_POISON_H) == CAST_OK)
                    m_uiLeechTimer = urand(10000, 15000);
            }
        }
        else
            m_uiLeechTimer -= uiDiff;

        if (m_uiGrabTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WEB_GRAB : SPELL_WEB_GRAB_H) == CAST_OK)
                m_uiGrabTimer = urand(25000, 30000);
        }
        else
            m_uiGrabTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hadronox(Creature* pCreature)
{
    return new boss_hadronoxAI(pCreature);
}

void AddSC_boss_hadronox()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_hadronox";
    pNewScript->GetAI = &GetAI_boss_hadronox;
    pNewScript->RegisterSelf();
}

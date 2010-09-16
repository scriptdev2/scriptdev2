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
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    // collosus' abilities
    SPELL_EMERGE                = 54850,
    SPELL_MIGHTY_BLOW           = 54719,
    SPELL_MORTAL_STRIKES        = 54715,
    SPELL_MORTAL_STRIKES_H      = 59454,

    // elemental's abilities
    SPELL_MERGE                 = 54878,
    SPELL_SURGE                 = 54801,
    SPELL_MOJO_VOLLEY           = 59453,
    SPELL_MOJO_VOLLEY_H         = 54849
};

/*######
## boss_colossus
######*/

struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bFirstEmerge;

    uint32 m_uiMightyBlowTimer;
    
    void Reset()
    {
        m_bFirstEmerge = false;
        m_uiMightyBlowTimer = 10000;
    }

    void Agrro()
    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MORTAL_STRIKES : SPELL_MORTAL_STRIKES_H);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);
    }
    
    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_MERGE)
        {
            // re-activate colossus here
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ELEMENTAL)
        {
            // handle elemental stuff
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        
        if (!m_bFirstEmerge && m_creature->GetHealthPercent() < 50.0f)
        {
            m_bFirstEmerge = true;
            DoCastSpellIfCan(m_creature, SPELL_EMERGE);
        }
        else if (m_creature->GetHealth() - uiDamage <= 0)
        {
            // prevent boss from dying if players deal the final blow
            if (pDoneBy->GetCharmerOrOwnerPlayerOrPlayerItself())
            {
                uiDamage = 0;
                DoCastSpellIfCan(m_creature, SPELL_EMERGE);
            }
        }

    }
			 
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 10000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;
        
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

void AddSC_boss_colossus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_colossus";
    pNewScript->GetAI = &GetAI_boss_colossus;
    pNewScript->RegisterSelf();
}

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
SDName: Dalaran
SD%Complete: 100
SDComment:
SDCategory: Dalaran
EndScriptData */

/* ContentData
npc_dalaran_guardian_mage
EndContentData */

#include "precompiled.h"

enum
{
    SPELL_TRESPASSER_H      = 54029,
    SPELL_TRESPASSER_A      = 54028,

    AREA_ID_SUNREAVER       = 4616,
    AREA_ID_SILVER_ENCLAVE  = 4740
};

struct MANGOS_DLL_DECL npc_dalaran_guardian_mageAI : public ScriptedAI
{
    npc_dalaran_guardian_mageAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
            return;

        if (pWho->isTargetableForAttack() && m_creature->IsHostileTo(pWho))
        {
            if (m_creature->IsWithinDistInMap(pWho, m_creature->GetAttackDistance(pWho)) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (Player* pPlayer = pWho->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    // it's mentioned that pet may also be teleported, if so, we need to tune script to apply to those in addition.

                    if (pPlayer->GetAreaId() == AREA_ID_SILVER_ENCLAVE)
                        DoCastSpellIfCan(pPlayer, SPELL_TRESPASSER_A);
                    else if (pPlayer->GetAreaId() == AREA_ID_SUNREAVER)
                        DoCastSpellIfCan(pPlayer, SPELL_TRESPASSER_H);
                }
            }
        }
    }

    void AttackedBy(Unit* /*pAttacker*/) {}

    void Reset() {}

    void UpdateAI(const uint32 /*uiDiff*/) {}
};

CreatureAI* GetAI_npc_dalaran_guardian_mage(Creature* pCreature)
{
    return new npc_dalaran_guardian_mageAI(pCreature);
}

void AddSC_dalaran()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_dalaran_guardian_mage";
    pNewScript->GetAI = &GetAI_npc_dalaran_guardian_mage;
    pNewScript->RegisterSelf();
}

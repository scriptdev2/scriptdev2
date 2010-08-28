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
SDName: Molten_Core
SD%Complete: 100
SDComment:
SDCategory: Molten Core
EndScriptData */

/* ContentData
mob_ancient_core_hound
EndContentData */

#include "precompiled.h"
#include "simple_ai.h"

enum
{
    SPELL_CONE_OF_FIRE          = 19630,
    SPELL_BITE                  = 19771,

    //Random Debuff (each hound has only one of these)
    SPELL_GROUND_STOMP          = 19364,
    SPELL_ANCIENT_DREAD         = 19365,
    SPELL_CAUTERIZING_FLAMES    = 19366,
    SPELL_WITHERING_HEAT        = 19367,
    SPELL_ANCIENT_DESPAIR       = 19369,
    SPELL_ANCIENT_HYSTERIA      = 19372
};

CreatureAI* GetAI_mob_ancient_core_hound(Creature* pCreature)
{
    SimpleAI* pAI = new SimpleAI(pCreature);

    pAI->m_Spell[0].bEnabled       = true;
    pAI->m_Spell[0].uiSpellId      = SPELL_CONE_OF_FIRE;
    pAI->m_Spell[0].uiCooldown     = 7000;
    pAI->m_Spell[0].iFirstCast     = 10000;
    pAI->m_Spell[0].CastTargetType = CAST_HOSTILE_TARGET;

    uint32 RandDebuff = 0;
    switch(urand(0, 5))
    {
        case 0: RandDebuff = SPELL_GROUND_STOMP;       break;
        case 1: RandDebuff = SPELL_ANCIENT_DREAD;      break;
        case 2: RandDebuff = SPELL_CAUTERIZING_FLAMES; break;
        case 3: RandDebuff = SPELL_WITHERING_HEAT;     break;
        case 4: RandDebuff = SPELL_ANCIENT_DESPAIR;    break;
        case 5: RandDebuff = SPELL_ANCIENT_HYSTERIA;   break;
    }
    pAI->m_Spell[1].bEnabled       = true;
    pAI->m_Spell[1].uiSpellId      = RandDebuff;
    pAI->m_Spell[1].uiCooldown     = 24000;
    pAI->m_Spell[1].iFirstCast     = 15000;
    pAI->m_Spell[1].CastTargetType = CAST_HOSTILE_TARGET;

    pAI->m_Spell[2].bEnabled       = true;
    pAI->m_Spell[2].uiSpellId      = SPELL_BITE;
    pAI->m_Spell[2].uiCooldown     = 6000;
    pAI->m_Spell[2].iFirstCast     = 4000;
    pAI->m_Spell[2].CastTargetType = CAST_HOSTILE_TARGET;

    pAI->EnterEvadeMode();

    return pAI;
}

void AddSC_molten_core()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "mob_ancient_core_hound";
    pNewscript->GetAI = &GetAI_mob_ancient_core_hound;
    pNewscript->RegisterSelf();
}

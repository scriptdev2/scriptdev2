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
SDName: SimpleAI
SD%Complete: 100
SDComment: Base Class for SimpleAI creatures
SDCategory: Creatures
EndScriptData */

#include "precompiled.h"
#include "simple_ai.h"

SimpleAI::SimpleAI(Creature* pCreature) : ScriptedAI(pCreature)
{
    //Clear all data
    memset(&m_aiAggroTextId, 0, sizeof(m_aiAggroTextId));
    memset(&m_auiAggroSound, 0, sizeof(m_auiAggroSound));

    memset(&m_aiDeathTextId, 0, sizeof(m_aiDeathTextId));
    memset(&m_auiDeathSound, 0, sizeof(m_auiDeathSound));
    m_uiDeathSpell = 0;
    m_DeathTargetType = CAST_SELF;

    memset(&m_aiKillTextId, 0, sizeof(m_aiKillTextId));
    memset(&m_auiKillSound, 0, sizeof(m_auiKillSound));
    m_uiKillSpell = 0;
    m_KillTargetType = CAST_SELF;

    memset(&m_Spell, 0, sizeof(m_Spell));

    EnterEvadeMode();
}

void SimpleAI::Reset()
{
}

void SimpleAI::Aggro(Unit* pWho)
{
    //Reset cast timers
    for(uint8 i = 0; i < 10; ++i)
        m_auiSpellTimer[i] = (m_Spell[i].iFirstCast >= 0) ? m_Spell[i].iFirstCast : 1000;

    uint8 uiRandomText = urand(0, 2);

    //Random text
    if (m_aiAggroTextId[uiRandomText])
        DoScriptText(m_aiAggroTextId[uiRandomText], m_creature, pWho);

    //Random sound
    if (m_auiAggroSound[uiRandomText])
        DoPlaySoundToSet(m_creature, m_auiAggroSound[uiRandomText]);
}

void SimpleAI::KilledUnit(Unit* pVictim)
{
    uint32 uiRandomText = urand(0, 2);

    //Random yell
    if (m_aiKillTextId[uiRandomText])
        DoScriptText(m_aiKillTextId[uiRandomText], m_creature, pVictim);

    //Random sound
    if (m_auiKillSound[uiRandomText])
        DoPlaySoundToSet(m_creature, m_auiKillSound[uiRandomText]);

    if (!m_uiKillSpell)
        return;

    Unit* pTarget = NULL;

    switch (m_KillTargetType)
    {
        case CAST_SELF:
            pTarget = m_creature;
            break;
        case CAST_HOSTILE_TARGET:
            pTarget = m_creature->getVictim();
            break;
        case CAST_HOSTILE_SECOND_AGGRO:
            pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1);
            break;
        case CAST_HOSTILE_LAST_AGGRO:
            pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_BOTTOMAGGRO, 0);
            break;
        case CAST_HOSTILE_RANDOM:
            pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            break;
        case CAST_KILLEDUNIT_VICTIM:
            pTarget = pVictim;
            break;
    }

    //Target is ok, cast a spell on it
    if (pTarget)
        DoCastSpellIfCan(pTarget, m_uiKillSpell);
}

void SimpleAI::DamageTaken(Unit* pDoneBy, uint32& uiDamage)
{
    //Return if damage taken won't kill us
    if (m_creature->GetHealth() > uiDamage)
        return;

    uint8 uiRandomText = urand(0, 2);

    // Random yell
    if (m_aiDeathTextId[uiRandomText])
        DoScriptText(m_aiDeathTextId[uiRandomText], m_creature, pDoneBy);

    // Random sound
    if (m_auiDeathSound[uiRandomText])
        DoPlaySoundToSet(m_creature, m_auiDeathSound[uiRandomText]);

    if (!m_uiDeathSpell)
        return;

    Unit* pTarget = NULL;

    switch (m_DeathTargetType)
    {
        case CAST_SELF:
            pTarget = m_creature;
            break;
        case CAST_HOSTILE_TARGET:
            pTarget = m_creature->getVictim();
            break;
        case CAST_HOSTILE_SECOND_AGGRO:
            pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1);
            break;
        case CAST_HOSTILE_LAST_AGGRO:
            pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_BOTTOMAGGRO, 0);
            break;
        case CAST_HOSTILE_RANDOM:
            pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            break;
        case CAST_JUSTDIED_KILLER:
            pTarget = pDoneBy;
            break;
    }

    // Target is ok, cast a spell on it
    if (pTarget)
        DoCastSpellIfCan(pTarget, m_uiDeathSpell);
}

void SimpleAI::UpdateAI(const uint32 uiDiff)
{
    //Return since we have no target
    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        return;

    //Spells
    for (uint8 i = 0; i < 10; ++i)
    {
        //Spell not valid
        if (!m_Spell[i].bEnabled || !m_Spell[i].uiSpellId)
            continue;

        if (m_auiSpellTimer[i] < uiDiff)
        {
            //Check if this is a percentage based
            if (m_Spell[i].iFirstCast < 0 && m_Spell[i].iFirstCast > -100 && m_creature->GetHealthPercent() > -m_Spell[i].iFirstCast)
                continue;

            //Check Current spell
            if (!(m_Spell[i].bInterruptPreviousCast && m_creature->IsNonMeleeSpellCasted(false)))
            {
                Unit* pTarget = NULL;

                switch (m_Spell[i].CastTargetType)
                {
                case CAST_SELF:
                    pTarget = m_creature;
                    break;
                case CAST_HOSTILE_TARGET:
                    pTarget = m_creature->getVictim();
                    break;
                case CAST_HOSTILE_SECOND_AGGRO:
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1);
                    break;
                case CAST_HOSTILE_LAST_AGGRO:
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_BOTTOMAGGRO, 0);
                    break;
                case CAST_HOSTILE_RANDOM:
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                    break;
                }

                //Target is ok, cast a spell on it and then do our random yell
                if (pTarget)
                {
                    if (m_creature->IsNonMeleeSpellCasted(false))
                        m_creature->InterruptNonMeleeSpells(false);

                    DoCastSpellIfCan(pTarget, m_Spell[i].uiSpellId);

                    //Yell and sound use the same number so that you can make
                    //the creature yell with the correct sound effect attached
                    uint32 uiRandomText = urand(0, 2);

                    //Random yell
                    if (m_Spell[i].aiTextId[uiRandomText])
                        DoScriptText(m_Spell[i].aiTextId[uiRandomText], m_creature, pTarget);

                    //Random sound
                    if (m_Spell[i].auiTextSound[uiRandomText])
                        DoPlaySoundToSet(m_creature, m_Spell[i].auiTextSound[uiRandomText]);
                }

            }

            //Spell will cast agian when the cooldown is up
            if (m_Spell[i].uiCooldownRandomAddition)
                m_auiSpellTimer[i] = m_Spell[i].uiCooldown + (rand() % m_Spell[i].uiCooldownRandomAddition);
            else
                m_auiSpellTimer[i] = m_Spell[i].uiCooldown;

        }
        else
            m_auiSpellTimer[i] -= uiDiff;

    }

    DoMeleeAttackIfReady();
}

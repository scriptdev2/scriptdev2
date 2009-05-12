/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Warlord_Najentus
SD%Complete: 90
SDComment: Does the GO need script? Uncomment code to test.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "def_black_temple.h"

enum
{
    SAY_AGGRO                       = -1564000,
    SAY_NEEDLE1                     = -1564001,
    SAY_NEEDLE2                     = -1564002,
    SAY_SLAY1                       = -1564003,
    SAY_SLAY2                       = -1564004,
    SAY_SPECIAL1                    = -1564005,
    SAY_SPECIAL2                    = -1564006,
    SAY_ENRAGE1                     = -1564007,             //is this text actually in use?
    SAY_ENRAGE2                     = -1564008,
    SAY_DEATH                       = -1564009,

    SPELL_CRASHINGWAVE              = 40100,
    SPELL_NEEDLE_SPINE              = 39835,
    SPELL_NEEDLE_AOE                = 39968,
    SPELL_TIDAL_BURST               = 39878,
    SPELL_TIDAL_SHIELD              = 39872,                // Not going to use this since Hurl Spine doesn't dispel it.
    SPELL_IMPALING_SPINE            = 39837,
    SPELL_CREATE_NAJENTUS_SPINE     = 39956,
    SPELL_HURL_SPINE                = 39948,
    SPELL_SHIELD_VISUAL             = 37136,
    SPELL_BERSERK                   = 26662
};

struct MANGOS_DLL_DECL boss_najentusAI : public ScriptedAI
{
    boss_najentusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 NeedleSpineTimer;
    uint32 EnrageTimer;
    uint32 SpecialYellTimer;
    uint32 TidalShieldTimer;
    uint32 ImpalingSpineTimer;
    uint32 CheckTimer;                                      // This timer checks if Najentus is Tidal Shielded and if so, regens health. If not, sets IsShielded to false
    uint32 DispelShieldTimer;                               // This shield is only supposed to last 30 seconds, but the SPELL_SHIELD_VISUAL lasts forever

    bool IsShielded;

    void Reset()
    {
        IsShielded = false;

        NeedleSpineTimer = 10000;
        EnrageTimer = MINUTE*8*IN_MILISECONDS;
        SpecialYellTimer = 45000 + (rand()%76)*1000;
        TidalShieldTimer = 60000;
        ImpalingSpineTimer = 45000;
        CheckTimer = 2000;
        DispelShieldTimer = 30000;

        if (pInstance)
        {
            if (m_creature->isAlive())
            {
                pInstance->SetData(DATA_HIGHWARLORDNAJENTUSEVENT, NOT_STARTED);
                ToggleGate(true);
            }
            else ToggleGate(false);
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit *victim)
    {
        if (pInstance)
        {
            pInstance->SetData(DATA_HIGHWARLORDNAJENTUSEVENT, DONE);
            ToggleGate(false);
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void ToggleGate(bool close)
    {
        if (GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_GAMEOBJECT_NAJENTUS_GATE)))
        {
            if (close)
                pGate->SetGoState(GO_STATE_READY);          // Closed
            else
                pGate->SetGoState(GO_STATE_ACTIVE);         // Opened
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (IsShielded)
        {
            if (spell->Id == SPELL_HURL_SPINE)
            {
                if (m_creature->HasAura(SPELL_SHIELD_VISUAL, 0))
                    m_creature->RemoveAurasDueToSpell(SPELL_SHIELD_VISUAL);

                if (m_creature->HasAura(SPELL_TIDAL_SHIELD, 0))
                    m_creature->RemoveAurasDueToSpell(SPELL_TIDAL_SHIELD);

                DoCast(m_creature->getVictim(), SPELL_TIDAL_BURST);
                IsShielded = false;
            }
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (IsShielded)
            damage = 0;
    }

    void Aggro(Unit *who)
    {
        if (pInstance)
            pInstance->SetData(DATA_HIGHWARLORDNAJENTUSEVENT, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
        DoZoneInCombat();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (EnrageTimer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            DoScriptText(SAY_ENRAGE2, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            EnrageTimer = MINUTE*5*IN_MILISECONDS;
        }else EnrageTimer -= diff;

        if (CheckTimer < diff)
        {
            //if (m_creature->HasAura(SPELL_TIDAL_SHIELD, 0))
            if (m_creature->HasAura(SPELL_SHIELD_VISUAL, 0))
                m_creature->SetHealth(m_creature->GetHealth() + (m_creature->GetMaxHealth()/100));
            else
                IsShielded = false;

            CheckTimer = 2000;
        }else CheckTimer -= diff;

        if (IsShielded)
        {
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();

            if (!m_creature->HasAura(SPELL_SHIELD_VISUAL, 0))
                DoCast(m_creature, SPELL_SHIELD_VISUAL);

            if (DispelShieldTimer < diff)
            {
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

                if (m_creature->HasAura(SPELL_SHIELD_VISUAL, 0))
                    m_creature->RemoveAurasDueToSpell(SPELL_SHIELD_VISUAL);

                IsShielded = false;
            }else DispelShieldTimer -= diff;

            return;                                         // Don't cast or do anything while Shielded
        }



        // Needle
        if (NeedleSpineTimer < diff)
        {
            for(uint8 i = 0; i < 3; ++i)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);

                if (!target)
                    target = m_creature->getVictim();

                DoCast(target, SPELL_NEEDLE_SPINE);
                target->CastSpell(target, SPELL_NEEDLE_AOE, false);
            }

            NeedleSpineTimer = 60000;
        }else NeedleSpineTimer -= diff;

        if (SpecialYellTimer < diff)
        {
            switch(rand()%2)
            {
                case 0: DoScriptText(SAY_SPECIAL1, m_creature); break;
                case 1: DoScriptText(SAY_SPECIAL2, m_creature); break;
            }
            SpecialYellTimer = 25000 + (rand()%76)*1000;
        }else SpecialYellTimer -= diff;

        if (ImpalingSpineTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1);

            if (!target)
                target = m_creature->getVictim();

            if (target && (target->GetTypeId() == TYPEID_PLAYER))
            {
                DoCast(target, SPELL_IMPALING_SPINE);

                //if (pInstance)
                    //pInstance->SetData64(DATA_SPINED_PLAYER,target->GetGUID());

                ImpalingSpineTimer = 45000;

                switch(rand()%2)
                {
                    case 0: DoScriptText(SAY_NEEDLE1, m_creature); break;
                    case 1: DoScriptText(SAY_NEEDLE2, m_creature); break;
                }
            }
        }else ImpalingSpineTimer -= diff;

        if (TidalShieldTimer < diff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature, SPELL_SHIELD_VISUAL, true);
            // DoCast(m_creature, SPELL_TIDAL_SHIELD);
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            IsShielded = true;
            TidalShieldTimer = 60000;
            CheckTimer = 2000;
            DispelShieldTimer = 30000;
        }else TidalShieldTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_najentus(Creature* pCreature)
{
    return new boss_najentusAI(pCreature);
}

/*
bool GOHello_go_najentus_spine(Player* pPlayer, GameObject* pGO)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pGO->GetInstanceData())
    {
        uint64 uiPlayerTargetGuid = pInstance->GetData64(DATA_SPINED_PLAYER);

        if (Player* pPlayerTarget = (Player*)Unit::GetUnit(*pPlayer, uiPlayerTargetGuid))
        {
            if (pPlayerTarget->HasAura(SPELL_IMPALING_SPINE))
                pPlayerTarget->RemoveAurasDueToSpell(SPELL_IMPALING_SPINE);

            pInstance->SetData64(DATA_SPINED_PLAYER,0);
        }
    }

    return false;
}
*/

void AddSC_boss_najentus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_najentus";
    newscript->GetAI = &GetAI_boss_najentus;
    newscript->RegisterSelf();

    /*newscript = new Script;
    newscript->Name = "go_najentus_spine";
    newscript->pGOHello = &GOHello_go_najentus_spine;
    newscript->RegisterSelf();*/
}

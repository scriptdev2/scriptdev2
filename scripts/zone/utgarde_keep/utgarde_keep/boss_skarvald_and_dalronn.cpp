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
SDName: Boss_Skarvald_and_Dalronn
SD%Complete: 20%
SDComment:
SDCategory: Utgarde Keep
EndScriptData */

#include "precompiled.h"
#include "def_utgarde_keep.h"

enum
{
    SAY_SKA_AGGRO                       = -1574011,
    SAY_SKA_DEATH                       = -1574012,
    SAY_SKA_DEATH_REAL                  = -1574013,
    SAY_SKA_KILL                        = -1574014,
    SAY_SKA_DAL_DIES_REPLY              = -1574015,

    SAY_DAL_AGGRO_REPLY                 = -1574016,
    SAY_DAL_DEATH                       = -1574017,
    SAY_DAL_DEATH_REAL                  = -1574018,
    SAY_DAL_KILL                        = -1574019,
    SAY_DAL_SKA_DIES_REPLY              = -1574020,

    SPELL_SUMMON_DAL_GHOST              = 48612,
    SPELL_SUMMON_SKA_GHOST              = 48613, 

    NPC_DAL_GHOST                       = 27389,
    NPC_SKA_GHOST                       = 27390
};

struct Yell
{
    int32   m_iTextId;
    int32   m_iTextReplyId;
};

Yell m_aYell[] =
{
    {SAY_SKA_AGGRO, SAY_DAL_AGGRO_REPLY},
    {SAY_SKA_DEATH, SAY_DAL_SKA_DIES_REPLY},
    {SAY_DAL_DEATH, SAY_SKA_DAL_DIES_REPLY}
};

struct MANGOS_DLL_DECL boss_s_and_d_dummyAI : public ScriptedAI
{
    boss_s_and_d_dummyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsHeroic = pCreature->GetMap()->IsHeroic();
        m_uiGhostGUID = 0;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroic;
    uint64 m_uiGhostGUID;

    Creature* GetBuddy()
    {
        return m_pInstance->instance->GetCreature(m_pInstance->GetData64(m_creature->GetEntry() == NPC_DALRONN ? NPC_SKARVALD : NPC_DALRONN));
    }

    void Reset() { }

    void EnterCombat(Unit* pWho)
    {
        if (!pWho)
            return;

        if (Creature* pBuddy = GetBuddy())
        {
            if (!pBuddy->getVictim())
                pBuddy->AI()->AttackStart(pWho);
        }

        Aggro(pWho);
    }

    void JustSummoned(Creature* pSummoned)
    {
        // EventAI can probably handle ghosts
        if (pSummoned->GetEntry() == NPC_DAL_GHOST || pSummoned->GetEntry() == NPC_SKA_GHOST)
        {
            m_uiGhostGUID = pSummoned->GetGUID();

            Unit* pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO,1);

            if (m_creature->getVictim())
                pSummoned->AI()->AttackStart(pTarget ? pTarget : m_creature->getVictim());
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (Creature* pBuddy = GetBuddy())
        {
            if (pBuddy->isAlive())
            {
                DoScriptText(m_creature->GetEntry() == NPC_SKARVALD ? m_aYell[1].m_iTextId : m_aYell[2].m_iTextId, m_creature);
                DoScriptText(m_creature->GetEntry() == NPC_SKARVALD ? m_aYell[1].m_iTextReplyId : m_aYell[2].m_iTextReplyId, pBuddy);

                pBuddy->CastSpell(m_creature, m_creature->GetEntry() == NPC_SKARVALD ? SPELL_SUMMON_SKA_GHOST : SPELL_SUMMON_DAL_GHOST, true);

                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
            else
            {
                // need proper way to despawn, not just evade
                if (Creature* pGhost = (Creature*)Unit::GetUnit(*m_creature,m_uiGhostGUID))
                    pGhost->AI()->EnterEvadeMode();

                pBuddy->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }
    }
};

/*######
## boss_skarvald
######*/

struct MANGOS_DLL_DECL boss_skarvaldAI : public boss_s_and_d_dummyAI
{
    boss_skarvaldAI(Creature* pCreature) : boss_s_and_d_dummyAI(pCreature) { Reset(); }

    uint32 m_uiYellDelayTimer;

    void Reset()
    {
        m_uiYellDelayTimer = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(m_aYell[0].m_iTextId, m_creature);
        m_uiYellDelayTimer = 5000;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SKA_KILL, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_uiYellDelayTimer && m_uiYellDelayTimer < uiDiff)
        {
            if (Creature* pBuddy = GetBuddy())
                DoScriptText(m_aYell[0].m_iTextReplyId, pBuddy);

            m_uiYellDelayTimer = 0;
        }
        else m_uiYellDelayTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_skarvald(Creature* pCreature)
{
    return new boss_skarvaldAI(pCreature);
}

/*######
## boss_dalronn
######*/

struct MANGOS_DLL_DECL boss_dalronnAI : public boss_s_and_d_dummyAI
{
    boss_dalronnAI(Creature* pCreature) : boss_s_and_d_dummyAI(pCreature) { Reset(); }

    void Reset() { }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_DAL_KILL, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dalronn(Creature* pCreature)
{
    return new boss_dalronnAI(pCreature);
}

void AddSC_boss_skarvald_and_dalronn()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_skarvald";
    newscript->GetAI = &GetAI_boss_skarvald;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_dalronn";
    newscript->GetAI = &GetAI_boss_dalronn;
    newscript->RegisterSelf();
}

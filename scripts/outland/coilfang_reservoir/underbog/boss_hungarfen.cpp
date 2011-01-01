/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Hungarfen
SD%Complete: 95
SDComment: Need confirmation if spell data are same in both modes. Summons should have faster rate in heroic
SDCategory: Coilfang Resevoir, Underbog
EndScriptData */

#include "precompiled.h"

#define SPELL_FOUL_SPORES   31673
#define SPELL_ACID_GEYSER   38739

struct MANGOS_DLL_DECL boss_hungarfenAI : public ScriptedAI
{
    boss_hungarfenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    bool Root;
    uint32 Mushroom_Timer;
    uint32 AcidGeyser_Timer;

    void Reset()
    {
        Root = false;
        Mushroom_Timer = 5000;                              // 1 mushroom after 5s, then one per 10s. This should be different in heroic mode
        AcidGeyser_Timer = 10000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() <= 20.0f)
        {
            if (!Root)
            {
                DoCastSpellIfCan(m_creature,SPELL_FOUL_SPORES);
                Root = true;
            }
        }

        if (Mushroom_Timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                m_creature->SummonCreature(17990, target->GetPositionX()+(rand()%8), target->GetPositionY()+(rand()%8), target->GetPositionZ(), (rand()%5), TEMPSUMMON_TIMED_DESPAWN, 22000);
            else
                m_creature->SummonCreature(17990, m_creature->GetPositionX()+(rand()%8), m_creature->GetPositionY()+(rand()%8), m_creature->GetPositionZ(), (rand()%5), TEMPSUMMON_TIMED_DESPAWN, 22000);

            Mushroom_Timer = 10000;
        }else Mushroom_Timer -= diff;

        if (AcidGeyser_Timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_ACID_GEYSER);
            AcidGeyser_Timer = urand(10000, 17500);
        }else AcidGeyser_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_hungarfen(Creature* pCreature)
{
    return new boss_hungarfenAI(pCreature);
}

#define SPELL_SPORE_CLOUD       34168
#define SPELL_PUTRID_MUSHROOM   31690
#define SPELL_GROW              31698

struct MANGOS_DLL_DECL mob_underbog_mushroomAI : public ScriptedAI
{
    mob_underbog_mushroomAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool Stop;
    uint32 Grow_Timer;
    uint32 Shrink_Timer;

    void Reset()
    {
        Stop = false;
        Grow_Timer = 0;
        Shrink_Timer = 20000;

        DoCastSpellIfCan(m_creature, SPELL_PUTRID_MUSHROOM, CAST_TRIGGERED);
        DoCastSpellIfCan(m_creature, SPELL_SPORE_CLOUD, CAST_TRIGGERED);
    }

    void MoveInLineOfSight(Unit *who) { return; }

    void AttackStart(Unit* who) { return; }

    void UpdateAI(const uint32 diff)
    {
        if (Stop)
            return;

        if (Grow_Timer <= diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_GROW);
            Grow_Timer = 3000;
        }else Grow_Timer -= diff;

        if (Shrink_Timer <= diff)
        {
            m_creature->RemoveAurasDueToSpell(SPELL_GROW);
            Stop = true;
        }else Shrink_Timer -= diff;
    }
};
CreatureAI* GetAI_mob_underbog_mushroom(Creature* pCreature)
{
    return new mob_underbog_mushroomAI(pCreature);
}

void AddSC_boss_hungarfen()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_hungarfen";
    newscript->GetAI = &GetAI_boss_hungarfen;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_underbog_mushroom";
    newscript->GetAI = &GetAI_mob_underbog_mushroom;
    newscript->RegisterSelf();
}

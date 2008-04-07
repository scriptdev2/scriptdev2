/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Test.cpp
SD%Complete: 100
SDComment: Script used for testing
SDCategory: Custom
EndScriptData */

#include "sc_creature.h"
#include "../creature/simple_ai.h"
#include "Map.h"
#include "GridNotifiersImpl.h"


struct MANGOS_DLL_DECL testAI : public ScriptedAI
{
    testAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Cleave_Timer;
    bool InCombat;

    void Reset()
    {
        Cleave_Timer = 5000;      //These times are probably wrong
        InCombat = false;
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (Cleave_Timer < diff)
        {
            /*char test[128];
            sprintf(test, "Found %d friendly units in 20 yard radius", GetNearByFriendlyUnits(20.0f).size());

            DoYell(test, LANG_UNIVERSAL, NULL);*/

            Cleave_Timer = 5000;
        }else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
/*
    std::list<Unit *> GetNearByFriendlyUnits(float radius_check)
    {
        CellPair p(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(p);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        //MaNGOS::WorldObjectWorker<MaNGOS::RespawnDo> worker(u_do);

        std::list<Unit *> friends;
        {
            MaNGOS::AnyUnitInObjectRangeCheck u_check(m_creature, radius_check);
            MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck> searcher(friends, u_check);

            TypeContainerVisitor<MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
            TypeContainerVisitor<MaNGOS::UnitListSearcher<MaNGOS::AnyUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

            CellLock<GridReadGuard> cell_lock(cell, p);
            cell_lock->Visit(cell_lock, world_unit_searcher, *m_creature->GetMap());
            cell_lock->Visit(cell_lock, grid_unit_searcher, *m_creature->GetMap());
        }

        for(std::list<Unit *>::iterator itr = friends.begin(); itr != friends.end();)
        {
            if(!m_creature->IsFriendlyTo(*itr))
            {
                std::list<Unit *>::iterator itr2 = itr;
                ++itr;
                friends.erase(itr2);
            }
            else
                ++itr;
        }

        return friends;
    }*/
};

CreatureAI* GetAI_test(Creature *_Creature)
{
    return new testAI (_Creature);
}

void AddSC_test()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="test";
    newscript->GetAI = GetAI_test;
    m_scripts[nrscripts++] = newscript;
}

/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../sc_defines.h"


struct MANGOS_DLL_DECL mob_lavaspawnAI : public ScriptedAI
{
    mob_lavaspawnAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Split_Timer;
    bool InCombat;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {

	Split_Timer = 8000;         //Split after 10 seconds
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            InCombat = true;
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

                DoStartMeleeAttack(who);
                InCombat = true;

            }
        }
    }

    void Split(Unit* victim)
    {
         Rand = rand()%4;
         switch (rand()%2)
         {
                case 0: RandX = 0 - Rand; break;
                case 1: RandX = 0 + Rand; break;
         }
         Rand = 0;
         Rand = rand()%4;
         switch (rand()%2)
         {
                case 0: RandY = 0 - Rand; break;
                case 1: RandY = 0 + Rand; break;
         }
         Rand = 0;
        Summoned = DoSpawnCreature(12265, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }


    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {


            //Split_Timer
            if (Split_Timer < diff)
            {

                //Inturrupt any spell casting
                 m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);

                //Cast
                Split(m_creature->getVictim());

                //8 seconds until we should cast this agian
                Split_Timer = 8000;
            }else Split_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};
CreatureAI* GetAI_mob_lavaspawn(Creature *_Creature)
{
    return new mob_lavaspawnAI (_Creature);
}


void AddSC_mob_lavaspawn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_lavaspawn";
    newscript->GetAI = GetAI_mob_lavaspawn;
    m_scripts[nrscripts++] = newscript;
}

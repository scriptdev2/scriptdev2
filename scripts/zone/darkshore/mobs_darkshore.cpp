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

/*######
## mob_lunaclaw
######*/

struct MANGOS_DLL_DECL mob_lunaclawAI : public ScriptedAI
{
    mob_lunaclawAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void DamageTaken(Unit *done_by, uint32 &damage) 
    {
        if (m_creature->GetHealth() <= damage)
        {
            m_creature->CastSpell(m_creature,18986,true);//summon lunaclaw spirit
        }
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
};
CreatureAI* GetAI_mob_lunaclaw(Creature *_Creature)
{
    return new mob_lunaclawAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_darkshore()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_lunaclaw";
    newscript->GetAI = GetAI_mob_lunaclaw;
    m_scripts[nrscripts++] = newscript;
}

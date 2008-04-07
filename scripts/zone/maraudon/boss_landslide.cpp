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
SDName: Boss_Landslide
SD%Complete: 100
SDComment: 
EndScriptData */

#include "sc_creature.h"

#define SPELL_KNOCKBACK             19813
#define SPELL_WARSTOMP              24375

struct MANGOS_DLL_DECL boss_landslideAI : public ScriptedAI
{
    boss_landslideAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 KnockBack_Timer;
    uint32 WarStomp_Timer;
    uint32 Adds_Timer;
    bool InCombat;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void Reset()
    {       
        KnockBack_Timer = 8000;
        WarStomp_Timer = 2000;
        Adds_Timer = 0;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
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

    void SummonAdds(Unit* victim)
    {
        Rand = rand()%8;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%8;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(2736, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //KnockBack_Timer
        if (KnockBack_Timer < diff)
        {
            Unit* target = NULL;

            //Cast
            DoCast(m_creature->getVictim(),SPELL_KNOCKBACK);

            //20 seconds
            KnockBack_Timer = 15000;
        }else KnockBack_Timer -= diff;

        //WarStomp_Timer
        if (WarStomp_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_WARSTOMP);

            //12 seconds until we should cast this agian
            WarStomp_Timer = 12000;
        }else WarStomp_Timer -= diff;


        //Adds_Timer
        if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 50 )
        {
            if (Adds_Timer < diff)
            {
                // summon 3 Adds  
                SummonAdds(m_creature->getVictim());
                SummonAdds(m_creature->getVictim());
                SummonAdds(m_creature->getVictim());

                //45 seconds until we should cast this agian
                Adds_Timer = 45000;
            } else Adds_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_landslide(Creature *_Creature)
{
    return new boss_landslideAI (_Creature);
}


void AddSC_boss_landslide()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_landslide";
    newscript->GetAI = GetAI_boss_landslide;
    m_scripts[nrscripts++] = newscript;
}

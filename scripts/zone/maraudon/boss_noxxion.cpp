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

#define SPELL_TOXICVOLLEY           21687
#define SPELL_UPPERCUT              22916

struct MANGOS_DLL_DECL boss_noxxionAI : public ScriptedAI
{
    boss_noxxionAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ToxicVolley_Timer;
    uint32 Uppercut_Timer;
    uint32 Adds_Timer;
    uint32 Invisible_Timer;
    bool InCombat;
    bool Invisible;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {
        ToxicVolley_Timer = 7000;
        Uppercut_Timer = 16000;
        Adds_Timer = 19000;
        Invisible_Timer = 15000;                            //Too much too low?
        InCombat = false;
        Invisible = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who || Invisible)
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
        if (!who || m_creature->getVictim() || Invisible)
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
        Summoned = DoSpawnCreature(13456, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }


    void UpdateAI(const uint32 diff)
    {
        if (Invisible && Invisible_Timer < diff)
        {
            //Become visible again 
            m_creature->setFaction(14);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->m_canMove = true; 
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11172);     //Noxxion model
            Invisible = false;
        } else if (Invisible)
               {
                  Invisible_Timer -= diff;
                  //Do nothing while invisible
                  return;
               }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //ToxicVolley_Timer
            if (ToxicVolley_Timer < diff)
            {

                DoCast(m_creature->getVictim(),SPELL_TOXICVOLLEY);

                //9 seconds
                ToxicVolley_Timer = 9000;
            }else ToxicVolley_Timer -= diff;

            //Uppercut_Timer
            if (Uppercut_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_UPPERCUT);

                //12 seconds until we should cast this agian
                Uppercut_Timer = 12000;
            }else Uppercut_Timer -= diff;


            //Adds_Timer
            if (!Invisible && Adds_Timer < diff)
            {

                //Inturrupt any spell casting
                 m_creature->InterruptSpell();
                m_creature->m_canMove = false;
                m_creature->setFaction(35);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // Invisible Model
                SummonAdds(m_creature->getVictim());
                SummonAdds(m_creature->getVictim());
                SummonAdds(m_creature->getVictim());
                SummonAdds(m_creature->getVictim());
                SummonAdds(m_creature->getVictim());
                Invisible = true;
                Invisible_Timer = 15000;
                //40 seconds until we should cast this agian
                Adds_Timer = 40000;
            }else Adds_Timer -= diff;


            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
}; 
CreatureAI* GetAI_boss_noxxion(Creature *_Creature)
{
    return new boss_noxxionAI (_Creature);
}


void AddSC_boss_noxxion()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_noxxion";
    newscript->GetAI = GetAI_boss_noxxion;
    m_scripts[nrscripts++] = newscript;
}

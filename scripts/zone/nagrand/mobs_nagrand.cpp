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

/* ScriptData
SDName: Mobs_Nagrand
SD%Complete: 100
SDComment: Nagrand mobs
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"

/*######
## mob_gurok_the_usurper
######*/

CreatureAI* GetAI_mob_gurok_the_usurper(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //knockback
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 38576;
    ai->Spell[0].Cooldown = 20000; 
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_shattered_rumbler
######*/

struct MANGOS_DLL_DECL mob_shattered_rumblerAI : public ScriptedAI
{
    bool Spawn;

    mob_shattered_rumblerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        Spawn = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        DoGoHome();
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if(Spellkind->Id == 32001 && !Spawn)
        {
            float x = m_creature->GetPositionX();
            float y = m_creature->GetPositionY();
            float z = m_creature->GetPositionZ();

            Hitter->SummonCreature(18181,x+(0.7 * (rand()%30)),y+(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            Hitter->SummonCreature(18181,x+(rand()%5),y-(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            Hitter->SummonCreature(18181,x-(rand()%5),y+(0.5 *(rand()%60)),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            m_creature->setDeathState(CORPSE);
            Spawn = true;
        }
        return;
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
CreatureAI* GetAI_mob_shattered_rumbler(Creature *_Creature)
{
    return new mob_shattered_rumblerAI (_Creature);
}

/*######
## mobs_kilsorrow_agent
######*/

struct MANGOS_DLL_DECL mobs_kilsorrow_agentAI : public ScriptedAI
{
    mobs_kilsorrow_agentAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
        {
            //check quest status
            if( ((Player*)Killer)->GetTeam() == ALLIANCE && ((Player*)Killer)->GetQuestStatus(9936) == QUEST_STATUS_INCOMPLETE && !((Player*)Killer)->GetReqKillOrCastCurrentCount(9936, m_creature->GetEntry()) )
            {
                ((Player*)Killer)->KilledMonster(21276, m_creature->GetGUID());
            }
            if( ((Player*)Killer)->GetTeam() == HORDE && ((Player*)Killer)->GetQuestStatus(9935) == QUEST_STATUS_INCOMPLETE && !((Player*)Killer)->GetReqKillOrCastCurrentCount(9935, m_creature->GetEntry()) )
            {
                ((Player*)Killer)->KilledMonster(21276, m_creature->GetGUID());
            }
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
CreatureAI* GetAI_mobs_kilsorrow_agent(Creature *_Creature)
{
    return new mobs_kilsorrow_agentAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_nagrand()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_gurok_the_usurper";
    newscript->GetAI = GetAI_mob_gurok_the_usurper;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_shattered_rumbler";
    newscript->GetAI = GetAI_mob_shattered_rumbler;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_kilsorrow_agent";
    newscript->GetAI = GetAI_mobs_kilsorrow_agent;
    m_scripts[nrscripts++] = newscript;
}

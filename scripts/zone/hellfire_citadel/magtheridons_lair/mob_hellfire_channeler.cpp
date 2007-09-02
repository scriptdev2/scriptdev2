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

#include "../../../sc_defines.h"

#define SPELL_SHADOW_BOLT_VOLLEY    39175
#define SPELL_DARK_MENDING          30528
#define SPELL_HELLFIRE_CHANNELING   31059
#define SPELL_HELLFIRE_CAST_VISUAL  24207
#define SPELL_FEAR                  39176

#define ADD 18946

struct MANGOS_DLL_DECL mob_hellfire_channelerAI : public ScriptedAI
{
    mob_hellfire_channelerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Shadow_Timer;
    uint32 Dark_Timer;
    uint32 Fear_Timer;
    uint32 Spawn_Timer;

    bool InCombat;
    bool HasAdd;

    int RandTime(int time) {return ((rand()%time)*1000);}

    void EnterEvadeMode()
    {
        Shadow_Timer = 10000;
        Dark_Timer = 30000;
        Fear_Timer = 20000;
        Spawn_Timer = 30000;

        InCombat = false;
        HasAdd = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();

        DoGoHome();

        // Must be scripted - don't know how.....
        //DoCast(m_creature,SPELL_HELLFIRE_CHANNELING);
        //DoCast(m_creature,SPELL_HELLFIRE_CAST_VISUAL);
    }

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            if (!InCombat) InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim()) return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura()) who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if (!InCombat) InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (Shadow_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (target)
            {
                DoFaceTarget(target);
                DoCast(target,SPELL_SHADOW_BOLT_VOLLEY);
            }
            else
            {
                DoFaceTarget(m_creature->getVictim());
                DoCast(m_creature->getVictim(),SPELL_SHADOW_BOLT_VOLLEY);
            }
            Shadow_Timer = RandTime(30);
        }else Shadow_Timer -= diff;

        //          Don't know how else i can select friendly creature... don't work...
        /*
        if (Dark_Timer < diff)
        {
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_SINGLE_FRIEND, 0);

        if (target && (target->GetMaxHealth() > target->GetHealth()))
        {
        DoFaceTarget(target);
        DoCast(target,SPELL_DARK_MENDING);
        }
        Dark_Timer = RandTime(60);
        }else Dark_Timer -= diff;
        */
        if (Fear_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (target)
            {
                DoFaceTarget(target);
                DoCast(target,SPELL_FEAR);
            }
            else
            {
                DoFaceTarget(m_creature->getVictim());
                DoCast(m_creature->getVictim(),SPELL_FEAR);
            }
            Fear_Timer = RandTime(50);
        }else Fear_Timer -= diff;

        if (!HasAdd && Spawn_Timer < diff)
        {
            float A = m_creature->GetAngle(m_creature);
            Creature* Infernal = m_creature->SummonCreature(ADD, m_creature->GetPositionX()+(rand()%15), m_creature->GetPositionY()+(rand()%15), m_creature->GetPositionZ(), A, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            Infernal->setFaction(m_creature->getFaction());

            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            Infernal->AI()->AttackStart(target);

            HasAdd = true;
        }else Spawn_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_hellfire_channeler(Creature *_Creature)
{
    return new mob_hellfire_channelerAI (_Creature);
}

void AddSC_mob_hellfire_channeler()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_hellfire_channeler";
    newscript->GetAI = GetAI_mob_hellfire_channeler;
    m_scripts[nrscripts++] = newscript;
}

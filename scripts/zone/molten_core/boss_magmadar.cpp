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
SDName: Boss_Magmadar
SD%Complete: 75
SDComment: Conflag on ground nyi, fear causes issues without VMAPs
EndScriptData */

#include "../../sc_defines.h"


#define SPELL_FRENZY                28371
#define SPELL_MAGMASPIT             19449       //This is actually a buff he gives himself
#define SPELL_LAVABREATH            19272
#define SPELL_PANIC                 19408       
#define SPELL_LAVABOMB              19411       //This calls a dummy server side effect that isn't implemented yet
#define SPELL_LAVABOMB_ALT          19428       //This is the spell that the lava bomb casts

struct MANGOS_DLL_DECL boss_magmadarAI : public ScriptedAI
{
    boss_magmadarAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Frenzy_Timer;
    uint32 LavaBreath_Timer;
    uint32 Panic_Timer;
    uint32 Lavabomb_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Frenzy_Timer = 30000;       
        LavaBreath_Timer = 7000;
        Panic_Timer = 20000;
        Lavabomb_Timer = 12000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->CastSpell(m_creature,SPELL_MAGMASPIT,true);
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

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Frenzy_Timer
        if (Frenzy_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_FRENZY);
            DoTextEmote("goes into a killing frenzy!",NULL);

            //30 seconds
            Frenzy_Timer = 30000;
        }else Frenzy_Timer -= diff;

        //LavaBreath_Timer
        if (LavaBreath_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_LAVABREATH);

            //7 seconds until we should cast this agian
            LavaBreath_Timer = 7000;
        }else LavaBreath_Timer -= diff;

        //Panic_Timer
        if (Panic_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_PANIC);

            //30 seconds until we should cast this agian
            Panic_Timer = 30000;
        }else Panic_Timer -= diff;

        //Lavabomb_Timer
        if (Lavabomb_Timer < diff)
        {
            //Cast on random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            //Casting Alt lava bomb since normal one isn't supported
            if (target)DoCast(target,SPELL_LAVABOMB_ALT);

            //12 seconds until we should cast this agian
            Lavabomb_Timer = 12000;
        }else Lavabomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_magmadar(Creature *_Creature)
{
    return new boss_magmadarAI (_Creature);
}


void AddSC_boss_magmadar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_magmadar";
    newscript->GetAI = GetAI_boss_magmadar;
    m_scripts[nrscripts++] = newscript;
}

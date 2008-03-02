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
SDName: Boss_Ebonroc
SD%Complete: 50
SDComment: Shadow of Ebonroc needs core support
SDCategory: Blackwing Lair
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_SHADOWFLAME           22539
#define SPELL_WINGBUFFET            18500
#define SPELL_SHADOWOFEBONROC       23340

struct MANGOS_DLL_DECL boss_ebonrocAI : public ScriptedAI
{
    boss_ebonrocAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowFlame_Timer;
    uint32 WingBuffet_Timer;
    uint32 ShadowOfEbonroc_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ShadowFlame_Timer = 15000;      //These times are probably wrong
        WingBuffet_Timer = 30000;
        ShadowOfEbonroc_Timer = 45000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;
        //Shadowflame Timer
        if (ShadowFlame_Timer < diff)
        {
            //Cast Shadowflame
            DoCast(m_creature->getVictim(),SPELL_SHADOWFLAME);

            //15-25 seconds till recast
            ShadowFlame_Timer = 15000 + rand()%10000;
        }else ShadowFlame_Timer -= diff;

        //Wing Buffet Timer
        if (WingBuffet_Timer < diff)
        {
            //Cast Wing Buffet
            DoCast(m_creature->getVictim(),SPELL_WINGBUFFET);

            //30 seconds until we should cast this agian
            WingBuffet_Timer = 30000;
        }else WingBuffet_Timer -= diff;

        //Shadow of Ebonroc Timer
        if (ShadowOfEbonroc_Timer < diff)
        {
            //Cast Shadow of Ebonroc
            DoCast(m_creature->getVictim(),SPELL_SHADOWOFEBONROC);

            //30-40 seconds until we should cast this agian
            ShadowOfEbonroc_Timer = 30000 + rand()%10000;
        }else ShadowOfEbonroc_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_ebonroc(Creature *_Creature)
{
    return new boss_ebonrocAI (_Creature);
}


void AddSC_boss_ebonroc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ebonroc";
    newscript->GetAI = GetAI_boss_ebonroc;
    m_scripts[nrscripts++] = newscript;
}

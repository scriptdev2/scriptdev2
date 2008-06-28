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
SDName: Boss_Murmur
SD%Complete: 100
SDComment: 
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "sc_creature.h"

#define SPELL_SONIC_BOOM           33666
#define SPELL_SONIC_BOOM_CAST      38796
#define SPELL_MURMURS_TOUCH        38794
#define SPELL_RESONANCE            33657

struct MANGOS_DLL_DECL boss_murmurAI : public ScriptedAI
{
    boss_murmurAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 SonicBoom_Timer;
    uint32 MurmursTouch_Timer;
    uint32 Resonance_Timer;
    bool SonicBoomVisual;

    void Reset()
    {
        SonicBoom_Timer = 30000;
        MurmursTouch_Timer = 20000;
        Resonance_Timer = 10000;
        SonicBoomVisual = false;
    }

    void Aggro(Unit *who)
    {

    }

    void AttackStart(Unit* who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack())
        {
            //Begin attack
            DoStartAttackNoMovement(who);

            if (!InCombat)
            {
                Aggro(who);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
        if( !m_creature->getVictim() && who->isTargetableForAttack() && ( m_creature->IsHostileTo( who )) && who->isInAccessablePlaceFor(m_creature) )
        {
            if (m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if(m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                DoStartAttackNoMovement(who);
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    Aggro(who);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //SonicBoom_Timer
        if(SonicBoom_Timer < diff)
        {
            if(!SonicBoomVisual)
            {
                DoCast(m_creature, SPELL_SONIC_BOOM_CAST);
                SonicBoomVisual = true;
                SonicBoom_Timer = 5000;
            }
            else
            {
                // it damage only the main tank, and it don't divides damage between other members / core related
                //DoCast(m_creature->getVictim(), SPELL_SONIC_BOOM);
                SonicBoomVisual = false;
                SonicBoom_Timer = 30000;
            }
        }else SonicBoom_Timer -= diff;

        //MurmursTouch_Timer
        if(MurmursTouch_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
                DoCast(target, SPELL_MURMURS_TOUCH);

            MurmursTouch_Timer = 30000;
        }else MurmursTouch_Timer -= diff;

        //Resonance_Timer
        if(Resonance_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_RESONANCE);

            Resonance_Timer = 40000;
        }else Resonance_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_murmur(Creature *_Creature)
{
    return new boss_murmurAI (_Creature);
}

void AddSC_boss_murmur()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_murmur";
    newscript->GetAI = GetAI_boss_murmur;
    m_scripts[nrscripts++] = newscript;
}

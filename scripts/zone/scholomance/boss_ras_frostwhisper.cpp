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
SDName: Boss_Ras_Frostwhisper
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_FROSTBOLT                21369
#define SPELL_ICEARMOR             18100       //This is actually a buff he gives himself
#define SPELL_FREEZE            18763
#define SPELL_FEAR                 26070
#define SPELL_CHILLNOVA              18099       
#define SPELL_FROSTVOLLEY         8398       

struct MANGOS_DLL_DECL boss_rasfrostAI : public ScriptedAI
{
    boss_rasfrostAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 IceArmor_Timer;
    uint32 Frostbolt_Timer;
    uint32 Freeze_Timer;
    uint32 Fear_Timer;
    uint32 ChillNova_Timer;
    uint32 FrostVolley_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        IceArmor_Timer = 2000;
        Frostbolt_Timer = 8000;
        ChillNova_Timer = 12000;
        Freeze_Timer = 18000;
        FrostVolley_Timer = 24000;
        Fear_Timer = 45000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->CastSpell(m_creature,SPELL_ICEARMOR,true);
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

        //IceArmor_Timer
        if (IceArmor_Timer < diff)
        {
            //Cast
            DoCast(m_creature, SPELL_ICEARMOR);

            //180 seconds
            IceArmor_Timer = 180000;
        }else IceArmor_Timer -= diff;



        //Frostbolt_Timer
        if (Frostbolt_Timer < diff)
        {

            //Cast FrostBolt on a Random target
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_FROSTBOLT);

            //18 seconds
            Frostbolt_Timer = 8000;
        }else Frostbolt_Timer -= diff;

        //Freeze_Timer
        if (Freeze_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FREEZE);

            //24 seconds until we should cast this agian
            Freeze_Timer = 24000;
        }else Freeze_Timer -= diff;

        //Fear_Timer
        if (Fear_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FEAR);

            //30 seconds until we should cast this agian
            Fear_Timer = 30000;
        }else Fear_Timer -= diff;

        //ChillNova_Timer
        if (ChillNova_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_CHILLNOVA);

            //14 seconds until we should cast this agian
            ChillNova_Timer = 14000;
        }else ChillNova_Timer -= diff;

        //FrostVolley_Timer
        if (FrostVolley_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FROSTVOLLEY);

            //7 seconds until we should cast this agian
            FrostVolley_Timer = 15000;
        }else FrostVolley_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_rasfrost(Creature *_Creature)
{
    return new boss_rasfrostAI (_Creature);
}


void AddSC_boss_rasfrost()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_boss_ras_frostwhisper";
    newscript->GetAI = GetAI_boss_rasfrost;
    m_scripts[nrscripts++] = newscript;
}

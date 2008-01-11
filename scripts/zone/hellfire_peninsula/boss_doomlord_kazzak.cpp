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
SDName: Boss_Doomlord_Kazzak
SD%Complete: 70
SDComment: Using incorrect spell for Mark of Kazzak
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_SHADOWVOLLEY                32963            
#define SPELL_CLEAVE           27794             //Not sure if right ID
#define SPELL_THUNDERCLAP            23931
#define SPELL_VOIDBOLT              39329
#define SPELL_MARKOFKAZZAK            21056   // We need to cast 21058 on a Player that dont have any mana.
#define SPELL_RAGE           32964
#define SPELL_CAPTURESOUL               32966   

struct MANGOS_DLL_DECL boss_doomlordkazzakAI : public ScriptedAI
{
    boss_doomlordkazzakAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowVolley_Timer;
    uint32 Cleave_Timer;
    uint32 ThunderClap_Timer;
    uint32 VoidBolt_Timer;
    uint32 MarkOfKazzak_Timer;
    uint32 Rage_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        ShadowVolley_Timer = 10000;
        Cleave_Timer = 5000;
        ThunderClap_Timer = 20000;
        VoidBolt_Timer = 30000;
        MarkOfKazzak_Timer = 25000;
        Rage_Timer = 55000;                //Cast 55
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
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

    void KilledUnit()
    {
        // When a player, pet or totem gets killed, Lord Kazzak casts this spell to instantly regenerate 70,000 health.
        DoCast(m_creature,SPELL_CAPTURESOUL);

    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //ShadowVolley_Timer
        if (ShadowVolley_Timer < diff)
        {
            if (rand()%100 < 55) //55% chance to cast
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_SHADOWVOLLEY);
            }
            //5 seconds
            ShadowVolley_Timer = 5000;
        }else ShadowVolley_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            if (rand()%100 < 50) //50% chance to cast
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);
            }
            //10 seconds until we should cast this agian
            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;


        //ThunderClap_Timer
        if (ThunderClap_Timer < diff)
        {
            if (rand()%100 < 20) //20% chance to cast
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);
            }
            //12 seconds until we should cast this agian
            ThunderClap_Timer = 10000;
        }else ThunderClap_Timer -= diff;


        //VoidBolt_Timer
        if (VoidBolt_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_VOIDBOLT);

            //18 seconds until we should cast this again
            VoidBolt_Timer = 18000;
        }else VoidBolt_Timer -= diff;

        //MarkOfKazzak_Timer
        if (MarkOfKazzak_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_MARKOFKAZZAK);
            //20 seconds until we should cast this agian
            MarkOfKazzak_Timer = 20000;
        }else MarkOfKazzak_Timer -= diff;

        //Rage_Timer
        if (Rage_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_RAGE);

            //27 seconds until we should cast this agian
            Rage_Timer = 27000;
        }else Rage_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_boss_doomlordkazzak(Creature *_Creature)
{
    return new boss_doomlordkazzakAI (_Creature);
}


void AddSC_boss_doomlordkazzak()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_doomlord_kazzak";
    newscript->GetAI = GetAI_boss_doomlordkazzak;
    m_scripts[nrscripts++] = newscript;
}

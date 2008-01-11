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
SDName: boss_nerubenkan
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define SPELL_ENCASINGWEBS          4962
#define SPELL_PIERCEARMOR           6016
#define SPELL_VIRULENTPOISON        16427
//#define SPELL_RAISEUNDEADSCARAB     17235

struct MANGOS_DLL_DECL boss_nerubenkanAI : public ScriptedAI
{
    boss_nerubenkanAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 EncasingWebs_Timer;
    uint32 PierceArmor_Timer;
    uint32 VirulentPoison_Timer;
    uint32 RaiseUndeadScarab_Timer;
    bool InCombat;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {
        VirulentPoison_Timer = 3000;
        EncasingWebs_Timer = 7000;
        PierceArmor_Timer = 19000;
        RaiseUndeadScarab_Timer = 11000;
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

    void RaiseUndeadScarab(Unit* victim)
    {
        Rand = rand()%10;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%10;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(10876, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //EncasingWebs
        if (EncasingWebs_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ENCASINGWEBS);
            //30 seconds until we should cast this again
            EncasingWebs_Timer = 30000;
        }else EncasingWebs_Timer -= diff;

        //PierceArmor
        if (PierceArmor_Timer < diff)
        {
            //Cast
            if (rand()%100 < 75) //75% chance to cast
            {
                DoCast(m_creature->getVictim(),SPELL_PIERCEARMOR);
            }
            //35 seconds until we should cast this again
            PierceArmor_Timer = 35000;
        }else PierceArmor_Timer -= diff;

        //VirulentPoison
        if (VirulentPoison_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_VIRULENTPOISON);
            //20 seconds until we should cast this again
            VirulentPoison_Timer = 20000;
        }else VirulentPoison_Timer -= diff;

        //RaiseUndeadScarab
        if (RaiseUndeadScarab_Timer < diff)
        {
            //Cast
            RaiseUndeadScarab(m_creature->getVictim());
            //16 seconds until we should cast this again
            RaiseUndeadScarab_Timer = 16000;
        }else RaiseUndeadScarab_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_nerubenkan(Creature *_Creature)
{
    return new boss_nerubenkanAI (_Creature);
}


void AddSC_boss_nerubenkan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_nerubenkan";
    newscript->GetAI = GetAI_boss_nerubenkan;
    m_scripts[nrscripts++] = newscript;
}

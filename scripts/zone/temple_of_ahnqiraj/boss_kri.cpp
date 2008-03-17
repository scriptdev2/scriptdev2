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
SDName: boss_kri
SD%Complete: 100
SDComment: Loot not linked
EndScriptData */


#include "../../sc_defines.h"
#include "def_temple_of_ahnqiraj.h"

#define SPELL_CLEAVE        26350
#define SPELL_TOXIC_VOLLEY  25812
#define SPELL_POISON_CLOUD  38718         //Only Spell with right dmg.
#define SPELL_ENRAGE        28131


struct MANGOS_DLL_DECL boss_kriAI : public ScriptedAI
{
    boss_kriAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Cleave_Timer;
    uint32 ToxicVolley_Timer;
    uint32 Check_Timer;

    bool VemDead;
    bool Death;

    void Reset()
    {
        Cleave_Timer = 4000 + rand()%4000;
        ToxicVolley_Timer = 6000 + rand()%6000;
        Check_Timer = 2000;

        VemDead = false;
        Death = false;

//        m_creature->RemoveAllAuras();
//        m_creature->DeleteThreatList();
//        m_creature->CombatStop();
//        DoGoHome();
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

        if (who->isTargetableForAttack() && IsVisible(who) && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
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

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                Cleave_Timer = 5000 + rand()%7000;
            }else Cleave_Timer -= diff;

            //ToxicVolley_Timer
            if (ToxicVolley_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_TOXIC_VOLLEY);
                ToxicVolley_Timer = 10000 + rand()%5000;
            }else ToxicVolley_Timer -= diff;


        if (m_creature->GetHealth() <= m_creature->GetMaxHealth() * 0.05 && !Death)
        {

                DoCast(m_creature->getVictim(),SPELL_POISON_CLOUD);
                Death = true;

        }

        //Checking if Vem is dead. If yes we will enrage.
        if(Check_Timer < diff)
        {
            if (!VemDead)
            {
                if(pInstance)
                {    
                    if(pInstance->GetData(DATA_VEMISDEAD))
                    DoCast(m_creature, SPELL_ENRAGE);
                    VemDead = true;
                }
            }
            Check_Timer = 2000;
        }else Check_Timer -= diff;

            DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_kri(Creature *_Creature)
{
    return new boss_kriAI (_Creature);
}


void AddSC_boss_kri()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kri";
    newscript->GetAI = GetAI_boss_kri;
    m_scripts[nrscripts++] = newscript;
}

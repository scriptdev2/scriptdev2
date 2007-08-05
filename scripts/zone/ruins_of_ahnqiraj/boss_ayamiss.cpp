/*
To do:
make him fly from 70-100%
*/

#include "../../sc_defines.h"



#define SPELL_STINGERSPRAY 25749
#define SPELL_POISONSTINGER 25748 //only used in phase1
#define SPELL_SUMMONSWARMER 25844 //might be 25708
// #define SPELL_PARALYZE 23414 doesnt work correct (core)


struct MANGOS_DLL_DECL boss_ayamissAI : public ScriptedAI
{
    boss_ayamissAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    Unit *pTarget;
	uint32 STINGERSPRAY_Timer;
    uint32 POISONSTINGER_Timer;
	uint32 SUMMONSWARMER_Timer;
	uint32 phase;

    void EnterEvadeMode()
    {
	    pTarget = NULL;
		STINGERSPRAY_Timer = 30000;
		POISONSTINGER_Timer = 30000;
		SUMMONSWARMER_Timer = 60000;
		phase=1;

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
            if (m_creature->IsWithinDistInMap(who, ATTACK_DISTANCE))
				DoStartMeleeAttack(who);
            else DoStartRangedAttack(who);
			
			pTarget = who;
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

                pTarget = who;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Check if we should stop attacking because our victim is no longer attackable
            if (needToStop())
            {
                EnterEvadeMode();
                return;
            }

			//If he is 70% start phase 2
            if (phase==1 && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 70 && !m_creature->IsNonMeleeSpellCasted(false))
            {
				phase=2;
				
            }



            //STINGERSPRAY_Timer (only in phase2)
            if (phase==2 && STINGERSPRAY_Timer < diff)
            {
				DoCast(m_creature->getVictim(),SPELL_STINGERSPRAY);
                STINGERSPRAY_Timer = 30000;
            }else STINGERSPRAY_Timer -= diff;

            //POISONSTINGER_Timer (only in phase1)
            if (phase==1 && POISONSTINGER_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_POISONSTINGER);
                POISONSTINGER_Timer = 30000;
            }else POISONSTINGER_Timer -= diff;

			 //SUMMONSWARMER_Timer (only in phase1)
            if (SUMMONSWARMER_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SUMMONSWARMER);
                SUMMONSWARMER_Timer = 60000;
            }else SUMMONSWARMER_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_boss_ayamiss(Creature *_Creature)
{
    return new boss_ayamissAI (_Creature);
}


void AddSC_boss_ayamiss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_ayamiss";
    newscript->GetAI = GetAI_boss_ayamiss;
    m_scripts[nrscripts++] = newscript;
}

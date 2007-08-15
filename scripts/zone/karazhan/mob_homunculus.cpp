// Copyright 2007 by WarHead (United Worlds of Mangos)

#include "../../sc_defines.h"

#define SPELL_FIREBOLT  18086 // Blasts a target for 150 Fire damage.

struct MANGOS_DLL_DECL mob_homunculusAI : public ScriptedAI
{
    mob_homunculusAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Firebolt_Timer;

    bool InCombat;

    void EnterEvadeMode()
    {
        Firebolt_Timer = 0;      
        
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();

        DoGoHome();
    }

    int RandTime(int time) {return ((rand()%time)*1000);}

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
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);

                if (!InCombat) InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget()) return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //Chain cast
            if (!m_creature->IsNonMeleeSpellCasted(false) && !m_creature->m_silenced)
                DoCast(m_creature->getVictim(),SPELL_FIREBOLT);
            else DoMeleeAttackIfReady();

        }
    }
};

CreatureAI* GetAI_mob_homunculus(Creature *_Creature)
{
    return new mob_homunculusAI (_Creature);
}

void AddSC_mob_homunculus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_homunculus";
    newscript->GetAI = GetAI_mob_homunculus;
    m_scripts[nrscripts++] = newscript;
}

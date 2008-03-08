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
SDName: Mob_Celebras_the_Cursed
SD%Complete: 100
SDComment: 
EndScriptData */


#include "../../sc_defines.h"

#define SPELL_WRATH                 21667
#define SPELL_ENTANGLINGROOTS       21331
#define SPELL_TWISTEDTRANQUILITY    21793

struct MANGOS_DLL_DECL celebras_the_cursedAI : public ScriptedAI
{
    celebras_the_cursedAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Wrath_Timer;
    uint32 EntanglingRoots_Timer;
    uint32 TwistedTranquility_Timer;
    uint32 Adds_Timer;
    bool InCombat;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void Reset()
    {
        Wrath_Timer = 8000;
        EntanglingRoots_Timer = 2000;
        TwistedTranquility_Timer = 20000;
        Adds_Timer = 30000;
        InCombat = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
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

    void SummonAdds(Unit* victim)
    {
        Rand = rand()%8;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%8;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(13743, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Wrath_Timer
        if (Wrath_Timer < diff)
        {
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_WRATH);

            //20 seconds
            Wrath_Timer = 20000;
        }else Wrath_Timer -= diff;

        //EntanglingRoots_Timer
        if (EntanglingRoots_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ENTANGLINGROOTS);

            //12 seconds until we should cast this agian
            EntanglingRoots_Timer = 12000;
        }else EntanglingRoots_Timer -= diff;


        //TwistedTranquility_Timer
        if (TwistedTranquility_Timer < diff)
        {

            DoCast(m_creature->getVictim(),SPELL_TWISTEDTRANQUILITY);

            //30 seconds until we should cast this agian
            TwistedTranquility_Timer = 30000;
        }else TwistedTranquility_Timer -= diff;


        //Adds_Timer
        if (Adds_Timer < diff)
        {
            //Cast
            SummonAdds(m_creature->getVictim());
            SummonAdds(m_creature->getVictim());

            //28 seconds until we should cast this agian
            Adds_Timer = 28000;
        }else Adds_Timer -= diff;

        DoMeleeAttackIfReady();
    }

}; 
CreatureAI* GetAI_celebras_the_cursed(Creature *_Creature)
{
    return new celebras_the_cursedAI (_Creature);
}


void AddSC_celebras_the_cursed()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="celebras_the_cursed";
    newscript->GetAI = GetAI_celebras_the_cursed;
    m_scripts[nrscripts++] = newscript;
}

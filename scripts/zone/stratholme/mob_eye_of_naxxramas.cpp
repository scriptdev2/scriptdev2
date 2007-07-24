/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../sc_defines.h"

// **** This script is still under Developement ****

#define SAY_LIVING        "The living are here!"

#define SOUND_FLYIN       6596
#define SOUND_FLYAWAY     6526

#define SPELL_SUPERINVIS  8149

#define SPELL_DAZED	      1604
//#define SPELL_SUMMONROCKWINGGARGOYLES         16381

struct MANGOS_DLL_DECL mob_eye_of_naxxramasAI : public ScriptedAI
{
    mob_eye_of_naxxramasAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Dazed_Timer;
    uint32 SummonRockwingGargoyles_Timer;
    bool HasYelled;
    bool InCombat;
    bool HasSummoned;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;

    void EnterEvadeMode()
    {
        Dazed_Timer = 2000;
        SummonRockwingGargoyles_Timer = 8000;
        HasYelled = false;
        HasSummoned = false;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoPlaySoundToSet(m_creature,SOUND_FLYAWAY);
        DoGoHome();
        m_creature->CastSpell(m_creature,SPELL_SUPERINVIS,true);
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

                if (!HasYelled)
                {
                    DoYell(SAY_LIVING,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_FLYIN);
                    m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    HasYelled = true;
                }

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
                InCombat = true;
            }
        }
    }

    void SummonRockwingGargoyles(Unit* victim)
    {
        Rand = rand()%5;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%5;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(10408, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand() % 10 == 1) //10% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //5 seconds until we should try try cast this again
                Dazed_Timer = 5000;
            }else Dazed_Timer -= diff;

            //SummonRockwingGargoyles
            if (SummonRockwingGargoyles_Timer < diff)
            {
                //Cast
                if (!HasSummoned)
                {
                    SummonRockwingGargoyles(m_creature->getVictim());
                    SummonRockwingGargoyles(m_creature->getVictim());
                    HasSummoned = true;
                }
            }else SummonRockwingGargoyles_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_eye_of_naxxramas(Creature *_Creature)
{
    return new mob_eye_of_naxxramasAI (_Creature);
}


void AddSC_mob_eye_of_naxxramas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_eye_of_naxxramas";
    newscript->GetAI = GetAI_mob_eye_of_naxxramas;
    m_scripts[nrscripts++] = newscript;
}

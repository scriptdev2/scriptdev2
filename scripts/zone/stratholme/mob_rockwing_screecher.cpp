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

//Status: fear effect disabled until proper support in core

#define SOUND_FLYIN    6596
#define SOUND_FLYAWAY    6526

#define SPELL_GARGOYLESTRIKE    16564
//#define SPELL_TERRIFYINGHOWL    8715
#define SPELL_DAZED    1604
#define SPELL_DEAFENINGSCREECH    3589
#define SPELL_DEMORALIZINGSHOUT    16244

struct MANGOS_DLL_DECL mob_rockwing_screecherAI : public ScriptedAI
{
    mob_rockwing_screecherAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 GargoyleStrike_Timer;
    //uint32 TerrifyingHowl_Timer;
    uint32 Dazed_Timer;
    uint32 DeafeningScreech_Timer;
    uint32 DemoralizingShout_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        GargoyleStrike_Timer = 2000;
        //TerrifyingHowl_Timer = 15000;
        Dazed_Timer = 10000;
        DeafeningScreech_Timer = 8000;
        DemoralizingShout_Timer = 6000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoPlaySoundToSet(m_creature,SOUND_FLYAWAY);
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if (!InCombat)
            {
                DoPlaySoundToSet(m_creature,SOUND_FLYIN);
                InCombat = true;
            }

            DoStartMeleeAttack(who);
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

                if (!InCombat)
                {
                    DoPlaySoundToSet(m_creature,SOUND_FLYIN);
                    m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                    InCombat = true;
                }

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
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

            //GargoyleStrike
            if (GargoyleStrike_Timer < diff)
            {
                //Cast
                if (rand()%100 < 90) //90% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_GARGOYLESTRIKE);
                }
                //4 seconds until we should try cast this again
                GargoyleStrike_Timer = 4000;
            }else GargoyleStrike_Timer -= diff;

            //TerrifyingHowl
            /*if (TerrifyingHowl_Timer < diff)
            {
                if (rand()%100 < 35) //35% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_TERRIFYINGHOWL);
                }
                //21 seconds until we should try cast this again
                TerrifyingHowl_Timer = 21000;
            }else TerrifyingHowl_Timer -= diff;*/

            //Dazed
            if (Dazed_Timer < diff)
            {
                //Cast
                if (rand()%100 < 25) //25% chance to cast
                {
                    DoCast(m_creature->getVictim(),SPELL_DAZED);
                }
                //19 seconds until we should try try cast this again
                Dazed_Timer = 19000;
            }else Dazed_Timer -= diff;

            //DeafeningScreech
            if (DeafeningScreech_Timer < diff)
            {
                if (rand()%100 < 45) //45% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEAFENINGSCREECH);
                }
                //26 seconds until we should try cast this again
                DeafeningScreech_Timer = 26000;
            }else DeafeningScreech_Timer -= diff;

            //DemoralizingShout
            if (DemoralizingShout_Timer < diff)
            {
                if (rand()%100 < 75) //75% chance to cast
                {
                    //Cast
                    DoCast(m_creature->getVictim(),SPELL_DEMORALIZINGSHOUT);
                }
                //20 seconds until we should try cast this again
                DemoralizingShout_Timer = 20000;
            }else DemoralizingShout_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
}; 
CreatureAI* GetAI_mob_rockwing_screecher(Creature *_Creature)
{
    return new mob_rockwing_screecherAI (_Creature);
}


void AddSC_mob_rockwing_screecher()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_rockwing_screecher";
    newscript->GetAI = GetAI_mob_rockwing_screecher;
    m_scripts[nrscripts++] = newscript;
}

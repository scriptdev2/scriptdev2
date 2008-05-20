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
SDName: boss_veklor
SD%Complete: 75
SDComment: HP not linked. Spells buggy.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "def_temple_of_ahnqiraj.h"

#define SPELL_HEAL_BROTHER          7393
#define SPELL_TWIN_TELEPORT         800     //Visual only

#define SPELL_SHADOWBOLT            26006
#define SPELL_BLIZZARD              26607
#define SPELL_ARCANEBURST           568
#define SPELL_EXPLODEBUG            804    //Buggy. Same as Golemaggs trust spell. Its casted on gamers not on the right creature...


//8657 - Aggro - To Late
//8658 - Kill - You will not
//8659 - Death
//8623 - too late
//8624 - like a fly
//8625 - my brother
//8626 - prepare to
//8627 - to decorate
//8628 - your brash
//8629 - you will not


struct MANGOS_DLL_DECL boss_veklorAI : public ScriptedAI
{
    boss_veklorAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 Heal_Timer;
    uint32 Teleport_Timer;
    uint32 ShadowBolt_Timer;
    uint32 Blizzard_Timer;
    uint32 ArcaneBurst_Timer;
    uint32 Scorpions_Timer;
    int Rand;
    int RandX;
    int RandY;

    Creature* Summoned;


    void Reset()
    {
        Heal_Timer = 25000 + rand()%15000;
        Teleport_Timer = 12000 + rand()%12000;
        ShadowBolt_Timer = 2000;
        Blizzard_Timer = 15000 + rand()%5000;;
        ArcaneBurst_Timer = 5000;
        Scorpions_Timer = 7000 + rand()%7000;

        m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, IMMUNE_DAMAGE_PHYSICAL, true);  //Added. Can be removed if its included in DB.

//        m_creature->RemoveAllAuras();
//        m_creature->DeleteThreatList();
//        m_creature->CombatStop();
//        DoGoHome();
    }

    void JustDied(Unit* Killer)
    {

    }

    void SummonScorpions(Unit* victim)
    {
        Rand = 10 + (rand()%5);
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = 10 + (rand()%5);
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }		
        Rand = 0;
        Summoned = DoSpawnCreature(15317, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
        if(Summoned)
	    {
	        DoCast(Summoned, SPELL_EXPLODEBUG);
                ((CreatureAI*)Summoned->AI())->AttackStart(victim);
	    }
    }

    

    void Aggro(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

            //ShadowBolt_Timer
            if (ShadowBolt_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT);
                ShadowBolt_Timer = 2500;
            }else ShadowBolt_Timer -= diff;

            //Blizzard_Timer
            if (Blizzard_Timer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_BLIZZARD);

                Blizzard_Timer = 15000+rand()%15000;
            }else Blizzard_Timer -= diff;


	    //If we are within range melee the target
            if( m_creature->getVictim() && m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Check if our attack isShock ready (swing timer)
                if( m_creature->isAttackReady() )
                {                 
                    //Send our melee swing and then reset our attack timer
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();

                   //ArcaneBurst_Timer
                   if (ArcaneBurst_Timer < diff)
                   {
                       DoCast(m_creature->getVictim(),SPELL_ARCANEBURST);
                       ArcaneBurst_Timer = 15000+rand()%15000;
                   }else ArcaneBurst_Timer -= diff;
                }
            }

            //Summon Scorpions
            if (Scorpions_Timer < diff)
            {                 
                SummonScorpions(SelectUnit(SELECT_TARGET_RANDOM,0));				
                Scorpions_Timer = 7000+rand()%3000;
            }else Scorpions_Timer -= diff;

            //Casting Heal to brother
            if(Heal_Timer < diff)
            {
                if(pInstance)
                {    
                    Unit *pVeknilash = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_VEKNILASH));
                    if(pVeknilash)
                        DoCast(pVeknilash, SPELL_HEAL_BROTHER);
                }
                       
                Heal_Timer = 3000;
            }else Heal_Timer -= diff;

            //Teleporting to brother
            if(Teleport_Timer < diff)
            {
                if(pInstance)
                {    
                    DoCast(m_creature, SPELL_TWIN_TELEPORT);
                    Unit *pVeknilash = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_VEKNILASH));
                    if(pVeknilash)
                    {
                        m_creature->Relocate(pVeknilash->GetPositionX(), pVeknilash->GetPositionY(), pVeknilash->GetPositionZ(), 0);  
                        m_creature->SendMoveToPacket(pVeknilash->GetPositionX(), pVeknilash->GetPositionY(), pVeknilash->GetPositionZ(), false, 0);
                        DoResetThreat();
                    }

                Teleport_Timer = 30000;
               }else Teleport_Timer -= diff;
            }
            



            DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_veklor(Creature *_Creature)
{
    return new boss_veklorAI (_Creature);
}


void AddSC_boss_veklor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_veklor";
    newscript->GetAI = GetAI_boss_veklor;
    m_scripts[nrscripts++] = newscript;
}

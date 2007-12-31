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

/* ScriptData
SDName: Boss_Jin'do the Hexxer
SD%Complete: 85
SDComment: Mind Control not working because of core bug. Heal function missing.
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../game/Player.h"



#define SPELL_BRAINWASHTOTEM            24262
#define SPELL_POWERFULLHEALINGWARD      24309                
#define SPELL_HEX                       24326
#define SPELL_DELUSIONSOFJINDO          24306
#define SPELL_SHADEOFJINDO              24308   //We will not use this spell. We will summon a shade by script.

//Healing Ward Spell
#define SPELL_HEAL                      24311


#define SAY_AGGRO         "Welcome to da great show friends! Step right up to die!"

#define SOUND_AGGRO       8425



struct MANGOS_DLL_DECL boss_jindoAI : public ScriptedAI
{
    boss_jindoAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
    
    uint32 BrainWashTotem_Timer;
    uint32 HealingWard_Timer;
    uint32 Hex_Timer;
    uint32 Delusions_Timer;
    uint32 Teleport_Timer;

    Creature *Shade;
    Creature *Skeletons;
    
    ScriptedInstance *pInstance;
    
    bool InCombat;

    void EnterEvadeMode()
    {
        BrainWashTotem_Timer = 20000;
        HealingWard_Timer = 10000;
        Hex_Timer = 8000;
        Delusions_Timer = 14000;
        Teleport_Timer = 11000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
      if (!InCombat)
            {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                InCombat = true;
            }
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
                if (!InCombat)
                {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                InCombat = true;
                }

            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //BrainWashTotem_Timer
        if (BrainWashTotem_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_BRAINWASHTOTEM);

            //16-24 seconds until we should cast this agian
            BrainWashTotem_Timer = 18000 + rand()%8000;
        }else BrainWashTotem_Timer -= diff;

        //HealingWard_Timer
        if (HealingWard_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_POWERFULLHEALINGWARD);

            //14-20 seconds until we should cast this agian
            HealingWard_Timer = 14000 + rand()%6000;
        }else HealingWard_Timer -= diff;

        //Hex_Timer
        if (Hex_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_HEX);

            //12-20 seconds until we should cast this agian
            Hex_Timer = 12000 + rand()%8000;
        }else Hex_Timer -= diff;
        
        //Casting the delusion curse with a shade. So shade will attack the same target with the curse.
        if (Delusions_Timer < diff)
        {
            //Cast
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            DoCast(target,SPELL_DELUSIONSOFJINDO);

            Shade = m_creature->SummonCreature(14986, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Shade->AI()->AttackStart(target); 

            //10-18 seconds until we should cast this agian
            Delusions_Timer = 15000 + rand()%5000;
        }else Delusions_Timer -= diff;
        
        //Teleporting a random gamer and spawning 9 skeletons that will attack this gamer
        if (Teleport_Timer < diff)
        {
        
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target && target->GetTypeId() == TYPEID_PLAYER) 
            {
            
            ((Player*)target)->TeleportTo(309,-11583.7783,-1249.4278,77.5471,4.745);
            m_creature->getThreatManager().modifyThreatPercent(target,-100);

            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()-2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()-4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+3, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            Skeletons->AI()->AttackStart(target); 
            
            }
            
            //17-25 seconds until we should cast this agian
            Teleport_Timer = 17000 + rand()%8000;
        }else Teleport_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 

//Healing Ward
struct MANGOS_DLL_DECL mob_healing_wardAI : public ScriptedAI
{
    mob_healing_wardAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }
    
    uint32 Heal_Timer;
    
    
    ScriptedInstance *pInstance;
    
    
    void EnterEvadeMode()
    {
        Heal_Timer = 3000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);
    }

    void AttackStart(Unit *who)
    {
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
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

                DoStartMeleeAttack(who);
            }
        }
    }
 
    void UpdateAI (const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;
     
        //Heal_Timer
        if(Heal_Timer < diff)
        {
            if(pInstance)
            {    

            Unit *pJindo = Unit::GetUnit(*m_creature, pInstance->GetData64("Jindo"));
            DoCast(pJindo, SPELL_HEAL);

            }

            Heal_Timer = 5000;
        }else Heal_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_jindo(Creature *_Creature)
{
    return new boss_jindoAI (_Creature);
}
CreatureAI* GetAI_mob_healing_ward(Creature *_Creature)
{
    return new mob_healing_wardAI (_Creature);
}

void AddSC_boss_jindo()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_jindo";
    newscript->GetAI = GetAI_boss_jindo;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_healing_ward";
    newscript->GetAI = GetAI_mob_healing_ward;
    m_scripts[nrscripts++] = newscript;

}

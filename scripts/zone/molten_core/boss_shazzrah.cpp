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
SDName: Boss_Shazzrah
SD%Complete: 75
SDComment: Teleport NYI
EndScriptData */

#include "../../sc_defines.h"


#define SPELL_ARCANEEXPLOSION           19712
#define SPELL_SHAZZRAHCURSE             19713
#define SPELL_DEADENMAGIC               19714
#define SPELL_COUNTERSPELL              19715

struct MANGOS_DLL_DECL boss_shazzrahAI : public ScriptedAI
{
    boss_shazzrahAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ArcaneExplosion_Timer;
    uint32 ShazzrahCurse_Timer;
    uint32 DeadenMagic_Timer;
    uint32 Countspell_Timer;
    uint32 Blink_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ArcaneExplosion_Timer = 6000;      //These times are probably wrong
        ShazzrahCurse_Timer = 10000;
        DeadenMagic_Timer = 24000;
        Countspell_Timer = 15000; 
        Blink_Timer = 30000;
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
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
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

  void ResetThreat()
  {
    std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
 
    for(uint32 i = 0; i <= (m_threatlist.size()-1); i++)
    {
      Unit* pUnit = SelectUnit(SELECT_TARGET_TOPAGGRO, i);
      if(pUnit)
        (m_creature->getThreatManager()).modifyThreatPercent(pUnit, -99);
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

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_ARCANEEXPLOSION);

            //6 seconds until we should cast this agian
            ArcaneExplosion_Timer = 5000 + rand()%4000;
        }else ArcaneExplosion_Timer -= diff;

        //ShazzrahCurse_Timer
        if (ShazzrahCurse_Timer < diff)
        {
            //Cast
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)DoCast(target,SPELL_SHAZZRAHCURSE);

            //30 seconds until we should cast this agian
            ShazzrahCurse_Timer = 25000 + rand()%5000;
        }else ShazzrahCurse_Timer -= diff;

        //DeadenMagic_Timer
        if (DeadenMagic_Timer < diff)
        {
            //Cast
            DoCast(m_creature,SPELL_DEADENMAGIC);

            //45 seconds until we should cast this agian
            DeadenMagic_Timer = 35000;
        }else DeadenMagic_Timer -= diff;

        //Countspell_Timer
        if (Countspell_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_COUNTERSPELL);

            //20 seconds until we should cast this agian
            Countspell_Timer = 16000 + rand()%4000;
        }else Countspell_Timer -= diff;


        //Blink_Timer
        if (Blink_Timer < diff)
        {

        // Teleporting him to a random gamer and casting Arcane Explosion after that.
        // Blink is not working cause of LoS System we need to do this hardcoded.
            
        Unit* target = NULL;
            
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
            
        m_creature->Relocate(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0);
        m_creature->SendMonsterMove(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),0,true,0);
        DoCast(target,SPELL_ARCANEEXPLOSION);
        ResetThreat();

            Blink_Timer = 45000;
        }else Blink_Timer -= diff;



        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_shazzrah(Creature *_Creature)
{
    return new boss_shazzrahAI (_Creature);
}


void AddSC_boss_shazzrah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_shazzrah";
    newscript->GetAI = GetAI_boss_shazzrah;
    m_scripts[nrscripts++] = newscript;
}

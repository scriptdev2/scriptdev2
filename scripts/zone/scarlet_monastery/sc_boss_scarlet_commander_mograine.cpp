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

#define SPELL_DIVINESHIELD2         1020
#define SPELL_CRUSADERSTRIKE5       35395
#define SPELL_HAMMEROFJUSTICE3      5589
#define SPELL_HOLYLIGHT6	        3472
#define SPELL_CONSECRATION3         20922
#define SPELL_BLESSINGOFWISDOM      1044
#define SPELL_RETRIBUTIONAURA3      10299
#define SPELL_BLESSINGOFPROTECTION3 10278
#define SPELL_FLASHHEAL6	        10916

#define SAY_AGGRO			"Infidels! They must be purified!"
#define SAY_RES				"At your side, milady!"
#define SAY_DEATH		    "Unworthy!"

#define SOUND_AGGRO			5835
#define SOUND_RES			5837
#define SOUND_DEATH		    5836

struct MANGOS_DLL_DECL boss_scarlet_commander_mograineAI : public ScriptedAI
{
    boss_scarlet_commander_mograineAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Heal_Timer;
    uint32 DivineShield2_Timer;
    uint32 CrusaderStrike5_Timer;
    uint32 HammerOfJustice3_Timer;
    uint32 Consecration3_Timer;
    uint32 BlessingOfWisdom_Timer;
    uint32 BlessingOfProtection3_Timer;
    bool InCombat;

    void Reset()
    {
        Heal_Timer = 80000;
        DivineShield2_Timer = 60000;
        CrusaderStrike5_Timer = 20000;
        HammerOfJustice3_Timer = 80000;
        Consecration3_Timer = 30000;
        BlessingOfWisdom_Timer = 45000;
        BlessingOfProtection3_Timer = 45000;
        InCombat = false;

        if (m_creature)
            EnterEvadeMode();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
        {
            DoCast(m_creature,SPELL_RETRIBUTIONAURA3);
            DoStartMeleeAttack(who);
            
            //Say our dialog
            DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO);

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
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the target died from some unknown soruce
        //But we still need to reset
        if (InCombat && !m_creature->SelectHostilTarget())
        {
            Reset();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //If we are <50% hp cast Arcane Bubble and start casting SPECIAL Arcane Explosion
            if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 50 && !m_creature->m_currentSpell)
            {
                
                //heal_Timer
                if (Heal_Timer < diff)
                {
                
                    //Switch between 2 different charge methods
                    switch (rand()%2)
                        {
                        case 0:
                            DoCast(m_creature,SPELL_HOLYLIGHT6);
                            break;
            
                        case 1:
                            DoCast(m_creature,SPELL_FLASHHEAL6);
                            break;
                        }
                    return;

                    //60 seconds until we should cast this agian
                    Heal_Timer = 60000;
                }else Heal_Timer -= diff;

            }
            
            //DivineShield2_Timer
            if (DivineShield2_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_DIVINESHIELD2);

                //360 seconds until we should cast this agian
                DivineShield2_Timer = 60000;
            }else DivineShield2_Timer -= diff;

            //CrusaderStrike5_Timer
            if (CrusaderStrike5_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CRUSADERSTRIKE5);

                //20 seconds until we should cast this agian
                CrusaderStrike5_Timer = 20000;
            }else CrusaderStrike5_Timer -= diff;

            //HammerOfJustice3_Timer
            if (HammerOfJustice3_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_HAMMEROFJUSTICE3);

                //80 seconds until we should cast this agian
                HammerOfJustice3_Timer = 30000;
            }else HammerOfJustice3_Timer -= diff;

            //Consecration3_Timer
            if (Consecration3_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CONSECRATION3);

                //30 seconds until we should cast this agian
                Consecration3_Timer = 20000;
            }else Consecration3_Timer -= diff;

            //BlessingOfWisdom_Timer
            if (BlessingOfWisdom_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_BLESSINGOFWISDOM);

                //45 seconds until we should cast this agian
                BlessingOfWisdom_Timer = 45000;
            }else BlessingOfWisdom_Timer -= diff;

            //BlessingOfProtection3_Timer
            if (BlessingOfProtection3_Timer < diff)
            {
                //Cast
                DoCast(m_creature,SPELL_BLESSINGOFPROTECTION3);

                //345 seconds until we should cast this agian
                BlessingOfProtection3_Timer = 50000;
            }else BlessingOfProtection3_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDist(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
};

CreatureAI* GetAI_boss_scarlet_commander_mograine(Creature *_Creature)
{
    return new boss_scarlet_commander_mograineAI (_Creature);
}


void AddSC_boss_scarlet_commander_mograine()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_scarlet_commander_mograine";
    newscript->GetAI = GetAI_boss_scarlet_commander_mograine;
    m_scripts[nrscripts++] = newscript;
}
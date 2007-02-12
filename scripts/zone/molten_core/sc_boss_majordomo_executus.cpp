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
// Adds NYI

#define SPELL_MAGICREFLECTION       20619
#define SPELL_DAMAGEREFLECTION      21075
#define SPELL_BLASTWAVE             20229
#define SPELL_AEGIS                 20620       //This is self casted whenever we are below 50%

#define SAY_AGGRO       "Reckless mortals, none may challenge the sons of the living flame!"
#define SOUND_AGGRO     8035

#define SAY_SPAWN       "The runes of warding have been destroyed! Hunt down the infedels my bretheren."
#define SOUND_SPAWN     8039

#define SAY_DEFEAT      "Impossible! Stay your attack mortals! I submitt! I submitt! Brashly you have come to rest the secrets of the living flame. You will soon regret the recklessness of your quest. I go now to summon the lord whos house this is. Should you seek an audiance with him your paltry lives will surly be forfit. Nevertheless seek out his lair if you dare!"
#define SOUND_DEFEAT    8038

#define SAY_KILL        "Ashes to Ashes!"
#define SOUND_KILL      8037

#define SAY_SPECIAL     "Burn mortals! Burn for this transgression!"
#define SOUND_SPECIAL   8036

#define SAY_SUMMON      "Behold Ragnaros, the Firelord! He who was ancient when this world was young! Bow before him, mortals! Bow before your ending!"
#define SOUND_SUMMON    8040

#define SAY_ARRIVAL1    "These mortal infidels, my lord! They have invaded your sanctum, and seek to steal your secrets!"
#define SOUND_ARRIVAL1  8041

struct MANGOS_DLL_DECL boss_majordomoAI : public ScriptedAI
{
    boss_majordomoAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 MagicReflection_Timer;
    uint32 DamageReflection_Timer;
    uint32 Blastwave_Timer;
    bool InCombat;

    void Reset()
    {
        MagicReflection_Timer = 60000;      //Damage reflection first so we alternate
        DamageReflection_Timer = 30000;
        Blastwave_Timer = 10000;
        InCombat = false;

        if (m_creature)
            EnterEvadeMode();
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%5)
            return;

        DoYell(SAY_KILL,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_KILL);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (m_creature->getVictim() == NULL && who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            //Say our dialog
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
            if (m_creature->IsWithinDist(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                    InCombat = true;
                }

                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //If we had a target and it wasn't cleared then it means the player died from some unknown soruce
        //But we still need to reset
        if (!m_creature->SelectHostilTarget())
        {
            Reset();
            return;
        }

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {

            //Cast Ageis if less than 50% hp
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 50)
            {
                DoCast(m_creature,SPELL_AEGIS);
            }
            
            //MagicReflection_Timer
            if (MagicReflection_Timer < diff)
            {
                //Cast NYI in core
                //DoCast(m_creature,SPELL_MAGICREFLECTION);

                //60 seconds until we should cast this agian
                MagicReflection_Timer = 60000;
            }else MagicReflection_Timer -= diff;

            //DamageReflection_Timer
            if (DamageReflection_Timer < diff)
            {
                //Cast NYI in core
                //DoCast(m_creature,SPELL_DAMAGEREFLECTION);

                //60 seconds until we should cast this agian
                DamageReflection_Timer = 60000;
            }else DamageReflection_Timer -= diff;

            //Blastwave_Timer
            if (Blastwave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BLASTWAVE);

                //10 seconds until we should cast this agian
                Blastwave_Timer = 10000;
            }else Blastwave_Timer -= diff;

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
CreatureAI* GetAI_boss_majordomo(Creature *_Creature)
{
    return new boss_majordomoAI (_Creature);
}


void AddSC_boss_majordomo()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_majordomo";
    newscript->GetAI = GetAI_boss_majordomo;
    m_scripts[nrscripts++] = newscript;
}
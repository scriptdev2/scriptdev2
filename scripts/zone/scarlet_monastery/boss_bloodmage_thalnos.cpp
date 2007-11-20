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
SDName: Boss_Bloodmage_Thalnos
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"



#define SPELL_FROSTNOVA2                865
#define SPELL_FLAMESHOCK3               8053
#define SPELL_SHADOWBOLT5               1106
#define SPELL_FLAMESPIKE                8814
#define SPELL_FIRENOVA                  16079

#define SAY_AGGRO                       "We hunger for vengeance."
#define SAY_HEALTH                      "No rest... for the angry dead!"
#define SAY_DEATH                       "More... More souls!"

#define SOUND_AGGRO                     5844
#define SOUND_HEALTH                    5846
#define SOUND_DEATH                     5845

struct MANGOS_DLL_DECL boss_bloodmage_thalnosAI : public ScriptedAI
{
    boss_bloodmage_thalnosAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FrostNova2_Timer;
    uint32 FlameShock3_Timer;
    uint32 ShadowBolt5_Timer;
    uint32 FlameSpike_Timer;
    uint32 FireNova_Timer;
    uint32 Yell_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Yell_Timer = 1;
        FrostNova2_Timer = 10000;
        FlameShock3_Timer = 15000;
        ShadowBolt5_Timer = 20000;
        FlameSpike_Timer = 20000;
        FireNova_Timer = 10000;
        InCombat = false;

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
            //Say our dialog
            if(!InCombat)
            {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                InCombat = true;
            }

            //Begin melee attack if we are within range
            if (m_creature->IsWithinDistInMap(who, ATTACK_DISTANCE))
                DoStartMeleeAttack(who);
            else DoStartRangedAttack(who);
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //If we are <35% hp
        if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 35)
        {
            Yell_Timer -= diff;

            if (Yell_Timer < diff)
            {
                DoYell(SAY_HEALTH,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_HEALTH);
                Yell_Timer = 900000;
            }
        }

        //FrostNova2_Timer
        if (FrostNova2_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FROSTNOVA2);

            //30 seconds until we should cast this agian
            FrostNova2_Timer = 10000;
        }else FrostNova2_Timer -= diff;

        //FlameShock3_Timer
        if (FlameShock3_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FLAMESHOCK3);

            //30 seconds until we should cast this agian
            FlameShock3_Timer = 15000;
        }else FlameShock3_Timer -= diff;

        //ShadowBolt5_Timer
        if (ShadowBolt5_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT5);

            //45 seconds until we should cast this agian
            ShadowBolt5_Timer = 20000;
        }else ShadowBolt5_Timer -= diff;

        //FlameSpike_Timer
        if (FlameSpike_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FLAMESPIKE);

            //45 seconds until we should cast this agian
            FlameSpike_Timer = 30000;
        }else FlameSpike_Timer -= diff;

        //FireNova_Timer
        if (FireNova_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_FIRENOVA);

            //30 seconds until we should cast this agian
            FireNova_Timer = 20000;
        }else FireNova_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_bloodmage_thalnos(Creature *_Creature)
{
    return new boss_bloodmage_thalnosAI (_Creature);
}


void AddSC_boss_bloodmage_thalnos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_bloodmage_thalnos";
    newscript->GetAI = GetAI_boss_bloodmage_thalnos;
    m_scripts[nrscripts++] = newscript;
}

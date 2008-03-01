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
SDName: Boss_Warlord_Kalithres
SD%Complete: 50
SDComment: Missing connection to containers, making fight impossible
EndScriptData */

#include "../../../sc_defines.h"
#include "def_steam_vault.h"

#define SPELL_SPELL_REFLECTION  23920
#define SPELL_IMPALE            26548
#define SPELL_WARLORDS_RAGE     36453

#define SOUND_INTRO         10390 // Intro
#define SOUND_REGENERATE    10391 // Regen - He should regenerate at two objects but don't know how.....
#define SOUND_AGGRO1        10392 // Aggro1
#define SOUND_AGGRO2        10393 // Aggro2
#define SOUND_AGGRO3        10394 // Aggro3
#define SOUND_SLAY1         10395 // Slay1
#define SOUND_SLAY2         10396 // Slay2
#define SOUND_DEATH         10397 // Death

struct MANGOS_DLL_DECL boss_warlord_kalithreshAI : public ScriptedAI
{
    boss_warlord_kalithreshAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Reflection_Timer;
    uint32 Impale_Timer;
    uint32 Rage_Timer;
    int RandTime(int time) { return ((rand()%time)*1000); }

    bool InCombat;

    void EnterEvadeMode()
    {
        Reflection_Timer = 10000;
        Impale_Timer = 30000;
        Rage_Timer = 60000;

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData(DATA_WARLORDKALITHRESHEVENT, 0);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoPlaySoundToSet(victim, SOUND_SLAY1);
            break;
        case 1:
            DoPlaySoundToSet(victim, SOUND_SLAY2);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if(pInstance)
            pInstance->SetData(DATA_WARLORDKALITHRESHEVENT, 0);
    }

    void StartEvent()
    {
        switch(rand()%3)
        {
        case 0:
            DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
            break;
        case 1:
            DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
            break;
        case 2:
            DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
            break;
        }

        InCombat = true;

        if(pInstance)
            pInstance->SetData(DATA_WARLORDKALITHRESHEVENT, 1);
    }

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
            if (!InCombat)
            StartEvent();
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
                if(who->HasStealthAura()) who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
                if (!InCombat)
                    StartEvent();
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Reflection_Timer
        if (Reflection_Timer < diff)
        {
            DoCast(m_creature, SPELL_SPELL_REFLECTION);
            Reflection_Timer = RandTime(60);
        }else Reflection_Timer -= diff;

        //Impale_Timer
        if (Impale_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (target)
            {
                DoFaceTarget(target);
                DoCast(target,SPELL_IMPALE);
            }
            else
            {
                DoFaceTarget(m_creature->getVictim());
                DoCast(m_creature->getVictim(), SPELL_IMPALE);
            }
            Impale_Timer = RandTime(40);
        }else Impale_Timer -= diff;

        //Rage_Timer
        if (Rage_Timer < diff)
        {
            DoCast(m_creature, SPELL_WARLORDS_RAGE);
            Rage_Timer = 600000;
        }else Rage_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_warlord_kalithresh(Creature *_Creature)
{
    return new boss_warlord_kalithreshAI (_Creature);
}

void AddSC_boss_warlord_kalithresh()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_warlord_kalithresh";
    newscript->GetAI = GetAI_boss_warlord_kalithresh;
    m_scripts[nrscripts++] = newscript;
}

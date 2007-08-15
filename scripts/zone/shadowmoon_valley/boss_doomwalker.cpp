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

#define SPELL_MARK_DEATH            37128
#define SPELL_SUNDER_ARMOR          30901

#define SPELL_CHAIN_LIGHTNING       33665
#define SOUND_CHAIN_LIGHTNING       11346

#define SPELL_KICK                  32637
#define SOUND_KICK                  11347

#define SPELL_ENRAGE                34624
#define SOUND_ENRAGE                11348

#define SPELL_EARTHQUAKE            32686
#define SOUND_EARTHQUAKE            11345

#define SOUND_AGGRO                 11344

#define SOUND_DIED1                 11349
#define SOUND_DIED2                 11350
#define SOUND_DIED3                 11351

#define SOUND_DEATH                 11352

struct MANGOS_DLL_DECL boss_doomwalkerAI : public ScriptedAI
{
    boss_doomwalkerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Chain_Timer, Enrage_Timer, Kick_Timer, Quake_Timer, Armor_Timer;

    bool InCombat, InEnrage;

    void EnterEvadeMode()
    {
        Enrage_Timer    = 0;
        Armor_Timer     = 10000;
        Chain_Timer     = 20000;
        Quake_Timer     = 60000;
        Kick_Timer      = 120000;

        InCombat = false;
        InEnrage = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();

        DoGoHome();
    }

    void KilledUnit(Unit* Victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoPlaySoundToSet(Victim, SOUND_DIED1);
            break;
        case 1:
            DoPlaySoundToSet(Victim, SOUND_DIED2);
            break;
        case 2:
            DoPlaySoundToSet(Victim, SOUND_DIED3);
            break;
        }
        if (InEnrage) DoCast(Victim,SPELL_MARK_DEATH);
    }

    void JustDied(Unit* Killer)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            if (!InCombat)
            {
                DoFaceTarget(m_creature->getVictim());
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
            DoStartMeleeAttack(who);
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

                if (!InCombat)
                {
                    DoFaceTarget(m_creature->getVictim());
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget()) return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {
            if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                if (Enrage_Timer < diff && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 20)
                {
                    if (!InEnrage)
                    {
                        DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
                        InEnrage = true;
                    }
                    DoCast(m_creature,SPELL_ENRAGE);
                    Enrage_Timer = 6000;
                }else Enrage_Timer -= diff;

                if (Kick_Timer < diff)
                {
                    DoPlaySoundToSet(m_creature, SOUND_KICK);
                    DoCast(m_creature->getVictim(),SPELL_KICK);
                    Kick_Timer = (100 + rand()% 80) * 1000;
                }else Kick_Timer -= diff;

                if (Quake_Timer < diff)
                {
                    DoPlaySoundToSet(m_creature, SOUND_EARTHQUAKE);
                    DoCast(m_creature,SPELL_EARTHQUAKE);
                    Quake_Timer = (80 + rand()% 20) * 1000;
                }else Quake_Timer -= diff;

                if (Chain_Timer < diff)
                {
                    DoPlaySoundToSet(m_creature, SOUND_CHAIN_LIGHTNING);
                    DoCast(m_creature->getVictim(),SPELL_CHAIN_LIGHTNING);
                    Chain_Timer = (50 + rand()% 50) * 1000;
                }else Chain_Timer -= diff;

                if (Armor_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SUNDER_ARMOR);
                    Armor_Timer = (30 + rand()% 30) * 1000;
                }else Armor_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        }
    }
}; 

CreatureAI* GetAI_boss_doomwalker(Creature *_Creature)
{
    return new boss_doomwalkerAI (_Creature);
}

void AddSC_boss_doomwalker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_doomwalker";
    newscript->GetAI = GetAI_boss_doomwalker;
    m_scripts[nrscripts++] = newscript;
}

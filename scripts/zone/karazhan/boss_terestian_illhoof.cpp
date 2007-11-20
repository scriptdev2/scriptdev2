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
SDName: Boss_Terestian_Illhoof
SD%Complete: 80
SDComment: VERIFY SCRIPT
EndScriptData */

#include "../../sc_defines.h"

#define SPELL_SUMMON_DEMONCHAINS    30120 // Summons demonic chains that maintain the ritual of sacrifice.
#define SPELL_DEMON_CHAINS          30206 // Instant - Visual Effect 
#define SPELL_ENRAGE                23537 // Increases the caster's attack speed by 50% and the Physical damage it deals by 219 to 281 for 10 min.
#define SPELL_SHADOW_BOLT           41957 // Hurls a bolt of dark magic at an enemy, inflicting Shadow damage.

#define SAY_DIED1       "Your blood will anoint my circle."
#define SOUND_DIED1     9264
#define SAY_DIED2       "The great one will be pleased."
#define SOUND_DIED2     9329

#define SAY_DEATH       "My life, is yours. Oh great one."
#define SOUND_DEATH     9262

#define SAY_AGGRO       "Ah, you're just in time. The rituals are about to begin."
#define SOUND_AGGRO     9260

#define SAY_SPECIAL1    "Please, accept this humble offering, oh great one."
#define SOUND_SPECIAL1  9263
#define SAY_SPECIAL2    "Let the sacrifice serve his testament to my fealty."
#define SOUND_SPECIAL2  9330

#define SAY_SPAWN1      "Come, you dwellers in the dark. Rally to my call!"
#define SOUND_SPAWN1    9265
#define SAY_SPAWN2      "Gather, my pets. There is plenty for all."
#define SOUND_SPAWN2    9331

struct MANGOS_DLL_DECL boss_terestian_illhoofAI : public ScriptedAI
{
    boss_terestian_illhoofAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 SummonChains_Timer;
    uint32 Enrage_Timer;
    uint32 SpawnKilrek_Timer;
    uint32 SpawnHomunculus_Timer;
    uint32 ShadowBolt_Timer;

    bool InCombat, KilrekAlive, FirstSpawn;

    void EnterEvadeMode()
    {
        SummonChains_Timer      = 45000;      
        Enrage_Timer            = 600000;
        SpawnKilrek_Timer       = 0;
        SpawnHomunculus_Timer   = 10000;
        ShadowBolt_Timer        = 20000;

        InCombat = false;
        FirstSpawn = true;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();

        DoGoHome();
    }

    void KilledUnit(Unit* Victim)
    {
        if(rand()%2) return;

        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_DIED1,LANG_UNIVERSAL,Victim);
            DoPlaySoundToSet(m_creature, SOUND_DIED1);
            break;
        case 1:
            DoYell(SAY_DIED2,LANG_UNIVERSAL,Victim);
            DoPlaySoundToSet(m_creature, SOUND_DIED2);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if (!InCombat)
            {
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
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
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (SpawnKilrek_Timer < diff)
        {
            float A = m_creature->GetAngle(m_creature);
            Creature* Kilrek = m_creature->SummonCreature(17229, m_creature->GetPositionX(), m_creature->GetPositionY()+10, m_creature->GetPositionZ(), A, TEMPSUMMON_CORPSE_DESPAWN, 10000);
            Kilrek->setFaction(m_creature->getFaction());
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (target)
                Kilrek->AI()->AttackStart(target);
            else Kilrek->AI()->AttackStart(m_creature->getVictim());

            SpawnKilrek_Timer = 20000;
        }

        if (SpawnHomunculus_Timer < diff)
        {
            if (FirstSpawn)
            {
                DoYell(SAY_SPAWN1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SPAWN1);
            }                
            float A = m_creature->GetAngle(m_creature);
            Creature* Homunculus = m_creature->SummonCreature(16539, m_creature->GetPositionX()+(rand()%15), m_creature->GetPositionY()+(rand()%15), m_creature->GetPositionZ(), A, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
            Homunculus->setFaction(m_creature->getFaction());
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (target)
                Homunculus->AI()->AttackStart(target);
            else Homunculus->AI()->AttackStart(m_creature->getVictim());

            if (FirstSpawn)
            {
                DoYell(SAY_SPAWN2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SPAWN2);
                FirstSpawn = false;
            }
            SpawnHomunculus_Timer = 6000;
        }else SpawnHomunculus_Timer -= diff;

        if (ShadowBolt_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_SHADOW_BOLT);
            ShadowBolt_Timer = 10000;
        }else ShadowBolt_Timer -= diff;

        if (SummonChains_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (target)
            {

                //TODO: Summon chains creature
                DoYell(SAY_SPECIAL1,LANG_UNIVERSAL,target);
                DoPlaySoundToSet(m_creature, SOUND_SPECIAL1);
                DoCast(target,SPELL_SUMMON_DEMONCHAINS);
                DoCast(target,SPELL_DEMON_CHAINS);
                DoYell(SAY_SPECIAL2,LANG_UNIVERSAL,target);
                DoPlaySoundToSet(m_creature, SOUND_SPECIAL2);
            }
            else
            {
                DoYell(SAY_SPECIAL1,LANG_UNIVERSAL,m_creature->getVictim());
                DoPlaySoundToSet(m_creature, SOUND_SPECIAL1);
                DoCast(m_creature->getVictim(),SPELL_SUMMON_DEMONCHAINS);
                DoCast(m_creature->getVictim(),SPELL_DEMON_CHAINS);
                DoYell(SAY_SPECIAL2,LANG_UNIVERSAL,m_creature->getVictim());
                DoPlaySoundToSet(m_creature, SOUND_SPECIAL2);
            }
            SummonChains_Timer = 50000;
        }else SummonChains_Timer -= diff;

        if (Enrage_Timer < diff)
        { 
            DoCast(m_creature,SPELL_ENRAGE);
            Enrage_Timer = 600000; 
        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_terestian_illhoof(Creature *_Creature)
{
    return new boss_terestian_illhoofAI (_Creature);
}

void AddSC_boss_terestian_illhoof()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_terestian_illhoof";
    newscript->GetAI = GetAI_boss_terestian_illhoof;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: boss_Void_Reaver
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../../sc_defines.h"
#include "def_the_eye.h"

#define SPELL_POUNDING              34162
#define SPELL_ARCANE_ORB_TRIGGER    34172
#define SPELL_ARCANE_ORB            34190
#define SPELL_KNOCK_AWAY            11130
#define SPELL_BERSERK               27680

#define SAY_AGGRO               "Alert, you are marked for extermination!"
#define SAY_SLAY1               "Extermination, successful."
#define SAY_SLAY2               "Imbecile life form, no longer functional."
#define SAY_SLAY3               "Threat neutralized."
#define SAY_DEATH               "Systems... shutting... down..."

#define SOUND_AGGRO             11213
#define SOUND_SLAY1             11215
#define SOUND_SLAY2             11216
#define SOUND_SLAY3             11217
#define SOUND_DEATH             11214

#define CREATURE_ORB_TARGET     19577

struct MANGOS_DLL_DECL boss_void_reaverAI : public ScriptedAI
{
    boss_void_reaverAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 Pounding_Timer;
    uint32 ArcaneOrb_Timer;
    uint32 KnockAway_Timer;
    uint32 Berserk_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {       
        Pounding_Timer = 12000;
        ArcaneOrb_Timer = 3000;
        KnockAway_Timer = 30000;
        Berserk_Timer = 600000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData(DATA_VOIDREAVEREVENT, 0);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_SLAY1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;

        case 1:
            DoYell(SAY_SLAY2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            break;

        case 2:
            DoYell(SAY_SLAY3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY3);
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if(pInstance)
            pInstance->SetData(DATA_VOIDREAVEREVENT, 0);
    }

    void AttackStart(Unit *who)
    {        
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            //Say our dialog
            if (!InCombat)
            {
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
                if(pInstance)
                    pInstance->SetData(DATA_VOIDREAVEREVENT, 1);
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

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                    if(pInstance)
                        pInstance->SetData(DATA_VOIDREAVEREVENT, 1);
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

        // Pounding
        if(Pounding_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_POUNDING);
            Pounding_Timer = 12000;
        }else Pounding_Timer -= diff;

        // Arcane Orb
        if(ArcaneOrb_Timer < diff)
        {
            Unit *target;
            std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();
            std::vector<Unit *> target_list;
            for(std::list<HostilReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if(target && target->GetDistance2dSq(m_creature) > 225)     //15 yard radius minimum
                    target_list.push_back(target);
                target = NULL;
            }
            if(target_list.size())
                target = *(target_list.begin()+rand()%target_list.size());

            if (target)
            {
                Unit* Spawn = NULL;
                Spawn = m_creature->SummonCreature(CREATURE_ORB_TARGET, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 3000);
                if (Spawn)
                    m_creature->CastSpell(Spawn, SPELL_ARCANE_ORB_TRIGGER, true);
            }

            ArcaneOrb_Timer = 3000;
        }else ArcaneOrb_Timer -= diff;

        // Single Target knock back, reduces aggro
        if(KnockAway_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_KNOCK_AWAY);

            //Drop 25% aggro
            m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-25);

            KnockAway_Timer = 30000;
        }else KnockAway_Timer -= diff;

        //Berserk
        if(Berserk_Timer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);

            DoCast(m_creature,SPELL_BERSERK);
            Berserk_Timer = 600000;
        }else Berserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 


struct MANGOS_DLL_DECL arcane_orb_targetAI : public ScriptedAI
{
    arcane_orb_targetAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {       
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {        
    }

    void MoveInLineOfSight(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
    }

    void SpellHit(Unit *Attacker, const SpellEntry *Spellkind)
    {
        if (!Attacker || Spellkind->Id != SPELL_ARCANE_ORB_TRIGGER)
            return;

        //Cast arcane orb
        m_creature->setFaction(14);
        m_creature->CastSpell(m_creature, SPELL_ARCANE_ORB, true);

        m_creature->setDeathState(JUST_DIED);
        m_creature->RemoveCorpse();
        m_creature->setFaction(35);
    }
}; 

CreatureAI* GetAI_boss_void_reaver(Creature *_Creature)
{
    return new boss_void_reaverAI (_Creature);
}

CreatureAI* GetAI_arcane_orb_target(Creature *_Creature)
{
    return new arcane_orb_targetAI (_Creature);
}

void AddSC_boss_void_reaver()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_void_reaver";
    newscript->GetAI = GetAI_boss_void_reaver;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_arcane_orb_target";
    newscript->GetAI = GetAI_arcane_orb_target;
    m_scripts[nrscripts++] = newscript;
}

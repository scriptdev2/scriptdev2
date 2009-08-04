/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Maexxna
SD%Complete: 60
SDComment: this needs review, and rewrite of the webwrap ability
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"

#define SPELL_WEBTRAP           28622                       //Spell is normally used by the webtrap on the wall NOT by Maexxna

#define SPELL_WEBSPRAY          29484
#define H_SPELL_WEBSPRAY        54125
#define SPELL_POISONSHOCK       28741
#define H_SPELL_POISONSHOCK     54122
#define SPELL_NECROTICPOISON    28776
#define H_SPELL_NECROTICPOISON  54121
#define SPELL_FRENZY            54123
#define H_SPELL_FRENZY          54124

//spellId invalid
#define SPELL_SUMMON_SPIDERLING 29434

#define LOC_X1    3546.796
#define LOC_Y1    -3869.082
#define LOC_Z1    296.450

#define LOC_X2    3531.271
#define LOC_Y2    -3847.424
#define LOC_Z2    299.450

#define LOC_X3    3497.067
#define LOC_Y3    -3843.384
#define LOC_Z3    302.384

struct MANGOS_DLL_DECL mob_webwrapAI : public ScriptedAI
{
    mob_webwrapAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 victimGUID;

    void Reset()
    {
        victimGUID = 0;
    }

    void SetVictim(Unit* victim)
    {
        if (victim)
        {
            victimGUID = victim->GetGUID();
            victim->CastSpell(victim, SPELL_WEBTRAP, true);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth())
        {
            if (victimGUID)
            {
                Unit* victim = NULL;
                victim = Unit::GetUnit((*m_creature), victimGUID);
                victim->RemoveAurasDueToSpell(SPELL_WEBTRAP);
            }
        }
    }

    void MoveInLineOfSight(Unit *who) { }
    void UpdateAI(const uint32 diff) { }
};

struct MANGOS_DLL_DECL boss_maexxnaAI : public ScriptedAI
{
    boss_maexxnaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 WebTrap_Timer;
    uint32 WebSpray_Timer;
    uint32 PoisonShock_Timer;
    uint32 NecroticPoison_Timer;
    uint32 SummonSpiderling_Timer;
    bool Enraged;

    void Reset()
    {
        WebTrap_Timer = 20000;                              //20 sec init, 40 sec normal
        WebSpray_Timer = 40000;                             //40 seconds
        PoisonShock_Timer = 20000;                          //20 seconds
        NecroticPoison_Timer = 30000;                       //30 seconds
        SummonSpiderling_Timer = 30000;                     //30 sec init, 40 sec normal
        Enraged = false;
    }

    void DoCastWebWrap()
    {
        std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();
        std::vector<Unit *> targets;

        //This spell doesn't work if we only have 1 player on threat list
        if (t_list.size() < 2)
            return;

        //begin + 1 , so we don't target the one with the highest threat
        std::list<HostilReference *>::iterator itr = t_list.begin();
        std::advance(itr, 1);

        //store the threat list in a different container
        for(; itr!= t_list.end(); ++itr)
        {
            Unit* target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());

            //only on alive players
            if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER)
                targets.push_back(target);
        }

        //cut down to size if we have more than 3 targets
        while(targets.size() > 3)
            targets.erase(targets.begin()+rand()%targets.size());

        int i = 0;

        for(std::vector<Unit *>::iterator iter = targets.begin(); iter!= targets.end(); ++iter, ++i)
        {
            // Teleport the 3 targets to a location on the wall and summon a Web Wrap on them
            switch(i)
            {
                case 0:
                    DoTeleportPlayer((*iter), LOC_X1, LOC_Y1, LOC_Z1, (*iter)->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(16486, LOC_X1, LOC_Y1, LOC_Z1, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
                        ((mob_webwrapAI*)pWrap->AI())->SetVictim((*iter));
                    break;
                case 1:
                    DoTeleportPlayer((*iter), LOC_X2, LOC_Y2, LOC_Z2, (*iter)->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(16486, LOC_X2, LOC_Y2, LOC_Z2, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
                        ((mob_webwrapAI*)pWrap->AI())->SetVictim((*iter));
                    break;
                case 2:
                    DoTeleportPlayer((*iter), LOC_X3, LOC_Y3, LOC_Z3, (*iter)->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(16486, LOC_X3, LOC_Y3, LOC_Z3, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
                        ((mob_webwrapAI*)pWrap->AI())->SetVictim((*iter));
                    break;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //WebTrap_Timer
        if (WebTrap_Timer < diff)
        {
            DoCastWebWrap();
            WebTrap_Timer = 40000;
        }else WebTrap_Timer -= diff;

        //WebSpray_Timer
        if (WebSpray_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_WEBSPRAY);
            WebSpray_Timer = 40000;
        }else WebSpray_Timer -= diff;

        //PoisonShock_Timer
        if (PoisonShock_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_POISONSHOCK);
            PoisonShock_Timer = 20000;
        }else PoisonShock_Timer -= diff;

        //NecroticPoison_Timer
        if (NecroticPoison_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_NECROTICPOISON);
            NecroticPoison_Timer = 30000;
        }else NecroticPoison_Timer -= diff;

        //SummonSpiderling_Timer
        if (SummonSpiderling_Timer < diff)
        {
            DoCast(m_creature, SPELL_SUMMON_SPIDERLING);
            SummonSpiderling_Timer = 40000;
        }else SummonSpiderling_Timer -= diff;

        //Enrage if not already enraged and below 30%
        if (!Enraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30)
        {
            DoCast(m_creature,SPELL_FRENZY);
            Enraged = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_webwrap(Creature* pCreature)
{
    return new mob_webwrapAI(pCreature);
}

CreatureAI* GetAI_boss_maexxna(Creature* pCreature)
{
    return new boss_maexxnaAI(pCreature);
}

void AddSC_boss_maexxna()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_maexxna";
    newscript->GetAI = &GetAI_boss_maexxna;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_webwrap";
    newscript->GetAI = &GetAI_mob_webwrap;
    newscript->RegisterSelf();
}

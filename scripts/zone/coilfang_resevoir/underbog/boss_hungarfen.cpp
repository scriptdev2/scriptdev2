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
SDName: Boss_Hungarfen
SD%Complete: 100
SDComment: Missing model id for Mushrooms
EndScriptData */

#include "sc_creature.h"

// Hungarfen

#define SPELL_FOUL_SPORES 31673 // 100% correct,however,heals enemies either O.o

struct MANGOS_DLL_DECL boss_hungarfenAI : public ScriptedAI
{
    boss_hungarfenAI(Creature *c) : ScriptedAI(c) {Reset();}

    bool Root;
    uint32 Mushroom_timer;

    void Reset()
    {
        Root = false;
        Mushroom_timer = 5000;     // 1 mushroom after 5s, then one per 10s.

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();       
    }

    void AttackStart(Unit *who)
    {
        if (!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin melee attack if we are within range
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


    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_creature->getVictim() && m_creature->isAlive()) {
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 20) { // under 20% self root for up to 11sec. and cast soul spores every sec.
                if (!Root) {
                    DoCast(m_creature, SPELL_FOUL_SPORES);
                    Root = true;
                }
            }

            if (Mushroom_timer < diff) {
                Unit* target = NULL;
                Unit* pSummon = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                if (target)     // spawn a mushroom around random player
                    pSummon = m_creature->SummonCreature(17990, target->GetPositionX()+(rand()%3), target->GetPositionY()+(rand()%3), target->GetPositionZ(), (rand()%5), TEMPSUMMON_TIMED_DESPAWN, 30000);
                else            // if player not found, spawn around boss
                    pSummon = m_creature->SummonCreature(17990, m_creature->GetPositionX()+(rand()%8), m_creature->GetPositionY()+(rand()%8), m_creature->GetPositionZ(), (rand()%5), TEMPSUMMON_TIMED_DESPAWN, 30000);
                
                if (pSummon)
                    pSummon->setFaction(m_creature->getFaction());

                Mushroom_timer = 10000;
            }else Mushroom_timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};
CreatureAI* GetAI_boss_hungarfen(Creature *_Creature)
{
    return new boss_hungarfenAI (_Creature);
}


// Underbog Mushroom: 17990

#define SPELL_SPORE_CLOUD 31689

struct MANGOS_DLL_DECL mob_underbog_mushroomAI : public ScriptedAI
{
    mob_underbog_mushroomAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Grow_timer;    // grow for 20s (adjust scale modifiers with this!)
    uint32 Shrink_timer;  // shrink in 3s
    float Scale;
    bool Exploded;
    bool RootSelf;

    void Reset() {
        Grow_timer = 20000;    // grow for 20s
        Shrink_timer = 3000;   // shrink in 3s
        Scale = 0.1;           // start with small mushroomie
        Exploded = false;
        RootSelf = false;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();

        // let's make mushrooms immune to CC effects to force 'em exploding &&frozen in place
        
        
        
        
        
        
        
        
        
        
        
        
        
    }

    void UpdateAI(const uint32 diff) 
    {
        if (!RootSelf) {
            DoCast(m_creature, 33356);  // stun animation :/
            RootSelf = true;
        }

        if (!(Grow_timer < diff)) 
        { // keep growing for 20s
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, Scale);
            Scale += 0.013;  // needs testing, final value 2.7
            Grow_timer -= diff;
        }else
        {                      // explode when grown
            if (!Exploded) 
            {
                DoCast(m_creature, SPELL_SPORE_CLOUD);
                Exploded = true;
            }

            if (!(Shrink_timer < diff)) 
            {   // start shrinking,then turn invisible;
                Scale -= 0.08;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, Scale);
                Shrink_timer -= diff;
            }else
            {
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);  // invisible
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false); // kill
            }
        }
    }
};
CreatureAI* GetAI_mob_underbog_mushroom(Creature *_Creature)
{
    return new mob_underbog_mushroomAI (_Creature);
}


void AddSC_boss_hungarfen()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_hungarfen";
    newscript->GetAI = GetAI_boss_hungarfen;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_underbog_mushroom";
    newscript->GetAI = GetAI_mob_underbog_mushroom;
    m_scripts[nrscripts++] = newscript;
}

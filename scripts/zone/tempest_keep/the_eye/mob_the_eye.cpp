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

#include "../../../sc_defines.h"
#include "../../../creature/simple_ai.h"

#define SPELL_BURN                             36721
#define PHOENIX                                21362
#define PHOENIX_EGG                            21364

struct MANGOS_DLL_DECL mob_phoenixAI : public ScriptedAI
{
    mob_phoenixAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Burn_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Burn_Timer = 1000;

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
            if(!InCombat)
            {
                InCombat = true;
            }

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

        //Check if we have a current target
        if (Burn_Timer < diff)
        {        
            if (m_creature->GetHealth() <= m_creature->GetMaxHealth() * 0.05)
            {
                m_creature->setDeathState(JUST_DIED);
                DoSpawnCreature(PHOENIX_EGG, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
            }
            else
                m_creature->SetHealth(m_creature->GetHealth() - m_creature->GetMaxHealth() * 0.05);
            DoCast(m_creature->getVictim(), SPELL_BURN);
            Burn_Timer = 1000;
        }else Burn_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_phoenix_eggAI : public ScriptedAI
{
    mob_phoenix_eggAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 RebirthTimer;

    void EnterEvadeMode()
    {
        RebirthTimer = 15000;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        return; //ignore all
    }

    void MoveInLineOfSight(Unit *who)
    {
        return; //ignore all
    }

    void UpdateAI(const uint32 diff)
    {
        if (RebirthTimer < diff)
        {        
            DoSpawnCreature(PHOENIX, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
            m_creature->setDeathState(JUST_DIED);
        }else RebirthTimer -= diff;
    }
};

CreatureAI* GetAI_mob_phoenix_egg(Creature *_Creature)
{
    return new mob_phoenix_eggAI (_Creature);
}

CreatureAI* GetAI_mob_phoenix(Creature *_Creature)
{
    return new mob_phoenixAI (_Creature);
}

void AddSC_mob_phoenix()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_phoenix";
    newscript->GetAI = GetAI_mob_phoenix;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_phoenix_egg";
    newscript->GetAI = GetAI_mob_phoenix_egg;
    m_scripts[nrscripts++] = newscript;
}


CreatureAI* GetAI_mob_the_eye(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        //Apprentice Star Scryer
    case 20043: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 40424;      //Arcane Volley       
        ai->Spell[0].Cooldown = 250000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 27082;        //Arcane Explosion
        ai->Spell[1].Cooldown = 19000;
        ai->Spell[1].First_Cast = 17000;
        ai->Spell[1].Cast_Target_Type = CAST_SELF;
        break;
        //Star Scryer
    case 20034: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 37124;      //Starfall     
        ai->Spell[0].Cooldown = 350000;                 
        ai->Spell[0].First_Cast = 25000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;
        //Astromancer
    case 20033: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 30482;        // Molten Armor
        ai->Spell[0].Cooldown = -1;                 
        ai->Spell[0].First_Cast = 100;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 29922;        // Fireball Volley
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 19000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 33933;        // Blast Wave
        ai->Spell[2].Cooldown = 22000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;
        break;
        //Astromancer Lord
    case 20046: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 35265;        // Fire Shield
        ai->Spell[0].Cooldown = -1;                 
        ai->Spell[0].First_Cast = 100;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 29922;        // Fireball Volley
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 19000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

        ai->Spell[2].Enabled = true;
        ai->Spell[2].Spell_Id = 33933;        // Blast Wave
        ai->Spell[2].Cooldown = 22000;
        ai->Spell[2].First_Cast = 15000;
        ai->Spell[2].Cast_Target_Type = CAST_SELF;

        ai->Spell[3].Enabled = true;
        ai->Spell[3].Spell_Id = 33043;        // Dragon's Breath
        ai->Spell[3].Cooldown = 29000;
        ai->Spell[3].First_Cast = 22000;
        ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Bloodwarder Vindicator
    case 20032: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 10308;        // Hammer of Justice
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
        //Bloodwarder Legionnaire
    case 20031: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 36981;        // Whirlwind
        ai->Spell[0].Cooldown = 27000;                 
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 26350;        // Cleave
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Bloodwarder Marshal
    case 20035: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 36981;        // Whirlwind
        ai->Spell[0].Cooldown = 29000;                 
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_SELF; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 26007;        // Uppercut
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Phoenix-Hawk
    case 20039: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 25380;        // Mana Burn Rank 7
        ai->Spell[0].Cooldown = 19000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_RANDOM; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 38110;        // Wing Buffet
        ai->Spell[1].Cooldown = 25000;
        ai->Spell[1].First_Cast = 20000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Crystalcore Sentinel
    case 20041: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 40340;        // Trample
        ai->Spell[0].Cooldown = 25000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

        ai->Spell[1].Enabled = true;
        ai->Spell[1].Spell_Id = 37106;        // Charged Arcane Explosion
        ai->Spell[1].Cooldown = 45000;
        ai->Spell[1].First_Cast = 25000;
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
        //Crystalcore Mechanic
    case 20052: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 37123;        // Saw Blade
        ai->Spell[0].Cooldown = 35000;                 
        ai->Spell[0].First_Cast = 15000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_mob_the_eye()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_the_eye";
    newscript->GetAI = GetAI_mob_the_eye;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: Mobs_Molten_Core
SD%Complete: 100
SDComment: 
SDCategory: Molten Core
EndScriptData */

#include "../../creature/simple_ai.h"



// -- Ancient Core Hound Spells --
 
//Standart Spells
#define SPELL_CONE_OF_FIRE          19630
#define SPELL_BITE                  19771

//Random Debuff (each hound has only one of these)
#define SPELL_GROUND_STOMP          19364    
#define SPELL_ANCIENT_DREAD         19365                     
#define SPELL_CAUTERIZING_FLAMES    19366 
#define SPELL_WITHERING_HEAT        19367 
#define SPELL_ANCIENT_DESPAIR       19369 
#define SPELL_ANCIENT_HYSTERIA      19372     

// -- FireLord Spells --

#define SPELL_SOUL_BURN             19393
#define SPELL_SUMMON_LAVA_SPAWN     19392


// -- Firewalker Spells --           

#define SPELL_INCITEFLAMES          19635    
#define SPELL_MELTARMOR             19631    


// -- FlameGuard Spells --

#define SPELL_FIRESHIELD            19626    
#define SPELL_FLAME                 19628  

// Also using Cone of Fire from Ancient Core Hound


// -- FlameWaker Spells --

#define SPELL_FISTOFRAGNAROS        20277      
#define SPELL_SUNDERARMOR           21081                     
#define SPELL_STRIKE                19730


// -- FlamewakerElite Spells --

#define SPELL_BLASTWAVE             20229


// -- FlamewakerHealer Spells --

// Using same spells as Flamewaker Elite 

#define SPELL_SHADOWBOLT            21077



// -- FlamewakerProtector Spells --

#define SPELL_CLEAVE                20691    
#define SPELL_DOMINATEMIND          20740  


// -- LavaSpawn Spells --

#define SPELL_SPLIT                 19569      // Not working I think.


// -- MoltenDestroyer Spells --

#define SPELL_TRAMPLE               19129    
#define SPELL_KNOCKDOWN             20276 
#define SPELL_SMASH                 18944    


// -- MoltenGiant Spells --

#define SPELL_KNOCKBACK             19813    
#define SPELL_STOMP                 16727       

 
CreatureAI* GetAI_mob_ancient_core_hound(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled          = true;
    ai->Spell[0].Spell_Id         = SPELL_CONE_OF_FIRE;
    ai->Spell[0].Cooldown         = 7000;
    ai->Spell[0].First_Cast       = 10000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
 
    uint32 RandDebuff;
    switch(rand()%6)
    {
        case 0 : RandDebuff = SPELL_GROUND_STOMP;       break;
        case 1 : RandDebuff = SPELL_ANCIENT_DREAD;      break;
        case 2 : RandDebuff = SPELL_CAUTERIZING_FLAMES; break;
        case 3 : RandDebuff = SPELL_WITHERING_HEAT;     break;
        case 4 : RandDebuff = SPELL_ANCIENT_DESPAIR;    break;
        case 5 : RandDebuff = SPELL_ANCIENT_HYSTERIA;   break;
    }
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = RandDebuff;
    ai->Spell[1].Cooldown         = 24000;
    ai->Spell[1].First_Cast       = 15000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->Spell[2].Enabled          = true;
    ai->Spell[2].Spell_Id         = SPELL_BITE;
    ai->Spell[2].Cooldown         = 6000;
    ai->Spell[2].First_Cast       = 4000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    ai->EnterEvadeMode();
 
    return ai;
}
 
CreatureAI* GetAI_mob_firelord(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_SOUL_BURN;
    ai->Spell[0].First_Cast             = 6000;
    ai->Spell[0].Cooldown               = 15000;
    ai->Spell[0].CooldownRandomAddition = 7000;
    ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_RANDOM;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_SUMMON_LAVA_SPAWN;
    ai->Spell[1].First_Cast       = 10000;
    ai->Spell[1].Cooldown         = 30000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
 
    ai->EnterEvadeMode();
 
    return ai;
}

CreatureAI* GetAI_mob_firewalker(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_INCITEFLAMES;
    ai->Spell[0].First_Cast             = 12000;
    ai->Spell[0].Cooldown               = 8000;
    ai->Spell[0].CooldownRandomAddition = 7000;
    ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_TARGET;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_MELTARMOR;
    ai->Spell[1].First_Cast       = 8000;
    ai->Spell[1].Cooldown         = 15000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
 
    ai->EnterEvadeMode();
 
    return ai;
}

CreatureAI* GetAI_mob_flame_guard(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_FIRESHIELD;
    ai->Spell[0].First_Cast             = 2000;
    ai->Spell[0].Cooldown               = 60000;
    ai->Spell[0].Cast_Target_Type       = CAST_SELF;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_FLAME;
    ai->Spell[1].First_Cast       = 7000;
    ai->Spell[1].Cooldown         = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->Spell[2].Enabled          = true;
    ai->Spell[2].Spell_Id         = SPELL_CONE_OF_FIRE;
    ai->Spell[2].First_Cast       = 12000;
    ai->Spell[2].Cooldown         = 10000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
 
    ai->EnterEvadeMode();
 
    return ai;
}

CreatureAI* GetAI_mob_flamewaker(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_FISTOFRAGNAROS;
    ai->Spell[0].First_Cast             = 12000;
    ai->Spell[0].Cooldown               = 12000;
    ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_TARGET;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_SUNDERARMOR;
    ai->Spell[1].First_Cast       = 5000;
    ai->Spell[1].Cooldown         = 15000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->Spell[2].Enabled          = true;
    ai->Spell[2].Spell_Id         = SPELL_STRIKE;
    ai->Spell[2].First_Cast       = 8000;
    ai->Spell[2].Cooldown         = 8000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
 
    ai->EnterEvadeMode();
 
    return ai;
}
 

CreatureAI* GetAI_mob_flamewaker_elite(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);

    ai->Spell[0].Enabled          = true;
    ai->Spell[0].Spell_Id         = SPELL_BLASTWAVE;
    ai->Spell[0].First_Cast       = 12000;
    ai->Spell[0].Cooldown         = 12000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
 
    ai->EnterEvadeMode();
 
    return ai;
}

CreatureAI* GetAI_mob_flamewaker_healer(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);

    ai->Spell[0].Enabled          = true;
    ai->Spell[0].Spell_Id         = SPELL_SHADOWBOLT;
    ai->Spell[0].First_Cast       = 2000;
    ai->Spell[0].Cooldown         = 2000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
  
    ai->EnterEvadeMode();
 
    return ai;
}


CreatureAI* GetAI_mob_flamewaker_protector(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_CLEAVE;
    ai->Spell[0].First_Cast             = 6000;
    ai->Spell[0].Cooldown               = 6000;
    ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_TARGET;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_DOMINATEMIND;
    ai->Spell[1].First_Cast       = 15000;
    ai->Spell[1].Cooldown         = 30000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;
 
    ai->EnterEvadeMode();
 
    return ai;
}

CreatureAI* GetAI_mob_lavaspawn(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled          = true;
    ai->Spell[0].Spell_Id         = SPELL_SUMMON_LAVA_SPAWN;
    ai->Spell[0].First_Cast       = 12000;
    ai->Spell[0].Cooldown         = 12000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;
 
    ai->EnterEvadeMode();
 
    return ai;
}
 
CreatureAI* GetAI_mob_molten_destroyer(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_KNOCKDOWN;
    ai->Spell[0].First_Cast             = 12000;
    ai->Spell[0].Cooldown               = 10000;
    ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_TARGET;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_SMASH;
    ai->Spell[1].First_Cast       = 7000;
    ai->Spell[1].Cooldown         = 8000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_TRAMPLE;
    ai->Spell[1].First_Cast       = 18000;
    ai->Spell[1].Cooldown         = 12000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    ai->EnterEvadeMode();
 
    return ai;
}

CreatureAI* GetAI_mob_molten_giant(Creature *_Creature)
{
    SimpleAI *ai = new SimpleAI(_Creature);
 
    ai->Spell[0].Enabled                = true;
    ai->Spell[0].Spell_Id               = SPELL_KNOCKBACK;
    ai->Spell[0].First_Cast             = 15000;
    ai->Spell[0].Cooldown               = 15000;
    ai->Spell[0].Cast_Target_Type       = CAST_HOSTILE_TARGET;
 
    ai->Spell[1].Enabled          = true;
    ai->Spell[1].Spell_Id         = SPELL_STOMP;
    ai->Spell[1].First_Cast       = 8000;
    ai->Spell[1].Cooldown         = 10000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
 
    ai->EnterEvadeMode();
 
    return ai;
}


void AddSC_mobs_molten_core()
{
    Script *newscript;
    
    //Ancient Core Hound
    newscript = new Script;
    newscript->Name="mob_ancient_core_hound";
    newscript->GetAI = GetAI_mob_ancient_core_hound;
    m_scripts[nrscripts++] = newscript;

    //Firelord
    newscript = new Script;
    newscript->Name="mob_firelord";
    newscript->GetAI = GetAI_mob_firelord;
    m_scripts[nrscripts++] = newscript;
 
    //Firewalker
    newscript = new Script;
    newscript->Name="mob_firewalker";
    newscript->GetAI = GetAI_mob_firewalker;
    m_scripts[nrscripts++] = newscript;

    //FlameGuard
    newscript = new Script;
    newscript->Name="mob_flame_guard";
    newscript->GetAI = GetAI_mob_flame_guard;
    m_scripts[nrscripts++] = newscript;

    //Flamewaker
    newscript = new Script;
    newscript->Name="mob_flamewaker";
    newscript->GetAI = GetAI_mob_flamewaker;
    m_scripts[nrscripts++] = newscript;

    //Flamewaker Elite
    newscript = new Script;
    newscript->Name="mob_flamewaker_elite";
    newscript->GetAI = GetAI_mob_flamewaker_elite;
    m_scripts[nrscripts++] = newscript;

    //Flamewaker Healer
    newscript = new Script;
    newscript->Name="mob_flamewaker_healer";
    newscript->GetAI = GetAI_mob_flamewaker_healer;
    m_scripts[nrscripts++] = newscript;

    //Flamewaker Protector
    newscript = new Script;
    newscript->Name="mob_flamewaker_protector";
    newscript->GetAI = GetAI_mob_flamewaker_protector;
    m_scripts[nrscripts++] = newscript;
 
    //Lava Spawn
    newscript = new Script;
    newscript->Name="mob_lavaspawn";
    newscript->GetAI = GetAI_mob_lavaspawn;
    m_scripts[nrscripts++] = newscript;

    //Molten Destroyer
    newscript = new Script;
    newscript->Name="mob_molten_destroyer";
    newscript->GetAI = GetAI_mob_molten_destroyer;
    m_scripts[nrscripts++] = newscript;

    //Molten Giant
    newscript = new Script;
    newscript->Name="mob_molten_giant";
    newscript->GetAI = GetAI_mob_molten_giant;
    m_scripts[nrscripts++] = newscript;

}

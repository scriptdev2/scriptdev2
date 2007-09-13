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
#include "../../creature/simple_ai.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"

// **** This script is still under Developement ****

/*######
## mob_freed_soul
######*/

//Possibly more of these quotes around.
#define SAY_ZAPPED0 "Thanks to Egan"
#define SAY_ZAPPED1 "Rivendare must die"
#define SAY_ZAPPED2 "Who you gonna call?"
#define SAY_ZAPPED3 "Don't cross those beams!"

struct MANGOS_DLL_DECL mob_freed_soulAI : public ScriptedAI
{
    mob_freed_soulAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        switch (rand()%4)
        {
        case 0:
            DoSay(SAY_ZAPPED0,LANG_UNIVERSAL,NULL);
            break;
        case 1:
            DoSay(SAY_ZAPPED1,LANG_UNIVERSAL,NULL);
            break;
        case 2:
            DoSay(SAY_ZAPPED2,LANG_UNIVERSAL,NULL);
            break;
        case 3:
            DoSay(SAY_ZAPPED3,LANG_UNIVERSAL,NULL);
            break;
        }
    }
};
CreatureAI* GetAI_mob_freed_soul(Creature *_Creature)
{
    return new mob_freed_soulAI (_Creature);
}
/*######
## mob_restless_soul
######*/

struct MANGOS_DLL_DECL mob_restless_soulAI : public ScriptedAI
{
    mob_restless_soulAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    Unit* PlayerHolder;
    uint32 Die_Timer;
    bool OkToDie;

    void EnterEvadeMode()
    {
        Die_Timer = 10000;
        OkToDie = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        PlayerHolder = NULL;
    }

    void SummonFreedSoul(Unit* victim)
    {
    int Rand;
    int RandX;
    int RandY;

        Rand = rand()%1;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%1;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        DoSpawnCreature(11136, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 300000);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if(!PlayerHolder && !OkToDie && spell->Id == 17368 && ((Player*)caster)->GetQuestStatus(5282) == QUEST_STATUS_INCOMPLETE)
            {
                PlayerHolder = caster;
                OkToDie = true;
                Die_Timer = 5000;
            }
        }
        return;
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
        {
            //check quest status
            if(PlayerHolder == ((Player*)Killer) && ((Player*)Killer)->GetQuestStatus(5282) == QUEST_STATUS_INCOMPLETE)
            {
                SummonFreedSoul(m_creature->getVictim());
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (PlayerHolder && OkToDie && Die_Timer < diff)
        {
            PlayerHolder->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
        }
        else Die_Timer -= diff;

        return;
    }
};
CreatureAI* GetAI_mob_restless_soul(Creature *_Creature)
{
    return new mob_restless_soulAI (_Creature);
}

/*######
## mobs_spectral_ghostly_citizen
######*/

struct MANGOS_DLL_DECL mobs_spectral_ghostly_citizenAI : public ScriptedAI
{
    mobs_spectral_ghostly_citizenAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    Unit* PlayerHolder;
    uint32 Die_Timer;
    bool OkToDie;

    void EnterEvadeMode()
    {
        Die_Timer = 5000;
        OkToDie = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        PlayerHolder = NULL;
    }

    void SummonRestlessSoul(Unit* victim)
    {
    int Rand;
    int RandX;
    int RandY;

        Rand = rand()%7;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%7;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        DoSpawnCreature(11122, RandX, RandY, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 600000);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if(!PlayerHolder && !OkToDie && spell->Id == 17368 && ((Player*)caster)->GetQuestStatus(5282) == QUEST_STATUS_INCOMPLETE)
            {
                PlayerHolder = caster;
                OkToDie = true;
                Die_Timer = 5000;
            }
        }
        return;
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
        {
            //check quest status
            if(PlayerHolder == ((Player*)Killer) && ((Player*)Killer)->GetQuestStatus(5282) == QUEST_STATUS_INCOMPLETE)
            {
                SummonRestlessSoul(m_creature->getVictim());                        //always one
                if (rand()%100 < 90) SummonRestlessSoul(m_creature->getVictim());   //90% a second
                if (rand()%100 < 50) SummonRestlessSoul(m_creature->getVictim());   //50% a third
                if (rand()%100 < 30) SummonRestlessSoul(m_creature->getVictim());   //30% a fourth
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (PlayerHolder && OkToDie && Die_Timer < diff)
        {
            PlayerHolder->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
        }
        else Die_Timer -= diff;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            DoMeleeAttackIfReady();
        }
    }
};
CreatureAI* GetAI_mobs_spectral_ghostly_citizen(Creature *_Creature)
{
    return new mobs_spectral_ghostly_citizenAI (_Creature);
}
bool ReciveEmote_mobs_spectral_ghostly_citizen(Player *player, Creature *_Creature, uint32 emote)
{
    _Creature->HandleEmoteCommand(emote);

    if (emote == EMOTE_STATE_DANCE)
        ((mobs_spectral_ghostly_citizenAI*)_Creature->AI())->EnterEvadeMode();

    return true;
}

/*######
## mob_bile_slime
######*/

CreatureAI* GetAI_mob_bile_slime(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //venom spit
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16866;
    ai->Spell[0].Cooldown = 10000; 
    ai->Spell[0].First_Cast = 4000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //slowing poison
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 7992;
    ai->Spell[1].Cooldown = 15000;
    ai->Spell[1].First_Cast = 1000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_bile_spewer
######*/

CreatureAI* GetAI_mob_bile_spewer(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //spawn bile slimes
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16865;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //venom spit
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 24011;
    ai->Spell[1].Cooldown = 15000;
    ai->Spell[1].First_Cast = 1000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_black_guard_sentry
######*/

CreatureAI* GetAI_mob_black_guard_sentry(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //shadow shock
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 17439;
    ai->Spell[0].Cooldown = 6000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //cleave
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 15584;
    ai->Spell[1].Cooldown = 10000;
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_broken_cadaver
######*/

CreatureAI* GetAI_mob_broken_cadaver(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //dazed
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 13496;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //exploding cadaver
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 16141;
    ai->Spell[1].Cooldown = -1;
    ai->Spell[1].First_Cast = 10000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_battle_mage
######*/

CreatureAI* GetAI_mob_crimson_battle_mage(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //scorch
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 13020;
    ai->Spell[0].Cooldown = 7000;
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //blast wave
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 13020;
    ai->Spell[1].Cooldown = 13000;
    ai->Spell[1].First_Cast = 8000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    //immolate
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 11667;
    ai->Spell[2].Cooldown = 20000;
    ai->Spell[2].First_Cast = 2000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_conjuror
######*/

CreatureAI* GetAI_mob_crimson_conjuror(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //summon water elemental
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 17162;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //frost bolt
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 17503;
    ai->Spell[1].Cooldown = 6000;
    ai->Spell[1].First_Cast = 2000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //frost nova
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 22645;
    ai->Spell[2].Cooldown = 15000;
    ai->Spell[2].First_Cast = 10000;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_defender
######*/

CreatureAI* GetAI_mob_crimson_defender(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //retribution aura
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 10301;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //hammer of justice
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 10308;
    ai->Spell[1].Cooldown = 60000;
    ai->Spell[1].First_Cast = 10000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //seal of light
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 20349;
    ai->Spell[2].Cooldown = 45000;
    ai->Spell[2].First_Cast = 5000;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    //should cast at 20% health left
    //divine shield
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 29382;
    ai->Spell[3].Cooldown = -1;
    ai->Spell[3].First_Cast = -20;
    ai->Spell[3].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_gallant
######*/

CreatureAI* GetAI_mob_crimson_gallant(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //devotion aura
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 17232;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //crusader strike
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 14518;
    ai->Spell[1].Cooldown = 10000;
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //holy strike
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 17284;
    ai->Spell[2].Cooldown = 5000;
    ai->Spell[2].First_Cast = 8000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_guardsman
######*/

CreatureAI* GetAI_mob_crimson_guardsman(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //shield charge
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 15749;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //shield bash
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 33871;
    ai->Spell[1].Cooldown = 8000;
    ai->Spell[1].First_Cast = 4000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //disarm
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 11879;
    ai->Spell[2].Cooldown = 15000;
    ai->Spell[2].First_Cast = 6000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //knockaway
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 10101;
    ai->Spell[3].Cooldown = 18000;
    ai->Spell[3].First_Cast = 8000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_initiate
######*/

CreatureAI* GetAI_mob_crimson_initiate(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //mind blast
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 17194;
    ai->Spell[0].Cooldown = 10000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //should cast at friendly target below 25% health
    //flash heal
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 17843;
    ai->Spell[1].Cooldown = -1;
    ai->Spell[1].First_Cast = -25;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    //should only cast below 50% health
    //renew
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 10928;
    ai->Spell[2].Cooldown = -1;
    ai->Spell[2].First_Cast = -50;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_inquisitor
######*/

CreatureAI* GetAI_mob_crimson_inquisitor(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //shadow word: pain
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 19776;
    ai->Spell[0].Cooldown = 25000; 
    ai->Spell[0].First_Cast = 6000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //mind flay
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 22919;
    ai->Spell[1].Cooldown = 15000;
    ai->Spell[1].First_Cast = 100;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //manaburn
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 22947;
    ai->Spell[2].Cooldown = 10000;
    ai->Spell[2].First_Cast = 10000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_monk
######*/

CreatureAI* GetAI_mob_crimson_monk(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //snap kick
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 24671;
    ai->Spell[0].Cooldown = 10000; 
    ai->Spell[0].First_Cast = 4000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //trash
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 8876;
    ai->Spell[1].Cooldown = -1;
    ai->Spell[1].First_Cast = 100;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_priest
######*/

CreatureAI* GetAI_mob_crimson_priest(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //holy smite
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 25054;
    ai->Spell[0].Cooldown = 5000; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //holy fire
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 18165;
    ai->Spell[1].Cooldown = 15000;
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //should heal friendly target, not self
    //holy light
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 15493;
    ai->Spell[2].Cooldown = 20000;
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_rifleman
######*/

CreatureAI* GetAI_mob_crimson_rifleman(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //shoot
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 15620;
    ai->Spell[0].Cooldown = 5000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crimson_sorcerer
######*/

CreatureAI* GetAI_mob_crimson_sorcerer(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //arcane bolt
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 15254;
    ai->Spell[0].Cooldown = 6000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //arcane explosion
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 22271;
    ai->Spell[1].Cooldown = 15000;
    ai->Spell[1].First_Cast = 4000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    //polymorph
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 15534;
    ai->Spell[2].Cooldown = 25000;
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_SECOND_AGGRO;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crypt_beast
######*/

CreatureAI* GetAI_mob_crypt_beast(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //encasing webs
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 4962;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //virulent poison
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 12251;
    ai->Spell[1].Cooldown = 12000;
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //deafening screech
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 3589;
    ai->Spell[2].Cooldown = 30000;
    ai->Spell[2].First_Cast = 12000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_crypt_crawler
######*/

CreatureAI* GetAI_mob_crypt_crawler(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //enveloping web
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 4962;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //deafening screech
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 12251;
    ai->Spell[1].Cooldown = 45000;
    ai->Spell[1].First_Cast = 15000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_eye_of_naxxramas
######*/

#define SAY_LIVING        "The living are here!"

CreatureAI* GetAI_mob_eye_of_naxxramas(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //say
    ai->Aggro_Yell[0] = SAY_LIVING;
    
    //FlyIn sound
    ai->Aggro_Sound[0] = 6596;
        
    //summon rockwing gargoyle
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16381;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 8000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    //super invis
    //ai->m_creature->CastSpell(m_creature,8149,true);
    
    return ai;
}

/*######
## mob_fleshflayer_ghoul
######*/

CreatureAI* GetAI_mob_fleshflayer_ghoul(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //rend
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 18200;
    ai->Spell[0].Cooldown = 20000; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //dazed
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 13496;
    ai->Spell[1].Cooldown = 20000; 
    ai->Spell[1].First_Cast = 8000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //head crack
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 9791;
    ai->Spell[2].Cooldown = 5000; 
    ai->Spell[2].First_Cast = 5000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //ghoul plague
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 16458;
    ai->Spell[3].Cooldown = -1; 
    ai->Spell[3].First_Cast = 40000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_ghoul_ravener
######*/

CreatureAI* GetAI_mob_ghoul_ravener(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //rend
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 18200;
    ai->Spell[0].Cooldown = 20000; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //ravenous claw
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 15608;
    ai->Spell[1].Cooldown = 8000; 
    ai->Spell[1].First_Cast = 7000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //head crack
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 9791;
    ai->Spell[2].Cooldown = 5000; 
    ai->Spell[2].First_Cast = 5000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //ghoul plague
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 16458;
    ai->Spell[3].Cooldown = -1; 
    ai->Spell[3].First_Cast = 40000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_mangled_cadaver
######*/

CreatureAI* GetAI_mob_mangled_cadaver(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //dazed
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 13496;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //cadaver worms
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 16458;
    ai->Spell[1].Cooldown = -1; 
    ai->Spell[1].First_Cast = 30000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_mindless_skeleton
######*/

CreatureAI* GetAI_mob_mindless_skeleton(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //dazed
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 13496;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_patchwork_horror
######*/

CreatureAI* GetAI_mob_patchwork_horror(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //disease cloud
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 27793;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;
    
    //knock away
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 10101;
    ai->Spell[1].Cooldown = 15000; 
    ai->Spell[1].First_Cast = 7000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    //dazed
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 13496;
    ai->Spell[2].Cooldown = 15000; 
    ai->Spell[2].First_Cast = 3000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_plague_ghoul
######*/

CreatureAI* GetAI_mob_plague_ghoul(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //rend
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 18200;
    ai->Spell[0].Cooldown = 20000; 
    ai->Spell[0].First_Cast = 3000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //cleave
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 15584;
    ai->Spell[1].Cooldown = 8000; 
    ai->Spell[1].First_Cast = 7000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //head crack
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 9791;
    ai->Spell[2].Cooldown = 5000; 
    ai->Spell[2].First_Cast = 5000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;
    
    //ghoul plague
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 16458;
    ai->Spell[3].Cooldown = -1; 
    ai->Spell[3].First_Cast = 40000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_ravaged_cadaver
######*/

CreatureAI* GetAI_mob_ravaged_cadaver(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //summon broken cadaver
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16324;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //strike
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 11976;
    ai->Spell[1].Cooldown = 7000; 
    ai->Spell[1].First_Cast = 2000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //dazed
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 13496;
    ai->Spell[2].Cooldown = 15000; 
    ai->Spell[2].First_Cast = 3000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //cadaver worms
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 16458;
    ai->Spell[3].Cooldown = -1; 
    ai->Spell[3].First_Cast = 30000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_rockwing_gargoyle
######*/

CreatureAI* GetAI_mob_rockwing_gargoyle(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //FlyIn
    ai->Aggro_Sound[0] = 6596;

    //gargoyle strike
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16997;
    ai->Spell[0].Cooldown = 7000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //deafening screech
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 3589;
    ai->Spell[1].Cooldown = 30000; 
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_rockwing_screecher
######*/

CreatureAI* GetAI_mob_rockwing_screecher(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //FlyIn
    ai->Aggro_Sound[0] = 6596;

    //gargoyle strike
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16997;
    ai->Spell[0].Cooldown = 6000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //deafening screech
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 3589;
    ai->Spell[1].Cooldown = 30000; 
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //terrifying howl
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 30752;
    ai->Spell[2].Cooldown = 35000; 
    ai->Spell[2].First_Cast = 9000;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_shrieking_banshee
######*/

CreatureAI* GetAI_mob_shrieking_banshee(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //banshee wail
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 16868;
    ai->Spell[0].Cooldown = 6000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //deafening screech
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 3589;
    ai->Spell[1].Cooldown = 30000; 
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_skeletal_berserker
######*/

CreatureAI* GetAI_mob_skeletal_berserker(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //strike
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 11976;
    ai->Spell[0].Cooldown = 6000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //cleave
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 15584;
    ai->Spell[1].Cooldown = 8000; 
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_SECOND_AGGRO;

    //demoralizing shout
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 19778;
    ai->Spell[2].Cooldown = -1; 
    ai->Spell[2].First_Cast = 9000;
    ai->Spell[2].Cast_Target_Type = CAST_SELF;

    //hamstring
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 26141;
    ai->Spell[3].Cooldown = 20000; 
    ai->Spell[3].First_Cast = 9000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_skeletal_guardian
######*/

CreatureAI* GetAI_mob_skeletal_guardian(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //demon armor
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 13787;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //arcane bolt
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 15254;
    ai->Spell[1].Cooldown = 7000; 
    ai->Spell[1].First_Cast = 7000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //arcane missiles
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 22272;
    ai->Spell[2].Cooldown = -1; 
    ai->Spell[2].First_Cast = 1000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //arcane explosion
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 13745;
    ai->Spell[3].Cooldown = 20000; 
    ai->Spell[3].First_Cast = 10000;
    ai->Spell[3].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_spiteful_phantom
######*/

CreatureAI* GetAI_mob_spiteful_phantom(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //chilling touch
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 12531;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //haunting phantoms
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 15254;
    ai->Spell[1].Cooldown = 40000; 
    ai->Spell[1].First_Cast = 10000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_RANDOM;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_thuzadin_acolyte
######*/

CreatureAI* GetAI_mob_thuzadin_acolyte(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //dazed
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 13496;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_thuzadin_necromancer
######*/

CreatureAI* GetAI_mob_thuzadin_necromancer(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //bone armor
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 11445;
    ai->Spell[0].Cooldown = -1; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //summon skeleton
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 17204;
    ai->Spell[1].Cooldown = 60000; 
    ai->Spell[1].First_Cast = 1000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    //soul tap
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 16430;
    ai->Spell[2].Cooldown = 20000; 
    ai->Spell[2].First_Cast = 10000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_thuzadin_shadowcaster
######*/

CreatureAI* GetAI_mob_thuzadin_shadowcaster(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //cripple
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 11443;
    ai->Spell[0].Cooldown = 25000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //shadow bolt
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 22336;
    ai->Spell[1].Cooldown = 8000; 
    ai->Spell[1].First_Cast = 3000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //piercing shadow
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 16429;
    ai->Spell[2].Cooldown = 45000; 
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_undead_scarab
######*/

CreatureAI* GetAI_mob_undead_scarab(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //poison
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 11443;
    ai->Spell[0].Cooldown = 20000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_vengeful_phantom
######*/

CreatureAI* GetAI_mob_vengeful_phantom(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //frostshock
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 15089;
    ai->Spell[0].Cooldown = 10000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_venom_belcher
######*/

CreatureAI* GetAI_mob_venom_belcher(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //venom spit
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 24011;
    ai->Spell[0].Cooldown = 15000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //slowing poison
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 7992;
    ai->Spell[1].Cooldown = 30000; 
    ai->Spell[1].First_Cast = 5000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_wailing_banshee
######*/

CreatureAI* GetAI_mob_wailing_banshee(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //anti magic shield
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 19645;
    ai->Spell[0].Cooldown = 30000; 
    ai->Spell[0].First_Cast = 100;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //banshee curse
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 16867;
    ai->Spell[1].Cooldown = 20000; 
    ai->Spell[1].First_Cast = 3000;
    ai->Spell[1].Cast_Target_Type = CAST_SELF;

    //soul tap
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 16430;
    ai->Spell[2].Cooldown = 40000; 
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //deafening screech
    ai->Spell[3].Enabled = true;
    ai->Spell[3].Spell_Id = 3589;
    ai->Spell[3].Cooldown = 40000; 
    ai->Spell[3].First_Cast = 15000;
    ai->Spell[3].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_wrath_phantom
######*/

CreatureAI* GetAI_mob_wrath_phantom(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //screams of the past
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 7074;
    ai->Spell[0].Cooldown = 10000; 
    ai->Spell[0].First_Cast = 1000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    //haunting phantoms
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 16336;
    ai->Spell[1].Cooldown = -1; 
    ai->Spell[1].First_Cast = 20000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## 
######*/

void AddSC_mobs_stratholme()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_freed_soul";
    newscript->GetAI = GetAI_mob_freed_soul;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_restless_soul";
    newscript->GetAI = GetAI_mob_restless_soul;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_spectral_ghostly_citizen";
    newscript->GetAI = GetAI_mobs_spectral_ghostly_citizen;
    newscript->pReceiveEmote = &ReciveEmote_mobs_spectral_ghostly_citizen;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_bile_slime";
    newscript->GetAI = GetAI_mob_bile_slime;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_bile_spewer";
    newscript->GetAI = GetAI_mob_bile_spewer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_black_guard_sentry";
    newscript->GetAI = GetAI_mob_black_guard_sentry;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_broken_cadaver";
    newscript->GetAI = GetAI_mob_broken_cadaver;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_battle_mage";
    newscript->GetAI = GetAI_mob_crimson_battle_mage;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_conjuror";
    newscript->GetAI = GetAI_mob_crimson_conjuror;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_defender";
    newscript->GetAI = GetAI_mob_crimson_defender;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_gallant";
    newscript->GetAI = GetAI_mob_crimson_gallant;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_guardsman";
    newscript->GetAI = GetAI_mob_crimson_guardsman;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_initiate";
    newscript->GetAI = GetAI_mob_crimson_initiate;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_inquisitor";
    newscript->GetAI = GetAI_mob_crimson_inquisitor;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_monk";
    newscript->GetAI = GetAI_mob_crimson_monk;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_priest";
    newscript->GetAI = GetAI_mob_crimson_priest;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_rifleman";
    newscript->GetAI = GetAI_mob_crimson_rifleman;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crimson_sorcerer";
    newscript->GetAI = GetAI_mob_crimson_sorcerer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crypt_beast";
    newscript->GetAI = GetAI_mob_crypt_beast;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_crypt_crawler";
    newscript->GetAI = GetAI_mob_crypt_crawler;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_eye_of_naxxramas";
    newscript->GetAI = GetAI_mob_eye_of_naxxramas;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_fleshflayer_ghoul";
    newscript->GetAI = GetAI_mob_fleshflayer_ghoul;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_ghoul_ravener";
    newscript->GetAI = GetAI_mob_ghoul_ravener;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_mangled_cadaver";
    newscript->GetAI = GetAI_mob_mangled_cadaver;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_mindless_skeleton";
    newscript->GetAI = GetAI_mob_mindless_skeleton;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_patchwork_horror";
    newscript->GetAI = GetAI_mob_patchwork_horror;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_plague_ghoul";
    newscript->GetAI = GetAI_mob_plague_ghoul;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_ravaged_cadaver";
    newscript->GetAI = GetAI_mob_ravaged_cadaver;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_rockwing_gargoyle";
    newscript->GetAI = GetAI_mob_rockwing_gargoyle;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_rockwing_screecher";
    newscript->GetAI = GetAI_mob_rockwing_screecher;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_shrieking_banshee";
    newscript->GetAI = GetAI_mob_shrieking_banshee;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_skeletal_berserker";
    newscript->GetAI = GetAI_mob_skeletal_berserker;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_skeletal_guardian";
    newscript->GetAI = GetAI_mob_skeletal_guardian;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_spiteful_phantom";
    newscript->GetAI = GetAI_mob_spiteful_phantom;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_thuzadin_acolyte";
    newscript->GetAI = GetAI_mob_thuzadin_acolyte;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_thuzadin_necromancer";
    newscript->GetAI = GetAI_mob_thuzadin_necromancer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_thuzadin_shadowcaster";
    newscript->GetAI = GetAI_mob_thuzadin_shadowcaster;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_undead_scarab";
    newscript->GetAI = GetAI_mob_undead_scarab;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_vengeful_phantom";
    newscript->GetAI = GetAI_mob_vengeful_phantom;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_venom_belcher";
    newscript->GetAI = GetAI_mob_venom_belcher;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_wailing_banshee";
    newscript->GetAI = GetAI_mob_wailing_banshee;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_wrath_phantom";
    newscript->GetAI = GetAI_mob_wrath_phantom;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: Mobs_Nagrand
SD%Complete: 90
SDComment: Quest support: 9849, 9918, 9935, 9935, 9874. Gurok, mini outdoor boss.
SDCategory: Nagrand
EndScriptData */

#include "sc_creature.h"
#include "../../creature/simple_ai.h"
#include "sc_gossip.h"

/*######
## mob_gurok_the_usurper
######*/

CreatureAI* GetAI_mob_gurok_the_usurper(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //knockback
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 24199;
    ai->Spell[0].Cooldown = 18000;
    ai->Spell[0].First_Cast = 12000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //earth shock
    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = 24685;
    ai->Spell[1].Cooldown = 12000;
    ai->Spell[1].First_Cast = 8000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

    //entangling roots
    ai->Spell[2].Enabled = true;
    ai->Spell[2].Spell_Id = 20699;
    ai->Spell[2].Cooldown = 25000;
    ai->Spell[2].First_Cast = 20000;
    ai->Spell[2].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_shattered_rumbler
######*/

struct MANGOS_DLL_DECL mob_shattered_rumblerAI : public ScriptedAI
{
    bool Spawn;

    mob_shattered_rumblerAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
        Spawn = false;
    }

    void Aggro(Unit* who)
    {
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if(Spellkind->Id == 32001 && !Spawn)
        {
            float x = m_creature->GetPositionX();
            float y = m_creature->GetPositionY();
            float z = m_creature->GetPositionZ();

            Hitter->SummonCreature(18181,x+(0.7 * (rand()%30)),y+(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            Hitter->SummonCreature(18181,x+(rand()%5),y-(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            Hitter->SummonCreature(18181,x-(rand()%5),y+(0.5 *(rand()%60)),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
            m_creature->setDeathState(CORPSE);
            Spawn = true;
        }
        return;
    }
};
CreatureAI* GetAI_mob_shattered_rumbler(Creature *_Creature)
{
    return new mob_shattered_rumblerAI (_Creature);
}

/*######
## mobs_kilsorrow_agent
######*/

struct MANGOS_DLL_DECL mobs_kilsorrow_agentAI : public ScriptedAI
{
    mobs_kilsorrow_agentAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(21276, m_creature->GetGUID());
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
                DoStartAttackAndMovement(who);
            }
        }
    }
};
CreatureAI* GetAI_mobs_kilsorrow_agent(Creature *_Creature)
{
    return new mobs_kilsorrow_agentAI (_Creature);
}

/*######
## mob_lump
######*/

/*
UPDATE `creature_template` SET `ScriptName` = 'mob_lump' WHERE `entry` = 18351;
*/
#define SPELL_VISUAL_SLEEP  16093
#define SPELL_SPEAR_THROW   32248

#define LUMP_SAY0 "In Nagrand, food hunt ogre!"
#define LUMP_SAY1 "You taste good with maybe a little salt and pepper."

#define LUMP_DEFEAT "OK, OK! Lump give up!"

struct MANGOS_DLL_DECL mob_lumpAI : public ScriptedAI
{
    mob_lumpAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Reset_Timer;
    uint32 Spear_Throw_Timer;
    bool bReset;
    
    void Reset()
    {
        Reset_Timer = 60000;
        Spear_Throw_Timer = 2000;
        bReset = false;

        m_creature->LoadCreaturesAddon();                   //reset to all default values. proper way?
        m_creature->setFaction(1711);                       //hostile
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit *done_by, uint32 & damage)
    {
        if (done_by->GetTypeId() == TYPEID_PLAYER && (m_creature->GetHealth() - damage)*100 / m_creature->GetMaxHealth() < 30)
        {
            if (!bReset && ((Player*)done_by)->GetQuestStatus(9918) == QUEST_STATUS_INCOMPLETE)
            {
                //Take 0 damage
                damage = 0;

                ((Player*)done_by)->AttackStop();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                //m_creature->RemoveAllAuras();
                //m_creature->DeleteThreatList();
                //m_creature->CombatStop();
                m_creature->setFaction(1080);               //friendly
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_SIT);
                m_creature->Say(LUMP_DEFEAT, LANG_UNIVERSAL, 0);

                bReset = true;
            }
        }
    }

    void Aggro(Unit *who)
    {
                if (m_creature->HasAura(SPELL_VISUAL_SLEEP,0))
                    m_creature->RemoveAura(SPELL_VISUAL_SLEEP,0);

                if (!m_creature->IsStandState())
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_NONE);

                switch(rand()%2)
                {
                case 0:
                    DoSay(LUMP_SAY0,LANG_UNIVERSAL,NULL);
                    break;
                case 1:
                    DoSay(LUMP_SAY1,LANG_UNIVERSAL,NULL);
                    break;
                }
    }

    void UpdateAI(const uint32 diff)
    {
        //check if we waiting for a reset
        if (bReset)
        {
            if (Reset_Timer < diff) { EnterEvadeMode(); }
            else Reset_Timer -= diff;
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Spear_Throw_Timer
        if (Spear_Throw_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SPEAR_THROW);

            Spear_Throw_Timer = 20000;
        }else Spear_Throw_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lump(Creature *_creature)
{
    return new mob_lumpAI(_creature);
}

bool GossipHello_mob_lump(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(9918) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "I need answers, ogre!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->PlayerTalkClass->SendGossipMenu(9352, _Creature->GetGUID());

    return true;
}

bool GossipSelect_mob_lump(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "Why are Boulderfist out this far? You know that this is Kurenai territory.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(9353, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "And you think you can just eat anything you want? You're obviously trying to eat the Broken of Telaar.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(9354, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "This means war, Lump! War I say!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(9355, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->SEND_GOSSIP_MENU(9356, _Creature->GetGUID());
            player->TalkedToCreature(18354, _Creature->GetGUID());
            break;
    }
    return true;
}

/*####
# mob_sunspring_villager
####*/

struct MANGOS_DLL_DECL mob_sunspring_villagerAI : public ScriptedAI
{
    mob_sunspring_villagerAI(Creature *c) : ScriptedAI(c) {Reset();}
		
    void Reset()
    {
		m_creature->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 32);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,7); // lay down

    }

    void Aggro(Unit *who)
    {
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(spell->Id == 32146)
		{
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			m_creature->RemoveCorpse();
		}
    }
};
CreatureAI* GetAI_mob_sunspring_villager(Creature *_Creature)
{
    return new mob_sunspring_villagerAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_nagrand()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_gurok_the_usurper";
    newscript->GetAI = GetAI_mob_gurok_the_usurper;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_shattered_rumbler";
    newscript->GetAI = GetAI_mob_shattered_rumbler;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_kilsorrow_agent";
    newscript->GetAI = GetAI_mobs_kilsorrow_agent;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_lump";
    newscript->GetAI = GetAI_mob_lump;
    newscript->pGossipHello =  &GossipHello_mob_lump;
    newscript->pGossipSelect = &GossipSelect_mob_lump;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_sunspring_villager";
    newscript->GetAI = GetAI_mob_sunspring_villager;
    m_scripts[nrscripts++] = newscript;
}

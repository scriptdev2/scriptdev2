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
SDName: Mobs_Nagrand
SD%Complete: 100
SDComment: Nagrand mobs
EndScriptData */

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/QuestDef.h"
#include "../../../../../game/GossipDef.h"

/*######
## mob_gurok_the_usurper
######*/

CreatureAI* GetAI_mob_gurok_the_usurper(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    //knockback
    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = 38576;
    ai->Spell[0].Cooldown = 20000; 
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_SELF;

    ai->EnterEvadeMode();

    return ai;
}

/*######
## mob_shattered_rumbler
######*/

struct MANGOS_DLL_DECL mob_shattered_rumblerAI : public ScriptedAI
{
    bool Spawn;

    mob_shattered_rumblerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        Spawn = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
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
    mobs_kilsorrow_agentAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
        {
            //check quest status
            if( ((Player*)Killer)->GetTeam() == ALLIANCE && ((Player*)Killer)->GetQuestStatus(9936) == QUEST_STATUS_INCOMPLETE && !((Player*)Killer)->GetReqKillOrCastCurrentCount(9936, m_creature->GetEntry()) )
            {
                ((Player*)Killer)->KilledMonster(21276, m_creature->GetGUID());
            }
            if( ((Player*)Killer)->GetTeam() == HORDE && ((Player*)Killer)->GetQuestStatus(9935) == QUEST_STATUS_INCOMPLETE && !((Player*)Killer)->GetReqKillOrCastCurrentCount(9935, m_creature->GetEntry()) )
            {
                ((Player*)Killer)->KilledMonster(21276, m_creature->GetGUID());
            }
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
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
#define LUMP_SAY0 "In Nagrand, food hunt ogre!"
#define LUMP_SAY1 "You taste good with maybe a little salt and pepper."

#define LUMP_DEFEAT "OK, OK! Lump give up!"

struct MANGOS_DLL_DECL mob_lumpAI : public ScriptedAI
{
    mob_lumpAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    static const uint32 friendlyFaction = 1080;             //not needed here, just to make it easy to change
    static const uint32 hostileFaction = 1711;              //values can be directly in functions
    static const uint32 resTimer = 180000;                  //
    uint32 Reset_Timer;
    bool InCombat;
    bool Reset;
    
    void EnterEvadeMode()
    {
        Reset_Timer = 0;
        InCombat = false;
        Reset = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->LoadCreaturesAddon();                   //reset to all default values. proper way?
        m_creature->setFaction(hostileFaction);             //
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit *done_by, uint32 & damage)
    {
        if (done_by->GetTypeId() == TYPEID_PLAYER && (m_creature->GetHealth() - damage)*100 / m_creature->GetMaxHealth() < 30)
        {
            if (!Reset && ((Player*)done_by)->GetQuestStatus(9918) == QUEST_STATUS_INCOMPLETE)
            {
                //Take 0 damage
                damage = 0;

                ((Player*)done_by)->AttackStop();
                m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop();
                m_creature->setFaction(friendlyFaction);
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_SIT);
                m_creature->Say(LUMP_DEFEAT, LANG_UNIVERSAL, NULL);

                Reset = true;
                Reset_Timer = resTimer;
            }
        }
        AttackedBy(done_by);// ?
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                if (m_creature->GetUInt32Value(UNIT_FIELD_AURA) == 16093)
                    m_creature->SetUInt32Value(UNIT_FIELD_AURA, 0);

                if (!m_creature->IsStandState())
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, PLAYER_STATE_NONE);

                switch(rand()%2)
                {
                case 0:
                    DoSay(LUMP_SAY0,LANG_UNIVERSAL,NULL);
                case 1:
                    DoSay(LUMP_SAY1,LANG_UNIVERSAL,NULL);
                }
                InCombat = true;
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //check if we waiting for a reset
        if (Reset && Reset_Timer < diff) { EnterEvadeMode(); }
        else Reset_Timer -= diff;

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady())
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }
    }
};

CreatureAI* GetAI_mob_lump(Creature *_creature)
{
    return new mob_lumpAI(_creature);
}

bool GossipHello_mob_lump(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(9918) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "I need answers", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->PlayerTalkClass->SendGossipMenu(9352, _Creature->GetGUID());

    return true;
}

bool GossipSelect_mob_lump(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            player->ADD_GOSSIP_ITEM( 0, "Why did you attack me?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(9353, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "You'd eat anything, i bet.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->SEND_GOSSIP_MENU(9354, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->ADD_GOSSIP_ITEM( 0, "Yeah right...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(9355, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            player->ADD_GOSSIP_ITEM( 0, "Behave Lump!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->SEND_GOSSIP_MENU(9356, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            player->PlayerTalkClass->CloseGossip();
            player->KilledMonster(18354, _Creature->GetGUID());
            break;
    }
    return true;
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
}

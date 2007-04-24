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

#include "../sc_defines.h"

struct MANGOS_DLL_DECL testAI : public ScriptedAI
{
    testAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Timer = 90000;

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS,65);
        m_creature->setFaction(35);
    }

    void KilledUnit(Unit* Victim)
    {
    }

    void JustDied(Unit* Killer)
    {
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            Timer-=diff;
        }
    }
};
CreatureAI* GetAI_test(Creature *_Creature)
{
    return new testAI (_Creature);
}

bool GossipHello_test(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, "Test Say Function", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM(1, "Test Yell Function", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM(2, "Test TextEmote Function", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->ADD_GOSSIP_ITEM(3, "Test Wisper Function", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM(4, "???", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->SEND_GOSSIP_MENU(3543,_Creature->GetGUID());
    player->PlayerTalkClass->SendTalking("Test NPC", "Hello, I'm the test NPC for ScriptDev2. Please select an option.");

    return true;
}

bool GossipSelect_test(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
        player->CLOSE_GOSSIP_MENU();
        ((testAI&)_Creature->AI()).m_creature->Say("Bla bla bla, say command.$N, $C", LANG_UNIVERSAL, player->GetGUID());
        ((testAI&)_Creature->AI()).DoSay("Bla bla bla, say command.$N, $C", LANG_UNIVERSAL, player);
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        player->CLOSE_GOSSIP_MENU();
        ((testAI&)_Creature->AI()).DoYell("Bla bla bla, yell command. $N, $C", LANG_UNIVERSAL, player);
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 3)
    {
        player->CLOSE_GOSSIP_MENU();
        ((testAI&)_Creature->AI()).DoTextEmote("Bla bla bla, emote command. $N, $C", player);
    }

    if (action == GOSSIP_ACTION_INFO_DEF + 4)
    {
        player->CLOSE_GOSSIP_MENU();
        ((testAI&)_Creature->AI()).m_creature->Whisper(player->GetGUID(),"Bla bla bla, wisper command. $N, $C");
    }

    return true;
}

void AddSC_test()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="test";
    newscript->pGossipHello          = &GossipHello_test;
    newscript->pGossipSelect         = &GossipSelect_test;
    newscript->GetAI = GetAI_test;
    m_scripts[nrscripts++] = newscript;
}
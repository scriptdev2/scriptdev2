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

#include "hyjalAI.h"

hyjalAI::hyjalAI(Creature *c) : ScriptedAI(c)
{
    pInstance = ((ScriptedInstance*)c->GetInstanceData());
    SetVariables();
}
void hyjalAI::SetVariables()
{
    PlayerGUID = 0;
    BossGUID[0] = 0;
    BossGUID[1] = 0;

    NextWaveTimer = 10000;
    WaveCount = 0;
    CheckTimer = 0;
    Faction = 0;

    EventBegun = false;
    InCombat = false;
    FirstBossDead = false;
    SecondBossDead = false;

    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 1);
}

void hyjalAI::EnterEvadeMode()
{
    SetVariables();

    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    DoGoHome();
}

void hyjalAI::AttackStart(Unit *who)
{        
    if(!who)
        return;

    if(who->isTargetableForAttack() && who!= m_creature)
    {
        DoStartMeleeAttack(who);

        if(!InCombat)
        {
            InCombat = true;
        }
    }

}

void hyjalAI::MoveInLineOfSight(Unit *who)
{
    if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
    {
        float attackRadius = m_creature->GetAttackDistance(who);
        if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
        {
            if(who->HasStealthAura())
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

            if(who && who->isAlive())
                m_creature->AddThreat(who, 1.0f);

            if(!InCombat)
            {
                InCombat = true;
            }
        }
    }
}

void hyjalAI::SetFaction(uint32 faction)
{
    Faction = faction;
}

void hyjalAI::SummonCreature(uint32 entry, bool IsBoss)
{
    uint32 random = rand()%4;
    float X = 0;
    float Y = 0;
    float Z = 0;
    if(Faction == 0) // Alliance
    {
        X = AllianceBase[random].x;
        Y = AllianceBase[random].y;
        Z = AllianceBase[random].z;
    }
    else if(Faction == 1) // Horde
    {
        X = HordeBase[random].x;
        Y = HordeBase[random].y;
        Z = HordeBase[random].z;
    }

    Creature* pCreature = m_creature->SummonCreature(entry, X, Y, Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
    if(pCreature)
    {
        if(PlayerGUID)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), PlayerGUID);
            if(pUnit)
                pCreature->AddThreat(pUnit, 10.0f);
        }

        pCreature->AddThreat(m_creature, 1.0f);
        if(IsBoss)
        {
            if(!FirstBossDead)
                BossGUID[0] = pCreature->GetGUID();
            else BossGUID[1] = pCreature->GetGUID();
        }
    }
}


void hyjalAI::SummonNextWave(Wave wave[18], uint32 Count, uint32 faction)
{
    if(wave[Count].IsBoss)
        SummonCreature(wave[Count].Mob1, true);

    if(wave[Count].Mob1 && (!wave[Count].IsBoss))
        SummonCreature(wave[Count].Mob1);
     if(wave[Count].Mob2)
         SummonCreature(wave[Count].Mob2);
     if(wave[Count].Mob3)
         SummonCreature(wave[Count].Mob3);
     if(wave[Count].Mob4)
         SummonCreature(wave[Count].Mob4);
     if(wave[Count].Mob5)
         SummonCreature(wave[Count].Mob5);
     if(wave[Count].Mob6)
         SummonCreature(wave[Count].Mob6);
     if(wave[Count].Mob7)
         SummonCreature(wave[Count].Mob7);
     if(wave[Count].Mob8)
         SummonCreature(wave[Count].Mob8);
     if(wave[Count].Mob9)
         SummonCreature(wave[Count].Mob9);
     if(wave[Count].Mob10)
         SummonCreature(wave[Count].Mob10);
     if(wave[Count].Mob11)
         SummonCreature(wave[Count].Mob11);
     if(wave[Count].Mob12)
         SummonCreature(wave[Count].Mob12);
     if(wave[Count].Mob13)
         SummonCreature(wave[Count].Mob13);
     if(wave[Count].Mob14)
         SummonCreature(wave[Count].Mob14);
     if(wave[Count].Mob15)
         SummonCreature(wave[Count].Mob15);
     if(wave[Count].Mob16)
         SummonCreature(wave[Count].Mob16);
     if(wave[Count].Mob17)
         SummonCreature(wave[Count].Mob17);
     if(wave[Count].Mob18)
         SummonCreature(wave[Count].Mob18);

     if(!wave[Count].IsBoss)
         NextWaveTimer = wave[Count].WaveTimer;
     else NextWaveTimer = 0;
}

void hyjalAI::TeleportRaid(Player* player, float X, float Y, float Z)
{
    if(!player)
    {
        if(PlayerGUID)
        {
            player = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
            if(!player) return;
        }else return;
    }
    
    Group* Raid = player->GetGroup();
    if(Raid)
    {
        const Group::MemberSlotList list = Raid->GetMemberSlots();
        Group::member_citerator itr = list.begin();
        for( ; itr != list.end(); itr++)
        {
            Player* member = ((Player*)Unit::GetUnit((*m_creature), itr->guid));
            if(member && member->isAlive() && (member->GetMapId() == m_creature->GetMapId()))
            {
                member->CastSpell(member, SPELL_TELEPORT_VISUAL, true);
                member->TeleportTo(m_creature->GetMapId(), X, Y, Z, member->GetOrientation());
            }
        }
    }
    else player->TeleportTo(m_creature->GetMapId(), X, Y, Z, player->GetOrientation());
}

void hyjalAI::StartEvent(Player* player)
{
    if(player)
    {
        EventBegun = true;
        NextWaveTimer = 15000;
        PlayerGUID = player->GetGUID();
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
    }
}

void hyjalAI::SetInstanceData(uint32 Event, uint32 data)
{
    if(!Event || !data)
        return;

    if(pInstance)
        pInstance->SetData(Event, data);
}

uint32 hyjalAI::GetInstanceData(uint32 Event)
{
    if(!Event)
        return 0;

    if(pInstance)
       return pInstance->GetData(Event);

    return 0;
}

void hyjalAI::UpdateAI(const uint32 diff)
{
    if(!EventBegun) return;

    if(NextWaveTimer)
    {
        if(NextWaveTimer < diff)
        {
            if(Faction == 0)
                SummonNextWave(AllianceWaves, WaveCount, 1);
            else if(Faction == 1)
                SummonNextWave(HordeWaves, WaveCount, 1);
            WaveCount++;
        }else NextWaveTimer -= diff;
    }

    if(CheckTimer < diff)
    {
        if(NextWaveTimer)
        {
            if(Faction == 0)
                SummonNextWave(AllianceWaves, WaveCount, 1);
            else if(Faction == 1)
                SummonNextWave(HordeWaves, WaveCount, 1);
            WaveCount++;
        }
        if(PlayerGUID)
        {
            Player* target = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
            if(target)
            {
                if(!target->isAlive())
                {
                    Group* Raid = target->GetGroup();
                    if(Raid)
                    {
                        const Group::MemberSlotList list = Raid->GetMemberSlots();
                        Group::member_citerator itr = list.begin();
                        uint32 DeathCount = 0;
                        uint32 RaidCount = 0;
                        for( ; itr != list.end(); itr++)
                        {
                            Player* member = ((Player*)Unit::GetUnit((*m_creature), itr->guid));

                            if(member->GetMapId() != m_creature->GetMapId()) // Make sure they are in the same map.
                                break;

                            if(member && member->isAlive())
                                break;
                            else
                                DeathCount++;

                            RaidCount++;
                        }
                        if(DeathCount == RaidCount)
                        {
                            EventBegun = false;
                            EnterEvadeMode();
                        }
                    }else
                    {
                        EventBegun = false;
                        EnterEvadeMode();
                    }
                }
            }
        }

        for(uint8 i = 0; i < 2; i++)
        {
            if(BossGUID[i])
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), BossGUID[i]);
                if(pUnit && (!pUnit->isAlive()))
                {
                    if(BossGUID[i] == BossGUID[0])
                        FirstBossDead = true;
                    else if(BossGUID[i] == BossGUID[1])
                        SecondBossDead = true;
                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 1);
                    BossGUID[i] = 0;
                }
            }
        }
        CheckTimer = 25000;
    }else CheckTimer -= diff;

    if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) return;

    DoMeleeAttackIfReady();
}

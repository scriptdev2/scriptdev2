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
SDName: HyjalAI
SD%Complete: 100
SDComment: 
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

#include "hyjalAI.h"

hyjalAI::hyjalAI(Creature *c) : ScriptedAI(c)
{
    pInstance = ((ScriptedInstance*)c->GetInstanceData());
    Reset();
}

void hyjalAI::Reset()
{
    /** GUIDs **/
    PlayerGUID = 0;
    BossGUID[0] = 0;
    BossGUID[1] = 0;

    /** Timers **/
    NextWaveTimer = 10000;
    CheckBossTimer = 0;

    /** Misc **/
    WaveCount = 0;

    /** Set faction properly based on creature entry**/
    switch(m_creature->GetEntry())
    {
        case 17772: 
            Faction = 0; 
            DoCast(m_creature, SPELL_BRILLIANCE_AURA, true);
            break;

        case 17852: Faction = 1; break;
    }

    /** Bools **/
    EventBegun = false;
    FirstBossDead = false;
    SecondBossDead = false;
    Summon = false;

    /** Flags **/
    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 1);

    /** Initialize spells **/
    memset(Spell, 0, sizeof(Spell));
}

void hyjalAI::Aggro(Unit *who)
{
    if(Spell[0].Cooldown)
        SpellTimer[0] = Spell[0].Cooldown;
    if(Spell[1].Cooldown)
        SpellTimer[1] = Spell[1].Cooldown;
    if(Spell[2].Cooldown)
        SpellTimer[2] = Spell[2].Cooldown;

    Talk(ATTACKED);
}

void hyjalAI::JustDied(Unit* killer)
{
    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 1);
    Talk(DEATH);
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
    if(!Faction) // Alliance
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

    Creature* pCreature = m_creature->SummonCreature(entry, X, Y, Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 45000);
    if(pCreature)
    {
        EnemyCount++; // Increment Enemy Count to be used in World States
        std::list<Player*> PlayerList = m_creature->GetMap()->GetPlayers();

        if(PlayerList.empty())
            return;

        for(std::list<Player*>::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            pCreature->AddThreat((*itr), 1.0f);
            if((*itr)->GetGUID() == PlayerGUID)
                pCreature->AI()->AttackStart((*itr)); // Attack whoever clicked.
        }
        pCreature->AddThreat(m_creature, 1.0f);

        if(IsBoss)
        {
            if(!FirstBossDead)
                BossGUID[0] = pCreature->GetGUID();
            else BossGUID[1] = pCreature->GetGUID();
            CheckBossTimer = 5000;
        }
    }
}

void hyjalAI::SummonNextWave(Wave wave[18], uint32 Count, uint32 faction)
{
    if(rand()%4 == 0) // 1 in 4 chance we give a rally yell. Not sure if the chance is Blizzlike.
        Talk(RALLY);

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
     {
         //uint32 stateValue = Count+1;
         //if(FirstBossDead)
         //    stateValue -= 9; // Subtract 8 from it to give the proper wave number if we are greater than 8
         //UpdateWorldState(WORLDSTATE_WAVES, stateValue); // Set world state to our current wave number
         //UpdateWorldState(WORLDSTATE_ENEMY, 1);
         //UpdateWorldState(WORLDSTATE_ENEMYCOUNT, EnemyCount);
         NextWaveTimer = wave[Count].WaveTimer;
     }
     else
     {
         //UpdateWorldState(0, WORLDSTATE_WAVES); // Set world state for waves to 0 to disable it.
         //UpdateWorldState(1, WORLDSTATE_ENEMY);
         //UpdateWorldState(0, WORLDSTATE_ENEMYCOUNT); // Set World State for enemies invading to 1.
         //UpdateWorldState(1, WORLDSTATE_ENEMYCOUNT);
         Summon = false;
         CheckBossTimer = 1000;
     }
}

void hyjalAI::TeleportRaid(Player* player, float X, float Y, float Z)
{
    std::list<Player*> PlayerList = m_creature->GetMap()->GetPlayers();
    if(PlayerList.empty())
        return;

    if(Faction == 1) // Horde
    {

    }

    for(std::list<Player*>::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
    {
        (*itr)->CastSpell((*itr), SPELL_TELEPORT_VISUAL, true);
        DoTeleportPlayer((*itr), X,Y,Z,(*itr)->GetOrientation());
    }
}

void hyjalAI::StartEvent(Player* player)
{
    if(player)
    {
        Talk(BEGIN);
        EventBegun = true;
        Summon = true;
        NextWaveTimer = 15000;
        CheckBossTimer = 5000;
        PlayerGUID = player->GetGUID();
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
        //UpdateWorldState(0, WORLDSTATE_WAVES);
        //UpdateWorldState(0, WORLDSTATE_ENEMY);
    }
}

uint32 hyjalAI::GetInstanceData(uint32 Event)
{
    if(!Event)
        return 0;

    if(pInstance)
       return pInstance->GetData(Event);

    return 0;
}
void hyjalAI::Talk(uint32 id)
{
    std::list<uint8> index;
    for(uint8 i = 0; i < 10; i++)
    {
        if(Faction == 0) // Alliance
        {
            if(JainaQuotes[i].id == id)
                index.push_back(i);
        }
        else if(Faction == 1) // Horde
        {
            if(ThrallQuotes[i].id == id)
                index.push_back(i);
        }
    }

    if(index.empty())
        return; // No quotes found, no use to continue

    uint8 ind = *(index.begin()) + rand()%index.size();

    char* Yell = NULL;
    uint32 Sound = 0;
    if(Faction == 0) // Alliance
    {
        Yell = JainaQuotes[ind].text;
        Sound = JainaQuotes[ind].sound;
    }
    else if(Faction == 1) // Horde
    {
        Yell = ThrallQuotes[ind].text;
        Sound = ThrallQuotes[ind].sound;
    }
    if(Yell)
        DoYell(Yell, LANG_UNIVERSAL, NULL);
    if(Sound)
        DoPlaySoundToSet(m_creature, Sound);
}

// Core patch required.
//void hyjalAI::UpdateWorldState(uint32 field, uint32 value)
//{
//    m_creature->GetMap()->UpdateWorldState(field, value);
//}

void hyjalAI::UpdateAI(const uint32 diff)
{
    if(!EventBegun) return;

    if(Summon)
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

    if(CheckBossTimer < diff)
    {
        for(uint8 i = 0; i < 2; i++)
        {
            if(BossGUID[i])
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), BossGUID[i]);
                if(pUnit && (!pUnit->isAlive()))
                {
                    if(BossGUID[i] == BossGUID[0])
                    {
                        Talk(INCOMING);
                        FirstBossDead = true;
                    }
                    else if(BossGUID[i] == BossGUID[1])
                    {
                        Talk(SUCCESS);
                        SecondBossDead = true;
                    }
                    EventBegun = false;
                    CheckBossTimer = 0;
                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, 1);
                    BossGUID[i] = 0;
                    //UpdateWorldState(WORLDSTATE_ENEMYCOUNT, 0);// Reset world state for enemies to disable it 
                }
            }
        }
        CheckBossTimer = 5000;
    }else CheckBossTimer -= diff;

    if(!m_creature->SelectHostilTarget() || !m_creature->getVictim()) return;

    for(uint8 i = 0; i < 3; ++i)
    {
        if(Spell[i].SpellId)
        {
            if(SpellTimer[i] < diff)
            {
                if(m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(false);

                Unit* target = NULL;
                
                switch(Spell[i].TargetType)
                {
                    case TARGETTYPE_SELF:
                        target = m_creature;
                        break;
                    case TARGETTYPE_RANDOM:
                        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        break;
                    case TARGETTYPE_VICTIM:
                        target = m_creature->getVictim();
                        break;
                }

                if(target && target->isAlive())
                {
                    DoCast(target, Spell[i].SpellId);
                    SpellTimer[i] = Spell[i].Cooldown;
                }
            }else SpellTimer[i] -= diff;
        }
    }

    DoMeleeAttackIfReady();
}

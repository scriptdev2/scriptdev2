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
#include "../../../../../../game/Player.h"
#include "../../../../../../game/Group.h"
#include "../../../../../../game/GossipDef.h"

// Trash Mobs summoned in waves
#define NECROMANCER         17899
#define ABOMINATION         17898
#define GHOUL               17895
#define BANSHEE             17905
#define CRYPT_FIEND         17897
#define GARGOYLE            17906
#define FROST_WYRM          17907
#define GIANT_INFERNAL      17908
#define FEL_STALKER         17916

// Bosses summoned after every 8 waves
#define RAGE_WINTERCHILL    17767
#define ANETHERON           17808
#define KAZROGAL            17888
#define AZGALOR             17842
#define ARCHIMONDE          17968

#define SPELL_TELEPORT_VISUAL   41232

struct Wave
{
    uint32 Mob1, Mob2, Mob3, Mob4, Mob5, Mob6, Mob7, Mob8, Mob9, Mob10, Mob11, Mob12, Mob13, Mob14, Mob15, Mob16, Mob17, Mob18;
    uint32 WaveTimer;
    bool IsBoss;
};

static Wave AllianceWaves[]=
{
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, 0, 0, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 1
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 2
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 3
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 4
    {GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 5
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, 0, 0, 0, 0, 0, 120000, false}, // Wave 6
    {GHOUL, GHOUL, GHOUL, GHOUL, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 7
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 120000, false}, // Wave 8
    {RAGE_WINTERCHILL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true}, // All 8 Waves are summoned, summon Rage Winterchill, next few waves are for Anetheron
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, 0, 0, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 1
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 2
    {GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 3
    {GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, BANSHEE, BANSHEE, BANSHEE, BANSHEE, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 4
    {CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, NECROMANCER, NECROMANCER, BANSHEE, BANSHEE, BANSHEE, BANSHEE, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 5
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 6
    {CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, BANSHEE, BANSHEE, BANSHEE, BANSHEE, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 7
    {GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, BANSHEE, BANSHEE, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 180000, false}, // Wave 8
    {ANETHERON, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true} // All 8 Waves are summoned, summon Anatheron
};

static Wave HordeWaves[]=
{
    {GHOUL, GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, BANSHEE, BANSHEE, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 1
    {GHOUL, GHOUL, GHOUL, GHOUL, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, 0, 0, 0, 0, 120000, false}, // Wave 2
    {GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 120000, false}, // Wave 3
    {CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, NECROMANCER, NECROMANCER            , 0, 0, 0, 0, 120000, false}, // Wave 4
    {GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 5
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, NECROMANCER, FROST_WYRM, 0, 0, 120000, false}, // Wave 6
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, NECROMANCER, FROST_WYRM, 0, 0, 120000, false}, // Wave 7
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, BANSHEE, BANSHEE, 120000, false}, // Wave 8
    {KAZROGAL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true}, // All 8 Waves are summoned, summon Kaz'Rogal, next few waves are for Azgalor
    {ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 1
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, FROST_WYRM, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, GARGOYLE, 0, 0, 0, 0, 120000, false}, // Wave 2
    {GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GHOUL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, 0, 0, 0, 0, 120000, false}, // Wave 3
    {GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, FEL_STALKER, FEL_STALKER, FEL_STALKER, FEL_STALKER, FEL_STALKER, FEL_STALKER, 0, 0, 0, 0, 120000, false}, // Wave 4
    {FEL_STALKER, FEL_STALKER, FEL_STALKER, FEL_STALKER, FEL_STALKER, FEL_STALKER, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, 0, 0, 0, 0, 120000, false}, // Wave 5
    {NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, NECROMANCER, BANSHEE, BANSHEE, BANSHEE, BANSHEE, BANSHEE, BANSHEE, 0, 0, 0, 0, 120000, false}, // Wave 6
    {GHOUL, GHOUL, CRYPT_FIEND, CRYPT_FIEND, FEL_STALKER, FEL_STALKER, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, GIANT_INFERNAL, 0, 0, 0, 0, 0, 0, 120000, false}, // Wave 7
    {CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, CRYPT_FIEND, FEL_STALKER, FEL_STALKER, ABOMINATION, ABOMINATION, ABOMINATION, ABOMINATION, BANSHEE, BANSHEE, BANSHEE, BANSHEE, NECROMANCER, NECROMANCER, 0, 0, 180000, false}, // Wave 8
    {AZGALOR, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, true} // All 8 Waves are summoned, summon Azgalor
};

struct Location
{
    float x, y, z;
};

static Location AllianceBase[]=
{
    {4979.010, -1709.134, 1339.674},
    {4969.123, -1705.904, 1341.363},
    {4970.260, -1698.546, 1341.200},
    {4975.262, -1698.239, 1341.427}
};

static Location HordeBase[]=
{
    {5554.399, -2581.419, 1480.820},
    {5538.996, -2577.742, 1479.790},
    {5565.642, -2565.666, 1481.635},
    {5547.218, -2574.589, 1479.194}
};

enum EncounterType
{
    ENCOUNTER_NOT_STARTED       = 0,
    ENCOUNTER_IN_PROGRESS       = 1,
    ENCOUNTER_FAILED            = 2,
    ENCOUNTER_COMPLETE          = 3,
};

struct Yells
{
    uint32 id; // used to determine if it's attack yells, etc.
    char* text;
    uint32 sound;
};

enum YellId
{
    ATTACKED     = 0,
    INCOMING     = 1,
    RALLY        = 2,
    FAILED       = 3,
    SUCCESS      = 4,
    DEATH        = 5,
};

static Yells JainaQuotes[]=
{
    {ATTACKED, "I'm in jeopardy, help me if you can!", 0},
    {ATTACKED, "They've broken through!", 0},
    {INCOMING, "Stay alert! Another wave approaches.", 0},
    {RALLY, "Don't give up! We must prevail!", 0},
    {RALLY, "We must hold strong!", 0},
    {FAILED, "We are lost. Fall back!", 0},
    {SUCCESS, "We have won valuable time. Now we must pull back!", 0},
    {SUCCESS, "We have played our part and done well. It is up to the others now.", 0},
    {DEATH, "I did... my best.", 0},
};

static Yells ThrallQuotes[]=
{ 
    {ATTACKED, "I will lie down for no one!", 0},
    {ATTACKED, "Bring the fight to me and pay with your lives!", 0},
    {INCOMING, "Make ready for another wave! Lok-Tar Ogar!", 0},
    {RALLY, "Hold them back! Do not falter!", 0},
    {RALLY, "Victory or death!", 0},
    {RALLY, "Do not give an inch of ground!", 0},
    {FAILED, "It is over. Withdraw! We have failed.", 0},
};

struct MANGOS_DLL_DECL hyjalAI : public ScriptedAI
{
    hyjalAI(Creature *c);

    void SetVariables();

    void Reset();

    void AttackStart(Unit *who);

    void MoveInLineOfSight(Unit *who);

    void UpdateAI(const uint32 diff);

    void SetFaction(uint32 faction);

    void TeleportRaid(Player *player, float X, float Y, float Z);

    void SummonCreature(uint32 entry, bool IsBoss = false);
    
    void SummonNextWave(Wave wave[18], uint32 Count, uint32 faction);

    void StartEvent(Player* player);
    
    void SetInstanceData(uint32 Event, uint32 data);

    uint32 GetInstanceData(uint32 Event);

    void Talk(uint32 id);
public:    
    ScriptedInstance* pInstance;

    uint64 PlayerGUID;
    uint64 BossGUID[2];

    uint32 NextWaveTimer;
    uint32 WaveCount;
    uint32 CheckTimer;
    uint32 Faction;

    bool EventBegun;
    bool FirstBossDead;
    bool SecondBossDead;

private:
    bool InCombat;
};

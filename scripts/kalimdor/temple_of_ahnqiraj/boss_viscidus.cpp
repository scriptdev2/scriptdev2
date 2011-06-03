/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Viscidus
SD%Complete: 0
SDComment: place holder
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

enum
{
    // Timer spells
    SPELL_POISON_SHOCK          = 25993,
    SPELL_POISONBOLT_VOLLEY     = 25991,
    SPELL_TOXIN                 = 26575,                    // Triggers toxin cloud
    SPELL_TOXIN_CLOUD           = 25989,

    // Debuffs gained by the boss on frost damage
    SPELL_VISCIDUS_SLOWED       = 26034,
    SPELL_VISCIDUS_SLOWED_MORE  = 26036,
    SPELL_VISCIDUS_FREEZE       = 25937,

    // When frost damage exceeds a certain limit, then boss explodes
    SPELL_REJOIN_VISCIDUS       = 25896,
    SPELL_VISCIDUS_EXPLODE      = 25938,                    // Casts a lot of spells in the same time: 25865 to 25884; All spells have target coords
    SPELL_VISCIDUS_SUICIDE      = 26003,

    NPC_GLOB_OF_VISCIDUS        = 15667
};

void AddSC_boss_viscidus()
{
}

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
SDName: mobs_hellfire_peninsula
SD%Complete: 100
SDComment: misc mobs, mostly related to quest
SDCategory: Hellfire Peninsula
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## mobs_bonechewer_orc
######*/

struct MANGOS_DLL_DECL mobs_bonechewer_orcAI : public ScriptedAI
{
    mobs_bonechewer_orcAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(21161, m_creature->GetGUID());
    }
};
CreatureAI* GetAI_mobs_bonechewer_orc(Creature *_Creature)
{
    return new mobs_bonechewer_orcAI (_Creature);
}


/*######
## mobs_shattered_hand_orc
######*/

struct MANGOS_DLL_DECL mobs_shattered_hand_orcAI : public ScriptedAI
{
    mobs_shattered_hand_orcAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(22334, m_creature->GetGUID());
    }
};
CreatureAI* GetAI_mobs_shattered_hand_orc(Creature *_Creature)
{
    return new mobs_shattered_hand_orcAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_hellfire_peninsula()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mobs_bonechewer_orc";
    newscript->GetAI = GetAI_mobs_bonechewer_orc;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_shattered_hand_orc";
    newscript->GetAI = GetAI_mobs_shattered_hand_orc;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: Mobs_Blades_Edge_Mountains
SD%Complete: 100
SDComment: Quest support: 10502, 10503, 10504, 10505
SDCategory: Blade's Edge Mountains
EndScriptData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## mobs_bladespire_ogre
######*/

struct MANGOS_DLL_DECL mobs_bladespire_ogreAI : public ScriptedAI
{
    mobs_bladespire_ogreAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(19995, m_creature->GetGUID());
    }
};
CreatureAI* GetAI_mobs_bladespire_ogre(Creature *_Creature)
{
    return new mobs_bladespire_ogreAI (_Creature);
}

/*######
## mobs_bloodmaul_ogre
######*/

struct MANGOS_DLL_DECL mobs_bloodmaul_ogreAI : public ScriptedAI
{
    mobs_bloodmaul_ogreAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(19991, m_creature->GetGUID());
    }
};
CreatureAI* GetAI_mobs_bloodmaul_ogre(Creature *_Creature)
{
    return new mobs_bloodmaul_ogreAI (_Creature);
}

/*######
## AddSC
######*/

void AddSC_mobs_blades_edge_mountains()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mobs_bladespire_ogre";
    newscript->GetAI = GetAI_mobs_bladespire_ogre;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_bloodmaul_ogre";
    newscript->GetAI = GetAI_mobs_bloodmaul_ogre;
    m_scripts[nrscripts++] = newscript;
}

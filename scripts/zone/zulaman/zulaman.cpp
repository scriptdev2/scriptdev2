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
SDName: Zulaman
SD%Complete: 75
SDComment: Forest Frog will turn into different NPC's. Instance script to keep tack of previously spawned not implemented.
SDCategory: Zul'Aman
EndScriptData */

/* ContentData
npc_forest_frog
EndContentData */

#include "precompiled.h"

/*######
## npc_forest_frog
######*/

struct MANGOS_DLL_DECL npc_forest_frogAI : public ScriptedAI
{
    npc_forest_frogAI(Creature* c) : ScriptedAI(c) { Reset(); }

    void Reset() { }
    void Aggro(Unit *who) { }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if( spell->Id == 43732 && caster->GetTypeId() == TYPEID_PLAYER )
        {
            if( rand()%99 == 50 )                           //increase or decrease chance?
                DoCast(caster,43923,true);
            else
            {
                uint32 cEntry = 0;

                switch(rand()%12)
                {
                    case 0: cEntry = 24024; break;          //Kraz
                    case 1: cEntry = 24397; break;          //Mannuth
                    case 2: cEntry = 24403; break;          //Deez
                    case 3: cEntry = 24404; break;          //Galathryn
                    case 4: cEntry = 24405; break;          //Adarrah
                    case 5: cEntry = 24406; break;          //Fudgerick
                    case 6: cEntry = 24407; break;          //Darwen
                    case 7: cEntry = 24408; break;          //Gunter
                    case 8: cEntry = 24409; break;          //Kyren
                    case 9: cEntry = 24445; break;          //Mitzi
                    case 10: cEntry = 24448; break;         //Christian
                    case 11: cEntry = 24453; break;         //Brennan
                    case 12: cEntry = 24455; break;         //Hollee
                }

                if( cEntry ) m_creature->UpdateEntry(cEntry);
            }
        }
    }
};
CreatureAI* GetAI_npc_forest_frog(Creature *_Creature)
{
    return new npc_forest_frogAI (_Creature);
}

void AddSC_zulaman()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_forest_frog";
    newscript->GetAI = GetAI_npc_forest_frog;
    m_scripts[nrscripts++] = newscript;
}

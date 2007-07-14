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

#define SPELL_PURIFY      29200
#define SPELL_TOXIC       29278
#define SPELL_PURIFIED    29277

bool ItemUse_purification_mixture(Player *player, Item* _Item)
{
    player->CastSpell(player,SPELL_PURIFY,true);

    if(!player->m_currentSpell)
    {
        if (rand()%100 < 65)
        {
            player->CastSpell(player,SPELL_TOXIC,true); //toxic
        }
        else
        {
            player->CastSpell(player,SPELL_PURIFIED,true); //purified
        }
    }
    return true;
}
void AddSC_purification_mixture()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="purification_mixture";
    newscript->pItemUse = ItemUse_purification_mixture;
    m_scripts[nrscripts++] = newscript;
}
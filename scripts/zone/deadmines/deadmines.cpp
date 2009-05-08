/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Deadmines
SD%Complete: 90
SDComment: Contains GO for event at end door
SDCategory: Deadmines
EndScriptData */

#include "precompiled.h"
#include "def_deadmines.h"

bool GOHello_go_door_lever_dm(Player* pPlayer, GameObject* _GO)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)_GO->GetInstanceData();

    if (!pInstance)
        return false;

    GameObject* pGo = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_DEFIAS_DOOR));
    if (pGo && pGo->GetGoState() == 1)
        return false;

    return true;
}

bool GOHello_go_defias_cannon(Player* pPlayer, GameObject* _GO)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)_GO->GetInstanceData();

    if (!pInstance)
        return false;

    if (pInstance->GetData(TYPE_DEFIAS_ENDDOOR) == DONE || pInstance->GetData(TYPE_DEFIAS_ENDDOOR) == IN_PROGRESS)
        return false;

    pInstance->SetData(TYPE_DEFIAS_ENDDOOR, IN_PROGRESS);
    return false;
}

void AddSC_deadmines()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_door_lever_dm";
    newscript->pGOHello = &GOHello_go_door_lever_dm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_defias_cannon";
    newscript->pGOHello = &GOHello_go_defias_cannon;
    newscript->RegisterSelf();
}

/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"

//return closest GO in grid, with range from pSource
GameObject* GetClosestGameObjectWithEntry(WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange)
{
    GameObject* pGo = NULL;

    MaNGOS::NearestGameObjectEntryInObjectRangeCheck go_check(*pSource, uiEntry, fMaxSearchRange);
    MaNGOS::GameObjectLastSearcher<MaNGOS::NearestGameObjectEntryInObjectRangeCheck> searcher(pSource, pGo, go_check);

    Cell::VisitGridObjects(pSource, searcher, fMaxSearchRange);

    return pGo;
}

//return closest creature alive in grid, with range from pSource
Creature* GetClosestCreatureWithEntry(WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange)
{
    Creature* pCreature = NULL;

    MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck creature_check(*pSource, uiEntry, true, fMaxSearchRange);
    MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(pSource, pCreature, creature_check);

    Cell::VisitGridObjects(pSource, searcher, fMaxSearchRange);

    return pCreature;
}

void GetGameObjectListWithEntryInGrid(std::list<GameObject*>& lList , WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange)
{
    AllGameObjectsWithEntryInRange check(pSource, uiEntry, fMaxSearchRange);
    MaNGOS::GameObjectListSearcher<AllGameObjectsWithEntryInRange> searcher(pSource, lList, check);

    Cell::VisitGridObjects(pSource, searcher, fMaxSearchRange);
}

void GetCreatureListWithEntryInGrid(std::list<Creature*>& lList, WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange)
{
    AllCreaturesOfEntryInRange check(pSource, uiEntry, fMaxSearchRange);
    MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange> searcher(pSource, lList, check);

    Cell::VisitGridObjects(pSource, searcher, fMaxSearchRange);
}

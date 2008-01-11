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
SDName: Instance_Molten_Core
SD%Complete: 0
SDComment: Place Holder
EndScriptData */

#include "../../sc_defines.h"

#define ID_LUCIFRON     12118
#define ID_MAGMADAR     11982
#define ID_GEHENNAS     12259
#define ID_GARR         12057
#define ID_GEDDON       12056
#define ID_SHAZZRAH     12264
#define ID_GOLEMAGG     11988
#define ID_SULFURON     12098
#define ID_DOMO         12018
#define ID_RAGNAROS     11502

class MANGOS_DLL_SPEC MoltenCore : public ScriptedInstance
{
public:

    MoltenCore(Map *map) : ScriptedInstance(map) {}
    ~MoltenCore() {}

    uint64 Lucifron, Magmadar, Gehennas, Garr, Geddon, Shazzrah, Sulfuron, Golemagg, Domo, Ragnaros;
    uint64 RuneGUID[8];

    uint32 CheckTimer;

    bool isDead(uint64 guid)
    {
        if (guid)
        {
            /*Unit* pUnit = Unit::GetUnit(??, guid);

            if (pUnit && pUnit->isAlive())
                return false;*/
        }

        return true; 
    }


    //On creation, NOT load.
    void Initialize() 
    {
        //Clear all GUIDs
        Lucifron = 0;
        Magmadar = 0;
        Gehennas = 0;
        Garr = 0;
        Geddon = 0;
        Shazzrah = 0;
        Sulfuron = 0;
        Golemagg = 0;
        Domo = 0;
        Ragnaros = 0;

        RuneGUID[0] = 0;
        RuneGUID[1] = 0;
        RuneGUID[2] = 0;
        RuneGUID[3] = 0;
        RuneGUID[4] = 0;
        RuneGUID[5] = 0;
        RuneGUID[6] = 0;
        RuneGUID[7] = 0;

        CheckTimer = 10000;
    }
    //Called every map update
    void Update(uint32 diff) 
    {

        if (CheckTimer < diff)
        {
            //Check if all bosses are dead and activate Major Domo

        }else CheckTimer -= diff;

    }

    //Used by the map's CanEnter function.
    //This is to prevent players from entering during boss encounters.
    bool IsEncounterInProgress() const 
    {
        return false; 
    };

    //Called when a gameobject is created
    void OnObjectCreate(GameObject *obj) 
    {
        //Still searching for the individual rune ids. 
        //Currently they don't exist within most databases and are hard to find on websites
    }

    //called on creature creation
    void OnCreatureCreate(Creature *creature, uint32 creature_entry) 
    {
        //Store specific creatures based on entry id
        switch (creature_entry)
        {
        case ID_LUCIFRON:
            Lucifron = creature->GetGUID();
            break;

        case ID_MAGMADAR:
            Magmadar = creature->GetGUID();
            break;

        case ID_GEHENNAS:
            Gehennas = creature->GetGUID();
            break;

        case ID_GARR:
            Garr = creature->GetGUID();
            break;

        case ID_GEDDON:
            Geddon = creature->GetGUID();
            break;

        case ID_SHAZZRAH:
            Shazzrah = creature->GetGUID();
            break;

        case ID_SULFURON:
            Sulfuron = creature->GetGUID();
            break;

        case ID_GOLEMAGG:
            Golemagg = creature->GetGUID();
            break;

        case ID_DOMO:
            Domo = creature->GetGUID();
            creature->SetVisibility(VISIBILITY_OFF);
            creature->setFaction(35);
            break;

        case ID_RAGNAROS:
            Ragnaros = creature->GetGUID();
            creature->SetVisibility(VISIBILITY_OFF);
            creature->setFaction(35);
            break;

            default:
            return;
        }
    }

    //Virtual functions that are not used
    Creature* GetUnit(char *identifier) { return NULL; };
    GameObject* GetGO(char *identifier) { return NULL; };

};

InstanceData* GetInstance_MoltenCore(Map *_Map)
{
    return new MoltenCore (_Map);
}


void AddSC_instance_moltencore()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="instance_molten_core";
    newscript->GetInstanceData = &GetInstance_MoltenCore;
    m_scripts[nrscripts++] = newscript;
}

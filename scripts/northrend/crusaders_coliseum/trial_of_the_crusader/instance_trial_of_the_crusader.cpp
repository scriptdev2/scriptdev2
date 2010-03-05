/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: instance_trial_of_the_crusader
SD%Complete: 100
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/* Trial Of The Crusader encounters:
0 - Northrend Beasts
1 - Jaraxxus
2 - Faction Champions
3 - Twin Valkyr
4 - Anubarak
*/

struct MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance
{
    instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

    uint32 m_uiGormokGUID;
    uint32 m_uiAcidmawGUID;
    uint32 m_uiDreadscaleGUID;
    uint32 m_uiIcehowlGUID;
    uint32 m_uiJaraxxusGUID;
    uint32 m_uiFjolaGUID;
    uint32 m_uiEydisGUID;
    uint32 m_uiAnubarakGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiGormokGUID     = 0;
        m_uiAcidmawGUID    = 0;
        m_uiDreadscaleGUID = 0;
        m_uiIcehowlGUID    = 0;
        m_uiJaraxxusGUID   = 0;
        m_uiFjolaGUID      = 0;
        m_uiEydisGUID      = 0;
        m_uiAnubarakGUID   = 0;

    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 34796: m_uiGormokGUID     = pCreature->GetGUID(); break;
            case 35144: m_uiAcidmawGUID    = pCreature->GetGUID(); break;
            case 34799: m_uiDreadscaleGUID = pCreature->GetGUID(); break;
            case 34797: m_uiIcehowlGUID    = pCreature->GetGUID(); break;
            case 34780: m_uiJaraxxusGUID   = pCreature->GetGUID(); break;
            case 34497: m_uiFjolaGUID      = pCreature->GetGUID(); break;
            case 34496: m_uiEydisGUID      = pCreature->GetGUID(); break;
            case 34564: m_uiAnubarakGUID   = pCreature->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Trial Of The Crusader: SetData received for type %u with data %u",uiType,uiData);

        switch(uiType)
        {
             case TYPE_NORTHREND_BEASTS:
                m_auiEncounter[0] = uiData;
                break;
             case TYPE_JARAXXUS:
                m_auiEncounter[1] = uiData;
                break;
             case TYPE_FACTION_CHAMPIONS:
                m_auiEncounter[2] = uiData;
                break;
             case TYPE_TWIN_VALKYR:
                m_auiEncounter[3] = uiData;
                break;
             case TYPE_ANUBARAK:
                m_auiEncounter[4] = uiData;
                break;
             default:
                error_log("SD2: Instance Trial of The Crusader: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_NORTHREND_BEASTS:  return m_auiEncounter[0];
            case TYPE_JARAXXUS:          return m_auiEncounter[1];
            case TYPE_FACTION_CHAMPIONS: return m_auiEncounter[2];
            case TYPE_TWIN_VALKYR:       return m_auiEncounter[3];
            case TYPE_ANUBARAK:          return m_auiEncounter[4];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_GORMOK:     return m_uiGormokGUID;
            case DATA_ACIDMAW:    return m_uiAcidmawGUID;
            case DATA_DREADSCALE: return m_uiDreadscaleGUID;
            case DATA_ICEHOWL:    return m_uiIcehowlGUID;
            case DATA_JARAXXUS:   return m_uiJaraxxusGUID;
            case DATA_FJOLA:      return m_uiFjolaGUID;
            case DATA_EYDIS:      return m_uiEydisGUID;
            case DATA_ANUBARAK:   return m_uiAnubarakGUID;
        }
        return 0;
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
            >> m_auiEncounter[4];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)            // Do not load an encounter as "In Progress" - reset it instead.
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_trial_of_the_crusader(Map* pMap)
{
    return new instance_trial_of_the_crusader(pMap);
}

void AddSC_instance_trial_of_the_crusader()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "instance_trial_of_the_crusader";
    newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_crusader;
    newscript->RegisterSelf();
}

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
SDName: Instance_Temple_of_Ahnqiraj
SD%Complete: 80
SDComment:
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "temple_of_ahnqiraj.h"

struct MANGOS_DLL_DECL instance_temple_of_ahnqiraj : public ScriptedInstance
{
    instance_temple_of_ahnqiraj(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    //Storing Skeram, Vem and Kri.
    uint64 m_uiSkeramGUID;
    uint64 m_uiVemGUID;
    uint64 m_uiKriGUID;
    uint64 m_uiVeklorGUID;
    uint64 m_uiVeknilashGUID;

    uint32 m_uiBugTrioDeathCount;

    uint32 m_uiCthunPhase;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiSkeramGUID = 0;
        m_uiVemGUID = 0;
        m_uiKriGUID = 0;
        m_uiVeklorGUID = 0;
        m_uiVeknilashGUID = 0;

        m_uiBugTrioDeathCount = 0;

        m_uiCthunPhase = 0;
    }

    void OnCreatureCreate (Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
            case 15263: m_uiSkeramGUID = pCreature->GetGUID(); break;
            case 15544: m_uiVemGUID = pCreature->GetGUID(); break;
            case 15511: m_uiKriGUID = pCreature->GetGUID(); break;
            case 15276: m_uiVeklorGUID = pCreature->GetGUID(); break;
            case 15275: m_uiVeknilashGUID = pCreature->GetGUID(); break;
        }
    }

    bool IsEncounterInProgress() const
    {
        //not active in AQ40
        return false;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_VEM:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_VEKLOR:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_VEKNILASH:
                m_auiEncounter[2] = uiData;
                break;

            case DATA_BUG_TRIO_DEATH:
                ++m_uiBugTrioDeathCount;
                break;

            case TYPE_CTHUN_PHASE:
                m_uiCthunPhase = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_VEM:
                return m_auiEncounter[0];

            case DATA_BUG_TRIO_DEATH:
                return m_uiBugTrioDeathCount;

            case TYPE_CTHUN_PHASE:
                return m_uiCthunPhase;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_SKERAM:
                return m_uiSkeramGUID;
            case DATA_VEM:
                return m_uiVemGUID;
            case DATA_KRI:
                return m_uiKriGUID;
            case DATA_VEKLOR:
                return m_uiVeklorGUID;
            case DATA_VEKNILASH:
                return m_uiVeknilashGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_temple_of_ahnqiraj(Map* pMap)
{
    return new instance_temple_of_ahnqiraj(pMap);
}

void AddSC_instance_temple_of_ahnqiraj()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_temple_of_ahnqiraj";
    newscript->GetInstanceData = &GetInstanceData_instance_temple_of_ahnqiraj;
    newscript->RegisterSelf();
}

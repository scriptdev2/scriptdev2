/* ScriptData
SDName: instance_Sunwell_Plateau
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Sunwell_Plateau
EndScriptData */


#include "GameObject.h"
#include "Player.h"
#include "def_sunwell_plateau.h"

#define ENCOUNTERS 7

/* Sunwell Plateau encounters:
1 - kalecgos - shatrovarr
2 - Brutalus
3 - Felmyst
4 - Eredar Twins
5 - M'uru
6 - Kil'Jaden
*/

struct MANGOS_DLL_DECL instance_sunwell_plateau : public ScriptedInstance
{
    instance_sunwell_plateau(Map *Map) : ScriptedInstance(Map) {Initialize();};

    bool Encounters[ENCOUNTERS];

    uint64 alythess_guid;
    uint64 sacrolash_guid;
    uint64 kalecgos_guid;
    uint64 shathrovarr_guid;
    uint64 kalecgos_force_field_guid;
    uint64 kalec_h_guid;
    uint32 kalec_fly;
    uint32 shat_die;
    uint32 checktimer;

    void Initialize()
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = false;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if(Encounters[i]) return true;

        return false;
    }

    void OnCreatureCreate(Creature *creature, uint32 creature_entry)
    {
        switch (creature_entry)
        {
        case 25165:
            sacrolash_guid = creature->GetGUID();
            break;
        case 25166:
            alythess_guid = creature->GetGUID();
            break;
        case 24850:
            kalecgos_guid  = creature->GetGUID();
            break;
        case 24892:
            shathrovarr_guid = creature->GetGUID();
            break;
        case 24891:
            kalec_h_guid = creature->GetGUID();
            break;
        }
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry()){
case KALECGOS_FORCE_FIELD:
    kalecgos_force_field_guid = go->GetGUID();
    go->SetLootState(GO_JUST_DEACTIVATED);
    break;
        }
    }

    void OpenDoor(GameObject *go)
    {
    }

    uint64 GetData64(uint32 identifier)
    {
        switch (identifier)
        {
        case DATA_KALECGOS:
            return kalecgos_guid;
            break;
        case DATA_SHATHROVARR:
            return shathrovarr_guid;
            break;
        case DATA_KALEC_HUMAN:
            return kalec_h_guid;
            break;
        case DATA_SACROLASH:
            return sacrolash_guid;
            break;
        case DATA_ALYTHESS:
            return alythess_guid;
            break;
        case KALECGOS_FORCE_FIELD:
            return kalecgos_force_field_guid;
        }
        return 0;
    }

    uint32 GetData(uint32 identifier)
    {
        switch(identifier)
        {
        case DATA_KALEC_FLY:
            return kalec_fly;
            break;

        case DATA_SHAT_DIE:
            return shat_die;
            break;
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type){
case DATA_KALEC_FLY:
    kalec_fly = data;
    break;
case DATA_SHAT_DIE:
    shat_die = data;
    break;
        }
    }
};
InstanceData* GetInstanceData_instance_sunwell_plateau(Map* map)
{
    return new instance_sunwell_plateau(map);
}

void AddSC_instance_sunwell_plateau()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_sunwell_plateau";
    newscript->GetInstanceData = GetInstanceData_instance_sunwell_plateau;
    m_scripts[nrscripts++] = newscript;
}

#ifndef SC_INSTANCE_H
#define SC_INSTANCE_H

#include "../ScriptMgr.h"
#include "InstanceData.h"
#include "Map.h"

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAILED        = 2,
    DONE          = 3
};

class MANGOS_DLL_DECL ScriptedInstance : public InstanceData
{
    public:

        ScriptedInstance(Map *map) : InstanceData(map) {}
        ~ScriptedInstance() {}

        //All-purpose data storage 64 bit
        virtual uint64 GetData64(uint32) {return 0; }
        virtual void SetData64(uint32, uint64) { }

        //All-purpose data storage 32 bit
        virtual uint32 GetData(uint32) { return 0; }
        virtual void SetData(uint32, uint32 data) {}

        // Called every instance update
        virtual void Update(uint32) {}
};

#endif

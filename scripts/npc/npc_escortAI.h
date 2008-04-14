#include "Creature.h"
#include "CreatureAI.h"
#include "../../ScriptMgr.h"

FUNC_PTR(EscortAICallback ,__stdcall ,void ,(Creature*, Player*, uint32))

struct Escort_Waypoint
{
    Escort_Waypoint(uint32 _id, float _x, float _y, float _z, EscortAICallback _f, uint32 _w)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        pCallBack = _f;
        WaitTimeMs = _w;
    }

    uint32 id;
    float x;
    float y;
    float z;
    EscortAICallback pCallBack;
    uint32 WaitTimeMs;
};

struct MANGOS_DLL_DECL npc_escortAI : private CreatureAI
{
public:

    // CreatureAI functions
    npc_escortAI(Creature *c) : m_creature(c), IsBeingEscorted(false), InCombat(false) {m_creature->GetPosition(LastPos.x, LastPos.y, LastPos.z);}

    bool IsVisible(Unit*) const;

    void AttackStart(Unit*);

    void MoveInLineOfSight(Unit*);

    void JustDied(Unit*);

    void EnterEvadeMode();

    void UpdateAI(const uint32);

    void MovementInform(uint32, uint32);

    void JustSpawned(){m_creature->SetUInt32Value(168, 1);}
    
    // EscortAI functions
    void AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs = 0, EscortAICallback pCallBack = NULL);

    void Start(bool Attack, bool Defend);

private:
    uint32 WaitTimer;

    struct
    {        
        float x;
        float y;
        float z;
    }LastPos;

    std::list<Escort_Waypoint> WaypointList;
    std::list<Escort_Waypoint>::iterator CurrentWP;

    Creature* m_creature;

    bool AttackMLOS;
    bool DefendAttackStart;
    bool IsBeingEscorted;
    bool InCombat;
    bool ReconnectWP;
};
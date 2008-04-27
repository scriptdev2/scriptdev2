#include "Creature.h"
#include "CreatureAI.h"
#include "../../ScriptMgr.h"

struct Escort_Waypoint
{
    Escort_Waypoint(uint32 _id, float _x, float _y, float _z, uint32 _w)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        WaitTimeMs = _w;
    }

    uint32 id;
    float x;
    float y;
    float z;
    uint32 WaitTimeMs;
};

struct MANGOS_DLL_DECL npc_escortAI : private CreatureAI
{
public:

    // Pure Virtual Functions
    virtual void WaypointReached(uint32) = 0;

    virtual void Aggro(Unit*) = 0;

    virtual void Reset() = 0;

    // CreatureAI functions
    npc_escortAI(Creature *c) : m_creature(c), IsBeingEscorted(false), InCombat(false), PlayerTimer(1000) {m_creature->GetPosition(LastPos.x, LastPos.y, LastPos.z);}

    bool IsVisible(Unit*) const;

    void AttackStart(Unit*);

    void MoveInLineOfSight(Unit*);

    void JustRespawned();

    void EnterEvadeMode();

    void UpdateAI(const uint32);

    void MovementInform(uint32, uint32);

    // EscortAI functions
    void AddWaypoint(uint32 id, float x, float y, float z, uint32 WaitTimeMs = 0);

    void Start(bool bAttack, bool bDefend, bool bRun, uint64 pGUID = 0);

    // EscortAI variables
protected:
    Creature* m_creature;
    uint64 PlayerGUID;
    bool IsBeingEscorted;
    bool InCombat;

private:
    uint32 WaitTimer;
    uint32 PlayerTimer;

    struct
    {        
        float x;
        float y;
        float z;
    }LastPos;

    std::list<Escort_Waypoint> WaypointList;
    std::list<Escort_Waypoint>::iterator CurrentWP;

    bool Attack;
    bool Defend;
    bool Returning;
    bool ReconnectWP;
    bool Run;
};

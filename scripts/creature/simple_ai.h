#include "../sc_defines.h"

enum CastTarget
{
    CAST_SELF = 0,                  //Self cast
    CAST_HOSTILE_TARGET,            //Our current target (ie: highest aggro)
    CAST_HOSTILE_SECOND_AGGRO,      //Second highest aggro (generaly used for cleaves and some special attacks)
    CAST_HOSTILE_LAST_AGGRO,        //Dead last on aggro (no idea what this could be used for)
    CAST_HOSTILE_RANDOM,            //Just any random target on our threat list
    CAST_FRIENDLY_RANDOM,           //NOT YET IMPLEMENTED

    //Special cases
    CAST_KILLEDUNIT_VICTIM,         //Only works within KilledUnit function
    CAST_JUSTDIED_KILLER,           //Only works within JustDied function
};

struct MANGOS_DLL_DECL SimpleAI : public ScriptedAI
{
    SimpleAI(Creature *c);// : ScriptedAI(c);

    void EnterEvadeMode();

    void AttackStart(Unit *who);

    void KilledUnit(Unit *victim);

    void DamageTaken(Unit *killer, uint32 &damage);

    void MoveInLineOfSight(Unit *who);

    void UpdateAI(const uint32 diff);

public:

    char* AggroYell[3];
    uint32 Aggro_Sound[3];

    char* Death_Yell[3];
    uint32 Death_Sound[3];
    uint32 Death_Spell;
    uint32 Death_Target_Type;

    char* Kill_Yell[3];
    uint32 Kill_Sound[3];
    uint32 Kill_Spell;
    uint32 Kill_Target_Type;

    struct SimpleAI_Spell
    {
        uint32 Spell_Id;                //Spell ID to cast
        int32 First_Cast;               //Delay for first cast
        int32 Cooldown;                 //Cooldown between casts
        uint32 Cast_Target_Type;        //Target type (note that certain spells may ignore this)
        bool InturruptPreviousCast;     //Inturrupt a previous cast if this spell needs to be cast
        bool Enabled;                   //Spell enabled or disabled (default: false)

        //3 yells to many?
        char* Yell[3];
        uint32 Yell_Sound[3];
    }Spell[10];

protected:
    uint32 Spell_Timer[10];
    bool InCombat;
};

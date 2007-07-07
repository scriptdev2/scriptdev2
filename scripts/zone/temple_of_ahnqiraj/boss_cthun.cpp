//Cthun - No txt only sound
//8580 Death is close
//8581 You are already dead
//8582 Your courage will fail
//8583 Your friends will abandon you
//8584 You will betray your friends
//8585 You will die
//8586 You are weak
//8587 Your heart will explode

#include "../../sc_defines.h"

//Eye Spells
#define SPELL_GREEN_BEAM                26134
#define SPELL_DARK_GLARE                26029

//Eye Tentacles Spells
#define SPELL_MIND_FLAY                 26143

//Claw Tentacles Spells
#define SPELL_GROUND_RUPTURE            26139
#define SPELL_HAMSTRING                 26141

//*****Phase 2******

//Body spells
#define SPELL_CARAPACE_CTHUN            26156

//Eye Tentacles Spells
//SAME AS PHASE1

//Giant Claw Tentacles 
#define SPELL_MASSIVE_GROUND_RUPTURE    26100

//Giant Eye Tentacles
//CHAIN CASTS "SPELL_GREEN_BEAM"

//Stomach Spells
#define SPELL_EXIT_STOMACH_KNOCKBACK     26230
#define SPELL_DIGESTIVE_ACID             26476

struct MANGOS_DLL_DECL eye_of_cthunAI : public ScriptedAI
{
    eye_of_cthunAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    
    void EnterEvadeMode()
    {

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }
    
    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        //We simply use this function to find players until we can use Map->GetPlayers()

        if (who && who->GetTypeId() == TYPEID_PLAYER && m_creature->IsHostileTo(who))
        {
            //Add them to our threat list
            m_creature->AddThreat(who,0.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;
        
        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {


        }
    }
};

CreatureAI* GetAI_eye_of_cthun(Creature *_Creature)
{
    return new eye_of_cthunAI (_Creature);
}

void AddSC_boss_eye_of_cthun()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_eye_of_cthun";
    newscript->GetAI = GetAI_eye_of_cthun;
    m_scripts[nrscripts++] = newscript;
}

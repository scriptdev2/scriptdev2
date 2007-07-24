#include "simple_ai.h"

SimpleAI::SimpleAI(Creature *c) : ScriptedAI(c)
{
    EnterEvadeMode();
}

void SimpleAI::EnterEvadeMode()
{
    InCombat = false;

    //Reset cast timers
    Spell_Timer[0] = Spell[0].First_Cast;
    Spell_Timer[1] = Spell[1].First_Cast;
    Spell_Timer[2] = Spell[2].First_Cast;
    Spell_Timer[3] = Spell[3].First_Cast;
    Spell_Timer[4] = Spell[4].First_Cast;
    Spell_Timer[5] = Spell[5].First_Cast;
    Spell_Timer[6] = Spell[6].First_Cast;
    Spell_Timer[7] = Spell[7].First_Cast;
    Spell_Timer[8] = Spell[8].First_Cast;
    Spell_Timer[9] = Spell[9].First_Cast;

    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    DoGoHome();
}

void SimpleAI::AttackStart(Unit *who)
{
    if (!who)
        return;

    if (who->isTargetableForAttack() && who!= m_creature)
    {
        //Begin melee attack if we are within range
        DoStartMeleeAttack(who);

        //Say our dialog
        if (!InCombat)
        {
            uint32 random_yell = rand()%3;
            //Random yell
            if (AggroYell[random_yell])
                DoYell(AggroYell[random_yell], LANG_UNIVERSAL, who);

            //Random sound
            if (Aggro_Sound[random_yell])
                DoPlaySoundToSet(m_creature, Aggro_Sound[random_yell]);

            InCombat = true;
        }
    }
}

void SimpleAI::MoveInLineOfSight(Unit *who)
{
    if (!who || m_creature->getVictim())
        return;

    if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
    {
        float attackRadius = m_creature->GetAttackDistance(who);
        if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
        {
            if(who->HasStealthAura())
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            
            //Say our dialog
            if (!InCombat)
            {
                uint32 random_yell = rand()%3;
                //Random yell
                if (AggroYell[random_yell])
                    DoYell(AggroYell[random_yell], LANG_UNIVERSAL, who);

                //Random sound
                if (Aggro_Sound[random_yell])
                    DoPlaySoundToSet(m_creature, Aggro_Sound[random_yell]);

                InCombat = true;
            }
        }
    }
}

void SimpleAI::KilledUnit(Unit *victim)
{
    uint32 random_yell = rand()%3;

    //Random yell
    if (Kill_Yell[random_yell])
        DoYell(Kill_Yell[random_yell], LANG_UNIVERSAL, victim);

    //Random sound
    if (Kill_Sound[random_yell])
        DoPlaySoundToSet(m_creature, Kill_Sound[random_yell]);

    if (!Kill_Spell)
        return;

    Unit* target = NULL;

    switch (Kill_Target_Type)
    {
    case CAST_SELF:
        target = m_creature;
        break;
    case CAST_HOSTILE_TARGET:
        target = m_creature->getVictim();
        break;
    case CAST_HOSTILE_SECOND_AGGRO:
        target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
        break;
    case CAST_HOSTILE_LAST_AGGRO:
        target = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
        break;
    case CAST_HOSTILE_RANDOM:
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        break;
    case CAST_KILLEDUNIT_VICTIM:
        target = victim;
        break;
    }

    //Target is ok, cast a spell on it
    if (target)
        DoCast(target, Kill_Spell);
}

void SimpleAI::JustDied(Unit *killer)
{
    uint32 random_yell = rand()%3;

    //Random yell
    if (Death_Yell[random_yell])
        DoYell(Death_Yell[random_yell], LANG_UNIVERSAL, killer);

    //Random sound
    if (Death_Sound[random_yell])
        DoPlaySoundToSet(m_creature, Death_Sound[random_yell]);

    if (!Kill_Spell)
        return;

    Unit* target = NULL;

    switch (Death_Target_Type)
    {
    case CAST_SELF:
        target = m_creature;
        break;
    case CAST_HOSTILE_TARGET:
        target = m_creature->getVictim();
        break;
    case CAST_HOSTILE_SECOND_AGGRO:
        target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
        break;
    case CAST_HOSTILE_LAST_AGGRO:
        target = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
        break;
    case CAST_HOSTILE_RANDOM:
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        break;
    case CAST_JUSTDIED_KILLER:
        target = killer;
        break;
    }

    //Target is ok, cast a spell on it
    if (target)
        DoCast(target, Death_Spell);
}

void SimpleAI::UpdateAI(const uint32 diff)
{
    //Return since we have no target
    if (!m_creature->SelectHostilTarget())
        return;

    //Check if we have a current target
    if( m_creature->getVictim() && m_creature->isAlive())
    {
        //Spells
        for (uint32 i = 0; i < 10; i++)
        {
            //Spell not valid
            if (!Spell[i].Enabled || !Spell[i].Spell_Id || !Spell[i].Cooldown)
                continue;

            if (Spell_Timer[i] < diff)
            {
                //Check Current spell
                if (!(Spell[i].InturruptPreviousCast && m_creature->m_currentSpells[CURRENT_GENERIC_SPELL]))
                {
                    Unit* target = NULL;

                    switch (Spell[i].Cast_Target_Type)
                    {
                    case CAST_SELF:
                        target = m_creature;
                        break;
                    case CAST_HOSTILE_TARGET:
                        target = m_creature->getVictim();
                        break;
                    case CAST_HOSTILE_SECOND_AGGRO:
                        target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
                        break;
                    case CAST_HOSTILE_LAST_AGGRO:
                        target = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
                        break;
                    case CAST_HOSTILE_RANDOM:
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        break;
                    }

                    //Target is ok, cast a spell on it and then do our random yell
                    if (target)
                    {
                        DoCast(target, Spell[i].Spell_Id);

                        //Yell and sound use the same number so that you can make
                        //the creature yell with the correct sound effect attached
                        uint32 random_yell = rand()%3;

                        //Random yell
                        if (Spell[i].Yell[random_yell])
                            DoYell(Spell[i].Yell[random_yell], LANG_UNIVERSAL, target);

                        //Random sound
                        if (Spell[i].Yell_Sound[random_yell])
                            DoPlaySoundToSet(m_creature, Spell[i].Yell_Sound[random_yell]);
                    }

                }

                //Spell will cast agian when the cooldown is up
                Spell_Timer[i] = Spell[i].Cooldown;
            }else Spell_Timer[i] -= diff;

        }

        DoMeleeAttackIfReady();
    }
}


#include "../../sc_defines.h"

#define SAY_AGGRO           "Madness has brought you here to me. I shall be your undoing."
#define SOUND_AGGRO         9218
#define SAY_SUMMON1         "You face not Malchezaar alone, but the legions I command!"
#define SOUND_SUMMON1       9322
#define SAY_SUMMON2         "All realities, all dimensions are open to me!"
#define SOUND_SUMMON2       9224
#define SAY_PHASE2          "Time is the fire in which you'll burn!"
#define SOUND_PHASE2        9220
#define SAY_PHASE3          "How can you hope to withstand against such overwhelming power?"
#define SOUND_PHASE3        9321
#define SAY_PDEATH1         "You are, but a plaything, unfit even to amuse."
#define SOUND_PDEATH1       9319
#define SAY_PDEATH2         "Your greed, your foolishness has brought you to this end."
#define SOUND_PDEATH2       9318
#define SAY_PDEATH3         "Surely you did not think you could win."
#define SOUND_PDEATH3       9222
#define SAY_DEATH           "I refuse to concede defeat. I am a prince of the Eredar! I am..."
#define SOUND_DEATH         9221

// 19 Coordinates for Infernal spawns
#define ADDZ  91

#define ADDX1 -11121
#define ADDY1 -1966

#define ADDX2 -11137
#define ADDY2 -1971

#define ADDX3 -11162
#define ADDY3 -1956

#define ADDX4 -11158
#define ADDY4 -1941

#define ADDX5 -11172
#define ADDY5 -1931

#define ADDX6 -11118
#define ADDY6 -1972

#define ADDX7 -11152
#define ADDY7 -1957

#define ADDX8 -11167
#define ADDY8 -1942

#define ADDX9 -11165
#define ADDY9 -1917

#define ADDX10 -11164
#define ADDY10 -1934

#define ADDX11 -11167
#define ADDY11 -1903

#define ADDX12 -11151
#define ADDY12 -1899

#define ADDX13 -11145
#define ADDY13 -1883

#define ADDX14 -11132
#define ADDY14 -1888

#define ADDX15 -11117
#define ADDY15 -1880

#define ADDX16 -11107
#define ADDY16 -1891

#define ADDX17 -11093
#define ADDY17 -1889

#define ADDX18 -11091
#define ADDY18 -1904

#define ADDX19 -11141
#define ADDY19 -1961

/*
SPECIAL?
9223
9320

AXETOSS2?
9317*/

//Enfeeble is supposed to reduce hp to 1 and then heal player back to full when it ends
//Along with reducing healing and regen while enfeebled to 0%
//This spell effect will only reduce healing
#define SPELL_ENFEEBLE          30843

#define SPELL_SHADOWNOVA        30852
#define SPELL_SW_PAIN           30854
#define SPELL_THRASH_PASSIVE    12787

#define SPELL_SUNDER_ARMOR      25225
#define SPELL_CLEAVE            15663

//Axe's use this spell
#define SPELL_AMPLIFY_DAMAGE    12738

//Infernals cast this once every 3 seconds and they do not move
#define SPELL_INFERNAL_EFFECT    23056
#define SPELL_HELLFIRE          39132

#define NETHERSPITE_INFERNAL    17646

struct MANGOS_DLL_DECL boss_malchezaarAI : public ScriptedAI
{
    boss_malchezaarAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 EnfeebleTimer;
    uint32 EnfeebleResetTimer;
    uint32 ShadowNovaTimer;
    uint32 SWPainTimer;
    uint32 SunderArmorTimer;
    uint32 CleaveTimer;
    uint32 AmplifyDamageTimer;

    Unit* target1;
    Unit* target2;
    Unit* target3;
    Unit* target4;
    Unit* target5;

    uint32 AddTimer;
    Creature* Invisible;
    Creature* Infernal;
    Creature* Axe;
    int Rand;
    int RandX;
    int RandY;

    uint32 phase;

    bool Enfeebled;
    bool InCombat;
    bool HasSummonedAxes;

    void EnterEvadeMode()
    {
        EnfeebleTimer = 30000;
        EnfeebleResetTimer = 38000;
        ShadowNovaTimer = 35000;
        SWPainTimer = 20000;
        CleaveTimer = 5000;
        SunderArmorTimer = 15000;
        AmplifyDamageTimer = 10000;
        AddTimer = 45000;
        phase = 1;

        InCombat = false;
        Enfeebled = false;
        HasSummonedAxes = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_PDEATH1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(victim, SOUND_PDEATH1);
            break;
        case 1:
            DoYell(SAY_PDEATH2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(victim, SOUND_PDEATH2);
            break;
        case 2:
            DoYell(SAY_PDEATH3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(victim, SOUND_PDEATH3);
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {        
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            //Say our dialog
            if (!InCombat)
            {
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
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

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }

                DoStartMeleeAttack(who);
            }
        }
    }

    void EnfeebleHealthEffect()
    {
        target1 = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target1 && (target1 != m_creature->getVictim()))
        {
            DoCast(target1, SPELL_ENFEEBLE);
            target1->SetHealth(1);
        }

        target2 = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target2 && (target2 != m_creature->getVictim()))
        {
            DoCast(target2, SPELL_ENFEEBLE);
            target2->SetHealth(1);
        }

        target3 = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target3 && (target3 != m_creature->getVictim()))
        {
            DoCast(target3, SPELL_ENFEEBLE);
            target3->SetHealth(1);
        }

        target4 = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target4 && (target4 != m_creature->getVictim()))
        {
            DoCast(target4, SPELL_ENFEEBLE);
            target4->SetHealth(1);
        }

        target5 = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target5 && (target5 != m_creature->getVictim()))
        {
            DoCast(target5, SPELL_ENFEEBLE);
            target5->SetHealth(1);
        }
        Enfeebled = true;
    }

    void EnfeebleResetHealth()
    {
        if(target1 && target1->isAlive())    target1->SetHealth(target1->GetMaxHealth());
        if(target2 && target2->isAlive())    target2->SetHealth(target2->GetMaxHealth());
        if(target3 && target3->isAlive())    target3->SetHealth(target3->GetMaxHealth());
        if(target4 && target4->isAlive())    target4->SetHealth(target4->GetMaxHealth());
        if(target5 && target5->isAlive())    target5->SetHealth(target5->GetMaxHealth());
        Enfeebled = false;
    }

    uint32 DoCalculateRandomLocation(uint32 Pos)
    {
        uint32 Loc;
        uint32 Rand = rand()%31;
        switch(rand()%2)
        {
        case 0:
            Loc = Pos + Rand;
            break;
        case 1:
            Loc = Pos -Rand;
            break;
        }

        return Loc;
    }

    void SummonInfernalKarazhan()
    {
        uint32 MapID = 532; // MapID for Karazhan Instance
        Invisible = NULL;
        Infernal = NULL;
        switch(rand()%19)
        {
        case 0:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX1, ADDY1, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX1, ADDY1, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 1:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX2, ADDY2, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX2, ADDY2, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 2:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX3, ADDY4, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX3, ADDY4, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 3:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX5, ADDY5, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX5, ADDY5, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 4:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX6, ADDY6, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX6, ADDY6, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 5:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX7, ADDY7, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX7, ADDY7, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 6:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX8, ADDY8, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX8, ADDY8, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 7:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX9, ADDY9, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX9, ADDY9, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 8:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX10, ADDY10, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX10, ADDY10, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 9:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX11, ADDY11, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX11, ADDY11, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 10:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX12, ADDY12, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX12, ADDY12, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 12:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX13, ADDY13, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX13, ADDY13, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 13:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX14, ADDY14, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX14, ADDY14, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 14:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX15, ADDY15, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX15, ADDY15, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 15:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX16, ADDY16, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX16, ADDY16, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 16:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX17, ADDY17, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX17, ADDY17, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 17:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX18, ADDY18, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX18, ADDY18, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        case 18:
            Invisible = m_creature->SummonCreature(12999, MapID, ADDX19, ADDY19, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, ADDX19, ADDY19, ADDZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
            break;
        }

        if (Invisible)
        {
            Invisible->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(Invisible, SPELL_INFERNAL_EFFECT);    
        }

        if (Infernal)
        {
            Infernal->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            //((netherspite_infernalAI*)Infernal)->SetMalchezzarTarget(m_creature);
        }
    }

    void SummonInfernal(const uint32 diff)
    {
        if(m_creature->GetMapId() != 532)
        {
            uint32 MapID = m_creature->GetMapId();
            uint32 PosX = m_creature->GetPositionX();
            uint32 PosY = m_creature->GetPositionY();
            uint32 PosZ = m_creature->GetPositionZ();

            uint32 LocX = DoCalculateRandomLocation(PosX);
            uint32 LocY = DoCalculateRandomLocation(PosY);

            Invisible = NULL;
            Infernal = NULL;

            Invisible = m_creature->SummonCreature(12999, MapID, LocX, LocY, PosZ, 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
            Infernal = m_creature->SummonCreature(NETHERSPITE_INFERNAL, MapID, LocX, LocY, PosZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);

            if (Invisible)
            {
                Invisible->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoCast(Invisible, SPELL_INFERNAL_EFFECT);    
            }

            if (Infernal)
            {
                Infernal->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                //((netherspite_infernalAI*)Infernal)->SetMalchezzarTarget(m_creature);
            }

        }
        else SummonInfernalKarazhan();

        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_SUMMON1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SUMMON1);
            break;
        case 1:
            DoYell(SAY_SUMMON2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SUMMON2);
            break;
        case 2:
            break;
        }
        if (Infernal)
            Infernal->AI()->UpdateAI(diff);
    }

    void SummonAxe()
    {
        uint32 MapID = m_creature->GetMapId();
        uint32 PosX = m_creature->GetPositionX();
        uint32 PosY = m_creature->GetPositionY();
        uint32 PosZ = m_creature->GetPositionZ();

        Axe = m_creature->SummonCreature(17650, MapID, PosX, PosY, PosZ, 0, TEMPSUMMON_TIMED_DESPAWN, 180000);
        Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);

        if(Axe && target)
            Axe->AI()->AttackStart(target);
    }

    void UpdateAI(const uint32 diff)
    {

        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 60)
                phase = 1;

            if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 60) && (phase == 1))
            {
                phase = 2;
                DoYell(SAY_PHASE2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_PHASE2);
                HasSummonedAxes = false;
                m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 40066);
                m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 40066);
            }

            if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30) && (phase == 2))
            {
                AddTimer = 20000;
                phase = 3;
                DoYell(SAY_PHASE3, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_PHASE3);
                m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
                m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 0);
            }

            if(phase == 1 || phase == 3)
            {
                if(SWPainTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_SW_PAIN);
                    SWPainTimer = 20000;
                }else SWPainTimer -= diff;

                if(ShadowNovaTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_SHADOWNOVA);
                    ShadowNovaTimer = 35000;
                }else ShadowNovaTimer -= diff;
            }

            if(phase == 1 || phase == 2)
            {
                if(AddTimer < diff)
                {
                    SummonInfernal(diff);
                    AddTimer = 45000;
                }else AddTimer -= diff;

                if(EnfeebleTimer < diff)
                {
                    EnfeebleHealthEffect();
                    EnfeebleTimer = 30000;
                    EnfeebleResetTimer = 8000;
                }else EnfeebleTimer -= diff;

                if((EnfeebleResetTimer < diff) && Enfeebled)
                {
                    EnfeebleResetHealth();
                }else EnfeebleResetTimer -= diff;

                if(ShadowNovaTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_SHADOWNOVA);
                    ShadowNovaTimer = 35000;
                }else ShadowNovaTimer -= diff;
            }

            if(phase == 2)
            {
                if(CleaveTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                    CleaveTimer = 5000;
                }else CleaveTimer -= diff;

                if(SunderArmorTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_SUNDER_ARMOR);
                    SunderArmorTimer = 15000;
                }else SunderArmorTimer -= diff;
            }

            if(phase == 3)
            {
                if(!HasSummonedAxes)
                {
                    Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    SummonAxe();
                    SummonAxe();
                    HasSummonedAxes = true;
                }

                if(AmplifyDamageTimer < diff)
                {
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    DoCast(target, SPELL_AMPLIFY_DAMAGE);
                    AmplifyDamageTimer = 10000;
                }else AmplifyDamageTimer -= diff;

                if(AddTimer < diff)
                {
                    SummonInfernal(diff);
                    AddTimer = 20000;
                }else AddTimer -= diff;

            }

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)                        
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
};


CreatureAI* GetAI_boss_malchezaar(Creature *_Creature)
{
    return new boss_malchezaarAI (_Creature);
}

void AddSC_boss_malchezaar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_malchezaar";
    newscript->GetAI = GetAI_boss_malchezaar;

    m_scripts[nrscripts++] = newscript;
}
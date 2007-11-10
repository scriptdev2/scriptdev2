/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "../../sc_defines.h"
#include "../../../../../shared/WorldPacket.h"
#include "../../../../../game/Player.h"

//Spells

#define SPELL_FLAMESTRIKE          41481
#define SPELL_BLIZZARD               41482
#define SPELL_ARCANE_BOLT          41483 // Won't use it, because Spellsteal is broken.
#define SPELL_ARCANE_EXPLOSION     41524
#define SPELL_CHROMATIC_AURA     41453

#define SPELL_EMPOWERED_SMITE          41471
#define SPELL_CIRCLE_OF_HEALING          41455
#define SPELL_REFLECTIVE_SHIELD          41475
#define SPELL_DIVINE_WRATH          41472

#define SPELL_BLESS_PROTECTION          41450 // To be tested
#define SPELL_BLESS_SPELLWARD          41451 // To be tested
#define SPELL_CONSECRATION          41541
#define SPELL_HAMMER_OF_JUSTICE          41468
#define SPELL_SEAL_OF_COMMAND          41469

#define SPELL_DEADLY_POISON          41485
#define SPELL_ENVENOM               41487
#define SPELL_VANISH               41479


//Speech'n'Sounds

#define SAY_GATH_AGGRO               "I have better things to do!"
#define SOUND_GATH_AGGRO          11422

#define SAY_GATH_SLAY               "Selama am'oronor!"
#define SOUND_GATH_SLAY               11423

#define SAY_GATH_COMNT               "Well done!"
#define SOUND_GATH_COMNT          11424

#define SAY_GATH_DEATH               "Lord Illidan... I..."
#define SOUND_GATH_DEATH          11425

#define SAY_GATH_SPECIAL1          "Enjoy your final moments!"
#define SOUND_GATH_SPECIAL1          11426

#define SAY_GATH_SPECIAL2          "You are mine!"
#define SOUND_GATH_SPECIAL2          11427

#define SAY_GATH_ENRAGE               "Enough games!"
#define SOUND_GATH_ENRAGE          11428


#define SAY_MALA_AGGRO               "Flee, or die!"
#define SOUND_MALA_AGGRO          11482

#define SAY_MALA_SLAY               "My work is done."
#define SOUND_MALA_SLAY               11483

#define SAY_MALA_COMNT               "As it should be!"
#define SOUND_MALA_COMNT          11484

#define SAY_MALA_DEATH               "Destiny... awaits."
#define SOUND_MALA_DEATH          11485

#define SAY_MALA_SPECIAL1          "No second chances!"
#define SOUND_MALA_SPECIAL1          11486

#define SAY_MALA_SPECIAL2          "I'm full of surprises!"
#define SOUND_MALA_SPECIAL2          11487

#define SAY_MALA_ENRAGE               "For Quel'Thalas! For the Sunwell!"
#define SOUND_MALA_ENRAGE          11488


#define SAY_ZERE_AGGRO               "Common... such a crude language. Bandal!"
#define SOUND_ZERE_AGGRO          11440

#define SAY_ZERE_SLAY               "Shorel'aran."
#define SOUND_ZERE_SLAY               11441

#define SAY_ZERE_COMNT               "Belesa menoor!"
#define SOUND_ZERE_COMNT          11442

#define SAY_ZERE_DEATH               ""
#define SOUND_ZERE_DEATH          11443

#define SAY_ZERE_SPECIAL1          ""
#define SOUND_ZERE_SPECIAL1          11444

#define SAY_ZERE_SPECIAL2          ""
#define SOUND_ZERE_SPECIAL2          11445

#define SAY_ZERE_ENRAGE               ""
#define SOUND_ZERE_ENRAGE          11446


#define SAY_VERA_AGGRO               "You wish to test me?"
#define SOUND_VERA_AGGRO          11524

#define SAY_VERA_SLAY               "Valiant effort!"
#define SOUND_VERA_SLAY               11525

#define SAY_VERA_COMNT               "A glorious kill!"
#define SOUND_VERA_COMNT          11526

#define SAY_VERA_DEATH               "You got lucky!"
#define SOUND_VERA_DEATH          11527

#define SAY_VERA_SPECIAL1          "You're not caught up for this!" // ?
#define SOUND_VERA_SPECIAL1          11528

#define SAY_VERA_SPECIAL2          "Anar'alah belore!"
#define SOUND_VERA_SPECIAL2          11529

#define SAY_VERA_ENRAGE               "You wish to kill me? Hahaha, you first!"
#define SOUND_VERA_ENRAGE          11530

//Locations
#define X_VANISH               111.643913
#define Y_VANISH               561.843872
#define Z_VANISH               98.430679

struct MANGOS_DLL_DECL boss_illidari_councilAI : public ScriptedAI
{
    boss_illidari_councilAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        else pInstance = NULL;

        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        Council[3] = 0;

        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint64 Council[4];

    uint32 CheckTimer;

    bool InCombat;

    void SetVariables()
    {     
        InCombat = false;
        CheckTimer = 2000;

        Creature* Member;
        for(uint8 i = 0; i < 4; i++)
        {
            Member = ((Creature*)Unit::GetUnit((*m_creature), Council[i]));
            if(Member)
            {
                ((Creature*)Member)->Respawn();
                ((Creature*)Member)->AI()->EnterEvadeMode();
            }
        }
        if(pInstance)
            pInstance->SetData("IllidariCouncilEvent",0);
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_OFF);
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        return;
    }

    void JustDied(Unit *killer)
    {
        DoYell("I have just been killed!", LANG_UNIVERSAL, NULL);
    }

    void MoveInLineOfSight(Unit *who)
    {          
        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(!InCombat)
                {
                    if(pInstance)
                        pInstance->SetData("IllidariCouncil", 1);

                    StartEvent(who);
                    DoYell("I sense a $N near me! ATTACK MY MINIONS!", LANG_UNIVERSAL, who);
                    InCombat = true;
                }
            }
        }
    }

    void StartEvent(Unit *target)
    {
        if(!pInstance) return;
        if(target)
        {
            Council[0] = pInstance->GetUnitGUID("GathiosTheShatterer");
            Council[1] = pInstance->GetUnitGUID("HighNethermancerZerevor");
            Council[2] = pInstance->GetUnitGUID("LadyMalande");
            Council[3] = pInstance->GetUnitGUID("VerasDarkshadow");

            if(!Council[0] || !Council[1] || !Council[2] || !Council[3])
                return;

            Creature* Gathios = NULL;
            Creature* Zerevor = NULL;
            Creature* Malande = NULL;
            Creature* Veras = NULL;

            Gathios = ((Creature*)Unit::GetUnit((*m_creature), Council[0]));
            Zerevor = ((Creature*)Unit::GetUnit((*m_creature), Council[1]));
            Malande = ((Creature*)Unit::GetUnit((*m_creature), Council[2]));
            Veras = ((Creature*)Unit::GetUnit((*m_creature), Council[3]));
            if(Gathios)
                Gathios->AddThreat(target, 1.0f);
            if(Zerevor)
                Zerevor->AddThreat(target, 1.0f);
            if(Malande)
                Malande->AddThreat(target, 1.0f);
            if(Veras)
                Veras->AddThreat(target, 1.0f);

            pInstance->SetData("IllidariCouncilEvent", 1);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!InCombat) return;

        if(CheckTimer < diff)
        {
            if(!Council[0] || !Council[1] || !Council[2] || !Council[3])
                return;

            uint32 CouncilCounter = 0;
            uint32 HealthLost = 0;


            Creature* Gathios = NULL;
            Creature* Zerevor = NULL;
            Creature* Malande = NULL;
            Creature* Veras = NULL;

            Gathios = ((Creature*)Unit::GetUnit((*m_creature), Council[0]));
            Zerevor = ((Creature*)Unit::GetUnit((*m_creature), Council[1]));
            Malande = ((Creature*)Unit::GetUnit((*m_creature), Council[2]));
            Veras = ((Creature*)Unit::GetUnit((*m_creature), Council[3]));

            // Don't allow players to pull one of the council's members, aggro another members' target if none present
            if(Gathios->isAlive() && !Gathios->SelectHostilTarget())
                Gathios->AddThreat(Veras->getVictim(), 1.0f);

            if(Zerevor->isAlive() && !Zerevor->SelectHostilTarget())
                Zerevor->AddThreat(Gathios->getVictim(), 1.0f);

            if(Malande->isAlive() && !Malande->SelectHostilTarget())
                Malande->AddThreat(Zerevor->getVictim(), 1.0f);

            if(Veras->isAlive() && !Veras->SelectHostilTarget())
                Veras->AddThreat(Malande->getVictim(), 1.0f);

            /*CouncilCounter = 0;
            HealthLost = 0;

            if(Gathios->isAlive())
            {
            CouncilCounter++;
            HealthLost += Gathios->GetMaxHealth() - Gathios->GetHealth();
            }
            if(Zerevor->isAlive())
            {
            CouncilCounter++;
            HealthLost += Zerevor->GetMaxHealth() - Zerevor->GetHealth();
            }
            if(Malande->isAlive())
            {
            CouncilCounter++;
            HealthLost += Malande->GetMaxHealth() - Malande->GetHealth();
            }
            if(Veras->isAlive())
            {
            CouncilCounter++;
            HealthLost += Veras->GetMaxHealth() - Veras->GetHealth();
            }

            if(CouncilCounter > 0)
            {
            HealthLost /= CouncilCounter;

            DoYell("Setting Health", LANG_UNIVERSAL, NULL);
            if(Gathios->isAlive())
            Gathios->SetHealth(Gathios->GetMaxHealth() - HealthLost);
            if(Zerevor->isAlive())
            Zerevor->SetHealth(Zerevor->GetMaxHealth() - HealthLost);
            if(Malande->isAlive())
            Malande->SetHealth(Malande->GetMaxHealth() - HealthLost);
            if(Veras->isAlive())
            Veras->SetHealth(Veras->GetMaxHealth() - HealthLost);
            }
            */
            if(!Gathios->isAlive() && !Zerevor->isAlive() && !Malande->isAlive() && !Veras->isAlive())
            {
                if(pInstance)
                    pInstance->SetData("IllidariCouncilEvent", 3);

                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
            }

            CheckTimer = 2000;
        }else CheckTimer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_gathios_the_shattererAI : public ScriptedAI
{
    boss_gathios_the_shattererAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        SetVariables();
    }

    uint64 Council[2];

    ScriptedInstance* pInstance;

    uint32 ConsecrationTimer;
    uint32 HammerOfJusticeTimer;
    uint32 SealOfCommandTimer;
    uint32 ChromaticAuraTimer;

    bool AcquiredGUIDs;
    bool InCombat;

    void SetVariables()
    {
        InCombat = false;
        AcquiredGUIDs = false;

        ConsecrationTimer = 40000;
        HammerOfJusticeTimer = 10000;
        SealOfCommandTimer = 20000;
        ChromaticAuraTimer = 90000;
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                DoYell(SAY_GATH_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_GATH_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    DoYell(SAY_GATH_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_GATH_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        DoYell(SAY_GATH_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_GATH_SLAY);
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_GATH_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_GATH_DEATH);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        for(uint8 i = 0; i < 3; i++)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit)
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetUnitGUID("HighNethermancerZerevor");
                Council[1] = pInstance->GetUnitGUID("LadyMalande");
                Council[2] = pInstance->GetUnitGUID("VerasDarkshadow");
            }
            AcquiredGUIDs = true;
        }

        if(ConsecrationTimer < diff)
        {
            DoCast(m_creature, SPELL_CONSECRATION);
            ConsecrationTimer = 40000;
        }else ConsecrationTimer -= diff;

        if(HammerOfJusticeTimer < diff)
        {
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
            {
                if((m_creature->GetAttackDistance(target) >= 13.889404) && (m_creature->GetAttackDistance(target) <= 42.387451)) // is in 10-40 yd range
                {
                    DoCast(target, SPELL_HAMMER_OF_JUSTICE);
                    HammerOfJusticeTimer = 34000;
                }
            }
        }else HammerOfJusticeTimer -= diff;

        if(SealOfCommandTimer < diff)
        {
            DoCast(m_creature, SPELL_SEAL_OF_COMMAND);
            SealOfCommandTimer = 40000;
        }else SealOfCommandTimer -= diff;

        if(ChromaticAuraTimer < diff)
        {
            DoCast(m_creature, SPELL_CHROMATIC_AURA);
            ChromaticAuraTimer = 90000;
        }else ChromaticAuraTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_high_nethermancer_zerevorAI : public ScriptedAI
{
    boss_high_nethermancer_zerevorAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint64 Council[2];

    uint32 BlizzardTimer;
    uint32 FlamestrikeTimer;
    uint32 ArcaneBoltTimer;
    uint32 Cooldown;

    bool AcquiredGUIDs;
    bool InCombat;

    void SetVariables()
    {
        AcquiredGUIDs = false;
        InCombat = false;

        BlizzardTimer = 30000 + rand()%61 * 1000;
        FlamestrikeTimer = 30000 + rand()%61 * 1000;
        ArcaneBoltTimer = 15000;
        Cooldown = 0;
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }


    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                DoYell(SAY_ZERE_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_ZERE_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {

        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    DoYell(SAY_ZERE_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_ZERE_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        DoYell(SAY_ZERE_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_ZERE_SLAY);
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_ZERE_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_ZERE_DEATH);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage>0)
        {
            if(Cooldown = 0)
            {
                if(m_creature->IsWithinDistInMap(done_by, 8))
                {
                    DoCast(m_creature, SPELL_ARCANE_EXPLOSION);
                    Cooldown = 3000;
                }
            }
        }
        for(uint8 i = 0; i < 3; i++)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit)
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetUnitGUID("GathiosTheShatterer");
                Council[1] = pInstance->GetUnitGUID("LadyMalande");
                Council[2] = pInstance->GetUnitGUID("VerasDarkshadow");
            }
            AcquiredGUIDs = true;
        }

        if(Cooldown >= diff)
            Cooldown -= diff;
        else Cooldown = 0;

        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->StopMoving();

        if(ArcaneBoltTimer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_ARCANE_BOLT);
            ArcaneBoltTimer = 15000;
        }else ArcaneBoltTimer -= diff;

        if(BlizzardTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                DoCast(target, SPELL_BLIZZARD);
                BlizzardTimer = 55000 + rand()%46 * 1000;
                FlamestrikeTimer += 10000;
            }
        }else BlizzardTimer -= diff;

        if(FlamestrikeTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                DoCast(target, SPELL_FLAMESTRIKE);
                FlamestrikeTimer = 55000 + rand()%46 * 1000;
                BlizzardTimer += 10000;
            }
        }else FlamestrikeTimer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_lady_malandeAI : public ScriptedAI
{
    boss_lady_malandeAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint64 Council[2];

    uint32 EmpoweredSmiteTimer;
    uint32 CircleOfHealingTimer;
    uint32 DivineWrathTimer;
    uint32 ReflectiveShieldTimer;
    uint32 BlessingSpellWardTimer;
    uint32 BlessingProtectionTimer;

    bool AcquiredGUIDs;
    bool InCombat;

    void SetVariables()
    {
        AcquiredGUIDs = false;
        InCombat = false;

        EmpoweredSmiteTimer = 38000;
        CircleOfHealingTimer = 20000;
        DivineWrathTimer = 40000;
        ReflectiveShieldTimer = 0;
        BlessingSpellWardTimer = 15000;
        BlessingProtectionTimer = 45000;
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                DoYell(SAY_MALA_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_MALA_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {

        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    DoYell(SAY_MALA_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_MALA_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        DoYell(SAY_MALA_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_MALA_SLAY);
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_MALA_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_MALA_DEATH);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {          
        for(uint8 i = 0; i < 3; i++)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit)
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetUnitGUID("GathiosTheShatterer");
                Council[1] = pInstance->GetUnitGUID("HighNethermancerZerevor");
                Council[2] = pInstance->GetUnitGUID("VerasDarkshadow");
            }
            AcquiredGUIDs = true;
        }

        if(BlessingSpellWardTimer < diff)
        {
            DoCast(m_creature, SPELL_BLESS_SPELLWARD);
            BlessingSpellWardTimer = 60000;
        }else BlessingSpellWardTimer -= diff;

        if(BlessingProtectionTimer < diff)
        {
            DoCast(m_creature, SPELL_BLESS_PROTECTION);
            BlessingProtectionTimer = 60000;
        }else BlessingProtectionTimer -= diff;

        if(EmpoweredSmiteTimer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_EMPOWERED_SMITE);
            EmpoweredSmiteTimer = 38000;
        }else EmpoweredSmiteTimer -= diff;

        if(CircleOfHealingTimer < diff)
        {
            DoCast(m_creature, SPELL_CIRCLE_OF_HEALING);
            CircleOfHealingTimer = 30000 + rand()%46 * 1000;
        }else CircleOfHealingTimer -= diff;

        if(DivineWrathTimer < diff)
        {
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
            {
                DoCast(target, SPELL_DIVINE_WRATH);
                DivineWrathTimer = 40000 + rand()%41 * 1000;
            }
        }else DivineWrathTimer -= diff;

        if(ReflectiveShieldTimer < diff)
        {
            DoCast(m_creature, SPELL_REFLECTIVE_SHIELD);
            ReflectiveShieldTimer = 65000;
        }else ReflectiveShieldTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_veras_darkshadowAI : public ScriptedAI
{
    boss_veras_darkshadowAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint64 Council[2];

    uint64 EnvenomTargetGUID;

    uint32 DeadlyPoisonTimer;
    uint32 VanishTimer;
    uint32 AppearEnvenomTimer;

    bool AcquiredGUIDs;
    bool InCombat;
    bool HasVanished;

    void SetVariables()
    {
        AcquiredGUIDs = false;
        InCombat = false;

        EnvenomTargetGUID = 0;

        DeadlyPoisonTimer = 20000;
        VanishTimer = 60000 + rand()%61 * 1000;
        AppearEnvenomTimer = 150000;

        HasVanished = false;
        (*m_creature).GetMotionMaster()->Clear(false);
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                DoYell(SAY_VERA_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_VERA_AGGRO);
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {

        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    DoYell(SAY_VERA_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_VERA_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        DoYell(SAY_VERA_SLAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_VERA_SLAY);
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_VERA_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_VERA_DEATH);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {          
        for(uint8 i = 0; i < 3; i++)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit)
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void ResetThreat()
    {
        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
        {
            Unit* pUnit = NULL;
            pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit)
            {
                m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetUnitGUID("GathiosTheShatterer");
                Council[1] = pInstance->GetUnitGUID("HighNethermancerZerevor");
                Council[2] = pInstance->GetUnitGUID("VerasDarkshadow");
            }
            AcquiredGUIDs = true;
        }

        if(!HasVanished)
        {
            if(DeadlyPoisonTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_DEADLY_POISON);
                DeadlyPoisonTimer = 20000 + rand()%31 * 1000;
            }else DeadlyPoisonTimer -= diff;

            /*
            if(VanishTimer < diff)
            {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
            {
            ResetThreat();
            m_creature->getThreatManager().tauntApply(target);
            }
            DoCast(m_creature, SPELL_VANISH);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->Idle();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            VanishTimer = 30000;
            AppearEnvenomTimer= 27000;
            HasVanished = true;
            }else VanishTimer -= diff;

            if(AppearEnvenomTimer < diff)
            {
            DoCast(m_creature->getVictim(), SPELL_ENVENOM);
            AppearEnvenomTimer = 90000;
            }else AppearEnvenomTimer -= diff;*/
        }
        else
        {
            if(VanishTimer < diff)
            {
                (*m_creature).GetMotionMaster()->Clear(false);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->getThreatManager().tauntApply(m_creature->getVictim());
                DoCast(m_creature->getVictim(), SPELL_DEADLY_POISON);
                DeadlyPoisonTimer += 6000;
                VanishTimer = 90000;
                AppearEnvenomTimer = 4000;
                HasVanished = false;
            }else VanishTimer -= diff;

            if(AppearEnvenomTimer < diff)
            {
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 21418);
                AppearEnvenomTimer = 7000;
            }else AppearEnvenomTimer -= diff;
        }
        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_illidari_council(Creature *_Creature)
{
    return new boss_illidari_councilAI (_Creature);
}

CreatureAI* GetAI_boss_gathios_the_shatterer(Creature *_Creature)
{
    return new boss_gathios_the_shattererAI (_Creature);
}

CreatureAI* GetAI_boss_lady_malande(Creature *_Creature)
{
    return new boss_lady_malandeAI (_Creature);
}

CreatureAI* GetAI_boss_veras_darkshadow(Creature *_Creature)
{
    return new boss_veras_darkshadowAI (_Creature);
}

CreatureAI* GetAI_boss_high_nethermancer_zerevor(Creature *_Creature)
{
    return new boss_high_nethermancer_zerevorAI (_Creature);
}

void AddSC_boss_illidari_council()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="illidari_council";
    newscript->GetAI = GetAI_boss_illidari_council;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_gathios_the_shatterer";
    newscript->GetAI = GetAI_boss_gathios_the_shatterer;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_lady_malande";
    newscript->GetAI = GetAI_boss_lady_malande;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_veras_darkshadow";
    newscript->GetAI = GetAI_boss_veras_darkshadow;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_high_nethermancer_zerevor";
    newscript->GetAI = GetAI_boss_high_nethermancer_zerevor;
    m_scripts[nrscripts++] = newscript;
}

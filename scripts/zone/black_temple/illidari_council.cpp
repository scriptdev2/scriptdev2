/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Illidari_Council
SD%Complete: 90
SDComment: Workarounds for Spellstealing Dampen Magic and Circle of Healing.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "def_black_temple.h"

// High Nethermancer Zerevor's spells
#define SPELL_FLAMESTRIKE          41481
#define SPELL_BLIZZARD             41482
#define SPELL_ARCANE_BOLT          41483
#define SPELL_ARCANE_EXPLOSION     41524
#define SPELL_DAMPEN_MAGIC         41478

// Lady Malande's spells
#define SPELL_EMPOWERED_SMITE      41471
#define SPELL_CIRCLE_OF_HEALING    41455
#define SPELL_REFLECTIVE_SHIELD    41475
#define SPELL_DIVINE_WRATH         41472
#define SPELL_HEAL_VISUAL          24171

// Gathios the Shatterer's spells
#define SPELL_BLESS_PROTECTION     41450
#define SPELL_BLESS_SPELLWARD      41451
#define SPELL_CONSECRATION         41541
#define SPELL_HAMMER_OF_JUSTICE    41468
#define SPELL_SEAL_OF_COMMAND      41469
#define SPELL_SEAL_OF_BLOOD        41459
#define SPELL_CHROMATIC_AURA       41453
#define SPELL_DEVOTION_AURA        41452

// Veras Darkshadow's spells
#define SPELL_DEADLY_POISON        41485
#define SPELL_ENVENOM              41487
#define SPELL_VANISH               41479

#define SPELL_BERSERK              45078

//Speech'n'Sounds
//Speech'n'Sounds
#define SAY_GATH_AGGRO            "I have better things to do!"
#define SOUND_GATH_AGGRO          11422
#define SAY_GATH_SLAY             "Selama am'oronor!"
#define SOUND_GATH_SLAY           11423
#define SAY_GATH_COMNT            "Well done!"
#define SOUND_GATH_COMNT          11424
#define SAY_GATH_DEATH            "Lord Illidan... I..."
#define SOUND_GATH_DEATH          11425
#define SAY_GATH_SPECIAL1         "Enjoy your final moments!"
#define SOUND_GATH_SPECIAL1       11426
#define SAY_GATH_SPECIAL2         "You are mine!"
#define SOUND_GATH_SPECIAL2       11427

#define SAY_MALA_AGGRO            "Flee, or die!"
#define SOUND_MALA_AGGRO          11482
#define SAY_MALA_SLAY             "My work is done."
#define SOUND_MALA_SLAY           11483
#define SAY_MALA_COMNT            "As it should be!"
#define SOUND_MALA_COMNT          11484
#define SAY_MALA_DEATH            "Destiny... awaits."
#define SOUND_MALA_DEATH          11485
#define SAY_MALA_SPECIAL1         "No second chances!"
#define SOUND_MALA_SPECIAL1       11486
#define SAY_MALA_SPECIAL2         "I'm full of surprises!"
#define SOUND_MALA_SPECIAL2       11487

#define SAY_ZERE_AGGRO            "Common... such a crude language. Bandal!"
#define SOUND_ZERE_AGGRO          11440
#define SAY_ZERE_SLAY             "Shorel'aran."
#define SOUND_ZERE_SLAY           11441
#define SAY_ZERE_COMNT            "Belesa menoor!"
#define SOUND_ZERE_COMNT          11442
#define SAY_ZERE_DEATH            "Diel ma'ahn... oreindel'o"
#define SOUND_ZERE_DEATH          11443
#define SAY_ZERE_SPECIAL1         "Diel fin'al"
#define SOUND_ZERE_SPECIAL1       11444
#define SAY_ZERE_SPECIAL2         "Sha'amoor ara mashal?"
#define SOUND_ZERE_SPECIAL2       11445

#define SAY_VERA_AGGRO            "You wish to test me?"
#define SOUND_VERA_AGGRO          11524
#define SAY_VERA_SLAY             "Valiant effort!"
#define SOUND_VERA_SLAY           11525
#define SAY_VERA_COMNT            "A glorious kill!"
#define SOUND_VERA_COMNT          11526
#define SAY_VERA_DEATH            "You got lucky!"
#define SOUND_VERA_DEATH          11527
#define SAY_VERA_SPECIAL1         "You're not caught up for this!"
#define SOUND_VERA_SPECIAL1       11528
#define SAY_VERA_SPECIAL2         "Anar'alah belore!"
#define SOUND_VERA_SPECIAL2       11529

#define ERROR_INST_DATA             "SD2 ERROR: Instance Data for Black Temple not set properly; abort Illidari Council event."
struct Yells
{
    char* text;
    uint32 soundId, timer;
};

static Yells CouncilEnrage[]=
{
    {"For Quel'Thalas! For the Sunwell!", 11488}, // Malande
    {"Sha'amoor sine menoor!", 11446}, // Zerevor
    {"Enough games!", 11428}, // Gathios
    {"You wish to kill me? Hahaha, you first!", 11530}, // Veras
};

struct MANGOS_DLL_DECL boss_illidari_councilAI : public ScriptedAI
{
    boss_illidari_councilAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        for(uint8 i = 0; i < 4; ++i)
            Council[i] = 0;

        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 Council[4];

    uint32 CheckTimer;
    uint32 EnrageTimer;

    bool EventBegun;

    void Reset()
    {     
        CheckTimer = 2000;
        EnrageTimer = 900000; // 15 minutes

        Creature* Member;
        for(uint8 i = 0; i < 4; i++)
        {
            if(Council[i])
            {
                Member = ((Creature*)Unit::GetUnit((*m_creature), Council[i]));
                if(Member)
                {
                    if(!Member->isAlive())
                    {
                        Member->RemoveCorpse();
                        Member->Respawn();
                    }
                    Member->AI()->EnterEvadeMode();
                }
            }
        }

        if(pInstance)
            pInstance->SetData(DATA_ILLIDARICOUNCILEVENT, NOT_STARTED);

        EventBegun = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
    }

    void Aggro(Unit *who)  
    {
       StartEvent(who);
    }

    void AttackStart(Unit* who) {}

    void MoveInLineOfSight(Unit *who)
    {
        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                   if(!EventBegun)
                       StartEvent(who);
            }
        }
    }

    void StartEvent(Unit *target)
    {
        if(!pInstance) return;

        outstring_log("IC Event Begun - target: %s", target->GetName());
        if(target && target->isAlive())
        {
            Council[0] = pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
            Council[1] = pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
            Council[2] = pInstance->GetData64(DATA_LADYMALANDE);
            Council[3] = pInstance->GetData64(DATA_VERASDARKSHADOW);

            for(uint8 i = 0; i < 4; i++)
            {
                Unit* Member = NULL;
                if(Council[i])
                {
                    Member = Unit::GetUnit((*m_creature), Council[i]);
                    if(Member && Member->isAlive())
                        Member->AddThreat(target, 1.0f);
                }
            }

            pInstance->SetData(DATA_ILLIDARICOUNCILEVENT, IN_PROGRESS);

            EventBegun = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!EventBegun) return;

        if(CheckTimer < diff)
        {
            Creature* Gathios = NULL;
            Creature* Zerevor = NULL;
            Creature* Malande = NULL;
            Creature* Veras = NULL;

            if(Council[0])
                Gathios = ((Creature*)Unit::GetUnit((*m_creature), Council[0]));
            if(Council[1])
                Zerevor = ((Creature*)Unit::GetUnit((*m_creature), Council[1]));
            if(Council[2])
                Malande = ((Creature*)Unit::GetUnit((*m_creature), Council[2]));
            if(Council[3])
                Veras = ((Creature*)Unit::GetUnit((*m_creature), Council[3]));
            
            // Don't allow players to pull one of the council's members, aggro another members' target if none present
            if(Gathios && Gathios->isAlive() && !Gathios->SelectHostilTarget() && Veras)
                Gathios->AddThreat(Veras->getVictim(), 1.0f);

            if(Zerevor && Zerevor->isAlive() && !Zerevor->SelectHostilTarget() && Gathios)
                Zerevor->AddThreat(Gathios->getVictim(), 1.0f);

            if(Malande && Malande->isAlive() && !Malande->SelectHostilTarget() && Zerevor)
                Malande->AddThreat(Zerevor->getVictim(), 1.0f);

            if(Veras && Veras->isAlive() && !Veras->SelectHostilTarget() && Malande)
                Veras->AddThreat(Malande->getVictim(), 1.0f);

            uint8 EvadeCheck = 0;
            uint8 DeathCheck = 0;
            for(uint8 i = 0; i < 4; ++i)
            {
                if(Council[i])
                {
                    Creature* Member = ((Creature*)Unit::GetUnit((*m_creature), Council[i]));
                    if(Member)
                    {
                        // This is the evade/death check. 
                        if(Member->isAlive() && !Member->SelectHostilTarget())
                            EvadeCheck++; //If all members evade, we reset so that players can properly reset the event
                        else if(!Member->isAlive())
                            DeathCheck++; // If all members are dead, set instance data and suicide.
                    }
                    else DeathCheck++;
                }
            }

            if(EvadeCheck > 3)
                Reset();

            if(DeathCheck > 3)
            {                
                if(pInstance)
                    pInstance->SetData(DATA_ILLIDARICOUNCILEVENT, DONE); // Completed

                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            CheckTimer = 2000;
        }else CheckTimer -= diff;

        if(EnrageTimer < diff)
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                Unit* Member = Unit::GetUnit((*m_creature), Council[i]);
                if(Member && Member->isAlive() && Member->SelectHostilTarget() && !Member->HasAura(SPELL_BERSERK, 0))
                {
                    Member->CastSpell(Member, SPELL_BERSERK, true);
                    char* text = CouncilEnrage[i].text;
                    uint32 sound = CouncilEnrage[i].soundId;
                    if(text)
                        Member->MonsterYell(text, LANG_UNIVERSAL, 0);
                    if(sound)
                        DoPlaySoundToSet(Member, sound);
                    EnrageTimer = 4000;
                    return; // Return so it exits the loop
                }
            }
        }else EnrageTimer -= diff;
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
        Reset();
    }

    uint64 Council[3];

    ScriptedInstance* pInstance;

    uint32 ConsecrationTimer;
    uint32 HammerOfJusticeTimer;
    uint32 SealTimer;
    uint32 AuraTimer;
    uint32 BlessingTimer;

    bool AcquiredGUIDs;

    void Reset()
    {
        AcquiredGUIDs = false;

        ConsecrationTimer = 40000;
        HammerOfJusticeTimer = 10000;
        SealTimer = 40000;
        AuraTimer = 90000;
        BlessingTimer = 60000;
    }

    void Aggro(Unit *who)
    {
        if(pInstance)
        {
           Creature* Controller = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ILLIDARICOUNCIL)));
           if(Controller)
               Controller->AddThreat(who, 1.0f);
        }
        else
        {
            error_log(ERROR_INST_DATA);
            EnterEvadeMode();
        }
        DoZoneInCombat();
        DoYell(SAY_GATH_AGGRO,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature, SOUND_GATH_AGGRO);
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
        damage /= 4;
        for(uint8 i = 0; i < 3; ++i)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit && (damage < pUnit->GetHealth()))
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    Unit* SelectCouncilMember()
    {       
        Unit* pUnit = NULL;
        uint32 member = 1;
        if(rand()%10 == 0)
        {
            switch(rand()%3)
            {
                case 0:
                    member = 0;
                    break;
                case 1:
                    member = 1;
                    break;
                case 2:
                    member = 5;
                    break;
            }
        }
        if(member != 5)
            pUnit = Unit::GetUnit((*m_creature), Council[member]);  
        return pUnit;
    }

    void CastAuraOnCouncil()
    {
        uint32 spellid = 0;
        switch(rand()%2)
        {
            case 0:
                spellid = SPELL_DEVOTION_AURA;
                break;
            case 1:
                spellid = SPELL_CHROMATIC_AURA;
                break;
        }
        for(uint8 i = 0; i < 3; i++)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), Council[i]);
            if(pUnit)
                pUnit->CastSpell(pUnit, spellid, true, 0, 0, m_creature->GetGUID());
        }
        DoCast(m_creature, spellid);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
                Council[1] = pInstance->GetData64(DATA_LADYMALANDE);
                Council[2] = pInstance->GetData64(DATA_VERASDARKSHADOW);
            }
            AcquiredGUIDs = true;
        }
        
        if(BlessingTimer < diff)
        {
            Unit* pUnit = SelectCouncilMember();
            if(pUnit)
            {
                switch(rand()%2)
                {
                    case 0:
                        DoCast(pUnit, SPELL_BLESS_SPELLWARD);
                        break;
                    case 1:
                        DoCast(pUnit, SPELL_BLESS_PROTECTION);
                        break;
                }
            }
            BlessingTimer = 60000;
        }else BlessingTimer -= diff;
        
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
                if(m_creature->GetDistance2d(target) > 10 && m_creature->GetDistance2d(target) < 40) // is in ~10-40 yd range
                {
                    DoCast(target, SPELL_HAMMER_OF_JUSTICE);
                    HammerOfJusticeTimer = 20000;
                }
            }
        }else HammerOfJusticeTimer -= diff;

        if(SealTimer < diff)
        {
            switch(rand()%2)
            {
                case 0:
                    DoCast(m_creature, SPELL_SEAL_OF_COMMAND);
                    break;
                case 1:
                    DoCast(m_creature, SPELL_SEAL_OF_BLOOD);
                    break;
            }
            SealTimer = 40000;
        }else SealTimer -= diff;

        if(AuraTimer < diff)
        {
            CastAuraOnCouncil();
            AuraTimer = 90000;
        }else AuraTimer -= diff;

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
        Reset();                                       
    }

    ScriptedInstance* pInstance;

    uint64 Council[3];

    uint32 BlizzardTimer;
    uint32 FlamestrikeTimer;
    uint32 ArcaneBoltTimer;
    uint32 DampenMagicTimer;
    uint32 Cooldown;
    uint32 AggroYellTimer;
    uint32 ArcaneExplosionTimer;

    bool AcquiredGUIDs;

    void Reset()
    {
        AcquiredGUIDs = false;

        BlizzardTimer = 30000 + rand()%61 * 1000;
        FlamestrikeTimer = 30000 + rand()%61 * 1000;
        ArcaneBoltTimer = 10000;
        DampenMagicTimer = 2000;
        ArcaneExplosionTimer = 14000;
        Cooldown = 0;
        AggroYellTimer = 0;
    }

    void Aggro(Unit *who)
    {
        if(pInstance)
        {
           Creature* Controller = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ILLIDARICOUNCIL)));
           if(Controller)
               Controller->AddThreat(who, 1.0f);
        }
        else
        {
            error_log(ERROR_INST_DATA);
            EnterEvadeMode();
        }

        DoZoneInCombat();
        AggroYellTimer = 9000;
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
        damage /= 4;
        for(uint8 i = 0; i < 3; ++i)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit && (damage < pUnit->GetHealth()))
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(AggroYellTimer)
        {
            if(AggroYellTimer < diff)
            {
                DoYell(SAY_ZERE_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_ZERE_AGGRO);
                AggroYellTimer = 0;
            }else AggroYellTimer -= diff;
        }

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
                Council[1] = pInstance->GetData64(DATA_LADYMALANDE);
                Council[2] = pInstance->GetData64(DATA_VERASDARKSHADOW);
            }
            AcquiredGUIDs = true;
        }

        if(Cooldown)
        {
            if(Cooldown < diff) Cooldown = 0;
            else
            {
                Cooldown -= diff;
                return; // Don't cast any other spells if global cooldown is still ticking
            }
        }

        if(DampenMagicTimer < diff)
        {
            DoCast(m_creature, SPELL_DAMPEN_MAGIC);
            Cooldown = 1000;
            DampenMagicTimer = 120000; // ~ 2 minutes
            ArcaneBoltTimer += 1000;
        }else DampenMagicTimer -= diff;
        
        if(ArcaneExplosionTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            Cooldown = 1000;
            ArcaneExplosionTimer = 14000;
        }else ArcaneExplosionTimer -= diff;

        if(ArcaneBoltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_BOLT);
            ArcaneBoltTimer = 3000;
            Cooldown = 2000;
        }else ArcaneBoltTimer -= diff;

        if(BlizzardTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                DoCast(target, SPELL_BLIZZARD);
                BlizzardTimer = 45000 + rand()%46 * 1000;
                FlamestrikeTimer += 10000;
                Cooldown = 1000;
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
                Cooldown = 2000;
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
        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 Council[3];

    uint32 EmpoweredSmiteTimer;
    uint32 CircleOfHealingTimer;
    uint32 DivineWrathTimer;
    uint32 ReflectiveShieldTimer;
    uint32 AggroYellTimer;

    bool AcquiredGUIDs;

    void Reset()
    {
        AcquiredGUIDs = false;

        EmpoweredSmiteTimer = 38000;
        CircleOfHealingTimer = 20000;
        DivineWrathTimer = 40000;
        ReflectiveShieldTimer = 0;
        AggroYellTimer = 0;
    }

    void Aggro(Unit *who)
    {
        if(pInstance)
        {
           Creature* Controller = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ILLIDARICOUNCIL)));
           if(Controller)
               Controller->AddThreat(who, 1.0f);
        }
        else
        {
            error_log(ERROR_INST_DATA);
            EnterEvadeMode();
        }

        DoZoneInCombat();
        AggroYellTimer = 5500;
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
        damage /= 4;
        for(uint8 i = 0; i < 3; ++i)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit && (damage < pUnit->GetHealth()))
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(AggroYellTimer)
        {
            if(AggroYellTimer < diff)
            {
                DoYell(SAY_MALA_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_MALA_AGGRO);
                AggroYellTimer = 0;
            }else AggroYellTimer -= diff;
        }

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
                Council[1] = pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
                Council[2] = pInstance->GetData64(DATA_VERASDARKSHADOW);
            }
            AcquiredGUIDs = true;
        }

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
            // Currently bugged and puts Malande on the threatlist of the other council members. It also heals players.
//            DoCast(m_creature, SPELL_CIRCLE_OF_HEALING);
            CircleOfHealingTimer = 60000;
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
        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 Council[3];

    uint64 EnvenomTargetGUID;

    uint32 DeadlyPoisonTimer;
    uint32 VanishTimer;
    uint32 AppearEnvenomTimer;
    uint32 AggroYellTimer;

    bool AcquiredGUIDs;
    bool HasVanished;

    void Reset()
    {
        AcquiredGUIDs = false;

        EnvenomTargetGUID = 0;

        DeadlyPoisonTimer = 20000;
        VanishTimer = 60000 + rand()%61 * 1000;
        AppearEnvenomTimer = 150000;
        AggroYellTimer = 0;

        HasVanished = false;
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit *who)
    {
        if(pInstance)
        {
           Creature* Controller = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_ILLIDARICOUNCIL)));
           if(Controller)
               Controller->AddThreat(who, 1.0f);
        }
        else
        {
            error_log(ERROR_INST_DATA);
            EnterEvadeMode();
        }

        DoZoneInCombat();
        AggroYellTimer = 3000;
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
                    AggroYellTimer = 3000;
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
        damage /= 4;
        for(uint8 i = 0; i < 3; ++i)
        {
            Unit* pUnit = NULL;
            if(Council[i])
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), Council[i]);
                if(pUnit && (damage < pUnit->GetHealth()))
                    pUnit->SetHealth(pUnit->GetHealth() - damage);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            
        if(AggroYellTimer)
        {
            if(AggroYellTimer < diff)
            {
                DoYell(SAY_VERA_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_VERA_AGGRO);
                AggroYellTimer = 0;
            }else AggroYellTimer -= diff;
        }

        if(!AcquiredGUIDs)
        {
            if(pInstance)
            {
                Council[0] = pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
                Council[1] = pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
                Council[2] = pInstance->GetData64(DATA_LADYMALANDE);
            }
            AcquiredGUIDs = true;
        }
 
        if(!HasVanished)
        {
            if(DeadlyPoisonTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_DEADLY_POISON);
                DeadlyPoisonTimer = 15000 + rand()%31 * 1000;
            }else DeadlyPoisonTimer -= diff;

            if(AppearEnvenomTimer < diff) // Cast Envenom. This is cast 4 seconds after Vanish is over
            {
                DoCast(m_creature->getVictim(), SPELL_ENVENOM);
                AppearEnvenomTimer = 90000;
            }else AppearEnvenomTimer -= diff;

            if(VanishTimer < diff) // Disappear and stop attacking, but follow a random unit
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                {        
                    VanishTimer = 30000;
                    AppearEnvenomTimer= 28000;
                    HasVanished = true;
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoResetThreat();
                    m_creature->AddThreat(target, 500000.0f); // Chase a unit. Check before DoMeleeAttackIfReady prevents from attacking
                    m_creature->GetMotionMaster()->MoveChase(target);
                }
            }else VanishTimer -= diff;

            DoMeleeAttackIfReady();
        }
        else
        {
            if(VanishTimer < diff) // Become attackable and poison current target
            {
                DoCast(m_creature->getVictim(), SPELL_DEADLY_POISON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoResetThreat();
                m_creature->AddThreat(m_creature->getVictim(), 3000.0f); // Make Veras attack his target for a while, he will cast Envenom 4 seconds after.
                DeadlyPoisonTimer += 6000;
                VanishTimer = 90000;
                AppearEnvenomTimer = 4000;
                HasVanished = false;
            }else VanishTimer -= diff;

            if(AppearEnvenomTimer < diff) // Appear 2 seconds before becoming attackable (Shifting out of vanish)
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->SetVisibility(VISIBILITY_ON);
                AppearEnvenomTimer = 6000;
            }else AppearEnvenomTimer -= diff;
        }
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

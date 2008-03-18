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
SD%Complete: 85
SDComment: Workarounds for Spellstealing Dampen Magic and Circle of Healing. Need more details on the enrage.
SDCategory: Black Temple
EndScriptData */

#include "../../sc_defines.h"
#include "def_black_temple.h"
#include "../../../../../game/TargetedMovementGenerator.h"

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
#define SAY_GATH_ENRAGE           "Enough games!"
#define SOUND_GATH_ENRAGE         11428

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
#define SAY_MALA_ENRAGE           "For Quel'Thalas! For the Sunwell!"
#define SOUND_MALA_ENRAGE         11488

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
#define SAY_ZERE_ENRAGE           "Sha'amoor sine menoor!"
#define SOUND_ZERE_ENRAGE         11446

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
#define SAY_VERA_ENRAGE           "You wish to kill me? Hahaha, you first!"
#define SOUND_VERA_ENRAGE         11530

struct MANGOS_DLL_DECL boss_illidari_councilAI : public ScriptedAI
{
    boss_illidari_councilAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());

        Council[0] = 0;
        Council[1] = 0;
        Council[2] = 0;
        Council[3] = 0;

        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 Council[4];

    uint32 CheckTimer;

    bool InCombat;

    void Reset()
    {     
        InCombat = false;
        CheckTimer = 2000;

        Creature* Member;
        for(uint8 i = 0; i < 3; i++)
        {
            if(Council[i])
            {
                Member = ((Creature*)Unit::GetUnit((*m_creature), Council[i]));
                if(Member)
                {
                    ((Creature*)Member)->Respawn();
                    ((Creature*)Member)->AI()->EnterEvadeMode();
                }
            }
        }
        if(pInstance)
            pInstance->SetData(DATA_ILLIDARICOUNCILEVENT,0);

        InCombat = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
    }

    void AttackStart(Unit *who)
    {
        return;
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
                        pInstance->SetData(DATA_ILLIDARICOUNCIL, 1);

                    StartEvent(who);
                    InCombat = true;
                }
            }
        }
    }

    void StartEvent(Unit *target)
    {
        if(!pInstance) return;

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

            pInstance->SetData(DATA_ILLIDARICOUNCILEVENT, 1);
        }
    }
    void UpdateAI(const uint32 diff)
    {
        if(!InCombat) return;

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
            
            //if((!Gathios || !Gathios->isAlive()) && (!Zerevor || !Zerevor->isAlive()) && (!Malande || !Malande->isAlive()) && (!Veras |!Veras->isAlive()))
            //{
            //    if(pInstance)
            //        pInstance->SetData(DATA_ILLIDARICOUNCILEVENT, 3); // Completed

            //    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
            //}

            // Don't allow players to pull one of the council's members, aggro another members' target if none present
            if(Gathios && Gathios->isAlive() && !Gathios->SelectHostilTarget())
                Gathios->AddThreat(Veras->getVictim(), 1.0f);

            if(Zerevor && Zerevor->isAlive() && !Zerevor->SelectHostilTarget())
                Zerevor->AddThreat(Gathios->getVictim(), 1.0f);

            if(Malande && Malande->isAlive() && !Malande->SelectHostilTarget())
                Malande->AddThreat(Zerevor->getVictim(), 1.0f);

            if(Veras && Veras->isAlive() && !Veras->SelectHostilTarget())
                Veras->AddThreat(Malande->getVictim(), 1.0f);

            uint8 EvadeCheck = 0;
            uint8 DeathCheck = 0;
            for(uint8 i = 0; i < 4; i++)
            {
                if(Council[i])
                {
                    Creature* Member = ((Creature*)Unit::GetUnit((*m_creature), Council[i]));
                    if(Member)
                    {
                        if(Member->isAlive() && !Member->SelectHostilTarget())
                            EvadeCheck++;
                        else if(!Member->isAlive())
                            DeathCheck++;
                    }
                    else DeathCheck++;
                }
            }

            if(EvadeCheck > 3)
                Reset();

            if(DeathCheck > 3)
            {                
                if(pInstance)
                    pInstance->SetData(DATA_ILLIDARICOUNCILEVENT, 3); // Completed

                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
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
    bool InCombat;

    void Reset()
    {
        AcquiredGUIDs = false;

        ConsecrationTimer = 40000;
        HammerOfJusticeTimer = 10000;
        SealTimer = 40000;
        AuraTimer = 90000;
        BlessingTimer = 60000;

        InCombat = false;

        if(pInstance)
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 0);
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
                Reset();
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
        InCombat = false;
        DoYell(SAY_GATH_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_GATH_DEATH);
        if(pInstance)
        {
            if(pInstance->GetData(DATA_COUNCIL_DEATH_COUNT) < 3)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 1);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 1)
        {    
            damage /= 4;
            for(uint8 i = 0; i < 3; i++)
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
                if(m_creature->GetDistance2dSq(target) > 196 && m_creature->GetDistance2dSq(target) < 1764) // is in ~10-40 yd range
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
    bool InCombat;

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

        InCombat = false;

        if(pInstance)
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 0);
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
                Reset();
                AggroYellTimer = 9000;
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
                    AggroYellTimer = 9000;
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
        InCombat = false;

        DoYell(SAY_ZERE_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_ZERE_DEATH);

        if(pInstance)
        {
            if(pInstance->GetData(DATA_COUNCIL_DEATH_COUNT) < 3)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); // Only one of us can be lootable.
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 1);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 1)
        {    
            damage /= 4;
            for(uint8 i = 0; i < 3; i++)
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
    }

    // THIS IS A WORKAROUND AS SPELLSTEAL DOES NOT WORK/DOES NOT ALLOW PLAYERS TO STEAL BUFFS OFF MOBS.
    void CastDampenMagicWorkaround()
    {
        Unit* target = m_creature->getVictim();

        if(target && (target->GetTypeId() == TYPEID_PLAYER))
        {
            if((target->getClass() == CLASS_MAGE) && (!target->HasAura(SPELL_DAMPEN_MAGIC, 0)))
            {
                target->CastSpell(target, SPELL_DAMPEN_MAGIC, true);
                target->SetPower(POWER_MANA, target->GetPower(POWER_MANA) + 600);
            }
        }
        else DoCast(m_creature, SPELL_DAMPEN_MAGIC);
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
            CastDampenMagicWorkaround();
            Cooldown = 1000;
            DampenMagicTimer = 5000; // Should be ~2 minutes.
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
    bool InCombat;

    void Reset()
    {
        AcquiredGUIDs = false;

        EmpoweredSmiteTimer = 38000;
        CircleOfHealingTimer = 20000;
        DivineWrathTimer = 40000;
        ReflectiveShieldTimer = 0;
        AggroYellTimer = 0;

        InCombat = false;
        
        if(pInstance)
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 0);
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
                Reset();
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
                    AggroYellTimer = 5500;
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
        InCombat = false;

        DoYell(SAY_MALA_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_MALA_DEATH);

        if(pInstance)
        {
            if(pInstance->GetData(DATA_COUNCIL_DEATH_COUNT) < 3)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); // Unlootable unless we are the last to die
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 1);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {          
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 1)
        {    
            damage /= 4;
            for(uint8 i = 0; i < 3; i++)
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
    }

    /** This is a workaround as Circle of Healing puts Malande in the threat list of others.
    Please remove this workaround when her Circle of Healing works properly */
    void DoCircleOfHealingWorkaround()
    {
        uint32 heal;
        for(uint8 i = 0; i < 3; i++)
        {
            heal = 5000 + rand()%5001; // 5 - 10k health healed
            Unit* pUnit = NULL;
            pUnit = Unit::GetUnit((*m_creature), Council[i]);
            if(pUnit && ((pUnit->GetHealth()*100 / pUnit->GetMaxHealth()) > 1))
            {
                pUnit->SetHealth(pUnit->GetHealth() + heal);
                pUnit->CastSpell(pUnit, SPELL_HEAL_VISUAL, true);
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
            DoCircleOfHealingWorkaround(); // Workaround till the actual spell works.
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
    bool InCombat;
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

        InCombat = false;

        if(pInstance)
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 0);
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
                Reset();
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
                    AggroYellTimer = 3000;
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
        InCombat = false;

        DoYell(SAY_VERA_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_VERA_DEATH);

        if(pInstance)
        {
            if(pInstance->GetData(DATA_COUNCIL_DEATH_COUNT) < 3)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE); // Unlootable if others are still alive
            pInstance->SetData(DATA_COUNCIL_DEATH_COUNT, 1);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {   
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 1)
        {    
            damage /= 4;
            for(uint8 i = 0; i < 3; i++)
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
                (*m_creature).GetMotionMaster()->Clear(false);
                DoCast(m_creature->getVictim(), SPELL_ENVENOM);
                AppearEnvenomTimer = 90000;
            }else AppearEnvenomTimer -= diff;

            if(VanishTimer < diff) // Disappear and stop attacking, but follow a random unit
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                (*m_creature).GetMotionMaster()->Clear();
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target)
                {
                    ResetThreat();
                    m_creature->AddThreat(target, 500000.0f); // Chase a unit. Check before DoMeleeAttackIfReady prevents from attacking
                   (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*target));
                }
                VanishTimer = 30000;
                AppearEnvenomTimer= 28000;
                HasVanished = true;
                m_creature->SetVisibility(VISIBILITY_OFF);
            }else VanishTimer -= diff;
            DoMeleeAttackIfReady();
        }
        else
        {
            if(VanishTimer < diff) // Become attackable and poison current target
            {
                DoCast(m_creature->getVictim(), SPELL_DEADLY_POISON);
                (*m_creature).GetMotionMaster()->Clear();
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                ResetThreat();
                m_creature->AddThreat(m_creature->getVictim(), 3000.0f); // Make Veras attack his target for a while, he will cast Envenom 4 seconds after.
                DeadlyPoisonTimer += 6000;
                VanishTimer = 90000;
                AppearEnvenomTimer = 4000;
                HasVanished = false;
            }else VanishTimer -= diff;

            if(AppearEnvenomTimer < diff) // Appear 2 seconds before becoming attackable (Shifting out of vanish)
            {
                (*m_creature).GetMotionMaster()->Clear();
                (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
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

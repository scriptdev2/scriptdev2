/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDComment: Circle of Healing not working properly.
SDCategory: Black Temple
EndScriptData */

#include "precompiled.h"
#include "black_temple.h"

//Speech'n'Sounds
#define SAY_GATH_SLAY           -1564085
#define SAY_GATH_SLAY_COMNT     -1564089
#define SAY_GATH_DEATH          -1564093
#define SAY_GATH_SPECIAL1       -1564077
#define SAY_GATH_SPECIAL2       -1564081

#define SAY_VERA_SLAY           -1564086
#define SAY_VERA_COMNT          -1564089
#define SAY_VERA_DEATH          -1564094
#define SAY_VERA_SPECIAL1       -1564078
#define SAY_VERA_SPECIAL2       -1564082

#define SAY_MALA_SLAY           -1564087
#define SAY_MALA_COMNT          -1564090
#define SAY_MALA_DEATH          -1564095
#define SAY_MALA_SPECIAL1       -1564079
#define SAY_MALA_SPECIAL2       -1564083

#define SAY_ZERE_SLAY           -1564088
#define SAY_ZERE_COMNT          -1564091
#define SAY_ZERE_DEATH          -1564096
#define SAY_ZERE_SPECIAL1       -1564080
#define SAY_ZERE_SPECIAL2       -1564084

#define ERROR_INST_DATA           "SD2 ERROR: Instance Data for Black Temple not set properly; Illidari Council event will not function properly."

struct CouncilYells
{
    int32 entry;
    uint32 timer;
};

static CouncilYells CouncilAggro[]=
{
    {-1564069, 5000},                                       // Gathios
    {-1564070, 5500},                                       // Veras
    {-1564071, 5000},                                       // Malande
    {-1564072, 0},                                          // Zerevor
};

// Need to get proper timers for this later
static CouncilYells CouncilEnrage[]=
{
    {-1564073, 2000},                                       // Gathios
    {-1564074, 6000},                                       // Veras
    {-1564075, 5000},                                       // Malande
    {-1564076, 0},                                          // Zerevor
};

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

struct MANGOS_DLL_DECL mob_blood_elf_council_voice_triggerAI : public ScriptedAI
{
    mob_blood_elf_council_voice_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        for(uint8 i = 0; i < 4; ++i)
            Council[i] = 0;
        Reset();
    }

    uint64 Council[4];

    uint32 EnrageTimer;
    uint32 AggroYellTimer;

    uint8 YellCounter;                                      // Serves as the counter for both the aggro and enrage yells

    bool EventStarted;

    void Reset()
    {
        EnrageTimer = 900000;                               // 15 minutes
        AggroYellTimer = 500;
        YellCounter = 0;

        EventStarted = false;
    }

    // finds and stores the GUIDs for each Council member using instance data system.
    void LoadCouncilGUIDs()
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)m_creature->GetInstanceData())
        {
            Council[0] = pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
            Council[1] = pInstance->GetData64(DATA_VERASDARKSHADOW);
            Council[2] = pInstance->GetData64(DATA_LADYMALANDE);
            Council[3] = pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
        }else error_log(ERROR_INST_DATA);
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}

    void UpdateAI(const uint32 diff)
    {
        if (!EventStarted)
            return;

        if (YellCounter > 3)
            return;

        if (AggroYellTimer)
        {
            if (AggroYellTimer <= diff)
            {
                if (Creature* pMember = m_creature->GetMap()->GetCreature(Council[YellCounter]))
                {
                    DoScriptText(CouncilAggro[YellCounter].entry, pMember);
                    AggroYellTimer = CouncilAggro[YellCounter].timer;
                }
                ++YellCounter;

                if (YellCounter > 3)
                    YellCounter = 0;                        // Reuse for Enrage Yells
            }else AggroYellTimer -= diff;
        }

        if (EnrageTimer)
        {
            if (EnrageTimer <= diff)
            {
                if (Creature* pMember = m_creature->GetMap()->GetCreature(Council[YellCounter]))
                {
                    pMember->CastSpell(pMember, SPELL_BERSERK, true);
                    DoScriptText(CouncilEnrage[YellCounter].entry, pMember);
                    EnrageTimer = CouncilEnrage[YellCounter].timer;
                }
                ++YellCounter;
            }else EnrageTimer -= diff;
        }
    }
};

struct MANGOS_DLL_DECL mob_illidari_councilAI : public ScriptedAI
{
    mob_illidari_councilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        for(uint8 i = 0; i < 4; ++i)
            Council[i] = 0;

        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 Council[4];

    uint32 CheckTimer;
    uint32 EndEventTimer;

    uint8 DeathCount;

    bool EventBegun;

    void Reset()
    {
        CheckTimer    = 2000;
        EndEventTimer = 0;

        DeathCount = 0;

        for(uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pMember = m_creature->GetMap()->GetCreature(Council[i]))
            {
                if (!pMember->isAlive())
                {
                    pMember->RemoveCorpse();
                    pMember->Respawn();
                }
                pMember->AI()->EnterEvadeMode();
            }
        }

        EventBegun = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetDisplayId(11686);

        if (m_pInstance)
        {
            //if already done, not do anything
            if (m_pInstance->GetData(TYPE_COUNCIL) == DONE)
                return;

            if (Creature* VoiceTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_BLOOD_ELF_COUNCIL_VOICE)))
                VoiceTrigger->AI()->EnterEvadeMode();

            m_pInstance->SetData(TYPE_COUNCIL, NOT_STARTED);
        }
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}

    void StartEvent(Unit *target)
    {
        if (!m_pInstance)
            return;

        if (target && target->isAlive() && !EventBegun)
        {
            Council[0] = m_pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
            Council[1] = m_pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
            Council[2] = m_pInstance->GetData64(DATA_LADYMALANDE);
            Council[3] = m_pInstance->GetData64(DATA_VERASDARKSHADOW);

            // Start the event for the Voice Trigger
            if (Creature* pVoiceTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_BLOOD_ELF_COUNCIL_VOICE)))
            {
                if (mob_blood_elf_council_voice_triggerAI* pVoiceAI = dynamic_cast<mob_blood_elf_council_voice_triggerAI*>(pVoiceTrigger->AI()))
                {
                    pVoiceAI->LoadCouncilGUIDs();
                    pVoiceAI->EventStarted = true;
                }
            }

            for(uint8 i = 0; i < 4; ++i)
            {
                if (Council[i])
                {
                    Creature* pMember = m_creature->GetMap()->GetCreature(Council[i]);
                    if (pMember && pMember->isAlive())
                        pMember->AI()->AttackStart(target);
                }
            }

            m_pInstance->SetData(TYPE_COUNCIL, IN_PROGRESS);

            EventBegun = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!EventBegun)
            return;

        if (EndEventTimer)
        {
            if (EndEventTimer <= diff)
            {
                if (DeathCount > 3)
                {
                    if (m_pInstance)
                    {
                        if (Creature* VoiceTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_BLOOD_ELF_COUNCIL_VOICE)))
                            VoiceTrigger->DealDamage(VoiceTrigger, VoiceTrigger->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                        m_pInstance->SetData(TYPE_COUNCIL, DONE);
                    }
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    return;
                }

                Creature* pMember = m_creature->GetMap()->GetCreature(Council[DeathCount]);
                if (pMember && pMember->isAlive())
                    pMember->DealDamage(pMember, pMember->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                ++DeathCount;
                EndEventTimer = 1500;
            }else EndEventTimer -= diff;
        }

        if (CheckTimer)
        {
            if (CheckTimer <= diff)
            {
                uint8 EvadeCheck = 0;
                for(uint8 i = 0; i < 4; ++i)
                {
                    if (Council[i])
                    {
                        if (Creature* Member = m_creature->GetMap()->GetCreature(Council[i]))
                        {
                            // This is the evade/death check.
                            if (Member->isAlive() && !Member->SelectHostileTarget())
                                ++EvadeCheck;               //If all members evade, we reset so that players can properly reset the event
                            else if (!Member->isAlive())    //If even one member dies, kill the rest, set instance data, and kill self.
                            {
                                EndEventTimer = 1000;
                                CheckTimer = 0;
                                return;
                            }
                        }
                    }
                }

                if (EvadeCheck > 3)
                    Reset();

                CheckTimer = 2000;
            }else CheckTimer -= diff;
        }
    }
};

struct MANGOS_DLL_DECL boss_illidari_councilAI : public ScriptedAI
{
    boss_illidari_councilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        for(uint8 i = 0; i < 4; ++i)
            Council[i] = 0;

        LoadedGUIDs = false;
    }

    ScriptedInstance* m_pInstance;

    uint64 Council[4];

    bool LoadedGUIDs;

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pController = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ILLIDARICOUNCIL)))
            {
                if (mob_illidari_councilAI* pControlAI = dynamic_cast<mob_illidari_councilAI*>(pController->AI()))
                    pControlAI->StartEvent(pWho);
            }
        }
        else
        {
            error_log(ERROR_INST_DATA);
            EnterEvadeMode();
        }

        m_creature->SetInCombatWithZone();

        // Load GUIDs on first aggro because the creature guids are only set as the creatures are created in world-
        // this means that for each creature, it will attempt to LoadGUIDs even though some of the other creatures are
        // not in world, and thus have no GUID set in the instance data system. Putting it in aggro ensures that all the creatures
        // have been loaded and have their GUIDs set in the instance data system.
        if (!LoadedGUIDs)
            LoadGUIDs();
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (done_by == m_creature)
            return;

        damage /= 4;
        for(uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pCouncil = m_creature->GetMap()->GetCreature(Council[i]))
            {
                if (pCouncil != m_creature && damage < pCouncil->GetHealth())
                    pCouncil->SetHealth(pCouncil->GetHealth() - damage);
            }
        }
    }

    void LoadGUIDs()
    {
        if (!m_pInstance)
        {
            error_log(ERROR_INST_DATA);
            return;
        }

        Council[0] = m_pInstance->GetData64(DATA_LADYMALANDE);
        Council[1] = m_pInstance->GetData64(DATA_HIGHNETHERMANCERZEREVOR);
        Council[2] = m_pInstance->GetData64(DATA_GATHIOSTHESHATTERER);
        Council[3] = m_pInstance->GetData64(DATA_VERASDARKSHADOW);

        LoadedGUIDs = true;
    }
};

struct MANGOS_DLL_DECL boss_gathios_the_shattererAI : public boss_illidari_councilAI
{
    boss_gathios_the_shattererAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 ConsecrationTimer;
    uint32 HammerOfJusticeTimer;
    uint32 SealTimer;
    uint32 AuraTimer;
    uint32 BlessingTimer;

    void Reset()
    {
        ConsecrationTimer = 40000;
        HammerOfJusticeTimer = 10000;
        SealTimer = 40000;
        AuraTimer = 90000;
        BlessingTimer = 60000;
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_GATH_SLAY, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_GATH_DEATH, m_creature);
    }

    Unit* SelectCouncilMember()
    {
        Unit* pUnit = m_creature;
        uint32 member = 0;                                  // He chooses Lady Malande most often

        if (!urand(0, 9))                                   // But there is a chance he picks someone else.
            member = urand(1, 3);

        if (member != 2)                                    // No need to create another pointer
            pUnit = m_creature->GetMap()->GetCreature(Council[member]);

        return pUnit;
    }

    void CastAuraOnCouncil()
    {
        uint32 spellid = 0;
        switch(urand(0, 1))
        {
            case 0: spellid = SPELL_DEVOTION_AURA;   break;
            case 1: spellid = SPELL_CHROMATIC_AURA;  break;
        }
        for(uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pCouncil = m_creature->GetMap()->GetCreature(Council[i]))
                pCouncil->CastSpell(pCouncil, spellid, true, NULL, NULL, m_creature->GetGUID());
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (BlessingTimer < diff)
        {
            if (Unit* pUnit = SelectCouncilMember())
                DoCastSpellIfCan(pUnit, urand(0, 1) ? SPELL_BLESS_SPELLWARD : SPELL_BLESS_PROTECTION);

            BlessingTimer = 60000;
        }else BlessingTimer -= diff;

        if (ConsecrationTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_CONSECRATION);
            ConsecrationTimer = 40000;
        }else ConsecrationTimer -= diff;

        if (HammerOfJusticeTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                // is in ~10-40 yd range
                if (m_creature->IsInRange(target, 10.0f, 40.0f, false))
                {
                    DoCastSpellIfCan(target, SPELL_HAMMER_OF_JUSTICE);
                    HammerOfJusticeTimer = 20000;
                }
            }
        }else HammerOfJusticeTimer -= diff;

        if (SealTimer < diff)
        {
            DoCastSpellIfCan(m_creature, urand(0, 1) ? SPELL_SEAL_OF_COMMAND : SPELL_SEAL_OF_BLOOD);
            SealTimer = 40000;
        }else SealTimer -= diff;

        if (AuraTimer < diff)
        {
            CastAuraOnCouncil();
            AuraTimer = 90000;
        }else AuraTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_high_nethermancer_zerevorAI : public boss_illidari_councilAI
{
    boss_high_nethermancer_zerevorAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 BlizzardTimer;
    uint32 FlamestrikeTimer;
    uint32 ArcaneBoltTimer;
    uint32 DampenMagicTimer;
    uint32 Cooldown;
    uint32 ArcaneExplosionTimer;

    void Reset()
    {
        BlizzardTimer = urand(30000, 90000);
        FlamestrikeTimer = urand(30000, 90000);
        ArcaneBoltTimer = 10000;
        DampenMagicTimer = 2000;
        ArcaneExplosionTimer = 14000;
        Cooldown = 0;
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_ZERE_SLAY, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_ZERE_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Cooldown)
        {
            if (Cooldown < diff)
                Cooldown = 0;
            else
            {
                Cooldown -= diff;
                return;                                     // Don't cast any other spells if global cooldown is still ticking
            }
        }

        if (DampenMagicTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DAMPEN_MAGIC);
            Cooldown = 1000;
            DampenMagicTimer = 110000;                      // almost 2 minutes
            ArcaneBoltTimer += 1000;                        // Give the Mage some time to spellsteal Dampen.
        }else DampenMagicTimer -= diff;

        if (ArcaneExplosionTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            Cooldown = 1000;
            ArcaneExplosionTimer = 14000;
        }else ArcaneExplosionTimer -= diff;

        if (ArcaneBoltTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_BOLT);
            ArcaneBoltTimer = 3000;
            Cooldown = 2000;
        }else ArcaneBoltTimer -= diff;

        if (BlizzardTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_BLIZZARD);
                BlizzardTimer = urand(45000, 90000);
                FlamestrikeTimer += 10000;
                Cooldown = 1000;
            }
        }else BlizzardTimer -= diff;

        if (FlamestrikeTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_FLAMESTRIKE);
                FlamestrikeTimer = urand(55000, 100000);
                BlizzardTimer += 10000;
                Cooldown = 2000;
            }
        }else FlamestrikeTimer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_lady_malandeAI : public boss_illidari_councilAI
{
    boss_lady_malandeAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 EmpoweredSmiteTimer;
    uint32 CircleOfHealingTimer;
    uint32 DivineWrathTimer;
    uint32 ReflectiveShieldTimer;

    void Reset()
    {
        EmpoweredSmiteTimer = 38000;
        CircleOfHealingTimer = 20000;
        DivineWrathTimer = 40000;
        ReflectiveShieldTimer = 0;
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_MALA_SLAY, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_MALA_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (EmpoweredSmiteTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_EMPOWERED_SMITE);
                EmpoweredSmiteTimer = 38000;
            }
        }else EmpoweredSmiteTimer -= diff;

        if (CircleOfHealingTimer < diff)
        {
            //Currently bugged and puts Malande on the threatlist of the other council members. It also heals players.
            //DoCastSpellIfCan(m_creature, SPELL_CIRCLE_OF_HEALING);
            CircleOfHealingTimer = 60000;
        }else CircleOfHealingTimer -= diff;

        if (DivineWrathTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_DIVINE_WRATH);
                DivineWrathTimer = urand(40000, 80000);
            }
        }else DivineWrathTimer -= diff;

        if (ReflectiveShieldTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTIVE_SHIELD);
            ReflectiveShieldTimer = 65000;
        }else ReflectiveShieldTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_veras_darkshadowAI : public boss_illidari_councilAI
{
    boss_veras_darkshadowAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint64 EnvenomTargetGUID;

    uint32 DeadlyPoisonTimer;
    uint32 VanishTimer;
    uint32 AppearEnvenomTimer;

    bool HasVanished;

    void Reset()
    {
        EnvenomTargetGUID = 0;

        DeadlyPoisonTimer = 20000;
        VanishTimer = urand(60000, 120000);
        AppearEnvenomTimer = 150000;

        HasVanished = false;
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(SAY_VERA_SLAY, m_creature);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_VERA_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!HasVanished)
        {
            if (DeadlyPoisonTimer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEADLY_POISON);
                DeadlyPoisonTimer = urand(15000, 45000);
            }else DeadlyPoisonTimer -= diff;

            if (AppearEnvenomTimer < diff)                  // Cast Envenom. This is cast 4 seconds after Vanish is over
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENVENOM);
                AppearEnvenomTimer = 90000;
            }else AppearEnvenomTimer -= diff;

            if (VanishTimer < diff)                         // Disappear and stop attacking, but follow a random unit
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    VanishTimer = 30000;
                    AppearEnvenomTimer= 28000;
                    HasVanished = true;
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoResetThreat();
                                                            // Chase a unit. Check before DoMeleeAttackIfReady prevents from attacking
                    m_creature->AddThreat(target, 500000.0f);
                    m_creature->GetMotionMaster()->MoveChase(target);
                }
            }else VanishTimer -= diff;

            DoMeleeAttackIfReady();
        }
        else
        {
            if (VanishTimer < diff)                         // Become attackable and poison current target
            {
                Unit* target = m_creature->getVictim();
                DoCastSpellIfCan(target, SPELL_DEADLY_POISON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoResetThreat();
                m_creature->AddThreat(target, 3000.0f);     // Make Veras attack his target for a while, he will cast Envenom 4 seconds after.
                DeadlyPoisonTimer += 6000;
                VanishTimer = 90000;
                AppearEnvenomTimer = 4000;
                HasVanished = false;
            }else VanishTimer -= diff;

            if (AppearEnvenomTimer < diff)                  // Appear 2 seconds before becoming attackable (Shifting out of vanish)
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->SetVisibility(VISIBILITY_ON);
                AppearEnvenomTimer = 6000;
            }else AppearEnvenomTimer -= diff;
        }
    }
};

CreatureAI* GetAI_mob_blood_elf_council_voice_trigger(Creature* pCreature)
{
    return new mob_blood_elf_council_voice_triggerAI(pCreature);
}

CreatureAI* GetAI_mob_illidari_council(Creature* pCreature)
{
    return new mob_illidari_councilAI(pCreature);
}

CreatureAI* GetAI_boss_gathios_the_shatterer(Creature* pCreature)
{
    return new boss_gathios_the_shattererAI(pCreature);
}

CreatureAI* GetAI_boss_lady_malande(Creature* pCreature)
{
    return new boss_lady_malandeAI(pCreature);
}

CreatureAI* GetAI_boss_veras_darkshadow(Creature* pCreature)
{
    return new boss_veras_darkshadowAI(pCreature);
}

CreatureAI* GetAI_boss_high_nethermancer_zerevor(Creature* pCreature)
{
    return new boss_high_nethermancer_zerevorAI(pCreature);
}

void AddSC_boss_illidari_council()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_illidari_council";
    newscript->GetAI = &GetAI_mob_illidari_council;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_blood_elf_council_voice_trigger";
    newscript->GetAI = &GetAI_mob_blood_elf_council_voice_trigger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_gathios_the_shatterer";
    newscript->GetAI = &GetAI_boss_gathios_the_shatterer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_lady_malande";
    newscript->GetAI = &GetAI_boss_lady_malande;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_veras_darkshadow";
    newscript->GetAI = &GetAI_boss_veras_darkshadow;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_high_nethermancer_zerevor";
    newscript->GetAI = &GetAI_boss_high_nethermancer_zerevor;
    newscript->RegisterSelf();
}

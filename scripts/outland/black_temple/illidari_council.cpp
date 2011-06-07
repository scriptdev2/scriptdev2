/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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

enum
{
    //Speech'n'Sounds
    SAY_GATH_SLAY               = -1564085,
    SAY_GATH_SLAY_COMNT         = -1564089,
    SAY_GATH_DEATH              = -1564093,
    SAY_GATH_SPECIAL1           = -1564077,
    SAY_GATH_SPECIAL2           = -1564081,

    SAY_VERA_SLAY               = -1564086,
    SAY_VERA_COMNT              = -1564089,
    SAY_VERA_DEATH              = -1564094,
    SAY_VERA_SPECIAL1           = -1564078,
    SAY_VERA_SPECIAL2           = -1564082,

    SAY_MALA_SLAY               = -1564087,
    SAY_MALA_COMNT              = -1564090,
    SAY_MALA_DEATH              = -1564095,
    SAY_MALA_SPECIAL1           = -1564079,
    SAY_MALA_SPECIAL2           = -1564083,

    SAY_ZERE_SLAY               = -1564088,
    SAY_ZERE_COMNT              = -1564091,
    SAY_ZERE_DEATH              = -1564096,
    SAY_ZERE_SPECIAL1           = -1564080,
    SAY_ZERE_SPECIAL2           = -1564084,
};

struct CouncilYells
{
    int32 entry;
    uint32 timer;
};

static const CouncilYells CouncilAggro[]=
{
    {-1564069, 5000},                                       // Gathios
    {-1564070, 5500},                                       // Veras
    {-1564071, 5000},                                       // Malande
    {-1564072, 0},                                          // Zerevor
};

// Need to get proper timers for this later
static const CouncilYells CouncilEnrage[]=
{
    {-1564073, 2000},                                       // Gathios
    {-1564074, 6000},                                       // Veras
    {-1564075, 5000},                                       // Malande
    {-1564076, 0},                                          // Zerevor
};

enum
{
    // High Nethermancer Zerevor's spells
    SPELL_FLAMESTRIKE           = 41481,
    SPELL_BLIZZARD              = 41482,
    SPELL_ARCANE_BOLT           = 41483,
    SPELL_ARCANE_EXPLOSION      = 41524,
    SPELL_DAMPEN_MAGIC          = 41478,

    // Lady Malande's spells
    SPELL_EMPOWERED_SMITE       = 41471,
    SPELL_CIRCLE_OF_HEALING     = 41455,
    SPELL_REFLECTIVE_SHIELD     = 41475,
    SPELL_DIVINE_WRATH          = 41472,
    SPELL_HEAL_VISUAL           = 24171,

    // Gathios the Shatterer's spells
    SPELL_BLESS_PROTECTION      = 41450,
    SPELL_BLESS_SPELLWARD       = 41451,
    SPELL_CONSECRATION          = 41541,
    SPELL_HAMMER_OF_JUSTICE     = 41468,
    SPELL_SEAL_OF_COMMAND       = 41469,
    SPELL_SEAL_OF_BLOOD         = 41459,
    SPELL_CHROMATIC_AURA        = 41453,
    SPELL_DEVOTION_AURA         = 41452,

    // Veras Darkshadow's spells
    SPELL_DEADLY_POISON         = 41485,
    SPELL_ENVENOM               = 41487,
    SPELL_VANISH                = 41479,

    SPELL_BERSERK               = 45078,
};

static const uint32 aCouncilMember[] = {NPC_GATHIOS, NPC_VERAS, NPC_LADY_MALANDE, NPC_ZEREVOR};

struct MANGOS_DLL_DECL mob_blood_elf_council_voice_triggerAI : public ScriptedAI
{
    mob_blood_elf_council_voice_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)(m_creature->GetInstanceData());
        Reset();
    }
    ScriptedInstance* m_pInstance;

    uint32 m_uiEnrageTimer;
    uint32 m_uiAggroYellTimer;

    uint8 m_uiYellCounter;                                  // Serves as the counter for both the aggro and enrage yells

    bool m_bEventStarted;

    void StartVoiceEvent()
    {
        if (!m_pInstance)
            return;

        m_bEventStarted = true;
    }

    void Reset()
    {
        m_uiEnrageTimer = 15*MINUTE*IN_MILLISECONDS;        // 15 minutes
        m_uiAggroYellTimer = 500;
        m_uiYellCounter = 0;

        m_bEventStarted = false;
    }

    void AttackStart(Unit* pWho) {}
    void MoveInLineOfSight(Unit* pWho) {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bEventStarted)
            return;

        if (m_uiYellCounter > 3)
            return;

        if (m_uiAggroYellTimer)
        {
            if (m_uiAggroYellTimer <= uiDiff)
            {
                if (Creature* pMember = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[m_uiYellCounter]))
                {
                    DoScriptText(CouncilAggro[m_uiYellCounter].entry, pMember);
                    m_uiAggroYellTimer = CouncilAggro[m_uiYellCounter].timer;
                }
                ++m_uiYellCounter;

                if (m_uiYellCounter > 3)
                    m_uiYellCounter = 0;                    // Reuse for Enrage Yells
            }
            else
                m_uiAggroYellTimer -= uiDiff;
        }

        if (m_uiEnrageTimer)
        {
            if (m_uiEnrageTimer <= uiDiff)
            {
                if (Creature* pMember = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[m_uiYellCounter]))
                {
                    pMember->CastSpell(pMember, SPELL_BERSERK, true);
                    DoScriptText(CouncilEnrage[m_uiYellCounter].entry, pMember);
                    m_uiEnrageTimer = CouncilEnrage[m_uiYellCounter].timer;
                }
                ++m_uiYellCounter;
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }
    }
};

struct MANGOS_DLL_DECL mob_illidari_councilAI : public ScriptedAI
{
    mob_illidari_councilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiCheckTimer;
    uint32 m_uiEndEventTimer;

    uint8 m_uiDeathCount;

    bool m_bEventBegun;

    void Reset()
    {
        m_uiCheckTimer    = 2000;
        m_uiEndEventTimer = 0;

        m_uiDeathCount = 0;

        m_bEventBegun = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetDisplayId(11686);

        if (m_pInstance)
        {
            // If already done, not do anything
            if (m_pInstance->GetData(TYPE_COUNCIL) == DONE)
                return;

            if (Creature* VoiceTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_COUNCIL_VOICE))
                VoiceTrigger->AI()->EnterEvadeMode();

            m_pInstance->SetData(TYPE_COUNCIL, NOT_STARTED);

            for(uint8 i = 0; i < 4; ++i)
            {
                if (Creature* pMember = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[i]))
                {
                    if (!pMember->isAlive())
                    {
                        pMember->RemoveCorpse();
                        pMember->Respawn();
                    }
                    pMember->AI()->EnterEvadeMode();
                }
            }
        }
    }

    void AttackStart(Unit* pWho) {}
    void MoveInLineOfSight(Unit* pWho) {}

    void StartEvent(Unit* pTarget)
    {
        if (!m_pInstance)
            return;

        if (pTarget && pTarget->isAlive() && !m_bEventBegun)
        {
            // Prevent further handling for next council uiMember aggroing
            m_bEventBegun = true;

            // Start the event for the Voice Trigger
            if (Creature* pVoiceTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_COUNCIL_VOICE))
            {
                if (mob_blood_elf_council_voice_triggerAI* pVoiceAI = dynamic_cast<mob_blood_elf_council_voice_triggerAI*>(pVoiceTrigger->AI()))
                    pVoiceAI->StartVoiceEvent();
            }

            for(uint8 i = 0; i < 4; ++i)
            {
                Creature* pMember = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[i]);
                if (pMember && pMember->isAlive() && !pMember->isInCombat())
                    pMember->AI()->AttackStart(pTarget);
            }

            // All are set into combat now, Set Instance Data
            m_pInstance->SetData(TYPE_COUNCIL, IN_PROGRESS);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_bEventBegun)                                    // Can only be true when m_pInstance is valid
            return;

        if (m_uiEndEventTimer)
        {
            if (m_uiEndEventTimer <= diff)
            {
                if (m_uiDeathCount > 3)
                {
                    if (Creature* VoiceTrigger = m_pInstance->GetSingleCreatureFromStorage(NPC_COUNCIL_VOICE))
                        VoiceTrigger->DealDamage(VoiceTrigger, VoiceTrigger->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                    m_pInstance->SetData(TYPE_COUNCIL, DONE);

                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    return;
                }

                Creature* pMember = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[m_uiDeathCount]);
                if (pMember && pMember->isAlive())
                    pMember->DealDamage(pMember, pMember->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                ++m_uiDeathCount;
                m_uiEndEventTimer = 1500;
            }
            else
                m_uiEndEventTimer -= diff;
        }

        if (m_uiCheckTimer)
        {
            if (m_uiCheckTimer <= diff)
            {
                uint8 EvadeCheck = 0;
                for(uint8 i = 0; i < 4; ++i)
                {
                    if (Creature* Member = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[i]))
                    {
                        // This is the evade/death check.
                        if (Member->isAlive() && !Member->SelectHostileTarget())
                            ++EvadeCheck;                   // If all members evade, we reset so that players can properly reset the event
                        else if (!Member->isAlive())        // If even one uiMember dies, kill the rest, set instance data, and kill self.
                        {
                            m_uiEndEventTimer = 1000;
                            m_uiCheckTimer = 0;
                            return;
                        }
                    }
                }

                if (EvadeCheck > 3)
                {
                    m_pInstance->SetData(TYPE_COUNCIL, FAIL);
                    Reset();
                }

                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= diff;
        }
    }
};

struct MANGOS_DLL_DECL boss_illidari_councilAI : public ScriptedAI
{
    boss_illidari_councilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pController = m_pInstance->GetSingleCreatureFromStorage(NPC_ILLIDARI_COUNCIL))
            {
                if (mob_illidari_councilAI* pControlAI = dynamic_cast<mob_illidari_councilAI*>(pController->AI()))
                    pControlAI->StartEvent(pWho);
            }
        }
        else
            EnterEvadeMode();
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (done_by == m_creature)
            return;

        damage /= 4;
        if (!m_pInstance)
            return;

        for(uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pCouncil = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[i]))
            {
                if (pCouncil != m_creature && damage < pCouncil->GetHealth())
                    pCouncil->SetHealth(pCouncil->GetHealth() - damage);
            }
        }
    }
};

struct MANGOS_DLL_DECL boss_gathios_the_shattererAI : public boss_illidari_councilAI
{
    boss_gathios_the_shattererAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 m_uiConsecrationTimer;
    uint32 m_uiHammerOfJusticeTimer;
    uint32 m_uiSealTimer;
    uint32 m_uiAuraTimer;
    uint32 m_uiBlessingTimer;

    void Reset()
    {
        m_uiConsecrationTimer = 40000;
        m_uiHammerOfJusticeTimer = 10000;
        m_uiSealTimer = 40000;
        m_uiAuraTimer = 90000;
        m_uiBlessingTimer = 60000;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_GATH_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_GATH_DEATH, m_creature);
    }

    Unit* SelectCouncilMember()
    {
        if (!m_pInstance)
            return m_creature;

        Unit* pUnit = m_creature;
        uint32 uiMember = 2;                                // He chooses Lady Malande most often

        if (!urand(0, 9))                                   // But there is a chance he picks someone else.
        {
            uiMember += urand(1, 3);
            uiMember %= 4;
        }

        if (uiMember != 0)                                  // No need to create another pointer
            pUnit = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[uiMember]);

        return pUnit;
    }

    void CastAuraOnCouncil()
    {
        uint32 uiSpellid = 0;
        switch(urand(0, 1))
        {
            case 0: uiSpellid = SPELL_DEVOTION_AURA;   break;
            case 1: uiSpellid = SPELL_CHROMATIC_AURA;  break;
        }

        if (!m_pInstance)
            return;

        for(uint8 i = 0; i < 4; ++i)
        {
            if (Creature* pCouncil = m_pInstance->GetSingleCreatureFromStorage(aCouncilMember[i]))
                pCouncil->CastSpell(pCouncil, uiSpellid, true, NULL, NULL, m_creature->GetObjectGuid());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBlessingTimer < uiDiff)
        {
            if (Unit* pUnit = SelectCouncilMember())
                DoCastSpellIfCan(pUnit, urand(0, 1) ? SPELL_BLESS_SPELLWARD : SPELL_BLESS_PROTECTION);

            m_uiBlessingTimer = 60000;
        }
        else
            m_uiBlessingTimer -= uiDiff;

        if (m_uiConsecrationTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_CONSECRATION);
            m_uiConsecrationTimer = 40000;
        }
        else
            m_uiConsecrationTimer -= uiDiff;

        if (m_uiHammerOfJusticeTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                // is in ~10-40 yd range
                if (m_creature->IsInRange(target, 10.0f, 40.0f, false))
                {
                    DoCastSpellIfCan(target, SPELL_HAMMER_OF_JUSTICE);
                    m_uiHammerOfJusticeTimer = 20000;
                }
            }
        }
        else
            m_uiHammerOfJusticeTimer -= uiDiff;

        if (m_uiSealTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, urand(0, 1) ? SPELL_SEAL_OF_COMMAND : SPELL_SEAL_OF_BLOOD);
            m_uiSealTimer = 40000;
        }
        else
            m_uiSealTimer -= uiDiff;

        if (m_uiAuraTimer < uiDiff)
        {
            CastAuraOnCouncil();
            m_uiAuraTimer = 90000;
        }
        else
            m_uiAuraTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_high_nethermancer_zerevorAI : public boss_illidari_councilAI
{
    boss_high_nethermancer_zerevorAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 m_uiBlizzardTimer;
    uint32 m_uiFlamestrikeTimer;
    uint32 m_uiArcaneBoltTimer;
    uint32 m_uiDampenMagicTimer;
    uint32 m_uiCooldown;
    uint32 m_uiArcaneExplosionTimer;

    void Reset()
    {
        m_uiBlizzardTimer = urand(30000, 90000);
        m_uiFlamestrikeTimer = urand(30000, 90000);
        m_uiArcaneBoltTimer = 10000;
        m_uiDampenMagicTimer = 2000;
        m_uiArcaneExplosionTimer = 14000;
        m_uiCooldown = 0;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_ZERE_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_ZERE_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCooldown)
        {
            if (m_uiCooldown < uiDiff)
                m_uiCooldown = 0;
            else
            {
                m_uiCooldown -= uiDiff;
                return;                                     // Don't cast any other spells if global cooldown is still ticking
            }
        }

        if (m_uiDampenMagicTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DAMPEN_MAGIC);
            m_uiCooldown = 1000;
            m_uiDampenMagicTimer = 110000;                  // Almost 2 minutes
            m_uiArcaneBoltTimer += 1000;                    // Give the Mage some time to spellsteal Dampen.
        }
        else
            m_uiDampenMagicTimer -= uiDiff;

        if (m_uiArcaneExplosionTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            m_uiCooldown = 1000;
            m_uiArcaneExplosionTimer = 14000;
        }
        else
            m_uiArcaneExplosionTimer -= uiDiff;

        if (m_uiArcaneBoltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANE_BOLT);
            m_uiArcaneBoltTimer = 3000;
            m_uiCooldown = 2000;
        }
        else
            m_uiArcaneBoltTimer -= uiDiff;

        if (m_uiBlizzardTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_BLIZZARD);
                m_uiBlizzardTimer = urand(45000, 90000);
                m_uiFlamestrikeTimer += 10000;
                m_uiCooldown = 1000;
            }
        }
        else
            m_uiBlizzardTimer -= uiDiff;

        if (m_uiFlamestrikeTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_FLAMESTRIKE);
                m_uiFlamestrikeTimer = urand(55000, 100000);
                m_uiBlizzardTimer += 10000;
                m_uiCooldown = 2000;
            }
        }
        else
            m_uiFlamestrikeTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_lady_malandeAI : public boss_illidari_councilAI
{
    boss_lady_malandeAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 m_uiEmpoweredSmiteTimer;
    uint32 m_uiCircleOfHealingTimer;
    uint32 m_uiDivineWrathTimer;
    uint32 m_uiReflectiveShieldTimer;

    void Reset()
    {
        m_uiEmpoweredSmiteTimer = 38000;
        m_uiCircleOfHealingTimer = 20000;
        m_uiDivineWrathTimer = 40000;
        m_uiReflectiveShieldTimer = 0;
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_MALA_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_MALA_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEmpoweredSmiteTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, SPELL_EMPOWERED_SMITE);
                m_uiEmpoweredSmiteTimer = 38000;
            }
        }
        else
            m_uiEmpoweredSmiteTimer -= uiDiff;

        if (m_uiCircleOfHealingTimer < uiDiff)
        {
            //Currently bugged and puts Malande on the threatlist of the other council members. It also heals players.
            //DoCastSpellIfCan(m_creature, SPELL_CIRCLE_OF_HEALING);
            m_uiCircleOfHealingTimer = 60000;
        }
        else
            m_uiCircleOfHealingTimer -= uiDiff;

        if (m_uiDivineWrathTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_DIVINE_WRATH);
                m_uiDivineWrathTimer = urand(40000, 80000);
            }
        }
        else
            m_uiDivineWrathTimer -= uiDiff;

        if (m_uiReflectiveShieldTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_REFLECTIVE_SHIELD);
            m_uiReflectiveShieldTimer = 65000;
        }
        else
            m_uiReflectiveShieldTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_veras_darkshadowAI : public boss_illidari_councilAI
{
    boss_veras_darkshadowAI(Creature* pCreature) : boss_illidari_councilAI(pCreature) { Reset(); }

    uint32 m_uiDeadlyPoisonTimer;
    uint32 m_uiVanishTimer;
    uint32 m_uiAppearEnvenomTimer;

    bool m_bHasVanished;

    void Reset()
    {
        m_uiDeadlyPoisonTimer = 20000;
        m_uiVanishTimer = urand(60000, 120000);
        m_uiAppearEnvenomTimer = 150000;

        m_bHasVanished = false;
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_VERA_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_VERA_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bHasVanished)
        {
            if (m_uiDeadlyPoisonTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEADLY_POISON);
                m_uiDeadlyPoisonTimer = urand(15000, 45000);
            }
            else
                m_uiDeadlyPoisonTimer -= uiDiff;

            if (m_uiAppearEnvenomTimer < uiDiff)            // Cast Envenom. This is cast 4 seconds after Vanish is over
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENVENOM);
                m_uiAppearEnvenomTimer = 90000;
            }
            else
                m_uiAppearEnvenomTimer -= uiDiff;

            if (m_uiVanishTimer < uiDiff)                   // Disappear and stop attacking, but follow a random unit
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_uiVanishTimer = 30000;
                    m_uiAppearEnvenomTimer= 28000;
                    m_bHasVanished = true;
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoResetThreat();
                                                            // Chase a unit. Check before DoMeleeAttackIfReady prevents from attacking
                    m_creature->AddThreat(pTarget, 500000.0f);
                    m_creature->GetMotionMaster()->MoveChase(pTarget);
                }
            }
            else
                m_uiVanishTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        else
        {
            if (m_uiVanishTimer < uiDiff)                   // Become attackable and poison current target
            {
                Unit* target = m_creature->getVictim();
                DoCastSpellIfCan(target, SPELL_DEADLY_POISON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoResetThreat();
                m_creature->AddThreat(target, 3000.0f);     // Make Veras attack his pTarget for a while, he will cast Envenom 4 seconds after.
                m_uiDeadlyPoisonTimer += 6000;
                m_uiVanishTimer = 90000;
                m_uiAppearEnvenomTimer = 4000;
                m_bHasVanished = false;
            }
            else
                m_uiVanishTimer -= uiDiff;

            if (m_uiAppearEnvenomTimer < uiDiff)            // Appear 2 seconds before becoming attackable (Shifting out of vanish)
            {
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->SetVisibility(VISIBILITY_ON);
                m_uiAppearEnvenomTimer = 6000;
            }
            else
                m_uiAppearEnvenomTimer -= uiDiff;
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
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_illidari_council";
    pNewScript->GetAI = &GetAI_mob_illidari_council;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_blood_elf_council_voice_trigger";
    pNewScript->GetAI = &GetAI_mob_blood_elf_council_voice_trigger;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_gathios_the_shatterer";
    pNewScript->GetAI = &GetAI_boss_gathios_the_shatterer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_lady_malande";
    pNewScript->GetAI = &GetAI_boss_lady_malande;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_veras_darkshadow";
    pNewScript->GetAI = &GetAI_boss_veras_darkshadow;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_high_nethermancer_zerevor";
    pNewScript->GetAI = &GetAI_boss_high_nethermancer_zerevor;
    pNewScript->RegisterSelf();
}

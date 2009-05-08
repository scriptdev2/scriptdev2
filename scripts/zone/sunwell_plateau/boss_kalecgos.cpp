/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Kalecgos
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "WorldPacket.h"
#include "def_sunwell_plateau.h"

//kalecgos dragon form
#define SAY_EVIL_AGGRO                  -1580000
#define SAY_EVIL_SPELL1                 -1580001
#define SAY_EVIL_SPELL2                 -1580002
#define SAY_EVIL_SLAY1                  -1580003
#define SAY_EVIL_SLAY2                  -1580004
#define SAY_EVIL_ENRAGE                 -1580005
//kalecgos humanoid form
#define SAY_GOOD_AGGRO                  -1580006
#define SAY_GOOD_NEAR_DEATH             -1580007
#define SAY_GOOD_NEAR_DEATH2            -1580008
#define SAY_GOOD_PLRWIN                 -1580009

#define SAY_SATH_AGGRO                  -1580010
#define SAY_SATH_DEATH                  -1580011
#define SAY_SATH_SPELL1                 -1580012
#define SAY_SATH_SPELL2                 -1580013
#define SAY_SATH_SLAY1                  -1580014
#define SAY_SATH_SLAY2                  -1580015
#define SAY_SATH_ENRAGE                 -1580016

#define SPELL_SPECTRAL_EXHAUSTION       44867
#define SPELL_TELEPORT_SPECTRAL_REALM   46019
#define SPELL_SPECTRAL_REALM            46021

#define NOTIFY_SPECTRALLY_EXHAUSTED     "Your body is too exhausted to travel to the Spectral Realm."
#define ERROR_INST_DATA                 "SD2: Instance Data not set properly for Sunwell Plateau. Kalecgos Encounter will be buggy."
#define ERROR_INST_DATA_PLR             "SD2 ERROR: Instance Data not set properly for Sunwell Plateau. Please report this to your administrator."
#define ERROR_UNABLE_TO_TELEPORT        "SD2: Unable to select target for Spectral Blast. Threatlist has too few players."
#define ERROR_MISSING_TELEPORT_GUID     "SD2: [Kalecgos] Invalid TeleportTargetGUID. Unable to teleport player."
#define ERROR_KALECGOS_NOT_FOUND        "SD2: Unable to create pointer to Kalecgos from Sathrovarr."

#define KALECGOS_ARENA_X                1704.34
#define KALECGOS_ARENA_Y                928.17
#define KALECGOS_ARENA_Z                53.08

/*** Kalecgos ****/
#define SPELL_SPECTRAL_BLAST            44866
#define SPELL_ARCANE_BUFFET             45018
#define SPELL_FROST_BREATH              44799
#define SPELL_HEROIC_STRIKE             45026
#define SPELL_REVITALIZE                45027
#define SPELL_TAIL_LASH                 45122
#define SPELL_TRANSFORM_KALEC           45027
#define SPELL_CRAZED_RAGE               44806

uint32 WildMagic[]= { 44978, 45001, 45002, 45004, 45006, 45010 };

/*** Sathrovarr ***/
#define SPELL_CORRUPTING_STRIKE         45029
#define SPELL_CURSE_OF_BOUNDLESS_AGONY  45032
#define SPELL_SHADOW_BOLT_VOLLEY        45031

/*** Misc ***/
#define SPELL_BANISH                    44836

void TeleportToInnerVeil(Player* plr)
{
    if (plr->HasAura(SPELL_SPECTRAL_EXHAUSTION, 0))
    {
        plr->GetSession()->SendNotification(NOTIFY_SPECTRALLY_EXHAUSTED);
        return;
    }

    ScriptedInstance* pInstance = ((ScriptedInstance*)plr->GetInstanceData());
    if (!pInstance)
    {
        error_log(ERROR_INST_DATA);
        plr->GetSession()->SendNotification(ERROR_INST_DATA_PLR);
        return;
    }

    pInstance->SetData64(DATA_PLAYER_SPECTRAL_REALM, plr->GetGUID());
    // Remove the player from Kalecgos' Threat List
    Creature* Kalecgos = ((Creature*)Unit::GetUnit(*plr, pInstance->GetData64(DATA_KALECGOS_DRAGON)));
    if (Kalecgos)
    {
        HostilReference* ref = Kalecgos->getThreatManager().getOnlineContainer().getReferenceByTarget(plr);
        if (ref)
            ref->removeReference();
    }

    // Add the player to Sathrovarr's Threat List
    Creature* Sathrovarr = ((Creature*)Unit::GetUnit(*plr, pInstance->GetData64(DATA_SATHROVARR)));
    if (Sathrovarr)
        Sathrovarr->AddThreat(plr, 1.0f);

    // Make them able to see Sathrovarr (he's invisible for some reason). Also, when this buff wears off, they get teleported back to Normal Realm (this is handled by Instance Script)
    plr->CastSpell(plr, SPELL_SPECTRAL_REALM, true);
    plr->CastSpell(plr, SPELL_TELEPORT_SPECTRAL_REALM, true);
}

struct MANGOS_DLL_DECL boss_kalecgosAI : public ScriptedAI
{
    boss_kalecgosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 TeleportTargetGUID;

    uint32 ArcaneBuffetTimer;
    uint32 FrostBreathTimer;
    uint32 WildMagicTimer;
    uint32 SpectralBlastTimer;
    uint32 SpectralTeleportTimer;
    uint32 ForceFieldTimer;
    uint32 ExitTimer;

    bool LockedArena;
    bool Uncorrupted;
    bool Banished;
    bool Checked;
    bool Enraged;

    void Reset()
    {
        TeleportTargetGUID = 0;

        // TODO: Fix timers
        ArcaneBuffetTimer       = 8000;
        FrostBreathTimer        = 24000;
        WildMagicTimer          = 18000;
        SpectralBlastTimer      = 30000;
        SpectralTeleportTimer   = SpectralBlastTimer + 2000;

        ForceFieldTimer = 20000;
        ExitTimer = 0;

        LockedArena = false;
        Uncorrupted = false;
        Banished    = false;
        Checked     = false;
        Enraged     = false;

        // Reset Sathrovarr too
        if (pInstance)
            if (Creature* Sath = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SATHROVARR))))
                Sath->AI()->EnterEvadeMode();
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_EVIL_AGGRO, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_KALECGOS_EVENT, IN_PROGRESS);
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && done_by != m_creature)
        {
            if (!Uncorrupted)
            {
                damage = 0;
                Banished = true;
                DoCast(m_creature, SPELL_BANISH, true);
                m_creature->GetMotionMaster()->MoveIdle();
            }
            else
            {
                damage = 0;
                BeginOutro();
            }
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_EVIL_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_EVIL_SLAY2, m_creature); break;
        }
    }

    void BeginOutro()
    {
        debug_log("SD2: KALEC: Beginning Outro");

        if (!pInstance)
        {
            error_log(ERROR_INST_DATA);
            return;
        }

        Unit* Sathrovarr = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SATHROVARR));
        if (Sathrovarr)
        {
            Sathrovarr->DealDamage(Sathrovarr, Sathrovarr->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            Sathrovarr->Relocate(KALECGOS_ARENA_X, KALECGOS_ARENA_Y, KALECGOS_ARENA_Z);
            Sathrovarr->SendMonsterMove(KALECGOS_ARENA_X, KALECGOS_ARENA_Y, KALECGOS_ARENA_Z, 0, 0, 0);
        }

        Creature* Kalec = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KALECGOS_HUMAN)));
        if (Kalec)
        {
            Kalec->DeleteThreatList();
            Kalec->SetVisibility(VISIBILITY_OFF);
        }

        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->setFaction(35);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id)
        {
            if (pInstance)
            {
                if (GameObject* pForceField = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_GO_FORCEFIELD)))
                    pForceField->SetGoState(GO_STATE_READY);

                pInstance->SetData(DATA_KALECGOS_EVENT, DONE);
            }
            else error_log(ERROR_INST_DATA);

            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostilTarget() || Banished)
            return;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !Enraged)
        {
            Unit* Sathrovarr = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SATHROVARR));
            if (Sathrovarr)
                Sathrovarr->CastSpell(Sathrovarr, SPELL_CRAZED_RAGE, true);
            DoCast(m_creature, SPELL_CRAZED_RAGE, true);
            Enraged = true;
        }

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 1) && !Checked)
        {
            Checked = true;

            if (!Uncorrupted)
            {
                Banished = true;
                DoCast(m_creature, SPELL_BANISH, true);
                m_creature->GetMotionMaster()->MoveIdle();
            }
            else
                BeginOutro();
        }

        if (ExitTimer)
        {
            if (ExitTimer <= diff)
            {
                debug_log("SD2: KALEC: Exiting the arena");
                DoScriptText(SAY_GOOD_PLRWIN, m_creature);

                m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
                float x, y, z;
                float iniX, iniY, iniZ;
                m_creature->GetPosition(iniX, iniY, iniZ);
                m_creature->GetRandomPoint(iniX, iniY, iniZ, 30, x, y, z);
                z = 70;
                m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
            }else ExitTimer -= diff;
        }

        if (!LockedArena)
        {
            if (ForceFieldTimer < diff)
            {
                if (pInstance)
                {
                    if (GameObject* pForceField = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_GO_FORCEFIELD)))
                        pForceField->SetGoState(GO_STATE_ACTIVE);

                    LockedArena = true;
                }else error_log(ERROR_INST_DATA);
            }else ForceFieldTimer -= diff;
        }

        if (ArcaneBuffetTimer < diff)
        {
            if (rand()%3 == 0)
                DoScriptText(SAY_EVIL_SPELL1, m_creature);

            DoCast(m_creature->getVictim(), SPELL_ARCANE_BUFFET);
            ArcaneBuffetTimer = 20000;
        }else ArcaneBuffetTimer -= diff;

        if (FrostBreathTimer < diff)
        {
            if (rand()%2 == 0)
                DoScriptText(SAY_EVIL_SPELL2, m_creature);

            DoCast(m_creature->getVictim(), SPELL_FROST_BREATH);
            FrostBreathTimer = 25000;
        }else FrostBreathTimer -= diff;

        if (WildMagicTimer < diff)
        {
            DoCast(m_creature->getVictim(), WildMagic[rand()%6]);
            WildMagicTimer = 19000;
        }else WildMagicTimer -= diff;

        if (SpectralBlastTimer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1))
            {
                TeleportTargetGUID = target->GetGUID();
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, TeleportTargetGUID);
                SpectralBlastTimer = 30000;
                SpectralTeleportTimer = 2000;
            }
        }else SpectralBlastTimer -= diff;

        if (SpectralTeleportTimer < diff)
        {
            if (TeleportTargetGUID)
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), TeleportTargetGUID);
                if (pUnit)
                {
                    pUnit->CastSpell(pUnit, SPELL_SPECTRAL_BLAST, true);
                    TeleportToInnerVeil((Player*)pUnit);
                }
                else error_log(ERROR_MISSING_TELEPORT_GUID);
            }
            else error_log(ERROR_MISSING_TELEPORT_GUID);

            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->getVictim()->GetGUID());
            TeleportTargetGUID = 0;
            SpectralTeleportTimer = SpectralBlastTimer + 2000;

        }else SpectralTeleportTimer -= diff;

        if (!Banished)
            DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_sathrovarrAI : public ScriptedAI
{
    boss_sathrovarrAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 CorruptingStrikeTimer;
    uint32 CurseOfBoundlessAgonyTimer;
    uint32 ShadowBoltVolleyTimer;
    bool Banished;
    bool Enraged;

    void Reset()
    {
        // FIXME: Timers
        CorruptingStrikeTimer = 5000;
        CurseOfBoundlessAgonyTimer = 15000;
        ShadowBoltVolleyTimer = 10000;

        Banished = false;
        Enraged  = false;

        DoCast(m_creature, SPELL_SPECTRAL_REALM, true);
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_SATH_AGGRO, m_creature);

        Creature* Kalec = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KALECGOS_HUMAN)));
        if (Kalec)
        {
            m_creature->AddThreat(Kalec, 10000000.0f);
            Kalec->AddThreat(m_creature, 10000000.0f);
        }
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth())
        {
            damage = 0;
            DoCast(m_creature, SPELL_BANISH, true);
            Banished = true;

            DoScriptText(SAY_SATH_DEATH, m_creature);

            if (!pInstance)
            {
                error_log(ERROR_INST_DATA);
                return;
            }

            pInstance->SetData(DATA_SET_SPECTRAL_CHECK, 5000);

            Creature* Kalecgos = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_KALECGOS_DRAGON)));
            if (Kalecgos)
            {
                ((boss_kalecgosAI*)Kalecgos->AI())->Checked = false;
                ((boss_kalecgosAI*)Kalecgos->AI())->Uncorrupted = true;
            }
            else error_log(ERROR_KALECGOS_NOT_FOUND);
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_SATH_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SATH_SLAY2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostilTarget() || Banished)
            return;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !Enraged)
        {
            Unit* Kalecgos = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KALECGOS_DRAGON));
            if (Kalecgos)
                Kalecgos->CastSpell(Kalecgos, SPELL_CRAZED_RAGE, true);
            DoCast(m_creature, SPELL_CRAZED_RAGE, true);
            Enraged = true;
        }

        if (CorruptingStrikeTimer < diff)
        {
            if (rand()%2 == 0)
                DoScriptText(SAY_SATH_SPELL2, m_creature);

            DoCast(m_creature->getVictim(), SPELL_CORRUPTING_STRIKE);
            CorruptingStrikeTimer = 13000;
        }else CorruptingStrikeTimer -= diff;

        if (CurseOfBoundlessAgonyTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_CURSE_OF_BOUNDLESS_AGONY);
            CurseOfBoundlessAgonyTimer = 35000;
            DoCast(m_creature, SPELL_SPECTRAL_REALM, true);
        }else CurseOfBoundlessAgonyTimer -= diff;

        if (ShadowBoltVolleyTimer < diff)
        {
            if (rand()%2 == 0)
                DoScriptText(SAY_SATH_SPELL1, m_creature);

            DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolleyTimer = 15000;
        }else ShadowBoltVolleyTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_kalecgos_humanoidAI : public ScriptedAI
{
    boss_kalecgos_humanoidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 RevitalizeTimer;
    uint32 HeroicStrikeTimer;

    bool HasYelled10Percent;
    bool HasYelled20Percent;

    void Reset()
    {
        //TODO: Times!
        RevitalizeTimer = 30000;
        HeroicStrikeTimer = 8000;

        HasYelled10Percent = false;
        HasYelled20Percent = false;

        DoCast(m_creature, SPELL_SPECTRAL_REALM, true);
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_GOOD_AGGRO, m_creature);
    }

    void JustDied(Unit* killer)
    {
        // Whatever happens when Kalec (Half-elf) dies
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostilTarget())
            return;

        if (RevitalizeTimer < diff)
        {
            if (pInstance)
            {
                Unit* pUnit = Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_RANDOM_SPECTRAL_PLAYER));
                if (pUnit)
                    DoCast(pUnit, SPELL_REVITALIZE);
                RevitalizeTimer = 30000;
            }
        }else RevitalizeTimer -= diff;

        if (HeroicStrikeTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_HEROIC_STRIKE);
            HeroicStrikeTimer = 30000;
        }else HeroicStrikeTimer -= diff;

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20) && !HasYelled20Percent)
        {
            DoScriptText(SAY_GOOD_NEAR_DEATH, m_creature);
            HasYelled20Percent = true;
        }

        if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10) && !HasYelled10Percent)
        {
            DoScriptText(SAY_GOOD_NEAR_DEATH2, m_creature);
            HasYelled10Percent = true;
        }
    }
};

bool GOHello_go_spectral_rift(Player* pPlayer, GameObject* _GO)
{
    TeleportToInnerVeil(pPlayer);
    return true;
}

CreatureAI* GetAI_boss_kalecgos(Creature* pCreature)
{
    return new boss_kalecgosAI(pCreature);
}

CreatureAI* GetAI_boss_sathrovarr(Creature* pCreature)
{
    return new boss_sathrovarrAI(pCreature);
}

CreatureAI* GetAI_boss_kalecgos_humanoid(Creature* pCreature)
{
    return new boss_kalecgos_humanoidAI(pCreature);
}

void AddSC_boss_kalecgos()
{
    Script* newscript;

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_kalecgos;
    newscript->Name = "boss_kalecgos";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_sathrovarr;
    newscript->Name = "boss_sathrovarr";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_boss_kalecgos_humanoid;
    newscript->Name = "boss_kalecgos_humanoid";
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->pGOHello = &GOHello_go_spectral_rift;
    newscript->Name = "go_spectral_rift";
    newscript->RegisterSelf();
}

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
SDName: Boss Sartharion
SD%Complete: 40%
SDComment: Abilities, Achievement and control of portals to be implemented
SDCategory: Obsidian Sanctum
EndScriptData */

#include "precompiled.h"
#include "def_obsidian_sanctum.h"

enum
{
    //Sartharion Yell
    SAY_SARTHARION_AGGRO                        = -1615018,
    SAY_SARTHARION_BERSERK                      = -1615019,
    SAY_SARTHARION_BREATH                       = -1615020,
    SAY_SARTHARION_CALL_SHADRON                 = -1615021,
    SAY_SARTHARION_CALL_TENEBRON                = -1615022,
    SAY_SARTHARION_CALL_VESPERON                = -1615023,
    SAY_SARTHARION_DEATH                        = -1615024,
    SAY_SARTHARION_SPECIAL_1                    = -1615025,
    SAY_SARTHARION_SPECIAL_2                    = -1615026,
    SAY_SARTHARION_SPECIAL_3                    = -1615027,
    SAY_SARTHARION_SPECIAL_4                    = -1615028,
    SAY_SARTHARION_SLAY_1                       = -1615029,
    SAY_SARTHARION_SLAY_2                       = -1615030,
    SAY_SARTHARION_SLAY_3                       = -1615031,

    WHISPER_LAVA_CHURN                          = -1615032,

    WHISPER_SHADRON_DICIPLE                     = -1615008,
    WHISPER_VESPERON_DICIPLE                    = -1615041,
    WHISPER_HATCH_EGGS                          = -1615017,
    WHISPER_OPEN_PORTAL                         = -1615042, //whisper, shared by two dragons

    //Sartharion Spells
    SPELL_BERSERK                               = 61632,    // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_CLEAVE                                = 56909,    // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_FLAME_BREATH                          = 56908,    // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_H                        = 58956,    // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
    SPELL_TAIL_LASH                             = 56910,    // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TAIL_LASH_H                           = 58957,    // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
    SPELL_WILL_OF_SARTHARION                    = 61254,    // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
    SPELL_LAVA_STRIKE                           = 57571,
    SPELL_TWILIGHT_REVENGE                      = 60639,

    SPELL_TWILIGHT_SHIFT_ENTER                  = 57620,    // enter phase. Player get this when click GO
    SPELL_TWILIGHT_SHIFT_REMOVAL                = 61187,    // leave phase
    SPELL_TWILIGHT_SHIFT_REMOVAL_ALL            = 61190,    // leave phase (probably version to make all leave)

    //Mini bosses common spells
    SPELL_TWILIGHT_RESIDUE                      = 61885,    // makes immune to shadow damage, applied when leave phase

    //Miniboses (Vesperon, Shadron, Tenebron)
    SPELL_SHADOW_BREATH_H                       = 59126,    // Inflicts 8788 to 10212 Fire damage to enemies in a cone in front of the caster.
    SPELL_SHADOW_BREATH                         = 57570,    // Inflicts 6938 to 8062 Fire damage to enemies in a cone in front of the caster.
    //TAIL LASH the same as Sartharion's
    SPELL_SHADOW_FISSURE_H                      = 59127,    // Deals 9488 to 13512 Shadow damage to any enemy within the Shadow fissure after 5 sec.
    SPELL_SHADOW_FISSURE                        = 57579,    // Deals 6188 to 8812 Shadow damage to any enemy within the Shadow fissure after 5 sec.

    //Vesperon
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal spawns multiple times
    NPC_ACOLYTE_OF_VESPERON                     = 31219,    // Acolyte of Vesperon
    SPELL_POWER_OF_VESPERON                     = 61251,    // Vesperon's presence decreases the maximum health of all enemies by 25%.
    SPELL_TWILIGHT_TORMENT_1                    = 57988,
    SPELL_TWILIGHT_TORMENT_4                    = 58835,

    //Shadron
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal spawns multiple times
    NPC_ACOLYTE_OF_SHADRON                      = 31218,    // Acolyte of Shadron
    SPELL_POWER_OF_SHADRON                      = 58105,    // Shadron's presence increases Fire damage taken by all enemies by 100%.                     
    SPELL_TWILIGHT_TORMENT_2                    = 57835,
    SPELL_TWILIGHT_TORMENT_3                    = 58766,    // on Sartharion

    //Tenebron
    //in the portal spawns 6 eggs, if not killed in time (approx. 20s)  they will hatch,  whelps can cast 60708
    SPELL_POWER_OF_TENEBRON                     = 61248,    // Tenebron's presence increases Shadow damage taken by all enemies by 100%.
    //Tenebron, dummy spell
    SPELL_SUMMON_TWILIGHT_WHELP                 = 58035,    // doesn't work, will spawn NPC_TWILIGHT_WHELP
    SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP      = 58826,    // doesn't work, will spawn NPC_SHARTHARION_TWILIGHT_WHELP
    SPELL_HATCH_EGGS_H                          = 58542,
    SPELL_HATCH_EGGS                            = 58793,

    //Whelps
    NPC_TWILIGHT_WHELP                          = 30890,
    NPC_SHARTHARION_TWILIGHT_WHELP              = 31214,
    NPC_FLAME_TSUNAMI                           = 30616,
    NPC_LAVA_BLAZE                              = 30643,
    SPELL_FADE_ARMOR                            = 60708,    // Reduces the armor of an enemy by 1500 for 15s

    //using these custom points for dragons start and end
    POINT_ID_INIT                               = 100,
    POINT_ID_LAND                               = 200
};

//POS_SARTHARION_X                            = 3246.57,
//POS_SARTHARION_Y                            = 551.263,

struct Waypoint
{
    float m_fX, m_fY, m_fZ;
};

//each dragons special points. First where fly to before connect to connon, second where land point is.
Waypoint m_aTene[]=
{
    {3212.854, 575.597, 109.856},                           //init
    {3246.425, 565.367, 61.249}                             //end
};

Waypoint m_aShad[]=
{
    {3293.238, 472.223, 106.968},
    {3271.669, 526.907, 61.931}
};

Waypoint m_aVesp[]=
{
    {3193.310, 472.861, 102.697},
    {3227.268, 533.238, 59.995}
};

//points around raid "isle", counter clockwise. should probably be adjusted to be more alike
Waypoint m_aDragonCommon[]=
{
    {3214.012, 468.932, 98.652},
    {3244.950, 468.427, 98.652},
    {3283.520, 496.869, 98.652},
    {3287.316, 555.875, 98.652},
    {3250.479, 585.827, 98.652},
    {3209.969, 566.523, 98.652}
};

/*######
## Boss Sartharion
######*/

struct MANGOS_DLL_DECL boss_sartharionAI : public ScriptedAI
{
    boss_sartharionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsHeroic = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroic;

    uint32 m_uiTenebronTimer;
    uint32 m_uiShadronTimer;
    uint32 m_uiVesperonTimer;

    bool m_bHasCalledTenebron;
    bool m_bHasCalledShadron;
    bool m_bHasCalledVesperon;

    void Reset() 
    {
        m_uiTenebronTimer = 30000;
        m_uiShadronTimer = 75000;
        m_uiVesperonTimer = 120000;

        m_bHasCalledTenebron = false;
        m_bHasCalledShadron = false;
        m_bHasCalledVesperon = false;

        if (m_pInstance)
        {
            if (!m_creature->isAlive())
                return;

            m_pInstance->SetData(TYPE_SARTHARION_EVENT, NOT_STARTED);

            //do dragons actually respawn like this?

            if (Unit* Temp1 = Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TENEBRON)))
                if (Temp1->isDead())
                    ((Creature*)Temp1)->Respawn();

            if (Unit* Temp2 = Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_SHADRON)))
                if (Temp2->isDead())
                    ((Creature*)Temp2)->Respawn();

            if (Unit* Temp3 = Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_VESPERON)))
                if (Temp3->isDead())
                    ((Creature*)Temp3)->Respawn();
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SARTHARION_AGGRO,m_creature);
        DoZoneInCombat();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, IN_PROGRESS);

        FetchDragons();
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        //why is this needed?
        if (pWho == m_creature || pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_SARTHARION_DEATH,m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SARTHARION_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SARTHARION_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SARTHARION_SLAY_3, m_creature); break;
        }
    }

    void FetchDragons()
    {
        Unit* pTene = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_TENEBRON));
        Unit* pShad = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_SHADRON));
        Unit* pVesp = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_VESPERON));

        if (pTene && pTene->isAlive() && !pTene->getVictim())
            pTene->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aTene[0].m_fX, m_aTene[0].m_fY, m_aTene[0].m_fZ);

        if (pShad && pShad->isAlive() && !pShad->getVictim())
            pShad->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aShad[0].m_fX, m_aShad[0].m_fY, m_aShad[0].m_fZ);

        if (pVesp && pVesp->isAlive() && !pVesp->getVictim())
            pVesp->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aVesp[0].m_fX, m_aVesp[0].m_fY, m_aVesp[0].m_fZ);
    }

    void UpdateAI(const uint32 uiDiff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // call tenebron
        if (!m_bHasCalledTenebron && m_uiTenebronTimer < uiDiff)
        {
            if (m_pInstance)
            {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TENEBRON));

                if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
                {
                    DoScriptText(SAY_SARTHARION_CALL_TENEBRON, m_creature);
                    pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aTene[1].m_fX, m_aTene[1].m_fY, m_aTene[1].m_fZ);

                    m_bHasCalledTenebron = true;
                }
            }
            m_uiTenebronTimer = 1000;
        }else m_uiTenebronTimer -= uiDiff;

        // call shadron
        if (!m_bHasCalledShadron && m_uiShadronTimer < uiDiff)
        {
            if (m_pInstance)
            {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_SHADRON));
                if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
                {
                    DoScriptText(SAY_SARTHARION_CALL_SHADRON, m_creature);
                    pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aShad[1].m_fX, m_aShad[1].m_fY, m_aShad[1].m_fZ);

                    m_bHasCalledShadron = true;
                }
            }
            m_uiShadronTimer = 1000;
        }else m_uiShadronTimer -= uiDiff;

        // call vesperon
        if (!m_bHasCalledVesperon && m_uiVesperonTimer < uiDiff)
        {
            if (m_pInstance)
            {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_VESPERON));
                if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
                {
                    DoScriptText(SAY_SARTHARION_CALL_VESPERON, m_creature);
                    pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aVesp[1].m_fX, m_aVesp[1].m_fY, m_aVesp[1].m_fZ);

                    m_bHasCalledVesperon = true;
                }
            }
            m_uiVesperonTimer = 1000;
        }else m_uiVesperonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sartharion(Creature* pCreature)
{
    return new boss_sartharionAI(pCreature);
}

//to control each dragons common abilities
struct MANGOS_DLL_DECL dummy_dragonAI : public ScriptedAI
{
    dummy_dragonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsHeroic = pCreature->GetMap()->IsHeroic();
        Reset(); 
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroic;

    uint32 m_uiWaypointId;
    uint32 m_uiMoveNextTimer;
    int32 m_iPortalRespawnTime;
    bool m_bCanMoveFree;

    void Reset()
    {
        m_uiWaypointId = 0;
        m_uiMoveNextTimer = 500;
        m_iPortalRespawnTime = 30000;
        m_bCanMoveFree = false;
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (!m_pInstance || uiType != POINT_MOTION_TYPE)
            return;

        debug_log("dummy_dragonAI: %s reached point %u", m_creature->GetName(), uiPointId);

        //if healers messed up the raid and we was already initialized
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            EnterEvadeMode();
            return;
        }

        //this is end, don't do anything but to clear stack
        if (uiPointId == POINT_ID_LAND)
        {
            m_creature->GetMotionMaster()->Clear(false);
            m_bCanMoveFree = false;
            return;
        }

        //get amount of common points
        uint32 uiCommonWPCount = sizeof(m_aDragonCommon)/sizeof(Waypoint);

        //increase
        m_uiWaypointId = uiPointId+1;

        //if we have reached a point bigger or equal to count, it mean we must reset to point 0
        if (m_uiWaypointId >= uiCommonWPCount)
        {
            if (!m_bCanMoveFree)
                m_bCanMoveFree = true;

            m_uiWaypointId = 0;
        }

        m_uiMoveNextTimer = 500;
    }

    //used when open portal and spawn mobs in phase
    void DoRaidWhisper(int32 iTextId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    DoScriptText(iTextId, m_creature, i->getSource());
            }
        }
    }

    //"opens" the portal and does the "opening" whisper
    void OpenPortal()
    {
        int32 iTextId = 0;

        //there are 4 portal spawn locations, each are expected to be spawned with negative spawntimesecs in database

        //using a grid search here seem to be more efficient than caching all four guids
        //in instance script and calculate range to each.
        GameObject* pPortal = GetClosestGameObjectWithEntry(m_creature,GO_TWILIGHT_PORTAL,50.0f);

        switch(m_creature->GetEntry())
        {
            case NPC_TENEBRON:
                iTextId = WHISPER_HATCH_EGGS;
                break;
            case NPC_SHADRON:
            case NPC_VESPERON:
                iTextId = WHISPER_OPEN_PORTAL;
                break;
        }

        DoRaidWhisper(iTextId);

        //By using SetRespawnTime() we will actually "spawn" the object with our defined time.
        //Once time is up, portal will disappear again.
        if (pPortal && !pPortal->isSpawned())
            pPortal->SetRespawnTime(m_iPortalRespawnTime);

        //Unclear what are expected to happen if one drake has a portal open already
        //Refresh respawnTime so time again are set to 30secs?
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanMoveFree && m_uiMoveNextTimer)
        {
            if (m_uiMoveNextTimer < uiDiff)
            {
                m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId,
                    m_aDragonCommon[m_uiWaypointId].m_fX, m_aDragonCommon[m_uiWaypointId].m_fY, m_aDragonCommon[m_uiWaypointId].m_fZ);

                debug_log("dummy_dragonAI: %s moving to point %u", m_creature->GetName(), m_uiWaypointId);
                m_uiMoveNextTimer = 0;
            } else m_uiMoveNextTimer -= uiDiff;
        }
    }
};

/*######
## Mob Tenebron
######*/

enum
{
    //Yell
    SAY_TENEBRON_AGGRO                      = -1615009,
    SAY_TENEBRON_SLAY_1                     = -1615010,
    SAY_TENEBRON_SLAY_2                     = -1615011,
    SAY_TENEBRON_DEATH                      = -1615012,
    SAY_TENEBRON_BREATH                     = -1615013,
    SAY_TENEBRON_RESPOND                    = -1615014,
    SAY_TENEBRON_SPECIAL_1                  = -1615015,
    SAY_TENEBRON_SPECIAL_2                  = -1615016
};

struct MANGOS_DLL_DECL mob_tenebronAI : public dummy_dragonAI
{
    mob_tenebronAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    void Reset()
    {
    }

    void Aggro(Unit* pWho) 
    {
        DoScriptText(SAY_TENEBRON_AGGRO,m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_TENEBRON_DEATH,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_TENEBRON_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_TENEBRON_SLAY_2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_tenebron(Creature* pCreature)
{
    return new mob_tenebronAI(pCreature);
}

/*######
## Mob Shadron
######*/

enum
{
    //Shadron Yell
    SAY_SHADRON_AGGRO                   = -1615000,
    SAY_SHADRON_SLAY_1                  = -1615001,
    SAY_SHADRON_SLAY_2                  = -1615002,
    SAY_SHADRON_DEATH                   = -1615003,
    SAY_SHADRON_BREATH                  = -1615004,
    SAY_SHADRON_RESPOND                 = -1615005,
    SAY_SHADRON_SPECIAL_1               = -1615006,
    SAY_SHADRON_SPECIAL_2               = -1615007
};

struct MANGOS_DLL_DECL mob_shadronAI : public dummy_dragonAI
{
    mob_shadronAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    void Reset()
    {
    }

    void Aggro(Unit* pWho) 
    {
        DoScriptText(SAY_SHADRON_AGGRO,m_creature);
    }

    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_SHADRON_DEATH,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_SHADRON_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SHADRON_SLAY_2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_shadron(Creature* pCreature)
{
    return new mob_shadronAI(pCreature);
}

/*######
## Mob Vesperon
######*/

enum
{
    //Vesperon Yell
    SAY_VESPERON_AGGRO                      = -1615033,
    SAY_VESPERON_SLAY_1                     = -1615034,
    SAY_VESPERON_SLAY_2                     = -1615035,
    SAY_VESPERON_DEATH                      = -1615036,
    SAY_VESPERON_BREATH                     = -1615037,
    SAY_VESPERON_RESPOND                    = -1615038,
    SAY_VESPERON_SPECIAL_1                  = -1615039,
    SAY_VESPERON_SPECIAL_2                  = -1615040
};

struct MANGOS_DLL_DECL mob_vesperonAI : public dummy_dragonAI
{
    mob_vesperonAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    void Reset()
    {
    }

    void Aggro(Unit* pWho) 
    {
        DoScriptText(SAY_VESPERON_AGGRO,m_creature);
    }

    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_VESPERON_DEATH,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_VESPERON_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_VESPERON_SLAY_2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_vesperon(Creature* pCreature)
{
    return new mob_vesperonAI(pCreature);
}

/*######
## Mob Acolyte of Shadron
######*/

struct MANGOS_DLL_DECL mob_acolyte_of_shadronAI : public ScriptedAI
{
    mob_acolyte_of_shadronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_shadron(Creature* pCreature)
{
    return new mob_acolyte_of_shadronAI(pCreature);
}

/*######
## Mob Acolyte of Vesperon
######*/

struct MANGOS_DLL_DECL mob_acolyte_of_vesperonAI : public ScriptedAI
{
    mob_acolyte_of_vesperonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_vesperon(Creature* pCreature)
{
    return new mob_acolyte_of_vesperonAI(pCreature);
}

/*######
## Mob Twilight Eggs
######*/

struct MANGOS_DLL_DECL mob_twilight_eggsAI : public ScriptedAI
{
    mob_twilight_eggsAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset()
    {
    }

    void AttackStart(Unit* pWho) { }
    void MoveInLineOfSight(Unit* pWho) { }
};

CreatureAI* GetAI_mob_twilight_eggs(Creature* pCreature)
{
    return new mob_twilight_eggsAI(pCreature);
}

/*######
## Mob Twilight Whelps
######*/

struct MANGOS_DLL_DECL mob_twilight_whelpAI : public ScriptedAI
{
    mob_twilight_whelpAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_twilight_whelp(Creature* pCreature)
{
    return new mob_twilight_whelpAI(pCreature);
}

void AddSC_boss_sartharion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_sartharion";
    newscript->GetAI = &GetAI_boss_sartharion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_vesperon";
    newscript->GetAI = &GetAI_mob_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_shadron";
    newscript->GetAI = &GetAI_mob_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_tenebron";
    newscript->GetAI = &GetAI_mob_tenebron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_acolyte_of_shadron";
    newscript->GetAI = &GetAI_mob_acolyte_of_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_acolyte_of_vesperon";
    newscript->GetAI = &GetAI_mob_acolyte_of_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_twilight_eggs";
    newscript->GetAI = &GetAI_mob_twilight_eggs;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_twilight_whelp";
    newscript->GetAI = &GetAI_mob_twilight_whelp;
    newscript->RegisterSelf();
}

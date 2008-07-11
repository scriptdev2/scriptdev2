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
SDName: Mobs_Shadowmoon_Valley
SD%Complete: 100
SDComment: Quest support: 10804, 10854. 
SDCategory: Shadowmoon Valley
EndScriptData */

#include "sc_creature.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

/*#####
# mob_mature_netherwing_drake
#####*/

#define SPELL_PLACE_CARCASS             38439
#define SPELL_JUST_EATEN                38502
#define SPELL_NETHER_BREATH             38467

#define SAY_JUST_EATEN              "Thank you, mortal."

struct MANGOS_DLL_DECL mob_mature_netherwing_drakeAI : public ScriptedAI
{
    mob_mature_netherwing_drakeAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
        PlayerGUID = 0;
    }

    uint64 PlayerGUID;

    bool IsEating;
    bool Evade;

    uint32 ResetTimer;
    uint32 CastTimer;
    uint32 EatTimer;

    void Reset() 
    {
        IsEating = false;
        Evade = false;

        ResetTimer = 120000;
        EatTimer = 5000;
        CastTimer = 5000;
    }

    void Aggro(Unit* who) { }

    void MoveInLineOfSight(Unit* who)
    {
        if(m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if(!caster)
            return;

        if(caster->GetTypeId() == TYPEID_PLAYER && spell->Id == SPELL_PLACE_CARCASS && !m_creature->HasAura(SPELL_JUST_EATEN, 0) && !PlayerGUID)
        {
            float PlayerX, PlayerY, PlayerZ;
            caster->GetClosePoint(PlayerX, PlayerY, PlayerZ, m_creature->GetObjectSize());
            m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
            m_creature->GetMotionMaster()->MovePoint(1, PlayerX, PlayerY, PlayerZ);
            PlayerGUID = caster->GetGUID();
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type != POINT_MOTION_TYPE)
            return;

        if(id == 1)
        {
            IsEating = true;
            EatTimer = 5000;
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_ATTACKUNARMED);
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(IsEating)
            if(EatTimer < diff)
            {
                IsEating = false;
                DoCast(m_creature, SPELL_JUST_EATEN);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                DoSay(SAY_JUST_EATEN, LANG_DRACONIC, NULL);
                if(PlayerGUID)
                {
                    Player* plr = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
                    if(plr && plr->GetQuestStatus(10804) == QUEST_STATUS_INCOMPLETE)
                    {
                        plr->KilledMonster(22131, m_creature->GetGUID());
                        Evade = true;
                        PlayerGUID = 0;
                    }
                }
            }else EatTimer -= diff;

        if(Evade)
            if(ResetTimer < diff)
                EnterEvadeMode();
            else ResetTimer -= diff;

            if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
                return;

            if(CastTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_NETHER_BREATH);
                CastTimer = 5000;
            }else CastTimer -= diff;

            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mature_netherwing_drake(Creature *_creature)
{
    return new mob_mature_netherwing_drakeAI(_creature);
}

/*###
# mob_enslaved_netherwing_drake
####*/

#define FACTION_DEFAULT     62
#define FACTION_FRIENDLY    1840 // Not sure if this is correct, it was taken off of Mordenai.

#define SPELL_HIT_FORCE_OF_NELTHARAKU   38762
#define SPELL_FORCE_OF_NELTHARAKU       38775

#define CREATURE_DRAGONMAW_SUBJUGATOR   21718
#define CREATURE_ESCAPE_DUMMY           22317

struct MANGOS_DLL_DECL mob_enslaved_netherwing_drakeAI : public ScriptedAI
{
    mob_enslaved_netherwing_drakeAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
        PlayerGUID = 0;
        Tapped = false;
    }

    uint64 PlayerGUID;
    uint32 FlyTimer;
    bool Tapped;

    void Reset()
    {
        if(!Tapped)
            m_creature->setFaction(FACTION_DEFAULT);

        FlyTimer = 10000;
        m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
        m_creature->SetVisibility(VISIBILITY_ON);
    }

    void Aggro(Unit* who) { }

    Creature* SelectCreatureInGrid(uint32 entry, float range)
    {
        Creature* pCreature = NULL;

        // Time for some omg mind blowing code to search for creature
        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck creature_check(*m_creature, entry, true, range);
        MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(pCreature, creature_check);

        TypeContainerVisitor<MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck>, GridTypeMapContainer> creature_searcher(searcher);

        CellLock<GridReadGuard> cell_lock(cell, pair);
        cell_lock->Visit(cell_lock, creature_searcher,*(m_creature->GetMap()));

        return pCreature;
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if(!caster)
            return;

        if(caster->GetTypeId() == TYPEID_PLAYER && spell->Id == SPELL_HIT_FORCE_OF_NELTHARAKU && !Tapped)
        {
            Tapped = true;
            PlayerGUID = caster->GetGUID();

            m_creature->setFaction(FACTION_FRIENDLY);
            DoCast(caster, SPELL_FORCE_OF_NELTHARAKU, true);

            Creature* Dragonmaw = SelectCreatureInGrid(CREATURE_DRAGONMAW_SUBJUGATOR, 50);

            if(Dragonmaw)
            {
                m_creature->AddThreat(Dragonmaw, 100000.0f);
                AttackStart(Dragonmaw);
            }

            HostilReference* ref = m_creature->getThreatManager().getOnlineContainer().getReferenceByTarget(caster);
            if(ref)
                ref->removeReference();
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type != POINT_MOTION_TYPE)
            return;

        if(id == 1)
        {
            if(PlayerGUID)
            {
                Unit* plr = Unit::GetUnit((*m_creature), PlayerGUID);
                if(plr)
                    DoCast(plr, SPELL_FORCE_OF_NELTHARAKU, true);

                PlayerGUID = 0;
            }
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_creature->RemoveCorpse();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
        {
            if(Tapped)
                if(FlyTimer < diff)
                {
                    Tapped = false;
                    if(PlayerGUID)
                    {
                        Player* plr = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
                        if(plr && plr->GetQuestStatus(10854) == QUEST_STATUS_INCOMPLETE)
                        {
                            plr->KilledMonster(22316, m_creature->GetGUID());
                            /*
                            float x,y,z;
                            m_creature->GetPosition(x,y,z);

                            float dx,dy,dz;
                            m_creature->GetRandomPoint(x, y, z, 20, dx, dy, dz);
                            dz += 20; // so it's in the air, not ground*/

                            float dx, dy, dz;

                            Creature* EscapeDummy = SelectCreatureInGrid(CREATURE_ESCAPE_DUMMY, 30);
                            if(EscapeDummy)
                                EscapeDummy->GetPosition(dx, dy, dz);
                            else
                            {
                                m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 20, dx, dy, dz);
                                dz += 25;
                            }

                            m_creature->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT + MOVEMENTFLAG_LEVITATING);
                            m_creature->GetMotionMaster()->MovePoint(1, dx, dy, dz);
                        }
                    }
                }else FlyTimer -= diff;
            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_enslaved_netherwing_drake(Creature* _Creature)
{
    return new mob_enslaved_netherwing_drakeAI(_Creature);
}

/*#####
# mob_dragonmaw_peon
#####*/

struct MANGOS_DLL_DECL mob_dragonmaw_peonAI : public ScriptedAI
{
    mob_dragonmaw_peonAI(Creature* c) : ScriptedAI(c)
    {
        Reset();
    }

    uint64 PlayerGUID;

    bool Tapped;

    uint32 PoisonTimer;

    void Reset()
    {
        PlayerGUID = 0;

        Tapped = false;

        PoisonTimer = 0;
    }

    void Aggro(Unit* who) { }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if(!caster)
            return;

        if(caster->GetTypeId() == TYPEID_PLAYER && spell->Id == 40468 && !Tapped)
        {
            PlayerGUID = caster->GetGUID();

            Tapped = true;
            float x, y, z;
            caster->GetClosePoint(x, y, z, m_creature->GetObjectSize());

            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
            m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type != POINT_MOTION_TYPE)
            return;

        if(id)
        {
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_EAT);
            PoisonTimer = 15000;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(PoisonTimer)
            if(PoisonTimer <= diff)
            {
                if(PlayerGUID)
                {
                    Player* plr = ((Player*)Unit::GetUnit((*m_creature), PlayerGUID));
                    if(plr && plr->GetQuestStatus(11020) == QUEST_STATUS_INCOMPLETE)
                        plr->KilledMonster(23209, m_creature->GetGUID());
                }
                PoisonTimer = 0;
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }else PoisonTimer -= diff;
    }
};
/*######
## AddSC
######*/

void AddSC_mobs_shadowmoon_valley()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_mature_netherwing_drake";
    newscript->GetAI = GetAI_mob_mature_netherwing_drake;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name = "mob_enslaved_netherwing_drake";
    newscript->GetAI = GetAI_mob_enslaved_netherwing_drake;
    m_scripts[nrscripts++] = newscript;
}

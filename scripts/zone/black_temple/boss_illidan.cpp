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
#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../../../../shared/WorldPacket.h"
#include "../../../../../game/SpellAuras.h"
#include "../../../../../game/Player.h"
#include "../../../../../game/GossipDef.h"

// Intro speech with Akama
#define SAY_INTRO_ILL1        "Akama... your duplicity is hardly surprising. I should have slaughtered you and your malformed brethren long ago."
#define SOUND_INTRO_ILL1    11463 // ~13 seconds
#define SAY_INTRO_AKA1        "We've come to end your reign, Illidan. My people and all of Outland shall be free!"
#define SOUND_INTRO_AKA1    11389 // ~12 seconds
#define SAY_INTRO_ILL2        "Boldly said. But I remain unconvinced."
#define SOUND_INTRO_ILL2    11464 // ~8 seconds
#define SAY_INTRO_AKA2        "The time has come! The moment is at hand!"
#define SOUND_INTRO_AKA2    11380 // ~5 seconds
#define SAY_INTRO_ILL3           "You are not prepared!"
#define SOUND_INTRO_ILL3            11466

#define GOSSIP_ITEM            "We wish to fight Illidan"

#define SAY_AKAMA_MINION    "Come, my minions. Deal with this traitor as he deserves!"
#define SOUND_AKAMA_MINION    11465

#define SAY_ATT_MAIEV        "Feel the hatred of ten thousand years!"
#define SOUND_ATT_MAIEV        11470

#define SAY_KILL1            "Who shall be next to taste my blades?!"
#define SOUND_KILL1            11473
#define SAY_KILL2            "This is too easy!"
#define SOUND_KILL2            11472

#define SAY_PHASE2            "Behold the flames of Azzinoth!"
#define SOUND_PHASE2        11480

#define SAY_BEAM            "Stare into the eyes of the Betrayer!"
#define SOUND_BEAM            11481

#define SAY_MORPH            "Behold the power... of the demon within!"
#define SOUND_MORPH            11475

#define SAY_ENRAGE            "You've wasted too much time mortals, now you shall fall!"
#define SOUND_ENRAGE        11474

// Speech when Maiev enters
#define SAY_SUMMON_MAIEV    "Is this it, mortals? Is this all the fury you can muster?"
#define SOUND_SUMMON_MAIEV    11476 // ~8 seconds
#define SAY_MAIEV1            "Their fury pales before mine, Illidan. We have some unsettled business between us."
#define SOUND_MAIEV1        11491 // ~9 seconds
#define SAY_MAIEV_ILL2        "Maiev... How is this even possible?"
#define SOUND_MAIEV_ILL2    11477 // ~7 seconds
#define SAY_MAIEV2            "Ah, my long hunt is finally over. Today, Justice will be done!"
#define SOUND_MAIEV2        11492 // ~10 seconds

// Death speech
#define SAY_DEATH_MAIEV1    "Ahh... It is finished. You are beaten."
#define SOUND_DEATH_MAIEV1    11496 // ~6 seconds
#define SAY_DEATH_ILL        "You have won... Maiev. But the huntress... is nothing without the hunt. You... are nothing... without me."
#define SOUND_DEATH_ILL        11478 // ~22 seconds
#define SAY_DEATH_MAIEV2    "He's right. I feel nothing. I am nothing. Farewell, champions."
#define SOUND_DEATH_MAIEV2    11497 // ~9 seconds
#define SAY_DEATH_AKA        "The Light will fill these dismal halls once again. I swear it."
#define SOUND_DEATH_AKA        11387 // ~8 seconds

/* Spells */
#define SPELL_ILLIDAN_KNEEL                39656
#define SPELL_PARASITIC                41914
#define SPELL_FLAME_CRASH            40832
#define SPELL_SHEAR                    41032
#define SPELL_DRAW_SOUL                40904
#define SPELL_FLAME_BURST            41126
#define SPELL_FLAME_CRASH_EFFECT    40836
#define SPELL_AGONIZING_FLAMES        40932
#define SPELL_FIREBALL                40598
#define SPELL_EYEBLAST                40017
#define SPELL_MOLTENFLAME            40253
#define SPELL_DEMON_FIRE            40029
#define SPELL_DEMON_FORM            40506
#define SPELL_SHADOW_BLAST            41078
#define SPELL_DARK_BARRAGE            40585
#define SPELL_SHADOW_PRISON            40647
#define SPELL_DEATH_EFFECT            40220

#define SPELL_MAIEV_BLINK            41221
#define SPELL_CAGE_TRAP                    40760

#define SPELL_POTION                    40545

// Flames of Azzinoth uses this
#define SPELL_FLAME_BLAST            40631
#define SPELL_SUMMON_BLAZE           40637
#define SPELL_BLAZE                  40610

#define DEMON_FORM_DISPLAYID        21322
#define NORMAL_FORM_DISPLAYID        21135

/* Coordinates for summons */
#define ILLIDAN_X          671.840
#define ILLIDAN_Y          304.100
#define ILLIDAN_Z          353.192

#define POS_Z            354

#define X_GLAIVE1        678
#define Y_GLAIVE1        287

#define X_FLAME1         677
#define Y_FLAME1         290

#define X_GLAIVE2        678
#define Y_GLAIVE2        321

#define X_FLAME2         677
#define Y_FLAME2         319

#define MAIEV_X          692
#define MAIEV_Y          351
#define MAIEV_Z          352

#define FLY_X1            692
#define FLY_Y1            331
#define FLY_Z1            353

#define FLY_X2            713
#define FLY_Y2            309
#define FLY_Z2            366

#define FLY_X3            687
#define FLY_Y3            271
#define FLY_Z3            353

#define FLY_X4            650
#define FLY_Y4            285
#define FLY_Z4            353

#define FLY_X5            653
#define FLY_Y5            331
#define FLY_Z5            371

// Illidan Glaive - 21431
//Flame of Azzinoth - 21320
// Dark Illidan - 21322
struct MANGOS_DLL_DECL boss_illidanAI : public ScriptedAI
{
    boss_illidanAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint64 Flame1GUID;
    uint64 Flame2GUID;
    uint64 Glaive1GUID;
    uint64 Glaive2GUID;
    uint64 MaievGUID;
    uint64 AkamaGUID;

    uint32 ShearTimer;
    uint32 FlameCrashTimer;
    uint32 DrawSoulTimer;
    uint32 ParasiticTimer;
    uint32 FlameBurstTimer;
    uint32 FireballTimer;
    uint32 EyeBlastTimer;
    uint32 AgonizingFlamesTimer;
    uint32 ShadowBlastTimer;
    uint32 DarkBarrageTimer;
    uint32 SummonTimer;
    uint32 DemonFormTimer;
    uint32 FlyTimer;

    uint32 DeadFlameCount;
    uint32 TalkTimer;
    uint32 TalkCount;

    uint32 phase;

    bool FlameOne;
    bool FlameTwo;
    bool DemonForm;
    bool HasSummoned;
    bool IsTalking;

    bool InCombat;

    void EnterEvadeMode();
    void MoveInLineOfSight(Unit *who);
    void AttackStart(Unit *who);
    void UpdateAI(const uint32 diff);
    void ResetThreat();
    void KilledUnit(Unit *victim);
    void JustDied(Unit *killer);
    void SpecialMove(float X, float Y, float Z, uint32 Time, Creature *creature);
};

void boss_illidanAI::EnterEvadeMode()
{
    Flame1GUID = 0;
    Flame2GUID = 0;
    Glaive1GUID = 0;
    Glaive2GUID = 0;
    MaievGUID = 0;
    AkamaGUID = 0;

    ShearTimer = 20000;
    FlameCrashTimer = 30000;
    DrawSoulTimer = 45000;
    ParasiticTimer = 50000;
    FireballTimer = 5000;
    EyeBlastTimer = 30000;
    FlameBurstTimer = 30000;
    AgonizingFlamesTimer = 25000;
    DarkBarrageTimer = 15000;
    ShadowBlastTimer = 5000;
    DemonFormTimer = 60000;
    SummonTimer = 35000;
    FlyTimer = 15000;

    TalkTimer = 0;
    TalkCount = 0;

    phase = 1;

    DemonForm = false;
    IsTalking = false;
    HasSummoned = false;

    InCombat = false;

    m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
    m_creature->SetHover(false);
    (*m_creature).GetMotionMaster()->Clear(false);
    m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, NORMAL_FORM_DISPLAYID);
    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    // m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    m_creature->RemoveAllAuras();
    m_creature->DeleteThreatList();
    m_creature->CombatStop();
    DoGoHome();
}

void boss_illidanAI::KilledUnit(Unit *victim)
{
    switch(rand()%2)
    {
    case 0:
        DoYell(SAY_KILL1, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_KILL1);
        break;
    case 1:
        DoYell(SAY_KILL2, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_KILL2);
        break;
    }
}

void boss_illidanAI::JustDied(Unit* killer)
{
    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}


void boss_illidanAI::AttackStart(Unit *who)
{
    if (!who)
        return;

    if (who->isTargetableForAttack() && who!= m_creature)
    {
        //Begin melee attack if we are within range
        DoStartMeleeAttack(who);

        if (!InCombat)
        {
            DoYell(SAY_INTRO_ILL3, LANG_UNIVERSAL, NULL);
            m_creature->RemoveAura(SPELL_ILLIDAN_KNEEL, 0);
            InCombat = true;
        }
    }
}

void boss_illidanAI::MoveInLineOfSight(Unit *who)
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

            if (!InCombat)
            {
                DoYell(SAY_INTRO_ILL3, LANG_UNIVERSAL, NULL);
                m_creature->RemoveAura(SPELL_ILLIDAN_KNEEL, 0);
                InCombat = true;
            }
        }
    }
}

void boss_illidanAI::UpdateAI(const uint32 diff)
{
    //Return since we have no target
    if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
    {
        if(!m_creature->HasAura(SPELL_ILLIDAN_KNEEL, 0))
            DoCast(m_creature, SPELL_ILLIDAN_KNEEL);
        return;
    }

    if(IsTalking)
    {
        if(TalkTimer < diff)
        {
            switch(TalkCount)
            {
            case 0:
                DoCast(m_creature->getVictim(), SPELL_SHADOW_PRISON);
                m_creature->HandleEmoteCommand(402);
                m_creature->RemoveAllAuras();
                DoYell(SAY_SUMMON_MAIEV, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_SUMMON_MAIEV);
                TalkTimer = 7000;
                TalkCount++;
                break;
            case 1:
                DoTextEmote("has finished talking", m_creature);
                if(MaievGUID)
                {
                    Creature* Maiev = NULL;
                    Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                    if(Maiev && (Maiev->isAlive()))
                    {
                        Maiev->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        Maiev->Yell(SAY_MAIEV1, LANG_UNIVERSAL, 0);
                        DoPlaySoundToSet(m_creature, SOUND_MAIEV1);
                        TalkTimer = 8000;
                        TalkCount++;
                    }
                }
                break;
            case 2:
                m_creature->HandleEmoteCommand(407);
                DoYell(SAY_MAIEV_ILL2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_MAIEV_ILL2);
                TalkTimer = 6000;
                TalkCount++;
                break;
            case 3:
                if(MaievGUID)
                {
                    Creature* Maiev = NULL;
                    Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                    if(Maiev && (Maiev->isAlive()))
                    {
                        Maiev->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        Maiev->Yell(SAY_MAIEV2, LANG_UNIVERSAL, 0);
                        DoPlaySoundToSet(m_creature, SOUND_MAIEV2);
                        TalkTimer = 8000;
                        TalkCount++;
                    }
                }
                break;
            case 4:
                if(MaievGUID)
                {
                    Creature* Maiev = NULL;
                    Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                    if(Maiev && (Maiev->isAlive()))
                    {
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Maiev->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Maiev->AI()->AttackStart(m_creature);
                        m_creature->AI()->AttackStart(Maiev);
                        (*m_creature).GetMotionMaster()->Clear(false);
                        phase = 5;
                        IsTalking = false;
                    }
                }
                break;
            case 5:
                if(MaievGUID)
                {
                    Creature* Maiev = NULL;
                    Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                    if(Maiev && (Maiev->isAlive()))
                    {
                        m_creature->CombatStop();
                        Maiev->CombatStop();
                        (*m_creature).GetMotionMaster()->Clear(false);
                        (*m_creature).GetMotionMaster()->Idle();
                        (*Maiev).GetMotionMaster()->Clear(false);
                        (*Maiev).GetMotionMaster()->Idle();
                        DoCast(m_creature, SPELL_DEATH_EFFECT);
                        Maiev->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                        Maiev->Yell(SAY_DEATH_MAIEV1, LANG_UNIVERSAL, 0);
                        DoPlaySoundToSet(m_creature, SOUND_DEATH_MAIEV1);
                        TalkTimer = 7000;
                        TalkCount++;
                    }
                }
                break;
            case 6:
                DoYell(SAY_DEATH_ILL, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_DEATH_ILL);
                TalkTimer = 22000;
                TalkCount++;
                break;
            case 7:
                if(MaievGUID)
                {
                    Creature* Maiev = NULL;
                    Maiev = ((Creature*)Unit::GetUnit((*m_creature), MaievGUID));
                    if(Maiev && (Maiev->isAlive()))
                    {
                        Maiev->Yell(SAY_DEATH_MAIEV2, LANG_UNIVERSAL, 0);
                        DoPlaySoundToSet(m_creature, SOUND_DEATH_MAIEV2);
                        Creature* Akama = NULL;
                        Akama = m_creature->SummonCreature(21700, (Maiev->GetPositionX()+5), Maiev->GetPositionY(), Maiev->GetPositionZ(), 5, TEMPSUMMON_TIMED_DESPAWN, 20000);
                        if(Akama)
                            AkamaGUID = Akama->GetGUID();
                        TalkTimer = 9000;
                        TalkCount++;
                    }
                }
                break;
            case 8:
                if(AkamaGUID)
                {
                    Creature* Akama = NULL;
                    Akama = ((Creature*)Unit::GetUnit((*m_creature), AkamaGUID));
                    if(Akama && (Akama->isAlive()))
                    {
                        Akama->Yell(SAY_DEATH_AKA, LANG_UNIVERSAL, m_creature->GetGUID());
                        DoPlaySoundToSet(m_creature, SOUND_DEATH_AKA);
                        m_creature->getVictim()->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
                        TalkTimer = 8000;
                    }
                }
                break;
            }
        }else TalkTimer -= diff;
        return;
    }

    if((!m_creature->isHover()) && (phase != 2) && (!IsTalking) && (!DemonForm))
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);

    if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 65) && (phase == 1))
    {
        Creature* Glaive1 = NULL;
        Creature* Glaive2 = NULL;
        Creature* Flame1 = NULL;
        Creature* Flame2 = NULL;

        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
        DoYell(SAY_PHASE2, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_PHASE2);
        DoTextEmote("is entering phase 2", m_creature);
        phase = 2;
        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
        (*m_creature).GetMotionMaster()->Clear(false);
        (*m_creature).GetMotionMaster()->Idle();
        m_creature->SetHover(true);
        m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 0);

        Glaive1 = m_creature->SummonCreature(22824, X_GLAIVE1, Y_GLAIVE1, POS_Z, 5, TEMPSUMMON_CORPSE_DESPAWN, 10000);
        Glaive2 = m_creature->SummonCreature(22824, X_GLAIVE2, Y_GLAIVE2, POS_Z, 5, TEMPSUMMON_CORPSE_DESPAWN, 10000);
        Flame1 = m_creature->SummonCreature(22997, X_GLAIVE1+15, Y_GLAIVE1+15, POS_Z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
        Flame2 = m_creature->SummonCreature(22997, X_GLAIVE2-15, Y_GLAIVE2-15, POS_Z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
        if(Glaive1 && Flame1)
        {
            Glaive1GUID = Glaive1->GetGUID();
            Flame1GUID = Flame1->GetGUID();
            Glaive1->CastSpell(Flame1, 40227, true);
        }
        if(Glaive2 && Flame2)
        {
            Glaive2GUID = Glaive2->GetGUID();
            Flame2GUID = Flame2->GetGUID();
            Glaive2->CastSpell(Flame2, 40227, true);
        }
        HasSummoned = true;
    }           

    if(HasSummoned && (phase == 2))
    {
        if(Flame1GUID)
        {
            Creature* Flame1 = NULL;
            Flame1 = ((Creature*)Unit::GetUnit((*m_creature), Flame1GUID));

            if(Flame1 && (!Flame1->isAlive()))
            {                   
                m_creature->SetHealth(m_creature->GetMaxHealth()*0.6);
                DeadFlameCount++;
                Flame1GUID = 0;
            }
        }

        if(Flame2GUID)
        {
            Creature* Flame2 = NULL;
            Flame2 = ((Creature*)Unit::GetUnit((*m_creature), Flame2GUID));

            if(Flame2 && (!Flame2->isAlive()))
            {                   
                m_creature->SetHealth(m_creature->GetMaxHealth()*0.6);
                DeadFlameCount++;
                Flame2GUID = 0;
            }
        }

        if(!Flame1GUID && !Flame2GUID)
        {
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            DoTextEmote("is entering phase 3", m_creature);
            phase = 3;

            m_creature->SetHealth(m_creature->GetMaxHealth()*0.6);

            if(Glaive1GUID)
            {
                Unit* Glaive1 = NULL;
                Glaive1 = Unit::GetUnit((*m_creature), Glaive1GUID);
                if(Glaive1)
                    Glaive1->DealDamage(Glaive1, Glaive1->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
            }
            if(Glaive2GUID)
            {
                Unit* Glaive2 = NULL;
                Glaive2 = Unit::GetUnit((*m_creature), Glaive2GUID);
                if(Glaive2)
                    Glaive2->DealDamage(Glaive2, Glaive2->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
            }

            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
            m_creature->SetHover(false);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 45479);
            m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY+1, 45481);
        }
    }

    if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30) && (phase == 3))
    {
        m_creature->RemoveAllAuras();
        DoYell("Entering Phase 4!", LANG_UNIVERSAL, NULL);
        DoTextEmote("is entering phase 4", m_creature);
        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
        Creature* Maiev = NULL;
        Maiev = m_creature->SummonCreature(23197, (m_creature->GetPositionX() + 10), (m_creature->GetPositionY() + 5), m_creature->GetPositionZ(), 5, TEMPSUMMON_DEAD_DESPAWN, 10000);
        if(Maiev)
        {
            MaievGUID = Maiev->GetGUID();
            DoCast(Maiev, SPELL_MAIEV_BLINK);
            Maiev->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->Idle();
            DemonForm = false;
            IsTalking = true;
            TalkTimer = 0;
            TalkCount = 0;
            phase = 4;
            Maiev->SetInFront(m_creature);
            m_creature->SetInFront(Maiev);
        }
    }

    if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 5) && (phase == 5))
    {
        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
        DoYell("ILLIDAN MUST DIE!!..wait what? That's me! NOO!!", LANG_UNIVERSAL, NULL);
        m_creature->RemoveAllAuras();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if(MaievGUID)
        {
            Unit* Maiev = NULL;
            Maiev = Unit::GetUnit((*m_creature), MaievGUID);
            if(Maiev)
            {
                (*m_creature).GetMotionMaster()->Clear(false);
                (*m_creature).GetMotionMaster()->Idle();
                (*Maiev).GetMotionMaster()->Clear(false);
                (*Maiev).GetMotionMaster()->Idle();
                IsTalking = true;
                TalkTimer = 0;
                TalkCount = 5;
                phase = 6;
            }
        }
    }

    if(phase != 2 && (!DemonForm))
    {
        if(ShearTimer < diff)
        {
            m_creature->StopMoving();
            DoYell("SHEAR!", LANG_UNIVERSAL, NULL);
            DoCast(m_creature->getVictim(), SPELL_SHEAR);
            ShearTimer = 20000;
        }else ShearTimer -= diff;

        if(FlameCrashTimer < diff)
        {
            m_creature->StopMoving();
            DoYell("FLAME CRASH!", LANG_UNIVERSAL, NULL);
            DoCast(m_creature->getVictim(), SPELL_FLAME_CRASH);
            FlameCrashTimer = 35000;
        }else FlameCrashTimer -= diff;

        if(DrawSoulTimer < diff)
        {
            DoYell("Draw Soul!", LANG_UNIVERSAL, NULL);

            DoCast(m_creature->getVictim(), SPELL_DRAW_SOUL);

            DrawSoulTimer = 60000;
        }else DrawSoulTimer -= diff;

        if(ParasiticTimer < diff)
        {
            DoYell("Parasitic Shadowfiend!", LANG_UNIVERSAL, NULL);

            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
                DoCast(target, SPELL_PARASITIC);

            ParasiticTimer = 50000;
        }else ParasiticTimer -= diff;

        if(m_creature->isAttackReady()) m_creature->HandleEmoteCommand(406);
        DoMeleeAttackIfReady();
    }

    if(phase == 2)
    {
        if(!m_creature->isHover())
        {
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            m_creature->SetHover(true);
        }

        if(FlyTimer < diff)
        {
            DoTextEmote("is flying to a new position", NULL);
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            switch(rand()%5)
            {
            case 0:
                SpecialMove(FLY_X1, FLY_Y1, FLY_Z1, 5000, m_creature);
                m_creature->Relocate(FLY_X1, FLY_Y1, FLY_Z1);
                break;
            case 1:
                SpecialMove(FLY_X2, FLY_Y2, FLY_Z2, 5000, m_creature);
                m_creature->Relocate(FLY_X2, FLY_Y2, FLY_Z2);
                break;
            case 2:
                SpecialMove(FLY_X3, FLY_Y3, FLY_Z3, 5000, m_creature);
                m_creature->Relocate(FLY_X3, FLY_Y3, FLY_Z3);
                break;
            case 3:
                SpecialMove(FLY_X4, FLY_Y4, FLY_Z4, 5000, m_creature);
                m_creature->Relocate(FLY_X4, FLY_Y4, FLY_Z4);
                break;
            case 4:
                SpecialMove(FLY_X5, FLY_Y5, FLY_Z5, 5000, m_creature);
                m_creature->Relocate(FLY_X5, FLY_Y5, FLY_Z5);
                break;
            }
            FlyTimer = 20000;
            FireballTimer += 5000;
            DarkBarrageTimer += 5000;
        }else FlyTimer -= diff;

        if(FireballTimer < diff)
        {
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_FIREBALL);

            FireballTimer = 7000;
        }else FireballTimer -= diff;

        if(EyeBlastTimer < diff)
        {
            Creature* BlastTarget;
            BlastTarget = m_creature->SummonCreature(12999, (float)658, (float)329, (float)353, 5, TEMPSUMMON_TIMED_DESPAWN, 30000);
            if(BlastTarget)
            {
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                DoTextEmote("gets a green glow around his eyes.", m_creature);
                DoYell(SAY_BEAM, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_BEAM);
                DoCast(BlastTarget, SPELL_EYEBLAST);
            }
            EyeBlastTimer = 90000;
        }else EyeBlastTimer -= diff;

        if(DarkBarrageTimer < diff)
        {
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            DoYell("DARK BARRAGE!", LANG_UNIVERSAL, NULL);
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_DARK_BARRAGE);

            DarkBarrageTimer = 60000;
            FireballTimer += 10000;
            FlyTimer += 10000;
        }else DarkBarrageTimer -= diff;
    }

    if((phase == 3 || phase == 4)&& (!DemonForm))
    {
        if(AgonizingFlamesTimer < diff)
        {
            DoYell("AGONIZING FLAMES!", LANG_UNIVERSAL, NULL);
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_AGONIZING_FLAMES);
            AgonizingFlamesTimer = 25000;
        }else AgonizingFlamesTimer -= diff;

        if(phase == 3)
        {
            if(DemonFormTimer < diff)
            {
                DoYell(SAY_MORPH, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_MORPH);
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                ResetThreat();
                (*m_creature).GetMotionMaster()->Clear(false);
                (*m_creature).GetMotionMaster()->Idle();
                m_creature->HandleEmoteCommand(403);
                DoCast(m_creature, SPELL_DEMON_FORM);
                DemonForm = true;
                DemonFormTimer = 60000;
            }else DemonFormTimer -= diff;
        }
    }           

    if(DemonForm)
    {
        if(ShadowBlastTimer < diff)
        {
            DoYell("SHADOW BLAST!", LANG_UNIVERSAL, NULL);
            Unit* target;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            DoCast(target, SPELL_SHADOW_BLAST);
            ShadowBlastTimer = 10000;
        }else ShadowBlastTimer -= diff;

        if(FlameBurstTimer < diff)
        {
            DoYell("FLAME BURST!", LANG_UNIVERSAL, NULL);
            DoCast(m_creature->getVictim(), SPELL_FLAME_BURST);
            FlameBurstTimer = 35000;
        }else FlameBurstTimer -= diff;

        if(SummonTimer < diff)
        {
            DoYell("SUMMONING BEHOLDERS!", LANG_UNIVERSAL, NULL);
            for(uint32 i = 0; i<5; i++)
            {
                Creature* Demon = m_creature->SummonCreature(23375, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 5, TEMPSUMMON_CORPSE_DESPAWN, 2000);
                if(Demon)
                {
                    Unit* target;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                    if(target)
                    {
                        Demon->AI()->AttackStart(target);
                        Demon->AddThreat(target, 50000);
                        (*Demon).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*Demon->getVictim()));
                    }
                }
            }
            SummonTimer = 35000;
        }else SummonTimer -= diff;

        if(DemonFormTimer < diff)
        {
            DoYell("LEAVING DEMON FORM", LANG_UNIVERSAL, NULL);
            ResetThreat();
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            (*m_creature).GetMotionMaster()->Clear(false);
            (*m_creature).GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, NORMAL_FORM_DISPLAYID);
            m_creature->RemoveAura(SPELL_DEMON_FORM, 0);
            m_creature->RemoveAura(SPELL_DEMON_FORM, 1);
            m_creature->RemoveAura(SPELL_DEMON_FORM, 2);
            DemonForm = false;
            DemonFormTimer = 60000;
        }else DemonFormTimer -= diff;

    }
}

void boss_illidanAI::ResetThreat()
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

void boss_illidanAI::SpecialMove(float X, float Y, float Z, uint32 Time, Creature* creature)
{
    WorldPacket data( SMSG_MONSTER_MOVE, (41+creature->GetPackGUID().size()) );
    data.append(creature->GetPackGUID());

    data << creature->GetPositionX() << creature->GetPositionY() << creature->GetPositionZ();
    // unknown field - unrelated to orientation
    // seems to increment about 1000 for every 1.7 seconds
    // for now, we'll just use mstime
    data << getMSTime();

    data << uint8(0);                                // walkback when walking from A to B
    data << uint32(0x0200);          // flags
    /* Flags:
    512: Floating, moving without walking/running
    */
    data << Time;                                           // Time in between points
    data << uint32(1);                                      // 1 single waypoint
    data << X << Y << Z;                  // the single waypoint Point B
    creature->SendMessageToSet( &data, true );
}

struct MANGOS_DLL_DECL flame_of_azzinothAI : public ScriptedAI
{
    flame_of_azzinothAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FlameBlastTimer;
    uint32 BlazeTimer;

    void EnterEvadeMode()
    {
        FlameBlastTimer = 5000;
        BlazeTimer = 0;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
            DoStartRangedAttack(who);
    }

    void MoveInLineOfSight(Unit *who)
    {
        //We simply use this function to find players until we can use Map->GetPlayers()

        if (who && who->GetTypeId() == TYPEID_PLAYER && m_creature->IsHostileTo(who) && m_creature->IsWithinLOSInMap(who))
        {
            //Add them to our threat list
            m_creature->AddThreat(who,0.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Check if we have a target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(FlameBlastTimer < diff)
        {
            DoCast(m_creature, SPELL_FLAME_BLAST);
            FlameBlastTimer = 15000;
        }else FlameBlastTimer -= diff;

        if (BlazeTimer < diff)
        {
            DoCast(m_creature, SPELL_SUMMON_BLAZE);
            BlazeTimer = 10000;
        }else BlazeTimer -= diff;
    }
};

struct MANGOS_DLL_DECL flamecrashAI : public ScriptedAI
{
    flamecrashAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 FlameCrashTimer;

    void EnterEvadeMode()
    {
        FlameCrashTimer = 1000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who){ return; }

    void MoveInLineOfSight(Unit *who){ return; }

    void UpdateAI(const uint32 diff)
    {
        if(FlameCrashTimer < diff)
        {
            DoCast(m_creature, SPELL_FLAME_CRASH_EFFECT);
            FlameCrashTimer = 15000;
        }else FlameCrashTimer -= diff;
    }
};

struct MANGOS_DLL_DECL demonfireAI : public ScriptedAI
{
    demonfireAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 DemonFireTimer;

    void EnterEvadeMode()
    {
        DemonFireTimer = 0;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who){ return; }

    void MoveInLineOfSight(Unit *who){ return; }

    void UpdateAI(const uint32 diff)
    {
        if(DemonFireTimer < diff)
        {
            DoCast(m_creature, SPELL_DEMON_FIRE);
            DemonFireTimer = 30000;
        }else DemonFireTimer -= diff;
    }
};

struct MANGOS_DLL_DECL blazeAI : public ScriptedAI
{
    blazeAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 BlazeTimer;

    void EnterEvadeMode(){ BlazeTimer = 1000; }

    void AttackStart(Unit *who){ return; }

    void MoveInLineOfSight(Unit *who){ return; }

    void UpdateAI(const uint32 diff)
    {
        if(BlazeTimer < diff)
        {
            DoCast(m_creature, SPELL_BLAZE);
            BlazeTimer = 15000;
        }else BlazeTimer -= diff;
    }
};

struct MANGOS_DLL_DECL cage_trap_triggerAI : public ScriptedAI
{
    cage_trap_triggerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 CagedTimer;

    void EnterEvadeMode()
    {
        CagedTimer = 0;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who){ return; }

    void MoveInLineOfSight(Unit *who){ return; }

    void UpdateAI(const uint32 diff)
    {
        if(CagedTimer < diff)
        {
            DoCast(m_creature, SPELL_CAGE_TRAP);
            CagedTimer = 5000;
        }else CagedTimer -= diff;
    }
};

struct MANGOS_DLL_DECL akamaAI : public ScriptedAI
{
    akamaAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint64 IllidanGUID;

    uint32 WalkTime;
    uint32 TalkTimer;
    uint32 TalkCount;
    uint32 DespawnTimer;
    bool IsTalking;
    bool Despawn;

    void EnterEvadeMode()
    {
        if(pInstance)
        {
            pInstance->SetData("IllidanStormrageEvent", 0); // Not Started
        }

        IllidanGUID = NULL;

        WalkTime = 0;
        TalkTimer = 0;
        TalkCount = 0;
        IsTalking = false;
        DespawnTimer = 0;
        Despawn = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 20681);
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void BeginEvent(Player* target)
    {
        if(!pInstance)
            return;

        IllidanGUID = pInstance->GetUnitGUID("IllidanStormrage");
        Creature* Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID));

        if(Illidan && Illidan->isAlive())
        {
            DoYell("Checking and removing aura", LANG_UNIVERSAL, NULL);
            if(Illidan->HasAura(SPELL_ILLIDAN_KNEEL, 0))
                Illidan->RemoveAura(SPELL_ILLIDAN_KNEEL, 0);

            DoYell("Setting destination", LANG_UNIVERSAL, NULL);
            float destX = Illidan->GetPositionX() + 20;
            float destY = Illidan->GetPositionY();
            float destZ = Illidan->GetPositionZ();

            DoYell("Setting Walk Time", LANG_UNIVERSAL, NULL);
            WalkTime = 20000;
            DoYell("Sending Monster Move", LANG_UNIVERSAL, NULL);
            m_creature->SendMonsterMove(destX, destY, destZ, 0, false, WalkTime);
        }  
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(damage > m_creature->GetHealth())
        {
            if(done_by != m_creature)
            {
                damage = 0;
                m_creature->SetHealth(m_creature->GetMaxHealth());
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(WalkTime > 0)
        {
            if(WalkTime < diff)
            {
                if(IllidanGUID)
                {
                    Creature* Illidan = NULL;
                    Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID));
                    if(Illidan && Illidan->isAlive())
                    {     
                        DoYell("Illidan Alive, idling", LANG_UNIVERSAL, NULL);
                        (*m_creature).GetMotionMaster()->Clear(false);
                        (*m_creature).GetMotionMaster()->Idle();
                        DoYell("AttackStart", LANG_UNIVERSAL, NULL);
                        AttackStart(Illidan);
                        DoYell("Set In Front", LANG_UNIVERSAL, NULL);
                        Illidan->SetInFront(m_creature);
                        m_creature->SetInFront(Illidan);
                        DoYell("Set In Combat", LANG_UNIVERSAL, NULL);
                        //InCombat = true;
                        DoYell("Talk Count", LANG_UNIVERSAL, NULL);
                        TalkCount = 0;
                        IsTalking = true;
                        WalkTime = 0;
                    }
                }
            }else WalkTime -= diff;
        }

        if(IsTalking)
        {
            if(IllidanGUID)
            {
                if(TalkTimer < diff)
                {
                    DoYell("Reacquiring GUID", LANG_UNIVERSAL, NULL);
                    Creature* Illidan = NULL;
                    Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID));
                    switch(TalkCount)
                    {
                    case 0:
                        DoYell("Entering Illidan Block", LANG_UNIVERSAL, NULL);
                        if(Illidan && Illidan->isAlive())
                        {
                            DoYell("Intro 1", LANG_UNIVERSAL, NULL);
                            Illidan->Yell(SAY_INTRO_ILL1, LANG_UNIVERSAL, 0);
                            DoPlaySoundToSet(m_creature, SOUND_INTRO_ILL1);
                            DoYell("Talk Timer", LANG_UNIVERSAL, NULL);
                            TalkTimer = 13000;
                            DoYell("Talk Count", LANG_UNIVERSAL, NULL);
                            TalkCount++;
                        }
                        break;
                    case 1:
                        DoYell(SAY_INTRO_AKA1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_INTRO_AKA1);
                        TalkTimer = 12000;
                        TalkCount++;
                        break;
                    case 2:
                        if(Illidan && Illidan->isAlive())
                        {
                            Illidan->Yell(SAY_INTRO_ILL2, LANG_UNIVERSAL, 0);
                            DoPlaySoundToSet(m_creature, SOUND_INTRO_ILL2);
                            TalkTimer = 8000;
                            TalkCount++;
                        }
                        break;
                    case 3:
                        DoYell(SAY_INTRO_AKA2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_INTRO_AKA2);
                        TalkTimer = 5000;
                        TalkCount++;
                        break;
                    case 4:
                        if(Illidan && Illidan->isAlive())
                        {
                            Illidan->Yell(SAY_INTRO_ILL3, LANG_UNIVERSAL, 0);
                            DoPlaySoundToSet(m_creature, SOUND_INTRO_ILL3);
                            (*m_creature).GetMotionMaster()->Clear(false);
                            Illidan->AI()->AttackStart(m_creature);
                            AttackStart(Illidan);
                            Illidan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            IsTalking = false;
                        }
                        break;
                    case 5:
                        if(Illidan && Illidan->isAlive())
                        {
                            Illidan->Yell(SAY_AKAMA_MINION, LANG_UNIVERSAL, 0);
                            DoPlaySoundToSet(m_creature, SOUND_AKAMA_MINION);
                            TalkTimer = 6000;
                            TalkCount++;
                        }
                        break;
                    case 6:
                        DoYell("Fight on champions! Fight on and strike at the betrayer!", LANG_UNIVERSAL, NULL);
                        TalkTimer = 3000;
                        DespawnTimer = 5000;
                        Despawn = true;
                        IsTalking = false;
                        break;                                   
                    }
                }else TalkTimer -= diff;
            }
        }

        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(IllidanGUID)
        {
            Creature* Illidan = NULL;
            Illidan = ((Creature*)Unit::GetUnit((*m_creature), IllidanGUID));

            if(Illidan && Illidan->isAlive())
            {
                if((Illidan->GetHealth()*100 / Illidan->GetMaxHealth()) < 90 && (TalkCount < 5))
                {
                    IsTalking = true;
                    TalkCount = 5;
                }
            }
        }

        if(Despawn)
        {
            if(DespawnTimer < diff)
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 11686);
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, 0, false);
            }else DespawnTimer -= diff;
        }

        if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30)
        {
            DoCast(m_creature, SPELL_POTION);
            //m_creature->SetHealth(m_creature->GetMaxHealth());
        }
    }
};

CreatureAI* GetAI_boss_illidan(Creature *_Creature)
{
    return new boss_illidanAI (_Creature);
}

CreatureAI* GetAI_npc_akama_illidan(Creature *_Creature)
{
    return new akamaAI (_Creature);
}

CreatureAI* GetAI_flame_of_azzinoth(Creature *_Creature)
{
    return new flame_of_azzinothAI (_Creature);
}

CreatureAI* GetAI_flamecrash(Creature *_Creature)
{
    return new flamecrashAI (_Creature);
}

CreatureAI* GetAI_demonfire(Creature *_Creature)
{
    return new demonfireAI (_Creature);
}

CreatureAI* GetAI_blaze(Creature *_Creature)
{
    return new blazeAI (_Creature);
}

CreatureAI* GetAI_cage_trap_trigger(Creature *_Creature)
{
    return new cage_trap_triggerAI (_Creature);
}

bool GossipSelect_npc_akama_illidan(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Fight time
    {
        _Creature->Whisper(player->GetGUID(), "Get ready");
        player->PlayerTalkClass->CloseGossip();
        ((akamaAI*)_Creature->AI())->BeginEvent(player);
    }

    return true;
}

bool GossipHello_npc_akama_illidan(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->PlayerTalkClass->SendGossipMenu(907, _Creature->GetGUID());
    _Creature->Whisper(player->GetGUID(),"Try your best, champion!");

    return true;
}

void AddSC_boss_illidan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_illidan";
    newscript->GetAI = GetAI_boss_illidan;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="flame_of_azzinoth";
    newscript->GetAI = GetAI_flame_of_azzinoth;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="flame_crash";
    newscript->GetAI = GetAI_flamecrash;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="demon_fire";
    newscript->GetAI = GetAI_demonfire;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="blaze";
    newscript->GetAI = GetAI_blaze;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="effect_caged";
    newscript->GetAI = GetAI_cage_trap_trigger;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_akama_illidan";
    newscript->GetAI = GetAI_npc_akama_illidan;
    newscript->pGossipHello = &GossipHello_npc_akama_illidan;
    newscript->pGossipSelect = &GossipSelect_npc_akama_illidan;
    m_scripts[nrscripts++] = newscript;
} 

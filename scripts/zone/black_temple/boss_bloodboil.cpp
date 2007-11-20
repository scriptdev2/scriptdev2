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

/* ScriptData
SDName: Boss_Bloodboil
SD%Complete: 80
SDComment: Bloodboil not working correctly
EndScriptData */

#include "../../sc_defines.h"
#include "../../../../../shared/WorldPacket.h"
#include "../../../../../game/TargetedMovementGenerator.h"

//Spells
#define SPELL_ACID_GEYSER           40630
#define SPELL_ACIDIC_WOUND          40481
#define SPELL_ARCING_SMASH          40599
#define SPELL_BLOODBOIL             42005 // This spell is AoE whereas it shouldn't, using it as an Enrage timer then.
#define SPELL_FEL_ACID              40508
#define SPELL_FEL_RAGE_SELF         40594
#define SPELL_FEL_RAGE_TARGET       40604
#define SPELL_FEL_RAGE_2            40616
#define SPELL_FEL_RAGE_3            41625
#define SPELL_KNOCKBACK             38576
#define SPELL_INSIGNIFIGANCE        40618

//Speech'n'Sound
#define SAY_AGGRO            "Horde will crush you!"
#define SOUND_AGGRO            11432

#define SAY_SLAY1            "Time to feast!"
#define SOUND_SLAY1            11433

#define SAY_SLAY2            "More! I want more!"
#define SOUND_SLAY2            11434

#define SAY_SPECIAL1        "Drink your blood! Eat your flesh!"
#define SOUND_SPECIAL1        11435

#define SAY_SPECIAL2        "I hunger!"
#define SOUND_SPECIAL2        11436

#define SAY_ENRAGE            "I'll rip the meat from your bones!"
#define SOUND_ENRAGE        11437

#define SOUND_DEATH            11439

struct MANGOS_DLL_DECL boss_gurtogg_bloodboilAI : public ScriptedAI
{
    boss_gurtogg_bloodboilAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        else pInstance = NULL;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint32 AcidGeyserTimer;
    uint32 AcidicWoundTimer;
    uint32 ArcingSmashTimer;
    uint32 EnrageTimer;
    uint32 FelAcidTimer;
    uint32 KnockbackTimer;
    uint32 PhaseChangeTimer;

    bool Phase1;
    bool InCombat;

    void SetVariables()
    {
        if(pInstance)
            pInstance->SetData("GurtoggBloodboilEvent", 0);

        AcidGeyserTimer = 1000;
        AcidicWoundTimer = 6000;
        ArcingSmashTimer = 19000;
        EnrageTimer = 600000;
        FelAcidTimer = 25000;
        KnockbackTimer = 10000;
        PhaseChangeTimer = 60000;

        InCombat = false;
        Phase1 = true;
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        (*m_creature).GetMotionMaster()->Clear(false);
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
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
                if(pInstance) pInstance->SetData("GurtoggBloodboilEvent", 1);
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
                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                    if(pInstance) pInstance->SetData("GurtoggBloodboilEvent", 1);
                }
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
            case 0:
                DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
        }
    }

    void JustDied(Unit *victim)
    {
        if(pInstance)
            pInstance->SetData("GurtoggBloodboilEvent", 3);

        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }


    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget())
            return;

        if(m_creature->getVictim() && m_creature->isAlive())
        {
            if(ArcingSmashTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_ARCING_SMASH);
                ArcingSmashTimer = 19000;
            }else ArcingSmashTimer -= diff;

            if(FelAcidTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_FEL_ACID);
                FelAcidTimer = 25000;
            }else FelAcidTimer -= diff;

            if(EnrageTimer < diff)
            {
                //DoCast(m_creature, SPELL_BLOODBOIL); //Bad idea
                EnrageTimer = 600000;
                DoYell(SAY_ENRAGE,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
            }else EnrageTimer -= diff;

            if(Phase1)
            {
                if(KnockbackTimer < diff)
                {
                    Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                    if(target)
                    {
                        (m_creature->getThreatManager()).modifyThreatPercent(target, -40);
                        DoCast(target, SPELL_KNOCKBACK);
                    }
                    KnockbackTimer = 22000;
                }else KnockbackTimer -= diff;

                if(AcidicWoundTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_ACIDIC_WOUND);
                    AcidicWoundTimer = 10000;
                }else AcidicWoundTimer -= diff;
            }

            if(!Phase1)
            {
                if(AcidGeyserTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_ACID_GEYSER);
                    AcidGeyserTimer = 30000;
                }else AcidGeyserTimer -= diff;
            }

            if(PhaseChangeTimer < diff)
            {
                if(Phase1)
                {
                    Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0); 
                    if(target && target->isAlive())
                    {
                        m_creature->TauntApply(target);
                        DoCast(m_creature, SPELL_INSIGNIFIGANCE, true);
                        DoCast(target, SPELL_FEL_RAGE_TARGET, true);
                        DoCast(target, SPELL_FEL_RAGE_2, true);
                        DoCast(target, SPELL_FEL_RAGE_3, true);

                        //Cast this one last without triggered to make sure it appears in combat logs and spell visual
                        DoCast(m_creature, SPELL_FEL_RAGE_SELF);

                        switch(rand()%2)
                        {
                            case 0:
                                DoYell(SAY_SPECIAL1,LANG_UNIVERSAL,NULL);
                                DoPlaySoundToSet(m_creature, SOUND_SPECIAL1);
                                break;
                            case 1:
                                DoYell(SAY_SPECIAL2,LANG_UNIVERSAL,NULL);
                                DoPlaySoundToSet(m_creature, SOUND_SPECIAL2);
                                break;
                        }

                        AcidGeyserTimer = 1000;
                        Phase1 = false;
                        PhaseChangeTimer = 30000;
                    }
                }

                if(!Phase1)
                {
                    m_creature->TauntFadeOut(m_creature->getVictim());
                    Phase1 = true;
                    AcidicWoundTimer += 2000;
                    ArcingSmashTimer += 2000;
                    FelAcidTimer += 2000;
                    KnockbackTimer += 2000;
                    PhaseChangeTimer = 60000;
                }
            }else PhaseChangeTimer -= diff;
        DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_gurtogg_bloodboil(Creature *_Creature)
{
    return new boss_gurtogg_bloodboilAI (_Creature);
}

void AddSC_boss_gurtogg_bloodboil()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gurtogg_bloodboil";
    newscript->GetAI = GetAI_boss_gurtogg_bloodboil;
    m_scripts[nrscripts++] = newscript;
}

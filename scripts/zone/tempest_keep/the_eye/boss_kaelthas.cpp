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

#include "../../../sc_defines.h"
#include "../../../../../../game/Player.h"

#define SPELL_PYROBLAST                   38535
#define SPELL_FLAME_STRIKE                36735
#define SPELL_ARCANE_DISRUPTION           36834
#define SPELL_SHOCK_BARRIER               36815
#define SPELL_PHOENIX_ANIMATION           36723
// phase 5 spells
#define SPELL_KNOCKBACK                   11027
#define SPELL_GRAVITY_LAPSE               39432
#define SPELL_NETHER_BEAM                 35873
#define SPELL_NETHER_VAPOR                35859

#define SAY_INTRO                         "Energy. Power. My people are addicted to it. Their dependence made manifest after the Sunwell was destroyed. Welcome to the future...a pity you're too late to stop it. No one can stop me now. Selama ashal'anore."
#define SAY_ASTROMANCER_CAPERNIAN         "Capernian will see to it that your stay here is a short one."
#define SAY_ENGINEER_TELONICUS            "Well done. You have proven worthy to test your skills against my Master Engineer, Telonicus."
#define SAY_THALADRED_THE_DARKENER        "Let us see how your nerves hold up against the Darkener, Thaladred"
#define SAY_LORD_SANGUINAR                "You have persevered against some of my best advisors. But none can withstand the might of the Bloodhammer. Behold, Lord Sanguinar."
#define SAY_SUMMON_ADVISORS               "Perhaps I underestimated you. It would be unfair to make you fight all four Advisors at once, but...fair treatment was never shown to my people. I'm just returning the favor."
#define SAY_SLAY1                         "You will not prevail."
#define SAY_SLAY2                         "You gambled...and lost."
#define SAY_MINDCONTROL1                  "Obey me."
#define SAY_MINDCONTROL2                  "Bow to my will."
#define SAY_GRAVITYLAPSE1                 "Let us see how you fare when your world is turned upside down."
#define SAY_GRAVITYLAPSE2                 "Anara'nel belore!"
#define SAY_SUMMON_PHOENIX1               "Having trouble staying grounded?"
#define SAY_SUMMON_PHOENIX2               "By the power of the sun!"
#define SAY_ENRAGE                        "I have not come this far to be stopped! The future I have planned will not be jeopardized! Now, you will taste true power!"
#define SAY_DEATH                          "For...Quel...thalas!"

#define SOUND_INTRO                        11256
#define SOUND_ASTROMANCER_CAPERNIAN        11257
#define SOUND_ENGINEER_TELONICUS           11258
#define SOUND_THALADRED_THE_DARKENER       11259
#define SOUND_LORD_SANGUINAR               11260
#define SOUND_SUMMON_ADVISORS              11262
#define SOUND_SLAY1                        11270
#define SOUND_SLAY2                        11271
#define SOUND_MINDCONTROL1                 11268
#define SOUND_MINDCONTROL2                 11269
#define SOUND_GRAVITYLAPSE1                11264
#define SOUND_GRAVITYLAPSE2                11265
#define SOUND_SUMMON_PHOENIX1              11266
#define SOUND_SUMMON_PHOENIX2              11267
#define SOUND_ENRAGE                       11273
#define SOUND_DEATH                        11274

#define PHOENIX                            21362
#define NETHER_VAPOR                       21002
/*
// kaelthas AI
struct MANGOS_DLL_DECL boss_kaelthasAI : public ScriptedAI
{
    boss_kaelthasAI(Creature *c) : ScriptedAI(c)
    {
        Advisor[0] = 0;
        Advisor[1] = 0;
        Advisor[2] = 0;
        Advisor[3] = 0;
        EnterEvadeMode();
    }

    uint32 ArcaneDisruption_Timer;
    uint32 Phoenix_Timer;
    uint32 ShockBarrier_Timer;
    uint32 GravityLapse_Timer;
    uint32 GravityLapse_Phase;
    uint32 NetherBeam_Timer;
    uint32 NetherVapor_Timer;
    uint32 Phase;
    uint32 Phase1Subphase;
    uint32 Phase_Timer; // generic timer

    bool InCombat;
    bool InGravityLapse;

    uint64 Advisor[4];

    void EnterEvadeMode()
    {
        Phase = 0;
        GravityLapse_Timer = 60000+rand()%30000;
        GravityLapse_Phase = 0;
        NetherBeam_Timer = 15000;
        NetherVapor_Timer = 10000;
        ArcaneDisruption_Timer = 45000;
        Phoenix_Timer = 30000;
        ShockBarrier_Timer = 60000;
        InGravityLapse = false;

        //if he was fighting
        if(InCombat)
        {
            //respawn if died & apply flags
            Unit *pUnit;
            for(uint8 i = 0; i < 4; i++)
            {
                pUnit = Unit::GetUnit((*m_creature), Advisor[i]);
                if(pUnit)
                {
                    ((Creature*)pUnit)->Respawn();
                    ((Creature*)pUnit)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    ((Creature*)pUnit)->setFaction(35);
                    ((Creature*)pUnit)->AI()->EnterEvadeMode();
                }
            }

            //reset encounter
            m_creature->GetInstanceData()->SetData("KaelThasEvent", 0); // 0 = NOT_STARTED
        }

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void StartEvent()
    {
        Advisor[0] = m_creature->GetInstanceData()->GetUnit("ThaladredTheDarkener")->GetGUID();
        Advisor[1] = m_creature->GetInstanceData()->GetUnit("LordSanguinar")->GetGUID();
        Advisor[2] = m_creature->GetInstanceData()->GetUnit("GrandAstromancerCapernian")->GetGUID();
        Advisor[3] = m_creature->GetInstanceData()->GetUnit("MasterEngineerTelonicus")->GetGUID();

        if(!Advisor[0] || !Advisor[1] || !Advisor[2] || !Advisor[3])
            return;

        DoYell(SAY_INTRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_INTRO);

        m_creature->GetInstanceData()->SetData("KaelThasEvent", 1); // 1 = IN_PROGRESS

        Phase1Subphase = 0;
        Phase_Timer = 23000;
        Phase = 1;
    }

    void KilledUnit()
    {
        switch(rand()%2)
        {
            case 0:
            DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY1);
            break;

            case 1:
            DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY2);
            break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);

        m_creature->GetInstanceData()->SetData("KaelThasEvent", 3); // 3 = DONE
    }

    void AttackStart(Unit *who)
    {
        if(Phase < 4)
            return;

        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (Phase >= 4 && m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
            else if (!Phase && m_creature->IsWithinDistInMap(who, 60.0f) && !m_creature->GetInstanceData()->GetData("KaelThasEvent")) // 0 = NOT STARTED
                StartEvent();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(Phase == 1)
        {
            Unit *target;
            Unit* pUnit;

            switch(Phase1Subphase)
            {
                // phase 1 - start subphase 1
                case 0:
                if(Phase_Timer < diff)
                {
                    DoYell(SAY_THALADRED_THE_DARKENER, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THALADRED_THE_DARKENER);

                    pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase]);

                    if(pUnit)
                    {
                        pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pUnit->setFaction(1620);

                        target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                        if(target)
                            pUnit->Attack(target);
                    }

                    Phase_Timer = 5000;
                    Phase1Subphase++;
                }else Phase_Timer -= diff;
                break;

                // phase 1 - start subphase 2
                case 1:
                pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase-1]);
                if(pUnit && !pUnit->isAlive())
                {
                    // small offlike time through advisors
                    if(Phase_Timer < diff)
                    {
                        DoYell(SAY_LORD_SANGUINAR, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_LORD_SANGUINAR);

                        pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase]);

                        if(pUnit)
                        {
                            pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            pUnit->setFaction(1620);

                            target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                            if(target)
                                pUnit->Attack(target);
                        }

                        Phase_Timer = 5000;
                        Phase1Subphase++;
                    }else Phase_Timer = diff;
                }
                break;

                // phase 1 - start subphase 3
                case 2:
                pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase-1]);
                if(pUnit && !pUnit->isAlive())
                {
                    // small offlike time through advisors
                    if(Phase_Timer < diff)
                    {
                        DoYell(SAY_ASTROMANCER_CAPERNIAN, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_ASTROMANCER_CAPERNIAN);

                        pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase]);
                        if(pUnit)
                        {
                            pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            pUnit->setFaction(1620);

                            target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                            if(target)
                                pUnit->Attack(target);
                        }

                        Phase_Timer = 5000;
                        Phase1Subphase++;
                    }else Phase_Timer = diff;
                }
                break;

                // phase 1 - start subphase 4
                case 3:
                pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase-1]);
                if(pUnit && !pUnit->isAlive())
                {
                    // small offlike time through advisors
                    if(Phase_Timer < diff)
                    {
                        DoYell(SAY_ENGINEER_TELONICUS, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_ENGINEER_TELONICUS);

                        pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase]);
                        if(pUnit)
                        {
                            pUnit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            pUnit->setFaction(1620);

                            target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                            if(target)
                                pUnit->Attack(target);
                        }

                        Phase_Timer = 5000;
                        Phase1Subphase++;
                    }else Phase_Timer = diff;
                }
                break;

                // end of phase 1
                case 4:
                pUnit = Unit::GetUnit((*m_creature), Advisor[Phase1Subphase-1]);
                if(pUnit && !pUnit->isAlive())
                {
                    // This is provv: at the moment we can't implement phase 2 and 3
                    Phase = 4;

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                    if(target)
                        DoStartMeleeAttack(target);

                    InCombat = true;
                }
                break;
            }

        }
        else
        {
            //Return since we have no target
            if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
               return;

            if(Phase >= 4)
            {
                //ArcaneDisruption_Timer
                if(ArcaneDisruption_Timer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_DISRUPTION);
                    ArcaneDisruption_Timer = 60000;
                }else ArcaneDisruption_Timer -= diff;

                //ShockBarrier_Timer
                if(ShockBarrier_Timer < diff)
                {
                    DoCast(m_creature, SPELL_SHOCK_BARRIER);
                    DoCast(m_creature, SPELL_PYROBLAST);

                    ShockBarrier_Timer = 60000;
                }else ShockBarrier_Timer -= diff;

                //Phoenix_Timer
                if(Phoenix_Timer < diff)
                {
                    DoCast(m_creature, SPELL_PHOENIX_ANIMATION);
                    Creature *Phoenix = DoSpawnCreature(PHOENIX, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);

                    if(Phoenix)
                    {
                        Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if (target)
                            Phoenix->AI()->AttackStart(target);
                    }

                    switch(rand()%2)
                    {
                        case 0:
                        DoYell(SAY_SUMMON_PHOENIX1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_SUMMON_PHOENIX1);
                        break;

                        case 1:
                        DoYell(SAY_SUMMON_PHOENIX2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature, SOUND_SUMMON_PHOENIX2);
                        break;
                    }

                    Phoenix_Timer = 60000;
                }else Phoenix_Timer -= diff;

                //Start Phase 5
                if(Phase == 4 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
                {
                    Phase = 5;
                }
                
                //Phase 5
                if(Phase == 5)
                {
                    //GravityLapse_Timer
                    if(GravityLapse_Timer < diff)
                    {
                        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
                        std::list<HostilReference*>::iterator i = m_threatlist.begin();
                        switch(GravityLapse_Phase)
                        {
                            case 0:
                            // 1) Kael'thas will portal the whole raid right into his body
                            for (i = m_threatlist.begin(); i!= m_threatlist.end();++i)
                            {
                                Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                                if(pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                                {
                                    ((Player*)pUnit)->TeleportTo(550, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), pUnit->GetOrientation());
                                }
                            }
                            GravityLapse_Timer = 500;
                            ++GravityLapse_Phase;
                            InGravityLapse = true;
                            break;

                            case 1:
                            switch(rand()%2)
                            {
                                case 0:
                                DoYell(SAY_GRAVITYLAPSE1, LANG_UNIVERSAL, NULL);
                                DoPlaySoundToSet(m_creature, SOUND_GRAVITYLAPSE1);
                                break;

                                case 1:
                                DoYell(SAY_GRAVITYLAPSE2, LANG_UNIVERSAL, NULL);
                                DoPlaySoundToSet(m_creature, SOUND_GRAVITYLAPSE2);
                                break;
                            }

                            // 2) At that point he will put a Gravity Lapse debuff on everyone
                            for (i = m_threatlist.begin(); i!= m_threatlist.end();i++)
                            {
                                Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                                if(pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                                {
                                    m_creature->CastSpell(pUnit, SPELL_KNOCKBACK, true);
                                    //Gravity lapse
                                    pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE, true);
                                }
                            }

                            GravityLapse_Timer = 30000;
                            GravityLapse_Phase++;
                            break;

                            case 2:
                            InGravityLapse = false;
                            GravityLapse_Timer = 60000+rand()%30000;
                            GravityLapse_Phase = 0;
                            break;
                        }
                    }else GravityLapse_Timer -= diff;

                    if(InGravityLapse)
                    {
                        //NetherBeam_Timer
                        if(NetherBeam_Timer < diff)
                        {
                            DoCast(m_creature->getVictim(), SPELL_NETHER_BEAM);
                            NetherBeam_Timer = 20000+rand()%10000;
                        }else NetherBeam_Timer -= diff;

                        //NetherVapor_Timer
                        if(NetherVapor_Timer < diff)
                        {
                            Creature *NetherVapor = DoSpawnCreature(NETHER_VAPOR, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);

                            NetherVapor_Timer = 10000;
                        }else NetherVapor_Timer -= diff;
                    }
                }

                DoMeleeAttackIfReady();
            }
        }
    }
};

// nether vapor AI
struct MANGOS_DLL_DECL mob_nether_vaporAI : public ScriptedAI
{
    mob_nether_vaporAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    void EnterEvadeMode()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->setFaction(35);

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        //if a player is within 8 yard, cast Nether Vapor
        if (m_creature->IsWithinDistInMap(who, 8))
        {
            DoCast(who, SPELL_NETHER_VAPOR);
        }
    }
}; 

CreatureAI* GetAI_boss_kaelthas(Creature *_Creature)
{
    return new boss_kaelthasAI (_Creature);
}
CreatureAI* GetAI_mob_nether_vapor(Creature *_Creature)
{
    return new mob_nether_vaporAI (_Creature);
}

void AddSC_boss_kaelthas()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_kaelthas";
    newscript->GetAI = GetAI_boss_kaelthas;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_nether_vapor";
    newscript->GetAI = GetAI_mob_nether_vapor;
    m_scripts[nrscripts++] = newscript;
}
*/
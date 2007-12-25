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
SDName: boss_Kaelthas
SD%Complete: 60
SDComment: SQL, phase 2, phase 3, Mind Control, taunt immunity
EndScriptData */

#include "../../../sc_defines.h"
#include "../../../../../../game/Player.h"
#include "../../../../../../game/TargetedMovementGenerator.h"

//Phase 2 spells
#define SPELL_SUMMON_WEAPONS              36976
#define SPELL_SUMMON_WEAPONA              36958
#define SPELL_SUMMON_WEAPONB              36959
#define SPELL_SUMMON_WEAPONC              36960
#define SPELL_SUMMON_WEAPOND              36961
#define SPELL_SUMMON_WEAPONE              36962
#define SPELL_SUMMON_WEAPONF              36963
#define SPELL_SUMMON_WEAPONG              36964
//Phase 4 spells
#define SPELL_FIREBALL                    22088 //wrong but works with CastCustomSpell
#define SPELL_PYROBLAST                   38535
#define SPELL_FLAME_STRIKE                36735
#define SPELL_ARCANE_DISRUPTION           36834
#define SPELL_SHOCK_BARRIER               36815
#define SPELL_PHOENIX_ANIMATION           36723
//Phase 5 spells
#define SPELL_KNOCKBACK                   11027
#define SPELL_GRAVITY_LAPSE               34480
#define SPELL_GRAVITY_LAPSE_AURA          39432
#define SPELL_NETHER_BEAM                 35873
//Thaladred the Darkener spells
#define SPELL_PSYCHIC_BLOW                10689
#define SPELL_SILENCE                     30225
//Lord Sanguinar spells
#define SPELL_BELLOWING_ROAR              40636
//Grand Astromancer Capernian spells
#define CAPERNIAN_DISTANCE                20 //she casts away from the target
#define SPELL_CAPERNIAN_FIREBALL          36971
#define SPELL_CONFLAGRATION               37018
#define SPELL_ARCANE_EXPLOSION            38197
//Master Engineer Telonicus spells
#define SPELL_BOMB                        37036
#define SPELL_REMOTE_TOY                  37027
//Nether Vapor spell
#define SPELL_NETHER_VAPOR                35859
//Phoenix spell
#define SPELL_BURN                        36721

//kael'thas Speech
#define SAY_INTRO                         "Energy. Power. My people are addicted to it... a dependence made manifest after the Sunwell was destroyed. Welcome... to the future. A pity you are too late to stop it. No one can stop me now! Selama ashal'anore!"
#define SAY_ASTROMANCER_CAPERNIAN         "Capernian will see to it that your stay here is a short one."
#define SAY_ENGINEER_TELONICUS            "Well done, you have proven worthy to test your skills against my master engineer, Telonicus."
#define SAY_THALADRED_THE_DARKENER        "Let us see how your nerves hold up against the Darkener, Thaladred"
#define SAY_LORD_SANGUINAR                "You have persevered against some of my best advisors... but none can withstand the might of the Blood Hammer. Behold, Lord Sanguinar!"
#define SAY_PHASE2                        "As you see, I have many weapons in my arsenal...."
#define SAY_PHASE3                        "Perhaps I underestimated you. It would be unfair to make you fight all four advisors at once, but... fair treatment was never shown to my people. I'm just returning the favor."
#define SAY_PHASE4                        "Alas, sometimes one must take matters into one's own hands. Balamore shanal!"
#define SAY_PHASE5                        "I have not come this far to be stopped! The future I have planned will not be jeopardized! Now you will taste true power!!"
#define SAY_SLAY1                         "You will not prevail."
#define SAY_SLAY2                         "You gambled...and lost."
#define SAY_MINDCONTROL1                  "Obey me."
#define SAY_MINDCONTROL2                  "Bow to my will."
#define SAY_GRAVITYLAPSE1                 "Let us see how you fare when your world is turned upside down."
#define SAY_GRAVITYLAPSE2                 "Having trouble staying grounded?"
#define SAY_SUMMON_PHOENIX1               "Anara'nel belore!"
#define SAY_SUMMON_PHOENIX2               "By the power of the sun!"
#define SAY_DEATH                         "For...Quel...thalas!"

#define SOUND_INTRO                       11256
#define SOUND_ASTROMANCER_CAPERNIAN       11257
#define SOUND_ENGINEER_TELONICUS          11258
#define SOUND_THALADRED_THE_DARKENER      11259
#define SOUND_LORD_SANGUINAR              11260
#define SOUND_PHASE2                      11261
#define SOUND_PHASE3                      11262
#define SOUND_PHASE4                      11263
#define SOUND_PHASE5                      11273
#define SOUND_SLAY1                       11270
#define SOUND_SLAY2                       11271
#define SOUND_MINDCONTROL1                11268
#define SOUND_MINDCONTROL2                11269
#define SOUND_GRAVITYLAPSE1               11264
#define SOUND_GRAVITYLAPSE2               11265
#define SOUND_SUMMON_PHOENIX1             11266
#define SOUND_SUMMON_PHOENIX2             11267
#define SOUND_DEATH                       11274

//Thaladred the Darkener speech
#define SAY_THALADRED_AGGRO               "Prepare yourselves!"
#define SAY_THALADRED_DEATH               "Forgive me, my prince! I have... failed."
#define SOUND_THALADRED_AGGRO             11203
#define SOUND_THALADRED_DEATH             11204
#define EMOTE_THALADRED_GAZE              "sets eyes on %s!"

//Lord Sanguinar speech
#define SAY_SANGUINAR_AGGRO               "Blood for blood!"
#define SAY_SANGUINAR_DEATH               "NO! I ...will... not..."
#define SOUND_SANGUINAR_AGGRO             11152
#define SOUND_SANGUINAR_DEATH             11153

//Grand Astromancer Capernian speech
#define SAY_CAPERNIAN_AGGRO               "The sin'dore reign supreme!"
#define SAY_CAPERNIAN_DEATH               "This is not over!"
#define SOUND_CAPERNIAN_AGGRO             11117
#define SOUND_CAPERNIAN_DEATH             11118

//Master Engineer Telonicus speech
#define SAY_TELONICUS_AGGRO               "Anar'alah belore!"
#define SAY_TELONICUS_DEATH               "More <missing>... await"
#define SOUND_TELONICUS_AGGRO             11157
#define SOUND_TELONICUS_DEATH             11158

#define PHOENIX                           21362
#define NETHER_VAPOR                      21002
#define PHOENIX_EGG                       21364

#define PI                                3.141592

//weapon id + position
float KaelthasWeapons[7][5] =
{
    {21270, 794.38, 15, 48.72, 2.9}, //[Cosmic Infuser]
    {21269, 785.47, 12.12, 48.72, 3.14}, //[Devastation]
    {21271, 781.25, 4.39, 48.72, 3.14}, //[Infinity Blade]
    {21273, 777.38, -0.81, 48.72, 3.06}, //[Phaseshift Bulwark]
    {21274, 781.48, -6.08, 48.72, 3.9}, //[Staff of Disintegration]
    {21272, 785.42, -13.59, 48.72, 3.4}, //[Warp Slicer]
    {21268, 793.06, -16.61, 48.72, 3.10} //[Netherstrand Longbow]
};

//Kael'thas AI
struct MANGOS_DLL_DECL boss_kaelthasAI : public ScriptedAI
{
    boss_kaelthasAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        AdvisorGuid[0] = 0;
        AdvisorGuid[1] = 0;
        AdvisorGuid[2] = 0;
        AdvisorGuid[3] = 0;
        InCombat = false;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 Fireball_Timer;
    uint32 ArcaneDisruption_Timer;
    uint32 Phoenix_Timer;
    uint32 ShockBarrier_Timer;
    uint32 GravityLapse_Timer;
    uint32 GravityLapse_Phase;
    uint32 NetherBeam_Timer;
    uint32 NetherVapor_Timer;
    uint32 Phase;
    uint32 PhaseSubphase; //generic
    uint32 Phase_Timer; //generic timer

    bool InCombat;
    bool InGravityLapse;
    bool IsCastingFireball;

    uint64 AdvisorGuid[4];

    void EnterEvadeMode()
    {
        Fireball_Timer = 5000+rand()%10000;
        ArcaneDisruption_Timer = 45000;
        Phoenix_Timer = 50000;
        ShockBarrier_Timer = 60000;
        GravityLapse_Timer = 60000+rand()%30000;
        GravityLapse_Phase = 0;
        NetherBeam_Timer = 15000;
        NetherVapor_Timer = 10000;
        Phase = 0;
        InGravityLapse = false;
        IsCastingFireball = false;

        if(InCombat)
            PrepareAdvisors();

        InCombat = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData("KaelThasEvent", 0);

        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
    }

    void PrepareAdvisors()
    {
        Creature *pCreature;
        for(uint8 i = 0; i < 4; i++)
        {
            pCreature = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[i]));
            if(pCreature)
            {
                pCreature->Respawn();
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pCreature->setFaction(35);
                pCreature->AI()->EnterEvadeMode();
            }
        }
    }

    void StartEvent()
    {
        if(!pInstance)
            return;

        AdvisorGuid[0] = pInstance->GetData64("ThaladredTheDarkener");
        AdvisorGuid[1] = pInstance->GetData64("LordSanguinar");
        AdvisorGuid[2] = pInstance->GetData64("GrandAstromancerCapernian");
        AdvisorGuid[3] = pInstance->GetData64("MasterEngineerTelonicus");

        if(!AdvisorGuid[0] || !AdvisorGuid[1] || !AdvisorGuid[2] || !AdvisorGuid[3])
            return;

        PrepareAdvisors();

        DoYell(SAY_INTRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_INTRO);

        pInstance->SetData("KaelThasEvent", 1);

        PhaseSubphase = 0;
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

        if(pInstance)
            pInstance->SetData("KaelThasEvent", 0);
    }

    void AttackStart(Unit *who)
    {
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if(Phase >= 4)
            {
                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
            else
            {
                if (pInstance && !pInstance->GetData("KaelThasEvent") && !Phase)
                    StartEvent();

                //add to the threat list, so we can use SelectTarget
                m_creature->AddThreat(who,0.0f);
            }
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
            else if(who->isAlive())
            {
                if (pInstance && !pInstance->GetData("KaelThasEvent") && !Phase && m_creature->IsWithinDistInMap(who, 60.0f))
                    StartEvent();

                //add to the threat list, so we can use SelectTarget
                m_creature->AddThreat(who,0.0f);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Phase 1
        if(Phase == 1)
        {
            Unit *target;
            Creature* Advisor;

            //Subphase switch
            switch(PhaseSubphase)
            {
                //Subphase 1 - Start
                case 0:
                if(Phase_Timer < diff)
                {
                    DoYell(SAY_THALADRED_THE_DARKENER, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THALADRED_THE_DARKENER);

                    //start advisor within 9 seconds
                    Phase_Timer = 9000;

                    PhaseSubphase++;
                }else Phase_Timer -= diff;
                break;

                //Subphase 1 - Unlock advisor
                case 1:
                if(Phase_Timer < diff)
                {
                    Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[0]));

                    if(Advisor)
                    {
                        Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Advisor->setFaction(14);

                        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if(target)
                            Advisor->AI()->AttackStart(target);
                    }

                    PhaseSubphase++;
                }else Phase_Timer -= diff;
                break;

                //Subphase 2 - Start
                case 2:
                Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[0]));
                if(Advisor && !Advisor->isAlive())
                {
                    DoYell(SAY_LORD_SANGUINAR, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_LORD_SANGUINAR);

                    //start advisor within 12.5 seconds
                    Phase_Timer = 12500;

                    PhaseSubphase++;
                }
                break;

                //Subphase 2 - Unlock advisor
                case 3:
                if(Phase_Timer < diff)
                {
                    Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[1]));

                    if(Advisor)
                    {
                        Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Advisor->setFaction(14);

                        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if(target)
                            Advisor->AI()->AttackStart(target);
                    }

                    PhaseSubphase++;
                }else Phase_Timer -= diff;
                break;

                //Subphase 3 - Start
                case 4:
                Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[1]));
                if(Advisor && !Advisor->isAlive())
                {
                    DoYell(SAY_ASTROMANCER_CAPERNIAN, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_ASTROMANCER_CAPERNIAN);

                    //start advisor within 7 seconds
                    Phase_Timer = 7000;

                    PhaseSubphase++;
                }
                break;

                //Subphase 3 - Unlock advisor
                case 5:
                if(Phase_Timer < diff)
                {
                    Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[2]));

                    if(Advisor)
                    {
                        Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Advisor->setFaction(14);

                        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if(target)
                            Advisor->AI()->AttackStart(target);
                    }

                    PhaseSubphase++;
                }else Phase_Timer -= diff;
                break;

                //Subphase 4 - Start
                case 6:
                Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[2]));
                if(Advisor && !Advisor->isAlive())
                {
                    DoYell(SAY_ENGINEER_TELONICUS, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_ENGINEER_TELONICUS);

                    //start advisor within 8.4 seconds
                    Phase_Timer = 8400;

                    PhaseSubphase++;
                }
                break;

                //Subphase 4 - Unlock advisor
                case 7:
                if(Phase_Timer < diff)
                {
                    Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[3]));

                    if(Advisor)
                    {
                        Advisor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Advisor->setFaction(14);

                        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if(target)
                            Advisor->AI()->AttackStart(target);
                    }

                    Phase_Timer = 3000;

                    PhaseSubphase++;
                }else Phase_Timer -= diff;
                break;

                //End of phase 1
                case 8:
                Advisor = (Creature*)(Unit::GetUnit((*m_creature), AdvisorGuid[3]));
                if(Advisor && !Advisor->isAlive())
                {
                    //This is provv: at the moment we can't implement phase 2 and 3
                    Phase = 4;
                    pInstance->SetData("KaelThasEvent", 4);

                    DoYell(SAY_PHASE4, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_PHASE4);

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    Unit *target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    if(target)
                    {
                        InCombat = true;
                        DoStartMeleeAttack(target);
                    }

                    /*if(Phase_Timer < diff)
                    {
                        //spell summon weapon
                        DoCast(m_creature, SPELL_SUMMON_WEAPONS);
                        Phase_Timer = 3000;
                        
                        Phase = 2;
                        PhaseSubphase = 0;
                    }else Phase_Timer -= diff;*/
                }
                break;
            }
        }
        /*else if(Phase == 2)
        {
            //Summon weapons
            if(!PhaseSubphase)
            {
                if(Phase_Timer < diff)
                {
                    DoYell(SAY_PHASE2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_PHASE2);

                    //TODO Summon weapons

                    PhaseSubphase++;
                }else Phase_Timer -= diff;
            }
        }*/
        else
        {
            //Return since we have no target
            if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
                return;

            //Fireball_Timer
            if(Fireball_Timer < diff)
            {
                if(!IsCastingFireball)
                {
                    if(!m_creature->IsNonMeleeSpellCasted(false))
                    {
                        //interruptable
                        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                        int32 dmg = 20000+rand()%5000;
                        m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_FIREBALL, &dmg, 0, 0, false);
                        IsCastingFireball = true;
                        Fireball_Timer = 2500;
                    }
                }
                else
                {
                    //apply resistance
                    m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                    IsCastingFireball = false;
                    Fireball_Timer = 5000+rand()%10000;
                }
            }else Fireball_Timer -= diff;

            //ArcaneDisruption_Timer
            if(ArcaneDisruption_Timer < diff)
            {
                if(!InGravityLapse)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_DISRUPTION);

                ArcaneDisruption_Timer = 60000;
            }else ArcaneDisruption_Timer -= diff;

            //ShockBarrier_Timer
            if(ShockBarrier_Timer < diff)
            {
                DoCast(m_creature, SPELL_SHOCK_BARRIER);

                if(Phase < 5)
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

                switch(rand()%3)
                {
                    case 0:
                    DoYell(SAY_SUMMON_PHOENIX1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SUMMON_PHOENIX1);
                    break;

                    case 1:
                    DoYell(SAY_SUMMON_PHOENIX2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SUMMON_PHOENIX2);
                    break;

                    default:
                    break;
                }

                Phoenix_Timer = 60000;
            }else Phoenix_Timer -= diff;

            //Start Phase 5
            if(Phase == 4 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
            {
                pInstance->SetData("KaelThasEvent", 4);
                Phase = 5;

                DoYell(SAY_PHASE5, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_PHASE5);
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
                                //m_creature->CastSpell(pUnit, SPELL_KNOCKBACK, true);
                                //Gravity lapse - needs an exception in Spell system to work

                                pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE, true);
                                pUnit->CastSpell(pUnit, SPELL_GRAVITY_LAPSE_AURA, true);
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
};

//Thaladred the Darkener AI
struct MANGOS_DLL_DECL boss_thaladred_the_darkenerAI : public ScriptedAI
{
    boss_thaladred_the_darkenerAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 Gaze_Timer;
    uint32 Silence_Timer;
    uint32 PsychicBlow_Timer;

    bool InCombat;

    void EnterEvadeMode()
    {
        Gaze_Timer = 8000+rand()%4000;
        Silence_Timer = 20000;
        PsychicBlow_Timer = 10000;

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true); 
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        //reset encounter
        if(pInstance && pInstance->GetData("KaelThasEvent") == 1)
        {
            Creature *Kaelthas = NULL;
            Kaelthas = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64("Kaelthas")));

            if(Kaelthas)
                Kaelthas->AI()->EnterEvadeMode();
        }
    }

    void ResetThreat()
    {
        Unit* pUnit = NULL;
        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for(i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
        {
            pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit)
                m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
        }
    }

    void JustDied(Unit *killer)
    {
        //yell only in phase 3 or 4
        if(pInstance && pInstance->GetData("KaelThasEvent") > 1)
        {
            DoPlaySoundToSet(m_creature, SOUND_THALADRED_DEATH);
            DoYell(SAY_THALADRED_DEATH, LANG_UNIVERSAL, NULL);
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                InCombat = true;
                DoYell(SAY_THALADRED_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_THALADRED_AGGRO);
                m_creature->AddThreat(who, 5000000.0f);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
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

                //Begin attack
                DoStartMeleeAttack(who);

                if(!InCombat)
                {
                    InCombat = true;
                    DoYell(SAY_THALADRED_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_THALADRED_AGGRO);
                    m_creature->AddThreat(who, 5000000.0f);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Gaze_Timer
        if(Gaze_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                ResetThreat();
                m_creature->AddThreat(target, 5000000.0f);
                //DoStartMeleeAttack(target);
                char str[1024];
                sprintf(str, EMOTE_THALADRED_GAZE, target->GetName());
                DoTextEmote(str, NULL);
                Gaze_Timer = 8500;
            }
        }else Gaze_Timer -= diff;

        //Silence_Timer
        if(Silence_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SILENCE);
            Silence_Timer = 20000;
        }else Silence_Timer -= diff;

        //PsychicBlow_Timer
        if(PsychicBlow_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_PSYCHIC_BLOW);
            PsychicBlow_Timer = 20000+rand()%5000;
        }else PsychicBlow_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Lord Sanguinar AI
struct MANGOS_DLL_DECL boss_lord_sanguinarAI : public ScriptedAI
{
    boss_lord_sanguinarAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 Fear_Timer;

    bool InCombat;

    void EnterEvadeMode()
    {
        Fear_Timer = 20000;

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true); 
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        //reset encounter
        if(pInstance && pInstance->GetData("KaelThasEvent") == 1)
        {
            Creature *Kaelthas = NULL;
            Kaelthas = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64("Kaelthas")));

            if(Kaelthas)
                Kaelthas->AI()->EnterEvadeMode();
        }
    }

    void JustDied(Unit *killer)
    {
        //yell only in phase 3 or 4
        if(pInstance && pInstance->GetData("KaelThasEvent") > 1)
        {
            DoPlaySoundToSet(m_creature, SOUND_SANGUINAR_DEATH);
            DoYell(SAY_SANGUINAR_DEATH, LANG_UNIVERSAL, NULL);
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                InCombat = true;
                DoYell(SAY_SANGUINAR_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_SANGUINAR_AGGRO);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
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

                //Begin attack
                DoStartMeleeAttack(who);

                if(!InCombat)
                {
                    InCombat = true;
                    DoYell(SAY_SANGUINAR_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SANGUINAR_AGGRO);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Fear_Timer
        if(Fear_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_BELLOWING_ROAR);

            Fear_Timer = 25000+rand()%10000; //approximately every 30 seconds
        }else Fear_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Grand Astromancer Capernian AI
struct MANGOS_DLL_DECL boss_grand_astromancer_capernianAI : public ScriptedAI
{
    boss_grand_astromancer_capernianAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 Fireball_Timer;
    uint32 Conflagration_Timer;
    uint32 ArcaneExplosion_Timer;
    uint32 Yell_Timer;

    bool InCombat;
    bool Yell;

    void EnterEvadeMode()
    {
        Fireball_Timer = 2000;
        Conflagration_Timer = 20000;
        ArcaneExplosion_Timer = 5000;
        Yell_Timer = 2000;

        InCombat = false;
        Yell = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_FIRE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true); 
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        //reset encounter
        if(pInstance && pInstance->GetData("KaelThasEvent") == 1)
        {
            Creature *Kaelthas = NULL;
            Kaelthas = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64("Kaelthas")));

            if(Kaelthas)
                Kaelthas->AI()->EnterEvadeMode();
        }
    }

    void JustDied(Unit *killer)
    {
        //yell only in phase 3 or 4
        if(pInstance && pInstance->GetData("KaelThasEvent") > 1)
        {
            DoPlaySoundToSet(m_creature, SOUND_CAPERNIAN_DEATH);
            DoYell(SAY_CAPERNIAN_DEATH, LANG_UNIVERSAL, NULL);
        }
    }

    void DoStartMeleeAttack(Unit *victim)
    {
        if (!victim)
            return;

        if ( m_creature->Attack(victim) )
        {
            m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*victim, CAPERNIAN_DISTANCE, PI/2));

            m_creature->AddThreat(victim, 0.0f);
            m_creature->resetAttackTimer();

            if (victim->GetTypeId() == TYPEID_PLAYER)
                m_creature->SetLootRecipient((Player*)victim);
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);

            if(!InCombat)
                InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
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

                //Begin attack
                DoStartMeleeAttack(who);

                if(!InCombat)
                    InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Yell_Timer
        if(!Yell)
            if(Yell_Timer < diff)
            {
                DoYell(SAY_CAPERNIAN_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_CAPERNIAN_AGGRO);

                Yell = true;
            }else Yell_Timer -= diff;

        //Fireball_Timer
        if(Fireball_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CAPERNIAN_FIREBALL);
            Fireball_Timer = 4000;
        }else Fireball_Timer -= diff;

        //Conflagration_Timer
        if(Conflagration_Timer < diff)
        {
            Unit *target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target && m_creature->IsWithinDistInMap(target, 30))
                DoCast(target, SPELL_CONFLAGRATION);
            else
                DoCast(m_creature->getVictim(), SPELL_CONFLAGRATION);

            Conflagration_Timer = 10000+rand()%5000;
        }else Conflagration_Timer -= diff;

        //ArcaneExplosion_Timer
        if(ArcaneExplosion_Timer < diff)
        {
            bool InMeleeRange = false;
            Unit *target = NULL;
            std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
            for (std::list<HostilReference*>::iterator i = m_threatlist.begin(); i!= m_threatlist.end();++i)
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                if(pUnit && pUnit->IsWithinDistInMap(m_creature, 5)) //if in melee range
                {
                    InMeleeRange = true;
                    target = pUnit;
                    break;
                }
            }

            if(InMeleeRange)
                DoCast(target, SPELL_ARCANE_EXPLOSION);

            ArcaneExplosion_Timer = 4000+rand()%2000;
        }else ArcaneExplosion_Timer -= diff;

        //Do NOT deal any melee damage.
    }
};

//Master Engineer Telonicus AI
struct MANGOS_DLL_DECL boss_master_engineer_telonicusAI : public ScriptedAI
{
    boss_master_engineer_telonicusAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance* pInstance;

    uint32 Bomb_Timer;
    uint32 RemoteToy_Timer;

    bool InCombat;

    void EnterEvadeMode()
    {
        Bomb_Timer = 10000;
        RemoteToy_Timer = 5000;

        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true); 
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

        //reset encounter
        if(pInstance && pInstance->GetData("KaelThasEvent") == 1)
        {
            Creature *Kaelthas = NULL;
            Kaelthas = (Creature*)(Unit::GetUnit((*m_creature), pInstance->GetData64("Kaelthas")));

            if(Kaelthas)
                Kaelthas->AI()->EnterEvadeMode();
        }
    }

    void JustDied(Unit *killer)
    {
        //yell only in phase 3 or 4
        if(pInstance && pInstance->GetData("KaelThasEvent") > 1)
        {
            DoPlaySoundToSet(m_creature, SOUND_TELONICUS_DEATH);
            DoYell(SAY_SANGUINAR_DEATH, LANG_UNIVERSAL, NULL);
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                InCombat = true;
                DoYell(SAY_TELONICUS_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_TELONICUS_AGGRO);
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
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

                //Begin attack
                DoStartMeleeAttack(who);

                if(!InCombat)
                {
                    InCombat = true;
                    DoYell(SAY_TELONICUS_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_TELONICUS_AGGRO);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Bomb_Timer
        if(Bomb_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_BOMB);
            Bomb_Timer = 25000;
        }else Bomb_Timer -= diff;

        //RemoteToy_Timer
        if(RemoteToy_Timer < diff)
        {
            Unit *target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SPELL_REMOTE_TOY);

            RemoteToy_Timer = 10000+rand()%5000;
        }else RemoteToy_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Nether Vapor AI
struct MANGOS_DLL_DECL mob_nether_vaporAI : public ScriptedAI
{
    mob_nether_vaporAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 NetherVapor_Timer;

    void EnterEvadeMode()
    {
        NetherVapor_Timer = 1500;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE); 
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->setFaction(14);

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who) { return; }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who)
            return;

        m_creature->AddThreat(who, 0.0f);
    }

    void UpdateAI(const uint32 diff)
    {
        //NetherVapor_Timer
        if(NetherVapor_Timer < diff)
        {
            std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
            for (std::list<HostilReference*>::iterator i = m_threatlist.begin(); i!= m_threatlist.end();++i)
            {
                Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
                if(pUnit && pUnit->IsWithinDistInMap(m_creature, 8) && !pUnit->HasAura(SPELL_NETHER_VAPOR, 0))
                {
                    DoCast(pUnit, SPELL_NETHER_VAPOR);
                }
            }
            NetherVapor_Timer = 1500;
        }else NetherVapor_Timer -= diff;
    }
};

//Phoenix AI
struct MANGOS_DLL_DECL mob_phoenixAI : public ScriptedAI
{
    mob_phoenixAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 Burn_Timer;

    void EnterEvadeMode()
    {
        Burn_Timer = 1000;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void JustDied(Unit *killer)
    {
        DoSpawnCreature(PHOENIX_EGG, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Check if we have a current target
        /*if (Burn_Timer < diff)
        {        
            if (m_creature->GetHealth() <= m_creature->GetMaxHealth() * 0.05)
            {
                m_creature->setDeathState(JUST_DIED);
                DoSpawnCreature(PHOENIX_EGG, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
            }
            else
                m_creature->SetHealth(m_creature->GetHealth() - m_creature->GetMaxHealth() * 0.05);

            DoCast(m_creature->getVictim(), SPELL_BURN);
            Burn_Timer = 1000;
        }else Burn_Timer -= diff;*/

        DoMeleeAttackIfReady();
    }
};

//Phoenix Egg
struct MANGOS_DLL_DECL mob_phoenix_eggAI : public ScriptedAI
{
    mob_phoenix_eggAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 RebirthTimer;
    bool Rebirth;

    void EnterEvadeMode()
    {
        RebirthTimer = 15000;
        Rebirth = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who) { return; }

    void MoveInLineOfSight(Unit *who) { return; }

    void UpdateAI(const uint32 diff)
    {
        if (!Rebirth && RebirthTimer < diff)
        {        
            DoSpawnCreature(PHOENIX, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
            m_creature->setDeathState(JUST_DIED);
            Rebirth = true;
        }else RebirthTimer -= diff;
    }
};

CreatureAI* GetAI_boss_kaelthas(Creature *_Creature)
{
    return new boss_kaelthasAI (_Creature);
}
CreatureAI* GetAI_boss_thaladred_the_darkener(Creature *_Creature)
{
    return new boss_thaladred_the_darkenerAI (_Creature);
}
CreatureAI* GetAI_boss_lord_sanguinar(Creature *_Creature)
{
    return new boss_lord_sanguinarAI (_Creature);
}
CreatureAI* GetAI_boss_grand_astromancer_capernian(Creature *_Creature)
{
    return new boss_grand_astromancer_capernianAI (_Creature);
}
CreatureAI* GetAI_boss_master_engineer_telonicus(Creature *_Creature)
{
    return new boss_master_engineer_telonicusAI (_Creature);
}
CreatureAI* GetAI_mob_nether_vapor(Creature *_Creature)
{
    return new mob_nether_vaporAI (_Creature);
}
CreatureAI* GetAI_mob_phoenix(Creature *_Creature)
{
    return new mob_phoenixAI (_Creature);
}
CreatureAI* GetAI_mob_phoenix_egg(Creature *_Creature)
{
    return new mob_phoenix_eggAI (_Creature);
}

void AddSC_boss_kaelthas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kaelthas";
    newscript->GetAI = GetAI_boss_kaelthas;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_thaladred_the_darkener";
    newscript->GetAI = GetAI_boss_thaladred_the_darkener;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_lord_sanguinar";
    newscript->GetAI = GetAI_boss_lord_sanguinar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_grand_astromancer_capernian";
    newscript->GetAI = GetAI_boss_grand_astromancer_capernian;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_master_engineer_telonicus";
    newscript->GetAI = GetAI_boss_master_engineer_telonicus;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_nether_vapor";
    newscript->GetAI = GetAI_mob_nether_vapor;
    m_scripts[nrscripts++] = newscript;

    /*newscript = new Script;
    newscript->Name="mob_phoenix";
    newscript->GetAI = GetAI_mob_phoenix;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_phoenix_egg";
    newscript->GetAI = GetAI_mob_phoenix_egg;
    m_scripts[nrscripts++] = newscript;*/
}

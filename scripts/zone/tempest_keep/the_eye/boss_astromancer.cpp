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

#include "../../../sc_defines.h"

#define SPELL_ARCANE_MISSLES                 33031                 
#define SPELL_MARK_OF_THE_ASTROMANCER        42783
#define SPELL_BLINDING_LIGHT                 33009
#define SPELL_FEAR                           29321
#define SPELL_VOID_BOLT                      39329

#define SAY_PHASE1                           "Tal anu'men no Sin'dorei!"
#define SAY_PHASE2                           "I will crush your delusions of grandeur!"
#define SAY_PHASE21                          "Ha ha ha! You are hopelessly outmatched!"
#define SAY_VOID                             "Enough of this! Now I call upon the fury of the cosmos itself."
#define SAY_VOID1                            "I become ONE... with the VOID!"
#define SAY_KILL1                            "Your soul belongs to the Abyss!"
#define SAY_KILL2                            "By the blood of the Highborne!"
#define SAY_KILL3                            "For the Sunwell!"
#define SAY_DEATH                            "The warmth of the sun... awaits."

#define POS1_X                               444.67  
#define POS1_Y                               -365.50
#define POS1_Z                               17.524 
#define POS1_O                               2.2540
#define POS2_X                               421.34
#define POS2_Y                               -381.12
#define POS2_Z                               17.327
#define POS2_O                               1.8959
#define POS3_X                               439.42
#define POS3_Y                               -387.85
#define POS3_Z                               17.196
#define POS3_O                               2.0027

#define SOLARIUM_AGENT                       18925
#define SOLARIUM_PRIEST                      18806
#define MODEL_VOIDWALKER                     17981
#define MODEL_HUMAN                          18805

#define SOLARIUM_HEAL                        41378
#define SOLARIUM_SMITE                       38582
#define SOLARIUM_SILENCE                     37160

struct MANGOS_DLL_DECL boss_high_astromancer_solarianAI : public ScriptedAI
{
    boss_high_astromancer_solarianAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Void = 0;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 ArcaneMissles_Timer;
    uint32 MarkOfTheAstromancer_Timer;
    uint32 BlindingLight_Timer;
    uint32 Fear_Timer;
    uint32 VoidBolt_Timer;
    uint32 Phase1_Timer;
    uint32 Phase2_Timer;
    uint32 Phase3_Timer;
    uint32 Summon_Timer;

    bool HumanForm;
    bool VoidForm;
    bool InCombat;

    uint8 Phase;

    uint64 Void;

    void EnterEvadeMode(){
        ArcaneMissles_Timer = 5000;
        MarkOfTheAstromancer_Timer = 15000;
        BlindingLight_Timer = 45000;
        Fear_Timer = 20000;
        VoidBolt_Timer = 10000;
        Phase1_Timer = 50000;
        Phase2_Timer = 10000;
        Phase3_Timer = 15000;
        Summon_Timer = 10000;
        HumanForm = true;
        InCombat = false;
        VoidForm = false;
        Phase = 1;

        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_HUMAN);

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData("HighAstromancerSolarianEvent", 0);

        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
    }

    void StartEvent()
    {
        DoYell(SAY_PHASE1, LANG_UNIVERSAL, NULL);

        if(pInstance)
            pInstance->SetData("HighAstromancerSolarianEvent", 1);

        InCombat = true;

    }

    void KIlledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_KILL1, LANG_UNIVERSAL, NULL);
            break;

        case 1:
            DoYell(SAY_KILL2, LANG_UNIVERSAL, NULL);
            break;

        case 2:
            DoYell(SAY_KILL3, LANG_UNIVERSAL, NULL);
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);

        if(pInstance)
            pInstance->SetData("HighAstromancerSolarianEvent", 0);
    }

    void AttackStart(Unit *who)
    {
        if(!who && who !=m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            if(!InCombat)
                StartEvent();
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

                DoStartMeleeAttack(who);

                if(!InCombat)
                    StartEvent();
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if (Phase == 1)
        {
            //ArcaneMissles_Timer
            if (ArcaneMissles_Timer < diff)
            {
                //Solarian casts Arcane Missiles on on random targets in the raid.
                Unit *target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                if(target)
                    DoCast(target, SPELL_ARCANE_MISSLES);

                ArcaneMissles_Timer = 4000;
            }else ArcaneMissles_Timer -= diff;

            //MarkOfTheAstromancer_Timer
            if (MarkOfTheAstromancer_Timer < diff)
            {
                //A debuff that lasts for 5 seconds, cast several times each phase on a random raid member, but not the main tank
                Unit *target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 1);

                if(target)
                    DoCast(target, SPELL_MARK_OF_THE_ASTROMANCER);

                MarkOfTheAstromancer_Timer = 15000;
            }else MarkOfTheAstromancer_Timer -= diff;

            //BlindingLight_Timer
            if (BlindingLight_Timer < diff)
            {
                //She casts this spell every 45 seconds. It is a kind of Moonfire spell, which she strikes down on the whole raid simultaneously. It hits everyone in the raid for 2280 to 2520 arcane damage.
                DoCast(m_creature->getVictim(), SPELL_BLINDING_LIGHT);

                BlindingLight_Timer = 45000;
            }else BlindingLight_Timer -= diff;

            //Phase1_Timer
            if (Phase1_Timer < diff)
            {
                Phase = 2;
                Phase1_Timer = 50000;
                //After these 50 seconds she portals to the middle of the room and disappears, leaving 3 light portals behind.
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetVisibility(VISIBILITY_OFF);

                m_creature->RemoveAllAuras();

                switch (rand()%2)
                {
                case 0:
                    DoYell(SAY_PHASE2, LANG_UNIVERSAL, NULL);
                    break;

                case 1:
                    DoYell(SAY_PHASE21, LANG_UNIVERSAL, NULL);
                    break;
                }
            }
        } else if(Phase == 2){

            //10 seconds after Solarian disappears, 12 mobs spawn out of the three portals.
            if (Phase2_Timer < diff)
            {
                Phase = 3;
                Creature *Solarian_Agent;
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS1_X, POS1_Y, POS1_Z, POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS1_X, POS1_Y, POS1_Z, POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS1_X, POS1_Y, POS1_Z, POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS1_X, POS1_Y, POS1_Z, POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS2_X, POS2_Y, POS2_Z, POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS2_X, POS2_Y, POS2_Z, POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS2_X, POS2_Y, POS2_Z, POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS2_X, POS2_Y, POS2_Z, POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS3_X, POS3_Y, POS3_Z, POS3_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS3_X, POS3_Y, POS3_Z, POS3_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS3_X, POS3_Y, POS3_Z, POS3_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Solarian_Agent = m_creature->SummonCreature(SOLARIUM_AGENT, POS3_X, POS3_Y, POS3_Z, POS3_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                Phase2_Timer = 10000;
            } else Phase2_Timer -= diff;
        } else if(Phase == 3){

            //Check Phase3_Timer
            if(Phase3_Timer < diff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetVisibility(VISIBILITY_ON);

                DoYell(SAY_PHASE1, LANG_UNIVERSAL, NULL);

                //15 seconds later Solarian reappears out of one of the 3 portals. Simultaneously, 2 healers appear in the two other portals.
                switch(rand()%3)
                {
                case 0:
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->Relocate(POS1_X, POS1_Y, POS1_Z, POS1_O);
                    m_creature->SendMoveToPacket(POS1_X, POS1_Y, POS1_Z, false, 0);

                    Creature *Solarian_Priest;
                    Solarian_Priest = m_creature->SummonCreature(SOLARIUM_PRIEST, POS2_X, POS2_Y, POS2_Z, POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    Solarian_Priest = m_creature->SummonCreature(SOLARIUM_PRIEST, POS3_X, POS3_Y, POS3_Z, POS3_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    break;

                case 1:
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->Relocate(POS2_X, POS2_Y, POS2_Z, POS2_O);
                    m_creature->SendMoveToPacket(POS2_X, POS2_Y, POS2_Z, false, 0);

                    Solarian_Priest = m_creature->SummonCreature(SOLARIUM_PRIEST, POS1_X, POS1_Y, POS1_Z, POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    Solarian_Priest = m_creature->SummonCreature(SOLARIUM_PRIEST, POS3_X, POS3_Y, POS3_Z, POS3_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    break;

                case 2:
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->Relocate(POS3_X, POS3_Y, POS3_Z, POS3_O);
                    m_creature->SendMoveToPacket(POS3_X, POS3_Y, POS3_Z, false, 0);

                    Solarian_Priest = m_creature->SummonCreature(SOLARIUM_PRIEST, POS1_X, POS1_Y, POS1_Z, POS1_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    Solarian_Priest = m_creature->SummonCreature(SOLARIUM_PRIEST, POS2_X, POS2_Y, POS2_Z, POS2_O, TEMPSUMMON_CORPSE_DESPAWN, 0);

                    break;
                }
                Phase = 1;
                Phase3_Timer = 25000;
            }else Phase3_Timer -= diff;
        } else if(Phase == 4){
            //Fear_Timer
            if (Fear_Timer < diff)
            {
                Unit *target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);

                if(target)
                    DoCast(target, SPELL_FEAR);
                Fear_Timer = 20000;
            }else Fear_Timer -= diff;

            //VoidBolt_Timer
            if (VoidBolt_Timer < diff)
            {
                Unit *target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);

                if(target)
                    DoCast(target, SPELL_VOID_BOLT);
                VoidBolt_Timer = 10000;
            }else VoidBolt_Timer -= diff;
        }

        //When Solarian reaches 20% she will transform into a huge void walker.
        if(Phase != 4 && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth())<20))
        {

            Phase = 4;

            //To make sure she wont be invisible or not selecatble
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetVisibility(VISIBILITY_ON);
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_VOIDWALKER);
            switch(rand()%2)
            {
            case 0:
                DoYell(SAY_VOID, LANG_UNIVERSAL, NULL);
                break;

            case 1:
                DoYell(SAY_VOID1, LANG_UNIVERSAL, NULL);
                break;
            }

        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_solarian_priestAI : public ScriptedAI
{
    mob_solarian_priestAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 healTimer;
    uint32 holysmiteTimer;
    uint32 aoesilenceTimer;

    void EnterEvadeMode()
    {
        healTimer = 10000;
        holysmiteTimer = 10000;
        aoesilenceTimer = 15000;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who){ return; }

    void MoveInLineOfSight(Unit *who) { return; }

    void UpdateAI(const uint32 diff)
    {
        Unit* astromancer = Unit::GetUnit((*m_creature), pInstance->GetData64("Astromancer"));
        if (healTimer < diff)
        {  
            DoCast(astromancer,SOLARIUM_HEAL);
            healTimer = 10000;
        }else healTimer -= diff;
        if(holysmiteTimer < diff)
        {
            DoCast(astromancer->getVictim(), SOLARIUM_SMITE);
            holysmiteTimer = 10000;
        }else holysmiteTimer -= diff;
        if(aoesilenceTimer < diff)
        {
            DoCast(NULL,SOLARIUM_SILENCE);
            aoesilenceTimer = 15000;
        }else aoesilenceTimer -= diff;
    }
};

CreatureAI* GetAI_mob_solarian_priest(Creature *_Creature)
{
    return new mob_solarian_priestAI (_Creature);
}

CreatureAI* GetAI_boss_high_astromancer_solarian(Creature *_Creature)
{
    return new boss_high_astromancer_solarianAI (_Creature);
}

void AddSC_boss_high_astromancer_solarian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_high_astromancer_solarian";
    newscript->GetAI = GetAI_boss_high_astromancer_solarian;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_solarian_priest";
    newscript->GetAI = GetAI_mob_solarian_priest;
    m_scripts[nrscripts++] = newscript;
}

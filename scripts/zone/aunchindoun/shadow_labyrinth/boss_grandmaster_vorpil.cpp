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

/* ScriptData
SDName: Boss_Grandmaster_Vorpil
SD%Complete: 50
SDComment: Missing Void Rifts, Void Traveler, and Draw Shadows
EndScriptData */


#define SPELL_DRAW_SHADOW_VISUAL       33563 // right but only animation works / too complex, it's useless implement it
#define SPELL_SHADOW_BOLT_VOLLEY       32963

#define SAY_INTRO                "Keep your minds focused for the days of reckoning are close at hand. Soon, the destroyer of worlds will return to make good on his promise. Soon the destruction of all that is will begin!"
#define SAY_AGGRO1               "I'll make an offering of your blood!"
#define SAY_AGGRO2               "You'll be a fine example, for the others."
#define SAY_AGGRO3               "Good, a worthy sacrifice."
#define SAY_HELP                 "Come to my aid, heed your master now!"
#define SAY_SLAY1                "I serve with pride."
#define SAY_SLAY2                "Your death is for the greater cause!"
#define SAY_DEATH                "I give my life... Gladly."

#define SOUND_INTRO              10522
#define SOUND_AGGRO1             10524
#define SOUND_AGGRO2             10525
#define SOUND_AGGRO3             10526
#define SOUND_HELP               10523
#define SOUND_SLAY1              10527
#define SOUND_SLAY2              10528
#define SOUND_DEATH              10529

#define VOID_TRAVELER            19226

struct MANGOS_DLL_DECL boss_grandmaster_vorpilAI : public ScriptedAI
{
    boss_grandmaster_vorpilAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        //say intro only one time
        Intro = false;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 ShadowBoltVolley_Timer;
    bool Intro;

    bool InCombat;

    void EnterEvadeMode()
    {
        ShadowBoltVolley_Timer = 15000;

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

        if(pInstance)
            pInstance->SetData("GrandmasterVorpilEvent", 0);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
            case 0:
            DoYell(SAY_SLAY1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY1);
            break;

            case 1:
            DoYell(SAY_SLAY2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_SLAY2);
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if(pInstance)
            pInstance->SetData("GrandmasterVorpilEvent", 2);
    }

    void StartEvent()
    {
        switch(rand()%3)
        {
            case 0:
            DoYell(SAY_AGGRO1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
            break;

            case 1:
            DoYell(SAY_AGGRO2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
            break;

            case 2:
            DoYell(SAY_AGGRO3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_AGGRO3);
            break;
        }

        InCombat = true;

        if(pInstance)
            pInstance->SetData("GrandmasterVorpilEvent", 1);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
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
        else if (!Intro && m_creature->IsWithinDistInMap(who, 50)) //not sure about right radius
        {
            DoYell(SAY_INTRO, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_INTRO);
            Intro = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //ShadowBoltVolley_Timer
        if (ShadowBoltVolley_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);

            ShadowBoltVolley_Timer = 30000;
        }else ShadowBoltVolley_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_grandmaster_vorpil(Creature *_Creature)
{
    return new boss_grandmaster_vorpilAI (_Creature);
}

void AddSC_boss_grandmaster_vorpil()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_grandmaster_vorpil";
    newscript->GetAI = GetAI_boss_grandmaster_vorpil;
    m_scripts[nrscripts++] = newscript;
}
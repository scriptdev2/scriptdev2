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
SDName: Boss_Leotheras_The_Blind
SD%Complete: 50
SDComment: Missing Inner Demons
EndScriptData */

#include "../../../sc_defines.h"

#define SPELL_WHIRLWIND         40653
#define SPELL_CHAOS_BLAST       37675
// #define SPELL_INSIDIOUS_WHISPER 37676 // useless - dummy effect that can't be implemented

#define SAY_AGGRO               "Finally my banishment ends!"
#define SAY_SWITCH_TO_DEMON     "Be gone trifling elf. I'm in control now."
#define SAY_INNER_DEMONS        "We all have our demons..."
#define SAY_DEMON_SLAY1         "I have no equal."
#define SAY_DEMON_SLAY2         "Perish, mortal."
#define SAY_DEMON_SLAY3         "Yes, YES! Ahahah!"
#define SAY_NIGHTELF_SLAY1      "Kill! KILL!"
#define SAY_NIGHTELF_SLAY2      "That's right! Yes!"
#define SAY_NIGHTELF_SLAY3      "Who's the master now?"
#define SAY_FINAL_FORM          "No! NO! What have you done?! I am the master, do you hear me? I... aaghh... Can't... contain him..."
#define SAY_FREE                "At last I am liberated. It has been too long since I have tasted true freedom!"
#define SAY_DEATH               "You cannot kill me! Fools, I'll be back! I'll... aarghh..."

#define SOUND_AGGRO             11312
#define SOUND_SWITCH_TO_DEMON   11304
#define SOUND_INNER_DEMONS      11305
#define SOUND_DEMON_SLAY1       11306
#define SOUND_DEMON_SLAY2       11307
#define SOUND_DEMON_SLAY3       11308
#define SOUND_NIGHTELF_SLAY1    11314
#define SOUND_NIGHTELF_SLAY2    11315
#define SOUND_NIGHTELF_SLAY3    11316
#define SOUND_FINAL_FORM        11313
#define SOUND_FREE              11309
#define SOUND_DEATH             11317

#define MODEL_DEMON             14555
#define MODEL_NIGHTELF          20514

struct MANGOS_DLL_DECL boss_leotheras_the_blindAI : public ScriptedAI
{
    boss_leotheras_the_blindAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;

    uint32 Whirlwind_Timer;
    uint32 ChaosBlast_Timer;
    uint32 Switch_Timer;

    bool DemonForm;
    bool InWhirlwind;
    bool InCombat;
    bool IsFinalForm;

    Creature *Invisible;

    void EnterEvadeMode()
    {
        Whirlwind_Timer = 20000;
        ChaosBlast_Timer = 1000;
        Switch_Timer = 45000;

        DemonForm = false;
        InWhirlwind = false;
        InCombat = false;
        IsFinalForm = false;

        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_NIGHTELF);

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        if(pInstance)
            pInstance->SetData("LeotherasTheBlindEvent", 0);
    }

    void SetDemonFinalForm(Unit *target)
    {
        IsFinalForm = true;
        DemonForm = true;
        InWhirlwind = false;

        DoYell(SAY_FREE, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_FREE);

        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_DEMON);

        DoStartMeleeAttack(target);
    }

    void SetNightelfFinalForm()
    {
        IsFinalForm = true;
        DemonForm = false;

        DoYell(SAY_FINAL_FORM, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_FINAL_FORM);

        m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_NIGHTELF);
    }

    void StartEvent()
    {
        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);

        if(pInstance)
            pInstance->SetData("LeotherasTheBlindEvent", 1);

        InCombat = true;
    }

    void KilledUnit(Unit *victim)
    {
        if(DemonForm)
            switch(rand()%3)
            {
                case 0:
                DoYell(SAY_DEMON_SLAY1, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_DEMON_SLAY1);
                break;

                case 1:
                DoYell(SAY_DEMON_SLAY2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_DEMON_SLAY2);
                break;

                case 2:
                DoYell(SAY_DEMON_SLAY3, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_DEMON_SLAY3);
                break;
            }
        else
            switch(rand()%3)
            {
                case 0:
                DoYell(SAY_NIGHTELF_SLAY1, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_NIGHTELF_SLAY1);
                break;

                case 1:
                DoYell(SAY_NIGHTELF_SLAY2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_NIGHTELF_SLAY2);
                break;

                case 2:
                DoYell(SAY_NIGHTELF_SLAY3, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_NIGHTELF_SLAY3);
                break;
            }
    }

    void JustDied(Unit *victim)
    {
        if(!DemonForm)
        {
            DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_DEATH);
        }

        if(pInstance)
            pInstance->SetData("LeotherasTheBlindEvent", 0);
    }

    void AttackStart(Unit *who)
    {
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            if(!DemonForm && !InWhirlwind)
                DoStartMeleeAttack(who);
            if(!InCombat && !IsFinalForm)
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

                if(!DemonForm && !InWhirlwind)
                    DoStartMeleeAttack(who);
                if(!InCombat && !IsFinalForm)
                    StartEvent();
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(!DemonForm)
        {
            //Whirlwind_Timer
            if(Whirlwind_Timer < diff)
            {
                if(!InWhirlwind)
                {
                    DoCast(m_creature, SPELL_WHIRLWIND);
                    InWhirlwind = true;
                    Whirlwind_Timer = 15000;
                }
                else
                {
                    InWhirlwind = false;
                    Whirlwind_Timer = 10000;
                }
            }else Whirlwind_Timer -= diff;

            //Switch_Timer
            if(!IsFinalForm && Switch_Timer < diff)
            {
                // switch to demon form
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_DEMON);
                DoYell(SAY_SWITCH_TO_DEMON, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_SWITCH_TO_DEMON);
                DemonForm = true;
                InWhirlwind = false;

                Switch_Timer = 60000;

            }else Switch_Timer -= diff;
        }
        else
        {
            //ChaosBlast_Timer
            if(ChaosBlast_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CHAOS_BLAST);
                ChaosBlast_Timer = 1000;
            }else ChaosBlast_Timer -= diff;

            //Switch_Timer
            if(!IsFinalForm && Switch_Timer < diff)
            {
                // switch to nightelf form
                m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_NIGHTELF);
                DemonForm = false;

                Switch_Timer = 45000;

            }else Switch_Timer -= diff;
        }

        if(!IsFinalForm && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 15)
        {
            // at this point he divides himself in two parts
            Creature *DemonForm = DoSpawnCreature(21215, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 999999);

            if(DemonForm)
                ((boss_leotheras_the_blindAI*)DemonForm->AI())->SetDemonFinalForm(m_creature->getVictim());

            SetNightelfFinalForm();
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_leotheras_the_blind(Creature *_Creature)
{
    return new boss_leotheras_the_blindAI (_Creature);
}

void AddSC_boss_leotheras_the_blind()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_leotheras_the_blind";
    newscript->GetAI = GetAI_boss_leotheras_the_blind;
    m_scripts[nrscripts++] = newscript;
}

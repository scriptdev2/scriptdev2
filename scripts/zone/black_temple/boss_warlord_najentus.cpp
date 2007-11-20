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
SDName: Boss_Warlord_Najentus
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"

//Aggro
#define SAY_AGGRO "You will die, in the name of Lady Vashj!"
#define SOUND_AGGRO 11450

//Needle (Random)
#define SAY_NEEDLE1 "Stick around!"
#define SOUND_NEEDLE1 11451

#define SAY_NEEDLE2 "I'll deal with you later!"
#define SOUND_NEEDLE2 11452

//Slay
#define SAY_SLAY1 "Your success was short lived!"
#define SOUND_SLAY1 11455

#define SAY_SLAY2 "Time for you, to go!"
#define SOUND_SLAY2 11456

//Special
#define SAY_SPECIAL1 "Bel'anen dal'lorei!"
#define SOUND_SPECIAL1 11453

#define SAY_SPECIAL2 "Blood will flow!"
#define SOUND_SPECIAL2 11454

//Enrage
#define SAY_ENRAGE "My patience has ran out! Die, DIE!"
#define SOUND_ENRAGE 11458

//Death
#define SAY_DEATH "Lord Illidan will... crush you."
#define SOUND_DEATH 11459


//Spells
#define SPELL_CRASHINGWAVE               40100
#define SPELL_NEEDLE_SINGLE               39835
#define SPELL_NEEDLE_AOE               39968
#define SPELL_ENRAGE                    28747
#define SPELL_TIDAL_BURST               39878

struct MANGOS_DLL_DECL boss_najentusAI : public ScriptedAI
{
    boss_najentusAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        pInstance = NULL;
        SetVariables();
    }

    ScriptedInstance* pInstance;
    uint32 CrashingWaveTimer;
    uint32 NeedleTimer;
    uint32 EnrageTimer;
    uint32 SpecialYellTimer;

    bool InCombat;

    void SetVariables()
    {
        InCombat = false;

        CrashingWaveTimer = 28000;
        NeedleTimer = 24000;
        EnrageTimer = 480000;
        SpecialYellTimer = 45000 + (rand()%76)*1000;

        if(pInstance)
            pInstance->SetData("HighWarlordNajentusEvent", 0);
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
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
        {
            pInstance->SetData("HighWarlordNajentusEvent", 3);
        }

        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    } 


    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {

            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                if(pInstance)
                    pInstance->SetData("HighWarlordNajentusEvent", 1);
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
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

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    if(pInstance)
                        pInstance->SetData("HighWarlordNajentusEvent", 1);
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Crashing Wave
        if(CrashingWaveTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CRASHINGWAVE);
            CrashingWaveTimer = 28500;
        }else CrashingWaveTimer -= diff;

        // Enrage
        if(EnrageTimer < diff)
        {
            DoYell(SAY_ENRAGE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
            m_creature->CastSpell(m_creature, SPELL_ENRAGE, true);
            DoCast(m_creature->getVictim(), SPELL_TIDAL_BURST);
            EnrageTimer = 480000;
        }else EnrageTimer -= diff;

        // Needle
        if(NeedleTimer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            m_creature->CastSpell(target, SPELL_NEEDLE_AOE, true);
            DoCast(target, SPELL_NEEDLE_SINGLE);
            NeedleTimer = 60000;

            switch(rand()%4)
            {
            case 0:
                DoYell(SAY_NEEDLE1, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_NEEDLE1);
                break;
            case 1:
                DoYell(SAY_NEEDLE2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_NEEDLE2);
                break;
            }
        }else NeedleTimer -= diff;

        if(SpecialYellTimer < diff)
        {
            switch(rand()%2)
            {
            case 0:
                DoPlaySoundToSet(m_creature, SOUND_SPECIAL1);
                break;
            case 1:
                DoYell(SAY_SPECIAL2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_SPECIAL2);
                break;
            }
            SpecialYellTimer = 45000 + (rand()%76)*1000;
        } else SpecialYellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_najentus(Creature *_Creature)
{
    return new boss_najentusAI (_Creature);
}


void AddSC_boss_najentus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_najentus";
    newscript->GetAI = GetAI_boss_najentus;
    m_scripts[nrscripts++] = newscript;
}

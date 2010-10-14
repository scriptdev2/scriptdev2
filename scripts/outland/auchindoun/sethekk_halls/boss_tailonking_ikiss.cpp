/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Talon_King_Ikiss
SD%Complete: 80
SDComment: Heroic supported. Some details missing, but most are spell related.
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "precompiled.h"
#include "sethekk_halls.h"

#define SAY_INTRO                   -1556007
#define SAY_AGGRO_1                 -1556008
#define SAY_AGGRO_2                 -1556009
#define SAY_AGGRO_3                 -1556010
#define SAY_SLAY_1                  -1556011
#define SAY_SLAY_2                  -1556012
#define SAY_DEATH                   -1556013
#define EMOTE_ARCANE_EXP            -1556015

#define SPELL_BLINK                 38194
#define SPELL_BLINK_TELEPORT        38203
#define SPELL_MANA_SHIELD           38151
#define SPELL_ARCANE_BUBBLE         9438
#define H_SPELL_SLOW                35032

#define SPELL_POLYMORPH             38245
#define H_SPELL_POLYMORPH           43309

#define SPELL_ARCANE_VOLLEY         35059
#define H_SPELL_ARCANE_VOLLEY       40424

#define SPELL_ARCANE_EXPLOSION      38197
#define H_SPELL_ARCANE_EXPLOSION    40425

struct MANGOS_DLL_DECL boss_talon_king_ikissAI : public ScriptedAI
{
    boss_talon_king_ikissAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 ArcaneVolley_Timer;
    uint32 Sheep_Timer;
    uint32 Blink_Timer;
    uint32 Slow_Timer;

    bool ManaShield;
    bool Blink;
    bool Intro;

    void Reset()
    {
        ArcaneVolley_Timer = 5000;
        Sheep_Timer = 8000;
        Blink_Timer = 35000;
        Slow_Timer = urand(15000, 30000);
        Blink = false;
        Intro = false;
        ManaShield = false;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_creature->getVictim() && who->isTargetableForAttack() && (m_creature->IsHostileTo(who)) && who->isInAccessablePlaceFor(m_creature))
        {
            if (!Intro && m_creature->IsWithinDistInMap(who, 100))
            {
                Intro = true;
                DoScriptText(SAY_INTRO, m_creature);
            }

            if (!m_creature->CanFly() && m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who))
            {
                who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(who);
            }
        }
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO_1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(DATA_IKISSDOOREVENT, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Blink)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_EXPLOSION : H_SPELL_ARCANE_EXPLOSION);
            m_creature->CastSpell(m_creature,SPELL_ARCANE_BUBBLE,true);
            Blink = false;
        }

        if (ArcaneVolley_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_VOLLEY : H_SPELL_ARCANE_VOLLEY);
            ArcaneVolley_Timer = urand(7000, 12000);
        }else ArcaneVolley_Timer -= diff;

        if (Sheep_Timer < diff)
        {
            //second top aggro target in normal, random target in heroic correct?
            Unit *target = NULL;
            if (m_bIsRegularMode ? target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 1) : target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_POLYMORPH : H_SPELL_POLYMORPH);
            Sheep_Timer = urand(15000, 17500);
        }else Sheep_Timer -= diff;

        //may not be correct time to cast
        if (!ManaShield && m_creature->GetHealthPercent() < 20.0f)
        {
            DoCastSpellIfCan(m_creature,SPELL_MANA_SHIELD);
            ManaShield = true;
        }

        if (!m_bIsRegularMode)
        {
            if (Slow_Timer < diff)
            {
                DoCastSpellIfCan(m_creature,H_SPELL_SLOW);
                Slow_Timer = urand(15000, 40000);
            }else Slow_Timer -= diff;
        }

        if (Blink_Timer < diff)
        {
            DoScriptText(EMOTE_ARCANE_EXP, m_creature);

            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                if (m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(false);

                //Spell doesn't work, but we use for visual effect at least
                DoCastSpellIfCan(target,SPELL_BLINK);

                float X = target->GetPositionX();
                float Y = target->GetPositionY();
                float Z = target->GetPositionZ();

                m_creature->GetMap()->CreatureRelocation(m_creature,X,Y,Z,0.0f);
                m_creature->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL, SPLINEFLAG_WALKMODE, 1);

                DoCastSpellIfCan(target,SPELL_BLINK_TELEPORT);
                Blink = true;
            }
            Blink_Timer = urand(35000, 40000);
        }else Blink_Timer -= diff;

        if (!Blink)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_talon_king_ikiss(Creature* pCreature)
{
    return new boss_talon_king_ikissAI(pCreature);
}

void AddSC_boss_talon_king_ikiss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_talon_king_ikiss";
    newscript->GetAI = &GetAI_boss_talon_king_ikiss;
    newscript->RegisterSelf();
}

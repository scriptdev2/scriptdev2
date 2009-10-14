/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Grandmaster_Vorpil
SD%Complete: 75
SDComment: Despawn all summoned on death not implemented. Void Traveler effects not implemented.
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "shadow_labyrinth.h"

#define SAY_INTRO                       -1555028
#define SAY_AGGRO1                      -1555029
#define SAY_AGGRO2                      -1555030
#define SAY_AGGRO3                      -1555031
#define SAY_HELP                        -1555032
#define SAY_SLAY1                       -1555033
#define SAY_SLAY2                       -1555034
#define SAY_DEATH                       -1555035

#define SPELL_DRAW_SHADOWS              33563
#define SPELL_VOID_PORTAL_A             33566               //spell only summon one unit, but we use it for the visual effect and summon the 4 other portals manual way(only one spell exist)
#define SPELL_VOID_PORTAL_VISUAL        33569
#define SPELL_SHADOW_BOLT_VOLLEY        32963
#define SPELL_SUMMON_VOIDWALKER_A       33582
#define SPELL_SUMMON_VOIDWALKER_B       33583
#define SPELL_SUMMON_VOIDWALKER_C       33584
#define SPELL_SUMMON_VOIDWALKER_D       33585
#define SPELL_SUMMON_VOIDWALKER_E       33586
#define SPELL_RAIN_OF_FIRE              33617
#define H_SPELL_RAIN_OF_FIRE            39363
#define H_SPELL_BANISH                  38791

#define ENTRY_VOID_PORTAL               19224
#define ENTRY_VOID_TRAVELER             19226

#define LOCX                            -253.06f
#define LOCY                            -264.02f
#define LOCZ                            17.08

struct MANGOS_DLL_DECL boss_grandmaster_vorpilAI : public ScriptedAI
{
    boss_grandmaster_vorpilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
        Intro = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 ShadowBoltVolley_Timer;
    uint32 DrawShadows_Timer;
    uint32 Teleport_Timer;
    uint32 VoidTraveler_Timer;
    uint32 Banish_Timer;
    bool Intro;
    bool Teleport;

    void Reset()
    {
        ShadowBoltVolley_Timer = urand(7000, 14000);
        DrawShadows_Timer = 40000;
        Teleport_Timer = 1000;
        VoidTraveler_Timer = 20000;
        Banish_Timer = 25000;
        Teleport = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VORPIL, NOT_STARTED);
    }

    void MoveInLineOfSight(Unit *who)
    {
        //not sure about right radius
        if (!Intro && m_creature->IsWithinDistInMap(who, 50))
        {
            DoScriptText(SAY_INTRO, m_creature);
            Intro = true;
        }

        ScriptedAI::MoveInLineOfSight(who);
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        DoCast(m_creature, SPELL_VOID_PORTAL_A,true);
        m_creature->SummonCreature(ENTRY_VOID_PORTAL,-262.40,-229.57,17.08,0,TEMPSUMMON_CORPSE_DESPAWN,0);
        m_creature->SummonCreature(ENTRY_VOID_PORTAL,-260.35,-297.56,17.08,0,TEMPSUMMON_CORPSE_DESPAWN,0);
        m_creature->SummonCreature(ENTRY_VOID_PORTAL,-292.05,-270.37,12.68,0,TEMPSUMMON_CORPSE_DESPAWN,0);
        m_creature->SummonCreature(ENTRY_VOID_PORTAL,-301.64,-255.97,12.68,0,TEMPSUMMON_CORPSE_DESPAWN,0);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VORPIL, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustSummoned(Creature *summoned)
    {
        if (summoned->GetEntry() == ENTRY_VOID_TRAVELER)
        {
            summoned->GetMotionMaster()->MoveChase(m_creature);
            summoned->SetSpeed(MOVE_WALK,0.8,true);
        }

        if (summoned->GetEntry() == ENTRY_VOID_PORTAL)
            summoned->CastSpell(summoned,SPELL_VOID_PORTAL_VISUAL,true);
    }

    void JustDied(Unit *victim)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VORPIL, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Teleport)
        {
            if (Teleport_Timer <= diff)
            {
                m_creature->GetMap()->CreatureRelocation(m_creature, LOCX, LOCY, LOCZ, 0.0f);
                m_creature->SendMonsterMove(LOCX, LOCY, LOCZ, 0, MONSTER_MOVE_WALK, 0);

                float ranX = LOCX;
                float ranY = LOCY;
                float ranZ = LOCZ;

                std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
                for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    Unit* target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                    if (target && target->GetTypeId() == TYPEID_PLAYER)
                    {
                        target->GetRandomPoint(LOCX,LOCY,LOCZ,3.0,ranX,ranY,ranZ);
                        DoTeleportPlayer(target,ranX,ranY,ranZ,m_creature->GetAngle(m_creature->GetPositionX(),m_creature->GetPositionY()));
                    }
                }
                Teleport = false;

                DoCast(m_creature->getVictim(), m_bIsHeroicMode ? H_SPELL_RAIN_OF_FIRE : SPELL_RAIN_OF_FIRE);

                Teleport_Timer = 1000;
            }else Teleport_Timer -= diff;
        }

        if (ShadowBoltVolley_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            ShadowBoltVolley_Timer = urand(15000, 30000);
        }else ShadowBoltVolley_Timer -= diff;

        if (DrawShadows_Timer < diff)
        {
            DoCast(m_creature,SPELL_DRAW_SHADOWS);
            DrawShadows_Timer = 30000;
            Teleport = true;
        }else DrawShadows_Timer -= diff;

        if (VoidTraveler_Timer < diff)
        {
            DoScriptText(SAY_HELP, m_creature);

            switch(urand(0, 4))
            {
                case 0: DoCast(m_creature,SPELL_SUMMON_VOIDWALKER_A,true); break;
                case 1: DoCast(m_creature,SPELL_SUMMON_VOIDWALKER_B,true); break;
                case 2: DoCast(m_creature,SPELL_SUMMON_VOIDWALKER_C,true); break;
                case 3: DoCast(m_creature,SPELL_SUMMON_VOIDWALKER_D,true); break;
                case 4: DoCast(m_creature,SPELL_SUMMON_VOIDWALKER_E,true); break;
            }
            //faster rate when below (X) health?
            VoidTraveler_Timer = 35000;
        }else VoidTraveler_Timer -= diff;

        if (m_bIsHeroicMode)
        {
            if (Banish_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1))
                    DoCast(target,H_SPELL_BANISH);
                Banish_Timer = 35000;
            }else Banish_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_grandmaster_vorpil(Creature* pCreature)
{
    return new boss_grandmaster_vorpilAI(pCreature);
}

void AddSC_boss_grandmaster_vorpil()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grandmaster_vorpil";
    newscript->GetAI = &GetAI_boss_grandmaster_vorpil;
    newscript->RegisterSelf();
}

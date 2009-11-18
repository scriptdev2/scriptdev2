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
SDName: Boss_Murmur
SD%Complete: 75
SDComment: Database should have `RegenHealth`=0 to prevent regen. Also, his shockwave triggered after magnetic pull may be incorrect. Murmur's Touch does not work properly.
SDCategory: Auchindoun, Shadow Labyrinth
EndScriptData */

#include "precompiled.h"
#include "shadow_labyrinth.h"

#define EMOTE_SONIC_BOOM            -1555036

#define SPELL_MAGNETIC_PULL         33689
#define SPELL_SONIC_BOOM_PRE        33923
#define SPELL_SONIC_BOOM_CAST       38795
#define SPELL_MURMURS_TOUCH         33711
#define SPELL_RESONANCE             33657
#define SPELL_SHOCKWAVE             33686

#define SPELL_SONIC_SHOCK           38797    //Heroic Spell
#define SPELL_THUNDERING_STORM      39365    //Heroic Spell

struct MANGOS_DLL_DECL boss_murmurAI : public ScriptedAI
{
    boss_murmurAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 SonicBoom_Timer;
    uint32 MurmursTouch_Timer;
    uint32 Resonance_Timer;
    uint32 MagneticPull_Timer;
    uint32 SonicShock_Timer;
    uint32 ThunderingStorm_Timer;
    bool CanSonicBoom;
    bool CanShockWave;
    uint64 pTarget;

    void Reset()
    {
        SonicBoom_Timer = 30000;
        MurmursTouch_Timer = urand(8000, 20000);
        Resonance_Timer = 5000;
        MagneticPull_Timer = urand(15000, 30000);
        SonicShock_Timer = urand(4000, 10000);
        ThunderingStorm_Timer = 12000;                //Casting directly after Sonic Boom.
        CanSonicBoom = false;
        CanShockWave = false;
        pTarget = 0;

        //database should have `RegenHealth`=0 to prevent regen
        uint32 hp = (m_creature->GetMaxHealth()*40)/100;
        if (hp)
            m_creature->SetHealth(hp);
    }

    void SonicBoomEffect()
    {
        std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
        for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
        {
           Unit* target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
           if (target && target->GetTypeId() == TYPEID_PLAYER)
           {
               //Not do anything without aura, spell can be resisted!
               if (target->HasAura(SPELL_SONIC_BOOM_CAST,1) && m_creature->IsWithinDistInMap(target, 34.0f))
               {
                   //This will be wrong calculation. Also, comments suggest it must deal damage
                   target->SetHealth(uint32(target->GetMaxHealth() - target->GetMaxHealth() * 0.8));
               }
           }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //SonicBoom_Timer
        if (SonicBoom_Timer < diff)
        {
            if (CanSonicBoom)
            {
                DoCast(m_creature, SPELL_SONIC_BOOM_CAST,true);
                SonicBoomEffect();

                CanSonicBoom = false;
                SonicBoom_Timer = 30000;
            }
            else
            {
                DoScriptText(EMOTE_SONIC_BOOM, m_creature);
                DoCast(m_creature,SPELL_SONIC_BOOM_PRE);
                CanSonicBoom = true;
                SonicBoom_Timer = 5000;
            }
        }else SonicBoom_Timer -= diff;

        //MurmursTouch_Timer
        if (MurmursTouch_Timer < diff)
        {
            /*Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)
                DoCast(target, SPELL_MURMURS_TOUCH);*/
            DoCast(m_creature, SPELL_MURMURS_TOUCH);
            MurmursTouch_Timer = urand(25000, 35000);
        }else MurmursTouch_Timer -= diff;

        //Resonance_Timer
        if (!CanSonicBoom && !m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            if (Resonance_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_RESONANCE);
                Resonance_Timer = m_bIsHeroicMode ? 3000 : 5000;
            }else Resonance_Timer -= diff;
        }

        if (m_bIsHeroicMode)
        {
            if (SonicShock_Timer < diff)
            {
                if (Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SONIC_SHOCK);
                SonicShock_Timer = urand(8000, 12000);
            }else SonicShock_Timer -= diff;

            if (ThunderingStorm_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_THUNDERING_STORM);
                ThunderingStorm_Timer = 12000;
            }else ThunderingStorm_Timer -= diff;
        }

        //MagneticPull_Timer
        if (MagneticPull_Timer < diff)
        {
            if (!CanShockWave)
            {
                if (Unit* temp = SelectUnit(SELECT_TARGET_RANDOM,0))
                {
                    if (temp->GetTypeId() == TYPEID_PLAYER)
                    {
                        DoCast(temp, SPELL_MAGNETIC_PULL);
                        pTarget = temp->GetGUID();
                        CanShockWave = true;
                    }
                    MagneticPull_Timer = 2500;
                }
            }
            else
            {
                if (Unit* target = Unit::GetUnit(*m_creature,pTarget))
                    target->CastSpell(target,SPELL_SHOCKWAVE,true);

                MagneticPull_Timer = urand(15000, 30000);
                CanShockWave = false;
                pTarget = 0;
            }
        }else MagneticPull_Timer -= diff;

        //no meele if preparing for sonic boom
        if (!CanSonicBoom)
            DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_murmur(Creature* pCreature)
{
    return new boss_murmurAI(pCreature);
}

void AddSC_boss_murmur()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_murmur";
    newscript->GetAI = &GetAI_boss_murmur;
    newscript->RegisterSelf();
}

/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Azuregos
SD%Complete: 90
SDComment: Teleport not included, spell reflect not effecting dots (Core problem)
SDCategory: Azshara
EndScriptData */

#include "precompiled.h"

#define SAY_TELEPORT            -1000100

#define SPELL_MARKOFFROST        23182
#define SPELL_MANASTORM          21097
#define SPELL_CHILL              21098
#define SPELL_FROSTBREATH        21099
#define SPELL_REFLECT            22067
#define SPELL_CLEAVE              8255                      //Perhaps not right ID
#define SPELL_ENRAGE             23537

struct MANGOS_DLL_DECL boss_azuregosAI : public ScriptedAI
{
    boss_azuregosAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 MarkOfFrost_Timer;
    uint32 ManaStorm_Timer;
    uint32 Chill_Timer;
    uint32 Breath_Timer;
    uint32 Teleport_Timer;
    uint32 Reflect_Timer;
    uint32 Cleave_Timer;
    uint32 Enrage_Timer;
    bool Enraged;

    void Reset()
    {
        MarkOfFrost_Timer = 35000;
        ManaStorm_Timer = urand(5000, 17000);
        Chill_Timer = urand(10000, 30000);
        Breath_Timer = urand(2000, 8000);
        Teleport_Timer = 30000;
        Reflect_Timer = urand(15000, 30000);
        Cleave_Timer = 7000;
        Enrage_Timer = 0;
        Enraged = false;
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Teleport_Timer < diff)
        {
            DoScriptText(SAY_TELEPORT, m_creature);

            std::vector<ObjectGuid> vGuids;
            m_creature->FillGuidsListFromThreatList(vGuids);
            for (std::vector<ObjectGuid>::const_iterator i = vGuids.begin();i != vGuids.end(); ++i)
            {
                Unit* pUnit = m_creature->GetMap()->GetUnit(*i);

                if (pUnit && pUnit->GetTypeId() == TYPEID_PLAYER)
                    DoTeleportPlayer(pUnit, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+3, pUnit->GetOrientation());
            }

            DoResetThreat();
            Teleport_Timer = 30000;
        }else Teleport_Timer -= diff;

        //        //MarkOfFrost_Timer
        //        if (MarkOfFrost_Timer < diff)
        //        {
        //            DoCastSpellIfCan(m_creature->getVictim(),SPELL_MARKOFFROST);
        //            MarkOfFrost_Timer = 25000;
        //        }else MarkOfFrost_Timer -= diff;

        //Chill_Timer
        if (Chill_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CHILL);
            Chill_Timer = urand(13000, 25000);
        }else Chill_Timer -= diff;

        //Breath_Timer
        if (Breath_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_FROSTBREATH);
            Breath_Timer = urand(10000, 15000);
        }else Breath_Timer -= diff;

        //ManaStorm_Timer
        if (ManaStorm_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_MANASTORM);
            ManaStorm_Timer = urand(7500, 12500);
        }else ManaStorm_Timer -= diff;

        //Reflect_Timer
        if (Reflect_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_REFLECT);
            Reflect_Timer = urand(20000, 35000);
        }else Reflect_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = 7000;
        }else Cleave_Timer -= diff;

        //Enrage_Timer
        if (m_creature->GetHealthPercent() < 26.0f && !Enraged)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            Enraged = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_azuregos(Creature* pCreature)
{
    return new boss_azuregosAI(pCreature);
}

void AddSC_boss_azuregos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_azuregos";
    newscript->GetAI = &GetAI_boss_azuregos;
    newscript->RegisterSelf();
}

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
SDName: Boss_Nalorakk
SD%Complete: 80
SDComment: Todo: Trash Waves
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_WAVE1_AGGRO         = -1568010,
    SAY_WAVE2_STAIR1        = -1568011,
    SAY_WAVE3_STAIR2        = -1568012,
    SAY_WAVE4_PLATFORM      = -1568013,

    SAY_EVENT1_SACRIFICE    = -1568014,
    SAY_EVENT2_SACRIFICE    = -1568015,

    SAY_AGGRO               = -1568016,
    SAY_SURGE               = -1568017,
    SAY_TOBEAR              = -1568018,
    SAY_TOTROLL             = -1568019,
    SAY_BERSERK             = -1568020,
    SAY_SLAY1               = -1568021,
    SAY_SLAY2               = -1568022,
    SAY_DEATH               = -1568023,

    SPELL_BERSERK           = 45078,                        //unsure, this increases damage, size and speed

    //Defines for Troll form
    SPELL_BRUTALSWIPE       = 42384,
    SPELL_MANGLE            = 42389,
    SPELL_SURGE             = 42402,
    SPELL_BEARFORM          = 42377,

    //Defines for Bear form
    SPELL_LACERATINGSLASH   = 42395,
    SPELL_RENDFLESH         = 42397,
    SPELL_DEAFENINGROAR     = 42398
};

struct MANGOS_DLL_DECL boss_nalorakkAI : public ScriptedAI
{
    boss_nalorakkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 ChangeForm_Timer;
    uint32 BrutalSwipe_Timer;
    uint32 Mangle_Timer;
    uint32 Surge_Timer;
    uint32 LaceratingSlash_Timer;
    uint32 RendFlesh_Timer;
    uint32 DeafeningRoar_Timer;
    uint32 ShapeShiftCheck_Timer;
    uint32 Berserk_Timer;
    bool inBearForm;
    bool Berserking;
    bool ChangedToBear;
    bool ChangedToTroll;

    void Reset()
    {
        ChangeForm_Timer = 45000;
        BrutalSwipe_Timer = 12000;
        Mangle_Timer = 15000;
        Surge_Timer = 20000;
        LaceratingSlash_Timer = 6000;
        RendFlesh_Timer = 6000;
        DeafeningRoar_Timer = 20000;
        ShapeShiftCheck_Timer = 40000;
        Berserk_Timer = 600000;
        inBearForm = false;
        Berserking = false;
        ChangedToBear = false;
        ChangedToTroll = true;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_NALORAKK, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Berserking
        if ((Berserk_Timer < diff) && (!Berserking))
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            Berserking = true;
        }else Berserk_Timer -= diff;

        //Don't check if we're shapeshifted every UpdateAI
        if (ShapeShiftCheck_Timer < diff)
        {
            //This will return true if we have bearform aura
            inBearForm = m_creature->HasAura(SPELL_BEARFORM, 0);
            ShapeShiftCheck_Timer = 1000;
        }else ShapeShiftCheck_Timer -= diff;

        //Spells for Troll Form (only to be casted if we NOT have bear phase aura)
        if (!inBearForm)
        {
            //We just changed to troll form!
            if (!ChangedToTroll)
            {
                DoScriptText(SAY_TOTROLL, m_creature);

                ChangedToTroll = true;
                ChangedToBear = false;
                //Reset spell timers
                LaceratingSlash_Timer = urand(6000, 25000);
                RendFlesh_Timer = urand(6000, 25000);
                DeafeningRoar_Timer = urand(15000, 25000);
                ShapeShiftCheck_Timer = 40000;
            }

            //Brutal Swipe (some sources may say otherwise, but I've never seen this in Bear form)
            if (BrutalSwipe_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_BRUTALSWIPE);
                BrutalSwipe_Timer = urand(7000, 15000);
            }else BrutalSwipe_Timer -= diff;

            //Mangle
            if (Mangle_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_MANGLE);
                Mangle_Timer = urand(3000, 15000);
            }else Mangle_Timer -= diff;

            //Surge
            if (Surge_Timer < diff)
            {
                //select a random unit other than the main tank
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 1);

                //if there aren't other units, cast on the tank
                if (!target)
                    target = m_creature->getVictim();

                DoCast(target, SPELL_SURGE);
                DoScriptText(SAY_SURGE, m_creature);

                Surge_Timer = urand(15000, 32500);
            }else Surge_Timer -= diff;

            //Change to Bear Form if we're in Troll Form for 45sec
            if (ChangeForm_Timer < diff)
            {
                m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
                m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
                DoCast(m_creature, SPELL_BEARFORM);
                //And 30sec (bear form) + 45sec (troll form) before we should cast this again
                ChangeForm_Timer = 75000;
            }else ChangeForm_Timer -= diff;
        }
        //Spells for Bear Form (only to be casted if we have bear phase aura)
        else
        {
            //We just changed to bear form!
            if (!ChangedToBear)
            {
                DoScriptText(SAY_TOBEAR, m_creature);

                ChangedToBear = true;
                ChangedToTroll = false;
                //Reset spell timers
                Surge_Timer = urand(15000, 32000);
                BrutalSwipe_Timer = urand(7000, 20000);
                Mangle_Timer = urand(3000, 20000);
                ShapeShiftCheck_Timer = 25000;
            }

            //Lacerating Slash
            if (LaceratingSlash_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_LACERATINGSLASH);
                LaceratingSlash_Timer = urand(6000, 20000);
            }else LaceratingSlash_Timer -= diff;

            //Rend Flesh
            if (RendFlesh_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_RENDFLESH);
                RendFlesh_Timer = urand(6000, 20000);
            }else RendFlesh_Timer -= diff;

            //Deafening Roar
            if (DeafeningRoar_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_DEAFENINGROAR);
                DeafeningRoar_Timer = urand(15000, 25000);
            }else DeafeningRoar_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_nalorakk(Creature* pCreature)
{
    return new boss_nalorakkAI(pCreature);
}

void AddSC_boss_nalorakk()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_nalorakk";
    newscript->GetAI = &GetAI_boss_nalorakk;
    newscript->RegisterSelf();
}

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
SDName: Boss_Razuvious
SD%Complete: 50
SDComment: Missing adds and event is impossible without Mind Control
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

//Razuvious - NO TEXT sound only
//8852 aggro01 - Hah hah, I'm just getting warmed up!
//8853 aggro02 Stand and fight!
//8854 aggro03 Show me what you've got!
//8861 slay1 - You should've stayed home!
//8863 slay2-
//8858 cmmnd3 - You disappoint me, students!
//8855 cmmnd1 - Do as I taught you!
//8856 cmmnd2 - Show them no mercy!
//8859 cmmnd4 - The time for practice is over! Show me what you've learned!
//8861 Sweep the leg! Do you have a problem with that?
//8860 death - An honorable... death...
//8947 - Aggro Mixed? - ?

enum
{
    SOUND_AGGRO1            = 8852,
    SOUND_AGGRO2            = 8853,
    SOUND_AGGRO3            = 8854,
    SOUND_SLAY1             = 8861,
    SOUND_SLAY2             = 8863,
    SOUND_COMMND1           = 8855,
    SOUND_COMMND2           = 8856,
    SOUND_COMMND3           = 8858,
    SOUND_COMMND4           = 8859,
    SOUND_COMMND5           = 8861,
    SOUND_DEATH             = 8860,
    SOUND_AGGROMIX          = 8847,

    SPELL_UNBALANCINGSTRIKE = 26613,
    SPELL_DISRUPTINGSHOUT   = 29107
};

struct MANGOS_DLL_DECL boss_razuviousAI : public ScriptedAI
{
    boss_razuviousAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 m_uiUnbalancingStrikeTimer;
    uint32 m_uiDisruptingShoutTimer;
    uint32 m_uiCommandSoundTimer;

    void Reset()
    {
        m_uiUnbalancingStrikeTimer = 30000;                 //30 seconds
        m_uiDisruptingShoutTimer = 25000;                   //25 seconds
        m_uiCommandSoundTimer = 40000;                      //40 seconds
    }

    void KilledUnit(Unit* Victim)
    {
        if (urand(0, 3))
            return;

        switch(urand(0, 1))
        {
            case 0:
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, DONE);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0:
                DoPlaySoundToSet(m_creature, SOUND_AGGRO1);
                break;
            case 1:
                DoPlaySoundToSet(m_creature, SOUND_AGGRO2);
                break;
            case 2:
                DoPlaySoundToSet(m_creature, SOUND_AGGRO3);
                break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        // Unbalancing Strike
        if (m_uiUnbalancingStrikeTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_UNBALANCINGSTRIKE);
            m_uiUnbalancingStrikeTimer = 30000;
        }
        else
            m_uiUnbalancingStrikeTimer -= uiDiff;

        // Disrupting Shout
        if (m_uiDisruptingShoutTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_DISRUPTINGSHOUT);
            m_uiDisruptingShoutTimer = 25000;
        }
        else
            m_uiDisruptingShoutTimer -= uiDiff;

        // Command Sound
        if (m_uiCommandSoundTimer < uiDiff)
        {
            switch(urand(0, 4))
            {
                case 0:
                    DoPlaySoundToSet(m_creature, SOUND_COMMND1);
                    break;
                case 1:
                    DoPlaySoundToSet(m_creature, SOUND_COMMND2);
                    break;
                case 2:
                    DoPlaySoundToSet(m_creature, SOUND_COMMND3);
                    break;
                case 3:
                    DoPlaySoundToSet(m_creature, SOUND_COMMND4);
                    break;
                case 4:
                    DoPlaySoundToSet(m_creature, SOUND_COMMND5);
                    break;
            }

            m_uiCommandSoundTimer = 40000;
        }
        else
            m_uiCommandSoundTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_razuvious(Creature* pCreature)
{
    return new boss_razuviousAI(pCreature);
}

void AddSC_boss_razuvious()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_razuvious";
    NewScript->GetAI = &GetAI_boss_razuvious;
    NewScript->RegisterSelf();
}

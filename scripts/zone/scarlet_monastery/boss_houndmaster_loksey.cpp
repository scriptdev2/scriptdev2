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

/* ScriptData
SDName: Boss_Houndmaster_Loksey
SD%Complete: 100
SDComment: 
EndScriptData */

#include "../../sc_defines.h"



#define SPELL_SUMMONSCARLETHOUND        16174
#define SPELL_ENRAGE                    28747

#define SAY_AGGRO                       "Release the hounds!"

#define SOUND_AGGRO                     5841


struct MANGOS_DLL_DECL boss_houndmaster_lokseyAI : public ScriptedAI
{
    boss_houndmaster_lokseyAI(Creature *c) : ScriptedAI(c) {Reset();}

    uint32 Enrage_Timer;
    bool InCombat;

    void Reset()
    {
        Enrage_Timer = 6000000;
        InCombat = true;

        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Say our dialog
            if(!InCombat)
            {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);

                //Cast
                DoCast(m_creature,SPELL_SUMMONSCARLETHOUND);

                InCombat = true;
            }

            DoStartMeleeAttack(who);
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

            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //If we are <10% hp cast healing spells at self and Mograine
        if ( m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 10 && !m_creature->IsNonMeleeSpellCasted(false) && Enrage_Timer < diff)
        {
            DoCast(m_creature,SPELL_ENRAGE);

            //shouldn't cast this agian
            Enrage_Timer = 900000;

        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_houndmaster_loksey(Creature *_Creature)
{
    return new boss_houndmaster_lokseyAI (_Creature);
}


void AddSC_boss_houndmaster_loksey()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_houndmaster_loksey";
    newscript->GetAI = GetAI_boss_houndmaster_loksey;
    m_scripts[nrscripts++] = newscript;
}

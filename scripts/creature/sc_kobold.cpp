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


#include "../sc_defines.h"

struct MANGOS_DLL_DECL KoboldAI : public ScriptedAI
{
    KoboldAI(Creature *c) : ScriptedAI(c) {}
    void AttackStart(Unit *who)
    {
    int hp_max = m_creature->GetMaxHealth();

        if( m_creature->getVictim() == NULL )
        {        
            srand ( time(NULL) );
            int chance = rand()%100;
            if (chance < 30)
            {
                DoSay("You no take candle!",LANG_UNIVERSAL,NULL);
                debug_log("Kobold is saying his text");
            }
            else
            {
                if (chance > 20 && m_creature->GetHealth() < (hp_max/5))
                {
                    DoSay("Me run from you!",LANG_UNIVERSAL,NULL);
                    AttackStop(who);
                    debug_log("Kobold flee");
                }
                else
                {		
                    DoSay("Yip, me kill!",LANG_UNIVERSAL,NULL);
                    debug_log("Kobold is saying his text");
                }
            }
        }
        DoStartMeleeAttack(who);
    }

    void DamageInflict(Unit *who, uint32 amount_damaged)
    {
        srand ( time(NULL) );
        int chance = rand()%100, hp_max = m_creature->GetMaxHealth();

        if (chance <= 20)
        {
            SpellEntry const *spellinfo_attack = m_creature->reachWithSpellAttack(who);
            DoCastSpell(who,spellinfo_attack);

        }
        else if (m_creature->GetHealth() <= (hp_max/2) && chance < 5)
        {
            DoCast(m_creature, 2050);
        }
    }
}; // end of class KoboldAI 
CreatureAI* GetAI_Kobold(Creature *_Creature)
{
    return new KoboldAI (_Creature);
}


void AddSC_kobold()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="kobold";
    newscript->GetAI = GetAI_Kobold;
    m_scripts[nrscripts++] = newscript;
}
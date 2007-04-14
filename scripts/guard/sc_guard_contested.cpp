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
#include "sc_guard_ai.h"

struct MANGOS_DLL_DECL guard_contested : public guardAI
{
    guard_contested(Creature *c) : guardAI(c) {}
    void MoveInLineOfSight(Unit *who)
    {
        if ( who->isAttackingPlayer() )
        {
            if(who->GetTypeId() == TYPEID_PLAYER || who->GetOwnerGUID() && GUID_HIPART(who->GetOwnerGUID())==HIGHGUID_PLAYER)
            {
                m_creature->AddThreat(who, 0.0f);
                if(Unit* owner = who->GetOwner())
                    m_creature->AddThreat(owner, 0.0f);

                if(!m_creature->isInCombat())
                {
                    if (m_creature->GetEntry() == 15184)//Cenarion Hold Infantry
                    {
                        srand (time(NULL));
                        if (rand()%100 <= 30)
                        {
                            DoSay("Taste blade, mongrel!", LANG_UNIVERSAL,NULL);
                        }
                        else if (rand()%100 > 30 && rand()%100 < 50)
                        {
                            DoSay("Please tell me that you didn`t just do what I think you just did. Please tell me that I`m not going to have to hurt you...", LANG_UNIVERSAL,NULL);
                        }
                        else if (rand()%100 >= 50)
                        {
                            DoSay("As if we don`t have enough problems, you go and create more!", LANG_UNIVERSAL,NULL);
                        }
                    }
                    else 
                    {
                        SpellEntry const *spell = m_creature->reachWithSpellAttack(who);
                        DoCastSpell(who, spell);
                    }
                }
                DoStartMeleeAttack(who);
            }
        }
        
    }
}; 
CreatureAI* GetAI_guard_contested(Creature *_Creature)
{
    return new guard_contested (_Creature);
}


void AddSC_guard_contested()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="guard_contested";
    newscript->GetAI = GetAI_guard_contested;
    m_scripts[nrscripts++] = newscript;
}

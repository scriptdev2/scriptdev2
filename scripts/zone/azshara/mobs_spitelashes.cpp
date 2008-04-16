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
SDName: Mob_Spitelashes
SD%Complete: 100
SDComment: For quest 9364
SDCategory: Azshara
EndScriptData */
 
#include "sc_creature.h"
#include "sc_gossip.h"

struct MANGOS_DLL_DECL mobs_spitelashesAI : public ScriptedAI
{
    uint32 morphtimer;
    bool spellhit;
    mobs_spitelashesAI(Creature *c) : ScriptedAI(c) {Reset();}
 
    void Reset()
    {
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //DoGoHome();
        morphtimer=0;
        spellhit=false;
    }
 
    void UpdateAI(const uint32 diff)
    {
        // we mustn't remove the creature in the same round in which we cast the summon spell, otherwise there will be no summons
        if(spellhit && morphtimer>=5000)
        {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                m_creature->RemoveCorpse(); //you don't see any corpse on off.
                EnterEvadeMode();   //spellhit will be set to false
        }
        // walk 5 seconds before summoning
        if(spellhit && morphtimer<5000)
        {
            morphtimer+=diff;
            if(morphtimer>=5000)
            {
                DoCast(m_creature,28406);   //summon copies
                DoCast(m_creature,6924);   //visual explosion
            }
        }
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;
        DoMeleeAttackIfReady();
    }
  
    void Aggro(Unit *who)
    {        
    }
 
    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
    {
        if(!spellhit &&
                Hitter->GetTypeId() == TYPEID_PLAYER &&
                ((Player*)Hitter)->GetQuestStatus(9364) == QUEST_STATUS_INCOMPLETE &&
                (Spellkind->Id==118 || Spellkind->Id== 12824 || Spellkind->Id== 12825 || Spellkind->Id== 12826 || Spellkind->Id== 28271))
        {
            spellhit=true;
            DoCast(m_creature,29124);   //become a sheep
        }
    }
 
 
};
CreatureAI* GetAI_mobs_spitelashes(Creature *_Creature)
{
    return new mobs_spitelashesAI (_Creature);
}
void AddSC_mobs_spitelashes()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mobs_spitelashes";
    newscript->GetAI = GetAI_mobs_spitelashes;
    m_scripts[nrscripts++] = newscript;
}

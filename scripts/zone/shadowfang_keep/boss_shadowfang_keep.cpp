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
SDName: Boss_Shadowfang_Keep
SD%Complete: 30
SDComment: Currently contains only basic scripts for bosses that control opening of doors. TODO: add spells, timers, etc
SDCategory: Shadowfang Keep
EndScriptData */

#include "sc_creature.h"
#include "def_shadowfang_keep.h"

/*#####
# boss_fenrus
#####*/

struct MANGOS_DLL_DECL boss_fenrusAI : public ScriptedAI
{
    boss_fenrusAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(TYPE_FENRUS, NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    { 
        if(pInstance)
            pInstance->SetData(TYPE_FENRUS, DONE);
    }

    void Aggro(Unit* who) {}

};
CreatureAI* GetAI_boss_fenrusAI(Creature *_Creature)
{
    return new boss_fenrusAI (_Creature);
}

/*#####
# boss_nandos
#####*/

struct MANGOS_DLL_DECL boss_nandosAI : public ScriptedAI
{
    boss_nandosAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance)
            pInstance->SetData(TYPE_NANDOS, NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    { 
        if(pInstance)
            pInstance->SetData(TYPE_NANDOS, DONE);
    }

    void Aggro(Unit* who) {}

};
CreatureAI* GetAI_boss_nandosAI(Creature *_Creature)
{
    return new boss_nandosAI (_Creature);
}

/*#####
# boss_rethilgore
#####*/

struct MANGOS_DLL_DECL boss_rethilgoreAI : public ScriptedAI
{
    boss_rethilgoreAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance)
        {
            pInstance->SetData(TYPE_RETHILGORE, NOT_STARTED);
            pInstance->SetData(TYPE_FREE_NPC, NOT_STARTED);
        }
    }

    void JustDied(Unit* Killer)
    { 
        if(pInstance)
            pInstance->SetData(TYPE_RETHILGORE, DONE);
    }

    void Aggro(Unit* who) {}

};
CreatureAI* GetAI_boss_rethilgoreAI(Creature *_Creature)
{
    return new boss_rethilgoreAI (_Creature);
}

/*#####
# 
#####*/

void AddSC_boss_shadowfang_keep()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_fenrus";
    newscript->GetAI = GetAI_boss_fenrusAI;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_nandos";
    newscript->GetAI = GetAI_boss_nandosAI;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_rethilgore";
    newscript->GetAI = GetAI_boss_rethilgoreAI;
    m_scripts[nrscripts++] = newscript;
}

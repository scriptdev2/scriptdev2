/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Guards
SD%Complete: 100
SDComment: CombatAI should be organized better for future.
SDCategory: Guards
EndScriptData */

/* ContentData
guard_azuremyst
guard_bluffwatcher
guard_contested
guard_darnassus
guard_dunmorogh
guard_durotar
guard_elwynnforest
guard_eversong
guard_exodar
guard_ironforge
guard_mulgore
guard_orgrimmar
guard_shattrath
guard_shattrath_aldor
guard_shattrath_scryer
guard_silvermoon
guard_stormwind
guard_teldrassil
guard_tirisfal
guard_undercity
EndContentData */

#include "precompiled.h"
#include "guard_ai.h"

CreatureAI* GetAI_guard_azuremyst(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_bluffwatcher(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_contested(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_darnassus(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_dunmorogh(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_durotar(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_elwynnforest(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_eversong(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_exodar(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_ironforge(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_mulgore(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_orgrimmar(Creature* pCreature)
{
    return new guardAI_orgrimmar(pCreature);
}

CreatureAI* GetAI_guard_shattrath(Creature* pCreature)
{
    return new guardAI(pCreature);
}

/*******************************************************
 * guard_shattrath_aldor
 *******************************************************/

#define SPELL_BANISHED_SHATTRATH_A  36642
#define SPELL_BANISHED_SHATTRATH_S  36671
#define SPELL_BANISH_TELEPORT       36643
#define SPELL_EXILE                 39533

struct MANGOS_DLL_DECL guard_shattrath_aldorAI : public guardAI
{
    guard_shattrath_aldorAI(Creature* pCreature) : guardAI(pCreature) { Reset(); }

    uint32 Exile_Timer;
    uint32 Banish_Timer;
    uint64 playerGUID;
    bool CanTeleport;

    void Reset()
    {
        Banish_Timer = 5000;
        Exile_Timer = 8500;
        playerGUID = 0;
        CanTeleport = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CanTeleport)
        {
            if (Exile_Timer < diff)
            {
                if (Unit* temp = Unit::GetUnit(*m_creature,playerGUID))
                {
                    temp->CastSpell(temp,SPELL_EXILE,true);
                    temp->CastSpell(temp,SPELL_BANISH_TELEPORT,true);
                }
                playerGUID = 0;
                Exile_Timer = 8500;
                CanTeleport = false;
            }else Exile_Timer -= diff;
        }
        else if (Banish_Timer < diff)
        {
            Unit* temp = m_creature->getVictim();
            if (temp && temp->GetTypeId() == TYPEID_PLAYER)
            {
                DoCastSpellIfCan(temp,SPELL_BANISHED_SHATTRATH_A);
                Banish_Timer = 9000;
                playerGUID = temp->GetGUID();
                if (playerGUID)
                    CanTeleport = true;
            }
        }else Banish_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_guard_shattrath_aldor(Creature* pCreature)
{
    return new guard_shattrath_aldorAI(pCreature);
}

/*******************************************************
 * guard_shattrath_scryer
 *******************************************************/

struct MANGOS_DLL_DECL guard_shattrath_scryerAI : public guardAI
{
    guard_shattrath_scryerAI(Creature* pCreature) : guardAI(pCreature) { Reset(); }

    uint32 Exile_Timer;
    uint32 Banish_Timer;
    uint64 playerGUID;
    bool CanTeleport;

    void Reset()
    {
        Banish_Timer = 5000;
        Exile_Timer = 8500;
        playerGUID = 0;
        CanTeleport = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CanTeleport)
        {
            if (Exile_Timer < diff)
            {
                if (Unit* temp = Unit::GetUnit(*m_creature,playerGUID))
                {
                    temp->CastSpell(temp,SPELL_EXILE,true);
                    temp->CastSpell(temp,SPELL_BANISH_TELEPORT,true);
                }
                playerGUID = 0;
                Exile_Timer = 8500;
                CanTeleport = false;
            }else Exile_Timer -= diff;
        }
        else if (Banish_Timer < diff)
        {
            Unit* temp = m_creature->getVictim();
            if (temp && temp->GetTypeId() == TYPEID_PLAYER)
            {
                DoCastSpellIfCan(temp,SPELL_BANISHED_SHATTRATH_S);
                Banish_Timer = 9000;
                playerGUID = temp->GetGUID();
                if (playerGUID)
                    CanTeleport = true;
            }
        }else Banish_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_guard_shattrath_scryer(Creature* pCreature)
{
    return new guard_shattrath_scryerAI(pCreature);
}

CreatureAI* GetAI_guard_silvermoon(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_stormwind(Creature* pCreature)
{
    return new guardAI_stormwind(pCreature);
}

CreatureAI* GetAI_guard_teldrassil(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_tirisfal(Creature* pCreature)
{
    return new guardAI(pCreature);
}

CreatureAI* GetAI_guard_undercity(Creature* pCreature)
{
    return new guardAI(pCreature);
}

void AddSC_guards()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "guard_azuremyst";
    newscript->GetAI = &GetAI_guard_azuremyst;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_bluffwatcher";
    newscript->GetAI = &GetAI_guard_bluffwatcher;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_contested";
    newscript->GetAI = &GetAI_guard_contested;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_darnassus";
    newscript->GetAI = &GetAI_guard_darnassus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_dunmorogh";
    newscript->GetAI = &GetAI_guard_dunmorogh;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_durotar";
    newscript->GetAI = &GetAI_guard_durotar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_elwynnforest";
    newscript->GetAI = &GetAI_guard_elwynnforest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_eversong";
    newscript->GetAI = &GetAI_guard_eversong;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_exodar";
    newscript->GetAI = &GetAI_guard_exodar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_ironforge";
    newscript->GetAI = &GetAI_guard_ironforge;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_mulgore";
    newscript->GetAI = &GetAI_guard_mulgore;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_orgrimmar";
    newscript->GetAI = &GetAI_guard_orgrimmar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_shattrath";
    newscript->GetAI = &GetAI_guard_shattrath;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_shattrath_aldor";
    newscript->GetAI = &GetAI_guard_shattrath_aldor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_shattrath_scryer";
    newscript->GetAI = &GetAI_guard_shattrath_scryer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_silvermoon";
    newscript->GetAI = &GetAI_guard_silvermoon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_stormwind";
    newscript->GetAI = &GetAI_guard_stormwind;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_teldrassil";
    newscript->GetAI = &GetAI_guard_teldrassil;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_tirisfal";
    newscript->GetAI = &GetAI_guard_tirisfal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "guard_undercity";
    newscript->GetAI = &GetAI_guard_undercity;
    newscript->RegisterSelf();
}

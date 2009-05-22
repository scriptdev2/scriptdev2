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
SDName: Instance_Magtheridons_Lair
SD%Complete: 100
SDComment:
SDCategory: Hellfire Citadel, Magtheridon's lair
EndScriptData */

#include "precompiled.h"
#include "def_magtheridons_lair.h"

#define SPELL_SOUL_TRANSFER         30531                   // core, does not support effectImplicitTarget 7
#define SPELL_BLAZE_TARGET          30541                   // core, does not support effectImplicitTarget 7
#define SPELL_DEBRIS_DAMAGE         30631                   // core, does not fully support effectImplicitTarget 8
#define SPELL_DEBRIS_KNOCKDOWN      36449                   // core, does not fully support effectImplicitTarget 8

#define CHAMBER_CENTER_X            -15.14
#define CHAMBER_CENTER_Y              1.8
#define CHAMBER_CENTER_Z             -0.4

#define ENCOUNTERS                  2

struct MANGOS_DLL_DECL instance_magtheridons_lair : public ScriptedInstance
{
    instance_magtheridons_lair(Map *map) : ScriptedInstance(map)
    {
         Initialize();

         // below here are pure spell hacks, feel free to experiment yourself
         // effectImplicitTarget 7, random target with certain entry spell, need implemntation in mangos

         /*
         SpellEntry *TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_BLAZE_TARGET);
         if (TempSpell && TempSpell->EffectImplicitTargetA[0] != 6)
         {
               TempSpell->EffectImplicitTargetA[0] = 6;
               TempSpell->EffectImplicitTargetB[0] = 0;
         }

         TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_SOUL_TRANSFER);
         if (TempSpell && TempSpell->EffectImplicitTargetB[0] != 30)
         {
               TempSpell->EffectImplicitTargetA[0] = 1;
               TempSpell->EffectImplicitTargetA[1] = 1;
               TempSpell->EffectImplicitTargetA[2] = 1;
               TempSpell->EffectImplicitTargetB[0] = 0;
               TempSpell->EffectImplicitTargetB[1] = 0;
               TempSpell->EffectImplicitTargetB[2] = 0;
         }

         // effectImplicitTarget 8, but core only push back the caster

         TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_DEBRIS_DAMAGE);
         if (TempSpell && TempSpell->EffectImplicitTargetA[0] != 53)
         {
               TempSpell->EffectImplicitTargetA[0] = 53;
               TempSpell->EffectImplicitTargetB[0] = 16;
         }

         TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_DEBRIS_KNOCKDOWN);
         if (TempSpell && TempSpell->EffectImplicitTargetA[0] != 53)
         {
               TempSpell->EffectImplicitTargetA[0] = 53;
               TempSpell->EffectImplicitTargetB[0] = 16;
               TempSpell->EffectImplicitTargetA[1] = 53;
               TempSpell->EffectImplicitTargetB[1] = 16;
         }
         */
    }

    uint32 Encounters[ENCOUNTERS];

    uint64 MagtheridonGUID;
    std::set<uint64> ChannelerGUID;
    uint64 DoorGUID;
    std::set<uint64> ColumnGUID;

    uint32 CageTimer;
    uint32 RespawnTimer;

    void Initialize()
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            Encounters[i] = NOT_STARTED;

        MagtheridonGUID = 0;
        ChannelerGUID.clear();
        DoorGUID = 0;
        ColumnGUID.clear();
        CageTimer = 0;
        RespawnTimer = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; i++)
            if (Encounters[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 17257: MagtheridonGUID = pCreature->GetGUID(); break;
            case 17256: ChannelerGUID.insert(pCreature->GetGUID()); break;
        }
    }

    void OnObjectCreate(GameObject *go)
    {
        switch(go->GetEntry())
        {
            case 181713:
                go->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
                break;
            case 183847:                                    //event door
                DoorGUID = go->GetGUID();
                break;
            case 184653:                                    // hall
            case 184634:                                    // six columns
            case 184635:
            case 184636:
            case 184637:
            case 184638:
            case 184639:
                ColumnGUID.insert(go->GetGUID());
                break;
        }
    }

    uint64 GetData64(uint32 type)
    {
        switch(type)
        {
            case DATA_MAGTHERIDON:
                return MagtheridonGUID;
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_MAGTHERIDON_EVENT:
                Encounters[0] = data;
                if (data == NOT_STARTED)
                    RespawnTimer = 10000;
                if (data != IN_PROGRESS)
                {
                    if (GameObject* pDoor = instance->GetGameObject(DoorGUID))
                        pDoor->SetGoState(GO_STATE_ACTIVE);
                }
                break;
            case TYPE_CHANNELER_EVENT:
                switch(data)
                {
                    case NOT_STARTED:                       // Reset all channelers once one is reset.
                        if (Encounters[1] != NOT_STARTED)
                        {
                            Encounters[1] = NOT_STARTED;

                            if (ChannelerGUID.empty())
                                debug_log("SD2: Instance Magtheridon: Channeler GUID list are empty.");

                            for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                            {
                                if (Creature* pChanneler = instance->GetCreature(*i))
                                {
                                    if (pChanneler->isAlive())
                                        pChanneler->AI()->EnterEvadeMode();
                                    else
                                        pChanneler->Respawn();
                                }
                            }

                            CageTimer = 0;

                            if (GameObject* pDoor = instance->GetGameObject(DoorGUID))
                                pDoor->SetGoState(GO_STATE_ACTIVE);
                        }
                        break;
                    case IN_PROGRESS:                       // Event start.
                        if (Encounters[1] != IN_PROGRESS)
                        {
                            Encounters[1] = IN_PROGRESS;

                            // Let all five channelers aggro.
                            for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                            {
                                Creature* pChanneler = instance->GetCreature(*i);

                                if (pChanneler && pChanneler->isAlive())
                                    AttackNearestTarget(pChanneler);
                            }

                            // Magtheridon breaks free after two minutes.
                            Creature* pMagtheridon = instance->GetCreature(MagtheridonGUID);

                            if (pMagtheridon && pMagtheridon->isAlive())
                                CageTimer = 120000;

                            if (GameObject* pDoor = instance->GetGameObject(DoorGUID))
                                pDoor->SetGoState(GO_STATE_READY);
                        }
                        break;
                    case DONE:                              // Add buff and check if all channelers are dead.
                        for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                        {
                            Creature* pChanneler = instance->GetCreature(*i);

                            if (pChanneler && pChanneler->isAlive())
                            {
                                //Channeler->InterruptNonMeleeSpells(false);
                                //Channeler->CastSpell(Channeler, SPELL_SOUL_TRANSFER, false);
                                data = IN_PROGRESS;
                                break;
                            }
                        }
                        break;
                }
                Encounters[1] = data;
                break;
            case TYPE_HALL_COLLAPSE:
                // IN_PROGRESS - collapse / NOT_STARTED - reset
                for(std::set<uint64>::iterator i = ColumnGUID.begin(); i != ColumnGUID.end(); ++i)
                {
                    DoUseDoorOrButton(*i);
                }
                break;
        }
    }

    uint32 GetData(uint32 type)
    {
        if (type == TYPE_MAGTHERIDON_EVENT)
            return Encounters[0];
        if (type == TYPE_CHANNELER_EVENT)
            return Encounters[1];

        return 0;
    }

    void AttackNearestTarget(Creature* pCreature)
    {
        float minRange = VISIBLE_RANGE;
        float range;
        Player* target = NULL;

        Map::PlayerList const& players = instance->GetPlayers();
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* i_pl = itr->getSource())
            {
                if (i_pl->isTargetableForAttack())
                {
                    range = i_pl->GetDistance(pCreature);
                    if (range < minRange)
                    {
                        minRange = range;
                        target = i_pl;
                    }
                }
            }
        }

        if (!target)
        {
            debug_log("SD2: Instance Magtheridon: AttackNearestTarget failed. No player.");
            return;
        }
        pCreature->AI()->AttackStart(target);
    }

    void Update(uint32 diff)
    {
        if (CageTimer)
        {
            if (CageTimer <= diff)
            {
                SetData(TYPE_MAGTHERIDON_EVENT, IN_PROGRESS);
                CageTimer = 0;
            }else CageTimer -= diff;
        }

        if (RespawnTimer)
        {
            if (RespawnTimer <= diff)
            {
                for(std::set<uint64>::iterator i = ChannelerGUID.begin(); i != ChannelerGUID.end(); ++i)
                {
                    if (Creature* pChanneler = instance->GetCreature(*i))
                    {
                        if (pChanneler->isAlive())
                            pChanneler->AI()->EnterEvadeMode();
                        else
                            pChanneler->Respawn();
                    }
                }

                RespawnTimer = 0;
            }else RespawnTimer -= diff;
        }
    }
};

InstanceData* GetInstanceData_instance_magtheridons_lair(Map* map)
{
    return new instance_magtheridons_lair(map);
}

void AddSC_instance_magtheridons_lair()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_magtheridons_lair";
    newscript->GetInstanceData = &GetInstanceData_instance_magtheridons_lair;
    newscript->RegisterSelf();
}

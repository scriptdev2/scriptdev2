/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/* ScriptData
SDName: Instance_Sunwell_Plateau
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Sunwell_Plateau
EndScriptData */

#include "precompiled.h"
#include "def_sunwell_plateau.h"

/*
UPDATE `creature_template` SET `ScriptName`='boss_brutallus' WHERE `entry`=24882;
UPDATE `creature_template` SET `ScriptName`='boss_kalecgos' WHERE `entry`=24850;
UPDATE `creature_template` SET `ScriptName`='boss_kalecgos_humanoid' WHERE `entry`=24891;
UPDATE `creature_template` SET `ScriptName`='boss_sathrovarr' WHERE `entry`=24892;
UPDATE `gameobject_template` SET `ScriptName`='go_spectral_rift' WHERE `entry`=187055;
UPDATE `instance_template` SET `script`='instance_sunwell_plateau' WHERE `map`=580;
*/

#define ENCOUNTERS 6

#define SPELL_SPECTRAL_REALM        46021
#define SPELL_TELEPORT_NORMAL_REALM 46020
#define SPELL_SPECTRAL_EXHAUSTION   44867

/* Sunwell Plateau:
0 - Kalecgos and Sathrovarr
1 - Brutallus
2 - Felmyst
3 - Eredar Twins (Alythess and Sacrolash)
4 - M'uru
5 - Kil'Jaeden
*/

struct MANGOS_DLL_DECL instance_sunwell_plateau : public ScriptedInstance
{
    instance_sunwell_plateau(Map *map) : ScriptedInstance(map) {Initialize();};

    uint32 Encounters[ENCOUNTERS];

    /** Creatures **/
    uint64 Kalecgos_Dragon;
    uint64 Kalecgos_Human;
    uint64 Sathrovarr;
    uint64 Brutallus;
    uint64 Felmyst;
    uint64 Alythess;
    uint64 Sacrolash;
    uint64 Muru;
    uint64 KilJaeden;
    uint64 KilJaedenController;
    uint64 Anveena;

    /** GameObjects **/
    uint64 ForceField;                                      // Kalecgos Encounter
    uint64 FireBarrier;                                     // Brutallus Encounter
    uint64 Gate[5];                                         // Rename this to be more specific after door placement is verified.

    /*** Misc ***/
    uint32 SpectralRealmTimer;
    std::vector<uint64> SpectralRealmList;

    void Initialize()
    {
        /*** Creatures ***/
        Kalecgos_Dragon         = 0;
        Kalecgos_Human          = 0;
        Sathrovarr              = 0;
        Brutallus               = 0;
        Felmyst                 = 0;
        Alythess                = 0;
        Sacrolash               = 0;
        Muru                    = 0;
        KilJaeden               = 0;
        KilJaedenController     = 0;
        Anveena                 = 0;

        /*** GameObjects ***/
        ForceField  = 0;
        FireBarrier = 0;
        Gate[0]     = 0;                                    // TODO: Rename Gate[n] with gate_<boss name> for better specificity
        Gate[1]     = 0;
        Gate[2]     = 0;
        Gate[3]     = 0;
        Gate[4]     = 0;

        /*** Encounters ***/
        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            Encounters[i] = NOT_STARTED;

        /*** Misc ***/
        SpectralRealmTimer = 5000;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if (Encounters[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 24850: Kalecgos_Dragon     = pCreature->GetGUID(); break;
            case 24891: Kalecgos_Human      = pCreature->GetGUID(); break;
            case 24892: Sathrovarr          = pCreature->GetGUID(); break;
            case 24882: Brutallus           = pCreature->GetGUID(); break;
            case 25038: Felmyst             = pCreature->GetGUID(); break;
            case 25166: Alythess            = pCreature->GetGUID(); break;
            case 25165: Sacrolash           = pCreature->GetGUID(); break;
            case 25741: Muru                = pCreature->GetGUID(); break;
            case 25315: KilJaeden           = pCreature->GetGUID(); break;
            case 25608: KilJaedenController = pCreature->GetGUID(); break;
            case 26046: Anveena             = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* gobj)
    {
        switch(gobj->GetEntry())
        {
            case 188421: ForceField     = gobj->GetGUID(); break;
            case 188075: FireBarrier    = gobj->GetGUID(); break;
            case 187979: Gate[0]        = gobj->GetGUID(); break;
            case 187770: Gate[1]        = gobj->GetGUID(); break;
            case 187896: Gate[2]        = gobj->GetGUID(); break;
            case 187990: Gate[3]        = gobj->GetGUID(); break;
            case 188118: Gate[4]        = gobj->GetGUID(); break;
        }
    }

    uint32 GetData(uint32 id)
    {
        switch(id)
        {
            case DATA_KALECGOS_EVENT:     return Encounters[0]; break;
            case DATA_BRUTALLUS_EVENT:    return Encounters[1]; break;
            case DATA_FELMYST_EVENT:      return Encounters[2]; break;
            case DATA_EREDAR_TWINS_EVENT: return Encounters[3]; break;
            case DATA_MURU_EVENT:         return Encounters[4]; break;
            case DATA_KILJAEDEN_EVENT:    return Encounters[5]; break;
        }

        return 0;
    }

    uint64 GetData64(uint32 id)
    {
        switch(id)
        {
            case DATA_KALECGOS_DRAGON:      return Kalecgos_Dragon;     break;
            case DATA_KALECGOS_HUMAN:       return Kalecgos_Human;      break;
            case DATA_SATHROVARR:           return Sathrovarr;          break;
            case DATA_BRUTALLUS:            return Brutallus;           break;
            case DATA_FELMYST:              return Felmyst;             break;
            case DATA_ALYTHESS:             return Alythess;            break;
            case DATA_SACROLASH:            return Sacrolash;           break;
            case DATA_MURU:                 return Muru;                break;
            case DATA_KILJAEDEN:            return KilJaeden;           break;
            case DATA_KILJAEDEN_CONTROLLER: return KilJaedenController; break;
            case DATA_ANVEENA:              return Anveena;             break;

            case DATA_RANDOM_SPECTRAL_PLAYER:
                return *(SpectralRealmList.begin() + rand()%SpectralRealmList.size());
                break;
        }
        return 0;
    }

    void SetData(uint32 id, uint32 data)
    {
        switch(id)
        {
            case DATA_KALECGOS_EVENT:      Encounters[0] = data; break;
            case DATA_BRUTALLUS_EVENT:     Encounters[1] = data; break;
            case DATA_FELMYST_EVENT:       Encounters[2] = data; break;
            case DATA_EREDAR_TWINS_EVENT:  Encounters[3] = data; break;
            case DATA_MURU_EVENT:          Encounters[4] = data; break;
            case DATA_KILJAEDEN_EVENT:     Encounters[5] = data; break;

            case DATA_SET_SPECTRAL_CHECK:  SpectralRealmTimer = data; break;
            case DATA_INST_EJECT_PLAYERS:  EjectPlayers(); break;
        }
    }

    void SetData64(uint32 id, uint64 guid)
    {
        switch(id)
        {
            case DATA_PLAYER_SPECTRAL_REALM:
                SpectralRealmList.push_back(guid);
                break;
        }
    }

    void EjectPlayer(Player* plr)
    {
        debug_log("SD2: INST: Ejecting Player %s from Spectral Realm", plr->GetName());
        // Remove player from Sathrovarr's threat list
        Creature* Sath = ((Creature*)Unit::GetUnit(*plr, Sathrovarr));
        if (Sath && Sath->isAlive())
        {
            HostilReference* ref = Sath->getThreatManager().getOnlineContainer().getReferenceByTarget(plr);
            if (ref)
            {
                ref->removeReference();
                debug_log("SD2: INST: Deleting %s from Sathrovarr's threatlist", plr->GetName());
            }
        }

        // Put player back in Kalecgos(Dragon)'s threat list
        Creature* Kalecgos = ((Creature*)Unit::GetUnit(*plr, Kalecgos_Dragon));
        if (Kalecgos && Kalecgos->isAlive())
        {
            debug_log("SD2: INST: Putting %s in Kalecgos' threatlist", plr->GetName());
            Kalecgos->AddThreat(plr, 1.0f);
        }

        plr->CastSpell(plr, SPELL_TELEPORT_NORMAL_REALM, true);
        plr->CastSpell(plr, SPELL_SPECTRAL_EXHAUSTION, true);
    }

    void EjectPlayers()
    {
        if (SpectralRealmList.empty())
            return;

        Map::PlayerList const& players = instance->GetPlayers();
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* plr = itr->getSource();
            if (!plr)
                continue;

            if (std::find(SpectralRealmList.begin(),SpectralRealmList.end(),plr->GetGUID())!=SpectralRealmList.end() &&
                !plr->HasAura(SPELL_SPECTRAL_REALM, 0))
            {
                EjectPlayer(plr);
            }
        }

        SpectralRealmList.clear();
    }

    void Update(uint32 diff)
    {
        // Only check for Spectral Realm if Kalecgos Encounter is running
        if (Encounters[0] == IN_PROGRESS)
            if (SpectralRealmTimer < diff)
        {
            EjectPlayers();
            SpectralRealmTimer = 5000;
        }else SpectralRealmTimer -= diff;
    }
};

InstanceData* GetInstanceData_instance_sunwell_plateau(Map* map)
{
    return new instance_sunwell_plateau(map);
}

void AddSC_instance_sunwell_plateau()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_sunwell_plateau";
    newscript->GetInstanceData = &GetInstanceData_instance_sunwell_plateau;
    newscript->RegisterSelf();
}

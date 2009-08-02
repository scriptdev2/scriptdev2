/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"
#include "Config/Config.h"
#include "config.h"
#include "Database/DatabaseEnv.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "ProgressBar.h"
#include "../system/ScriptLoader.h"

int num_sc_scripts;
Script *m_scripts[MAX_SCRIPTS];

DatabaseType SD2Database;
Config SD2Config;

std::string strVersion;

// String text additional data, used in TextMap
struct StringTextData
{
    uint32 SoundId;
    uint8  Type;
    uint32 Language;
    uint32 Emote;
};

#define TEXT_SOURCE_RANGE   -1000000                        //the amount of entries each text source has available

// Text Maps
UNORDERED_MAP<int32, StringTextData> TextMap;

// Waypoint map (escorts)
UNORDERED_MAP<uint32, std::vector<PointMovement> > PointMovementMap;

void FillSpellSummary();

void LoadDatabase()
{
    //Get db string from file
    char const* sd2dbstring = NULL;

    if (!SD2Config.GetString("ScriptDev2DatabaseInfo", &sd2dbstring))
    {
        error_log("SD2: Missing Scriptdev2 database info from configuration file. Load database aborted.");
        return;
    }

    //Initialize connection to DB
    if (sd2dbstring && SD2Database.Initialize(sd2dbstring))
        outstring_log("SD2: ScriptDev2 database: %s",sd2dbstring);
    else
    {
        error_log("SD2: Unable to connect to Database. Load database aborted.");
        return;
    }

    //***Preform all DB queries here***
    QueryResult *result;

    //Get Version information
    result = SD2Database.PQuery("SELECT version FROM sd2_db_version LIMIT 1");

    if (result)
    {
        Field *fields = result->Fetch();

        strVersion = fields[0].GetCppString();
        strVersion.append(_FULLVERSION);

        outstring_log("Loading %s", strVersion.c_str());
        outstring_log("");
        delete result;

    }
    else
    {
        error_log("SD2: Missing `sd2_db_version` information.");
        outstring_log("");
    }

    // Load Script Text
    outstring_log("SD2: Loading Script Texts...");
    LoadMangosStrings(SD2Database,"script_texts",TEXT_SOURCE_RANGE,1+(TEXT_SOURCE_RANGE*2));

    // Gather Additional data from Script Texts
    result = SD2Database.PQuery("SELECT entry, sound, type, language, emote FROM script_texts");

    outstring_log("SD2: Loading Script Texts additional data...");
    if (result)
    {
        barGoLink bar(result->GetRowCount());
        uint32 count = 0;

        do
        {
            bar.step();
            Field* fields = result->Fetch();
            StringTextData temp;

            int32 i             = fields[0].GetInt32();
            temp.SoundId        = fields[1].GetInt32();
            temp.Type           = fields[2].GetInt32();
            temp.Language       = fields[3].GetInt32();
            temp.Emote          = fields[4].GetInt32();

            if (i >= 0)
            {
                error_db_log("SD2: Entry %i in table `script_texts` is not a negative value.",i);
                continue;
            }

            if (i > TEXT_SOURCE_RANGE || i <= TEXT_SOURCE_RANGE*2)
            {
                error_db_log("SD2: Entry %i in table `script_texts` is out of accepted entry range for table.",i);
                continue;
            }

            if (temp.SoundId)
            {
                if (!GetSoundEntriesStore()->LookupEntry(temp.SoundId))
                    error_db_log("SD2: Entry %i in table `script_texts` has soundId %u but sound does not exist.",i,temp.SoundId);
            }

            if (!GetLanguageDescByID(temp.Language))
                error_db_log("SD2: Entry %i in table `script_texts` using Language %u but Language does not exist.",i,temp.Language);

            if (temp.Type > CHAT_TYPE_ZONE_YELL)
                error_db_log("SD2: Entry %i in table `script_texts` has Type %u but this Chat Type does not exist.",i,temp.Type);

            TextMap[i] = temp;
            ++count;
        } while (result->NextRow());

        delete result;

        outstring_log("");
        outstring_log(">> Loaded %u additional Script Texts data.", count);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 additional Script Texts data. DB table `script_texts` is empty.");
    }

    // Load Custom Text
    outstring_log("SD2: Loading Custom Texts...");
    LoadMangosStrings(SD2Database,"custom_texts",TEXT_SOURCE_RANGE*2,1+(TEXT_SOURCE_RANGE*3));

    // Gather Additional data from Custom Texts
    result = SD2Database.PQuery("SELECT entry, sound, type, language, emote FROM custom_texts");

    outstring_log("SD2: Loading Custom Texts additional data...");
    if (result)
    {
        barGoLink bar(result->GetRowCount());
        uint32 count = 0;

        do
        {
            bar.step();
            Field* fields = result->Fetch();
            StringTextData temp;

            int32 i             = fields[0].GetInt32();
            temp.SoundId        = fields[1].GetInt32();
            temp.Type           = fields[2].GetInt32();
            temp.Language       = fields[3].GetInt32();
            temp.Emote          = fields[4].GetInt32();

            if (i >= 0)
            {
                error_db_log("SD2: Entry %i in table `custom_texts` is not a negative value.",i);
                continue;
            }

            if (i > TEXT_SOURCE_RANGE*2 || i <= TEXT_SOURCE_RANGE*3)
            {
                error_db_log("SD2: Entry %i in table `custom_texts` is out of accepted entry range for table.",i);
                continue;
            }

            if (temp.SoundId)
            {
                if (!GetSoundEntriesStore()->LookupEntry(temp.SoundId))
                    error_db_log("SD2: Entry %i in table `custom_texts` has soundId %u but sound does not exist.",i,temp.SoundId);
            }

            if (!GetLanguageDescByID(temp.Language))
                error_db_log("SD2: Entry %i in table `custom_texts` using Language %u but Language does not exist.",i,temp.Language);

            if (temp.Type > CHAT_TYPE_ZONE_YELL)
                error_db_log("SD2: Entry %i in table `custom_texts` has Type %u but this Chat Type does not exist.",i,temp.Type);

            TextMap[i] = temp;
            ++count;
        } while (result->NextRow());

        delete result;

        outstring_log("");
        outstring_log(">> Loaded %u additional Custom Texts data.", count);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 additional Custom Texts data. DB table `custom_texts` is empty.");
    }

    // Drop Existing Waypoint list
    PointMovementMap.clear();
    uint64 uiCreatureCount = 0;

    // Load Waypoints
    result = SD2Database.PQuery("SELECT COUNT(entry) FROM script_waypoint GROUP BY entry");
    if (result)
    {
        uiCreatureCount = result->GetRowCount();
        delete result;
    }

    outstring_log("SD2: Loading Script Waypoints for %u creature(s)...", uiCreatureCount);

    result = SD2Database.PQuery("SELECT entry, pointid, location_x, location_y, location_z, waittime FROM script_waypoint ORDER BY pointid");

    if (result)
    {
        barGoLink bar(result->GetRowCount());
        uint32 uiNodeCount = 0;

        do
        {
            bar.step();
            Field* pFields = result->Fetch();
            PointMovement pTemp;

            pTemp.m_uiCreatureEntry  = pFields[0].GetUInt32();
            uint32 uiCreatureEntry   = pTemp.m_uiCreatureEntry;
            pTemp.m_uiPointId        = pFields[1].GetUInt32();
            pTemp.m_fX               = pFields[2].GetFloat();
            pTemp.m_fY               = pFields[3].GetFloat();
            pTemp.m_fZ               = pFields[4].GetFloat();
            pTemp.m_uiWaitTime       = pFields[5].GetUInt32();

            CreatureInfo const* pCInfo = GetCreatureTemplateStore(pTemp.m_uiCreatureEntry);
            if (!pCInfo)
            {
                error_db_log("SD2: DB table script_waypoint has waypoint for non-existant creature entry %u", pTemp.m_uiCreatureEntry);
                continue;
            }

            if (!pCInfo->ScriptID)
                error_db_log("SD2: DB table script_waypoint has waypoint for creature entry %u, but creature does not have ScriptName defined and then useless.", pTemp.m_uiCreatureEntry);

            PointMovementMap[uiCreatureEntry].push_back(pTemp);
            ++uiNodeCount;
        } while (result->NextRow());

        delete result;

        outstring_log("");
        outstring_log(">> Loaded %u Script Waypoint nodes.", uiNodeCount);
    }
    else
    {
        barGoLink bar(1);
        bar.step();
        outstring_log("");
        outstring_log(">> Loaded 0 Script Waypoints. DB table `script_waypoint` is empty.");
    }

    //Free database thread and resources
    SD2Database.HaltDelayThread();

}

struct TSpellSummary {
    uint8 Targets;                                          // set of enum SelectTarget
    uint8 Effects;                                          // set of enum SelectEffect
}extern *SpellSummary;

MANGOS_DLL_EXPORT
void ScriptsFree()
{
    // Free Spell Summary
    delete []SpellSummary;

    // Free resources before library unload
    for(int i=0; i<MAX_SCRIPTS; ++i)
        delete m_scripts[i];

    num_sc_scripts = 0;
}

MANGOS_DLL_EXPORT
void ScriptsInit()
{
    bool CanLoadDB = true;

    //ScriptDev2 startup
    outstring_log("");
    outstring_log(" MMM  MMM    MM");
    outstring_log("M  MM M  M  M  M");
    outstring_log("MM    M   M   M");
    outstring_log(" MMM  M   M  M");
    outstring_log("   MM M   M MMMM");
    outstring_log("MM  M M  M ");
    outstring_log(" MMM  MMM  http://www.scriptdev2.com");
    outstring_log("");

    //Get configuration file
    if (!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
    {
        CanLoadDB = false;
        error_log("SD2: Unable to open configuration file. Database will be unaccessible. Configuration values will use default.");
    }
    else
        outstring_log("SD2: Using configuration file %s",_SCRIPTDEV2_CONFIG);

    //Check config file version
    if (SD2Config.GetIntDefault("ConfVersion", 0) != SD2_CONF_VERSION)
        error_log("SD2: Configuration file version doesn't match expected version. Some config variables may be wrong or missing.");

    outstring_log("");

    //Load database (must be called after SD2Config.SetSource). In case it failed, no need to even try load.
    if (CanLoadDB)
        LoadDatabase();

    outstring_log("SD2: Loading C++ scripts");
    barGoLink bar(1);
    bar.step();
    outstring_log("");

    for(int i=0; i<MAX_SCRIPTS; ++i)
        m_scripts[i]=NULL;

    FillSpellSummary();

    AddScripts();

    outstring_log(">> Loaded %i C++ Scripts.", num_sc_scripts);
}

//*********************************
//*** Functions used globally ***

void DoScriptText(int32 textEntry, WorldObject* pSource, Unit* target)
{
    if (!pSource)
    {
        error_log("SD2: DoScriptText entry %i, invalid Source pointer.",textEntry);
        return;
    }

    if (textEntry >= 0)
    {
        error_log("SD2: DoScriptText with source entry %u (TypeId=%u, guid=%u) attempts to process text entry %i, but text entry must be negative.",pSource->GetEntry(),pSource->GetTypeId(),pSource->GetGUIDLow(),textEntry);
        return;
    }

    UNORDERED_MAP<int32, StringTextData>::iterator i = TextMap.find(textEntry);

    if (i == TextMap.end())
    {
        error_log("SD2: DoScriptText with source entry %u (TypeId=%u, guid=%u) could not find text entry %i.",pSource->GetEntry(),pSource->GetTypeId(),pSource->GetGUIDLow(),textEntry);
        return;
    }

    debug_log("SD2: DoScriptText: text entry=%i, Sound=%u, Type=%u, Language=%u, Emote=%u",textEntry,(*i).second.SoundId,(*i).second.Type,(*i).second.Language,(*i).second.Emote);

    if ((*i).second.SoundId)
    {
        if (GetSoundEntriesStore()->LookupEntry((*i).second.SoundId))
            pSource->PlayDirectSound((*i).second.SoundId);
        else
            error_log("SD2: DoScriptText entry %i tried to process invalid sound id %u.",textEntry,(*i).second.SoundId);
    }

    if ((*i).second.Emote)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT || pSource->GetTypeId() == TYPEID_PLAYER)
        {
            ((Unit*)pSource)->HandleEmoteCommand((*i).second.Emote);
        }
        else
            error_log("SD2: DoScriptText entry %i tried to process emote for invalid TypeId (%u).",textEntry,pSource->GetTypeId());
    }

    switch((*i).second.Type)
    {
        case CHAT_TYPE_SAY:
            pSource->MonsterSay(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_YELL:
            pSource->MonsterYell(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            pSource->MonsterTextEmote(textEntry, target ? target->GetGUID() : 0);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            pSource->MonsterTextEmote(textEntry, target ? target->GetGUID() : 0, true);
            break;
        case CHAT_TYPE_WHISPER:
            {
                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    pSource->MonsterWhisper(textEntry, target->GetGUID());
                else error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", textEntry);
            }break;
        case CHAT_TYPE_BOSS_WHISPER:
            {
                if (target && target->GetTypeId() == TYPEID_PLAYER)
                    pSource->MonsterWhisper(textEntry, target->GetGUID(), true);
                else error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", textEntry);
            }break;
        case CHAT_TYPE_ZONE_YELL:
            pSource->MonsterYellToZone(textEntry, (*i).second.Language, target ? target->GetGUID() : 0);
            break;
    }
}

//*********************************
//*** Functions used internally ***

void Script::RegisterSelf()
{
    int id = GetScriptId(Name.c_str());
    if (id != 0)
    {
        m_scripts[id] = this;
        ++num_sc_scripts;
    }
    else
    {
        debug_log("SD2: RegisterSelf, but script named %s does not have ScriptName assigned in database.",(this)->Name.c_str());
        delete this;
    }
}

//********************************
//*** Functions to be Exported ***

MANGOS_DLL_EXPORT
char const* ScriptsVersion()
{
    if (!strVersion.empty())
        return strVersion.c_str();

    return _FULLVERSION;
}

MANGOS_DLL_EXPORT
bool GossipHello(Player* pPlayer, Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pGossipHello) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipHello(pPlayer, pCreature);
}

MANGOS_DLL_EXPORT
bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    debug_log("SD2: Gossip selection, sender: %u, action: %u", uiSender, uiAction);

    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pGossipSelect) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelect(pPlayer, pCreature, uiSender, uiAction);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char* sCode)
{
    debug_log("SD2: Gossip selection with code, sender: %u, action: %u", uiSender, uiAction);

    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pGossipSelectWithCode) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGossipSelectWithCode(pPlayer, pCreature, uiSender, uiAction, sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pQuestAccept) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestAccept(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
bool QuestSelect(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pQuestSelect) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestSelect(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
bool QuestComplete(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pQuestComplete) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pQuestComplete(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
bool ChooseReward(Player* pPlayer, Creature* pCreature, const Quest* pQuest, uint32 opt)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pChooseReward) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pChooseReward(pPlayer, pCreature, pQuest, opt);
}

MANGOS_DLL_EXPORT
uint32 NPCDialogStatus(Player* pPlayer, Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->pNPCDialogStatus) return 100;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pNPCDialogStatus(pPlayer, pCreature);
}

MANGOS_DLL_EXPORT
uint32 GODialogStatus(Player* pPlayer, GameObject* pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGODialogStatus) return 100;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGODialogStatus(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool ItemHello(Player* pPlayer, Item *_Item, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[_Item->GetProto()->ScriptId];
    if (!tmpscript || !tmpscript->pItemHello) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemHello(pPlayer,_Item, pQuest);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept(Player* pPlayer, Item *_Item, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[_Item->GetProto()->ScriptId];
    if (!tmpscript || !tmpscript->pItemQuestAccept) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pItemQuestAccept(pPlayer,_Item, pQuest);
}

MANGOS_DLL_EXPORT
bool GOHello(Player* pPlayer, GameObject* pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGOHello) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOHello(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept(Player* pPlayer, GameObject* pGo, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGOQuestAccept) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOQuestAccept(pPlayer, pGo, pQuest);
}

MANGOS_DLL_EXPORT
bool GOChooseReward(Player* pPlayer, GameObject* pGo, const Quest* pQuest, uint32 opt)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];
    if (!tmpscript || !tmpscript->pGOChooseReward) return false;

    pPlayer->PlayerTalkClass->ClearMenus();
    return tmpscript->pGOChooseReward(pPlayer, pGo, pQuest,opt);
}

MANGOS_DLL_EXPORT
bool AreaTrigger(Player* pPlayer, AreaTriggerEntry * atEntry)
{
    Script *tmpscript = m_scripts[GetAreaTriggerScriptId(atEntry->id)];
    if (!tmpscript || !tmpscript->pAreaTrigger) return false;

    return tmpscript->pAreaTrigger(pPlayer, atEntry);
}

MANGOS_DLL_EXPORT
CreatureAI* GetAI(Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];
    if (!tmpscript || !tmpscript->GetAI) return NULL;

    return tmpscript->GetAI(pCreature);
}

MANGOS_DLL_EXPORT
bool ItemUse(Player* pPlayer, Item* _Item, SpellCastTargets const& targets)
{
    Script *tmpscript = m_scripts[_Item->GetProto()->ScriptId];
    if (!tmpscript || !tmpscript->pItemUse) return false;

    return tmpscript->pItemUse(pPlayer,_Item,targets);
}

MANGOS_DLL_EXPORT
bool EffectDummyCreature(Unit *pCaster, uint32 spellId, uint32 effIndex, Creature *pCreatureTarget)
{
    Script *tmpscript = m_scripts[pCreatureTarget->GetScriptId()];

    if (!tmpscript || !tmpscript->pEffectDummyCreature) return false;

    return tmpscript->pEffectDummyCreature(pCaster, spellId, effIndex, pCreatureTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyGameObj(Unit *pCaster, uint32 spellId, uint32 effIndex, GameObject *pGameObjTarget)
{
    Script *tmpscript = m_scripts[pGameObjTarget->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyGameObj) return false;

    return tmpscript->pEffectDummyGameObj(pCaster, spellId, effIndex, pGameObjTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyItem(Unit *pCaster, uint32 spellId, uint32 effIndex, Item *pItemTarget)
{
    Script *tmpscript = m_scripts[pItemTarget->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyItem) return false;

    return tmpscript->pEffectDummyItem(pCaster, spellId, effIndex, pItemTarget);
}

MANGOS_DLL_EXPORT
InstanceData* CreateInstanceData(Map *map)
{
    if (!map->IsDungeon()) return NULL;

    Script *tmpscript = m_scripts[((InstanceMap*)map)->GetScriptId()];
    if (!tmpscript || !tmpscript->GetInstanceData) return NULL;

    return tmpscript->GetInstanceData(map);
}

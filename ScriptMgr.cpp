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
#include "../system/system.h"

int num_sc_scripts;
Script *m_scripts[MAX_SCRIPTS];

Config SD2Config;

void FillSpellSummary();

void LoadDatabase()
{
    std::string strSD2DBinfo = SD2Config.GetStringDefault("ScriptDev2DatabaseInfo", "");

    if (strSD2DBinfo.empty())
    {
        error_log("SD2: Missing Scriptdev2 database info from configuration file. Load database aborted.");
        return;
    }

    //Initialize connection to DB
    if (SD2Database.Initialize(strSD2DBinfo.c_str()))
    {
        outstring_log("SD2: ScriptDev2 database at %s initialized.", strSD2DBinfo.c_str());
        outstring_log("");

        pSystemMgr.LoadVersion();
        pSystemMgr.LoadScriptTexts();
        pSystemMgr.LoadScriptTextsCustom();
        pSystemMgr.LoadScriptWaypoints();
    }
    else
    {
        error_log("SD2: Unable to connect to Database. Load database aborted.");
        return;
    }

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
        error_log("SD2: Unable to open configuration file. Database will be unaccessible. Configuration values will use default.");
    else
        outstring_log("SD2: Using configuration file %s",_SCRIPTDEV2_CONFIG);

    //Check config file version
    if (SD2Config.GetIntDefault("ConfVersion", 0) != SD2_CONF_VERSION)
        error_log("SD2: Configuration file version doesn't match expected version. Some config variables may be wrong or missing.");

    outstring_log("");

    //Load database (must be called after SD2Config.SetSource).
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

void DoScriptText(int32 iTextEntry, WorldObject* pSource, Unit* pTarget)
{
    if (!pSource)
    {
        error_log("SD2: DoScriptText entry %i, invalid Source pointer.", iTextEntry);
        return;
    }

    if (iTextEntry >= 0)
    {
        error_log("SD2: DoScriptText with source entry %u (TypeId=%u, guid=%u) attempts to process text entry %i, but text entry must be negative.",
            pSource->GetEntry(), pSource->GetTypeId(), pSource->GetGUIDLow(), iTextEntry);

        return;
    }

    const StringTextData* pData = pSystemMgr.GetTextData(iTextEntry);

    if (!pData)
    {
        error_log("SD2: DoScriptText with source entry %u (TypeId=%u, guid=%u) could not find text entry %i.",
            pSource->GetEntry(), pSource->GetTypeId(), pSource->GetGUIDLow(), iTextEntry);

        return;
    }

    debug_log("SD2: DoScriptText: text entry=%i, Sound=%u, Type=%u, Language=%u, Emote=%u",
        iTextEntry, pData->uiSoundId, pData->uiType, pData->uiLanguage, pData->uiEmote);

    if (pData->uiSoundId)
    {
        if (GetSoundEntriesStore()->LookupEntry(pData->uiSoundId))
            pSource->PlayDirectSound(pData->uiSoundId);
        else
            error_log("SD2: DoScriptText entry %i tried to process invalid sound id %u.", iTextEntry, pData->uiSoundId);
    }

    if (pData->uiEmote)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT || pSource->GetTypeId() == TYPEID_PLAYER)
            ((Unit*)pSource)->HandleEmoteCommand(pData->uiEmote);
        else
            error_log("SD2: DoScriptText entry %i tried to process emote for invalid TypeId (%u).", iTextEntry, pSource->GetTypeId());
    }

    switch(pData->uiType)
    {
        case CHAT_TYPE_SAY:
            pSource->MonsterSay(iTextEntry, pData->uiLanguage, pTarget ? pTarget->GetGUID() : 0);
            break;
        case CHAT_TYPE_YELL:
            pSource->MonsterYell(iTextEntry, pData->uiLanguage, pTarget ? pTarget->GetGUID() : 0);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            pSource->MonsterTextEmote(iTextEntry, pTarget ? pTarget->GetGUID() : 0);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            pSource->MonsterTextEmote(iTextEntry, pTarget ? pTarget->GetGUID() : 0, true);
            break;
        case CHAT_TYPE_WHISPER:
        {
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(iTextEntry, pTarget->GetGUID());
            else
                error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", iTextEntry);

            break;
        }
        case CHAT_TYPE_BOSS_WHISPER:
        {
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(iTextEntry, pTarget->GetGUID(), true);
            else
                error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", iTextEntry);

            break;
        }
        case CHAT_TYPE_ZONE_YELL:
            pSource->MonsterYellToZone(iTextEntry, pData->uiLanguage, pTarget ? pTarget->GetGUID() : 0);
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
    if (!strSD2Version.empty())
    {
        strSD2Version.append(_FULLVERSION);
        return strSD2Version.c_str();
    }

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

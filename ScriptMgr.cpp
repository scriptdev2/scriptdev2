/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
#include "../../../game/ScriptMgr.h"

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
        pSystemMgr.LoadScriptGossipTexts();
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
void FreeScriptLibrary()
{
    // Free Spell Summary
    delete []SpellSummary;

    // Free resources before library unload
    for(int i=0; i<MAX_SCRIPTS; ++i)
        delete m_scripts[i];

    num_sc_scripts = 0;
}

MANGOS_DLL_EXPORT
void InitScriptLibrary()
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
            ((Unit*)pSource)->HandleEmote(pData->uiEmote);
        else
            error_log("SD2: DoScriptText entry %i tried to process emote for invalid TypeId (%u).", iTextEntry, pSource->GetTypeId());
    }

    switch(pData->uiType)
    {
        case CHAT_TYPE_SAY:
            pSource->MonsterSay(iTextEntry, pData->uiLanguage, pTarget);
            break;
        case CHAT_TYPE_YELL:
            pSource->MonsterYell(iTextEntry, pData->uiLanguage, pTarget);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            pSource->MonsterTextEmote(iTextEntry, pTarget);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            pSource->MonsterTextEmote(iTextEntry, pTarget, true);
            break;
        case CHAT_TYPE_WHISPER:
        {
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(iTextEntry, pTarget);
            else
                error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", iTextEntry);

            break;
        }
        case CHAT_TYPE_BOSS_WHISPER:
        {
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(iTextEntry, pTarget, true);
            else
                error_log("SD2: DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", iTextEntry);

            break;
        }
        case CHAT_TYPE_ZONE_YELL:
            pSource->MonsterYellToZone(iTextEntry, pData->uiLanguage, pTarget);
            break;
    }
}

//*********************************
//*** Functions used internally ***

void Script::RegisterSelf(bool bReportError)
{
    int id = GetScriptId(Name.c_str());
    if (id != 0)
    {
        m_scripts[id] = this;
        ++num_sc_scripts;
    }
    else
    {
        if (bReportError)
            error_log("SD2: Script registering but ScriptName %s is not assigned in database. Script will not be used.", (this)->Name.c_str());

        delete this;
    }
}

//********************************
//*** Functions to be Exported ***

MANGOS_DLL_EXPORT
char const* GetScriptLibraryVersion()
{
    return strSD2Version.c_str();
}

MANGOS_DLL_EXPORT
bool GossipHello(Player* pPlayer, Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pGossipHello)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipHello(pPlayer, pCreature);
}

MANGOS_DLL_EXPORT
bool GOGossipHello(Player *pPlayer, GameObject *pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGossipHelloGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipHelloGO(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    debug_log("SD2: Gossip selection, sender: %u, action: %u", uiSender, uiAction);

    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pGossipSelect)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelect(pPlayer, pCreature, uiSender, uiAction);
}

MANGOS_DLL_EXPORT
bool GOGossipSelect(Player *pPlayer, GameObject *pGo, uint32 sender, uint32 action)
{
    debug_log("SD2: GO Gossip selection, sender: %u, action: %u", sender, action);

    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGossipSelectGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectGO(pPlayer, pGo, sender, action);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char* sCode)
{
    debug_log("SD2: Gossip selection with code, sender: %u, action: %u", uiSender, uiAction);

    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pGossipSelectWithCode)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectWithCode(pPlayer, pCreature, uiSender, uiAction, sCode);
}

MANGOS_DLL_EXPORT
bool GOGossipSelectWithCode(Player *pPlayer, GameObject *pGo, uint32 sender, uint32 action, const char* sCode)
{
    debug_log("SD2: GO Gossip selection with code, sender: %u, action: %u", sender, action);

    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGossipSelectGOWithCode)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectGOWithCode(pPlayer, pGo, sender, action, sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pQuestAcceptNPC)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestAcceptNPC(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
bool QuestRewarded(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pQuestRewardedNPC)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestRewardedNPC(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
uint32 GetNPCDialogStatus(Player* pPlayer, Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pDialogStatusNPC)
        return 100;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pDialogStatusNPC(pPlayer, pCreature);
}

MANGOS_DLL_EXPORT
uint32 GetGODialogStatus(Player* pPlayer, GameObject* pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pDialogStatusGO)
        return 100;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pDialogStatusGO(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest)
{
    Script *tmpscript = m_scripts[pItem->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pQuestAcceptItem)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestAcceptItem(pPlayer, pItem, pQuest);
}

MANGOS_DLL_EXPORT
bool GOUse(Player* pPlayer, GameObject* pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGOUse)
        return false;

    return tmpscript->pGOUse(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept(Player* pPlayer, GameObject* pGo, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pQuestAcceptGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestAcceptGO(pPlayer, pGo, pQuest);
}

MANGOS_DLL_EXPORT
bool GOQuestRewarded(Player* pPlayer, GameObject* pGo, Quest const* pQuest)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pQuestRewardedGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestRewardedGO(pPlayer, pGo, pQuest);
}

MANGOS_DLL_EXPORT
bool AreaTrigger(Player* pPlayer, AreaTriggerEntry const* atEntry)
{
    Script *tmpscript = m_scripts[GetAreaTriggerScriptId(atEntry->id)];

    if (!tmpscript || !tmpscript->pAreaTrigger)
        return false;

    return tmpscript->pAreaTrigger(pPlayer, atEntry);
}

MANGOS_DLL_EXPORT
bool ProcessEvent(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    Script *tmpscript = m_scripts[GetEventIdScriptId(uiEventId)];

    if (!tmpscript || !tmpscript->pProcessEventId)
        return false;

    // bIsStart may be false, when event is from taxi node events (arrival=false, departure=true)
    return tmpscript->pProcessEventId(uiEventId, pSource, pTarget, bIsStart);
}

MANGOS_DLL_EXPORT
CreatureAI* GetCreatureAI(Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->GetAI)
        return NULL;

    return tmpscript->GetAI(pCreature);
}

MANGOS_DLL_EXPORT
bool ItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
    Script *tmpscript = m_scripts[pItem->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pItemUse)
        return false;

    return tmpscript->pItemUse(pPlayer, pItem, targets);
}

MANGOS_DLL_EXPORT
bool EffectDummyCreature(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Creature* pTarget)
{
    Script *tmpscript = m_scripts[pTarget->GetScriptId()];

    if (!tmpscript || !tmpscript->pEffectDummyNPC)
        return false;

    return tmpscript->pEffectDummyNPC(pCaster, spellId, effIndex, pTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyGameObject(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, GameObject* pTarget)
{
    Script *tmpscript = m_scripts[pTarget->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyGO)
        return false;

    return tmpscript->pEffectDummyGO(pCaster, spellId, effIndex, pTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyItem(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Item* pTarget)
{
    Script *tmpscript = m_scripts[pTarget->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyItem)
        return false;

    return tmpscript->pEffectDummyItem(pCaster, spellId, effIndex, pTarget);
}

MANGOS_DLL_EXPORT
bool AuraDummy(Aura const* pAura, bool apply)
{
    Script *tmpscript = m_scripts[((Creature*)pAura->GetTarget())->GetScriptId()];

    if (!tmpscript || !tmpscript->pEffectAuraDummy)
        return false;

    return tmpscript->pEffectAuraDummy(pAura, apply);
}

MANGOS_DLL_EXPORT
InstanceData* CreateInstanceData(Map* pMap)
{
    Script *tmpscript = m_scripts[pMap->GetScriptId()];

    if (!tmpscript || !tmpscript->GetInstanceData)
        return NULL;

    return tmpscript->GetInstanceData(pMap);
}

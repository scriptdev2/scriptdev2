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
SDName: Boss_Tomb_Of_Seven
SD%Complete: 90
SDComment: Learning Smelt Dark Iron if tribute quest rewarded. Basic event implemented. Correct order and timing of event is unknown.
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"
#include "def_blackrock_depths.h"

enum
{
    FACTION_NEUTRAL             = 734,
    FACTION_HOSTILE             = 754,

    SPELL_SUNDERARMOR           = 11971,
    SPELL_SHIELDBLOCK           = 12169,
    SPELL_STRIKE                = 15580
};

struct MANGOS_DLL_DECL boss_angerrelAI : public ScriptedAI
{
    boss_angerrelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 SunderArmor_Timer;
    uint32 ShieldBlock_Timer;
    uint32 Strike_Timer;

    void Reset()
    {
        SunderArmor_Timer = 8000;
        ShieldBlock_Timer = 15000;
        Strike_Timer = 12000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //SunderArmor_Timer
        if (SunderArmor_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_SUNDERARMOR);
            SunderArmor_Timer = 28000;
        }else SunderArmor_Timer -= diff;

        //ShieldBlock_Timer
        if (ShieldBlock_Timer < diff)
        {
            DoCast(m_creature,SPELL_SHIELDBLOCK);
            ShieldBlock_Timer = 25000;
        }else ShieldBlock_Timer -= diff;

        //Strike_Timer
        if (Strike_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_STRIKE);
            Strike_Timer = 10000;
        }else Strike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_angerrel(Creature* pCreature)
{
    return new boss_angerrelAI(pCreature);
}

enum
{
    SPELL_SINISTERSTRIKE        = 15581,
    SPELL_BACKSTAB              = 15582,
    SPELL_GOUGE                 = 12540
};

struct MANGOS_DLL_DECL boss_doperelAI : public ScriptedAI
{
    boss_doperelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 SinisterStrike_Timer;
    uint32 BackStab_Timer;
    uint32 Gouge_Timer;

    void Reset()
    {
        SinisterStrike_Timer = 8000;
        BackStab_Timer = 12000;
        Gouge_Timer = 6000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //SinisterStrike_Timer
        if (SinisterStrike_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_SINISTERSTRIKE);
            SinisterStrike_Timer = 7000;
        }else SinisterStrike_Timer -= diff;

        //BackStab_Timer
        if (BackStab_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_BACKSTAB);
            BackStab_Timer = 6000;
        }else BackStab_Timer -= diff;

        //Gouge_Timer
        if (Gouge_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_GOUGE);
            Gouge_Timer = 8000;
        }else Gouge_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_doperel(Creature* pCreature)
{
    return new boss_doperelAI(pCreature);
}

enum
{
    SPELL_SHADOWBOLT        = 15232,
    SPELL_MANABURN          = 14033,
    SPELL_SHADOWSHIELD      = 12040,
};

struct MANGOS_DLL_DECL boss_haterelAI : public ScriptedAI
{
    boss_haterelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 ShadowBolt_Timer;
    uint32 ManaBurn_Timer;
    uint32 ShadowShield_Timer;
    uint32 Strike_Timer;

    void Reset()
    {
        ShadowBolt_Timer = 15000;
        ManaBurn_Timer = 3000;
        ShadowShield_Timer = 8000;
        Strike_Timer = 12000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ShadowBolt_Timer
        if (ShadowBolt_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_SHADOWBOLT);

            ShadowBolt_Timer = 7000;
        }else ShadowBolt_Timer -= diff;

        //ManaBurn_Timer
        if (ManaBurn_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_MANABURN);

            ManaBurn_Timer = 13000;
        }else ManaBurn_Timer -= diff;

        //ShadowShield_Timer
        if (ShadowShield_Timer < diff)
        {
            DoCast(m_creature,SPELL_SHADOWSHIELD);
            ShadowShield_Timer = 25000;
        }else ShadowShield_Timer -= diff;

        //Strike_Timer
        if (Strike_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_STRIKE);
            Strike_Timer = 10000;
        }else Strike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_haterel(Creature* pCreature)
{
    return new boss_haterelAI(pCreature);
}

enum
{
    SPELL_MINDBLAST             = 15587,
    SPELL_HEAL                  = 15586,
    SPELL_PRAYEROFHEALING       = 15585,
    SPELL_SHIELD                = 11974
};

struct MANGOS_DLL_DECL boss_vilerelAI : public ScriptedAI
{
    boss_vilerelAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 MindBlast_Timer;
    uint32 Heal_Timer;
    uint32 PrayerOfHealing_Timer;
    uint32 Shield_Timer;

    void Reset()
    {
        MindBlast_Timer = 10000;
        Heal_Timer = 35000;
        PrayerOfHealing_Timer = 25000;
        Shield_Timer = 3000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //MindBlast_Timer
        if (MindBlast_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MINDBLAST);
            MindBlast_Timer = 7000;
        }else MindBlast_Timer -= diff;

        //Heal_Timer
        if (Heal_Timer < diff)
        {
            DoCast(m_creature,SPELL_HEAL);
            Heal_Timer = 20000;
        }else Heal_Timer -= diff;

        //PrayerOfHealing_Timer
        if (PrayerOfHealing_Timer < diff)
        {
            DoCast(m_creature,SPELL_PRAYEROFHEALING);
            PrayerOfHealing_Timer = 30000;
        }else PrayerOfHealing_Timer -= diff;

        //Shield_Timer
        if (Shield_Timer < diff)
        {
            DoCast(m_creature,SPELL_SHIELD);
            Shield_Timer = 30000;
        }else Shield_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vilerel(Creature* pCreature)
{
    return new boss_vilerelAI(pCreature);
}

enum
{
    SPELL_FROSTBOLT         = 12675,
    SPELL_FROSTARMOR        = 12544,                        //This is actually a buff he gives himself
    SPELL_CONEOFCOLD        = 15244,
    SPELL_FROSTNOVA         = 12674,
    SPELL_FROSTWARD         = 15044
};

struct MANGOS_DLL_DECL boss_seethrelAI : public ScriptedAI
{
    boss_seethrelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 FrostArmor_Timer;
    uint32 Frostbolt_Timer;
    uint32 ConeofCold_Timer;
    uint32 FrostNova_Timer;
    uint32 FrostWard_Timer;

    void Reset()
    {
        FrostArmor_Timer = 2000;
        Frostbolt_Timer = 6000;
        ConeofCold_Timer = 18000;
        FrostNova_Timer = 12000;
        FrostWard_Timer = 25000;

        m_creature->CastSpell(m_creature,SPELL_FROSTARMOR,true);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //FrostArmor_Timer
        if (FrostArmor_Timer < diff)
        {
            DoCast(m_creature, SPELL_FROSTARMOR);
            FrostArmor_Timer = 180000;
        }else FrostArmor_Timer -= diff;

        //Frostbolt_Timer
        if (Frostbolt_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);
            Frostbolt_Timer = 15000;
        }else Frostbolt_Timer -= diff;

        //ConeofCold_Timer
        if (ConeofCold_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_CONEOFCOLD);

            ConeofCold_Timer = 22000;
        }else ConeofCold_Timer -= diff;

        //FrostNova_Timer
        if (FrostNova_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_FROSTNOVA);
            FrostNova_Timer = 14000;
        }else FrostNova_Timer -= diff;

        //FrostWard_Timer
        if (FrostWard_Timer < diff)
        {
            DoCast(m_creature,SPELL_FROSTWARD);
            FrostWard_Timer = 68000;
        }else FrostWard_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_seethrel(Creature* pCreature)
{
    return new boss_seethrelAI(pCreature);
}

enum
{
    SPELL_HAMSTRING             = 9080,
    SPELL_CLEAVE                = 40504,
    SPELL_MORTALSTRIKE          = 13737
};

struct MANGOS_DLL_DECL boss_gloomrelAI : public ScriptedAI
{
    boss_gloomrelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Hamstring_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;

    void Reset()
    {
        Hamstring_Timer = 19000;
        Cleave_Timer = 6000;
        MortalStrike_Timer = 10000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Hamstring_Timer
        if (Hamstring_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_HAMSTRING);
            Hamstring_Timer = 14000;
        }else Hamstring_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = 8000;
        }else Cleave_Timer -= diff;

        //MortalStrike_Timer
        if (MortalStrike_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MORTALSTRIKE);
            MortalStrike_Timer = 12000;
        }else MortalStrike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gloomrel(Creature* pCreature)
{
    return new boss_gloomrelAI(pCreature);
}

// TODO: convert all above here to ACID and test event

enum
{
    SPELL_SMELT_DARK_IRON   = 14891,
    SPELL_LEARN_SMELT       = 14894,
    QUEST_SPECTRAL_CHALICE  = 4083,
    SKILLPOINT_MIN          = 230
};

#define GOSSIP_ITEM_TEACH_1 "Teach me the art of smelting dark iron"
#define GOSSIP_ITEM_TEACH_2 "Continue..."
#define GOSSIP_ITEM_TRIBUTE "I want to pay tribute"

bool GossipHello_boss_gloomrel(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_TOMB_OF_SEVEN) == NOT_STARTED)
        {
            if (pPlayer->GetQuestRewardStatus(QUEST_SPECTRAL_CHALICE) &&
                pPlayer->GetSkillValue(SKILL_MINING) >= SKILLPOINT_MIN &&
                !pPlayer->HasSpell(SPELL_SMELT_DARK_IRON))
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TEACH_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            if (!pPlayer->GetQuestRewardStatus(QUEST_SPECTRAL_CHALICE) &&
                pPlayer->GetSkillValue(SKILL_MINING) >= SKILLPOINT_MIN)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TRIBUTE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }
    }
    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());
    return true;
}

bool GossipSelect_boss_gloomrel(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TEACH_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
            pPlayer->SEND_GOSSIP_MENU(2606, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+11:
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->CastSpell(pPlayer, SPELL_LEARN_SMELT, false);
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] Continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            pPlayer->SEND_GOSSIP_MENU(2604, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+22:
            pPlayer->CLOSE_GOSSIP_MENU();
            if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
            {
                //are 5 minutes expected? go template may have data to despawn when used at quest
                pInstance->DoRespawnGameObject(pInstance->GetData64(DATA_GO_CHALICE),MINUTE*5);
            }
            break;
    }
    return true;
}

enum
{
    SPELL_SHADOWBOLTVOLLEY              = 15245,
    SPELL_IMMOLATE                      = 12742,
    SPELL_CURSEOFWEAKNESS               = 12493,
    SPELL_DEMONARMOR                    = 13787,
    SPELL_SUMMON_VOIDWALKERS            = 15092,

    MAX_DWARF                           = 7
};

#define GOSSIP_ITEM_CHALLENGE   "Your bondage is at an end, Doom'rel. I challenge you!"

struct MANGOS_DLL_DECL boss_doomrelAI : public ScriptedAI
{
    boss_doomrelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowVolley_Timer;
    uint32 m_uiImmolate_Timer;
    uint32 m_uiCurseOfWeakness_Timer;
    uint32 m_uiDemonArmor_Timer;
    uint32 m_uiCallToFight_Timer;
    uint8 m_uiDwarfRound;
    bool m_bHasSummoned;

    void Reset()
    {
        m_uiShadowVolley_Timer = 10000;
        m_uiImmolate_Timer = 18000;
        m_uiCurseOfWeakness_Timer = 5000;
        m_uiDemonArmor_Timer = 16000;
        m_uiCallToFight_Timer = 0;
        m_uiDwarfRound = 0;
        m_bHasSummoned = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, FAIL);
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    Creature* GetDwarfForPhase(uint8 uiPhase)
    {
        switch(uiPhase)
        {
            case 0:
                return m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SEETHREL));
            case 1:
                return m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_VILEREL));
            case 2:
                return m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_HATEREL));
            case 3:
                return m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_ANGERREL));
            case 4:
                return m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_GLOOMREL));
            case 5:
                return m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_DOPEREL));
            case 6:
                return m_creature;
        }
        return NULL;
    }

    void CallToFight(bool bStartFight)
    {
        if (Creature* pDwarf = GetDwarfForPhase(m_uiDwarfRound))
        {
            if (bStartFight && pDwarf->isAlive())
            {
                pDwarf->setFaction(FACTION_HOSTILE);
                pDwarf->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
                pDwarf->SetInCombatWithZone();              // attackstart
            }
            else
            {
                if (!pDwarf->isAlive() || pDwarf->isDead())
                    pDwarf->Respawn();

                pDwarf->setFaction(FACTION_NEUTRAL);
                pDwarf->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_8);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_TOMB_OF_SEVEN) == IN_PROGRESS)
            {
                if (m_uiDwarfRound < MAX_DWARF)
                {
                    if (m_uiCallToFight_Timer < diff)
                    {
                        CallToFight(true);
                        ++m_uiDwarfRound;
                        m_uiCallToFight_Timer = 30000;
                    }
                    else
                        m_uiCallToFight_Timer -= diff;
                }
            }
            else if (m_pInstance->GetData(TYPE_TOMB_OF_SEVEN) == FAIL)
            {
                for (m_uiDwarfRound = 0; m_uiDwarfRound < MAX_DWARF; ++m_uiDwarfRound)
                    CallToFight(false);

                m_uiDwarfRound = 0;
                m_uiCallToFight_Timer = 0;

                if (m_pInstance)
                    m_pInstance->SetData(TYPE_TOMB_OF_SEVEN, NOT_STARTED);
            }
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ShadowVolley_Timer
        if (m_uiShadowVolley_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_SHADOWBOLTVOLLEY);
            m_uiShadowVolley_Timer = 12000;
        }
        else
            m_uiShadowVolley_Timer -= diff;

        //Immolate_Timer
        if (m_uiImmolate_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_IMMOLATE);

            m_uiImmolate_Timer = 25000;
        }
        else
            m_uiImmolate_Timer -= diff;

        //CurseOfWeakness_Timer
        if (m_uiCurseOfWeakness_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CURSEOFWEAKNESS);
            m_uiCurseOfWeakness_Timer = 45000;
        }
        else
            m_uiCurseOfWeakness_Timer -= diff;

        //DemonArmor_Timer
        if (m_uiDemonArmor_Timer < diff)
        {
            DoCast(m_creature,SPELL_DEMONARMOR);
            m_uiDemonArmor_Timer = 300000;
        }
        else
            m_uiDemonArmor_Timer -= diff;

        //Summon Voidwalkers
        if (!m_bHasSummoned && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 50)
        {
            m_creature->CastSpell(m_creature, SPELL_SUMMON_VOIDWALKERS, true);
            m_bHasSummoned = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_doomrel(Creature* pCreature)
{
    return new boss_doomrelAI(pCreature);
}

bool GossipHello_boss_doomrel(Player* pPlayer, Creature* pCreature)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_TOMB_OF_SEVEN) == NOT_STARTED)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_CHALLENGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    }

    pPlayer->SEND_GOSSIP_MENU(2601, pCreature->GetGUID());
    return true;
}

bool GossipSelect_boss_doomrel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "[PH] Continue...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(2605, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();

            // start event
            if (ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData())
                pInstance->SetData(TYPE_TOMB_OF_SEVEN, IN_PROGRESS);

            break;
    }
    return true;
}

void AddSC_boss_tomb_of_seven()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_angerrel";
    newscript->GetAI = &GetAI_boss_angerrel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_doperel";
    newscript->GetAI = &GetAI_boss_doperel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_haterel";
    newscript->GetAI = &GetAI_boss_haterel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vilerel";
    newscript->GetAI = &GetAI_boss_vilerel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_seethrel";
    newscript->GetAI = &GetAI_boss_seethrel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_gloomrel";
    newscript->GetAI = &GetAI_boss_gloomrel;
    newscript->pGossipHello = &GossipHello_boss_gloomrel;
    newscript->pGossipSelect = &GossipSelect_boss_gloomrel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_doomrel";
    newscript->GetAI = &GetAI_boss_doomrel;
    newscript->pGossipHello = &GossipHello_boss_doomrel;
    newscript->pGossipSelect = &GossipSelect_boss_doomrel;
    newscript->RegisterSelf();
}

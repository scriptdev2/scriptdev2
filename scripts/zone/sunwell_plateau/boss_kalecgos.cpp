/* Copyright © 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/* ScriptData
SDName: Boss_Kalecgos
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "def_sunwell_plateau.h"

#define KALEC_SAY_NON_FRIENDLY_ON_AGGRO             "No longer will I be a slave to Malygos! Challenge me and you will be destroyed!"
#define KALEC_SOUND_NON_FIRENDLY_ON_AGGRO           12422
#define KALEC_SAY_NON_FRIENDLY_ON_SPELL_1           "I will purge you!"
#define KALEC_SOUND_NON_FRIENDLY_ON_SPELL_1         12423
#define KALEC_SAY_NON_FRIENDLY_ON_SPELL_2           "Your pain has only begun!"
#define KALEC_SOUND_NON_FRIENDLY_ON_SPELL_2         12424
#define KALEC_SAY_NON_FRIENDLY_ON_KILL_1            "In the name of Kil'jaeden!"
#define KALEC_SOUND_NON_FRIENDLY_ON_KILL_1          12425
#define KALEC_SAY_NON_FRIENDLY_ON_KILL_2            "You were warned!"
#define KALEC_SOUND_NON_FRIENDLY_ON_KILL_2          12426
#define KALEC_SAY_NON_FRIENDLY_BEFORE_FLY_AWAY      "My awakening is complete! You shall all perish!"
#define KALEC_SOUND_NON_FRIENDLY_BEFORE_FLY_AWAY    12427

#define SHAT_SAY_ON_AGGRO            "Gyahaha... There will be no reprieve. My work here is nearly finished."
#define SHAT_SOUND_ON_AGGRO         12451
#define SHAT_SAY_ON_DIE             "I'm... never on... the losing... side..."
#define SHAT_SOUND_ON_DIE           12452
#define SHAT_SAY_ON_SPELL_1         "Your misery is my delight!"
#define SHAT_SOUND_ON_SPELL_1       12453
#define SHAT_SAY_ON_SPELL_2         "I will watch you bleed!"
#define SHAT_SOUND_ON_SPELL_2       12454
#define SHAT_SAY_ON_KILL_1          "Pitious mortal!"
#define SHAT_SOUND_ON_KILL_1        12455
#define SHAT_SAY_ON_KILL_2          "Haven't you heard? I always win!"
#define SHAT_SOUND_ON_KILL_2        12456
#define SHAT_SAY_ON_ENRAGE          "I have toyed with you long enough!"
#define SHAT_SOUND_ON_ENRAGE        12457

#define KALEC_SAY_FRIENDLY_ON_AGGRO         "I need... your help... Cannot... resist him... much longer..."
#define KALEC_SOUND_FRIENDLY_ON_AGGRO       12428
#define KALEC_SAY_FRIENDLY_ON_LOSING        "Aaahhh! Help me, before I lose my mind!"
#define KALEC_SOUND_FRIENDLY_ON_LOSING      12429
#define KALEC_SAY_FRIENDLY_ON_NEAR_LOSING   "Hurry! There is not much of me left!" //???
#define KALEC_SOUND_FRIENDLY_ON_NEAR_LOSING 12430
#define KALEC_SAY_FRIENDLY_AT_END           "I am forever in your debt. Once we have triumphed over Kil'jaeden, this entire world will be in your debt as well."
#define KALEC_SOUND_FRIENDLY_AT_END         12431

#define GO_FAILED               "You are unable to use this currently."

#define FLY_X       1551
#define FLY_Y       788
#define FLY_Z       121

#define AURA_PLAYER_NOT_ABLE_TO_TELEPORT        44867
#define AURA_PLAYER_IS_TELEPORTED               46021
#define KALECGOS_TELEPORT                       44866 //summon go, and dealdmg(currently) (spellname: spectral blast)
#define TRANSFORM_TO_KALEC                      44670

#define ARCANE_BUFFET               45018
#define FROST_BREATH                44799

#define CORRUPTING_STRIKE           45029
#define COURSE_OF_BUNDLESS_AGONY    45032
#define SHADOW_BOLT_VOLLEY          45031

#define SPELL_BANISH                40370
#define BERSERK                     26662 //...need info

#define HEROIC_STRIKE               45026 //aggro..:)
#define REVITALIZE                  45860

//timers not blizzlike
//say @ spellcastion: not implemented

struct MANGOS_DLL_DECL boss_kalecgosAI : public ScriptedAI
{
    boss_kalecgosAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
    
    uint32 wild_magics[6];
    uint32 arcane_buffet_timer;
    uint32 frost_breath_timer;
    uint32 wild_magic_timer;
    uint32 spectral_blast_timer;
    uint32 checktimer;
    uint32 end_timer;

    bool flying;
    bool end;
    bool hasYelled;
    bool friendly;

    ScriptedInstance *pInstance;


    void Reset()
    {
        m_creature->setFaction(14);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        wild_magics[0] = 44978;
        wild_magics[1] = 45001;
        wild_magics[2] = 45002;
        wild_magics[3] = 45004;
        wild_magics[4] = 45006;
        wild_magics[5] = 45010;
        end_timer = 600000;
        arcane_buffet_timer = 8000; //every 8 sec we cast this 100%
        frost_breath_timer = 15000;
        wild_magic_timer = 10000;
        spectral_blast_timer = 20000+(rand()%5000);
        checktimer = spectral_blast_timer+20000; //after spectral blast
        
        if(pInstance)
        {
            pInstance->SetData(DATA_KALEC_FLY,0);
            pInstance->SetData(DATA_SHAT_DIE,0);
        }

        flying = false;
        end = false;
        hasYelled = false;
        friendly = false;
    }

    void Aggro(Unit* who)
    {
        if(pInstance->GetData(DATA_KALEC_FLY) == 1)
            fly();
        else
        {
            DoYell(KALEC_SAY_NON_FRIENDLY_ON_AGGRO,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,KALEC_SOUND_NON_FIRENDLY_ON_AGGRO);
            uint64 GoGUID = pInstance->GetData64(KALECGOS_FORCE_FIELD);
            GameObject *GO = GameObject::GetGameObject(*m_creature,GoGUID);
            GO->SetLootState(GO_ACTIVATED);
        }        
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
            case 0:
                DoYell(KALEC_SAY_NON_FRIENDLY_ON_KILL_1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,KALEC_SOUND_NON_FRIENDLY_ON_KILL_1);
                break;
            case 1:
                DoYell(KALEC_SAY_NON_FRIENDLY_ON_KILL_2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,KALEC_SOUND_NON_FRIENDLY_ON_KILL_2);
                break;
        }
    }
    void JustDied(Unit *victim)
    {
        if(pInstance->GetData(DATA_KALEC_FLY) != 1)
        {
            m_creature->Respawn();
            m_creature->CastSpell(m_creature,BERSERK,false);
            uint64 ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);
            Unit *shat = Unit::GetUnit(*m_creature,ShatGUID);
            if(shat)
            {
                if(shat->isAlive())
                    shat->CastSpell(shat,BERSERK,false);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(pInstance->GetData(DATA_KALEC_FLY) == 1)
        {
            if(!flying)
                fly();

            flying = true;
            return;
        }
        if(pInstance->GetData(DATA_SHAT_DIE) == 1)
        {
            if(!friendly)
                ChangeToFriendly();
            friendly = true;
            return;
        }

        if(end_timer < 60000 && !hasYelled)
        {
            uint64 KalecGUID = pInstance->GetData64(DATA_KALEC_HUMAN);
            Unit *Kalec = Unit::GetUnit(*m_creature,KalecGUID);
            if(Kalec && Kalec->isAlive())
            {
                ((Creature*)Kalec)->Yell(KALEC_SAY_FRIENDLY_ON_NEAR_LOSING,LANG_UNIVERSAL,0);
                DoPlaySoundToSet(Kalec,KALEC_SOUND_FRIENDLY_ON_NEAR_LOSING);
            }
            hasYelled = true;
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!end)
            if(end_timer < diff)
            {
                m_creature->CastSpell(m_creature,BERSERK,false);
                uint64 ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);
                Unit *shat = Unit::GetUnit(*m_creature,ShatGUID);
                if(shat)
                    shat->CastSpell(shat,BERSERK,false);
                end = true;

            }else end_timer -= diff;

        if(arcane_buffet_timer < diff)
        {
            DoCast(m_creature->getVictim(), ARCANE_BUFFET);
            arcane_buffet_timer = 8000;
        } else arcane_buffet_timer -= diff;

        if(frost_breath_timer < diff)
        {
            DoCast(m_creature->getVictim(), FROST_BREATH);
            frost_breath_timer = 15000;
        } else frost_breath_timer -= diff;

        if(wild_magic_timer < diff)
        {
            int wild_num = rand()%5;
            DoCast(m_creature->getVictim(), wild_magics[wild_num]);
            wild_magic_timer = 20000;
        } else wild_magic_timer -= diff;

        if(spectral_blast_timer < diff)
        {
            Unit *goback = NULL;
            Unit *target = NULL;

            std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostilReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                goback = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if(!goback)
                    continue;

                if(goback->GetTypeId() == TYPEID_PLAYER && goback->HasAura(AURA_PLAYER_NOT_ABLE_TO_TELEPORT,0) && !goback->HasAura(AURA_PLAYER_IS_TELEPORTED,0) && goback != m_creature->getVictim())
                    target = goback;
            }
            if(target)
            {
                DoCast(target, KALECGOS_TELEPORT);

                DoTeleportPlayer(target,target->GetPositionX(),target->GetPositionY(),SHATROV_LOC_Z,target->GetOrientation());
                
                target->CastSpell(target,AURA_PLAYER_IS_TELEPORTED,false);
                uint64 ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);
                Unit *shat = Unit::GetUnit(*m_creature,ShatGUID);
                if(shat)
                    shat->AddThreat(target,0.0f);
            }
            spectral_blast_timer = 20000+(rand()%5000);
        }else spectral_blast_timer -= diff;

        if(checktimer < diff)
        {
            uint64 ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);

            Unit *shat = Unit::GetUnit(*m_creature,ShatGUID);

            uint32 shat_curr_hp = (shat->GetHealth()*100)/shat->GetMaxHealth();
            uint32 kalec_curr_hp = (m_creature->GetHealth()*100)/m_creature->GetMaxHealth();

            if(shat_curr_hp - kalec_curr_hp < 5 && shat_curr_hp - kalec_curr_hp > -5)
            { 
                //is blizzlike?
                DoCast(m_creature,BERSERK);
                uint64 ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);
                Unit *shat = Unit::GetUnit(*m_creature,ShatGUID);
                if(shat)
                    shat->CastSpell(shat,BERSERK,false);
            }

            checktimer = 30000; //every 30sec we check this
        }else checktimer -= diff;

        if((m_creature->GetHealth()*100)/m_creature->GetMaxHealth() <= 2)
        {
            uint64 shatGUID = pInstance->GetData64(DATA_SHATHROVARR);
            Unit *shat = Unit::GetUnit(*m_creature,shatGUID);
            if(shat && shat->isAlive())
            {
                if((shat->GetHealth()*100)/shat->GetMaxHealth() >= 2)
                    m_creature->CastSpell(m_creature,SPELL_BANISH,false);
            }
        }

        DoMeleeAttackIfReady();
    }
    void ChangeToFriendly()
    {
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->setFaction(35);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Unit *stopattack;
        std::list<HostilReference *> stop_list = m_creature->getThreatManager().getThreatList();
        for(std::list<HostilReference *>::iterator itr = stop_list.begin(); itr!= stop_list.end(); ++itr)
        {
            stopattack = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
            stopattack->InterruptNonMeleeSpells(true);
            stopattack->AttackStop();
        }
        m_creature->DeleteThreatList();
        m_creature->AttackStop();
        uint64 GoGUID = pInstance->GetData64(KALECGOS_FORCE_FIELD);
        GameObject *GO = GameObject::GetGameObject(*m_creature,GoGUID);
        GO->SetLootState(GO_JUST_DEACTIVATED);
    }
    void fly()
    {
        DoYell(KALEC_SAY_NON_FRIENDLY_BEFORE_FLY_AWAY,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,KALEC_SOUND_NON_FRIENDLY_BEFORE_FLY_AWAY);
        m_creature->SetUnitMovementFlags(MOVEMENTFLAG_ONTRANSPORT);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(0,FLY_X,FLY_Y,FLY_Z);
    }

    void MovementInform(uint32 type,uint32 id)
    {
        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        m_creature->RemoveCorpse();

    }
};
struct MANGOS_DLL_DECL boss_SathrovarrAI : public ScriptedAI
{
    uint32 corruption_strike_timer;
    uint32 course_of_bundless_agony_timer;
    uint32 shadow_bolt_volley_timer;
    uint32 refresh;
    ScriptedInstance *pInstance;
    Unit *target;
    bool kalec_enranged;

    boss_SathrovarrAI(Creature *c) : ScriptedAI(c) 
    {
        Reset();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }

    void Reset()
    {
        m_creature->SetCorpseDelay(600000);
        shadow_bolt_volley_timer = 7000+(rand()%3000);
        course_of_bundless_agony_timer = 20000;
        corruption_strike_timer = 13000;
        refresh = 1000;
        kalec_enranged = false;
    }

    void Aggro(Unit* who)
    {
        uint64 kalec_hGUID = pInstance->GetData64(DATA_KALEC_HUMAN);
        Unit *kalec_h = Unit::GetUnit(*m_creature,kalec_hGUID);
        if(kalec_h && !kalec_h->isAlive())
        {
            ((Creature*)kalec_h)->Respawn();
        }
        if(kalec_h && who != kalec_h)
        {
            kalec_h->Attack(m_creature, true);
            float aggro = m_creature->getThreatManager().getThreat(who);
            m_creature->AddThreat(kalec_h,aggro+1);
        }
        DoYell(SHAT_SAY_ON_AGGRO,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SHAT_SOUND_ON_AGGRO);
    }

    void KilledUnit(Unit *)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SHAT_SAY_ON_KILL_1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SHAT_SOUND_ON_KILL_1);
            break;
        case 1:
            DoYell(SHAT_SAY_ON_KILL_2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SHAT_SOUND_ON_KILL_2);
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SHAT_SAY_ON_DIE,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SHAT_SOUND_ON_DIE);
        uint64 kalecGUID = pInstance->GetData64(DATA_KALECGOS);

        Unit *kalec = Unit::GetUnit(*m_creature,kalecGUID);
        Unit *target;
        std::list<HostilReference *> back_l = kalec->getThreatManager().getThreatList();

        for(std::list<HostilReference *>::iterator itr = back_l.begin(); itr != back_l.end() ; ++itr)
        {
            target = Unit::GetUnit(*m_creature,(*itr)->getUnitGuid());
            if(target->GetTypeId() == TYPEID_PLAYER)
            {
                if(target->HasAura(AURA_PLAYER_IS_TELEPORTED,0))
                {
                    DoTeleportPlayer(target, target->GetPositionX(),target->GetPositionY(),KALEC_LOC_Z,target->GetOrientation());
                    //((Player*)target)->TeleportTo(target->GetMapId(),target->GetPositionX(),target->GetPositionY(),KALEC_LOC_Z,target->GetOrientation());
                    target->RemoveAurasDueToSpell(AURA_PLAYER_IS_TELEPORTED);
                }
            }
        }
        m_creature->Relocate(m_creature->GetPositionX(),m_creature->GetPositionY(),KALEC_LOC_Z,m_creature->GetOrientation());
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->HasAura(AURA_PLAYER_IS_TELEPORTED,0))
            DoCast(m_creature,AURA_PLAYER_IS_TELEPORTED);

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;    

        if(refresh < diff)
        {
            Unit *goback;
            std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();

            for(std::list<HostilReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                goback = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if(goback->GetTypeId() == TYPEID_PLAYER)
                {
                    if(!goback->HasAura(AURA_PLAYER_IS_TELEPORTED,0))
                    {
                        m_creature->InterruptNonMeleeSpells(true);
                        m_creature->getThreatManager().getThreatList().remove((*itr));

                        DoTeleportPlayer(goback,goback->GetPositionX(),goback->GetPositionY(),KALEC_LOC_Z,goback->GetOrientation());
                        //((Player*)goback)->TeleportTo(goback->GetMapId(),goback->GetPositionX(),goback->GetPositionY(),KALEC_LOC_Z,goback->GetOrientation());
                        
                        goback->CastSpell(goback,AURA_PLAYER_NOT_ABLE_TO_TELEPORT,false);
                    }
                }
            }
            refresh = 1000;
        }else refresh -= diff;

        if(shadow_bolt_volley_timer< diff)
        {
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, SHADOW_BOLT_VOLLEY);

            shadow_bolt_volley_timer = 7000+(rand()%3000);
        }else shadow_bolt_volley_timer -= diff;

        if(course_of_bundless_agony_timer < diff)
        {
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if(target)
                DoCast(target, COURSE_OF_BUNDLESS_AGONY);

            course_of_bundless_agony_timer = 20000;
        }else course_of_bundless_agony_timer -= diff;

        if(corruption_strike_timer < diff)
        {
            DoCast(m_creature->getVictim(), CORRUPTING_STRIKE);
            corruption_strike_timer = 13000;
        }
        else corruption_strike_timer -= diff;

        if((m_creature->GetHealth()*100)/m_creature->GetMaxHealth() < 5 && !kalec_enranged)
        {
            DoYell(SHAT_SAY_ON_ENRAGE,LANG_UNIVERSAL,NULL);
            uint64 kalecGUID = pInstance->GetData64(DATA_KALECGOS);
            Unit *kalec = Unit::GetUnit(*m_creature,kalecGUID);

            if(kalec)
                kalec->CastSpell(kalec,BERSERK,false);
            kalec_enranged = true;
        }
        DoMeleeAttackIfReady();    
    }
};
struct MANGOS_DLL_DECL boss_kalecAI : public ScriptedAI
{
    ScriptedInstance *pInstance;
    Unit *shat;
    uint32 timer_to_say;
    uint32 timer_to_fly;
    uint32 timer_to_aggro_say;
    bool data_send;
    bool ready_to_aggro_say;
    uint32 revitalize;
    uint32 heroic_timer;
    uint64 ShatGUID;
    bool hasYelled;

    boss_kalecAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        Reset();
    }

    void Reset()
    {        
        revitalize = 5000;
        heroic_timer = 3000;
        ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);
        shat = Unit::GetUnit(*m_creature,ShatGUID);
        hasYelled = true;
        data_send = false;
        ready_to_aggro_say = false;
        timer_to_say = 7000;
        timer_to_fly = 11000;
        timer_to_aggro_say = 11000;
    }

    void Aggro(Unit* who)
    {
        ready_to_aggro_say = true;
    }

    void JustDied(Unit *victim)
    {
        if(shat && shat->isAlive())
        {
            //-xsec from end
            uint64 KalecGUID = pInstance->GetData64(DATA_KALECGOS);
            Unit *kalec = Unit::GetUnit(*m_creature,KalecGUID);
            ((boss_kalecgosAI*)kalec)->end_timer = ((boss_kalecgosAI*)kalec)->end_timer-2000;
        }
        m_creature->RemoveCorpse();
    }
    void UpdateAI(const uint32 diff)
    {
        if(!shat)
            return;

        if(ready_to_aggro_say)
        {
            if(timer_to_aggro_say < diff)
            {
                ready_to_aggro_say = false;
                DoYell(KALEC_SAY_FRIENDLY_ON_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,KALEC_SOUND_FRIENDLY_ON_AGGRO);

            }else timer_to_aggro_say -= diff;
        }

        if(!m_creature->HasAura(AURA_PLAYER_IS_TELEPORTED,0))
            DoCast(m_creature,AURA_PLAYER_IS_TELEPORTED);

        if(!m_creature->HasAura(TRANSFORM_TO_KALEC,0))
            DoCast(m_creature,TRANSFORM_TO_KALEC);

        if(!data_send)
        {
            if(!shat->isAlive())
            {
                pInstance->SetData(DATA_SHAT_DIE,1);
                data_send = true;
                hasYelled = false;
            }
        }
        if(!hasYelled)
        {
            if(timer_to_say < diff)
            {
                DoYell(KALEC_SAY_FRIENDLY_AT_END,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,KALEC_SOUND_FRIENDLY_AT_END);
                hasYelled = true;

            }else timer_to_say -= diff;
        }
        if(timer_to_say < diff)
        {
            if(hasYelled)
            {
                if(timer_to_fly < diff)
                {
                    pInstance->SetData(DATA_KALEC_FLY,1);
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                
                }else timer_to_fly -= diff;
            }
        }

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(revitalize < diff)
        {
            Unit *target;
            std::list<HostilReference *> t_list = shat->getThreatManager().getThreatList();
            for(std::list<HostilReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());

                if(target->GetTypeId() == TYPEID_PLAYER)
                {
                    if(target->GetHealth() < 4000)
                        DoCast(m_creature,REVITALIZE);
                }
            }
            revitalize = 5000;
        }else revitalize -= diff;

        if(shat && shat->getVictim() == m_creature)
        {
            if(heroic_timer < diff)
            {
                DoCast(shat,HEROIC_STRIKE);
                heroic_timer = 2000;

            }else heroic_timer -= diff;

            DoMeleeAttackIfReady();
        }else
            m_creature->AttackStop();

    }
};

bool GOkalocegos_teleporter(Player *player, GameObject* _GO)
{
    if(player->HasAura(AURA_PLAYER_NOT_ABLE_TO_TELEPORT,0))
    {
        player->GetSession()->SendNotification(GO_FAILED);
        return false;
    }
    ScriptedInstance *pInstance  = (_GO->GetInstanceData()) ? ((ScriptedInstance*)_GO->GetInstanceData()) : NULL;
    uint64 ShatGUID = pInstance->GetData64(DATA_SHATHROVARR);
    Unit *shat = Unit::GetUnit(*player,ShatGUID);
    if(shat)
    {
        shat->AddThreat(player,0.0f);
    }

    ((Player*)player)->TeleportTo(player->GetMapId(),player->GetPositionX(),player->GetPositionY(),SHATROV_LOC_Z,player->GetOrientation());
    player->CastSpell(player,AURA_PLAYER_IS_TELEPORTED,true);
    return true;
}

CreatureAI* GetAI_boss_kalecgos(Creature *_Creature)
{
    return new boss_kalecgosAI (_Creature);
}

CreatureAI* GetAI_boss_Sathrovarr(Creature *_Creature)
{
    return new boss_SathrovarrAI (_Creature);
}
CreatureAI* GetAI_boss_kalec(Creature *_Creature)
{
    return new boss_kalecAI (_Creature);
}
void AddSC_boss_kalecgos()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_kalecgos";
    newscript->GetAI = GetAI_boss_kalecgos;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_sathrovarr";
    newscript->GetAI = GetAI_boss_Sathrovarr;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_kalec";
    newscript->GetAI = GetAI_boss_kalec;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="kalocegos_teleporter";
    newscript->pGOHello = GOkalocegos_teleporter;
    m_scripts[nrscripts++] = newscript;
}

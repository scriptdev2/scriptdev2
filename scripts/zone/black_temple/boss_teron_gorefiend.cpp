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

#include "../../sc_defines.h"
#include "def_black_temple.h"

/* ScriptData
SDName: Boss_Teron_Gorefiend
SD%Complete: 60
SDComment: Requires Mind Control support for Ghosts.
SDCategory: Black Temple
EndScriptData */

//Spells

#define SPELL_INCINERATE          40239
#define SPELL_CRUSHING_SHADOWS          40243
#define SPELL_SHADOWBOLT          40185          
#define SPELL_PASSIVE_SHADOWFORM        41913


//Speech'n'sound

#define SAY_INTRO          "I was the first, you know. For me, the wheel of death has spun many times. So much time has passed. I have a lot of catching up to do..." 
#define SOUND_INTRO         11512

#define SAY_AGGRO          "Vengeance is mine!"
#define SOUND_AGGRO        11513

#define SAY_SLAY1          "I have use for you!"
#define SOUND_SLAY1        11514

#define SAY_SLAY2          "It gets worse..."
#define SOUND_SLAY2        11515

#define SAY_SPELL1          "What are you afraid of?"
#define SOUND_SPELL1        11517

#define SAY_SPELL2          "Death... really isn't so bad."
#define SOUND_SPELL2        11516

#define SAY_SPECIAL1        "Give in!"
#define SOUND_SPECIAL1      11518

#define SAY_SPECIAL2        "I have something for you..."
#define SOUND_SPECIAL2      11519

#define SAY_ENRAGE          "YOU WILL SHOW THE PROPER RESPECT!"
#define SOUND_ENRAGE        11520

#define SAY_DEATH           "The wheel...spins...again...."
#define SOUND_DEATH         11521

//Locations

#define Z_SUMMON           193.201141

#define X_SUMMON1          527.814087
#define Y_SUMMON1          380.843170

#define X_SUMMON2          527.814087
#define Y_SUMMON2          421.829803

struct MANGOS_DLL_DECL mob_doom_blossomAI : public ScriptedAI
{
    mob_doom_blossomAI(Creature *c) : ScriptedAI(c)
    {
        SetVariables();
    }

    bool InCombat;

    uint32 CheckTeronTimer;
    uint32 ShadowBoltTimer;
    uint64 TeronGUID;

    void SetVariables()
    {
        CheckTeronTimer = 5000;
        InCombat = false;
        ShadowBoltTimer = 12000;
        TeronGUID = 0;
    }

    void EnterEvadeMode()
    {
        SetVariables();

        m_creature->CombatStop();
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        DoGoHome();
    }

    void MoveInLineOfSight(Unit *who)
    {
        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if(who && who->isAlive())
                {
                    InCombat = true;
                    m_creature->AddThreat(who, 1.0f);
                }
            }
        }
    }

    void AttackStart(Unit *who) { return; }

    void UpdateAI(const uint32 diff)
    {
        if(!InCombat)
            return;

        if(!m_creature->isInCombat()) m_creature->SetInCombat();

        if(CheckTeronTimer < diff)
        {
            if(TeronGUID)
            {
                Creature* Teron = ((Creature*)Unit::GetUnit((*m_creature), TeronGUID));
                if((Teron) && (!Teron->isAlive() || Teron->IsInEvadeMode()))
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_NORMAL, NULL, false);
            }
            CheckTeronTimer = 5000;
        }else CheckTeronTimer -= diff;

        if(ShadowBoltTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_SHADOWBOLT);
            ShadowBoltTimer = 15000;
        }else ShadowBoltTimer -= diff;
    }

    void SetTeronGUID(uint64 guid)
    {
        if(guid)
            TeronGUID = guid;
    }
};

struct MANGOS_DLL_DECL boss_teron_gorefiendAI : public ScriptedAI
{
    boss_teron_gorefiendAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint32 IncinerateTimer;
    uint32 SummonDoomBlossomTimer;
    uint32 EnrageTimer;
    uint32 CrushingShadowsTimer;
    uint32 SummonShadowsTimer;
    uint32 RandomYellTimer;
    uint32 AggroTimer;
    uint64 AggroTargetGUID;

    bool InCombat;
    bool Intro;
    bool HasEnraged;

    void SetVariables()
    {
        if(pInstance)
            pInstance->SetData(DATA_TERONGOREFIENDEVENT, 0);

        IncinerateTimer = 40000;
        SummonDoomBlossomTimer = 12000;
        EnrageTimer = 600000;
        CrushingShadowsTimer = 22000;
        SummonShadowsTimer = 60000;
        RandomYellTimer = 50000;

        HasEnraged = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE); // Start off unattackable so that the intro is done properly
    }

    void EnterEvadeMode()
    {
        SetVariables();
        InCombat = false;
        AggroTimer = 20000;
        AggroTargetGUID = 0;
        Intro = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!who || Intro)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if(who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);

            if(!who || (!who->isAlive())) return;

            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                
                m_creature->AddThreat(who, 1.0f);
            }
            
            if(!InCombat && !Intro && m_creature->IsWithinDistInMap(who, 200.0f))
            {
                if(pInstance)
                    pInstance->SetData(DATA_TERONGOREFIENDEVENT, 1);

                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoYell(SAY_INTRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_INTRO);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                AggroTargetGUID = who->GetGUID();
                Intro = true;
            }
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
            case 0:
                DoYell(SAY_SLAY1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY1);
                break;
            case 1:
                DoYell(SAY_SLAY2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_SLAY2);
                break;
        }
    }

    void JustDied(Unit *victim)
    {
        if(pInstance)
            pInstance->SetData(DATA_TERONGOREFIENDEVENT, 3);

        InCombat = false;
        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    float CalculateRandomLocation(float Loc)
    {
        float coord = Loc;
        switch(rand()%2)
        {
            case 0:
                coord += rand()%5;
                break;
            case 1:
                coord -= rand()%5;
                break;
        }
        return coord;
    }

    void SetThreatList(Creature* Blossom)
    {
        if(!Blossom) return;

        std::list<HostilReference*>& m_threatlist = m_creature->getThreatManager().getThreatList();
        std::list<HostilReference*>::iterator i = m_threatlist.begin();
        for(i = m_threatlist.begin(); i != m_threatlist.end(); i++)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());
            if(pUnit && pUnit->isAlive())
            {
                float threat = m_creature->getThreatManager().getThreat(pUnit);
                Blossom->AddThreat(pUnit, threat);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(Intro)
        {
            if(AggroTimer < diff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE);
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                InCombat = true;
                Intro = false;
                if(AggroTargetGUID)
                {
                    Unit* pUnit = Unit::GetUnit((*m_creature), AggroTargetGUID);
                    if(pUnit) AttackStart(pUnit);
                }
            }else AggroTimer -= diff;
        }

        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() || Intro)
            return;

        if(SummonShadowsTimer < diff)
        {
            Creature* Shadow1 = NULL;
            Shadow1 = m_creature->SummonCreature(23111, m_creature->GetPositionX()+20, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            Shadow1->CastSpell(Shadow1, SPELL_PASSIVE_SHADOWFORM, true);
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && Shadow1)
                Shadow1->AddThreat(target, 1.0f);

            Creature* Shadow2 = NULL;
            Shadow2 = m_creature->SummonCreature(23111, m_creature->GetPositionX()-20,m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            Shadow2->CastSpell(Shadow2, SPELL_PASSIVE_SHADOWFORM, true);
            Unit* target2 = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target2 && Shadow2)
                Shadow2->AddThreat(target2, 1.0f);

            SummonShadowsTimer = 60000;
        }else SummonShadowsTimer -= diff;

        if(SummonDoomBlossomTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                float X = CalculateRandomLocation(m_creature->GetPositionX());
                float Y = CalculateRandomLocation(m_creature->GetPositionY());
                Creature* DoomBlossom = NULL;
                DoomBlossom = m_creature->SummonCreature(23123, X, Y, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
                if(DoomBlossom)
                {
                    DoomBlossom->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    DoomBlossom->setFaction(m_creature->getFaction());
                    DoomBlossom->AddThreat(target, 1.0f);
                    ((mob_doom_blossomAI*)DoomBlossom->AI())->SetTeronGUID(m_creature->GetGUID());
                    SetThreatList(DoomBlossom);
                }
            }
            SummonDoomBlossomTimer = 45000;
        }else SummonDoomBlossomTimer -= diff;

        if(IncinerateTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target)
            {
                switch(rand()%2)
                {
                    case 0:
                        DoYell(SAY_SPECIAL1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_SPECIAL1);
                        break;
                    case 1:
                        DoYell(SAY_SPECIAL2,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature, SOUND_SPECIAL2);
                        break;
                }                         
                DoCast(target, SPELL_INCINERATE);
                IncinerateTimer = 10000 + rand()%31 * 1000;
            }
        }else IncinerateTimer -= diff;

        if(CrushingShadowsTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCast(target, SPELL_CRUSHING_SHADOWS);
            CrushingShadowsTimer = 10000 + rand()%16 * 1000;
        }else CrushingShadowsTimer -= diff;

        if(RandomYellTimer < diff)
        {
            switch(rand()%2)
            {
                case 0:
                    DoYell(SAY_SPELL1,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SPELL1);
                    break;
                case 1:
                    DoYell(SAY_SPELL2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SPELL2);
                   break;
            }
            RandomYellTimer = 50000 + rand()%51 * 1000;
        }else RandomYellTimer -= diff;

        if(!HasEnraged)
        {
            if(EnrageTimer < diff)
            {
                DoYell(SAY_ENRAGE,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
                HasEnraged = true;
            }else EnrageTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_doom_blossom(Creature *_Creature)
{
    return new mob_doom_blossomAI(_Creature);
}

CreatureAI* GetAI_boss_teron_gorefiend(Creature *_Creature)
{
    return new boss_teron_gorefiendAI (_Creature);
}

void AddSC_boss_teron_gorefiend()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "mob_doom_blossom";
    newscript->GetAI = GetAI_mob_doom_blossom;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_teron_gorefiend";
    newscript->GetAI = GetAI_boss_teron_gorefiend;
    m_scripts[nrscripts++] = newscript;
}

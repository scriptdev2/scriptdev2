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

/* ScriptData
SDName: Boss_Teron_Gorefiend
SD%Complete: 100
SDComment: 
SDCategory: Black Temple
EndScriptData */

//Spells

#define SPELL_INCINERATE          40239
#define SPELL_CRUSHING_SHADOWS          40243
#define SPELL_SHADOWBOLT          40185          
#define SPELL_PASSIVE_SHADOWFORM        41913


//Speech'n'sound

#define SAY_INTRO          "I was the first, you know. For me, the wheel of death has spun many times. So much time has passed. I have a lot of catching up to do..." 
#define SOUND_INTRO          11512

#define SAY_AGGRO          "Vengeance is mine!"
#define SOUND_AGGRO          11513

#define SAY_SLAY1          "I have use for you!"
#define SOUND_SLAY1          11514

#define SAY_SLAY2          "It gets worse..."
#define SOUND_SLAY2          11515

#define SAY_SPELL1          "What are you afraid of?"
#define SOUND_SPELL1          11517

#define SAY_SPELL2          "Death... really isn't so bad."
#define SOUND_SPELL2          11516

#define SAY_SPECIAL1          "Give in!"
#define SOUND_SPECIAL1          11518

#define SAY_SPECIAL2          "I have something for you..."
#define SOUND_SPECIAL2          11519

#define SAY_ENRAGE          "YOU WILL SHOW THE PROPER RESPECT!"
#define SOUND_ENRAGE          11520

#define SAY_DEATH          "The wheel...spins...again...."
#define SOUND_DEATH          11521


//Locations

#define Z_SUMMON          193.201141

#define X_SUMMON1          527.814087
#define Y_SUMMON1          380.843170

#define X_SUMMON2          527.814087
#define Y_SUMMON2          421.829803


struct MANGOS_DLL_DECL boss_teron_gorefiendAI : public ScriptedAI
{
    boss_teron_gorefiendAI(Creature *c) : ScriptedAI(c) 
    {
        if(c->GetInstanceData())
            pInstance = ((ScriptedInstance*)c->GetInstanceData());
        else pInstance = NULL;
        SetVariables();
    }

    ScriptedInstance* pInstance;

    uint32 IncinerateTimer;
    uint32 ShadowBoltTimer;
    uint32 EnrageTimer;
    uint32 CrushingShadowsTimer;
    uint32 SummonShadowsTimer;
    uint32 RandomYellTimer;

    bool InCombat;
    bool Intro;
    bool HasEnraged;

    void SetVariables()
    {
        if(pInstance)
            pInstance->SetData("TeronGorefiendEvent", 0);

        IncinerateTimer = 40000;
        ShadowBoltTimer = 22000;
        EnrageTimer = 600000;
        CrushingShadowsTimer = 22000;
        SummonShadowsTimer = 60000;
        RandomYellTimer = 50000;

        Intro = false;
        HasEnraged = false;
        InCombat = false;
    }

    void EnterEvadeMode()
    {
        InCombat = false;

        (*m_creature).GetMotionMaster()->Clear(false);
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if(!who)
            return;

        if(who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if(!InCombat)
            {
                if(pInstance)
                    pInstance->SetData("TeronGorefiendEvent", 1);

                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                InCombat = true;
            }
        }
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
                    m_creature->AddThreat(who, 1.0f);

                if(!InCombat)
                {
                    if(pInstance)
                        pInstance->SetData("TeronGorefiendEvent", 1);

                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }
            }
            else if (!Intro && m_creature->IsWithinDistInMap(who, 100.0f))
            {
                DoYell(SAY_INTRO,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature, SOUND_INTRO);
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
            pInstance->SetData("TeronGorefiendEvent", 3);

        DoYell(SAY_DEATH,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
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

        if(ShadowBoltTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
            {
                DoCast(target, SPELL_SHADOWBOLT);
                if(!HasEnraged)
                {
                    ShadowBoltTimer = 20 + rand()%7 * 1000;
                }
                else
                {
                    ShadowBoltTimer = 1000;
                }     
            }
        }else ShadowBoltTimer -= diff;

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
                IncinerateTimer = 20000 + rand()%31 * 1000;
            }
        }else IncinerateTimer -= diff;

        if(CrushingShadowsTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCast(target, SPELL_CRUSHING_SHADOWS);
            CrushingShadowsTimer = 30000 + rand()%16 * 1000;
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

CreatureAI* GetAI_boss_teron_gorefiend(Creature *_Creature)
{
    return new boss_teron_gorefiendAI (_Creature);
}

void AddSC_boss_teron_gorefiend()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_teron_gorefiend";
    newscript->GetAI = GetAI_boss_teron_gorefiend;
    m_scripts[nrscripts++] = newscript;
}

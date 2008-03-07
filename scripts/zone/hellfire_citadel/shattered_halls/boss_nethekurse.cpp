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

/* ScriptData
SDName: Boss_Nethekurse
SD%Complete: 90
SDComment: Using wrong death coil spell
EndScriptData */

#include "../../../sc_defines.h"

#define SPELL_DEATH_COIL    30500 // fear you only 2 meters in the air (core bug but better than fear under the map)
#define SPELL_DARK_SPIN     30502 // core bug spell attack caster :D
#define SPELL_SHADOW_FISSURE        30496 //Summon the ShadowFissure NPC 
#define SPELL_CONSUMPTION        30497 // 100% right    

#define SOUND_TAUNT_1    10259
#define SAY_TAUNT_1        "Make for your pittyfull life!"
#define SOUND_TAUNT_2    10260
#define SAY_TAUNT_2        ""
#define SOUND_TAUNT_3    10261
#define SAY_TAUNT_3        "Your pain amuses me."
#define SOUND_AGGRO_1    10271    
#define SAY_AGGRO_1        "I'm already bored."
#define SOUND_AGGRO_2    10272
#define SAY_AGGRO_2        "Come on! ... Show me a real fight."
#define SOUND_AGGRO_3    10273
#define SAY_AGGRO_3        "I had more fun torturing the peons."
#define SOUND_SLAY_1    10274
#define SAY_SLAY_1        "You Loose."
#define SOUND_SLAY_2    10275    
#define SAY_SLAY_2        "Ohh! Just die."
#define SOUND_DIE        10276
#define SAY_DIE            "What a ... a shame."

/* // Sounds for Killing Servants (StartEvent)
10262,"A_HELL_Neth_PlyrAttack01"
10263,"A_HELL_Neth_AttckSrv01"
10264,"A_HELL_Neth_AttckSrv02"
10265,"A_HELL_Neth_AttckSrv03"
10266,"A_HELL_Neth_AttckSrv04"
10267,"A_HELL_Neth_KillSrv01"
10268,"A_HELL_Neth_KillSrv02"
10269,"A_HELL_Neth_KillSrv03"
10270,"A_HELL_Neth_KillSrvLst01"
*/

struct MANGOS_DLL_DECL boss_grand_warlock_nethekurseAI : public ScriptedAI
{
    boss_grand_warlock_nethekurseAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();} 

    uint32 deathcoil_timer;
    uint32 shadowfissure_timer;
    bool onlyonce;

    bool InCombat;
    bool HasTaunted;

    void EnterEvadeMode()
    {   
        m_creature->RemoveAllAuras(); 
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        InCombat = false;

        deathcoil_timer = 20000;
        shadowfissure_timer = 8000;
        HasTaunted = false;
        onlyonce = false;
    }

    void JustDied(Unit* Killer)
    { 
        DoPlaySoundToSet(m_creature,SOUND_DIE);
        DoYell(SAY_DIE,LANG_UNIVERSAL,NULL);
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%2)
            return;

        switch(rand()%2)
        {        
        case 0:
            DoPlaySoundToSet(m_creature,SOUND_SLAY_1);
            DoYell(SAY_SLAY_1,LANG_UNIVERSAL,NULL);
            break;

        case 1:
            DoPlaySoundToSet(m_creature,SOUND_SLAY_2);
            DoYell(SAY_SLAY_2,LANG_UNIVERSAL,NULL);
            break;        
        }
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                switch(rand()%3)
                {
                case 0:
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                    DoYell(SAY_AGGRO_1,LANG_UNIVERSAL,NULL);

                    break;

                case 1:
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                    DoYell(SAY_AGGRO_2,LANG_UNIVERSAL,NULL);

                    break; 

                case 2:
                    DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                    DoYell(SAY_AGGRO_3,LANG_UNIVERSAL,NULL);

                    break;                    
                }
                InCombat = true;
            }
        }
    }

    void MoveInLineOfSight(Unit *who)
    {  
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH); 
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    switch(rand()%3)
                    {
                    case 0:
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                        DoYell(SAY_AGGRO_1,LANG_UNIVERSAL,NULL);
                        break;

                    case 1:
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                        DoYell(SAY_AGGRO_2,LANG_UNIVERSAL,NULL);
                        break;

                    case 2:
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                        DoYell(SAY_AGGRO_3,LANG_UNIVERSAL,NULL);
                        break;
                    }
                    InCombat = true;
                }
            }
            else if (!HasTaunted && m_creature->IsWithinDistInMap(who, 60.0f))
            {
                switch(rand()%3)
                {
                case 0:
                    DoPlaySoundToSet(m_creature,SOUND_TAUNT_1);
                    DoYell(SAY_TAUNT_1,LANG_UNIVERSAL,NULL);
                    break;

                case 1:
                    DoPlaySoundToSet(m_creature,SOUND_TAUNT_2);
                    DoYell(SAY_TAUNT_2,LANG_UNIVERSAL,NULL);
                    break;

                case 2:
                    DoPlaySoundToSet(m_creature,SOUND_TAUNT_3);
                    DoYell(SAY_TAUNT_3,LANG_UNIVERSAL,NULL);
                    break;
                }
                HasTaunted = true;
            }
        }
    }  

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget())
            return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {
            if(m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                if((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() > 20)
                {
                    if(deathcoil_timer < diff)
                    {
                        Unit* target = NULL;
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        DoCast(target,SPELL_DEATH_COIL);

                        deathcoil_timer = 10000+rand()%5000;
                    }else deathcoil_timer -= diff;


                    if(shadowfissure_timer < diff)
                    {
                        Unit* target = NULL;
                        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        DoCast(target,SPELL_SHADOW_FISSURE);    
                        shadowfissure_timer = 8000;
                    }else shadowfissure_timer -= diff;


                    if( m_creature->isAttackReady() )
                    {
                        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        m_creature->AttackerStateUpdate(m_creature->getVictim());
                        m_creature->resetAttackTimer();
                    }

                }
                if((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() < 20 && !onlyonce)
                {
                    DoCast(m_creature,SPELL_DARK_SPIN);
                    onlyonce = true;
                }                
            }
        }
    }
};

struct MANGOS_DLL_DECL mob_shadowfissureAI : public ScriptedAI
{
    mob_shadowfissureAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();} 

    bool start;
    uint32 stop_timer;

    void EnterEvadeMode()
    {   
        start = false;
        stop_timer = 26000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!start)
        {
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS,0);
            m_creature->setFaction(45);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            start = true;
            DoCast(m_creature,SPELL_CONSUMPTION);            
        }
        if(stop_timer < diff)
        {
            m_creature->RemoveFromWorld();
        }else (stop_timer -= diff);
    }
};

CreatureAI* GetAI_boss_grand_warlock_nethekurseAI(Creature *_Creature)
{
    return new boss_grand_warlock_nethekurseAI (_Creature);
}

CreatureAI* GetAI_mob_shadowfissureAI(Creature *_Creature)
{
    return new mob_shadowfissureAI (_Creature);
}

void AddSC_boss_grand_warlock_nethekurse()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_grand_warlock_nethekurse";
    newscript->GetAI = GetAI_boss_grand_warlock_nethekurseAI;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_nethe_shadowfissure";
    newscript->GetAI = GetAI_mob_shadowfissureAI;
    m_scripts[nrscripts++] = newscript;
}

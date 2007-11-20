/* Copyright (C) 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Magtheridon
SD%Complete: 60
SDComment: In Development
EndScriptData */

#include "../../../sc_defines.h"
#include "../../../../../../game/Player.h"
#include "../../../../../../game/Map.h"
#include "../../../../../../game/GameObject.h"

//Spells
#define SPELL_QUAKE                 30571
#define SPELL_BLASTNOVA             30616        

#define SPELL_CLEAVE                29561

#define SPELL_BERSERK               27680
#define SPELL_BANISH                40825

#define SPELL_DEBRIS                30631
#define SPELL_CAMERA_SHAKE          36455

//Dialog
#define SAY_AGGRO                   "Thank you for releasing me. Now...die!"
#define SOUND_AGGRO                 10254

#define SAY_BANISH                  "Not again...NOT AGAIN!"
#define SOUND_BANISH                10256

#define SAY_FREED                   "I...am...UNLEASHED!!!"
#define SOUND_FREED                 10253

#define SAY_CHAMBER_DESTROY         "I will not be taken so easily. Let the walls of this prison tremble...and FALL!!!"
#define SOUND_CHAMBER_DESTROY       10257

#define SAY_PLAYER_KILLED           "Did you think me weak? Soft? Who is the weak one now?!"
#define SOUND_PLAYER_KILLED         10255

#define SAY_DEATH                   "The Legion...will consume you...all...."
#define SOUND_DEATH                 10258 

#define EMOTE_BERSERK               "becomes enraged!"

//Spawned objects
#define SPELL_COLLAPSE              34233   //This spell casted by the "cave in" type object

#define SPELL_CONFLAGERATION        35840       //Actually casted by a creature or object spawned on the ground

//Cubes
#define SPELL_MIND_EXHAUSTIOIN      30509   //Casted by the cubes when channeling ends

//Channeler spells
//#define MOB_HELLFIRE_CHANNELLER    17256

#define SPELL_SOUL_TRANSFER         30531
#define SPELL_SHADOW_BOLT_VOLLEY    39175
#define SPELL_DARK_MENDING          30528
#define SPELL_HELLFIRE_CHANNELING   31059
#define SPELL_HELLFIRE_CAST_VISUAL  24207
#define SPELL_FEAR                  39176

#define SPELL_BURNING_ABYSSAL       30511

/*

// Unkown sounds

ProgressSounds[0] = 10247;
ProgressSounds[1] = 10248;
ProgressSounds[2] = 10249;
ProgressSounds[3] = 10250;
ProgressSounds[4] = 10251;
ProgressSounds[5] = 10252;
*/

struct MANGOS_DLL_DECL boss_magtheridonAI : public ScriptedAI
{
    boss_magtheridonAI(Creature *c) : ScriptedAI(c) 
    {
        EnterEvadeMode();
    }

    uint32 Phase1_Timer;
    uint32 Cleave_Timer;
    uint32 BlastNova_Timer;
    uint32 Quake_Timer;
    uint32 QuakePhase;
    uint32 Collapse_Timer;
    uint32 Berserk_Timer;

    bool Phase3;

    void EnterEvadeMode()
    {
        Phase1_Timer = 1000;      //120000 - 2 minutes
        Cleave_Timer = 15000;
        Berserk_Timer = 1200000;     //20 minutes
        BlastNova_Timer = 60000;
        Quake_Timer = 40000;
        QuakePhase = 0;
        Collapse_Timer = 0;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();

        //m_creature->setFaction(35);
        //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void KilledUnit(Unit* victim)
    {
        DoYell(SAY_PLAYER_KILLED, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_PLAYER_KILLED);
    }

    void JustDied(Unit* Killer)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (Phase1_Timer)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (Phase1_Timer)
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        //REPLACE WITH EVENT CHECK!!!
        //if (false)
        //    return;

        //Phase timer
        if (Phase1_Timer)
            if (Phase1_Timer < diff)
            {
                m_creature->setFaction(14);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                m_creature->RemoveAllAuras();

                Phase1_Timer = 0;
            }else 
            {
                Phase1_Timer -= diff;
                return;
            }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //Berserk_Timer
        if (Berserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoTextEmote(EMOTE_BERSERK, NULL);

            Berserk_Timer = 300000;
        }else Berserk_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);

            Cleave_Timer = 10000;
        }else Cleave_Timer -= diff;

        //Quake_Timer
        if (Quake_Timer < diff)
        {
            switch (QuakePhase)
            {

            case 8:
            case 9:
                QuakePhase = 0;
                Quake_Timer = 40000;
                break;

            default:
                DoCast(m_creature, SPELL_QUAKE);
                Quake_Timer = 1200;
                QuakePhase++;
                break;
            }

        }else Quake_Timer -= diff;

        //BlastNova_Timer
        if (BlastNova_Timer < diff)
        {
            //Inturrupt Quake if it is casting
            m_creature->InterruptNonMeleeSpells(false);
            if (QuakePhase)
                QuakePhase = 8;

            DoCast(m_creature, SPELL_BLASTNOVA);

            BlastNova_Timer = 40000;
        }else BlastNova_Timer -= diff;

        //Phase3 if not already enraged and below 30%
        if (!Phase3 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30)
        {
            Phase3 = true;

            DoYell(SAY_CHAMBER_DESTROY, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_CHAMBER_DESTROY);
        }

        //Melee
        DoMeleeAttackIfReady();
    }
}; 

struct MANGOS_DLL_DECL mob_hellfire_channelerAI : public ScriptedAI
{
    mob_hellfire_channelerAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 ShadowBoltVolley_Timer;
    uint32 DarkMending_Timer;
    uint32 Fear_Timer;
    uint32 Infernal_Timer;
    std::list<uint64> FriendlyList;

    bool InfernalSpawned;

    void EnterEvadeMode()
    {
        ShadowBoltVolley_Timer = 10000;
        DarkMending_Timer = 30000;
        Fear_Timer = 20000;
        Infernal_Timer = 30000;

        InfernalSpawned = false;

        FriendlyList.clear();

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();

        DoGoHome();

        // Must be scripted - don't know how.....
        //DoCast(m_creature,SPELL_HELLFIRE_CHANNELING);
        //DoCast(m_creature,SPELL_HELLFIRE_CAST_VISUAL);
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }

        return false;
    }

    void AttackStart(Unit *who)
    {
        if (!who) return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            DoStartMeleeAttack(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who) return;

        if (!m_creature->IsHostileTo(who) && !ListContains(FriendlyList, who->GetGUID()))
            FriendlyList.push_back(who->GetGUID());

        if (m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura()) who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Shadow bolt volley
        if (ShadowBoltVolley_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_SHADOW_BOLT_VOLLEY);

            ShadowBoltVolley_Timer = 10000 + (rand()%10000);
        }else ShadowBoltVolley_Timer -= diff;

        //Dark Mending
        if (DarkMending_Timer < diff)
        {
            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
            {
                //If no friends then heal self
                if (FriendlyList.empty())
                    DoCast(m_creature, SPELL_DARK_MENDING);

                //Cast Dark Mending on the target with the lowest
                //amount of HP within 30 yards (cast range)
                uint32 LowestHP = 0;
                Unit* pLowestHPTarget = NULL;
                Unit* pTemp = NULL;

                std::list<uint64>::iterator i;

                for (i = FriendlyList.begin(); i!=FriendlyList.end(); ++i)
                {
                    pTemp = Unit::GetUnit((*m_creature),(*i));
                    if (pTemp && pTemp->isAlive() && pTemp->GetHealth() < LowestHP && m_creature->GetDistance2dSq(pTemp) < 900)
                    {
                        LowestHP = pTemp->GetHealth();
                        pLowestHPTarget = pTemp;
                    }
                }

                //Cast on ourselves if we are lower then lowest hp friendly unit
                if (pLowestHPTarget && LowestHP < m_creature->GetHealth())
                    DoCast(pLowestHPTarget, SPELL_DARK_MENDING);
                else DoCast(m_creature, SPELL_DARK_MENDING);
            }

            DarkMending_Timer = 10000 + (rand() % 10000);
        }else DarkMending_Timer -= diff;

        //Fear
        if (Fear_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 1);

            if (target)
                DoCast(target,SPELL_FEAR);

            Fear_Timer = 25000 + (rand()%15000);
        }else Fear_Timer -= diff;

        //Infernal spawning
        if (!InfernalSpawned && Infernal_Timer < diff)
        {
            Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);

            if (target)
                DoCast(target, SPELL_BURNING_ABYSSAL);

            /*Creature* Infernal = m_creature->SummonCreature(CREATURE_INFERNAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);

            if (Infernal)
            {
                Infernal->setFaction(m_creature->getFaction());

                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                if (target)
                    Infernal->AI()->AttackStart(target);
            }*/

            InfernalSpawned = true;
        }else Infernal_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};

//Manticron Cube
bool GOHello_go_Manticron_Cube(Player *player, GameObject* _GO)
{
    //Can't use this here
    if (!_GO->GetMap()->GetInstanceData())
    {
        _GO->TextEmote("Mantricon Cube: NO INSTANCE", 0);
        return true;
    }

    ScriptedInstance* pInst = (ScriptedInstance*)_GO->GetMap()->GetInstanceData();

    Unit* pUnit = Unit::GetUnit(*_GO, pInst->GetData64("Magtheridon"));

    if (!pUnit || !pUnit->isAlive() || !player)
    {
        _GO->TextEmote("Mantricon Cube: NO TARGET", 0);
        return true;
    }

    player->InterruptNonMeleeSpells(false);
    //player->CastSpell(pUnit, ?, false);

    _GO->Say("Mantricon Cube Clicked", LANG_UNIVERSAL, 0);
    return true;
}

CreatureAI* GetAI_boss_magtheridon(Creature *_Creature)
{
    return new boss_magtheridonAI (_Creature);
}

CreatureAI* GetAI_mob_hellfire_channeler(Creature *_Creature)
{
    return new mob_hellfire_channelerAI (_Creature);
}

void AddSC_boss_magtheridon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_magtheridon";
    newscript->GetAI = GetAI_boss_magtheridon;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_hellfire_channeler";
    newscript->GetAI = GetAI_mob_hellfire_channeler;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="go_manticron_cube";
    newscript->pGOHello = &GOHello_go_Manticron_Cube;
    m_scripts[nrscripts++] = newscript;
}
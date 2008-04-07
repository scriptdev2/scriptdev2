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
SDName: Boss_TailonKing_Ikiss
SD%Complete: 100
SDComment: 
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "sc_creature.h"
#include "def_sethekk_halls.h"

#define SPELL_BLINK                 38194
#define SPELL_ARCANE_VOLLEY         36738
#define SPELL_POLYMORPH             12826
#define SPELL_ARCANE_EXPLOSION      38197
#define SPELL_MANA_SHILD            38151

#define SAY_INTRO                   "<squawk>..Trinkets yes pretty Trinkets..<squawk>..power, great power.<squawk>..power in Trinkets..<squawk>"
#define SOUND_INTRO                 10557

#define SAY_AGGRO_1                 "You make war on Ikiss?..<squawk>"
#define SOUND_AGGRO_1               10554
#define SAY_AGGRO_2                 "Ikiss cut you pretty..<squawk>..slice you. Yes!"
#define SOUND_AGGRO_2               10555
#define SAY_AGGRO_3                 "No escape for..<squawk>..for you"
#define SOUND_AGGRO_3               10556

#define SAY_SLAY_1                  "You die..<squawk>..stay away from Trinkets"
#define SOUND_SLAY_1                10558
#define SAY_SLAY_2                  "<squawk>"
#define SOUND_SLAY_2                10559

#define SAY_DEATH                   "Ikiss will not..<squawk>..die"
#define SOUND_DEATH                 10560

struct MANGOS_DLL_DECL boss_tailonking_ikissAI : public ScriptedAI
{
    boss_tailonking_ikissAI(Creature *c) : ScriptedAI(c) 
    {
        Reset();
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
    }   

    ScriptedInstance* pInstance;

    uint32 arcanevolley_timer;
    uint32 sheep_timer;
    uint32 blink_timer;
    uint32 wait_timer;
    uint32 manashild_timer;
    uint32 sheep;

    bool manashild;
    bool blink;
    bool wait;
    bool intro;

    bool InCombat;

    void Reset()
    {   
        arcanevolley_timer = 5000; 
        sheep_timer = 7000;
        blink_timer = 1000;
        blink = false;
        wait = false;
        intro = false;
        sheep = 0;
        InCombat = false;
        manashild = false;
    }

    void JustDied(Unit* Killer)
    { 
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);

        if(pInstance)
            pInstance->SetData(DATA_IKISSDOOREVENT, 1);
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%2)
            return;

        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_SLAY_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_1);
            break;

        case 1:
            DoYell(SAY_SLAY_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_2);
            break;        
        }
    }

    void Aggro(Unit *who)
    {
       
                switch(rand()%3)
                {
                    case 0:
                        DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
                        break;

                    case 1:
                        DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
                        break; 

                    case 2:
                        DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                        DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
                        break;
                }
                manashild_timer = 15000+rand()%20000; // I dont now when he is casting that so totalrandom in fight (casted once);
                
    }

    void MoveInLineOfSight(Unit *who)
    {  
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who) && !wait)
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH); 
                DoStartMeleeAttack(who);

                Aggro(who);
            }else
            {
                if(!intro)
                {
                    intro=true;
                    DoYell(SAY_INTRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_INTRO);
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
        
        if(!wait)
        {   
            if(!manashild)
                if(manashild_timer < diff)
                {     
                        DoCast(m_creature,SPELL_MANA_SHILD);
                        manashild = true;
                }else manashild_timer -= diff;

            if(arcanevolley_timer < diff)
            {
                DoCast(m_creature, SPELL_ARCANE_VOLLEY);
                arcanevolley_timer = 3000+ rand()%8000;
            }else arcanevolley_timer -= diff;
        
            if(sheep < 2)
            {
                if(sheep_timer < diff)
                {     
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if(target)
                    {
                        DoCast(target,SPELL_POLYMORPH);
                        sheep_timer = 5000;
                        sheep++;
                    }
                    if(sheep == 2) blink_timer = 5000; 
                }else sheep_timer -= diff;
            }
            
            DoMeleeAttackIfReady();
                       
            if(sheep == 2 && !blink)
            {
                if(blink_timer < diff)
                {     
                    Unit* target = NULL;
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    if(target)
                    {
                        //THIS SPELL DOESNT WORK
                        //if(target != m_creature->getVictim()) DoCast(target,SPELL_BLINK);
                        float X = target->GetPositionX();
                        float Y = target->GetPositionY();
                        float Z = target->GetPositionZ();
                        m_creature->Relocate(X,Y,Z);
                        m_creature->SendMonsterMove(X, Y, Z, 0, true, 1000);
                        blink = true;
                    }
                }else blink_timer -= diff;
            }
                    
            if(blink)
            {
                wait = true;
                wait_timer = 5000;
                DoCast(m_creature,SPELL_ARCANE_EXPLOSION);
                wait_timer = 5100;
                blink = false;
            }             
        }
        
        if(wait)
        {
            if (wait_timer < diff)
            {
                sheep = 0;
                sheep_timer= 5000;
                wait = false;
            }
            else
            {
                wait_timer -= diff;
                //m_creature->StopMoving();
            }
        }
    }

};

CreatureAI* GetAI_boss_tailonking_ikissAI(Creature *_Creature)
{
    return new boss_tailonking_ikissAI (_Creature);
}

void AddSC_boss_tailonking_ikiss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_tailonking_ikiss";
    newscript->GetAI = GetAI_boss_tailonking_ikissAI;
    m_scripts[nrscripts++] = newscript;
}

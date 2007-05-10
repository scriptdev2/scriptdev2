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

//*** NEEDS ADDITIOINAL REVIEW ***

#include "../../sc_defines.h"

#define SPELL_ESSENCEOFTHERED       23513
#define SPELL_FLAMEBREATH           18435       //Wrong spell. Damage is right but it should not proc the heated blade.
#define SPELL_FIRENOVA              23462
#define SPELL_TAILSWIPE             15847
#define SPELL_BURNINGADRENALINE     18173
#define SPELL_CLEAVE                20691       //Chain cleave is most likely named something different and contains a dummy effect

#define SAY_LINE1           "Too late...friends. Nefarius' corruption has taken hold. I cannot...control myself. "
#define SOUND_LINE1         8281

#define SAY_LINE2           "I beg you Mortals, flee! Flee before I lose all control. The Black Fire rages within my heart. I must release it!"
#define SOUND_LINE2         8282

#define SAY_LINE3           "FLAME! DEATH! DESTRUCTION! COWER MORTALS BEFORE THE WRATH OF LORD....NO! I MUST FIGHT THIS!"
#define SOUND_LINE3         8283

#define SAY_HALFLIFE        "Nefarius' hate has made me stronger than ever before. You should have fled, while you could, mortals! The fury of Blackrock courses through my veins! "
#define SOUND_HALFLIFE      8285

#define SAY_KILLTARGET      "Forgive me $N, your death only adds to my failure."
#define SOUND_KILLTARGET    8284

#define GOSSIP_ITEM         "Start Event <Needs Gossip Text>"

struct MANGOS_DLL_DECL boss_vaelAI : public ScriptedAI
{
    boss_vaelAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    Unit *PlayerHolder;
    uint32 SpeachTimer;
    uint32 SpeachNum;
    uint32 Cleave_Timer;
    uint32 FlameBreath_Timer;
    uint32 FireNova_Timer;
    uint32 BurningAdrenalineCaster_Timer;
    uint32 BurningAdrenalineTank_Timer;
    uint32 TailSwipe_Timer;
    bool HasYelled;
    bool DoingSpeach;
    bool InCombat;

    void EnterEvadeMode()
    {
        PlayerHolder = NULL;
        SpeachTimer = NULL;
        SpeachNum = 0;
        Cleave_Timer = 15000;      //These times are probably wrong
        FlameBreath_Timer = 25000;
        BurningAdrenalineCaster_Timer = 15000;
        BurningAdrenalineTank_Timer = 45000;
        FireNova_Timer = 5000;
        TailSwipe_Timer = 20000;
        HasYelled = false;
        DoingSpeach = false;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void BeginSpeach(Unit* target)
    {
        //Stand up and begin speach
        PlayerHolder = target;

        //10 seconds
        DoYell(SAY_LINE1,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_LINE1);
        SpeachTimer = 10000;
        SpeachNum = 0;
        DoingSpeach = true;
    }

    void KilledUnit(Unit *victim)
    {
        if (rand()%5)
            return;

        DoYell(SAY_KILLTARGET,LANG_UNIVERSAL,victim);
        DoPlaySoundToSet(m_creature,SOUND_KILLTARGET);
    }

    void AttackStart(Unit *who)
    {
        if (!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            //Say our dialog
            if (!InCombat)
            {
                DoCast(m_creature,SPELL_ESSENCEOFTHERED);
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
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE)
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                if (!InCombat)
                {
                    DoCast(m_creature,SPELL_ESSENCEOFTHERED);
                    InCombat = true;
                }

                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Speach
        if (DoingSpeach)
            if (SpeachTimer < diff)
            {
                switch (SpeachNum)
                {
                case 0:
                    //16 seconds till next line
                    DoYell(SAY_LINE2,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_LINE2);
                    SpeachTimer = 16000;
                    SpeachNum++;
                    break;

                case 1:
                    //This one is actually 16 seconds but we only go to 10 seconds because he starts attacking after he says "I must fight this!"
                    DoYell(SAY_LINE3,LANG_UNIVERSAL,NULL);
                    DoPlaySoundToSet(m_creature,SOUND_LINE3);
                    SpeachTimer = 10000;
                    SpeachNum++;
                    break;

                case 2:
                default:
                    m_creature->setFaction(103);
                    m_creature->SetHealth(int(m_creature->GetMaxHealth()*.3));
                    if (PlayerHolder)
                    {
                        DoStartMeleeAttack(PlayerHolder);
                        DoCast(m_creature,SPELL_ESSENCEOFTHERED);
                    }

                    SpeachTimer = 0;
                    DoingSpeach = false;
                    break;
                }
            }else SpeachTimer -= diff;

        
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if( m_creature->getVictim() && m_creature->isAlive())
        {
            
            // Yell if hp lower than 15%
            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 15 && !HasYelled)
            {
                //Say our dialog
                DoYell(SAY_HALFLIFE,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_HALFLIFE);
                HasYelled = true;
            }
            
            //Cleave_Timer
            if (Cleave_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);

                //25 seconds until we should cast this again
                Cleave_Timer = 15000;
            }else Cleave_Timer -= diff;

            //FlameBreath_Timer
            if (FlameBreath_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FLAMEBREATH);

                //25 seconds until we should cast this again
                FlameBreath_Timer = 25000;
            }else FlameBreath_Timer -= diff;
            
            //BurningAdrenalineCaster_Timer (NOT YET IMPLEMENTED due to the fact that we can't randomly select a target yet)
            if (BurningAdrenalineCaster_Timer < diff)
            {
                //Cast
                //DoCast(m_creature->getVictim(),SPELL_BURNINGADRENALINE);
                Unit* target = NULL;

                target = SelectUnit(SELECT_TARGET_RANDOM,0);

                if (target)
                    DoCast(target,SPELL_BURNINGADRENALINE);

                //15 seconds until we should cast this agian
                BurningAdrenalineCaster_Timer = 15000;
            }else BurningAdrenalineCaster_Timer -= diff;

            //BurningAdrenalineTank_Timer
            if (BurningAdrenalineTank_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_BURNINGADRENALINE);

                //45 seconds until we should cast this agian
                BurningAdrenalineTank_Timer = 45000;
            }else BurningAdrenalineTank_Timer -= diff;
            
            //FireNova_Timer
            if (FireNova_Timer < diff)
            {
                //Cast
                DoCast(m_creature->getVictim(),SPELL_FIRENOVA);

                //5 seconds until we should cast this again
                FireNova_Timer = 5000;
            }else FireNova_Timer -= diff;
            
            //TailSwipe_Timer
            if (TailSwipe_Timer < diff)
            {
                //Only cast if we are behind
                /*if (!m_creature->HasInArc( M_PI, m_creature->getVictim()))
                {
                    DoCast(m_creature->getVictim(),SPELL_TAILSWIPE);
                }*/

                //20 seconds until we should cast this again
                TailSwipe_Timer = 20000;
            }else TailSwipe_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DIST))
            {
                //Make sure our attack is ready and we arn't currently casting
                if( m_creature->isAttackReady() && !m_creature->m_currentSpell)
                {
                    m_creature->AttackerStateUpdate(m_creature->getVictim());
                    m_creature->resetAttackTimer();
                }
            }
        }
    }
};

void SendDefaultMenu_boss_vael(Player *player, Creature *_Creature, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)//Fight time
    {
        player->PlayerTalkClass->CloseGossip();
        ((boss_vaelAI&)_Creature->AI()).BeginSpeach((Unit*)player);
    }
}

bool GossipSelect_boss_vael(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_boss_vael(player, _Creature, action);

    return true;
}

bool GossipHello_boss_vael(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM( 0, GOSSIP_ITEM        , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->PlayerTalkClass->SendGossipMenu(907,_Creature->GetGUID());

    return true;
}

CreatureAI* GetAI_boss_vael(Creature *_Creature)
{
    return new boss_vaelAI (_Creature);
}

void AddSC_boss_vael()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_vaelastrasz";
    newscript->GetAI = GetAI_boss_vael;
    newscript->pGossipHello = &GossipHello_boss_vael;
    newscript->pGossipSelect = &GossipSelect_boss_vael;

    m_scripts[nrscripts++] = newscript;
}
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

#include "../../sc_defines.h"

#define SPELL_GROWTH            36300
#define SPELL_CAVEIN            36240
#define SPELL_GROUNDSLAM        33525 // AoE Ground Slam applying Ground Slam to everyone with a script effect (most likely the knock back, we can code it to a set knockback)
#define SPELL_SHATTER           33671
#define SPELL_HURTFULSTRIKE     33813 
#define SPELL_REVERBERATION     36297 //AoE Silence
//#define SPELL_GRONNLORDSGRASP   33572 //Already handled in GroundSlam
#define SPELL_STONED            33652 //-- Spell is self cast
#define SPELL_SHATTER           33654

#define EMOTE_GROW              "grows in size!"
#define SAY_AGGRO               "Come.... and die."

struct MANGOS_DLL_DECL boss_gruulAI : public ScriptedAI
{
    boss_gruulAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 Growth_Timer;
    uint32 GrowthCounter;
    uint32 CaveIn_Timer;
    uint32 GroundSlam_Timer;
    uint32 GroundSlamPhase;
    uint32 IsInGroundSlam;
    uint32 HurtfulStrike_Timer;
    uint32 Reverberation_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Growth_Timer= 30000;
        GrowthCounter= 0;
        CaveIn_Timer= 40000;
        GroundSlam_Timer= 35000;;
        GroundSlamPhase= 0;
        IsInGroundSlam= false;
        HurtfulStrike_Timer= 8000;
        Reverberation_Timer= 60000+45000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who != m_creature)
        {
            //Begin attack
            DoStartMeleeAttack(who);

            if (!InCombat)
            {
                DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
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

                //Begin attack
                DoStartMeleeAttack(who);

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
                    InCombat = true;
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget())
            return;

        //Check if we have a current target
        if(m_creature->getVictim() && m_creature->isAlive())
        {
            // Growth
            // Gruul can cast this spell up to 30 times
            if (Growth_Timer < diff && GrowthCounter < 30)
            {
                DoCast(m_creature,SPELL_GROWTH);
                Growth_Timer = 30000;
                GrowthCounter++;
            }else Growth_Timer -= diff;

            // Hurtful Strike
            if (HurtfulStrike_Timer < diff && !IsInGroundSlam)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);

                //Cast
                if (target && m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
                     DoCast(target,SPELL_HURTFULSTRIKE);
                else
                     DoCast(m_creature->getVictim(),SPELL_HURTFULSTRIKE); 

                /*Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);

                if (target && m_creature->IsWithinDistInMap(target, 30))
                    DoCast(target,SPELL_HURTFULSTRIKE);
                else
                {
                    // If there's no 2nd target, he casts this spell on the tank
                    DoCast(m_creature->getVictim(),SPELL_HURTFULSTRIKE);
                }*/

                HurtfulStrike_Timer= 8000;
            }else HurtfulStrike_Timer -= diff;

            // Reverberation
            if (Reverberation_Timer < diff && !IsInGroundSlam)
            {
                //Modify reverberation to cast on all creatures within 200 yards
                //On official a dummy spell does this but we don't have that luxury
                SpellEntry i =(*GetSpellStore()->LookupEntry(SPELL_REVERBERATION));
                i.EffectRadiusIndex[0] = 22;    //Index 22 = 200 yard radius
                i.rangeIndex = 13;              // Index 13 = 50,000 yard range
                m_creature->CastSpell(m_creature->getVictim(),&i ,true);

                Reverberation_Timer = 45000;
            }else Reverberation_Timer -= diff;

            // Cave In
            if (CaveIn_Timer < diff && !IsInGroundSlam)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                
                if(target)
                    DoCast(target,SPELL_CAVEIN);

                CaveIn_Timer =40000;
            }else CaveIn_Timer -= diff;

            // Ground Slam, Gronn Lord's Grasp, Stoned, Shatter
            if (GroundSlam_Timer < diff)
            {
                switch(GroundSlamPhase)
                {
                    case 0:
                    DoCast(m_creature,39258);//Root?
                    IsInGroundSlam= true;

                    // **Ground Slam Knockback**
                    // Using 24199 (knockback 350) as a base we will create the AoE knockback
                    // Note that on official this isn't actually a knock back but a directed pull twoard other players with a knockback mixed in
                    // That is currently impossible though so this should do fine

                    SpellEntry spell;
                    spell =(*GetSpellStore()->LookupEntry(24199));
                    spell.EffectRadiusIndex[0] = 22;    //Index 22 = 200 yard radius
                    spell.rangeIndex = 13;              // Index 13 = 50,000 yard range
                    spell.EffectAmplitude[0] = 75;
                    //spell.EffectImplicitTargetA[0] = TARGET_ALL_ENEMY_IN_AREA;
                    //spell.EffectImplicitTargetB[0] = TARGET_ALL_ENEMY_IN_AREA;
                    m_creature->CastSpell(m_creature->getVictim(),&spell ,true);

                    // Seconds before next phase
                    GroundSlam_Timer =2000;
                    GroundSlamPhase++;
                    break;

                    // Gronn Lord's Grasp
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    // **Ground Slam (Auto-stacking Gronn Lord's grasp**
                    // 33525 - Interesting spell. Just cast it and it will automatically
                    // Stack the gronn lords grasp on all playres but it needs to be casted every 2 seconds
                    DoCast(m_creature,33525);
                    
                    /*39187
                    SpellEntry spell;
                    spell =(*GetSpellStore()->LookupEntry(39187));
                    spell.EffectRadiusIndex[0] = 22;    //Index 22 = 200 yard radius
                    spell.rangeIndex = 13;              // Index 13 = 50,000 yard range
                    spell.EffectAmplitude[0] = 75;
                    m_creature->CastSpell(m_creature->getVictim(),&spell ,true);*/

                    // Seconds before next phase
                    GroundSlam_Timer = 2000;
                    GroundSlamPhase++;
                    break;

                    // Stoned
                    case 11:
                    // ** Stoned **
                    // This spell works fine except that it needs a radius
                    // So lets give it one

                    //SpellEntry 
                    spell =(*GetSpellStore()->LookupEntry(SPELL_STONED));
                    spell.EffectRadiusIndex[0] = 22;    //Index 22 = 200 yard radius
                    spell.rangeIndex = 13;              // Index 13 = 50,000 yard range
                    spell.EffectImplicitTargetA[0] = 22;
                    spell.EffectImplicitTargetB[0] = 22;
                    m_creature->CastSpell(m_creature->getVictim(),&spell ,true);

                    // Seconds before next phase
                    GroundSlam_Timer =8000;
                    GroundSlamPhase++;
                    break;

                    // Shatter
                    case 12:
                    // ** Shatter **
                    // There is absolutly no spell associated with Shatter
                    // So we'll make one from a shatter that does 9000 dmg
                    // We'll make this spell so it dispels stun and then
                    // Does 2000 damage to all players within a 20 yard radius
                    // That isn't quite how it works on official but close enough
                    
                    //Cast the dummy spell so boss mods work correctly
                    DoCast(m_creature,33654);

                    //Here is the spell we'll have players cast
                    //SpellEntry 
                    spell =(*GetSpellStore()->LookupEntry(33671));
                    spell.rangeIndex = 0;              // Index 0 = self only
                    spell.EffectAmplitude[0] = 2000;

                    ThreatList m_threatlist;
                    m_threatlist = m_creature->GetThreatList();

                    ThreatList::iterator  i = m_threatlist.begin();

                    //If you have to do a loop over all units in threat list do it this way
                    for (i = m_threatlist.begin(); i!= m_threatlist.end();++i)
                    {
                        Unit* pCastUnit = Unit::GetUnit((*m_creature), i->UnitGuid);
                        if (pCastUnit)
                        {
                            //Force player to cast 22890 -- Dispels all slowing and stun effects
                            pCastUnit->CastSpell(pCastUnit,22890,true);

                            //Force player to cast the AoE dmg spell
                            pCastUnit->CastSpell(pCastUnit,&spell ,true);

                        }
                    }
                    
                    // resets everything
                    IsInGroundSlam= false;
                    GroundSlam_Timer =120000;
                    GroundSlamPhase =0;
                    HurtfulStrike_Timer= 8000;
                    Reverberation_Timer = 45000;
                    CaveIn_Timer =35000;
                    break;
                }
            }else GroundSlam_Timer -= diff;

            //If we are within range melee the target
            if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
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

CreatureAI* GetAI_boss_gruul(Creature *_Creature)
{
    return new boss_gruulAI (_Creature);
}


void AddSC_boss_gruul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gruul";
    newscript->GetAI = GetAI_boss_gruul;
    m_scripts[nrscripts++] = newscript;
}

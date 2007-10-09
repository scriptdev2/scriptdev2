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

#define SAY_AGGRO           "The Menagerie is for guests only."
#define SOUND_AGGRO         9183

#define SAY_SUMMON1         "Gallery rules will be strictly enforced."
#define SOUND_SUMMON1       9188

#define SAY_SUMMON2         "This curator is equipped for gallery protection."
#define SOUND_SUMMON2       9309

#define SAY_EVOCATE         "Your request cannot be processed."
#define SOUND_EVOCATE       9186

#define SAY_ENRAGE          "Failure to comply will result in offensive action."
#define SOUND_ENRAGE        9185

#define SAY_KILL1           "Do not touch the displays."
#define SOUND_KILL1         9186

#define SAY_KILL2           "You are not a guest."
#define SOUND_KILL2         9308

#define SAY_DEATH           "This Curator is no longer op... er... ation... al."
#define SOUND_DEATH         9184

//Flare spell info
#define SPELL_ASTRAL_FLARE_PASSIVE      30234
#define SPELL_ASTRAL_FLARE_NE           30236
#define SPELL_ASTRAL_FLARE_NW           30239
#define SPELL_ASTRAL_FLARE_SE           30240
#define SPELL_ASTRAL_FLARE_SW           30241

//Curator spell info
#define SPELL_HATEFUL_BOLT              30383
#define SPELL_EVOCATION                 30254
#define SPELL_ENRAGE                    19953
#define SPELL_BERSERK                   26662


struct MANGOS_DLL_DECL boss_curatorAI : public ScriptedAI
{
    boss_curatorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 AddTimer;
    uint32 HatefulBoltTimer;
    uint32 BerserkTimer;
    uint32 phase;

    bool InCombat;

    void EnterEvadeMode()
    {
        AddTimer = 10000;
        HatefulBoltTimer = 15000; // This time is probably wrong
        BerserkTimer = 720000; //12 minutes
        phase = 1;

        InCombat = false;
        m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_ARCANE, true);
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_KILL1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(victim, SOUND_KILL1);
            break;
        case 1:
            DoYell(SAY_KILL2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(victim, SOUND_KILL2);
            break;
        case 2:
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(NULL, SOUND_DEATH);
    }

    void AttackStart(Unit *who)
    {        
        if(!who && who != m_creature)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            //Say our dialog
            if (!InCombat)
            {
                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_AGGRO);
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

                if (!InCombat)
                {
                    DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_AGGRO);
                    InCombat = true;
                }

                DoStartMeleeAttack(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(m_creature->GetPower(POWER_MANA) <= 0)
        {
            m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
            DoYell(SAY_EVOCATE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_EVOCATE);
            DoCast(m_creature, SPELL_EVOCATION);
            m_creature->DeleteThreatList();
        }

        if(phase == 1)
        {
            if(AddTimer < diff)
            {
                uint32 LocX = m_creature->GetPositionX();
                uint32 LocY = m_creature->GetPositionY();
                uint32 LocZ = m_creature->GetPositionZ();
                //Summon Astral Flare
                Creature* AstralFlare = DoSpawnCreature(17096, LocX, LocY, LocZ, 5.000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                if (AstralFlare && target)
                    AstralFlare->AI()->AttackStart(target);

                //Reduce Mana by 10%
                uint32 mana = ((m_creature->GetMaxPower(POWER_MANA) / (m_creature->GetMaxPower(POWER_MANA) / 10)) - m_creature->GetPower(POWER_MANA));                
                m_creature->SetPower(POWER_MANA, mana);
                switch(rand()%3)
                {
                case 0:
                    DoYell(SAY_SUMMON1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SUMMON1);
                    break;
                case 1:
                    DoYell(SAY_SUMMON2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SUMMON2);
                    break;
                case 2:
                    DoYell(SAY_SUMMON1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SUMMON1);
                    break;
                case 3:
                    DoYell(SAY_SUMMON2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature, SOUND_SUMMON2);
                    break;                            
                }
                AddTimer = 10000;
            }else AddTimer -= diff;

            if(HatefulBoltTimer < diff)
            {
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);
                DoCast(target, SPELL_HATEFUL_BOLT);

                HatefulBoltTimer = 15000;
            }else HatefulBoltTimer -= diff;
        }

        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 15)
        {
            phase = 2;
            DoCast(m_creature, SPELL_ENRAGE);
            DoYell(SAY_ENRAGE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
        }

        if(BerserkTimer < diff)
        {
            phase = 2;
            DoCast(m_creature, SPELL_BERSERK);
            DoYell(SAY_ENRAGE, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_ENRAGE);
        }

        DoMeleeAttackIfReady();
    }

};


CreatureAI* GetAI_boss_curator(Creature *_Creature)
{
    return new boss_curatorAI (_Creature);
}

void AddSC_boss_curator()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_curator";
    newscript->GetAI = GetAI_boss_curator;

    m_scripts[nrscripts++] = newscript;
}

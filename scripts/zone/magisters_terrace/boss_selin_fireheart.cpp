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
SDName: Boss_Selin_Fireheart
SD%Complete: 99
SDComment: Visual effects is missing and maybe better approach to come close to crystal, now he come directly to the crystal
SDCategory: Magister's Terrace
EndScriptData */

#include "def_magisters_terrace.h"
#include "GameObject.h"

#define SAY_AGGRO           "You only waste my time!"
#define SOUND_AGGRO         12378 

#define SAY_ENERGY          "My hunger knows no bounds! "
#define SOUND_ENERGY        12381 

#define SAY_EMPOWERED       "Yes! I am a god!"
#define SOUND_EMPOWERED     12382

#define SAY_KILL_1          "Enough distractions!"
#define SOUND_KILL_1        12388

#define SAY_KILL_2          "I am invincible!"
#define SOUND_KILL_2        12385
 
#define SAY_DEATH           "No! More... I must have more!"
#define SOUND_DEATH         12383

//Crystal efect spells
#define SPELL_FEL_CRYSTAL_COSMETIC        44374
#define SPELL_FEL_CRYSTAL_DUMMY           44329
#define SPELL_FEL_CRYSTAL_VISUAL          44355
#define SPELL_MANA_RAGE                   44320 // This spell triggers 44321, which changes scale and regens mana Requires an entry in spell_script_target

//Selin's spells
#define SPELL_DRAIN_LIFE                  44294
#define SPELL_FEL_EXPLOSION               44314   

#define CRYSTALS_NUMBER           5
#define DATA_CRYSTALS             6

struct MANGOS_DLL_DECL boss_selin_fireheartAI : public ScriptedAI
{
    boss_selin_fireheartAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());

        // GUIDs per instance is static, so we only need to load them once.
        if(pInstance)
        {
            for(uint8 i = 0; i < CRYSTALS_NUMBER; ++i)
                FelCrystals[i] = pInstance->GetData64(DATA_CRYSTALS+i);
        }
        Reset();
    }

    ScriptedInstance* pInstance;

    uint64 FelCrystals[CRYSTALS_NUMBER];

    uint32 CombatPulseTimer; // Use a combat pulse timer instead of calling DoZoneInCombat every diff to reduce stress on the server.
    uint32 DrainLifeTimer;
    uint32 FelExplosionTimer;
    uint32 DrainCrystalTimer;

    bool DrainingCrystal;
    uint64 CrystalGUID; // This will help us create a pointer to the crystal we are draining. We store GUIDs, never units in case unit is deleted/offline (offline if player of course).

    void Reset()
    {
        if(pInstance)
        {
            for(uint8 i = 0; i < CRYSTALS_NUMBER; ++i)
            {
                Unit* pUnit = Unit::GetUnit(*m_creature, FelCrystals[i]);
                if(pUnit)
                {
                    if(!pUnit->isAlive())
                        ((Creature*)pUnit)->Respawn(); // Let MaNGOS handle setting death state, etc.

                    // Only need to set unselectable flag. You can't attack unselectable units so non_attackable flag is not necessary here.
                    pUnit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            GameObject* Door = GameObject::GetGameObject(*m_creature, pInstance->GetData64(DATA_SELIN_ENCOUNTER_DOOR));
            if(Door)
                Door->SetGoState(1);                        // Close the door. Open it only in JustDied.

            pInstance->SetData(DATA_SELIN_EVENT, NOT_STARTED); // Set Inst data for encounter
        }else error_log(ERROR_INST_DATA);

        CombatPulseTimer = 5000;
        DrainLifeTimer = 18000;
        FelExplosionTimer = 2100;
        DrainCrystalTimer = 5000;

        DrainingCrystal = false;
        CrystalGUID = 0;
    }

    void SelectNearestCrystal()
    {
        float ShortestDistance = 0;
        CrystalGUID = 0;
        Unit* pCrystal = NULL;
        Unit* CrystalChosen = NULL;
        for(uint8 i =  0; i < CRYSTALS_NUMBER; ++i)
        {
            pCrystal = NULL;
            pCrystal = Unit::GetUnit(*m_creature, FelCrystals[i]);
            if(pCrystal && pCrystal->isAlive())
            {
                if(!ShortestDistance || (ShortestDistance > m_creature->GetDistance2d(pCrystal)))
                {
                    ShortestDistance = m_creature->GetDistance2d(pCrystal);
                    CrystalGUID = pCrystal->GetGUID();
                    CrystalChosen = pCrystal;// Store a copy of pCrystal so we don't need to recreate a pointer to closest crystal for the movement and yell.
                }
            }
        }
        if(CrystalChosen)
        {
            DoYell(SAY_ENERGY, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature, SOUND_ENERGY);
            CrystalChosen->CastSpell(CrystalChosen, SPELL_FEL_CRYSTAL_COSMETIC, true);
            
            float x, y, z; // coords that we move to, close to the crystal.
            CrystalChosen->GetClosePoint(x, y, z, m_creature->GetObjectSize(), CONTACT_DISTANCE);

            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
            m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
            DrainingCrystal = true;
        }
    }

    void ShatterRemainingCrystals()
    {
        for(uint8 i = 0; i < CRYSTALS_NUMBER; ++i)
        {
            Creature* pCrystal = ((Creature*)Unit::GetUnit(*m_creature, FelCrystals[i]));
            if(pCrystal && pCrystal->isAlive())
                pCrystal->DealDamage(pCrystal, pCrystal->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit* who)
    {
        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%2)
        {
            case 0:
                DoYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_KILL_1);
                break;
            case 1:
                DoYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_KILL_2);
                break;
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
         if(type == POINT_MOTION_TYPE && id == 1)
         {
             Unit* CrystalChosen = Unit::GetUnit(*m_creature, CrystalGUID);
             if(CrystalChosen && CrystalChosen->isAlive())
             {
                 // Make the crystal attackable
                 // We also remove NON_ATTACKABLE in case the database has it set.
                 CrystalChosen->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE);
                 CrystalChosen->CastSpell(m_creature, SPELL_MANA_RAGE, false);
             }
             else
             {
                 // Make an error message in case something weird happened here
                 error_log("SD2: Selin Fireheart unable to drain crystal as the crystal is either dead or despawned");
                 DrainingCrystal = false;
             }
         }
    }
    
    void JustDied(Unit* killer)
    {
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_DEATH);     
         
        if(!pInstance)
        {
            error_log(ERROR_INST_DATA);
            return;
        }

        pInstance->SetData(DATA_SELIN_EVENT, DONE);         // Encounter complete!
        GameObject* EncounterDoor = GameObject::GetGameObject((*m_creature), pInstance->GetData64(DATA_SELIN_ENCOUNTER_DOOR));
        if(EncounterDoor)
            EncounterDoor->SetGoState(1);                   // Open door now

        ShatterRemainingCrystals();
    }
         
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(!DrainingCrystal)
        {
            if(DrainLifeTimer < diff)
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), SPELL_DRAIN_LIFE);
                DrainLifeTimer = 11000;
            }else DrainLifeTimer -= diff;
            
            if(FelExplosionTimer < diff)
            {
                if(!m_creature->IsNonMeleeSpellCasted(false))
                {
                    DoCast(m_creature, SPELL_FEL_EXPLOSION);
                    FelExplosionTimer = 2000;
                }
            }else FelExplosionTimer -= diff;
            
            if(m_creature->GetPower(POWER_MANA)*100 / m_creature->GetMaxPower(POWER_MANA) < 10) // If below 10% mana, start recharging
                SelectNearestCrystal();
        }else
        {
            if(m_creature->GetPower(POWER_MANA) == m_creature->GetMaxPower(POWER_MANA))
            {
                DrainingCrystal = false;
                DoYell(SAY_EMPOWERED, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature, SOUND_EMPOWERED);
                Unit* CrystalChosen = Unit::GetUnit(*m_creature, CrystalGUID);
                if(CrystalChosen && CrystalChosen->isAlive())
                    CrystalChosen->DealDamage(CrystalChosen, CrystalChosen->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); // Use Deal Damage to kill it, not setDeathState.
             
                CrystalGUID = 0;
                DrainingCrystal = false;
            }
        }
        
        if(CombatPulseTimer < diff)
        {
            DoZoneInCombat();
            CombatPulseTimer = 5000;
        }else CombatPulseTimer -= diff;
        
        DoMeleeAttackIfReady(); // No need to check if we are draining crystal here, as the spell has a stun.
    }
};

CreatureAI* GetAI_boss_selin_fireheart(Creature *_Creature)
{
    return new boss_selin_fireheartAI (_Creature);
};

struct MANGOS_DLL_DECL mob_fel_crystalAI : public ScriptedAI
{
    mob_fel_crystalAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset() {}

    void Aggro(Unit* who) {}

    void JustDied(Unit* killer)
    {
        if(ScriptedInstance* pInstance = ((ScriptedInstance*)m_creature->GetInstanceData()))
        {
            Creature* Selin = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_SELIN)));
            if(Selin && Selin->isAlive())
            {
                if(((boss_selin_fireheartAI*)Selin->AI())->CrystalGUID == m_creature->GetGUID())
                    ((boss_selin_fireheartAI*)Selin->AI())->DrainingCrystal = false; // Set this to false if we are the creature that Selin is draining so his AI flows properly
            }
        }else error_log(ERROR_INST_DATA);
    }
};

CreatureAI* GetAI_mob_fel_crystal(Creature *_Creature)
{
    return new mob_fel_crystalAI (_Creature);
};

void AddSC_boss_selin_fireheart()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_selin_fireheart";
    newscript->GetAI = GetAI_boss_selin_fireheart;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_fel_crystal";
    newscript->GetAI = GetAI_mob_fel_crystal;
    m_scripts[nrscripts++] = newscript;
}

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
SDName: boss_zereketh
SD%Complete: 0
SDComment: boss_zereketh_the_unbound, mob_zerekethvoidzone
SDCategory: Tempest Keep, Arcatraz
EndScriptData */

#include "def_arcatraz.h"

#define SPELL_SEED_OF_CORRUPTION 36123
#define SPELL_SHADOW_NOVA 36127
#define SPELL_VOID_ZONE_DAMAGE 36120

/*
Unbound Void Zone [Creature] 21101
Summon Unbound Void Zone 36119
*/

#define SAY_AGGRO "Life energy to... consume."
#define SOUND_AGGRO 11250 

#define SAY_KILL_1 "This vessel...is empty."
#define SOUND_KILL_1 11251
#define SAY_KILL_2 "No... More... Life..."
#define SOUND_KILL_2 11252

#define SOUND_ZONE_1 11253
#define SAY_ZONE_1 "The shadow... will engulf you."
#define SOUND_ZONE_2 11254
#define SAY_ZONE_2 "Darkness... consumes all."

#define SAY_DEATH "The Void... beckons."
#define SOUND_DEATH 11255

struct MANGOS_DLL_DECL boss_zerekethAI : public ScriptedAI
{
    boss_zerekethAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        EnterEvadeMode();
    }

    ScriptedInstance *pInstance;
    uint32 seedofcorruption_timer;
    uint32 shadownova_timer;
    uint32 voidzone_timer;
    bool InCombat;
    bool wait;
    uint32 wait_timer;

    void Reset()
    { 
        //m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
        //m_creature->CombatStop();
        //DoGoHome();

        seedofcorruption_timer = 10000;
        shadownova_timer = 15000;
        voidzone_timer = 5000;
        InCombat = false;
        wait = false;
        wait_timer = 0;

        if(pInstance) pInstance->SetData(DATA_ZEREKETHTHEUNBOUNDEVENT,0);
    }

    void JustDied(Unit* Killer)
    { 
        DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);
        if(pInstance) pInstance->SetData(DATA_ZEREKETHTHEUNBOUNDEVENT,0);
    }

    void KilledUnit(Unit* victim)
    {
        if (rand()%2)
            return;

        switch(rand()%2)
        { 
        case 0:
            DoYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_1);
            break;

        case 1:
            DoYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_2);
            break; 
        }
    }

    void Aggro(Unit *who)
    {

                DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRO);
                if(pInstance) pInstance->SetData(DATA_ZEREKETHTHEUNBOUNDEVENT,1);

    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
        if(!wait)
        {
            if(seedofcorruption_timer < diff)
            { 
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_SEED_OF_CORRUPTION);
                seedofcorruption_timer = 15000;

                wait = true;
                wait_timer = 2000;
            }else seedofcorruption_timer -= diff;

            if(shadownova_timer < diff)
            { 
                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                DoCast(target,SPELL_SHADOW_NOVA);
                shadownova_timer = 15000;

                wait = true;
                wait_timer = 1500;
            }else shadownova_timer -= diff;

            if(voidzone_timer < diff)
            { 

                Unit* target = NULL;
                target = SelectUnit(SELECT_TARGET_RANDOM,0);
                Unit* Summoned = NULL;
                Summoned = m_creature->SummonCreature(21101,target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,20000);

                voidzone_timer = 15000;

                switch(rand()%2)
                { 
                case 0:
                    DoYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_ZONE_1);
                    break;

                case 1:
                    DoYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
                    DoPlaySoundToSet(m_creature,SOUND_ZONE_2);
                    break; 
                }

            }else voidzone_timer -= diff;

            DoMeleeAttackIfReady();
        }

        if(wait && wait_timer <= diff)
        {
            wait = false;
        }else wait_timer -= diff;
    }
};

struct MANGOS_DLL_DECL mob_zerekethvoidzoneAI : public ScriptedAI
{
    mob_zerekethvoidzoneAI(Creature *c) : ScriptedAI(c) {Reset();} 

    void Reset()
    { 
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS,0);
        m_creature->setFaction(16);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        DoCast(m_creature,SPELL_VOID_ZONE_DAMAGE); 

    }

    void Aggro(Unit* who)
    {
    }
};

CreatureAI* GetAI_boss_zerekethAI(Creature *_Creature)
{
    return new boss_zerekethAI (_Creature);
}

CreatureAI* GetAI_mob_zerekethvoidzoneAI(Creature *_Creature)
{
    return new mob_zerekethvoidzoneAI (_Creature);
}

void AddSC_boss_zereketh()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_zereketh_the_unbound";
    newscript->GetAI = GetAI_boss_zerekethAI;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_zerekethvoidzone";
    newscript->GetAI = GetAI_mob_zerekethvoidzoneAI;
    m_scripts[nrscripts++] = newscript;
}

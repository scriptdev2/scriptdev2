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
SDName: Boss_kazrogal
SD%Complete: 85
SDComment: Untested
SDCategory: Caverns Of Time, Mount Hyjal
EndScriptData */

#include "def_hyjal.h"

#define SPELL_MARK          31447
#define SPELL_WARSTOMP      38911
#define SPELL_CLEAVE        30619

#define SAY_AGGRO           "Cry for mercy! Your meaningless lives will soon be forfeit."
#define SOUND_AGGRO         11015
#define SAY_MARK1           "Your death will be a painful one."
#define SOUND_MARK1         11016
#define SAY_MARK2           "You... are marked."
#define SOUND_MARK2         11052
#define SAY_SLAY1           "Shaza'kiel"
#define SOUND_SLAY1         11017
#define SAY_SLAY2           "Miserable nuisance!"
#define SOUND_SLAY2         11054
#define SAY_SLAY3           "You... are nothing!"
#define SOUND_SLAY3         11053
#define SOUND_DEATH         11018 // no real text since he laughs...

struct MANGOS_DLL_DECL boss_kazrogalAI : public ScriptedAI
{
    boss_kazrogalAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 MarkTimer;
    uint32 WarstompTimer;
    uint32 CleaveTimer;

    void Reset()
    {
        MarkTimer = 50000;
        WarstompTimer = 30000;
        CleaveTimer = 10000;

        if(pInstance)
            pInstance->SetData(DATA_KAZROGALEVENT, NOT_STARTED);
    }

    void Aggro(Unit* who)
    {
        DoYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_AGGRO);

        if(pInstance)
            pInstance->SetData(DATA_KAZROGALEVENT, IN_PROGRESS);

        DoZoneInCombat();
    }

    void JustDied(Unit* killer)
    {
        DoPlaySoundToSet(m_creature, SOUND_DEATH);

        if(pInstance)
            pInstance->SetData(DATA_KAZROGALEVENT, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
           case 0:
               DoYell(SAY_SLAY1, LANG_UNIVERSAL, NULL);
               DoPlaySoundToSet(m_creature, SOUND_SLAY1);
               break;
           case 1:
               DoYell(SAY_SLAY2, LANG_UNIVERSAL, NULL);
               DoPlaySoundToSet(m_creature, SOUND_SLAY2);
               break;
           case 2:
               DoYell(SAY_SLAY3, LANG_UNIVERSAL, NULL);
               DoPlaySoundToSet(m_creature, SOUND_SLAY3);
               break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(MarkTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_MARK);
            switch(rand()%2)
            {
               case 0:
                   DoYell(SAY_MARK1, LANG_UNIVERSAL, NULL);
                   DoPlaySoundToSet(m_creature, SOUND_MARK1);
                   break;
               case 1:
                   DoYell(SAY_MARK2, LANG_UNIVERSAL, NULL);
                   DoPlaySoundToSet(m_creature, SOUND_MARK2);
                   break;
            }
            MarkTimer = 60000;
        }else MarkTimer -= diff;

        if(WarstompTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_WARSTOMP);
            WarstompTimer = 30000;
        }else WarstompTimer -= diff;

        if(CleaveTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            CleaveTimer = 10000 + rand()%5000;
        }else CleaveTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kazrogal(Creature *_Creature)
{
    return new boss_kazrogalAI(_Creature);
};

void AddSC_boss_kazrogal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kazrogal";
    newscript->GetAI = GetAI_boss_kazrogal;
    m_scripts[nrscripts++] = newscript;
}

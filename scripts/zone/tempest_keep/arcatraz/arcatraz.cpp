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
SDName: Arcatraz
SD%Complete: 50
SDComment: Warden Mellichar, event controller for Skyriss event. Millhouse Manastorm. TODO: coordinate speech and make combat AI for Millhouse + many details. mob_zerekethvoidzone to be removed later(database)
SDCategory: Tempest Keep, The Arcatraz
EndScriptData */

/* ContentData
npc_millhouse_manastorm
npc_warden_mellichar
mob_zerekethvoidzone
EndContentData */

#include "def_arcatraz.h"

/*#####
# npc_millhouse_manastorm
#####*/

#define SAY_INTRO_1     "Where in Bonzo's brass buttons am I? And who are-- yaaghh, that's one mother of a headache!"
#define SOUND_INTRO_1   11171
#define SAY_INTRO_2     "Lowly? Nobody refers to the mighty Millhouse Manastorm as lowly! I have no idea what goes on here, but I will gladly join your fight against this impudent imbecile! Prepare to defend yourself!"
#define SOUND_INTRO_2   11172

#define SAY_WATER       "I just need to get some things ready first. You guys go ahead and get started."
#define SOUND_WATER     11173

#define SAY_BUFFS       "Fantastic! Next, some protective spells. Yes! Now we're cookin'"
#define SOUND_BUFFS     11174

#define SAY_DRINK       "And of course i'll need some mana. You guys are gonna love this, just wait."
#define SOUND_DRINK     11175

#define SAY_READY       "Aaalllriiiight!! Who ordered up an extra large can of whoop-ass?"
#define SOUND_READY     11176

#define SAY_AGGRO       "Bear witness to the agent of your demise!"
#define SOUND_AGGRO     11123

#define SAY_KILL_1      "I didn't even break a sweat on that one."
#define SOUND_KILL_1    11177
#define SAY_KILL_2      "You guys, feel free to jump in anytime."
#define SOUND_KILL_2    11178

#define SAY_PYRO        "I'm gonna light you up, sweet cheeks!"
#define SOUND_PYRO      11179

#define SAY_ICEBLOCK    "Ice, ice, baby!"
#define SOUND_ICEBLOCK  11180

#define SAY_LOWHP       "Heal me! Oh, for the love of all that is holy, HEAL me! I'm dying!"
#define SOUND_LOWHP     11181

#define SAY_DEATH       "You'll be hearing from my lawyer..."
#define SOUND_DEATH     11182

#define SAY_COMPLETE    "Who's bad? Who's bad? That's right: we bad!"
#define SOUND_COMPLETE  11183

struct MANGOS_DLL_DECL npc_millhouse_manastormAI : public ScriptedAI
{
    npc_millhouse_manastormAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 EventProgress_Timer;
    uint32 Phase;
    bool Init;
    bool LowHp;

    void Reset()
    {
        EventProgress_Timer = 5000;
        LowHp = false;
        Init = false;
        Phase = 1;

        if( pInstance )
            if( pInstance->GetData(TYPE_WARDEN_2) == DONE )
                Init = true;
    }

    void Aggro(Unit *who)
    {
        DoYell(SAY_AGGRO,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_AGGRO);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%2)
        {
        case 0:
            DoYell(SAY_KILL_1,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_1);
            break;
        case 1:
            DoYell(SAY_KILL_2,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_KILL_2);
            break;
        }
    }

    void JustDied(Unit *victim)
    {
        DoYell(SAY_DEATH,LANG_UNIVERSAL,0);
        DoPlaySoundToSet(m_creature,SOUND_DEATH);

        /*for questId 10886 (heroic mode only)
        if( pInstance && pInstance->GetData(TYPE_HARBINGERSKYRISS) != DONE )
            ->FailQuest();*/
    }

    void UpdateAI(const uint32 diff)
    {
        if( !Init )
            if( EventProgress_Timer < diff )
            {
                if( Phase < 8 )
                {
                    switch( Phase )
                    {
                        case 1:
                            DoYell(SAY_INTRO_1,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature,SOUND_INTRO_1);
                            break;
                        case 2:
                            DoYell(SAY_INTRO_2,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature,SOUND_INTRO_2);
                            break;
                        case 3:
                            DoYell(SAY_WATER,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature,SOUND_WATER);
                            break;
                        case 4:
                            DoYell(SAY_BUFFS,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature,SOUND_BUFFS);
                            break;
                        case 5:
                            DoYell(SAY_DRINK,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature,SOUND_DRINK);
                            break;
                        case 6:
                            DoYell(SAY_READY,LANG_UNIVERSAL,NULL);
                            DoPlaySoundToSet(m_creature,SOUND_READY);
                            break;
                        case 7:
                            if( pInstance )
                                pInstance->SetData(TYPE_WARDEN_2,DONE);
                            Init = true;
                            break;
                    }
                    Phase = Phase++;
                    EventProgress_Timer = 15000;
                }
            } else
                EventProgress_Timer -= diff;

        if( !m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if( !LowHp && ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20) )
        {
            DoYell(SAY_LOWHP,LANG_UNIVERSAL,NULL);
            DoPlaySoundToSet(m_creature,SOUND_LOWHP);
            LowHp = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_millhouse_manastorm(Creature *_Creature)
{
    return new npc_millhouse_manastormAI (_Creature);
}

/*#####
# npc_warden_mellichar
#####*/

#define YELL_INTRO1         "I knew the prince would be angry but, I... I have not been myself. I had to let them out! The great one speaks to me, you see. Wait--outsiders. Kael'thas did not send you! Good... I'll just tell the prince you released the prisoners!"
#define SOUND_INTRO1        11222

#define YELL_INTRO2         "The naaru kept some of the most dangerous beings in existence here in these cells. Let me introduce you to another...."
#define SOUND_INTRO2        11223

#define YELL_RELEASE1       "Yes, yes... another! Your will is mine!"
#define SOUND_RELEASE1      11224

#define YELL_RELEASE2A      "Behold another terrifying creature of incomprehensible power!"
#define SOUND_RELEASE2A     11225
#define YELL_RELEASE2B      "What is this? A lowly gnome? I will do better, O'great one."
#define SOUND_RELEASE2B     11226

#define YELL_RELEASE3       "Anarchy! Bedlam! Oh, you are so wise! Yes, I see it now, of course!"
#define SOUND_RELEASE3      11227

#define YELL_RELEASE4       "One final cell remains. Yes, O'great one, right away!"
#define SOUND_RELEASE4      11228

#define YELL_WELCOME        "Welcome, O'great one. I am your humble servant."
#define SOUND_WELCOME       11229

//phase 2(acid mobs)
#define ENTRY_TRICKSTER     20905
#define ENTRY_PH_HUNTER     20906
//phase 3
#define ENTRY_MILLHOUSE     20977
//phase 4(acid mobs)
#define ENTRY_AKKIRIS       20908
#define ENTRY_SULFURON      20909
//phase 5(acid mobs)
#define ENTRY_TW_DRAK       20910
#define ENTRY_BL_DRAK       20911
//phase 6
#define ENTRY_SKYRISS       20912

//TARGET_SCRIPT
#define SPELL_TARGET_ALPHA  36856
#define SPELL_TARGET_BETA   36854
#define SPELL_TARGET_DELTA  36857
#define SPELL_TARGET_GAMMA  36858
#define SPELL_TARGET_OMEGA  36852
#define SPELL_BUBBLE_VISUAL 36849

struct MANGOS_DLL_DECL npc_warden_mellicharAI : public ScriptedAI
{
    npc_warden_mellicharAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    bool IsRunning;
    bool CanSpawn;

    uint32 EventProgress_Timer;
    uint32 Phase;

    void Reset()
    {
        IsRunning = false;
        CanSpawn = false;

        EventProgress_Timer = 20000;
        Phase = 1;

        m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature,SPELL_TARGET_OMEGA);
        if( pInstance ) pInstance->SetData(TYPE_HARBINGERSKYRISS,NOT_STARTED);
    }

    void AttackStart(Unit* who) { return; }

    void MoveInLineOfSight(Unit *who)
    {
        if( IsRunning )
            return;

        if( !m_creature->getVictim() && who->isTargetableForAttack() && ( m_creature->IsHostileTo( who )) && who->isInAccessablePlaceFor(m_creature) )
        {
            if (m_creature->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                return;
            if (who->GetTypeId() != TYPEID_PLAYER)
                return;

            float attackRadius = m_creature->GetAttackDistance(who)/10;
            if( m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->IsWithinLOSInMap(who) )
                Aggro(who);
        }
    }

    void Aggro(Unit *who)
    {
        DoYell(YELL_INTRO1,LANG_UNIVERSAL,NULL);
        DoPlaySoundToSet(m_creature,SOUND_INTRO1);
        //possibly wrong spell OR should also cast second spell to make bubble appear (visual for this spell appear to be the correct)
        DoCast(m_creature,SPELL_BUBBLE_VISUAL);

        if( pInstance )
        {
            pInstance->SetData(TYPE_HARBINGERSKYRISS,IN_PROGRESS);
            IsRunning = true;
        }
    }

    uint32 CanProgress()
    {
        if( pInstance )
        {
            if( Phase == 6 && pInstance->GetData(TYPE_WARDEN_4) == DONE )
                return true;
            if( Phase == 5 && pInstance->GetData(TYPE_WARDEN_3) == DONE )
                return true;
            if( Phase == 4 && pInstance->GetData(TYPE_WARDEN_2) == DONE )
                return true;
            if( Phase == 3 && pInstance->GetData(TYPE_WARDEN_1) == DONE )
                return true;
            if( Phase == 2 && pInstance->GetData(TYPE_HARBINGERSKYRISS) == IN_PROGRESS )
                return true;
            if( Phase == 1 && pInstance->GetData(TYPE_HARBINGERSKYRISS) == IN_PROGRESS )
                return true;
            return false;
        }
        return false;
    }

    void DoUpdate()
    {
        CanSpawn = true;
    }

    void DoPrepareForPhase()
    {
        if( pInstance )
        {
            m_creature->InterruptNonMeleeSpells(true);
            m_creature->RemoveSpellsCausingAura(SPELL_AURA_DUMMY);

            switch( Phase )
            {
                case 2:
                    DoCast(m_creature,SPELL_TARGET_ALPHA);
                    pInstance->SetData(TYPE_WARDEN_1,IN_PROGRESS);
                    DoUpdate();
                    break;
                case 3:
                    DoCast(m_creature,SPELL_TARGET_BETA);
                    pInstance->SetData(TYPE_WARDEN_2,IN_PROGRESS);
                    DoUpdate();
                    break;
                case 4:
                    DoCast(m_creature,SPELL_TARGET_DELTA);
                    pInstance->SetData(TYPE_WARDEN_3,IN_PROGRESS);
                    DoUpdate();
                    break;
                case 5:
                    DoCast(m_creature,SPELL_TARGET_GAMMA);
                    pInstance->SetData(TYPE_WARDEN_4,IN_PROGRESS);
                    DoUpdate();
                    break;
                case 6:
                    pInstance->SetData(TYPE_WARDEN_5,IN_PROGRESS);
                    DoUpdate();
                    break;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if( !IsRunning )
            return;

        if( EventProgress_Timer < diff )
        {
            if( pInstance )
                if( pInstance->GetData(TYPE_HARBINGERSKYRISS) == FAIL )
                    Reset();

            if( CanSpawn )
            {
                //continue beam omega pod, unless we are about to summon skyriss
                if( Phase != 6 )
                    DoCast(m_creature,36852);

                switch( Phase )
                {
                    case 2:
                        switch( rand()%2 )
                        {
                            case 0:
                                m_creature->SummonCreature(ENTRY_TRICKSTER,478.326,-148.505,42.56,3.19,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                                break;
                            case 1:
                                m_creature->SummonCreature(ENTRY_PH_HUNTER,478.326,-148.505,42.56,3.19,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                                break;
                        }
                        break;
                    case 3:
                        m_creature->SummonCreature(ENTRY_MILLHOUSE,413.292,-148.378,42.56,6.27,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,240000);
                        DoYell(YELL_RELEASE2B,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_RELEASE2B);
                        break;
                    case 4:
                        switch( rand()%2 )
                        {
                            case 0:
                                m_creature->SummonCreature(ENTRY_AKKIRIS,420.179,-174.396,42.58,0.02,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                                break;
                            case 1:
                                m_creature->SummonCreature(ENTRY_SULFURON,420.179,-174.396,42.58,0.02,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                                break;
                        }
                        break;
                    case 5:
                        switch( rand()%2 )
                        {
                            case 0:
                                m_creature->SummonCreature(ENTRY_TW_DRAK,471.795,-174.58,42.58,3.06,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                                break;
                            case 1:
                                m_creature->SummonCreature(ENTRY_BL_DRAK,471.795,-174.58,42.58,3.06,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                                break;
                        }
                        break;
                    case 6:
                        m_creature->SummonCreature(ENTRY_SKYRISS,445.763,-191.639,44.64,1.60,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                        DoYell(YELL_WELCOME,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_WELCOME);
                        //possible remove flag for skyriss to kill?
                        //m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                        break;
                }
                CanSpawn = false;
                Phase = Phase++;
            }
            if( CanProgress() )
            {
                switch( Phase )
                {
                    case 1:
                        DoYell(YELL_INTRO2,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_INTRO2);
                        Phase = Phase++;
                        break;
                    case 2:
                        DoYell(YELL_RELEASE1,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_RELEASE1);
                        DoPrepareForPhase();
                        break;
                    case 3:
                        DoYell(YELL_RELEASE2A,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_RELEASE2A);
                        DoPrepareForPhase();
                        break;
                    case 4:
                        DoYell(YELL_RELEASE3,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_RELEASE3);
                        DoPrepareForPhase();
                        break;
                    case 5:
                        DoYell(YELL_RELEASE4,LANG_UNIVERSAL,NULL);
                        DoPlaySoundToSet(m_creature,SOUND_RELEASE4);
                        DoPrepareForPhase();
                        break;
                    case 6:
                        DoPrepareForPhase();
                        break;
                }
            }
            EventProgress_Timer = 15000;
        } else
            EventProgress_Timer -= diff;
    }
};
CreatureAI* GetAI_npc_warden_mellichar(Creature *_Creature)
{
    return new npc_warden_mellicharAI (_Creature);
}

/*#####
# mob_zerekethvoidzone (this script probably not needed in future -> `creature_template_addon`.`auras`='36120 0')
#####*/

#define SPELL_VOID_ZONE_DAMAGE 36120

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

    void Aggro(Unit* who) {}
};
CreatureAI* GetAI_mob_zerekethvoidzoneAI(Creature *_Creature)
{
    return new mob_zerekethvoidzoneAI (_Creature);
}

void AddSC_arcatraz()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_millhouse_manastorm";
    newscript->GetAI = GetAI_npc_millhouse_manastorm;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_warden_mellichar";
    newscript->GetAI = GetAI_npc_warden_mellichar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_zerekethvoidzone";
    newscript->GetAI = GetAI_mob_zerekethvoidzoneAI;
    m_scripts[nrscripts++] = newscript;
}

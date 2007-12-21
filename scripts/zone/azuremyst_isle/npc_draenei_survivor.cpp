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
SDName: npc_draenei_survivor
SD%Complete: 100
SDComment: For quest 9283
SDCategory: Azuremyst Isle
EndScriptData */

#include "../../sc_defines.h"
#include <cmath>

#define HEAL1        "The last thing I remember is the ship falling and us getting into the pods. I'll go see how I can help. Thank you!"
#define HEAL2        "$C, Where am I? Who are you? Oh no! What happened to the ship?."
#define HEAL3        "$C You saved me! I owe you a debt that I can never repay. I'll go see if I can help the others."
#define HEAL4        "Ugh... what is this place? Is that all that's left of the ship over there?"

#define HELP1        "Oh, the pain..."
#define HELP2        "Everything hurts, Please make it stop..."
#define HELP3        "Ughhh... I hurt. Can you help me?"
#define HELP4        "I don't know if I can make it, please help me..."

struct MANGOS_DLL_DECL draenei_survivorAI : public ScriptedAI
{
    draenei_survivorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

    uint32 UnSpawnTimer;
    uint32 ResetlifeTimer;
    uint32 SayingTimer;
    uint32 HealSayTimer;
    bool UnSpawn;
    bool say;
    bool HealSay;
    bool isRun;
    bool isMove;

    void EnterEvadeMode()
    {
        UnSpawnTimer = 2500;
        ResetlifeTimer= 60000;
        SayingTimer    = 5000;
        HealSayTimer = 6000;
        say = false;
        isRun = false;
        isMove = false;
        UnSpawn    = false;
        HealSay = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
        m_creature->CastSpell(m_creature, 29152, false, NULL); //cast red shining
        m_creature->SetHealth(int(m_creature->GetMaxHealth()*.1)); //set creature health
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 3); //ley down
    }

    void CreatureMove(float DestX, float DestY, float DestZ)
    {
        float FromX = m_creature->GetPositionX();
        float FromY = m_creature->GetPositionY();
        float FromZ = m_creature->GetPositionZ();

        float dx = DestX - FromX;
        float dy = DestY - FromY;
        float dz = DestZ - FromZ;
        double dist = ::sqrt((dx*dx) + (dy*dy) + (dz*dz));
        double speed = m_creature->GetSpeed(MOVE_RUN);

        if(speed<=0)
            speed = 2.5f;
        speed *= 0.001f;

        uint32 TotalTime = static_cast<uint32>( dist/speed + 0.5 );
        m_creature->SendMonsterMove(DestX,DestY,DestZ,0,true,TotalTime);
    }


    void AttackStart(Unit *who)
    {
        return; //ignore all attackstart commands
    }

    void MoveInLineOfSight(Unit *who) //MoveInLineOfSight is called if creature could see you, updated all 100 ms
    {
        if (!who)
            return;

        if(who->GetTypeId() == TYPEID_PLAYER && m_creature->IsFriendlyTo(who) && m_creature->IsWithinDistInMap(who, 15) && say && !isRun)
        {
            switch (rand()%4)//Random switch between 4 texts
            {
            case 0:
                DoSay(HELP1, LANG_UNIVERSAL, NULL);
                SayingTimer = 15000;
                say = false;
                break;
            case 1:
                DoSay(HELP2, LANG_UNIVERSAL, NULL);
                SayingTimer = 15000;
                say = false;
                break;
            case 2:
                DoSay(HELP3, LANG_UNIVERSAL, NULL);
                SayingTimer = 15000;
                say = false;
                break;
            case 3:
                DoSay(HELP4, LANG_UNIVERSAL, NULL);
                SayingTimer = 15000;
                say = false;
                break;
            }
        }else{ isRun = false; }
    }

    void UpdateAI(const uint32 diff)//Is also called each ms for Creature AI Updates...
    {

        if (m_creature->GetHealth() > 50)
        {
            if(ResetlifeTimer < diff)
            {
                ResetlifeTimer = 60000;
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
                m_creature->SetHealth(int(m_creature->GetMaxHealth()*.1)); //set creature health
                m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1,3); // ley down
            }
            else ResetlifeTimer -= diff;
        }

        if(HealSay)
        {
            if (HealSayTimer < diff)
            {
                UnSpawn = true;
                isRun = true;
                isMove = true;
            }else HealSayTimer -= diff;
        }

        if(UnSpawn)
        {
            if(isMove)
            {
                (*m_creature).GetMotionMaster()->Clear();
                CreatureMove(-4115.053711f,-13754.831055f,73.508949f);
                isMove = false;
            }

            if (UnSpawnTimer < diff)
            {
                m_creature->StopMoving();
                //m_creature->Despawn(0);
                EnterEvadeMode();
                m_creature->SetHealth(int(m_creature->GetMaxHealth()*.1)); //set creature health

            }else UnSpawnTimer -= diff;
        }

        if(SayingTimer < diff)
        {
            say = true;
        }else SayingTimer -= diff;
    }

    void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)//Called if you cast a spell and do some things if Specified spell is true!
    {
        if (Spellkind->Id == 28880)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
            m_creature->HandleEmoteCommand(ANIM_RISE);
            m_creature->RemoveAllAuras();
            switch (rand()%4)//This switch doesn't work at all, creature say nothing!
            {
            case 0: DoSay(HEAL1, LANG_UNIVERSAL, Hitter); break;
            case 1: DoSay(HEAL2, LANG_UNIVERSAL, Hitter); break;
            case 2: DoSay(HEAL3, LANG_UNIVERSAL, Hitter); break;
            case 3: DoSay(HEAL4, LANG_UNIVERSAL, Hitter); break;
            }
            HealSay    = true;
        }
        return;
    }
};
CreatureAI* GetAI_draenei_survivor(Creature *_Creature)
{
    return new draenei_survivorAI (_Creature);
}


void AddSC_draenei_survivor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="draenei_survivor";
    newscript->GetAI = GetAI_draenei_survivor;
    m_scripts[nrscripts++] = newscript;
}

/* 
   ########################
    RAGEFIRE CHASM ver 1.0
   ########################
by Myav

I use comments only in central situations
*/

#include "../../sc_defines.h"
#include "../../creature/simple_ai.h"

/* ##### SPELLS ##### */

#define SPELL_SHADOWBOLT          705 /* Searing Blade Warlock */
#define SPELL_POISON             2818 /* Bazzalan */
#define SPELL_SINISTER          14873 /* Bazzalan */

/* Maur Grimtotem (NPC) */ 
struct MANGOS_DLL_DECL npc_maur_grimtotemAI : public ScriptedAI
{
    npc_maur_grimtotemAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    bool InCombat;

    void EnterEvadeMode()
    {
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

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if(!InCombat)
            {
                InCombat = true;
            }
//not start melee attack
        }
    }
};
/*
Searing Blade Warlock (summon Voidwalker)
*/
struct MANGOS_DLL_DECL mob_searing_warlockAI : public ScriptedAI
{
    mob_searing_warlockAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 WarlockCast_Timer;
	uint32 SummonVoidWalker_Timer;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;

    void EnterEvadeMode()
    {
		Rand = rand()%3000 + 3000;
        WarlockCast_Timer = Rand;
		Rand = rand()%7000 + 8000;
	    SummonVoidWalker_Timer = Rand;
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

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if(!InCombat)
            {
                InCombat = true;
            }

            DoStartMeleeAttack(who);
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

                DoStartMeleeAttack(who);
            }
        }
    }
	void SummonVoidWalker(Unit* victim)
    {
        Rand = rand()%2;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%2;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(5676, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            //Summon VoidWalker
		    if (SummonVoidWalker_Timer < diff)
			{
		    Unit* target = NULL;
            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            SummonVoidWalker(target);
			Rand = rand()%15000 + 30000;
			SummonVoidWalker_Timer = Rand;
			}else SummonVoidWalker_Timer -= diff;
			//Shadowbolt
            if (WarlockCast_Timer < diff)
			{
			DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT);
			Rand = rand()%5000 + 5000;
			WarlockCast_Timer = Rand;
			}else WarlockCast_Timer -= diff;

            DoMeleeAttackIfReady();
	}
};
/*
Bazzalan (poison knife)
*/
struct MANGOS_DLL_DECL boss_bazzalanAI : public ScriptedAI
{
    boss_bazzalanAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 Poison_Timer;
	uint32 Sinister_Timer;
    int Rand;
	bool Poison;

    bool InCombat;

    void EnterEvadeMode()
    {
		Rand = rand()%5000 + 5000;
        Sinister_Timer = Rand;
		Rand = rand()%3000 + 3000;
	    Poison_Timer = Rand;
		Poison = false;
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

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            if(!InCombat)
            {
                InCombat = true;
            }

            DoStartMeleeAttack(who);
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

                DoStartMeleeAttack(who);
            }
        }
    }
	void DamageDeal(Unit *done_to, uint32 &damage)
    {
		if(Poison)
		{
		 m_creature->resetAttackTimer();
 	     DoCast(done_to,SPELL_POISON);
         Poison = false;
		}
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
		    //poison
			if(Poison_Timer < diff)
			{
			 Poison = true;
			 Rand = rand()%4000 + 4000;
             Poison_Timer = Rand;
			}else Poison_Timer -= diff;
			//Sinister
            if (Sinister_Timer < diff)
			{
			 DoCast(m_creature->getVictim(),SPELL_SINISTER);
			 Rand = rand()%5000 + 5000;
			 Sinister_Timer = Rand;
			}else Sinister_Timer -= diff;

            DoMeleeAttackIfReady();

	}
};
CreatureAI* GetAI_mob_searing_warlock(Creature *_Creature)
{
    return new mob_searing_warlockAI (_Creature);
}
CreatureAI* GetAI_npc_maur_grimtotem(Creature *_Creature)
{
    return new npc_maur_grimtotemAI (_Creature);
}
CreatureAI* GetAI_boss_bazzalan(Creature *_Creature)
{
    return new boss_bazzalanAI (_Creature);
}
CreatureAI* GetAI_boss_ragefire_chasm(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    uint32 CreatureID = _Creature->GetEntry();

    switch (CreatureID) 
    {
        //Jergosh the Invoker
    case 11518: 
        ai->Spell[0].Enabled = true;               
        ai->Spell[0].Spell_Id = 1094;      //Immolate    
        ai->Spell[0].Cooldown = 9000;                 
        ai->Spell[0].First_Cast = 6000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;  

        ai->Spell[1].Enabled = true;               
        ai->Spell[1].Spell_Id = 1108;      //Curse of Weakness   
        ai->Spell[1].Cooldown = 20000;                 
        ai->Spell[1].First_Cast = 10000;             
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;  
        break;

        //Oggleflint
    case 11517: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 11427;        // Cleave
        ai->Spell[0].Cooldown = 8000;                 
        ai->Spell[0].First_Cast = 6000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 
        break;

        // Taragaman the Hungerer
    case 11520: 
        ai->Spell[0].Enabled = true;                
        ai->Spell[0].Spell_Id = 16079;        // Fire Nova      
		ai->Spell[0].Cooldown = 15000;
        ai->Spell[0].First_Cast = 10000;             
        ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET; 

		ai->Spell[1].Enabled = true;                
        ai->Spell[1].Spell_Id = 10966;        // Uppercut      
		ai->Spell[1].Cooldown = 8000;
        ai->Spell[1].First_Cast = 5000;             
        ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;
        break;
    }

    ai->EnterEvadeMode();

    return ai;
}

void AddSC_instance_ragefire_chasm()
{
    Script *newscript;

	newscript = new Script;
    newscript->Name="npc_maur_grimtotem";
    newscript->GetAI = GetAI_npc_maur_grimtotem;
    m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="boss_bazzalan";
    newscript->GetAI = GetAI_boss_bazzalan;
    m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="mob_searing_warlock";
    newscript->GetAI = GetAI_mob_searing_warlock;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_ragefire_chasm";
    newscript->GetAI = GetAI_boss_ragefire_chasm;
    m_scripts[nrscripts++] = newscript;
}
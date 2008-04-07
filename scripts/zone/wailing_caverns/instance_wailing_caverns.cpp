/* 
   #########################
    WAILING CAVERNS ver 1.0
   #########################
by Myav

I use comments only in central situations
*/

#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../sc_defines.h"

/* ##### SPELLS ##### */

#define SPELL_SNEAK                 22766 /* Mad Magglish */
#define SPELL_SMOKEBOMB              7964 /* Mad Magglish */
#define SPELL_DEVOURE                7944 /* Devouring Ectoplasm */
#define SPELL_EVOLVE                 7946 /* Evolving Ectoplasm */
#define SPELL_DRUIDBOLT               915 /* Druid of the Fang */
#define SPELL_BOSSBOLT                943 /* Boss of the Fang */
#define SPELL_SERPENTFORM           32817 /* Druid of the Fang */
#define SPELL_LORDFORM               7965 /* Druid of the Fang */
#define SPELL_SLEEP                  1090 /* Boss of the Fang */
#define SPELL_POISON                34969 /* Lord Cobrahn */

/* ##### QUOTES(TEXT) ##### */

#define SAY_SCREAM                  "Call out for help!"
#define SAY_AGGROLADY               "None can stand against the serpent lords!"
#define SAY_AGGROCOBRAHN            "You will never wake the dreamer!"

/* ##### QUOTES(SOUND) ##### */

#define SOUND_AGGROLADY               5786
#define SOUND_AGGROCOBRAHN            5785

/* 
Mob Wailing Caverns
*/
struct MANGOS_DLL_DECL mob_wailing_cavernsAI : public ScriptedAI
{
    mob_wailing_cavernsAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

	uint32 Cast_Timer;
	uint32 Polymorph_Timer;
    uint32 Clone_Timer;
	int Rand;
	int RandX;
    int RandY;
	int Entry;
    Creature* Summoned;
	Unit* target;
	bool Polymorph;
    bool InCombat;

    void EnterEvadeMode()
    {
		target = NULL;
		Rand = 0;
        Rand = rand()%5000;
        Cast_Timer = Rand + 3000;
		Rand = 0;
        Rand = rand()%5000;
        Polymorph_Timer = Rand + 3000;
		Rand = 0;
		Rand = rand()%15000;
        Clone_Timer = Rand + 12000;
		Polymorph = false;
        InCombat = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

	    void Aggro(Unit *who)
    {
        
                if(m_creature->GetEntry() == 3840)
				{
				 DoCast(m_creature->getVictim(),SPELL_DRUIDBOLT);
				 Rand = 0;
				 Rand = rand()%5000;
				 Cast_Timer = Rand + 3000;
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
				if (who->IsWithinLOSInMap(m_creature) && InCombat)
                   if(m_creature->HasStealthAura())
                     m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
        }
    }

	void Clone(Unit* victim)
    {
        switch (rand()%2)
        {
         case 0: RandX = 0 - 0.3; break;
         case 1: RandX = 0 + 0.3; break;
        }
        Rand = 0;
        Rand = rand()%2;
        switch (rand()%2)
        {
         case 0: RandY = 0 - 0.3; break;
         case 1: RandY = 0 + 0.3; break;
        }
        if(m_creature->GetEntry() == 5780)
           Summoned = DoSpawnCreature(5780, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
		if(m_creature->GetEntry() == 3640)
           Summoned = DoSpawnCreature(3640, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000);
        if(Summoned)
		   {
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
			Summoned->SetMaxHealth(950);
			Summoned->SetHealth(950);
		   }
    }

	void Druid(Unit* victim)
    {
		Rand = 0;
        Rand = rand()%20;
        switch (rand()%2)
        {
         case 0: RandX = 0 - 10 - Rand; break;
         case 1: RandX = 0 + 10 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%20;
        switch (rand()%2)
        {
         case 0: RandY = 0 - 10 - Rand; break;
         case 1: RandY = 0 + 10 + Rand; break;
        }
           Summoned = DoSpawnCreature(3840, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

	void Raptor(Unit* victim)
    {
		Rand = 0;
        Rand = rand()%20;
        switch (rand()%2)
        {
         case 0: RandX = 0 - 10 - Rand; break;
         case 1: RandX = 0 + 10 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%20;
        switch (rand()%2)
        {
         case 0: RandY = 0 - 10 - Rand; break;
         case 1: RandY = 0 + 10 + Rand; break;
        }
           Summoned = DoSpawnCreature(3636, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
	{
		if(!InCombat)
		 {
		   Rand = 0;
		   Rand = rand()%5000;
           Cast_Timer = Rand + 3000;
		   Rand = 0;
           Rand = rand()%5000;
           Polymorph_Timer = Rand + 3000;
		   Rand = 0;
		   Rand = rand()%10000;
           Clone_Timer = Rand + 8000;
		   Polymorph = false;
		 }

	    if (!m_creature->SelectHostilTarget())
            return;

   if( m_creature->getVictim() && m_creature->isAlive())
     {
	   Entry = m_creature->GetEntry();

	   /* Devouring Ectoplasm */
       if(Entry == 3638)
	     {
		   if( m_creature->GetHealth()*5 / m_creature->GetMaxHealth() <= 3)
		      {
			    if ( Polymorph_Timer < diff ) //Etherald
			     {
				    if(!Polymorph && Polymorph_Timer < diff)
				       {		
			              m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_PHYSICAL, true); 
				          m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_MAGIC, true);
			              Polymorph_Timer = 10000;
						  Polymorph = true;
					   }
				    if(Polymorph && Polymorph_Timer < diff)
				       {
			              m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_PHYSICAL, false); 
				          m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_MAGIC, false);
			              Polymorph_Timer = 90000;
						  Polymorph = false;
					   }
			       }else Polymorph_Timer -= diff;	
		      }
	     }
	     
	   /* Cloned Ectoplasm */
       if(Entry == 5780)
	      {
             if( m_creature->GetHealth()*5 / m_creature->GetMaxHealth() <= 3)
			  if(m_creature->GetMaxHealth() > 1000)
			  {
			   if(Clone_Timer < diff) //Clone
			    {
					target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    Clone(target);
                    Clone_Timer = 180000;
			    }else Clone_Timer -= diff;
			  }
	      }
	   /* Evolving Ectoplasm */
       if(Entry == 3640)
	      {
			  if( m_creature->GetHealth()*5 / m_creature->GetMaxHealth() <= 3)
			   {
			     if ( Polymorph_Timer < diff ) //Etherald
			     {
				    if(!Polymorph && Polymorph_Timer < diff)
				       {		
			              m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_PHYSICAL, true); 
				          m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_MAGIC, true);
			              Polymorph_Timer = 10000;
						  Polymorph = true;
					   }
				    if(Polymorph && Polymorph_Timer < diff)
				       {
			              m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_PHYSICAL, false); 
				          m_creature->ApplySpellImmune(0, IMMUNITY_SCHOOL, IMMUNE_SCHOOL_MAGIC, false);
			              Polymorph_Timer = 90000;
						  Polymorph = false;
					   }
			       }else Polymorph_Timer -= diff;

				   if(m_creature->GetMaxHealth() > 1000)
                    if( Clone_Timer < diff ) //Clone
			         {
			   		    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                        Clone(target);
                        Clone_Timer = 180000;
			         }else Clone_Timer -= diff;
			  }
	      }
	      /* Deviate Guardian */
          if(Entry == 3637)
			if( m_creature->GetHealth()*7 / m_creature->GetMaxHealth() <= 1)
             if(Cast_Timer < diff)
			    {
				   //fear
                   DoYell(SAY_SCREAM,LANG_UNIVERSAL,NULL);
                   switch (rand()%6)
                     {
				   case 0:{
					       target = SelectUnit(SELECT_TARGET_RANDOM,0);
                           Raptor(target);
					       }break;
                    case 1:{
					        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                            Raptor(target);
					       }break;
					case 2:{
					        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                            Raptor(target);
					       }break;
					case 3:{
					        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                            Raptor(target);
						    Druid(target);
					       }break;
					case 5:{
					        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                            Raptor(target);
						    Druid(target);
					       }break;
				    case 4:{
					        target = SelectUnit(SELECT_TARGET_RANDOM,0);
                            Raptor(target);
						    Druid(target);
							Raptor(target);
					       }break;
                     }
                   Cast_Timer = 180000;
			    }else Cast_Timer -= diff;
		  /* Druid of the Fang */
          if(Entry == 3840)
		    {
		      if(Cast_Timer < diff) //bolt
			    {  
				  DoCast(m_creature->getVictim(),SPELL_DRUIDBOLT);
				  Rand = 0;
				  Rand = rand()%5000;
				  Cast_Timer = Rand + 6000;
		        }else Cast_Timer -= diff;

			  if(Polymorph_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false)) //Fang
			    {  
				  DoCast(m_creature,SPELL_SERPENTFORM);
				  Cast_Timer += 5000;
				  Rand = 0;
				  Rand = rand()%10000;
				  Polymorph_Timer = Rand + 8000;
		        }else Polymorph_Timer -= diff;
		    }

	   DoMeleeAttackIfReady();
   }
 }
};
    
/* 
Mad Magglish
*/
struct MANGOS_DLL_DECL boss_mad_magglishAI : public ScriptedAI
{
    boss_mad_magglishAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 Cast_Timer;
	int Rand;
    bool InCombat;

    void EnterEvadeMode()
    {
		Rand = 0;
        Rand = rand()%5000;
        Cast_Timer = Rand + 3000;
        InCombat = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void Aggro(Unit *who)
    {
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
				if (who->IsWithinLOSInMap(m_creature) && InCombat)
                  if(m_creature->HasStealthAura())
                     m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
        }
    }
    void UpdateAI(const uint32 diff)
    {

		//Sneak
			{
			   if(!InCombat && !m_creature->HasStealthAura())
				   DoCast(m_creature,SPELL_SNEAK);
			}

        if (!m_creature->SelectHostilTarget())
            return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Smoke Bomb
            if (Cast_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false))
            {
                DoCast(m_creature->getVictim(),SPELL_SMOKEBOMB);
				Rand = 0;
				Rand = rand()%5000;
                Cast_Timer = Rand + 4500;
            }else Cast_Timer -= diff;
			
            DoMeleeAttackIfReady();
		}
	}
};
/* 
Lady Anacondra
*/
struct MANGOS_DLL_DECL boss_lady_anacondraAI : public ScriptedAI
{
    boss_lady_anacondraAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 Cast_Timer;
	uint32 Polymorph_Timer;
	int Rand;
    bool InCombat;

    void EnterEvadeMode()
    {
		Rand = 0;
        Rand = rand()%5000;
        Cast_Timer = Rand + 5000;
		Rand = 0;
        Rand = rand()%8000;
        Polymorph_Timer = Rand + 7000;
        InCombat = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void Aggro(Unit *who)
    {
                DoPlaySoundToSet(m_creature,SOUND_AGGROLADY);
				DoYell(SAY_AGGROLADY,LANG_UNIVERSAL,NULL);
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
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(Cast_Timer < diff) //bolt
			    {  
				  m_creature->RemoveAllAuras();
                  switch (rand()%3)
                   {
                    case 0:DoCast(m_creature->getVictim(),SPELL_BOSSBOLT);break;
                    case 1:DoCast(m_creature->getVictim(),SPELL_BOSSBOLT);break;
					case 2:{
						    Unit* target = NULL;
				            Unit* target1 = NULL;
				            //sleep most threated enemy
                            target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
                            if (target)DoCast(target,SPELL_SLEEP);
				            //not attack sleeped enemy
				            target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
				            target1 = SelectUnit(SELECT_TARGET_TOPAGGRO,2);				
				            m_creature->DeleteThreatList();
				            //attack other enemies or go home
				            m_creature->AddThreat(target, 100.0f);
				            m_creature->AddThreat(target1, 50.0f);
				            m_creature->Attack(target);
						   }break;
					}
                  Rand = 0;
				  Rand = rand()%5000;
				  Cast_Timer = Rand + 5000;
		        }else Cast_Timer -= diff;

			  if(Polymorph_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false)) //Fang
			    {  
				  DoCast(m_creature,SPELL_SERPENTFORM);
				  Cast_Timer += 10000;
				  Rand = 0;
				  Rand = rand()%10000;
				  Polymorph_Timer = Rand + 8000;
		        }else Polymorph_Timer -= diff;
			
            DoMeleeAttackIfReady();
	}
};
/* 
Lord Cobrahn
*/
struct MANGOS_DLL_DECL boss_lord_cobrahnAI : public ScriptedAI
{
    boss_lord_cobrahnAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 Cast_Timer;
	uint32 Summon_Timer;
	int Rand;
	int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;

    void EnterEvadeMode()
    {
		Rand = 0;
        Rand = rand()%5000;
        Cast_Timer = Rand + 5000;
		Rand = 0;
        Rand = rand()%8000;
        Summon_Timer = Rand + 10000;
        InCombat = false;
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void Aggro(Unit *who)
    {
                DoPlaySoundToSet(m_creature,SOUND_AGGROCOBRAHN);
				DoYell(SAY_AGGROCOBRAHN,LANG_UNIVERSAL,NULL);
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
    
	void Snake(Unit* victim)
    {
		Rand = 0;
        Rand = rand()%10;
        switch (rand()%2)
        {
         case 0: RandX = 0 - Rand; break;
         case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%10;
        switch (rand()%2)
        {
         case 0: RandY = 0 - Rand; break;
         case 1: RandY = 0 + Rand; break;
        }
           Summoned = DoSpawnCreature(8886, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
        if ( m_creature->GetHealth()*10 / m_creature->GetMaxHealth() > 4)
		   {
            if(Cast_Timer < diff) //bolt
			    {  
                  switch (rand()%2)
                   {
                    case 0:DoCast(m_creature->getVictim(),SPELL_BOSSBOLT);break;
                    case 1:DoCast(m_creature->getVictim(),SPELL_POISON);break;
					}
                  Rand = 0;
				  Rand = rand()%6000;
				  Cast_Timer = Rand + 5000;
		        }else Cast_Timer -= diff;
		}else
		 {
          if(Cast_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false)) //Fang
			    {  
				  DoCast(m_creature,SPELL_LORDFORM);
				  Cast_Timer = 300000;
		        }else Cast_Timer -= diff;
		 }

			  if(Summon_Timer < diff) //Snake
			    {  
				  Unit* target = NULL;
				  Rand = 0;
				  Rand = rand()%3;
				  Rand += 2;
                  for(int i = 0; i < Rand;i++)
                  {
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    Snake(target);
                  }
				  Rand = 0;
				  Rand = rand()%15000;
				  Summon_Timer = Rand + 30000;
		        }else Summon_Timer -= diff;
			
            DoMeleeAttackIfReady();
	}
};
CreatureAI* GetAI_mob_wailing_caverns(Creature *_Creature)
{
    return new mob_wailing_cavernsAI (_Creature);
}
CreatureAI* GetAI_boss_mad_magglish(Creature *_Creature)
{
    return new boss_mad_magglishAI (_Creature);
}
CreatureAI* GetAI_boss_lady_anacondra(Creature *_Creature)
{
    return new boss_lady_anacondraAI (_Creature);
}
CreatureAI* GetAI_boss_lord_cobrahn(Creature *_Creature)
{
    return new boss_lord_cobrahnAI (_Creature);
}
void AddSC_instance_wailing_caverns()
{   
	Script *newscript;

	newscript = new Script;
    newscript->Name="mob_wailing_caverns";
    newscript->GetAI = GetAI_mob_wailing_caverns;
    m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="boss_mad_magglish";
    newscript->GetAI = GetAI_boss_mad_magglish;
    m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="boss_lady_anacondra";
    newscript->GetAI = GetAI_boss_lady_anacondra;
    m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="boss_lord_cobrahn";
    newscript->GetAI = GetAI_boss_lord_cobrahn;
    m_scripts[nrscripts++] = newscript;
}
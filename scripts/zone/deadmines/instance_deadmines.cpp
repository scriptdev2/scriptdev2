/* 
   ###################
    DEADMINES ver 1.1
   ###################
by Myav

I use comments only in central situations
*/

#include "../../../../../game/TargetedMovementGenerator.h"
#include "../../sc_defines.h"

/* ##### SPELLS ##### */

#define SPELL_FIREBALL                  3140 /* Brainwashed Noble, Defias Conjurer */
#define SPELL_STEALTH                   1785 /* Defias Blackguard */
#define SPELL_FROSTNOVA                  865 /* Defias Wizard */
#define SPELL_WIZARDSHIELD              7300 /* Defias Wizard */
#define SPELL_LIVINGFLAME               5113 /* Living Flame */
#define SPELL_NOBLESHEEP                 118 /* Brainwashed Noble */
#define SPELL_SLAM                      6304 /* Rhahkzor */
#define SPELL_DISTRACTINGPAIN           3603 /* Sneeds Shredder */
#define SPELL_TERRIFY                   7399 /* Sneeds Shredder */
#define SPELL_SMITESTOMP                6432 /* Mr. Smite */
#define SPELL_SMITESLAM                 6435 /* Mr. Smite */

/* ##### QUOTES(TEXT) ##### */

#define SAY_AGGRORHAHKZOR               "VanCleef pay big for your heads!"
#define SAY_PIRATESATTACK               "We're under attack! A vast, ye swabs! Repel the invaders!"
#define SAY_AGGROSMITE                  "You there, check out that noise!"
#define SAY_SMITESTOMP1                 "You landlubbers are tougher than i thought. I'll have to improvise!"
#define SAY_SMITESTOMP2                 "D'ah! Now you're making me angry!"
#define SAY_AGGROVANCLEEF               "None may challenge the Brotherhood!"
#define SAY_VANCLEEFADDS1               "Lapdogs, all of you!" 
#define SAY_VANCLEEFADDS2               "Fools! Our cause is righteous!"
#define SAY_VANCLEEFADDS3               "The Brotherhood shall prevail!"
#define SAY_VANCLEEFKILL                "And stay down!"

/* ##### QUOTES(SOUND) ##### */

#define SOUND_AGGRORHAHKZOR               5774
#define SOUND_PIRATESATTACK               5777
#define SOUND_AGGROSMITE                  5775
#define SOUND_SMITESTOMP1                 5778
#define SOUND_SMITESTOMP2                 5779
#define SOUND_AGGROVANCLEEF               5780
#define SOUND_VANCLEEFADDS1               5782
#define SOUND_VANCLEEFADDS2               5783
#define SOUND_VANCLEEFADDS3               5784
#define SOUND_VANCLEEFKILL                5781

/* ##### OTHER SOUNDS ##### */

#define SOUND_CANNONFIRE                  1400
#define SOUND_DESTROYDOOR                 3079 //3363 //3380
#define SOUND_GOLEM                       2092
#define SOUND_COMPANION                   3604
#define SOUND_FLAME                       1288

/* 
Mob Deadmines (Defias Wizard, Defias Squallshaper, Living Flame, Defias Blackguard, Goblin Craftsman, Defias Conjurer )
*/
struct MANGOS_DLL_DECL mob_deadminesAI : public ScriptedAI
{
    mob_deadminesAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
	uint32 Cast_Timer;
	uint32 Summon_Timer;
	int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
	Unit* target;
    bool InCombat;

    void EnterEvadeMode()
    {
		target = NULL;
		Rand = rand()%5000;
        Cast_Timer = Rand + 3000;
		Rand = rand()%10000;
		Summon_Timer = Rand + 5000;
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
				/* Defias Wizard */ 
				if(m_creature->GetEntry() == 4418)
				   {
				      //Cast Shield when aggro
                      DoCast(m_creature,SPELL_WIZARDSHIELD);
				   }

				/* Defias Squallshaper */ 
				if(m_creature->GetEntry() == 1732)
				   {
				      //Summon Defias Companion
                      target = SelectUnit(SELECT_TARGET_RANDOM,0);
                      Summon(target);
					  DoPlaySoundToSet(m_creature,SOUND_COMPANION);
			       }

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
				/* Defias Blackguard */
			    if (who->IsWithinLOSInMap(m_creature) && InCombat)
                  if(m_creature->HasStealthAura())
                     m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
            }
        }
    }

	void Summon(Unit* victim)
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
        if(m_creature->GetEntry() == 1731)
           Summoned = DoSpawnCreature(2520, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000);
        if(m_creature->GetEntry() == 1732)
           Summoned = DoSpawnCreature(3450, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000);
		if(m_creature->GetEntry() == 619)
           Summoned = DoSpawnCreature(3417, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
		if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
		/* Defias Blackguard */
            if(m_creature->GetEntry() == 636)
			{
			   if(!InCombat && !m_creature->HasStealthAura())
				   DoCast(m_creature,SPELL_STEALTH);
			}

        if (!m_creature->SelectHostilTarget())
            return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {			
			/* Defias Wizard */
			if(m_creature->GetEntry() == 4418)
		    {
               //Cast Spell (66% - Fireball, 33% - Frost Nova)
               if (Cast_Timer < diff)
               {   switch (rand()%3)
                   {
                   case 0:DoCast(m_creature->getVictim(),SPELL_FIREBALL); break;
                   case 1:DoCast(m_creature->getVictim(),SPELL_FIREBALL); break;
			       case 2:DoCast(m_creature->getVictim(),SPELL_FROSTNOVA); break;
		        	}
			       Rand = rand()%5000;
                   Cast_Timer = Rand + 4500;
               }else Cast_Timer -= diff;
			}
			/* Goblin Craftsman */
            if(m_creature->GetEntry() == 1731)
			{
			   //Summon Remote-Controlled Golem
               if (Summon_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false))
               {
               target = SelectUnit(SELECT_TARGET_RANDOM,0);
               Summon(target);
			   DoPlaySoundToSet(m_creature,SOUND_GOLEM);
			   Rand = rand()%60000;
			   Summon_Timer = Rand + 45000;
               }else Summon_Timer -= diff;
			}

			/* Defias Conjurer */
            if(m_creature->GetEntry() == 619)
			{
			   //Summon Living Flame
			   if (Summon_Timer < diff)
			   {
                  target = SelectUnit(SELECT_TARGET_RANDOM,0);
                  Summon(target);
			      DoPlaySoundToSet(m_creature,SOUND_FLAME);
				  Rand = rand()%20000;
			      Summon_Timer = Rand + 15000;
			   }else Summon_Timer -= diff; 
               //Fireball
               if (Cast_Timer < diff)
			   {
			      DoCast(m_creature->getVictim(),SPELL_FIREBALL);
				  Rand = rand()%5000;
			      Cast_Timer = Rand + 4000;
			   }else Cast_Timer -= diff;
			}			
            
            /* Living Flame */
            if(m_creature->GetEntry() == 3417)
			{
			   //Living Flames
               if (Cast_Timer < diff)
			   {
			      DoCast(m_creature->getVictim(),SPELL_LIVINGFLAME);
				  Rand = rand()%5000;
			      Cast_Timer = Rand + 3000;
			   }else Cast_Timer -= diff;
			}

            DoMeleeAttackIfReady();
        }
    }
};
/* 
Brainwashed Noble (has better AI for Polymorph)
*/
struct MANGOS_DLL_DECL boss_brainwashed_nobleAI : public ScriptedAI
{
    boss_brainwashed_nobleAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
    uint32 Fireball_Timer;
	uint32 Sheep_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        Fireball_Timer = 4000;
		Sheep_Timer = 9000;
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
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget())
            return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Fireball_Timer
            if (Fireball_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false))
            {
                DoCast(m_creature->getVictim(),SPELL_FIREBALL);
                Fireball_Timer = 5000;
            }else Fireball_Timer -= diff;
			//Sheep_Timer
            if (Sheep_Timer < diff && !m_creature->IsNonMeleeSpellCasted(false))
			{
                Unit* target = NULL;
				Unit* target1 = NULL;
				//polymorph most threated enemy
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
                if (target)DoCast(target,SPELL_NOBLESHEEP);
				//not attack polymorphed enemy
				target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
				target1 = SelectUnit(SELECT_TARGET_TOPAGGRO,2);				
				m_creature->DeleteThreatList();
				//attack other enemies or go home
				m_creature->AddThreat(target, 100.0f);
				m_creature->AddThreat(target1, 50.0f);
				m_creature->Attack(target);
                Sheep_Timer = 8000;
            }else Sheep_Timer -= diff;

            DoMeleeAttackIfReady();
		}
	}
};
/* 
RHAHKZOR (Aggro Quotes, add defias spawn after death)
*/
struct MANGOS_DLL_DECL boss_rhahkzorAI : public ScriptedAI
{
    boss_rhahkzorAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
	uint32 Yell_Timer;
    uint32 Slam_Timer;
	float x;
	float y;
	float z;
	int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;

    void EnterEvadeMode()
    {
        Yell_Timer = 58000;
        Slam_Timer = 3000;
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
            //Say our dialog
            if(!InCombat)
            {
                DoYell(SAY_AGGRORHAHKZOR,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGRORHAHKZOR); 
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

	void SummonTaskmaster(Unit* victim)
     {
        Summoned = DoSpawnCreature(4417, 29.19, 37.34, 3.43, 3.92, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
     }

	void SummonEvoker(Unit* victim)
     {
        Summoned = DoSpawnCreature(1729, 26.58, 40.39, 3.9, 4, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
     }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget())
            return;

        if( m_creature->getVictim() && m_creature->isAlive())
        {
            //Slam Cast
            if (Slam_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SLAM);
                Slam_Timer = 7000;
            }else Slam_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    }
    //Summon two defias after death
	void JustDied(Unit* m_creature)
       {
	   Unit* target = NULL;
	   target = SelectUnit(SELECT_TARGET_RANDOM,0);
	   m_creature->GetPosition(x,y,z);
       SummonTaskmaster(target);
       SummonEvoker(target);
       }
};
/* 
SNEEDS SHREDDER (spells, updated AI for fear, Sneed Jump from Shredder after death)
*/
struct MANGOS_DLL_DECL boss_sneed_shredderAI : public ScriptedAI
{
    boss_sneed_shredderAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
	uint32 DistractingPain_Timer;
    uint32 Terrify_Timer;

    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;

    void EnterEvadeMode()
    {
        DistractingPain_Timer = 3000;
        Terrify_Timer = 6000;
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

	  void SummonSneed(Unit* victim)
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
        Summoned = DoSpawnCreature(643, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
            //DistractingPain Cast
            if (DistractingPain_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_DISTRACTINGPAIN);
                DistractingPain_Timer = 15000;
            }else DistractingPain_Timer -= diff;

			//Terrify Cast
            if (Terrify_Timer < diff)
            {
				Unit* target = NULL;
				Unit* target1 = NULL;
				//Cast fear on most threated enemy
                target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);				
                if (target)DoCast(target,SPELL_TERRIFY);
				//not attack feared enemy
				target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
				target1 = SelectUnit(SELECT_TARGET_TOPAGGRO,2);
				m_creature->DeleteThreatList();
				//attack other enemies or go home
				m_creature->AddThreat(target, 200.0f);
				m_creature->AddThreat(target1, 100.0f);
				m_creature->Attack(target);
                Terrify_Timer = 20000;
            }else Terrify_Timer -= diff;

            DoMeleeAttackIfReady();
    }
	//summon sneed after death
	void JustDied(Unit* m_creature)
       {
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_RANDOM,0);
        SummonSneed(target);	   
       }
};
/* 
MR SMITE (blizzlike) 
*/
struct MANGOS_DLL_DECL boss_mrsmiteAI : public ScriptedAI
{
    boss_mrsmiteAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}

	bool Chest;
	float x;
	float y;
	float z;
	uint32 Yell_Timer;
    uint32 SmiteCast_Timer;
	uint32 SmiteStomp1_Timer;
	uint32 SmiteStomp2_Timer;
	uint32 Chest_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
		Chest = false;
        Yell_Timer = 58000;
        SmiteCast_Timer = 2500;
		SmiteStomp1_Timer = 0;
		SmiteStomp2_Timer = 0;
		Chest_Timer = 0;
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
		if (Chest)
			return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Say our dialog
            if(!InCombat)
            {
                DoYell(SAY_AGGROSMITE,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGROSMITE); 
                InCombat = true;
            }

            DoStartMeleeAttack(who);
        }
    }

	void MoveInLineOfSight(Unit *who)
    {    
		if (Chest)
			return;
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
		if(Chest && Chest_Timer < diff)
		  {
			m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
            m_creature->AI_SendMoveToPacket(x,y,z,2000,true,0);
			Chest = false;
		  }else Chest_Timer -= diff;
			//If we are <66% hp SmiteStomp
            if ( (int) (m_creature->GetHealth()*3 / m_creature->GetMaxHealth() +1) == 2)
			{
				if(SmiteStomp1_Timer < diff)
               {m_creature->RemoveAllAuras();
                DoYell(SAY_SMITESTOMP1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_SMITESTOMP1); 
                DoCast(m_creature->getVictim(),SPELL_SMITESTOMP);
				SmiteStomp1_Timer = 580000;
				Chest_Timer = 5000;
				Chest = true;
				//run to chest
				m_creature->GetPosition(x, y, z);
				m_creature->RemoveAllAuras();
				m_creature->AI_SendMoveToPacket(2.05,-778.28,9.6,3000,true,0);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 16);	
			//	
			}else SmiteStomp1_Timer -= diff;
			}
			//If we are <33% hp SmiteStomp
            if ( (int) (m_creature->GetHealth()*3 / m_creature->GetMaxHealth() +1) == 1)
			{
				if(SmiteStomp2_Timer < diff)
                {
				m_creature->RemoveAllAuras();
                DoYell(SAY_SMITESTOMP2,LANG_UNIVERSAL,NULL);
			    DoPlaySoundToSet(m_creature,SOUND_SMITESTOMP2); 
                DoCast(m_creature->getVictim(),SPELL_SMITESTOMP);
                SmiteStomp2_Timer = 580000;
				Chest_Timer = 5000;
				Chest = true;
				m_creature->GetPosition(x, y, z);
				m_creature->RemoveAllAuras();
				m_creature->AI_SendMoveToPacket(2.05,-778.28,9.6,3000,true,0);
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 16);

            }else SmiteStomp2_Timer -= diff;
			}
            //SmiteSlam Cast
            if (SmiteCast_Timer < diff && !Chest)
            {  
                DoCast(m_creature->getVictim(),SPELL_SMITESLAM);
                SmiteCast_Timer = 8000;
            }else SmiteCast_Timer -= diff;
			if( !Chest)
			{
            DoMeleeAttackIfReady();
			}
	}
};
/* 
VANCLEEF (Many Quotes, add spawns in combat, two-hands fight) 
*/
struct MANGOS_DLL_DECL boss_edwin_vancleefAI : public ScriptedAI
{
    boss_edwin_vancleefAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    
	uint32 Yell_Timer;
    uint32 Adds1_Timer;
	uint32 Adds2_Timer;
	uint32 Adds3_Timer;
	uint32 SummonAdds_Timer;
    int Rand;
    int RandX;
    int RandY;
    Creature* Summoned;
    bool InCombat;

    void EnterEvadeMode()
    {
        Yell_Timer = 58000;
        Adds1_Timer = 0;
		Adds2_Timer = 0;
		Adds3_Timer = 0;
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
            //Say our dialog
            if(!InCombat)
            {
                DoYell(SAY_AGGROVANCLEEF,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_AGGROVANCLEEF);
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
	   void SummonAdds(Unit* victim)
    {
        Rand = rand()%3;
        switch (rand()%2)
        {
        case 0: RandX = 0 - Rand; break;
        case 1: RandX = 0 + Rand; break;
        }
        Rand = 0;
        Rand = rand()%3;
        switch (rand()%2)
        {
        case 0: RandY = 0 - Rand; break;
        case 1: RandY = 0 + Rand; break;
        }
        Rand = 0;
        Summoned = DoSpawnCreature(636, RandX, RandY, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
        if(Summoned)
            ((CreatureAI*)Summoned->AI())->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {  
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;  
			//If we are <75% hp Say
            if ( (int) (m_creature->GetHealth()*4 / m_creature->GetMaxHealth() +1) == 3)
			{if(Adds1_Timer < diff)
                {
                DoYell(SAY_VANCLEEFADDS1,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_VANCLEEFADDS1);
                //Shouldn't say this agian
                Adds1_Timer = 580000;
            }else Adds1_Timer -= diff;
			}
            //If we are <50% hp Say
            if ( (int) (m_creature->GetHealth()*4 / m_creature->GetMaxHealth() +1) == 2)
			{if(Adds2_Timer < diff)
                {
                DoYell(SAY_VANCLEEFADDS2,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_VANCLEEFADDS2);   
				// summon 2 adds 
                Unit* target = NULL;
                for(int i = 0; i < 2;i++)
                {
                    target = SelectUnit(SELECT_TARGET_RANDOM,0);
                    SummonAdds(target);
                } 
                //Shouldn't do this agian
                Adds2_Timer = 580000;
            }else Adds2_Timer -= diff;
			}
            //If we are <75% hp Say
            if ( (int) (m_creature->GetHealth()*4 / m_creature->GetMaxHealth() +1) == 1)
			{if(Adds3_Timer < diff)
                {
                DoYell(SAY_VANCLEEFADDS3,LANG_UNIVERSAL,NULL);
                DoPlaySoundToSet(m_creature,SOUND_VANCLEEFADDS3);
                //Shouldn't say this agian
                Adds3_Timer = 580000;
            }else Adds3_Timer -= diff;
			}

    DoMeleeAttackIfReady();
  }
void KilledUnit(Unit *m_creature)
{
 DoYell(SAY_VANCLEEFKILL,LANG_UNIVERSAL,NULL);
 DoPlaySoundToSet(m_creature,SOUND_VANCLEEFKILL);
}
};
CreatureAI* GetAI_mob_deadmines(Creature *_Creature)
{
    return new mob_deadminesAI (_Creature);
}
CreatureAI* GetAI_boss_brainwashed_noble(Creature *_Creature)
{
    return new boss_brainwashed_nobleAI (_Creature);
}
CreatureAI* GetAI_boss_rhahkzor(Creature *_Creature)
{
    return new boss_rhahkzorAI (_Creature);
}
CreatureAI* GetAI_boss_sneed_shredder(Creature *_Creature)
{
    return new boss_sneed_shredderAI (_Creature);
}
CreatureAI* GetAI_boss_mrsmite(Creature *_Creature)
{
    return new boss_mrsmiteAI (_Creature);
}
CreatureAI* GetAI_boss_edwin_vancleef(Creature *_Creature)
{
    return new boss_edwin_vancleefAI (_Creature);
}
void AddSC_instance_deadmines()
{   
	Script *newscript;

	newscript = new Script;
    newscript->Name="mob_deadmines";
    newscript->GetAI = GetAI_mob_deadmines;
    m_scripts[nrscripts++] = newscript;

	newscript = new Script;
    newscript->Name="boss_brainwashed_noble";
    newscript->GetAI = GetAI_boss_brainwashed_noble;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_rhahkzor";
    newscript->GetAI = GetAI_boss_rhahkzor;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_sneed_shredder";
    newscript->GetAI = GetAI_boss_sneed_shredder;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_mrsmite";
    newscript->GetAI = GetAI_boss_mrsmite;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="boss_edwin_vancleef";
    newscript->GetAI = GetAI_boss_edwin_vancleef;
    m_scripts[nrscripts++] = newscript;
}
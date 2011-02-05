/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_PET_H
#define SC_PET_H

// Using CreatureAI for now. Might change later and use PetAI (need to export for dll first)
class MANGOS_DLL_DECL ScriptedPetAI : public CreatureAI
{
    public:
        explicit ScriptedPetAI(Creature* pCreature);
        ~ScriptedPetAI() {}

        void MoveInLineOfSight(Unit* /*pWho*/);

        void AttackStart(Unit* /*pWho*/);

        void AttackedBy(Unit* /*pAttacker*/);

        void KilledUnit(Unit* /*pVictim*/) {}

        void OwnerKilledUnit(Unit* /*pVictim*/) {}

        void UpdateAI(const uint32 uiDiff);

        virtual void Reset() {}

        virtual void UpdatePetAI(const uint32 uiDiff);      // while in combat

        virtual void UpdatePetOOCAI(const uint32 uiDiff) {} // when not in combat

    protected:
        void ResetPetCombat();
};

#endif

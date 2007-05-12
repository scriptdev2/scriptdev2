//Fairlena
//8799 greet - Your old lives, your mortal desires, mean nothing. You are acolytes of the master now, and you will serve the cause without question! The greatest glory is to die in the master's service!
//8794 aggro1 - Slay them in the master's name!
//8795 aggro2 - You cannot hide from me!
//8796 aggro3 - Kneel before me, worm!
//8797 aggro4 - Run while you still can!
//8800 slay1 - You have failed!
//8801 slay2 - Pathetic wretch!
//8798 death - The master... will avenge me!
//8955 - Random Aggro ? - ?

#include "../../sc_defines.h"

#define SAY_GREET           "Your old lives, your mortal desires, mean nothing. You are acolytes of the master now, and you will serve the cause without question! The greatest glory is to die in the master's service!"
#define SAY_AGGRO1          "Slay them in the master's name!"
#define SAY_AGGRO2          "You cannot hide from me!"
#define SAY_AGGRO3          "Kneel before me, worm!"
#define SAY_AGGRO4          "Run while you still can!"
#define SAY_SLAY1           "You have failed!"
#define SAY_SLAY2           "Pathetic wretch!"
#define SAY_DEATH           "The master... will avenge me!"
#define SAY_RANDOM_AGGRO    "???"

#define SOUND_GREET         8799
#define SOUND_AGGRO1        8794
#define SOUND_AGGRO2        8795
#define SOUND_AGGRO3        8796
#define SOUND_AGGRO4        8797
#define SOUND_SLAY1         8800
#define SOUND_SLAY2         8801
#define SOUND_DEATH         8798
#define SOUND_RANDOM_AGGRO  8955

#define SPELL_POSIONBOLT_VOLLEY     28796
#define SPELL_RAINOFFIRE            28794   //Not sure if targeted AoEs work if casted directly upon a player
#define SPELL_ENRAGE                26527

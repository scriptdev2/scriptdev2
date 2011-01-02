/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#include "../../../shared/revision_nr.h"

#ifdef WIN32
#   define MANGOS_DLL_EXPORT extern "C" __declspec(dllexport)
#elif defined( __GNUC__ )
#   define MANGOS_DLL_EXPORT extern "C"
#else
#   define MANGOS_DLL_EXPORT extern "C" export
#endif


MANGOS_DLL_EXPORT
char const* GetMangosRevStr()
{
    return REVISION_NR;
}

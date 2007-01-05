AM_CXXFLAGS = @COMPILER_OPTIONS@ @EXTRA_COMPILER_OPTIONS@

INCLUDES += -I./../../../src/shared -I./../../../src/framework -I./../../../dep/include -I./ -I./../../dep/include -I./cpp-scripts -I./../../../src/mangosd -I./../../../src/game
pkglibdir = $(libdir)

lib_LTLIBRARIES = libMaNGOSScript.la

libMaNGOSScript_la_LDFLAGS=-export-dynamic

libMaNGOSScript_la_SOURCES = \
config.h \
ScriptMgr.h
ScriptMgr.cpp \
system.cpp \
scripts\sc_default.cpp \
scripts\sc_defines.h \
scripts\sc_defines.cpp \
scripts\boss\sc_boss_onyxia.cpp \
scripts\creature\sc_generic_caster.cpp \
scripts\creature\sc_generic_melee.cpp \
scripts\creature\sc_kobold.cpp \
scripts\custom\sc_custom_example.cpp \
scripts\guard\sc_guard_bluffwatcher.cpp \
scripts\guard\sc_guard_contested.cpp \
scripts\guard\sc_guard_darnassus.cpp \
scripts\guard\sc_guard_dunmorogh.cpp \
scripts\guard\sc_guard_durotar.cpp \
scripts\guard\sc_guard_elwynnforest.cpp \
scripts\guard\sc_guard_ironforge.cpp \
scripts\guard\sc_guard_mulgore.cpp \
scripts\guard\sc_guard_orgrimmar.cpp \
scripts\guard\sc_guard_stormwind.cpp \
scripts\guard\sc_guard_teldrassil.cpp \
scripts\guard\sc_guard_tirisfal.cpp \
scripts\guard\sc_guard_undercity.cpp \
scripts\hornor\sc_honor_vendor.cpp \
scripts\npc\sc_marchal_mcbride.cpp \
scripts\npc\sc_silas_darkmoon.cpp \
scripts\npc\sc_silva_filnaveth.cpp \
scripts\npc\sc_skorn_whitecloud.cpp \
scripts\servers\sc_battlemaster.cpp \
scripts\servers\sc_guildmaster.cpp \
scripts\servers\sc_travelmaster.cpp
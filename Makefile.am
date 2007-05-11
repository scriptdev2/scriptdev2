# Copyright © 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

## Process this file with automake to produce Makefile.in

## Sub-directories to parse
SUBDIRS = sql

## CPP flags for includes, defines, etc.
AM_CPPFLAGS = $(MYSQL_INCLUDES) -I$(srcdir) -I$(srcdir)/../../../dep/include -I$(srcdir)/../../shared/ -I$(srcdir)/../../framework/

## Build MaNGOS script library as shared library.
# libmangosscript shared library will later be reused by world server daemon.
lib_LTLIBRARIES = libmangosscript.la
libmangosscript_la_SOURCES = \
	ScriptMgr.cpp \
	ScriptMgr.h \
	config.h \
	scripts/creature/sc_defias.cpp \
	scripts/creature/sc_generic_creature.cpp \
	scripts/creature/sc_kobold.cpp \
	scripts/creature/sc_lumberjack.cpp \
	scripts/creature/sc_miner.cpp \
	scripts/custom/sc_custom_example.cpp \
	scripts/custom/sc_test.cpp \
	scripts/guard/sc_guard_ai.h \
	scripts/guard/sc_guard_bluffwatcher.cpp \
	scripts/guard/sc_guard_contested.cpp \
	scripts/guard/sc_guard_darnassus.cpp \
	scripts/guard/sc_guard_dunmorogh.cpp \
	scripts/guard/sc_guard_durotar.cpp \
	scripts/guard/sc_guard_elwynnforest.cpp \
	scripts/guard/sc_guard_exodar.cpp \
	scripts/guard/sc_guard_ironforge.cpp \
	scripts/guard/sc_guard_mulgore.cpp \
	scripts/guard/sc_guard_orgrimmar.cpp \
	scripts/guard/sc_guard_silvermoon.cpp \
	scripts/guard/sc_guard_stormwind.cpp \
	scripts/guard/sc_guard_teldrassil.cpp \
	scripts/guard/sc_guard_tirisfal.cpp \
	scripts/guard/sc_guard_undercity.cpp \
	scripts/honor/sc_honor_vendor.cpp \
	scripts/item/sc_item_test.cpp \
	scripts/sc_defines.cpp \
	scripts/sc_defines.h \
	scripts/servers/sc_battlemaster.cpp \
	scripts/servers/sc_guildmaster.cpp \
	scripts/servers/sc_travelmaster.cpp \
	scripts/zone/azuremyst_isle/sc_injured_draenei.cpp \
	scripts/zone/blackwing_lair/sc_broodlord_lashlayer.cpp \
	scripts/zone/blackwing_lair/sc_chromaggus.cpp \
	scripts/zone/blackwing_lair/sc_ebonroc.cpp \
	scripts/zone/blackwing_lair/sc_firemaw.cpp \
	scripts/zone/blackwing_lair/sc_flamegor.cpp \
	scripts/zone/blackwing_lair/sc_nefarian.cpp \
	scripts/zone/blackwing_lair/sc_razorgore.cpp \
	scripts/zone/blackwing_lair/sc_vaelastrasz.cpp \
	scripts/zone/elwynn_forest/sc_henze_faulk.cpp \
	scripts/zone/elwynn_forest/sc_marshal_mcbride.cpp \
	scripts/zone/karazhan/sc_curator.cpp \
	scripts/zone/karazhan/sc_maiden_of_virtue.cpp \
	scripts/zone/karazhan/sc_midnight.cpp \
	scripts/zone/karazhan/sc_moroes.cpp \
	scripts/zone/karazhan/sc_netherspite.cpp \
	scripts/zone/karazhan/sc_nightbane.cpp \
	scripts/zone/karazhan/sc_prince_malchezaar.cpp \
	scripts/zone/karazhan/sc_shade_of_aran.cpp \
	scripts/zone/karazhan/sc_terestian_illhoof.cpp \
	scripts/zone/molten_core/sc_boss_baron_geddon.cpp \
	scripts/zone/molten_core/sc_boss_garr.cpp \
	scripts/zone/molten_core/sc_boss_gehennas.cpp \
	scripts/zone/molten_core/sc_boss_golemagg.cpp \
	scripts/zone/molten_core/sc_boss_lucifron.cpp \
	scripts/zone/molten_core/sc_boss_magmadar.cpp \
	scripts/zone/molten_core/sc_boss_majordomo_executus.cpp \
	scripts/zone/molten_core/sc_boss_ragnaros.cpp \
	scripts/zone/molten_core/sc_boss_shazzrah.cpp \
	scripts/zone/molten_core/sc_boss_sulfuron_harbinger.cpp \
	scripts/zone/molten_core/sc_mob_firelord.cpp \
	scripts/zone/moonglade/sc_bunthen_plainswind.cpp \
	scripts/zone/moonglade/sc_silva_filnaveth.cpp \
	scripts/zone/mulgore/sc_skorn_whitecloud.cpp \
	scripts/zone/naxxramas/sc_anubrekhan.cpp \
	scripts/zone/naxxramas/sc_faerlina.cpp \
	scripts/zone/naxxramas/sc_feugen.cpp \
	scripts/zone/naxxramas/sc_gluth.cpp \
	scripts/zone/naxxramas/sc_gothik.cpp \
	scripts/zone/naxxramas/sc_grobbulus.cpp \
	scripts/zone/naxxramas/sc_heigan.cpp \
	scripts/zone/naxxramas/sc_highlord_mograine.cpp \
	scripts/zone/naxxramas/sc_kelthuzad.cpp \
	scripts/zone/naxxramas/sc_lady_blaumeux.cpp \
	scripts/zone/naxxramas/sc_loatheb.cpp \
	scripts/zone/naxxramas/sc_maexxna.cpp \
	scripts/zone/naxxramas/sc_noth.cpp \
	scripts/zone/naxxramas/sc_patchwerk.cpp \
	scripts/zone/naxxramas/sc_razuvious.cpp \
	scripts/zone/naxxramas/sc_sapphiron.cpp \
	scripts/zone/naxxramas/sc_sir_zeliek.cpp \
	scripts/zone/naxxramas/sc_stalagg.cpp \
	scripts/zone/naxxramas/sc_thaddius.cpp \
	scripts/zone/naxxramas/sc_thane_korthazz.cpp \
	scripts/zone/onyxias_lair/sc_boss_onyxia.cpp \
	scripts/zone/ruins_of_ahnqiraj/sc_ayamiss.cpp \
	scripts/zone/ruins_of_ahnqiraj/sc_buru.cpp \
	scripts/zone/ruins_of_ahnqiraj/sc_kurinnaxx.cpp \
	scripts/zone/ruins_of_ahnqiraj/sc_moam.cpp \
	scripts/zone/ruins_of_ahnqiraj/sc_ossirian.cpp \
	scripts/zone/ruins_of_ahnqiraj/sc_rajaxx.cpp \
	scripts/zone/scarlet_monastery/sc_boss_arcanist_doan.cpp \
	scripts/zone/scarlet_monastery/sc_boss_azshir_the_sleepless.cpp \
	scripts/zone/scarlet_monastery/sc_boss_bloodmage_thalnos.cpp \
	scripts/zone/scarlet_monastery/sc_boss_herod.cpp \
	scripts/zone/scarlet_monastery/sc_boss_high_inquisitor_fairbanks.cpp \
	scripts/zone/scarlet_monastery/sc_boss_high_inquisitor_whitemane.cpp \
	scripts/zone/scarlet_monastery/sc_boss_houndmaster_loksey.cpp \
	scripts/zone/scarlet_monastery/sc_boss_interrogator_vishas.cpp \
	scripts/zone/scarlet_monastery/sc_boss_scarlet_commander_mograine.cpp \
	scripts/zone/scarlet_monastery/sc_boss_scorn.cpp \
	scripts/zone/scarlet_monastery/sc_scarlet_torturer.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_cthun.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_fankriss.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_huhuran.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_kri.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_ouro.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_sartura.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_skeram.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_veklor.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_veknilash.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_vem.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_viscidus.cpp \
	scripts/zone/temple_of_ahnqiraj/sc_yauj.cpp \
	scripts/zone/zulgurub/sc_arlokk.cpp \
	scripts/zone/zulgurub/sc_gahzranka.cpp \
	scripts/zone/zulgurub/sc_grilek.cpp \
	scripts/zone/zulgurub/sc_hakkar.cpp \
	scripts/zone/zulgurub/sc_hazzarah.cpp \
	scripts/zone/zulgurub/sc_jeklik.cpp \
	scripts/zone/zulgurub/sc_jindo.cpp \
	scripts/zone/zulgurub/sc_mandokir.cpp \
	scripts/zone/zulgurub/sc_marli.cpp \
	scripts/zone/zulgurub/sc_renataki.cpp \
	scripts/zone/zulgurub/sc_thekal.cpp \
	scripts/zone/zulgurub/sc_venoxis.cpp \
	scripts/zone/zulgurub/sc_wushoolay.cpp \
	system.cpp

## libtool settings
# API versioning
# Link against dependencies
# How to increase version info:
# - only bug fixes implemented:
# bump the version to LTMANGOS_CURRENT:LTMANGOS_REVISION+1:LTMANGOS_AGE
# - augmented the interface:
# bump the version to LTMANGOS_CURRENT+1:0:LTMANGOS_AGE+1
# - broken old interface:
# bump the version to LTMANGOS_CURRENT+1:0:0
LTMANGOS_CURRENT = 0
LTMANGOS_REVISION = 0
LTMANGOS_AGE = 0
libmangosscript_la_LIBADD = $(MYSQL_LIBS)
libmangosscript_la_LIBFLAGS = -version-info $(LTMANGOS_CURRENT):$(LTMANGOS_REVISION):$(LTMANGOS_AGE)

## Additional files to include when running 'make dist'
# Scripts defaults.
EXTRA_DIST = \
	Scripts/sc_default.cpp \
	Scripts/sc_defines.cpp \
	Scripts/sc_defines.h

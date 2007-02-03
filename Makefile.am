# Copyright (C) 2005 MaNGOS <http://www.mangosproject.org/>
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

AM_CXXFLAGS = @COMPILER_OPTIONS@ @EXTRA_COMPILER_OPTIONS@
INCLUDES +=-I$(srcdir) -I$(srcdir)/../../../dep/include -I$(srcdir)/../../shared -I$(srcdir)/../../framework -I$(srcdir)/../../bindings/ScriptDev2/scripts

pkglibdir = $(libdir)

lib_LTLIBRARIES = libMaNGOSScript.la

libMaNGOSScript_la_LIBADD=@LINKER_OPTIONS@ @EXTRA_LINKER_OPTIONS@
libMaNGOSScript_la_LDFLAGS= \
	-release $(LTMS_RELEASE)\
	-version-info $(LTMS_CURRENT):$(LTMS_REVISION):$(LTMS_AGE)

LIBADD=@LINKER_OPTIONS@ @EXTRA_LINKER_OPTIONS@

libMaNGOSScript_la_SOURCES = \
ScriptMgr.cpp \
ScriptMgr.h \
config.h \
scripts/creature/sc_defias.cpp \
scripts/creature/sc_generic_creature.cpp \
scripts/creature/sc_kobold.cpp \
scripts/creature/sc_lumberjack.cpp \
scripts/creature/sc_miner.cpp \
scripts/custom/sc_custom_example.cpp \
scripts/guard/sc_guard_bluffwatcher.cpp \
scripts/guard/sc_guard_contested.cpp \
scripts/guard/sc_guard_darnassus.cpp \
scripts/guard/sc_guard_dunmorogh.cpp \
scripts/guard/sc_guard_durotar.cpp \
scripts/guard/sc_guard_elwynnforest.cpp \
scripts/guard/sc_guard_ironforge.cpp \
scripts/guard/sc_guard_mulgore.cpp \
scripts/guard/sc_guard_orgrimmar.cpp \
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
scripts/zone/blackwing_lair/sc_broodlord_lashlayer.cpp \
scripts/zone/blackwing_lair/sc_chromaggus.cpp \
scripts/zone/blackwing_lair/sc_ebenroc.cpp \
scripts/zone/blackwing_lair/sc_firemaw.cpp \
scripts/zone/blackwing_lair/sc_flamegor.cpp \
scripts/zone/blackwing_lair/sc_nefarian.cpp \
scripts/zone/blackwing_lair/sc_razorgore.cpp \
scripts/zone/blackwing_lair/sc_vaelastrasz.cpp \
scripts/zone/elwynn_forest/sc_henze_faulk.cpp \
scripts/zone/elwynn_forest/sc_marshal_mcbride.cpp \
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
scripts/zone/moonglade/sc_bunthen_plainswind.cpp \
scripts/zone/moonglade/sc_silva_filnaveth.cpp \
scripts/zone/mulgore/sc_skorn_whitecloud.cpp \
scripts/zone/onyxias_lair/sc_boss_onyxia.cpp \
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
system.cpp

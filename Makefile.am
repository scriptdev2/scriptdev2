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
scripts/boss/sc_boss_emeriss.cpp \
scripts/boss/sc_boss_lethon.cpp \
scripts/boss/sc_boss_taerar.cpp \
scripts/boss/sc_boss_ysondre.cpp \
scripts/creature/sc_defias.cpp \
scripts/creature/sc_generic_creature.cpp \
scripts/creature/sc_kobold.cpp \
scripts/creature/sc_lumberjack.cpp \
scripts/creature/sc_miner.cpp \
scripts/creature/sc_shade_of_taerar.cpp \
scripts/custom/sc_custom_example.cpp \
scripts/custom/sc_test.cpp \
scripts/go/sc_go_teleporter.cpp \
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
scripts/zone/azshara/sc_boss_azuregos.cpp \
scripts/zone/azuremyst_isle/sc_draenei_survivor.cpp \
scripts/zone/azuremyst_isle/sc_injured_draenei.cpp \
scripts/zone/blackrock_depths/sc_boss_ambassador_flamelash.cpp \
scripts/zone/blackrock_depths/sc_boss_angerrel.cpp \
scripts/zone/blackrock_depths/sc_boss_anubshiah.cpp \
scripts/zone/blackrock_depths/sc_boss_baelgar.cpp \
scripts/zone/blackrock_depths/sc_boss_doomrel.cpp \
scripts/zone/blackrock_depths/sc_boss_doperel.cpp \
scripts/zone/blackrock_depths/sc_boss_emperor_dagran_thaurissan.cpp \
scripts/zone/blackrock_depths/sc_boss_fineous_darkvire.cpp \
scripts/zone/blackrock_depths/sc_boss_general_angerforge.cpp \
scripts/zone/blackrock_depths/sc_boss_gloomrel.cpp \
scripts/zone/blackrock_depths/sc_boss_golem_lord_argelmach.cpp \
scripts/zone/blackrock_depths/sc_boss_gorosh_the_dervish.cpp \
scripts/zone/blackrock_depths/sc_boss_grizzle.cpp \
scripts/zone/blackrock_depths/sc_boss_haterel.cpp \
scripts/zone/blackrock_depths/sc_boss_high_interrogator_gerstahn.cpp \
scripts/zone/blackrock_depths/sc_boss_lord_incendius.cpp \
scripts/zone/blackrock_depths/sc_boss_lord_roccor.cpp \
scripts/zone/blackrock_depths/sc_boss_magmus.cpp \
scripts/zone/blackrock_depths/sc_boss_moira_bronzebeard.cpp \
scripts/zone/blackrock_depths/sc_boss_pyromancer_loregrain.cpp \
scripts/zone/blackrock_depths/sc_boss_seethrel.cpp \
scripts/zone/blackrock_depths/sc_boss_verek.cpp \
scripts/zone/blackrock_depths/sc_boss_vilerel.cpp \
scripts/zone/blackrock_depths/sc_boss_warder_stilgiss.cpp \
scripts/zone/blackrock_depths/sc_overmaster_pyron.cpp \
scripts/zone/blackrock_depths/sc_phalanx.cpp \
scripts/zone/blackrock_spire/sc_boss_drakkisath.cpp \
scripts/zone/blackrock_spire/sc_boss_halycon.cpp \
scripts/zone/blackrock_spire/sc_boss_highlord_omokk.cpp \
scripts/zone/blackrock_spire/sc_boss_mother_smolderweb.cpp \
scripts/zone/blackrock_spire/sc_boss_overlord_wyrmthalak.cpp \
scripts/zone/blackrock_spire/sc_boss_shadow_hunter_voshgajin.cpp \
scripts/zone/blackrock_spire/sc_boss_the_beast.cpp \
scripts/zone/blackrock_spire/sc_boss_warmaster_voone.cpp \
scripts/zone/blackrock_spire/sc_quartermaster_zigris.cpp \
scripts/zone/blackwing_lair/sc_broodlord_lashlayer.cpp \
scripts/zone/blackwing_lair/sc_chromaggus.cpp \
scripts/zone/blackwing_lair/sc_ebonroc.cpp \
scripts/zone/blackwing_lair/sc_firemaw.cpp \
scripts/zone/blackwing_lair/sc_flamegor.cpp \
scripts/zone/blackwing_lair/sc_nefarian.cpp \
scripts/zone/blackwing_lair/sc_razorgore.cpp \
scripts/zone/blackwing_lair/sc_vaelastrasz.cpp \
scripts/zone/blackwing_lair/sc_victor_nefarius.cpp \
scripts/zone/blasted_lands/sc_boss_kruul.cpp \
scripts/zone/blasted_lands/sc_teremus_the_devourer.cpp \
scripts/zone/burning_steppes/sc_lothos_riftwaker.cpp \
scripts/zone/dun_morogh/sc_npc_narm_faulk.cpp \
scripts/zone/eastern_plaguelands/sc_mob_q5211.cpp \
scripts/zone/eastern_plaguelands/sc_npc_darrowshire_spirit.cpp \
scripts/zone/elwynn_forest/sc_henze_faulk.cpp \
scripts/zone/elwynn_forest/sc_marshal_mcbride.cpp \
scripts/zone/eversong_woods/sc_mob_q8346.cpp \
scripts/zone/gruuls_lair/sc_boss_gruul.cpp \
scripts/zone/hellfire_peninsula/sc_boss_doomlord_kazzak.cpp \
scripts/zone/karazhan/sc_curator.cpp \
scripts/zone/karazhan/sc_maiden_of_virtue.cpp \
scripts/zone/karazhan/sc_midnight.cpp \
scripts/zone/karazhan/sc_moroes.cpp \
scripts/zone/karazhan/sc_netherspite.cpp \
scripts/zone/karazhan/sc_nightbane.cpp \
scripts/zone/karazhan/sc_prince_malchezaar.cpp \
scripts/zone/karazhan/sc_shade_of_aran.cpp \
scripts/zone/karazhan/sc_terestian_illhoof.cpp \
scripts/zone/maraudon/sc_boss_landslide.cpp \
scripts/zone/maraudon/sc_boss_lord_vyletongue.cpp \
scripts/zone/maraudon/sc_boss_noxxion.cpp \
scripts/zone/maraudon/sc_boss_princess_theradras.cpp \
scripts/zone/maraudon/sc_boss_rotgrip.cpp \
scripts/zone/maraudon/sc_boss_tinkerer_gizlock.cpp \
scripts/zone/maraudon/sc_celebras_the_cursed.cpp \
scripts/zone/maraudon/sc_meshlok_the_harvester.cpp \
scripts/zone/maraudon/sc_razorlash.cpp \
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
scripts/zone/razorfen_downs/sc_boss_amnennar_the_coldbringer.cpp \
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
scripts/zone/scholomance/sc_boss_doctor_theolen_krastinov.cpp \
scripts/zone/scholomance/sc_boss_illucia_barov.cpp \
scripts/zone/scholomance/sc_boss_instructor_malicia.cpp \
scripts/zone/scholomance/sc_boss_jandice_barov.cpp \
scripts/zone/scholomance/sc_boss_kormok.cpp \
scripts/zone/scholomance/sc_boss_lord_alexei_barov.cpp \
scripts/zone/scholomance/sc_boss_lorekeeper_polkelt.cpp \
scripts/zone/scholomance/sc_boss_ras_frostwhisper.cpp \
scripts/zone/scholomance/sc_boss_the_ravenian.cpp \
scripts/zone/scholomance/sc_boss_vectus.cpp \
scripts/zone/silithus/sc_npc_q8304.cpp \
scripts/zone/silithus/sc_npc_q8507_q8731.cpp \
scripts/zone/stratholme/sc_boss_archivist_galford.cpp \
scripts/zone/stratholme/sc_boss_baron_rivendare.cpp \
scripts/zone/stratholme/sc_boss_baroness_anastari.cpp \
scripts/zone/stratholme/sc_boss_black_guard_swordsmith.cpp \
scripts/zone/stratholme/sc_boss_cannon_master_willey.cpp \
scripts/zone/stratholme/sc_boss_crimson_hammersmith.cpp \
scripts/zone/stratholme/sc_boss_dathrohan_balnazzar.cpp \
scripts/zone/stratholme/sc_boss_fras_siabi.cpp \
scripts/zone/stratholme/sc_boss_hearthsinger_forresten.cpp \
scripts/zone/stratholme/sc_boss_magistrate_barthilas.cpp \
scripts/zone/stratholme/sc_boss_maleki_the_pallid.cpp \
scripts/zone/stratholme/sc_boss_malor_the_zealous.cpp \
scripts/zone/stratholme/sc_boss_nerubenkan.cpp \
scripts/zone/stratholme/sc_boss_postmaster_malown.cpp \
scripts/zone/stratholme/sc_boss_ramstein_the_gorger.cpp \
scripts/zone/stratholme/sc_boss_skul.cpp \
scripts/zone/stratholme/sc_boss_stonespine.cpp \
scripts/zone/stratholme/sc_boss_the_unforgiven.cpp \
scripts/zone/stratholme/sc_boss_timmy_the_cruel.cpp \
scripts/zone/stratholme/sc_mob_bile_slime.cpp \
scripts/zone/stratholme/sc_mob_bile_spewer.cpp \
scripts/zone/stratholme/sc_mob_black_guard_sentry.cpp \
scripts/zone/stratholme/sc_mob_broken_cadaver.cpp \
scripts/zone/stratholme/sc_mob_crimson_battle_mage.cpp \
scripts/zone/stratholme/sc_mob_crimson_conjuror.cpp \
scripts/zone/stratholme/sc_mob_crimson_defender.cpp \
scripts/zone/stratholme/sc_mob_crimson_gallant.cpp \
scripts/zone/stratholme/sc_mob_crimson_guardsman.cpp \
scripts/zone/stratholme/sc_mob_crimson_initiate.cpp \
scripts/zone/stratholme/sc_mob_crimson_inquisitor.cpp \
scripts/zone/stratholme/sc_mob_crimson_monk.cpp \
scripts/zone/stratholme/sc_mob_crimson_priest.cpp \
scripts/zone/stratholme/sc_mob_crimson_rifleman.cpp \
scripts/zone/stratholme/sc_mob_crimson_sorcerer.cpp \
scripts/zone/stratholme/sc_mob_crypt_beast.cpp \
scripts/zone/stratholme/sc_mob_crypt_crawler.cpp \
scripts/zone/stratholme/sc_mob_eye_of_naxxramas.cpp \
scripts/zone/stratholme/sc_mob_fleshflayer_ghoul.cpp \
scripts/zone/stratholme/sc_mob_freed_soul.cpp \
scripts/zone/stratholme/sc_mob_ghostly_citizen.cpp \
scripts/zone/stratholme/sc_mob_ghoul_ravener.cpp \
scripts/zone/stratholme/sc_mob_mangled_cadaver.cpp \
scripts/zone/stratholme/sc_mob_mindless_skeleton.cpp \
scripts/zone/stratholme/sc_mob_patchwork_horror.cpp \
scripts/zone/stratholme/sc_mob_plague_ghoul.cpp \
scripts/zone/stratholme/sc_mob_ravaged_cadaver.cpp \
scripts/zone/stratholme/sc_mob_restless_soul.cpp \
scripts/zone/stratholme/sc_mob_rockwing_gargoyle.cpp \
scripts/zone/stratholme/sc_mob_rockwing_screecher.cpp \
scripts/zone/stratholme/sc_mob_shrieking_banshee.cpp \
scripts/zone/stratholme/sc_mob_skeletal_berserker.cpp \
scripts/zone/stratholme/sc_mob_skeletal_guardian.cpp \
scripts/zone/stratholme/sc_mob_spectral_citizen.cpp \
scripts/zone/stratholme/sc_mob_spiteful_phantom.cpp \
scripts/zone/stratholme/sc_mob_thuzadin_acolyte.cpp \
scripts/zone/stratholme/sc_mob_thuzadin_necromancer.cpp \
scripts/zone/stratholme/sc_mob_thuzadin_shadowcaster.cpp \
scripts/zone/stratholme/sc_mob_undead_scarab.cpp \
scripts/zone/stratholme/sc_mob_vengeful_phantom.cpp \
scripts/zone/stratholme/sc_mob_venom_belcher.cpp \
scripts/zone/stratholme/sc_mob_wailing_banshee.cpp \
scripts/zone/stratholme/sc_mob_wrath_phantom.cpp \
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

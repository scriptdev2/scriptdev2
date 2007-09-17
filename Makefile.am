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
scripts/boss/boss_emeriss.cpp \
scripts/boss/boss_lethon.cpp \
scripts/boss/boss_taerar.cpp \
scripts/boss/boss_ysondre.cpp \
scripts/creature/mob_defias.cpp \
scripts/creature/mob_generic_creature.cpp \
scripts/creature/mob_kobold.cpp \
scripts/creature/npc_lumberjack.cpp \
scripts/creature/npc_miner.cpp \
scripts/creature/simple_ai.cpp \
scripts/creature/simple_ai.h \
scripts/custom/custom_example.cpp \
scripts/custom/test.cpp \
scripts/go/go_orb_of_command.cpp \
scripts/go/go_teleporter.cpp \
scripts/guard/guard_ai.cpp \
scripts/guard/guard_ai.h \
scripts/guard/guards.cpp \
scripts/honor/honor_vendor.cpp \
scripts/item/area_52_special.cpp \
scripts/item/item_test.cpp \
scripts/item/nether_wraith_beacon.cpp \
scripts/item/purification_mixture.cpp \
scripts/npc/npc_guardian.cpp \
scripts/npc/npc_not_selectable.cpp \
scripts/sc_defines.cpp \
scripts/sc_defines.h \
scripts/zone/azshara/boss_azuregos.cpp \
scripts/zone/azuremyst_isle/npc_draenei_survivor.cpp \
scripts/zone/azuremyst_isle/npc_injured_draenei.cpp \
scripts/zone/black_temple/instance_black_temple.cpp \
scripts/zone/black_temple/mob_blacktemple.cpp \
scripts/zone/blackrock_depths/boss_ambassador_flamelash.cpp \
scripts/zone/blackrock_depths/boss_angerrel.cpp \
scripts/zone/blackrock_depths/boss_anubshiah.cpp \
scripts/zone/blackrock_depths/boss_baelgar.cpp \
scripts/zone/blackrock_depths/boss_doomrel.cpp \
scripts/zone/blackrock_depths/boss_doperel.cpp \
scripts/zone/blackrock_depths/boss_emperor_dagran_thaurissan.cpp \
scripts/zone/blackrock_depths/boss_fineous_darkvire.cpp \
scripts/zone/blackrock_depths/boss_general_angerforge.cpp \
scripts/zone/blackrock_depths/boss_gloomrel.cpp \
scripts/zone/blackrock_depths/boss_golem_lord_argelmach.cpp \
scripts/zone/blackrock_depths/boss_gorosh_the_dervish.cpp \
scripts/zone/blackrock_depths/boss_grizzle.cpp \
scripts/zone/blackrock_depths/boss_haterel.cpp \
scripts/zone/blackrock_depths/boss_high_interrogator_gerstahn.cpp \
scripts/zone/blackrock_depths/boss_lord_incendius.cpp \
scripts/zone/blackrock_depths/boss_lord_roccor.cpp \
scripts/zone/blackrock_depths/boss_magmus.cpp \
scripts/zone/blackrock_depths/boss_moira_bronzebeard.cpp \
scripts/zone/blackrock_depths/boss_pyromancer_loregrain.cpp \
scripts/zone/blackrock_depths/boss_seethrel.cpp \
scripts/zone/blackrock_depths/boss_verek.cpp \
scripts/zone/blackrock_depths/boss_vilerel.cpp \
scripts/zone/blackrock_depths/boss_warder_stilgiss.cpp \
scripts/zone/blackrock_depths/mob_overmaster_pyron.cpp \
scripts/zone/blackrock_depths/mob_phalanx.cpp \
scripts/zone/blackrock_spire/boss_drakkisath.cpp \
scripts/zone/blackrock_spire/boss_gyth.cpp \
scripts/zone/blackrock_spire/boss_halycon.cpp \
scripts/zone/blackrock_spire/boss_highlord_omokk.cpp \
scripts/zone/blackrock_spire/boss_mother_smolderweb.cpp \
scripts/zone/blackrock_spire/boss_overlord_wyrmthalak.cpp \
scripts/zone/blackrock_spire/boss_pyroguard_emberseer.cpp \
scripts/zone/blackrock_spire/boss_rend_blackhand.cpp \
scripts/zone/blackrock_spire/boss_shadow_hunter_voshgajin.cpp \
scripts/zone/blackrock_spire/boss_the_beast.cpp \
scripts/zone/blackrock_spire/boss_warmaster_voone.cpp \
scripts/zone/blackrock_spire/mob_chromatic_elite_guard.cpp \
scripts/zone/blackrock_spire/mob_quartermaster_zigris.cpp \
scripts/zone/blackwing_lair/boss_broodlord_lashlayer.cpp \
scripts/zone/blackwing_lair/boss_chromaggus.cpp \
scripts/zone/blackwing_lair/boss_ebonroc.cpp \
scripts/zone/blackwing_lair/boss_firemaw.cpp \
scripts/zone/blackwing_lair/boss_flamegor.cpp \
scripts/zone/blackwing_lair/boss_nefarian.cpp \
scripts/zone/blackwing_lair/boss_razorgore.cpp \
scripts/zone/blackwing_lair/boss_vaelastrasz.cpp \
scripts/zone/blackwing_lair/instance_blackwing_lair.cpp \
scripts/zone/blackwing_lair/npc_victor_nefarius.cpp \
scripts/zone/blasted_lands/boss_kruul.cpp \
scripts/zone/blasted_lands/mob_teremus_the_devourer.cpp \
scripts/zone/caverns_of_time/hyjal/instance_hyjal.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/instance_serpent_shrine.cpp \
scripts/zone/coilfang_resevoir/steam_vault/boss_warlord_kalithresh.cpp \
scripts/zone/darkshore/mobs_darkshore.cpp \
scripts/zone/dun_morogh/npc_narm_faulk.cpp \
scripts/zone/eastern_plaguelands/mobs_ghoul_flayer.cpp \
scripts/zone/eastern_plaguelands/npc_darrowshire_spirit.cpp \
scripts/zone/elwynn_forest/npc_henze_faulk.cpp \
scripts/zone/eversong_woods/mobs_mana_tapped.cpp \
scripts/zone/gruuls_lair/boss_gruul.cpp \
scripts/zone/gruuls_lair/instance_gruuls_lair.cpp \
scripts/zone/hellfire_citadel/magtheridons_lair/boss_magtheridon.cpp \
scripts/zone/hellfire_citadel/magtheridons_lair/instance_magtheridons_lair.cpp \
scripts/zone/hellfire_citadel/magtheridons_lair/mob_hellfire_channeler.cpp \
scripts/zone/hellfire_citadel/magtheridons_lair/mob_hellfire_warder.cpp \
scripts/zone/hellfire_peninsula/boss_doomlord_kazzak.cpp \
scripts/zone/hellfire_peninsula/mobs_bonechewer_orc.cpp \
scripts/zone/karazhan/boss_curator.cpp \
scripts/zone/karazhan/boss_maiden_of_virtue.cpp \
scripts/zone/karazhan/boss_midnight.cpp \
scripts/zone/karazhan/boss_moroes.cpp \
scripts/zone/karazhan/boss_netherspite.cpp \
scripts/zone/karazhan/boss_nightbane.cpp \
scripts/zone/karazhan/boss_prince_malchezaar.cpp \
scripts/zone/karazhan/boss_shade_of_aran.cpp \
scripts/zone/karazhan/boss_terestian_illhoof.cpp \
scripts/zone/karazhan/instance_karazhan.cpp \
scripts/zone/karazhan/mob_homunculus.cpp \
scripts/zone/karazhan/mob_kilrek.cpp \
scripts/zone/karazhan/mob_netherspite_infernal.cpp \
scripts/zone/maraudon/boss_landslide.cpp \
scripts/zone/maraudon/boss_lord_vyletongue.cpp \
scripts/zone/maraudon/boss_noxxion.cpp \
scripts/zone/maraudon/boss_princess_theradras.cpp \
scripts/zone/maraudon/boss_rotgrip.cpp \
scripts/zone/maraudon/boss_tinkerer_gizlock.cpp \
scripts/zone/maraudon/mob_celebras_the_cursed.cpp \
scripts/zone/maraudon/mob_meshlok_the_harvester.cpp \
scripts/zone/maraudon/mob_razorlash.cpp \
scripts/zone/molten_core/boss_baron_geddon.cpp \
scripts/zone/molten_core/boss_garr.cpp \
scripts/zone/molten_core/boss_gehennas.cpp \
scripts/zone/molten_core/boss_golemagg.cpp \
scripts/zone/molten_core/boss_lucifron.cpp \
scripts/zone/molten_core/boss_magmadar.cpp \
scripts/zone/molten_core/boss_majordomo_executus.cpp \
scripts/zone/molten_core/boss_ragnaros.cpp \
scripts/zone/molten_core/boss_shazzrah.cpp \
scripts/zone/molten_core/boss_sulfuron_harbinger.cpp \
scripts/zone/molten_core/instance_molten_core.cpp \
scripts/zone/molten_core/mob_ancient_core_hound.cpp \
scripts/zone/molten_core/mob_core_rager.cpp \
scripts/zone/molten_core/mob_firelord.cpp \
scripts/zone/molten_core/mob_firewalker.cpp \
scripts/zone/molten_core/mob_flame_guard.cpp \
scripts/zone/molten_core/mob_flamewaker.cpp \
scripts/zone/molten_core/mob_flamewaker_elite.cpp \
scripts/zone/molten_core/mob_flamewaker_healer.cpp \
scripts/zone/molten_core/mob_flamewaker_protector.cpp \
scripts/zone/molten_core/mob_lavaspawn.cpp \
scripts/zone/molten_core/mob_molten_destroyer.cpp \
scripts/zone/molten_core/mob_molten_giant.cpp \
scripts/zone/moonglade/npc_bunthen_plainswind.cpp \
scripts/zone/moonglade/npc_great_bear_spirit.cpp \
scripts/zone/moonglade/npc_silva_filnaveth.cpp \
scripts/zone/mulgore/npc_skorn_whitecloud.cpp \
scripts/zone/nagrand/mobs_nagrand.cpp \
scripts/zone/nagrand/npcs_nagrand.cpp \
scripts/zone/naxxramas/boss_anubrekhan.cpp \
scripts/zone/naxxramas/boss_faerlina.cpp \
scripts/zone/naxxramas/boss_feugen.cpp \
scripts/zone/naxxramas/boss_gluth.cpp \
scripts/zone/naxxramas/boss_gothik.cpp \
scripts/zone/naxxramas/boss_grobbulus.cpp \
scripts/zone/naxxramas/boss_heigan.cpp \
scripts/zone/naxxramas/boss_highlord_mograine.cpp \
scripts/zone/naxxramas/boss_kelthuzad.cpp \
scripts/zone/naxxramas/boss_lady_blaumeux.cpp \
scripts/zone/naxxramas/boss_loatheb.cpp \
scripts/zone/naxxramas/boss_maexxna.cpp \
scripts/zone/naxxramas/boss_noth.cpp \
scripts/zone/naxxramas/boss_patchwerk.cpp \
scripts/zone/naxxramas/boss_razuvious.cpp \
scripts/zone/naxxramas/boss_sapphiron.cpp \
scripts/zone/naxxramas/boss_sir_zeliek.cpp \
scripts/zone/naxxramas/boss_stalagg.cpp \
scripts/zone/naxxramas/boss_thaddius.cpp \
scripts/zone/naxxramas/boss_thane_korthazz.cpp \
scripts/zone/naxxramas/instance_naxxramas.cpp \
scripts/zone/onyxias_lair/boss_onyxia.cpp \
scripts/zone/razorfen_downs/boss_amnennar_the_coldbringer.cpp \
scripts/zone/ruins_of_ahnqiraj/boss_ayamiss.cpp \
scripts/zone/ruins_of_ahnqiraj/boss_buru.cpp \
scripts/zone/ruins_of_ahnqiraj/boss_kurinnaxx.cpp \
scripts/zone/ruins_of_ahnqiraj/boss_moam.cpp \
scripts/zone/ruins_of_ahnqiraj/boss_ossirian.cpp \
scripts/zone/ruins_of_ahnqiraj/boss_rajaxx.cpp \
scripts/zone/ruins_of_ahnqiraj/instance_ruins_of_ahnqiraj.cpp \
scripts/zone/scarlet_monastery/boss_arcanist_doan.cpp \
scripts/zone/scarlet_monastery/boss_azshir_the_sleepless.cpp \
scripts/zone/scarlet_monastery/boss_bloodmage_thalnos.cpp \
scripts/zone/scarlet_monastery/boss_herod.cpp \
scripts/zone/scarlet_monastery/boss_high_inquisitor_fairbanks.cpp \
scripts/zone/scarlet_monastery/boss_high_inquisitor_whitemane.cpp \
scripts/zone/scarlet_monastery/boss_houndmaster_loksey.cpp \
scripts/zone/scarlet_monastery/boss_interrogator_vishas.cpp \
scripts/zone/scarlet_monastery/boss_scarlet_commander_mograine.cpp \
scripts/zone/scarlet_monastery/boss_scorn.cpp \
scripts/zone/scarlet_monastery/mob_scarlet_torturer.cpp \
scripts/zone/scholomance/boss_death_knight_darkreaver.cpp \
scripts/zone/scholomance/boss_doctor_theolen_krastinov.cpp \
scripts/zone/scholomance/boss_illucia_barov.cpp \
scripts/zone/scholomance/boss_instructor_malicia.cpp \
scripts/zone/scholomance/boss_jandice_barov.cpp \
scripts/zone/scholomance/boss_kormok.cpp \
scripts/zone/scholomance/boss_lord_alexei_barov.cpp \
scripts/zone/scholomance/boss_lorekeeper_polkelt.cpp \
scripts/zone/scholomance/boss_ras_frostwhisper.cpp \
scripts/zone/scholomance/boss_the_ravenian.cpp \
scripts/zone/scholomance/boss_vectus.cpp \
scripts/zone/searing_gorge/npc_lothos_riftwalker.cpp \
scripts/zone/shadowmoon_valley/boss_doomwalker.cpp \
scripts/zone/shadowmoon_valley/npc_neltharaku.cpp \
scripts/zone/silithus/npcs_captains_blackanvil_and_skullsplit.cpp \
scripts/zone/silithus/npcs_rutgar_and_frankal.cpp \
scripts/zone/silvermoon/npc_blood_knight_stillblade.cpp \
scripts/zone/stormwind/npc_bartleby.cpp \
scripts/zone/stormwind/npc_dashel_stonefist.cpp \
scripts/zone/stranglethorn_vale/mobs_stranglethorn_vale.cpp \
scripts/zone/stratholme/boss_archivist_galford.cpp \
scripts/zone/stratholme/boss_baron_rivendare.cpp \
scripts/zone/stratholme/boss_baroness_anastari.cpp \
scripts/zone/stratholme/boss_black_guard_swordsmith.cpp \
scripts/zone/stratholme/boss_cannon_master_willey.cpp \
scripts/zone/stratholme/boss_crimson_hammersmith.cpp \
scripts/zone/stratholme/boss_dathrohan_balnazzar.cpp \
scripts/zone/stratholme/boss_fras_siabi.cpp \
scripts/zone/stratholme/boss_hearthsinger_forresten.cpp \
scripts/zone/stratholme/boss_magistrate_barthilas.cpp \
scripts/zone/stratholme/boss_maleki_the_pallid.cpp \
scripts/zone/stratholme/boss_malor_the_zealous.cpp \
scripts/zone/stratholme/boss_nerubenkan.cpp \
scripts/zone/stratholme/boss_postmaster_malown.cpp \
scripts/zone/stratholme/boss_ramstein_the_gorger.cpp \
scripts/zone/stratholme/boss_skul.cpp \
scripts/zone/stratholme/boss_stonespine.cpp \
scripts/zone/stratholme/boss_the_unforgiven.cpp \
scripts/zone/stratholme/boss_timmy_the_cruel.cpp \
scripts/zone/stratholme/mobs_stratholme.cpp \
scripts/zone/tanaris/npcs_tanaris.cpp \
scripts/zone/tempest_keep/the_eye/instance_the_eye.cpp \
scripts/zone/tempest_keep/the_eye/mob_crystalcore_devastator.cpp \
scripts/zone/tempest_keep/the_eye/mob_the_eye.cpp \
scripts/zone/temple_of_ahnqiraj/boss_cthun.cpp \
scripts/zone/temple_of_ahnqiraj/boss_fankriss.cpp \
scripts/zone/temple_of_ahnqiraj/boss_huhuran.cpp \
scripts/zone/temple_of_ahnqiraj/boss_kri.cpp \
scripts/zone/temple_of_ahnqiraj/boss_ouro.cpp \
scripts/zone/temple_of_ahnqiraj/boss_sartura.cpp \
scripts/zone/temple_of_ahnqiraj/boss_skeram.cpp \
scripts/zone/temple_of_ahnqiraj/boss_veklor.cpp \
scripts/zone/temple_of_ahnqiraj/boss_veknilash.cpp \
scripts/zone/temple_of_ahnqiraj/boss_vem.cpp \
scripts/zone/temple_of_ahnqiraj/boss_viscidus.cpp \
scripts/zone/temple_of_ahnqiraj/boss_yauj.cpp \
scripts/zone/temple_of_ahnqiraj/instance_temple_of_ahnqiraj.cpp \
scripts/zone/terokkar_forest/mobs_terokkar_forest.cpp \
scripts/zone/uldaman/boss_ironaya.cpp \
scripts/zone/uldaman/mob_jadespine_basilisk.cpp \
scripts/zone/uldaman/mob_uldaman.cpp \
scripts/zone/zangramarsh/npcs_zangarmarsh.cpp \
scripts/zone/zulgurub/boss_arlokk.cpp \
scripts/zone/zulgurub/boss_gahzranka.cpp \
scripts/zone/zulgurub/boss_grilek.cpp \
scripts/zone/zulgurub/boss_hakkar.cpp \
scripts/zone/zulgurub/boss_hazzarah.cpp \
scripts/zone/zulgurub/boss_jeklik.cpp \
scripts/zone/zulgurub/boss_jindo.cpp \
scripts/zone/zulgurub/boss_mandokir.cpp \
scripts/zone/zulgurub/boss_marli.cpp \
scripts/zone/zulgurub/boss_renataki.cpp \
scripts/zone/zulgurub/boss_thekal.cpp \
scripts/zone/zulgurub/boss_venoxis.cpp \
scripts/zone/zulgurub/boss_wushoolay.cpp \
scripts/zone/zulgurub/instance_zulgurub.cpp \
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

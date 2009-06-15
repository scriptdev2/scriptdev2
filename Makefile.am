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

BUILT_SOURCES = revision.h
CLEANFILES = revision.h
## CPP flags for includes, defines, etc.
AM_CPPFLAGS = $(MANGOS_INCLUDES) -I$(srcdir) -I$(srcdir)/../../../dep/include -I$(srcdir)/../../shared/ -I$(srcdir)/../../framework/ -I$(srcdir)/../../game/ -I$(srcdir)/include/

## Build MaNGOS script library as shared library.
# libmangosscript shared library will later be reused by world server daemon.
lib_LTLIBRARIES = libmangosscript.la
libmangosscript_la_SOURCES = \
ScriptMgr.cpp \
ScriptMgr.h \
config.h \
include/precompiled.cpp \
include/precompiled.h \
include/sc_creature.cpp \
include/sc_creature.h \
include/sc_gossip.h \
include/sc_grid_searchers.cpp \
include/sc_grid_searchers.h \
include/sc_instance.cpp \
include/sc_instance.h \
scripts/areatrigger/areatrigger_scripts.cpp \
scripts/boss/boss_emeriss.cpp \
scripts/boss/boss_lethon.cpp \
scripts/boss/boss_taerar.cpp \
scripts/boss/boss_ysondre.cpp \
scripts/creature/mob_generic_creature.cpp \
scripts/creature/simple_ai.cpp \
scripts/creature/simple_ai.h \
scripts/examples/example_creature.cpp \
scripts/examples/example_escort.cpp \
scripts/examples/example_gossip_codebox.cpp \
scripts/examples/example_misc.cpp \
scripts/go/go_scripts.cpp \
scripts/guard/guard_ai.cpp \
scripts/guard/guard_ai.h \
scripts/guard/guards.cpp \
scripts/item/item_scripts.cpp \
scripts/npc/npc_escortAI.cpp \
scripts/npc/npc_escortAI.h \
scripts/npc/npc_innkeeper.cpp \
scripts/npc/npc_professions.cpp \
scripts/npc/npcs_special.cpp \
scripts/zone/alterac_mountains/alterac_mountains.cpp \
scripts/zone/arathi_highlands/arathi_highlands.cpp \
scripts/zone/ashenvale_forest/ashenvale.cpp \
scripts/zone/aunchindoun/auchenai_crypts/boss_exarch_maladaar.cpp \
scripts/zone/aunchindoun/mana_tombs/boss_nexusprince_shaffar.cpp \
scripts/zone/aunchindoun/mana_tombs/boss_pandemonius.cpp \
scripts/zone/aunchindoun/sethekk_halls/boss_darkweaver_syth.cpp \
scripts/zone/aunchindoun/sethekk_halls/boss_tailonking_ikiss.cpp \
scripts/zone/aunchindoun/sethekk_halls/def_sethekk_halls.h \
scripts/zone/aunchindoun/sethekk_halls/instance_sethekk_halls.cpp \
scripts/zone/aunchindoun/shadow_labyrinth/boss_ambassador_hellmaw.cpp \
scripts/zone/aunchindoun/shadow_labyrinth/boss_blackheart_the_inciter.cpp \
scripts/zone/aunchindoun/shadow_labyrinth/boss_grandmaster_vorpil.cpp \
scripts/zone/aunchindoun/shadow_labyrinth/boss_murmur.cpp \
scripts/zone/aunchindoun/shadow_labyrinth/def_shadow_labyrinth.h \
scripts/zone/aunchindoun/shadow_labyrinth/instance_shadow_labyrinth.cpp \
scripts/zone/azjol-nerub/azjol-nerub/def_azjol-nerub.h \
scripts/zone/azjol-nerub/azjol-nerub/instance_azjol-nerub.cpp \
scripts/zone/azshara/azshara.cpp \
scripts/zone/azshara/boss_azuregos.cpp \
scripts/zone/azuremyst_isle/azuremyst_isle.cpp \
scripts/zone/barrens/the_barrens.cpp \
scripts/zone/black_temple/black_temple.cpp \
scripts/zone/black_temple/boss_bloodboil.cpp \
scripts/zone/black_temple/boss_illidan.cpp \
scripts/zone/black_temple/boss_mother_shahraz.cpp \
scripts/zone/black_temple/boss_reliquary_of_souls.cpp \
scripts/zone/black_temple/boss_shade_of_akama.cpp \
scripts/zone/black_temple/boss_supremus.cpp \
scripts/zone/black_temple/boss_teron_gorefiend.cpp \
scripts/zone/black_temple/boss_warlord_najentus.cpp \
scripts/zone/black_temple/def_black_temple.h \
scripts/zone/black_temple/illidari_council.cpp \
scripts/zone/black_temple/instance_black_temple.cpp \
scripts/zone/blackfathom_deeps/def_blackfathom_deeps.h \
scripts/zone/blackfathom_deeps/instance_blackfathom_deeps.cpp \
scripts/zone/blackrock_depths/blackrock_depths.cpp \
scripts/zone/blackrock_depths/boss_ambassador_flamelash.cpp \
scripts/zone/blackrock_depths/boss_anubshiah.cpp \
scripts/zone/blackrock_depths/boss_emperor_dagran_thaurissan.cpp \
scripts/zone/blackrock_depths/boss_general_angerforge.cpp \
scripts/zone/blackrock_depths/boss_gorosh_the_dervish.cpp \
scripts/zone/blackrock_depths/boss_grizzle.cpp \
scripts/zone/blackrock_depths/boss_high_interrogator_gerstahn.cpp \
scripts/zone/blackrock_depths/boss_magmus.cpp \
scripts/zone/blackrock_depths/boss_moira_bronzebeard.cpp \
scripts/zone/blackrock_depths/boss_tomb_of_seven.cpp \
scripts/zone/blackrock_depths/def_blackrock_depths.h \
scripts/zone/blackrock_depths/instance_blackrock_depths.cpp \
scripts/zone/blackrock_spire/boss_drakkisath.cpp \
scripts/zone/blackrock_spire/boss_gyth.cpp \
scripts/zone/blackrock_spire/boss_halycon.cpp \
scripts/zone/blackrock_spire/boss_highlord_omokk.cpp \
scripts/zone/blackrock_spire/boss_mother_smolderweb.cpp \
scripts/zone/blackrock_spire/boss_overlord_wyrmthalak.cpp \
scripts/zone/blackrock_spire/boss_pyroguard_emberseer.cpp \
scripts/zone/blackrock_spire/boss_quartermaster_zigris.cpp \
scripts/zone/blackrock_spire/boss_rend_blackhand.cpp \
scripts/zone/blackrock_spire/boss_shadow_hunter_voshgajin.cpp \
scripts/zone/blackrock_spire/boss_the_beast.cpp \
scripts/zone/blackrock_spire/boss_warmaster_voone.cpp \
scripts/zone/blackwing_lair/boss_broodlord_lashlayer.cpp \
scripts/zone/blackwing_lair/boss_chromaggus.cpp \
scripts/zone/blackwing_lair/boss_ebonroc.cpp \
scripts/zone/blackwing_lair/boss_firemaw.cpp \
scripts/zone/blackwing_lair/boss_flamegor.cpp \
scripts/zone/blackwing_lair/boss_nefarian.cpp \
scripts/zone/blackwing_lair/boss_razorgore.cpp \
scripts/zone/blackwing_lair/boss_vaelastrasz.cpp \
scripts/zone/blackwing_lair/boss_victor_nefarius.cpp \
scripts/zone/blackwing_lair/instance_blackwing_lair.cpp \
scripts/zone/blades_edge_mountains/blades_edge_mountains.cpp \
scripts/zone/blasted_lands/blasted_lands.cpp \
scripts/zone/blasted_lands/boss_kruul.cpp \
scripts/zone/bloodmyst_isle/bloodmyst_isle.cpp \
scripts/zone/borean_tundra/borean_tundra.cpp \
scripts/zone/burning_steppes/burning_steppes.cpp \
scripts/zone/caverns_of_time/dark_portal/boss_aeonus.cpp \
scripts/zone/caverns_of_time/dark_portal/boss_chrono_lord_deja.cpp \
scripts/zone/caverns_of_time/dark_portal/boss_temporus.cpp \
scripts/zone/caverns_of_time/dark_portal/dark_portal.cpp \
scripts/zone/caverns_of_time/dark_portal/def_dark_portal.h \
scripts/zone/caverns_of_time/dark_portal/instance_dark_portal.cpp \
scripts/zone/caverns_of_time/hyjal/boss_archimonde.cpp \
scripts/zone/caverns_of_time/hyjal/def_hyjal.h \
scripts/zone/caverns_of_time/hyjal/hyjal.cpp \
scripts/zone/caverns_of_time/hyjal/hyjalAI.cpp \
scripts/zone/caverns_of_time/hyjal/hyjalAI.h \
scripts/zone/caverns_of_time/hyjal/instance_hyjal.cpp \
scripts/zone/caverns_of_time/old_hillsbrad/boss_captain_skarloc.cpp \
scripts/zone/caverns_of_time/old_hillsbrad/boss_epoch_hunter.cpp \
scripts/zone/caverns_of_time/old_hillsbrad/boss_leutenant_drake.cpp \
scripts/zone/caverns_of_time/old_hillsbrad/def_old_hillsbrad.h \
scripts/zone/caverns_of_time/old_hillsbrad/instance_old_hillsbrad.cpp \
scripts/zone/caverns_of_time/old_hillsbrad/old_hillsbrad.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/boss_fathomlord_karathress.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/boss_hydross_the_unstable.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/boss_lady_vashj.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/boss_leotheras_the_blind.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/boss_morogrim_tidewalker.cpp \
scripts/zone/coilfang_resevoir/serpent_shrine/def_serpent_shrine.h \
scripts/zone/coilfang_resevoir/serpent_shrine/instance_serpent_shrine.cpp \
scripts/zone/coilfang_resevoir/steam_vault/boss_hydromancer_thespia.cpp \
scripts/zone/coilfang_resevoir/steam_vault/boss_mekgineer_steamrigger.cpp \
scripts/zone/coilfang_resevoir/steam_vault/boss_warlord_kalithresh.cpp \
scripts/zone/coilfang_resevoir/steam_vault/def_steam_vault.h \
scripts/zone/coilfang_resevoir/steam_vault/instance_steam_vault.cpp \
scripts/zone/coilfang_resevoir/underbog/boss_hungarfen.cpp \
scripts/zone/darkshore/darkshore.cpp \
scripts/zone/deadmines/deadmines.cpp \
scripts/zone/deadmines/def_deadmines.h \
scripts/zone/deadmines/instance_deadmines.cpp \
scripts/zone/desolace/desolace.cpp \
scripts/zone/dragonblight/dragonblight.cpp \
scripts/zone/dun_morogh/dun_morogh.cpp \
scripts/zone/dustwallow_marsh/dustwallow_marsh.cpp \
scripts/zone/eastern_plaguelands/eastern_plaguelands.cpp \
scripts/zone/ebon_hold/ebon_hold.cpp \
scripts/zone/elwynn_forest/elwynn_forest.cpp \
scripts/zone/eversong_woods/eversong_woods.cpp \
scripts/zone/felwood/felwood.cpp \
scripts/zone/feralas/feralas.cpp \
scripts/zone/ghostlands/ghostlands.cpp \
scripts/zone/gruuls_lair/boss_gruul.cpp \
scripts/zone/gruuls_lair/boss_high_king_maulgar.cpp \
scripts/zone/gruuls_lair/def_gruuls_lair.h \
scripts/zone/gruuls_lair/instance_gruuls_lair.cpp \
scripts/zone/hellfire_citadel/blood_furnace/boss_broggok.cpp \
scripts/zone/hellfire_citadel/blood_furnace/boss_kelidan_the_breaker.cpp \
scripts/zone/hellfire_citadel/blood_furnace/boss_the_maker.cpp \
scripts/zone/hellfire_citadel/blood_furnace/def_blood_furnace.h \
scripts/zone/hellfire_citadel/blood_furnace/instance_blood_furnace.cpp \
scripts/zone/hellfire_citadel/hellfire_ramparts/boss_nazan_and_vazruden.cpp \
scripts/zone/hellfire_citadel/hellfire_ramparts/boss_omor_the_unscarred.cpp \
scripts/zone/hellfire_citadel/hellfire_ramparts/boss_watchkeeper_gargolmar.cpp \
scripts/zone/hellfire_citadel/hellfire_ramparts/def_hellfire_ramparts.h \
scripts/zone/hellfire_citadel/hellfire_ramparts/instance_hellfire_ramparts.cpp \
scripts/zone/hellfire_citadel/magtheridons_lair/boss_magtheridon.cpp \
scripts/zone/hellfire_citadel/magtheridons_lair/def_magtheridons_lair.h \
scripts/zone/hellfire_citadel/magtheridons_lair/instance_magtheridons_lair.cpp \
scripts/zone/hellfire_citadel/shattered_halls/boss_nethekurse.cpp \
scripts/zone/hellfire_citadel/shattered_halls/boss_warbringer_omrogg.cpp \
scripts/zone/hellfire_citadel/shattered_halls/boss_warchief_kargath_bladefist.cpp \
scripts/zone/hellfire_citadel/shattered_halls/def_shattered_halls.h \
scripts/zone/hellfire_citadel/shattered_halls/instance_shattered_halls.cpp \
scripts/zone/hellfire_peninsula/boss_doomlord_kazzak.cpp \
scripts/zone/hellfire_peninsula/hellfire_peninsula.cpp \
scripts/zone/ironforge/ironforge.cpp \
scripts/zone/isle_of_queldanas/isle_of_queldanas.cpp \
scripts/zone/karazhan/boss_curator.cpp \
scripts/zone/karazhan/boss_maiden_of_virtue.cpp \
scripts/zone/karazhan/boss_midnight.cpp \
scripts/zone/karazhan/boss_moroes.cpp \
scripts/zone/karazhan/boss_netherspite.cpp \
scripts/zone/karazhan/boss_nightbane.cpp \
scripts/zone/karazhan/boss_prince_malchezaar.cpp \
scripts/zone/karazhan/boss_shade_of_aran.cpp \
scripts/zone/karazhan/boss_terestian_illhoof.cpp \
scripts/zone/karazhan/bosses_opera.cpp \
scripts/zone/karazhan/def_karazhan.h \
scripts/zone/karazhan/instance_karazhan.cpp \
scripts/zone/karazhan/karazhan.cpp \
scripts/zone/loch_modan/loch_modan.cpp \
scripts/zone/magisters_terrace/boss_felblood_kaelthas.cpp \
scripts/zone/magisters_terrace/boss_priestess_delrissa.cpp \
scripts/zone/magisters_terrace/boss_selin_fireheart.cpp \
scripts/zone/magisters_terrace/boss_vexallus.cpp \
scripts/zone/magisters_terrace/def_magisters_terrace.h \
scripts/zone/magisters_terrace/instance_magisters_terrace.cpp \
scripts/zone/magisters_terrace/magisters_terrace.cpp \
scripts/zone/maraudon/boss_celebras_the_cursed.cpp \
scripts/zone/maraudon/boss_landslide.cpp \
scripts/zone/maraudon/boss_noxxion.cpp \
scripts/zone/maraudon/boss_princess_theradras.cpp \
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
scripts/zone/molten_core/def_molten_core.h \
scripts/zone/molten_core/instance_molten_core.cpp \
scripts/zone/molten_core/molten_core.cpp \
scripts/zone/moonglade/moonglade.cpp \
scripts/zone/mulgore/mulgore.cpp \
scripts/zone/nagrand/nagrand.cpp \
scripts/zone/naxxramas/boss_anubrekhan.cpp \
scripts/zone/naxxramas/boss_faerlina.cpp \
scripts/zone/naxxramas/boss_four_horsemen.cpp \
scripts/zone/naxxramas/boss_gluth.cpp \
scripts/zone/naxxramas/boss_gothik.cpp \
scripts/zone/naxxramas/boss_grobbulus.cpp \
scripts/zone/naxxramas/boss_heigan.cpp \
scripts/zone/naxxramas/boss_kelthuzad.cpp \
scripts/zone/naxxramas/boss_loatheb.cpp \
scripts/zone/naxxramas/boss_maexxna.cpp \
scripts/zone/naxxramas/boss_noth.cpp \
scripts/zone/naxxramas/boss_patchwerk.cpp \
scripts/zone/naxxramas/boss_razuvious.cpp \
scripts/zone/naxxramas/boss_sapphiron.cpp \
scripts/zone/naxxramas/boss_thaddius.cpp \
scripts/zone/naxxramas/instance_naxxramas.cpp \
scripts/zone/netherstorm/netherstorm.cpp \
scripts/zone/nexus/nexus/boss_anomalus.cpp \
scripts/zone/nexus/nexus/boss_keristrasza.cpp \
scripts/zone/nexus/nexus/boss_ormorok.cpp \
scripts/zone/nexus/nexus/boss_telestra.cpp \
scripts/zone/obsidian_sanctum/boss_sartharion.cpp \
scripts/zone/obsidian_sanctum/def_obsidian_sanctum.h \
scripts/zone/obsidian_sanctum/instance_obsidian_sanctum.cpp \
scripts/zone/onyxias_lair/boss_onyxia.cpp \
scripts/zone/orgrimmar/orgrimmar.cpp \
scripts/zone/razorfen_downs/boss_amnennar_the_coldbringer.cpp \
scripts/zone/razorfen_downs/razorfen_downs.cpp \
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
scripts/zone/scarlet_monastery/boss_houndmaster_loksey.cpp \
scripts/zone/scarlet_monastery/boss_interrogator_vishas.cpp \
scripts/zone/scarlet_monastery/boss_mograine_and_whitemane.cpp \
scripts/zone/scarlet_monastery/boss_scorn.cpp \
scripts/zone/scarlet_monastery/def_scarlet_monastery.h \
scripts/zone/scarlet_monastery/instance_scarlet_monastery.cpp \
scripts/zone/scholomance/boss_darkmaster_gandling.cpp \
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
scripts/zone/scholomance/def_scholomance.h \
scripts/zone/scholomance/instance_scholomance.cpp \
scripts/zone/searing_gorge/searing_gorge.cpp \
scripts/zone/shadowfang_keep/def_shadowfang_keep.h \
scripts/zone/shadowfang_keep/instance_shadowfang_keep.cpp \
scripts/zone/shadowfang_keep/shadowfang_keep.cpp \
scripts/zone/shadowmoon_valley/boss_doomwalker.cpp \
scripts/zone/shadowmoon_valley/shadowmoon_valley.cpp \
scripts/zone/shattrath/shattrath_city.cpp \
scripts/zone/silithus/silithus.cpp \
scripts/zone/silvermoon/silvermoon_city.cpp \
scripts/zone/silverpine_forest/silverpine_forest.cpp \
scripts/zone/stonetalon_mountains/stonetalon_mountains.cpp \
scripts/zone/stormwind/stormwind_city.cpp \
scripts/zone/stranglethorn_vale/stranglethorn_vale.cpp \
scripts/zone/stratholme/boss_baron_rivendare.cpp \
scripts/zone/stratholme/boss_baroness_anastari.cpp \
scripts/zone/stratholme/boss_cannon_master_willey.cpp \
scripts/zone/stratholme/boss_dathrohan_balnazzar.cpp \
scripts/zone/stratholme/boss_magistrate_barthilas.cpp \
scripts/zone/stratholme/boss_maleki_the_pallid.cpp \
scripts/zone/stratholme/boss_nerubenkan.cpp \
scripts/zone/stratholme/boss_order_of_silver_hand.cpp \
scripts/zone/stratholme/boss_postmaster_malown.cpp \
scripts/zone/stratholme/boss_ramstein_the_gorger.cpp \
scripts/zone/stratholme/boss_timmy_the_cruel.cpp \
scripts/zone/stratholme/def_stratholme.h \
scripts/zone/stratholme/instance_stratholme.cpp \
scripts/zone/stratholme/stratholme.cpp \
scripts/zone/sunwell_plateau/boss_brutallus.cpp \
scripts/zone/sunwell_plateau/boss_kalecgos.cpp \
scripts/zone/sunwell_plateau/def_sunwell_plateau.h \
scripts/zone/sunwell_plateau/instance_sunwell_plateau.cpp \
scripts/zone/tanaris/tanaris.cpp \
scripts/zone/teldrassil/teldrassil.cpp \
scripts/zone/tempest_keep/arcatraz/arcatraz.cpp \
scripts/zone/tempest_keep/arcatraz/boss_harbinger_skyriss.cpp \
scripts/zone/tempest_keep/arcatraz/def_arcatraz.h \
scripts/zone/tempest_keep/arcatraz/instance_arcatraz.cpp \
scripts/zone/tempest_keep/botanica/boss_high_botanist_freywinn.cpp \
scripts/zone/tempest_keep/botanica/boss_laj.cpp \
scripts/zone/tempest_keep/botanica/boss_warp_splinter.cpp \
scripts/zone/tempest_keep/the_eye/boss_astromancer.cpp \
scripts/zone/tempest_keep/the_eye/boss_kaelthas.cpp \
scripts/zone/tempest_keep/the_eye/boss_void_reaver.cpp \
scripts/zone/tempest_keep/the_eye/def_the_eye.h \
scripts/zone/tempest_keep/the_eye/instance_the_eye.cpp \
scripts/zone/tempest_keep/the_eye/the_eye.cpp \
scripts/zone/tempest_keep/the_mechanar/boss_gatewatcher_gyrokill.cpp \
scripts/zone/tempest_keep/the_mechanar/boss_gatewatcher_ironhand.cpp \
scripts/zone/tempest_keep/the_mechanar/boss_nethermancer_sepethrea.cpp \
scripts/zone/tempest_keep/the_mechanar/boss_pathaleon_the_calculator.cpp \
scripts/zone/tempest_keep/the_mechanar/def_mechanar.h \
scripts/zone/tempest_keep/the_mechanar/instance_mechanar.cpp \
scripts/zone/temple_of_ahnqiraj/boss_bug_trio.cpp \
scripts/zone/temple_of_ahnqiraj/boss_cthun.cpp \
scripts/zone/temple_of_ahnqiraj/boss_fankriss.cpp \
scripts/zone/temple_of_ahnqiraj/boss_huhuran.cpp \
scripts/zone/temple_of_ahnqiraj/boss_ouro.cpp \
scripts/zone/temple_of_ahnqiraj/boss_sartura.cpp \
scripts/zone/temple_of_ahnqiraj/boss_skeram.cpp \
scripts/zone/temple_of_ahnqiraj/boss_twinemperors.cpp \
scripts/zone/temple_of_ahnqiraj/boss_viscidus.cpp \
scripts/zone/temple_of_ahnqiraj/def_temple_of_ahnqiraj.h \
scripts/zone/temple_of_ahnqiraj/instance_temple_of_ahnqiraj.cpp \
scripts/zone/temple_of_ahnqiraj/mob_anubisath_sentinel.cpp \
scripts/zone/terokkar_forest/terokkar_forest.cpp \
scripts/zone/thousand_needles/thousand_needles.cpp \
scripts/zone/thunder_bluff/thunder_bluff.cpp \
scripts/zone/tirisfal_glades/tirisfal_glades.cpp \
scripts/zone/uldaman/boss_ironaya.cpp \
scripts/zone/uldaman/uldaman.cpp \
scripts/zone/undercity/undercity.cpp \
scripts/zone/utgarde_keep/utgarde_keep/boss_ingvar.cpp \
scripts/zone/utgarde_keep/utgarde_keep/boss_keleseth.cpp \
scripts/zone/utgarde_keep/utgarde_keep/boss_skarvald_and_dalronn.cpp \
scripts/zone/utgarde_keep/utgarde_keep/def_utgarde_keep.h \
scripts/zone/utgarde_keep/utgarde_keep/instance_utgarde_keep.cpp \
scripts/zone/utgarde_keep/utgarde_keep/utgarde_keep.cpp \
scripts/zone/wailing_caverns/instance_wailing_caverns.cpp \
scripts/zone/western_plaguelands/western_plaguelands.cpp \
scripts/zone/westfall/westfall.cpp \
scripts/zone/winterspring/winterspring.cpp \
scripts/zone/zangarmarsh/zangarmarsh.cpp \
scripts/zone/zulaman/boss_akilzon.cpp \
scripts/zone/zulaman/boss_halazzi.cpp \
scripts/zone/zulaman/boss_janalai.cpp \
scripts/zone/zulaman/boss_malacrass.cpp \
scripts/zone/zulaman/boss_nalorakk.cpp \
scripts/zone/zulaman/boss_zuljin.cpp \
scripts/zone/zulaman/def_zulaman.h \
scripts/zone/zulaman/instance_zulaman.cpp \
scripts/zone/zulaman/zulaman.cpp \
scripts/zone/zulfarrak/zulfarrak.cpp \
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
scripts/zone/zulgurub/def_zulgurub.h \
scripts/zone/zulgurub/instance_zulgurub.cpp \
revision.h \
system.cpp


## magic to include revision data in SD2 version string
revision.h: FORCE
	$(top_builddir)/src/tools/genrevision/genrevision $(srcdir)

FORCE:

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
libmangosscript_la_LIBFLAGS = -version-info $(LTMANGOS_CURRENT):$(LTMANGOS_REVISION):$(LTMANGOS_AGE)

## Additional files to include when running 'make dist'
# Scripts defaults.
EXTRA_DIST = \
	Scripts/sc_default.cpp \
	Scripts/sc_defines.cpp \
	Scripts/sc_defines.h \
	scriptdev2.conf.dist

## Additional files to install
sysconf_DATA = \
	scriptdev2.conf.dist

install-data-hook:
	@list='$(sysconf_DATA)'; for p in $$list; do \
      dest=`echo $$p | sed -e s/.dist//`; \
	  if test -f $(DESTDIR)$(sysconfdir)/$$dest; then \
        echo "$@ will not overwrite existing $(DESTDIR)$(sysconfdir)/$$dest"; \
      else \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$dest"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$dest; \
	  fi; \
	done

clean-local:
	rm -f $(sysconf_DATA)

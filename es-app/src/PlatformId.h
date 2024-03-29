#pragma once

#include <map>

namespace PlatformIds
{
  /*!
   * @brief Scrapable platforms
   * @note PLEASE, keep this list consistent and ordered!
   */
  enum class PlatformId : unsigned int
  {
    PLATFORM_UNKNOWN = 0,

    AMSTRAD_CPC,
    AMSTRAD_GX4000,
    APPLE_II,
    APPLE_IIGS,
    APPLE_MACOS,
    ARCADE,
    ATARI_8BITS,
    ATARI_2600,
    ATARI_5200,
    ATARI_7800,
    ATARI_JAGUAR,
    ATARI_JAGUAR_CD,
    ATARI_LYNX,
    ATARI_ST, // Atari ST/STE/Falcon
    BANDAI_WONDERSWAN,
    BANDAI_WONDERSWAN_COLOR,
    COLECOVISION,
    COMMODORE_64,
    COMMODORE_AMIGA,
    COMMODORE_CD32,
    COMMODORE_CDTV,
    FAIRCHILD_CHANNELF,
    GAMEENGINE_DAPHNE,
    GAMEENGINE_LUTRO,
    GAMEENGINE_OPENBOR,
    GAMEENGINE_SCUMMVM,
    GAMEENGINE_SOLARUS,
    GAMEENGINE_TIC80,
    GCE_VECTREX,
    IBM_PC,
    MAGNAVOX_ODYSSEY2,
    MATTEL_INTELLIVISION,
    MGT_SAMCOUPE,
    MICROSOFT_MSX,
    MICROSOFT_MSX1,
    MICROSOFT_MSX2,
    MICROSOFT_MSXTURBOR,
    MICROSOFT_XBOX,
    MICROSOFT_XBOX_360,
    NEC_PC88,
    NEC_PC98,
    NEC_PCENGINE,
    NEC_PCENGINE_CD,
    NEC_PCFX,
    NEC_SUPERGRAFX,
    NEOGEO,
    NEOGEO_CD,
    NEOGEO_POCKET,
    NEOGEO_POCKET_COLOR,
    NINTENDO_3DS,
    NINTENDO_64,
    NINTENDO_DS,
    NINTENDO_FDS,
    NINTENDO_GAME_AND_WATCH,
    NINTENDO_GAMEBOY,
    NINTENDO_GAMEBOY_ADVANCE,
    NINTENDO_GAMEBOY_COLOR,
    NINTENDO_GAMECUBE,
    NINTENDO_NES,
    NINTENDO_POKEMINI,
    NINTENDO_SATELLAVIEW,
    NINTENDO_SNES,
    NINTENDO_SUFAMITURBO,
    NINTENDO_VIRTUAL_BOY,
    NINTENDO_WII,
    NINTENDO_WII_U,
    OSH_UZEBOX,
    PALM_PDA,
    PANASONIC_3DO,
    PORT_PRBOOM,
    PORT_CAVE_STORY,
    SAMMY_ATOMISWAVE,
    SEGA_32X,
    SEGA_CD,
    SEGA_DREAMCAST,
    SEGA_GAME_GEAR,
    SEGA_GENESIS,
    SEGA_MASTER_SYSTEM,
    SEGA_MEGA_DRIVE,
    SEGA_NAOMI,
    SEGA_SATURN,
    SEGA_SG1000,
    SHARP_X1,
    SHARP_X68000,
    SINCLAIR_ZX_SPECTRUM,
    SINCLAIR_ZX_81,
    SONY_PLAYSTATION,
    SONY_PLAYSTATION_2,
    SONY_PLAYSTATION_3,
    SONY_PLAYSTATION_4,
    SONY_PLAYSTATION_VITA,
    SONY_PLAYSTATION_PORTABLE,
    SPECTRAVISION_SPECTRAVIDEO,
    STREAM_MOONLIGHT,
    TANDERINE_ORICATMOS,
    THOMSON_MOTO,

    PLATFORM_IGNORE, // do not allow scraping for this system
    PLATFORM_COUNT
  };
  /*
   * PLEASE, keep this list consistent and ordered!
   */

  PlatformId getPlatformId(const std::string& str);
  std::string getPlatformName(PlatformId id);

  const char* getCleanMameName(const char* from);
}

//
// Created by matthieu on 28/01/17.
//

#ifndef EMULATIONSTATION_ALL_MENUMESSAGES_H
#define EMULATIONSTATION_ALL_MENUMESSAGES_H


#include <string>

class MenuMessages {
public:
    static constexpr const char *SYSTEM_HELP_MSG = "Configure your recalbox language, select an external drive to store your games and configurations, check your current version and free space on drive";
    static constexpr const char *UPDATE_HELP_MSG = "Manage your recalbox updates. Select the update type. Activate update check.";

    static constexpr const char *UPDATE_TYPE_HELP_MSG = "Stable updates will check for updates on stable recalbox releases. Stable update are tested and approved by the recalbox team and testers.\nUnstable updates allow you to get the last recalbox features by checking our unstable repository. You can test and validate with us the very last version of recalbox.\nIf you choose unstable update, be kind to report issues on recalbox-os issue board (https://github.com/recalbox/recalbox-os/issues)";
    static constexpr const char *UPDATE_CHECK_HELP_MSG = "Automatically check if an update is avaialble. If so, notifies you with a message.";
    static constexpr const char *OVERCLOCK_HELP_MSG = "Overclock your board to increase performance.\nOverclock settings are tested and validated by the community. Keep in mind that overclocking your board can void the warranty.";
    static constexpr const char *BOOT_ON_SYSTEM_HELP_MSG = "Select the system to show when the recalbox frontend starts. The default value is 'favorites'.";
    static constexpr const char *BOOTGAMELIST_HELP_MSG = "On boot, recalbox will show the list of the games of the selected system rather than the system view.";
    static constexpr const char *GAMELISTONLY_HELP_MSG = "Only show games contained in the gamelist.xml file (located in your roms directories).\nThis option highly speeds up boot time, but new games are not detected.";
    static constexpr const char *HIDESYSTEMVIEW_HELP_MSG = "This option allows you to fix the selected system. The user cannot move to other systems.";
    static constexpr const char *EMULATOR_ADVANCED_HELP_MSG = "Override global options like emulator, core, ratio and more for each available system in your recalbox.";
    static constexpr const char *BOOT_HELP_MSG = "Configure boot options that makes your recalbox boot straight on a system or in kodi, lock user to a single system, or show the gamelist.";
    static constexpr const char *KODI_HELP_MSG = "Enable or disable Kodi, customize Kodi startup, allow X button to start Kodi";
    static constexpr const char *SECURITY_HELP_MSG = "Manage your recalbox security.";
};

#endif //EMULATIONSTATION_ALL_MENUMESSAGES_H

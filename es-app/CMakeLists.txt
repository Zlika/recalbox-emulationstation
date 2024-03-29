project("emulationstation")

set(ES_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MameNameMap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DemoMode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EmulationStation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ItemType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FolderData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EmptyData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RootFolderData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSorts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MetadataDescriptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MetadataFieldDescriptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformId.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ScraperCmdLine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SystemData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VolumeControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Gamelist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/NetworkThread.h

    # GuiComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/AsyncReqComponent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/RatingComponent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/ScraperSearchComponent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/TextListComponent.h

    # Guis
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiMetaDataEd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiGameScraper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiGamelistOptions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiMenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiSettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiScraperMulti.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiScraperStart.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiUpdate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiInfoPopup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/MenuMessages.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiNetPlay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiHashStart.h

    # Scrapers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/Scraper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/GamesDBJSONScraper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/GamesDBJSONScraperResources.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/MamedbScraper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/ScreenscraperScraper.h

    # Views
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/BasicGameListView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/DetailedGameListView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/IGameListView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/ISimpleGameListView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/GridGameListView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/SystemView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/ViewController.h

    # Animations
    ${CMAKE_CURRENT_SOURCE_DIR}/src/animations/LaunchAnimation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/animations/MoveCameraAnimation.h

    # Recalbox
    ${CMAKE_CURRENT_SOURCE_DIR}/src/recalbox/RecalboxSystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/recalbox/RecalboxUpgrade.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LibretroRatio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandThread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/NetPlayThread.h
)

set(ES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DemoMode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FolderData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RootFolderData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSorts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MameNameMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MetadataDescriptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MetadataFieldDescriptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformId.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ScraperCmdLine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SystemData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VolumeControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Gamelist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/NetworkThread.cpp


    # GuiComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/AsyncReqComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/RatingComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components/ScraperSearchComponent.cpp

    # Guis
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiMetaDataEd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiGameScraper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiGamelistOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiSettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiScraperMulti.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiScraperStart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiUpdate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiInfoPopup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/MenuMessages.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiNetPlay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/guis/GuiHashStart.cpp

    # Scrapers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/Scraper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/GamesDBJSONScraper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/GamesDBJSONScraperResources.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/MamedbScraper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scrapers/ScreenscraperScraper.cpp

    # Views
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/BasicGameListView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/DetailedGameListView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/IGameListView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/ISimpleGameListView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/gamelist/GridGameListView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/SystemView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/views/ViewController.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/recalbox/RecalboxSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/recalbox/RecalboxUpgrade.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LibretroRatio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/NetPlayThread.cpp

)

#-------------------------------------------------------------------------------
# define OS specific sources and headers
if(MSVC)
    LIST(APPEND ES_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/EmulationStation.rc
    )
endif()

#-------------------------------------------------------------------------------
# define target
include_directories(${COMMON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(emulationstation ${ES_SOURCES} ${ES_HEADERS} src/guis/GuiLoading.cpp src/guis/GuiLoading.h)
target_link_libraries(emulationstation ${COMMON_LIBRARIES} es-core)

# special properties for Windows builds
if(MSVC)
    # Always compile with the "WINDOWS" subsystem to avoid console window flashing at startup 
    # when --debug is not set (see es-core/src/main.cpp for explanation).
    # The console will still be shown if launched with --debug.
    # Note that up to CMake 2.8.10 this feature is broken: http://public.kitware.com/Bug/view.php?id=12566
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif()


#-------------------------------------------------------------------------------
# set up CPack install stuff so `make install` does something useful

install(TARGETS emulationstation
    RUNTIME
    DESTINATION bin)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A flexible graphical emulator front-end")
SET(CPACK_PACKAGE_DESCRIPTION "EmulationStation is a flexible, graphical front-end designed for keyboardless navigation of your multi-platform retro game collection.")

SET(CPACK_RESOURCE_FILE LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
SET(CPACK_RESOURCE_FILE README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alec Lofquist <allofquist@yahoo.com>")
SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libsdl2-2.0-0, libsdl2-mixer-2.0-0, libboost-system1.54.0,libboost-thread1.54.0, libboost-filesystem1.54.0, libfreeimage3, libfreetype6, libcurl3, libasound2")
SET(CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS "debhelper (>= 8.0.0), cmake, g++ (>= 4.8), libsdl2-dev, libsdl2-mixer-dev, libboost-system-dev, libboost-thread-dev , libboost-filesystem-dev, libboost-date-time-dev, libfreeimage-dev, libfreetype6-dev, libeigen3-dev, libcurl4-openssl-dev, libasound2-dev, libgl1-mesa-dev")

SET(CPACK_PACKAGE_VENDOR "emulationstation.org")
SET(CPACK_PACKAGE_VERSION "2.0.0~rc1")
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "emulationstation_${CMAKE_PACKAGE_VERSION}")
SET(CPACK_PACKAGE_EXECUTABLES "emulationstation" "emulationstation")

SET(CPACK_GENERATOR "TGZ;DEB")

INCLUDE(CPack)

cmake_minimum_required(VERSION 3.15.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

# --- Configuration -- #

set(vst3sdk_SOURCE_DIR "/Users/jonab/Source/Repos/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty.")
endif()

project(JaGain
    VERSION 1.0.0
    DESCRIPTION "JaGain VST3 Plug-in"
)

# --- Variables --- #

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(JaGain
    src/version.h
    src/cids.h
    src/processor.h
    src/processor.cpp
    src/controller.h
    src/controller.cpp
    src/entry.cpp
)

target_link_libraries(JaGain
    PRIVATE
    sdk
)

# --- VSTGUI --- #

if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(JaGain
        PRIVATE
        res/editor.uidesc
    )
    target_link_libraries(JaGain
        PRIVATE
        vstgui_support
    )
    smtg_target_add_plugin_resources(JaGain
        RESOURCES
        "res/editor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)

# --- Rules --- #

smtg_target_configure_version_file(JaGain)

if(SMTG_MAC)
    smtg_target_set_bundle(JaGain
        BUNDLE_IDENTIFIER sh.jta.gain
        COMPANY_NAME "jta.sh"
    )
    smtg_target_set_debug_executable(JaGain
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(JaGain
        PRIVATE
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT JaGain)
        smtg_target_set_debug_executable(JaGain
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)

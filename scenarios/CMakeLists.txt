add_subdirectory(artery)
add_subdirectory(gemv2)
add_subdirectory(highway-police)
if(WITH_STORYBOARD)
    add_subdirectory(storyboard)
endif()
if(WITH_TESTBED)
    add_subdirectory(testbed)
endif()
if(WITH_TRANSFUSION)
    add_subdirectory(transfusion)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/mt-its2017/omnetpp.ini)
    add_opp_run(mtits2017 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mt-its2017)
    add_opp_test(mtits2017 SUFFIX test SIMTIME_LIMIT 30s)
endif()

if(TARGET lte)
    add_library(artery-lte INTERFACE)
    target_link_libraries(artery-lte INTERFACE artery lte)
    add_opp_run(simulte
        DEPENDENCY artery-lte
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/simulte)
    add_opp_test(simulte SUFFIX cellular CONFIG VoIP-UpLink SIMTIME_LIMIT 10s)
    add_opp_test(simulte SUFFIX d2d CONFIG AlertMulticast SIMTIME_LIMIT 25s)
endif()

if(TARGET lte AND WITH_STORYBOARD)
    add_subdirectory(lte-blackice)
endif()

add_opp_run(car2car-grid WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/car2car-grid)
add_opp_test(car2car-grid SUFFIX cam_bsp CONFIG cam_bsp RUN 0 SIMTIME_LIMIT 30s)

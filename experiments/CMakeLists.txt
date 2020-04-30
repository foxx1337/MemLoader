add_subdirectory("ChronoUnits")
add_subdirectory("FunWithMoveSemantics")
add_subdirectory("SpeedChecks")
add_subdirectory("TimedExecutor")

if (BUILD_MEMLOADER_GUI)
    add_subdirectory("QtWithStdThread")
endif()

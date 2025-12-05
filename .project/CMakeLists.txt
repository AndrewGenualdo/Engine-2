set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
        GLOB_RECURSE PROJECT_INC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.h *.hpp
)

file(
        GLOB_RECURSE ASSIGNMENT1_SRC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.c *.cpp
)
#Copies this project's asset folder to bin when it is built
add_custom_target(copyAssetsProject ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${PROJECT_INC} DESTINATION include/project)
add_executable(project ${ASSIGNMENT1_SRC} ${PROJECT_INC}
        scenes/farminggame/farmingWorld.cpp
        scenes/farminggame/farmingWorld.h
        scenes/farminggame/misc/littleGuy.cpp
        scenes/farminggame/misc/littleGuy.h
        scenes/farminggame/tasks/Task.cpp
        scenes/farminggame/tasks/Task.h
        scenes/farminggame/tasks/TaskTravel.cpp
        scenes/farminggame/tasks/TaskTravel.h
        scenes/farminggame/items/Item.cpp
        scenes/farminggame/items/Item.h
        scenes/farminggame/tiles/Tile.cpp
        scenes/farminggame/tiles/Tile.h
        scenes/farminggame/items/seeds/ItemSeedTomato.cpp
        scenes/farminggame/items/seeds/ItemSeedTomato.h
        scenes/farminggame/items/produce/ItemProduceTomato.cpp
        scenes/farminggame/items/produce/ItemProduceTomato.h
        scenes/farminggame/tiles/plants/TilePlant.cpp
        scenes/farminggame/tiles/plants/TilePlant.h
        scenes/farminggame/FarmingObject.cpp
        scenes/farminggame/FarmingObject.h
        scenes/farminggame/tiles/plants/TilePlantTomato.cpp
        scenes/farminggame/tiles/plants/TilePlantTomato.h
        scenes/farminggame/items/produce/ItemProduceCarrot.cpp
        scenes/farminggame/items/produce/ItemProduceCarrot.h
        scenes/farminggame/items/seeds/ItemSeedCarrot.cpp
        scenes/farminggame/items/seeds/ItemSeedCarrot.h
        scenes/farminggame/tasks/TaskPickupItem.cpp
        scenes/farminggame/tasks/TaskPickupItem.h
        scenes/farminggame/items/seeds/ItemSeed.cpp
        scenes/farminggame/items/seeds/ItemSeed.h
        scenes/farminggame/items/produce/ItemProduce.cpp
        scenes/farminggame/items/produce/ItemProduce.h
        scenes/farminggame/tasks/TaskHarvestPlant.cpp
        scenes/farminggame/tasks/TaskHarvestPlant.h
        scenes/farminggame/tiles/plants/TilePlantCarrot.cpp
        scenes/farminggame/tiles/plants/TilePlantCarrot.h
        scenes/farminggame/tasks/TaskDepositItem.cpp
        scenes/farminggame/tasks/TaskDepositItem.h
        scenes/farminggame/tasks/TaskWithdrawItem.cpp
        scenes/farminggame/tasks/TaskWithdrawItem.h
        scenes/farminggame/tasks/TaskWait.cpp
        scenes/farminggame/tasks/TaskWait.h
        scenes/farminggame/tasks/TaskPlantSeed.cpp
        scenes/farminggame/tasks/TaskPlantSeed.h
        scenes/farminggame/misc/LittleGuyManager.cpp
        scenes/farminggame/misc/LittleGuyManager.h
        scenes/farminggame/items/produce/ItemProduceBlueberry.cpp
        scenes/farminggame/items/produce/ItemProduceBlueberry.h
        scenes/farminggame/items/seeds/ItemSeedBlueberry.cpp
        scenes/farminggame/items/seeds/ItemSeedBlueberry.h
        scenes/farminggame/tiles/plants/TilePlantBlueberry.cpp
        scenes/farminggame/tiles/plants/TilePlantBlueberry.h
        scenes/farminggame/misc/Truck.cpp
        scenes/farminggame/misc/Truck.h
        scenes/farminggame/items/misc/ItemGold.cpp
        scenes/farminggame/items/misc/ItemGold.h
        scenes/farminggame/tasks/TaskSell.cpp
        scenes/farminggame/tasks/TaskSell.h
)

if (EMSCRIPTEN)
    set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/.project/assets")
    target_link_options(project PRIVATE
            -sUSE_GLFW=3
            -sWASM=1
            -sFULL_ES3=1
            -sALLOW_MEMORY_GROWTH=1
            -sDISABLE_EXCEPTION_CATCHING=0
            --preload-file ${ASSETS_DIR}@/assets
    )
endif()

target_link_libraries(project PUBLIC core IMGUI glm)
target_include_directories(project PUBLIC ${CORE_INC_DIR} ${CMAKE_SOURCE_DIR}/core ${stb_INCLUDE_DIR})

#Trigger asset copy when project is built
add_dependencies(project copyAssetsProject)
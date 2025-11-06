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
        scenes/farminggame/littleGuy.cpp
        scenes/farminggame/littleGuy.h
        scenes/farminggame/tasks/Task.cpp
        scenes/farminggame/tasks/Task.h
        scenes/farminggame/tasks/TaskTravel.cpp
        scenes/farminggame/tasks/TaskTravel.h
        scenes/farminggame/tasks/TaskRetrieveItem.cpp
        scenes/farminggame/tasks/TaskRetrieveItem.h
        scenes/farminggame/items/Item.cpp
        scenes/farminggame/items/Item.h
        scenes/farminggame/tiles/Tile.cpp
        scenes/farminggame/tiles/Tile.h
        scenes/farminggame/items/seeds/ItemTomatoSeed.cpp
        scenes/farminggame/items/seeds/ItemTomatoSeed.h
        scenes/farminggame/items/produce/ItemTomato.cpp
        scenes/farminggame/items/produce/ItemTomato.h
        scenes/farminggame/tiles/plants/TilePlant.cpp
        scenes/farminggame/tiles/plants/TilePlant.h
        scenes/farminggame/FarmingObject.cpp
        scenes/farminggame/FarmingObject.h
        scenes/farminggame/tiles/plants/TilePlantTomato.cpp
        scenes/farminggame/tiles/plants/TilePlantTomato.h
        scenes/farminggame/items/produce/ItemCarrot.cpp
        scenes/farminggame/items/produce/ItemCarrot.h
        scenes/farminggame/items/seeds/ItemCarrotSeed.cpp
        scenes/farminggame/items/seeds/ItemCarrotSeed.h
        scenes/farminggame/tasks/TaskPickupItem.cpp
        scenes/farminggame/tasks/TaskPickupItem.h
        scenes/farminggame/items/seeds/ItemSeed.cpp
        scenes/farminggame/items/seeds/ItemSeed.h
        scenes/farminggame/items/produce/ItemProduce.cpp
        scenes/farminggame/items/produce/ItemProduce.h
)

target_link_libraries(project PUBLIC core IMGUI glm assimp enet)
target_include_directories(project PUBLIC ${CORE_INC_DIR} ${CMAKE_SOURCE_DIR}/core ${stb_INCLUDE_DIR})

#Trigger asset copy when project is built
add_dependencies(project copyAssetsProject)
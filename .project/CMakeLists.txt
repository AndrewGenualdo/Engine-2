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
        scenes/game/player.h
        scenes/game/level.h
        scenes/game/platform.h
        scenes/game/player.cpp
        scenes/game/client.h
        scenes/game/server.h
        scenes/game/client.cpp
        scenes/game/server.cpp
        scenes/game/game.h
        scenes/game/hitbox.h
        scenes/game/hitbox.cpp
        scenes/game/chatBox.h
        scenes/comic/comic.cpp
        scenes/comic/comic.h
        scenes/comic/comic.h
        scenes/factory/factory.cpp
        scenes/factory/factory.h
        scenes/factory/items/item.cpp
        scenes/factory/items/item.h
        scenes/factory/structures/structure.cpp
        scenes/factory/structures/structure.h
        scenes/factory/structures/miner.cpp
        scenes/factory/structures/miner.h
        scenes/factory/structures/collector.cpp
        scenes/factory/structures/collector.h
        scenes/factory/items/rock.cpp
        scenes/factory/items/rock.h
        scenes/factory/structures/bouncer.cpp
        scenes/factory/structures/bouncer.h
        scenes/factory/structures/wall.cpp
        scenes/factory/structures/wall.h
        scenes/factory/structures/track.cpp
        scenes/factory/structures/track.h
        scenes/gravity/gravity.cpp
        scenes/gravity/gravity.h
        scenes/world/world.cpp
        scenes/world/world.h)
target_link_libraries(project PUBLIC core IMGUI glm assimp enet)
target_include_directories(project PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when project is built
add_dependencies(project copyAssetsProject)
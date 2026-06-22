#include "Engine.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#define INCLUDE_CORE_COMPONENTS
#define INCLUDE_SPRITE
#define INCLUDE_ANIMATION

#include "presets.hpp"

#include "comp/ui/CursorFollower.hpp"
#include "sys/transform/CursorFollowersSystem.hpp"

#define BLOCK_SIZE 32
#define SCALE 3
#define GAME_WIDTH 8 * BLOCK_SIZE
#define GAME_HEIGTH 6 * BLOCK_SIZE
    
int main(int argc, char *argv[])
{
    mtrs::engine::Config config;
    config.size_in_pixels = glm::vec2(GAME_WIDTH, GAME_HEIGTH);
    config.name_window = "Engine";
    config.start_pixel_scale = SCALE;
    config.executable_path = *argv;
    config.display_cursor = false;
    config.scenes_path = "res/";
    config.resurce_path = "res/";
    
    mtrs::engine::Core engine(config);

    REGISTER_CORE_COMPONENTS(engine);
    REGISTER_SPRITE(engine);
    REGISTER_ANIMATION(engine);
    
    engine.systems.register_update<mtrs::sys::CursorFollowersSystem>();
    engine.systems.register_always_update<mtrs::sys::CursorFollowersSystem>();

    engine.world.load_scene("scene", engine.resources);

    engine.pre_update();
    double lastTime = glfwGetTime(), currentTime, delta;
    while (!engine.is_close_window())
    {
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;

        engine.update(delta);

        lastTime = currentTime;
    }

    engine.shutdown();

    return 0;
}
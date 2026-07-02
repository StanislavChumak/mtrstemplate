#include "Engine.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#define INCLUDE_CORE_COMPONENTS
#define INCLUDE_SPRITE
#define INCLUDE_ANIMATION
#define INCLUDE_SOUNDS

#include "presets.hpp"

#include "comp/ui/CursorFollower.hpp"
#include "sys/transform/CursorFollowersSystem.hpp"

#include "comp/rendering/SpriteMap.hpp"
#include "sys/rendering/SpriteMapRenderSystem.hpp"
#include "sys/rendering/MapAnimatorSystem.hpp"

#define BLOCK_SIZE 32
#define SCALE 3
#define GAME_WIDTH 8 * BLOCK_SIZE
#define GAME_HEIGTH 6 * BLOCK_SIZE

#include "sys/System.hpp"
namespace mtrs::sys
{
class TestSoundSystem : public System<TestSoundSystem>
{
public:
    inline static double _delta = 0.f;
    static void update_imp(comp::ECSWorld &world, const double &delta)
    {
        for(auto [entity, folower, player] : world.view<comp::CursorFollower, comp::SoundPlayer>())
        {
            _delta += delta;
            if(_delta > 1.f)
            {
                _delta -= 1.f;
                player->sound->start();
            }
        }
    }
    static constexpr int get_prioritet_imp() { return SystemPriority::AUDIO; };
};
}
    
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

    engine.systems.register_update<mtrs::sys::SpriteMapRenderSystem>();
    engine.systems.register_always_update<mtrs::sys::SpriteMapRenderSystem>();

    engine.systems.register_update<mtrs::sys::MapAnimatorSystem>();
    engine.systems.register_always_update<mtrs::sys::MapAnimatorSystem>();

    engine.systems.register_update<mtrs::sys::TestSoundSystem>();
    engine.systems.register_always_update<mtrs::sys::TestSoundSystem>();

    engine.world.load_scene("scene", engine.resources);

    double time = 0.f;

    engine.pre_update();
    double last_time = glfwGetTime(), current_time, delta;
    while (!engine.is_close_window())
    {
        current_time = glfwGetTime();

        engine.update(current_time - last_time);

        last_time = current_time;
    }

    engine.shutdown();

    return 0;
}
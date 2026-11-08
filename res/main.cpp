#define SCRIPT
#include "script/engine_api.hpp"

extern "C"
{

EXPORT void on_load(mtrs::comp::EntityID entity, mtrs::EngineAPI *engine_api)
{
    api = engine_api;
    mtrs::load_scene(mtrs::math::hash64("grassland/grassland"), mtrs::math::hash64("grassland"));
    mtrs::load_scene(mtrs::math::hash64("cursor/cursor"), mtrs::math::hash64("cursor"));
}

EXPORT void on_unload()
{
}

}
#define SCRIPT
#include "script/engine_api.hpp"

#include "comp/single/Cursor.hpp"
#include "comp/core/Transform.hpp"

static mtrs::comp::Transform *transform = nullptr;
static mtrs::comp::Cursor *cursor = nullptr;

void set_position()
{
    transform->position.set(cursor->position.get());
}

extern "C"
{

EXPORT void on_load(mtrs::comp::EntityID entity, mtrs::EngineAPI *engine_api)
{
    api = engine_api;

    transform = mtrs::get_comp<mtrs::comp::Transform>(entity);
    cursor = mtrs::get_single_comp<mtrs::comp::Cursor>();

    cursor->subscribe(set_position);
}

EXPORT void on_unload()
{
}

}
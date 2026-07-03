#include "res/asset/ScriptFile.hpp"

#include "comp/single/KeyButtons.hpp"
#include "comp/core/Transform.hpp"

#include "mtrsstruct/util/hash.hpp"

static mtrs::EngineAPI *api = nullptr;

#define get_comp(entity, component) reinterpret_cast<mtrs::comp::component*>(\
    api->get_component(api->world, mtrs::util::hash_c_string<uint64_t>(#component), entity))
#define get_singl_comp(component) reinterpret_cast<mtrs::comp::component*>(\
    api->get_single_component(api->world, mtrs::util::hash_c_string<uint64_t>(#component)))


static mtrs::comp::Transform *t = nullptr;
static mtrs::comp::KeyButtons *k = nullptr;

extern "C"
{

EXPORT void on_load(mtrs::comp::EntityID entity, mtrs::EngineAPI *engine_api)
{
    api = engine_api;
    t = get_comp(entity, Transform);
    k = get_singl_comp(KeyButtons);
}

EXPORT void on_unload()
{
}

const int spead = 7;

EXPORT void update(mtrs::comp::EntityID entity, const double &delta)
{
    if(k->keys[87])
    {
        t->position += glm::vec2(0.f, -spead * delta * 10);
    }
    if(k->keys[83])
    {
        t->position += glm::vec2(0.f, spead * delta * 10);
    }

    if(k->keys[68])
    {
        t->position += glm::vec2(spead * delta * 10, 0.f);
    }
    if(k->keys[65])
    {
        t->position += glm::vec2(-spead * delta * 10, 0.f);
    }
}

}


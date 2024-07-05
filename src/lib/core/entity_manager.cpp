// header
#include "entity_manager.hpp"

// local
#include "../utils/assert.hpp"
#include "../utils/log.hpp"


/* -------------------------------------------------------------------------- */
/*                                   Entity                                   */
/* -------------------------------------------------------------------------- */

void Entity::give_child_guard(EntityChildGuard guard)
{
    ib_debug_assert(this->has_child_guard() == false);
    this->child_guard = guard;
}

bool Entity::has_child_guard() const
{
    return this->child_guard.has_value();
}

std::optional<EntityChildGuard> Entity::get_child_guard() const
{
    if (this->has_child_guard() == false)
        return std::nullopt;

    if (this->is_child_guard_public() == false)
        return std::nullopt;

    ib_debug_assert(this->child_guard.has_value());
    return this->child_guard;
}

EntityID Entity::add_child(EntityManager& manager, std::unique_ptr<Entity> child_entity,
                           std::optional<std::string> name)
{
    ib_debug_assert(this->has_child_guard());
    auto child_id = manager.add_child(this->child_guard.value(), std::move(child_entity));

    if (name.has_value())
    {
        auto child_index = manager.get_child_count(this->child_guard.value().get_parent_id());
        manager.set_child_name(this->child_guard.value(), child_index, name.value());
    }

    return child_id;
}

void Entity::remove_child_by_index(EntityManager& manager, size_t index)
{
    ib_debug_assert(this->has_child_guard());
    manager.remove_child_by_index(this->child_guard.value(), index);
}

void Entity::remove_child_by_name(EntityManager& manager, std::string_view name)
{
    ib_debug_assert(this->has_child_guard());
    manager.remove_child_by_name(this->child_guard.value(), name);
}

void Entity::set_child_name(EntityManager& manager, size_t index, std::string const& name)
{
    ib_debug_assert(this->has_child_guard());
    manager.set_child_name(this->child_guard.value(), index, name);
}

std::optional<std::string> Entity::get_child_name_by_index(EntityManager const& manager,
                                                           size_t index) const
{
    ib_debug_assert(this->has_child_guard());
    return manager.get_child_name_by_index(this->child_guard.value().get_parent_id(), index)
        .value();
}

void Entity::remove_child_name(EntityManager& manager, size_t index)
{
    ib_debug_assert(this->has_child_guard());
    manager.remove_child_name(this->child_guard.value(), index);
}

void Entity::set_child_index(EntityManager& manager, size_t index, size_t new_index)
{
    ib_debug_assert(this->has_child_guard());
    manager.set_child_index(this->child_guard.value(), index, new_index);
}

EntityID Entity::get_child_id_by_index(EntityManager const& manager, size_t index) const
{
    ib_debug_assert(this->has_child_guard());
    return manager.get_child_id_by_index(this->child_guard.value().get_parent_id(), index);
}

EntityID Entity::get_child_id_by_name(EntityManager const& manager, std::string_view name) const
{
    ib_debug_assert(this->has_child_guard());
    return manager.get_child_id_by_name(this->child_guard.value().get_parent_id(), name);
}

size_t Entity::get_child_count(EntityManager const& manager) const
{
    ib_debug_assert(this->has_child_guard());
    return manager.get_child_count(this->child_guard.value().get_parent_id());
}

Entity& Entity::get_child_by_index(EntityManager& manager, size_t index) const
{
    auto id = this->get_child_id_by_index(manager, index);
    return manager.get_entity(id);
}

Entity const& Entity::get_child_by_index(EntityManager const& manager, size_t index) const
{
    auto id = this->get_child_id_by_index(manager, index);
    return manager.get_entity(id);
}

Entity& Entity::get_child_by_name(EntityManager& manager, std::string_view name) const
{
    auto id = this->get_child_id_by_name(manager, name);
    return manager.get_entity(id);
}

Entity const& Entity::get_child_by_name(EntityManager const& manager, std::string_view name) const
{
    auto id = this->get_child_id_by_name(manager, name);
    return manager.get_entity(id);
}

Entity& Entity::get_child_by_id(EntityManager& manager, EntityID child_id) const // NOLINT(*-convert-member-functions-to-static)
{

    return manager.get_entity(child_id);
}

Entity const& Entity::get_child_by_id(EntityManager const& manager, EntityID child_id) const // NOLINT(*-convert-member-functions-to-static)
{

    return manager.get_entity(child_id);
}



/* -------------------------------------------------------------------------- */
/*                               Entity Manager                               */
/* -------------------------------------------------------------------------- */

EntityManager::EntityManager(std::unique_ptr<Entity> root_entity): root_id(entity_counter++)
{
    ib_debug_assert(root_entity != nullptr);
    this->entities.emplace(std::pair<size_t, EntityData>(
        this->root_id.id, EntityData{std::move(root_entity), std::nullopt}));
    this->entities.at(this->root_id.id).entity->give_child_guard(EntityChildGuard{this->root_id});
}

EntityID EntityManager::add_child(EntityChildGuard guard, std::unique_ptr<Entity> child_entity)
{
    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    ib_debug_assert(child_entity != nullptr);

    // create the new entity for the child
    auto child_id = EntityID(entity_counter++);
    this->entities.emplace(std::pair<size_t, EntityData>(
        child_id.id, EntityData{std::move(child_entity), guard.get_parent_id()}));
    this->entities.at(child_id.id).entity->give_child_guard(EntityChildGuard{child_id});

    // give the child guard to the parent
    this->entities.at(guard.get_parent_id().id).children.push_back(child_id);

    return child_id;
}

void EntityManager::set_child_name(EntityChildGuard guard, size_t child_index,
                                   std::string const& name)
{
    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);

    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());

    // check if the name is already taken
    ib_debug_assert(parent.child_name_to_index.contains(name) == false);

    // set the name and return true to indicate success
    this->entities.at(guard.get_parent_id().id).child_name_to_index[name] = child_index;
}

void EntityManager::remove_child_name(EntityChildGuard guard, size_t child_index)
{
    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);

    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());

    // remove the name
    for (auto& [name, index]: parent.child_name_to_index)
    {
        if (index == child_index)
        {
            parent.child_name_to_index.erase(name);
            return;
        }
    }

    panic("The child index does not have a name");
}

void EntityManager::set_child_index(EntityChildGuard guard, size_t child_index, size_t new_index)
{
    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);

    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());
    ib_debug_assert(new_index < parent.children.size());

    // pop the child entity from its position
    EntityID child_id = parent.children[child_index];
    parent.children.erase(parent.children.begin() + child_index); // NOLINT(*-narrowing-conversions)

    // update the index for all names pointing to after the old child position
    for (auto& [name, index]: parent.child_name_to_index)
    {
        if (index > child_index)
        {
            index--;
        }
    }

    // insert the child entity at the new position
    parent.children.insert(parent.children.begin() + new_index, // NOLINT(*-narrowing-conversions)
                           child_id);

    // update the index for all names pointing to after the new child position
    for (auto& [name, index]: parent.child_name_to_index)
    {
        if (index >= new_index)
        {
            index++;
        }
    }
}

void EntityManager::remove_child_by_index(EntityChildGuard guard, size_t child_index)
{
    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);

    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());

    // remove the name
    for (auto& [name, index]: parent.child_name_to_index)
    {
        if (index == child_index)
        {
            parent.child_name_to_index.erase(name);
            break;
        }
    }

    // remove the child entity
    parent.children.erase(parent.children.begin() + child_index); // NOLINT(*-narrowing-conversions)
}

void EntityManager::remove_child_by_name(EntityChildGuard guard, std::string_view _name)
{
    // turn the string_view into an owned string, since the unordered_map do not support string_view
    std::string const name = std::string(_name);

    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);

    // check if the name exists
    ib_debug_assert(parent.child_name_to_index.contains(name));

    // remove the child entity
    size_t child_index = parent.child_name_to_index.at(name);
    ib_debug_assert(child_index < parent.children.size());
    parent.children.erase(parent.children.begin() + child_index); // NOLINT(*-narrowing-conversions)

    // remove the name
    parent.child_name_to_index.erase(name);
}

std::optional<EntityID> EntityManager::get_parent_id(EntityID entity_id) const
{
    ib_debug_assert(this->has_entity(entity_id));
    return this->entities.at(entity_id.id).parent;
}

size_t EntityManager::get_child_count(EntityID parent_id) const
{
    ib_debug_assert(this->has_entity(parent_id));
    return this->entities.at(parent_id.id).children.size();
}

EntityID EntityManager::get_child_id_by_index(EntityID parent_id, size_t index) const
{
    ib_debug_assert(this->has_entity(parent_id));
    ib_debug_assert(index < this->entities.at(parent_id.id).children.size());
    return this->entities.at(parent_id.id).children[index];
}

std::optional<std::string> EntityManager::get_child_name_by_index(EntityID parent_id,
                                                                  size_t child_index) const
{
    ib_debug_assert(this->has_entity(parent_id));
    auto& parent = this->entities.at(parent_id.id);

    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());

    // find the name
    for (auto& [name, index]: parent.child_name_to_index)
    {
        if (index == child_index)
        {
            return name;
        }
    }

    return std::nullopt;
}

size_t EntityManager::get_child_index_by_name(EntityID parent_id, std::string_view name) const
{
    // turn the string_view into an owned string, since the unordered_map do not support string_view
    std::string const _name = std::string(name);

    ib_debug_assert(this->has_entity(parent_id));
    auto& parent = this->entities.at(parent_id.id);

    // check if the name exists
    ib_debug_assert(parent.child_name_to_index.contains(_name));

    return parent.child_name_to_index.at(_name);
}

EntityID EntityManager::get_child_id_by_name(EntityID parent_id, std::string_view name) const
{
    // turn the string_view into an owned string, since the unordered_map do not support string_view
    std::string const _name = std::string(name);

    ib_debug_assert(this->has_entity(parent_id));
    auto& parent = this->entities.at(parent_id.id);

    // check if the name exists
    ib_debug_assert(parent.child_name_to_index.contains(_name));

    size_t child_index = parent.child_name_to_index.at(_name);
    ib_debug_assert(child_index < parent.children.size());
    return parent.children[child_index];
}

EntityID EntityManager::get_root_id() const
{
    return this->root_id;
}

Entity& EntityManager::get_entity(EntityID entity_id)
{
    ib_debug_assert(this->has_entity(entity_id));
    return *this->entities.at(entity_id.id).entity;
}

Entity const& EntityManager::get_entity(EntityID entity_id) const
{
    ib_debug_assert(this->has_entity(entity_id));
    return *this->entities.at(entity_id.id).entity;
}

bool EntityManager::has_entity(EntityID entity_id) const
{
    return this->entities.contains(entity_id.id);
}

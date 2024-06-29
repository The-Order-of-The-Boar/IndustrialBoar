// header
#include "entity_manager.hpp"

// local
#include "../utils/assert.hpp"


/* -------------------------------------------------------------------------- */
/*                                   Entity                                   */
/* -------------------------------------------------------------------------- */

void Entity::give_child_guard(EntityChildGuard guard) {

    ib_debug_assert(this->has_child_guard() == false);
    this->child_guard = guard;
}

bool Entity::has_child_guard() const {

    return this->child_guard.has_value();
}

std::optional<EntityChildGuard> Entity::get_child_guard() const {

    if (this->has_child_guard() == false)
        return std::nullopt;

    if (this->is_child_guard_public() == false)
        return std::nullopt;

    ib_debug_assert(this->child_guard.has_value());
    return this->child_guard;
}



/* -------------------------------------------------------------------------- */
/*                               Entity Manager                               */
/* -------------------------------------------------------------------------- */

EntityManager::EntityManager(std::unique_ptr<Entity> root_entity): root_id(entity_counter++) {

    ib_debug_assert(root_entity != nullptr);
    this->entities.emplace(std::pair<size_t, EntityData>(this->root_id.id, EntityData{std::move(root_entity)}));
}

EntityID EntityManager::add_child(EntityChildGuard guard, std::unique_ptr<Entity> child_entity) {

    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    ib_debug_assert(child_entity != nullptr);

    // create the new entity for the child
    EntityID child_id = EntityID(entity_counter++);
    this->entities.emplace(std::pair<size_t, EntityData>(child_id.id, EntityData{std::move(child_entity)}));

    // give the child guard to the parent
    this->entities.at(guard.get_parent_id().id).children.push_back(child_id);

    return child_id;
}

bool EntityManager::set_child_name(EntityChildGuard guard, size_t child_index, std::string const& name) {

    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);
    
    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());

    // check if the name is already taken
    if (parent.child_name_to_index.contains(name))
        return false;

    // set the name and return true to indicate success
    this->entities.at(guard.get_parent_id().id).child_name_to_index[name] = child_index;
    return true;
}

void EntityManager::remove_child_name(EntityChildGuard guard, size_t child_index) {

    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);
    
    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());

    // remove the name
    for (auto& [name, index] : parent.child_name_to_index) {
        if (index == child_index) {
            parent.child_name_to_index.erase(name);
            return;
        }
    }
}

void EntityManager::set_child_index(EntityChildGuard guard, size_t child_index, size_t new_index) {
    
    ib_debug_assert(this->has_entity(guard.get_parent_id()));
    auto& parent = this->entities.at(guard.get_parent_id().id);
    
    // check if the child index is valid
    ib_debug_assert(child_index < parent.children.size());
    ib_debug_assert(new_index < parent.children.size());

    // pop the child entity from its position
    EntityID child_id = parent.children[child_index];
    parent.children.erase(parent.children.begin() + child_index);

    // update the index for all names pointing to after the old child position
    for (auto& [name, index] : parent.child_name_to_index) {
        if (index > child_index) {
            index--;
        }
    }

    // insert the child entity at the new position
    parent.children.insert(parent.children.begin() + new_index, child_id);

    // update the index for all names pointing to after the new child position
    for (auto& [name, index] : parent.child_name_to_index) {
        if (index >= new_index) {
            index++;
        }
    }
}

// TODO: implement 'remove_child_by_index'
// TODO: implement 'remove_child_by_name'
// TODO: implement 'get_parent_id'
// TODO: implement 'get_child_count'
// TODO: implement 'get_child_id'
// TODO: implement 'get_child_name'
// TODO: implement 'get_child_index_by_name'
// TODO: implement 'get_root_id'
// TODO: implement 'get_entity'
// TODO: implement 'get_entity' const
// TODO: implement 'has_entity'

#pragma once

// builtin
#include <memory>
#include <cstddef>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

// TODO: write tests for the entity manager



class EntityManager;

class EntityID {
    friend class EntityManager;
    friend struct std::hash<EntityID>;
    size_t id;

    explicit EntityID(size_t _id): id(_id) {}
};

template <>
struct std::hash<EntityID> {
    std::size_t operator()(EntityID const& id) const {
        return std::hash<size_t>{}(id.id);
    }
};

class EntityChildGuard {
private:
    friend class EntityManager;
    EntityID parent_id;

public:
    [[nodiscard]] EntityID get_parent_id() const { return this->parent_id; }
    [[nodiscard]] explicit operator EntityID() { return this->parent_id; }
};

class Entity {
private:

    std::optional<EntityChildGuard> child_guard = std::nullopt;

public:  
    
    Entity() = default;
    virtual ~Entity() = default;

    // base functionality
    void give_child_guard(EntityChildGuard guard);
    [[nodiscard]] bool has_child_guard() const;
    [[nodiscard]] std::optional<EntityChildGuard> get_child_guard() const;

    // useful tooling
    EntityID add_child(EntityManager& manager, std::unique_ptr<Entity> child_entity, std::optional<std::string> name = std::nullopt);
    void remove_child_by_index(EntityManager& manager, size_t index);
    void remove_child_by_name(EntityManager& manager, std::string_view name);
    void set_child_name(EntityManager& manager, size_t index, std::string const& name);
    [[nodiscard]] std::optional<std::string> get_child_name_by_index(const EntityManager &manager, size_t index) const;
    void remove_child_name(EntityManager& manager, size_t index);
    void set_child_index(EntityManager& manager, size_t index, size_t new_index);
    [[nodiscard]] EntityID get_child_by_index(EntityManager const& manager, size_t index) const;
    [[nodiscard]] EntityID get_child_by_name(EntityManager const& manager, std::string_view name) const;

    // implementation specific
    [[nodiscard]] virtual bool is_child_guard_public() const = 0;
    virtual void update(double delta) = 0;
    virtual void render() const = 0;
};

class EntityManager {
private:

    struct EntityData {
        std::unique_ptr<Entity> entity;
        std::optional<EntityID> parent;
        std::vector<EntityID> children;
        std::unordered_map<std::string, size_t> child_name_to_index;

        explicit EntityData(std::unique_ptr<Entity> _entity, std::optional<EntityID> _parent)
        : entity(std::move(_entity)), parent(_parent) {}
    };

private:

    size_t entity_counter = 0;
    std::unordered_map<size_t, EntityData> entities;
    EntityID root_id;

public:

    explicit EntityManager(std::unique_ptr<Entity> root_entity);

    /* --------------------------------- create --------------------------------- */
    EntityID add_child(EntityChildGuard guard, std::unique_ptr<Entity> child_entity);

    /* --------------------------------- modify --------------------------------- */
    void set_child_name(EntityChildGuard guard, size_t child_index, std::string const& name);
    void remove_child_name(EntityChildGuard guard, size_t child_index);
    void set_child_index(EntityChildGuard guard, size_t child_index, size_t new_index);

    
    /* --------------------------------- remove --------------------------------- */
    void remove_child_by_index(EntityChildGuard guard, size_t child_index);
    void remove_child_by_name(EntityChildGuard guard, std::string_view name);

    /* --------------------------------- access --------------------------------- */
    std::optional<EntityID> get_parent_id(EntityID entity_id) const;
    size_t get_child_count(EntityID parent_id) const;
    EntityID get_child_id_by_index(EntityID parent_id, size_t index) const;
    EntityID get_child_id_by_name(EntityID parent_id, std::string_view name) const;
    std::optional<std::string> get_child_name_by_index(EntityID parent_id, size_t child_index) const;
    size_t get_child_index_by_name(EntityID parent_id, std::string_view name) const;
    
    /* ----------------------------------- get ---------------------------------- */
    EntityID get_root_id() const;
    Entity& get_entity(EntityID entity_id);
    Entity const& get_entity(EntityID entity_id) const;
    bool has_entity(EntityID entity_id) const;
};

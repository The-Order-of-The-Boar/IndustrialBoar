
// local
#include <core/entity_manager.hpp>
#include <utils/log.hpp>

// external
#include <catch2/catch_test_macros.hpp>



class TestEntity final : public Entity
{
public:

    uint32_t value;

public:

    explicit TestEntity(uint32_t _value): value(_value) {}

    [[nodiscard]] bool is_child_guard_public() const final
    {
        return true;
    }
};


TEST_CASE("Entity Manager")
{
    enable_panic_throw_mode();
    EntityManager manager{std::make_unique<TestEntity>(0)};

    SECTION("root value")
    {
        auto& root = dynamic_cast<TestEntity&>(manager.get_entity(manager.get_root_id()));
        REQUIRE(root.value == 0);
    }

    SECTION("add child")
    {
        auto& root    = dynamic_cast<TestEntity&>(manager.get_entity(manager.get_root_id()));
        auto child_id = root.add_child(manager, std::make_unique<TestEntity>(666));
        REQUIRE(root.get_child_count(manager) == 1);
        REQUIRE(dynamic_cast<TestEntity&>(manager.get_entity(child_id)).value == 666);

        SECTION("remove child")
        {
            root.remove_child_by_index(manager, root.get_child_count(manager) - 1);
            REQUIRE(root.get_child_count(manager) == 0);
        }
    }

    SECTION("add multiple children")
    {
        auto& root     = dynamic_cast<TestEntity&>(manager.get_entity(manager.get_root_id()));
        auto child1_id = root.add_child(manager, std::make_unique<TestEntity>(666));
        auto child2_id = root.add_child(manager, std::make_unique<TestEntity>(667));
        auto child3_id = root.add_child(manager, std::make_unique<TestEntity>(668));
        REQUIRE(root.get_child_count(manager) == 3);
        REQUIRE(dynamic_cast<TestEntity&>(manager.get_entity(child1_id)).value == 666);
        REQUIRE(dynamic_cast<TestEntity&>(manager.get_entity(child2_id)).value == 667);
        REQUIRE(dynamic_cast<TestEntity&>(manager.get_entity(child3_id)).value == 668);

        SECTION("remove multiple children")
        {
            root.remove_child_by_index(manager, 1);
            REQUIRE(root.get_child_count(manager) == 2);
            REQUIRE(dynamic_cast<TestEntity&>(root.get_child_by_id(manager, child1_id)).value ==
                    666);
            REQUIRE(dynamic_cast<TestEntity&>(root.get_child_by_index(manager, 0)).value == 666);
            REQUIRE(dynamic_cast<TestEntity&>(root.get_child_by_id(manager, child3_id)).value ==
                    668);
            REQUIRE(dynamic_cast<TestEntity&>(root.get_child_by_index(manager, 1)).value == 668);

            root.remove_child_by_index(manager, 1);
            REQUIRE(root.get_child_count(manager) == 1);
            REQUIRE(dynamic_cast<TestEntity&>(root.get_child_by_id(manager, child1_id)).value ==
                    666);
            REQUIRE(dynamic_cast<TestEntity&>(root.get_child_by_index(manager, 0)).value == 666);

            root.remove_child_by_index(manager, 0);
            REQUIRE(root.get_child_count(manager) == 0);
        }
    }
}
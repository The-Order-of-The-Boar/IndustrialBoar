#include "construction_manager.hpp"

#include "../../utils/assert.hpp"

ConstructionManager::ConstructionManager(World& world): world{world} {}

void ConstructionManager::set_selected_building(BuildingType const building_type)
{
    this->selected_building.release();
    switch (building_type)
    {
        case BuildingType::BELT:
            this->selected_building = std::make_unique<Belt>(0, glm::u64vec2{0, 0});
            break;
        case BuildingType::MINE:
            this->selected_building = std::make_unique<Mine>(
                0, glm::u64vec2{0, 0}, Building::Rotation::UP, Resource::ResourceType::COOPER);
            break;
        case BuildingType::NO_BUILDING:
            this->selected_building = nullptr;
            break;
        default:
            ib_debug_assert(false, "Unimplemented build selection of " + building_type);
    }
}

void ConstructionManager::update(double delta, FrameInput const& frame_input)
{
    (void)delta;

    if (this->selected_building == nullptr)
        return;

    for (auto const& key: frame_input.key_inputs)
    {
        if (key.state == InputEventState::PRESSED)
        {
            if (key.type == InputEventType::ROTATE_LEFT)
                this->selected_building->rotate_left();
            else if (key.type == InputEventType::ROTATE_RIGHT)
                this->selected_building->rotate_right();
        }
    }
    if (this->selected_building != nullptr)
    {
        auto const build_index               = frame_input.mouse_input.mouse_index;
        this->selected_building->world_index = build_index;
        bool const can_be_build_at =
            this->selected_building->can_be_build_at(this->world.get_tile(build_index));

        std::cout << "CAN BE BUILT: " << can_be_build_at << std::endl;
        if (can_be_build_at)
        {
            this->selected_building->modulate = Constants::WHITE;
            if (frame_input.mouse_input.button == MouseInput::MouseButton::LEFT)
            {
                switch (this->selected_building->building_type)
                {
                    case BuildingType::BELT:
                        this->world.add_building<Belt>((Belt*)this->selected_building.get());
                        break;
                    case BuildingType::MINE:
                    {
                        auto resource = this->world.get_tile(build_index).ground;
                        ((Mine*)this->selected_building.get())->mine_resource.type =
                            ((Resource::ResourceType)(resource - 4));
                        this->world.add_building<Mine>((Mine*)this->selected_building.get());
                        break;
                    }
                    case BuildingType::NO_BUILDING:
                        ib_debug_assert(false, "tried to build NO_BUILDING");
                        break;
                    default:
                        ib_debug_assert(false, "Unimplemented building of " +
                                                   this->selected_building->building_type);
                        break;
                }
            }
        }
        else
        {
            this->selected_building->modulate = CANNOT_BUILD_COLOR;
        }
    }
}
void ConstructionManager::render(ScreenRenderer& renderer) const
{
    if (this->selected_building != nullptr)
        this->selected_building->render(renderer);
}
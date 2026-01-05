#include "world_generator.hpp"

// builtin
#include <iostream>

// local
#include "../utils/time_utils.hpp"

static constexpr double WATER_THRESHOLD  = 0.2;
static constexpr double SAND_THRESHOLD   = 0.3;
static constexpr double GRASS_THRESHOLD  = 0.45;
static constexpr double STONE_THRESHOLD  = 1;
static constexpr double COOPER_THRESHOLD = 0.70;
static constexpr double IRON_THRESHOLD   = 0.82;

static constexpr double COOPER_THRESHOLD_SIZE = 0.03;
static constexpr double IRON_THRESHOLD_SIZE   = 0.02;

World WorldGenerator::generate()
{
    World world;

    TimeMeasurer world_gen_time{"tiles Generation Time", TimeMeasurer::TimeUnit::MILLISECOND};
    // uint32_t const seed = static_cast<uint32_t>(time(nullptr));
    // uint32_t const seed = static_cast<uint32_t>(time(nullptr));
    uint32_t const seed = 1766613113;
    std::cout << "Seed: " << seed << std::endl;
    PerlimNoise noiser{seed};

    for (size_t y = 0; y < Constants::WORLD_SIZE; ++y)
    {
        for (size_t x = 0; x < Constants::WORLD_SIZE; ++x)
        {
            double const noise = (noiser.get_fractal_brownian_motion_2d(glm::dvec2(x, y), 8));
            Tile::GroundType ground;
            if (noise < WATER_THRESHOLD)
                ground = Tile::GroundType::WATER;
            else if (noise < SAND_THRESHOLD)
                ground = Tile::GroundType::SAND;
            else if (noise < GRASS_THRESHOLD)
                ground = Tile::GroundType::GRASS;
            else
            {
                if (noise < COOPER_THRESHOLD && noise > (COOPER_THRESHOLD - COOPER_THRESHOLD_SIZE))
                    ground = Tile::GroundType::COOPER;
                else if (noise < IRON_THRESHOLD && noise > (IRON_THRESHOLD - IRON_THRESHOLD_SIZE))
                    ground = Tile::GroundType::IRON;
                else
                    ground = Tile::GroundType::STONE;
            }


            world.tiles[x][y].ground = ground;
        }
    }

    // world.mines.emplace_back(0, glm::u64vec2{2, 0}, Building::Rotation::DOWN,
    //                          Resource::ResourceType::COOPER);
    // world.tiles[2][0].building = world.mines[0].get_ref();

    // for (int64_t i = 0; i < 3; ++i)
    // {
    //     world.belts.emplace_back(world.belts.size(), glm::u64vec2{2, i + 1},
    //     Belt::Rotation::DOWN); world.tiles[2][i + 1].building = world.belts.at(world.belts.size()
    //     - 1).get_ref();
    // }

    // for (int64_t i = -2; i < 4; ++i)
    // {
    //     world.belts.emplace_back(world.belts.size(), glm::u64vec2{4 + i, 4},
    //     Belt::Rotation::RIGHT); world.tiles[4 + i][4].building =
    //     world.belts.at(world.belts.size() - 1).get_ref();
    // }
    // Belt* b     = &world.belts.at(5);
    // b->resource = Resource{b->world_index, Resource::ResourceType::COOPER};
    // for (int64_t i = 0; i < 4; ++i)
    // {
    //     world.belts.emplace_back(world.belts.size(), glm::u64vec2{4 + 4, 4 + i},
    //                              Belt::Rotation::DOWN);
    //     world.tiles[4 + 4][4 + i].building = world.belts.at(world.belts.size() - 1).get_ref();
    // }
    // b           = &world.belts.at(4);
    // b->resource = Resource{b->world_index, Resource::ResourceType::IRON};

    // for (int64_t i = 0; i < 4; ++i)
    // {
    //     world.belts.emplace_back(world.belts.size(), glm::u64vec2{4 + 4 - i, 4 + 4},
    //                              Belt::Rotation::LEFT);
    //     world.tiles[4 + 4 - i][4 + 4].building = world.belts.at(world.belts.size() -
    //     1).get_ref();
    // }
    // world_gen_time.print_time();

    return world;
}

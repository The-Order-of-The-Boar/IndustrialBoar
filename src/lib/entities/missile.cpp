#include "missile.hpp"

#include "../utils/math.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>


TextureIDHolder texture{"assets/textures/missile.png"};

void Missile::update([[maybe_unused]] double const delta)
{
    glm::vec2 const direction = this->target_position - this->position;
    double const target_angle = Math::get_vector_rotation_deg(direction);
    std::cout << "--\n";
    std::cout << "rotation angle: " << std::fmod(this->rotation + 180, 360) << std::endl;
    std::cout << "target angle  : " << std::fmod(target_angle + 180, 360) << std::endl;

    double a = target_angle + 180;
    double b = this->rotation + 180;
    double rotation_dif;
    if (a > b)
        rotation_dif = b - a;
    else
        rotation_dif = a - b;
    if (rotation_dif == 0)
        return;
    double const sign = rotation_dif / std::abs(rotation_dif);
    std::cout << "sign: " << sign << std::endl;
    double const rotation_delta = ROTATION_SPEED * sign;
    std::cout << "rotation_difg:  " << rotation_dif << std::endl;
    std::cout << "rotation_delta: " << rotation_delta << std::endl;
    double const step = std::min((rotation_delta * delta) / sign, rotation_dif / sign) / sign;
    std::cout << "step:           " << step << std::endl;
    this->rotation += step;
    if (rotation > 180)
    {
        rotation -= 360;
    }
    if (rotation < -180)
    {
        rotation += 360;
    }
    // this->rotation              = target_angle;



    // std::cout << this->position << std::endl;
}

void Missile::render(ScreenRenderer& renderer) const
{
    renderer.draw_texture(texture, {this->position.x, this->position.y}, std::nullopt, std::nullopt,
                          this->rotation);
}

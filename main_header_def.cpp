#include "main_header.hpp"

LevelGrid::LevelGrid(pair<int> level_dimensions, std::vector<std::vector<int>> level_walls) : level_dimensions_(level_dimensions), level_walls_(level_walls) {}

Entity::Entity(pair<int> pos) : pos_(pos) {}

LivingEntity::LivingEntity(pair<int> pos, int direction, int speed) : Entity(pos), direction_(direction), speed_(speed), walls_touched_(4, false) {}
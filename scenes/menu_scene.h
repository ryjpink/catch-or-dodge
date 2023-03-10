#pragma once

#include "scene.h"
#include "game.h"

#include <memory>

std::unique_ptr<Scene> CreateMenuScene(Game& game);
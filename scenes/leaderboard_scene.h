#pragma once

#include "leaderboard.h"
#include "scene.h"
#include "game.h"

#include <memory>

std::unique_ptr<Scene> CreateLeaderboardScene(Game& game, std::vector<PlayerScore> scores);
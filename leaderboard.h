#pragma once

#include <string>
#include <vector>

struct PlayerScore
{
    int rank;
    std::string playerName;
    int scoreInSeconds;
};

std::vector<PlayerScore> SubmitScore(std::string playerName, int scoreInSeconds);
std::vector<PlayerScore> GetHighscores();
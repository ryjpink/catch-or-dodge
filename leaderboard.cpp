#include "leaderboard.h"

#include <cpr/body.h>
#include <cpr/cpr.h>
#include <cpr/cprtypes.h>
#include <cpr/util.h>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string baseUrl = "http://localhost:5000";

std::vector<PlayerScore> ParseHighscores(const std::string &data)
{
    std::vector<PlayerScore> highscores;
    json records = json::parse(data);
    for (auto &record : records)
    {
        int rank = record["rank"];
        std::string name = record["name"];
        int score = record["score"];
        highscores.push_back(PlayerScore{rank, name, score});
    }
    return highscores;
}

std::vector<PlayerScore> SubmitScore(std::string playerName, int scoreInSeconds)
{
    json body = {{"score", scoreInSeconds}};
    fmt::print("{}", body.dump());
    cpr::Response response = cpr::Put(cpr::Url{fmt::format("{}/records/{}", baseUrl, cpr::util::urlEncode(playerName))},
                                      cpr::Header{{"content-type", "application/json"}}, cpr::Body{body.dump()});
    if (response.status_code != 200)
    {
        return {PlayerScore{0, fmt::format("Error: Response status code {}", response.status_code), 0}};
    }
    return ParseHighscores(response.text);
}

std::vector<PlayerScore> GetHighscores()
{
    cpr::Response response = cpr::Get(cpr::Url{fmt::format("{}/records", baseUrl)});
    if (response.status_code != 200)
    {
        return {PlayerScore{0, fmt::format("Error: Response status code", response.status_code), 0}};
    }
    return ParseHighscores(response.text);
}
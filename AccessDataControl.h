#pragma once

#include "Database.h"
#include "Maalesession.h"
#include <vector>

class AccessDataControl {
public:
    AccessDataControl(Database& database);

    std::vector<MaaleSession> getConsumptionData(int userId);
    bool getLatestConsumptionData(int userId, MaaleSession& outSession);

    std::vector<MaaleSession> getLeaderboardRank();
    double getEstimatedPrice(double waterLiters, double waterPricePerLiter);

private:
    Database& database_;
};
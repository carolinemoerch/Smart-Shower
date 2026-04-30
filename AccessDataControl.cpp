#include "AccessDataControl.h"

AccessDataControl::AccessDataControl(Database& database)
    : database_(database)
{}

MaaleSession AccessDataControl::getConsumptionData(int userId) {
    std::vector<MaaleSession> userSessions = database_.getSessionsByUserId(userId);

    if (!userSessions.empty()) {
        return userSessions.back();
    }

    return MaaleSession(userId);
}

std::vector<MaaleSession> AccessDataControl::getLeaderboardRank() {
    return database_.getSessions();
}

double AccessDataControl::getEstimatedPrice(double waterLiters, double waterPricePerLiter) {
    return waterLiters * waterPricePerLiter;
}
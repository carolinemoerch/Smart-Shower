#include "AccessDataControl.h"
#include <algorithm>

using namespace std;

AccessDataControl::AccessDataControl(Database& database)
    : database_(database)
{}





vector<MaaleSession> AccessDataControl::getConsumptionData(int userId) {
    return database_.getSessionsByUserId(userId);
}

bool AccessDataControl::getLatestConsumptionData(int userId, MaaleSession& outSession) {
    vector<MaaleSession> userSessions = database_.getSessionsByUserId(userId);
    if (userSessions.empty()) {
        return false;
    }

    outSession = userSessions.back();
    return true;
}



vector<MaaleSession> AccessDataControl::getLeaderboardRank() {
    vector<MaaleSession> sessions = database_.getSessions();

    sort(sessions.begin(), sessions.end(),
         [](const MaaleSession& a, const MaaleSession& b) {
             return a.getTotalWaterUsed() < b.getTotalWaterUsed();
         });

    return sessions;
}



double AccessDataControl::getEstimatedPrice(double waterLiters, double waterPricePerLiter) {
    return waterLiters * waterPricePerLiter;
}
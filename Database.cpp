#include "Database.h"

using namespace std;


Database::Database() {}

void Database::saveSession(const MaaleSession& session) {
    sessions_.push_back(session);
}

vector<MaaleSession> Database::getSessions() const         //funktionen henter alle gemte sessions. den bruges til leaderboard eller samlet statistik
{
    return sessions_;    //returnerer hele listen.
}


vector<MaaleSession> Database::getSessionsByUserId(int userId) const       //dunktionen henter kun sessions for en bestemt bruger
{
    vector<MaaleSession> result;

    for (const auto& session : sessions_) {
        if (session.getUserId() == userId) {        //mangler getUser i maaleSession
            result.push_back(session);
        }
    }

    return result;
}

void Database::updateStatistics() {
    //udfylder senere, fx nårvi laver samlet statistik eller leaderboard?
}
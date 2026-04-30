#include "Database.h"
#include <vector>

Database::Database(std::vector<MaaleSession> data_) : data(data_) {
}

void Database::saveSession(MaaleSession session) {
    data.push_back(session);

}

std::vector<MaaleSession> Database::getAllSessions() {
    return data;
}

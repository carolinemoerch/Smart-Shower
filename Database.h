#pragma once

#include "Maalesession.h"
#include <vector>

class Database {
public:
    Database();

    void saveSession(const MaaleSession& session);

    std::vector<MaaleSession> getSessions() const;

    std::vector<MaaleSession> getSessionsByUserId(int userId) const;

    void updateStatistics();

private:
    std::vector<MaaleSession> sessions_;
};


#pragma once
#include <vector>
#include "Maalesession.h"

class Database{
public:
    void saveSession(MaaleSession session);
    Database(std::vector<MaaleSession> data_);
    Database();
    
    std::vector<MaaleSession> getAllSessions();

private:
    std::vector<MaaleSession> data;

};

#pragma once
#include <vector>
#include "Maalesession.h"

class Database{
public:
    void saveSession(MaaleSession);
    Database(std::vector<MaaleSession> data_);

private:
    std::vector<MaaleSession> data;

};

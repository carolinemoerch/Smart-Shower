#include "MaaleSession.h"

MaaleSession::MaaleSession(int userId):UserId_(userId),
      Date_(0),
      TotalWaterUsed_(0.0),
      AverageFlowRate_(0.0),
      AverageTemp_(0.0),
      ShowerLength_(0),
      EnergyUsed_(0),
      AverageEnergyPrice_(0.0),
      WaterPrice_(0.065), // stat fra danva 65 kr / 1000 liter = 0.065 kr per liter
      EnergyPrice_(2.5) //kr pr kwh danmarks statistik 2025
      {};


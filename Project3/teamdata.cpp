#include "teamdata.h"
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "priorityqueue.h"
#include <cmath>
#include <iostream>

TeamData::TeamData(std::string filename) {
     std::ifstream inFile(filename);
     if (!inFile){
          std::cerr << "Error opening file " << filename << std::endl;
     }

     nlohmann::json data = nlohmann::json::parse(inFile);
     inFile.close();
     numPlayers_ = (data["metadata"]["numPlayers"]);
     std::vector<nlohmann::json> vec = data["teamStats"];

     winPercentages_.resize(numPlayers_, std::vector<double>(numPlayers_));

     for (long unsigned int a = 0; a < vec.size(); a++){
          int i = data["teamStats"][a]["playerOne"];
          int j = data["teamStats"][a]["playerTwo"];
          double winPct = data["teamStats"][a]["winPercentage"];
          double abs50win = fabs(50.0 - winPct);
          winPercentages_.at(i).at(j) = abs50win;
     }
}

int TeamData::numPlayers() const{
     return numPlayers_;
}
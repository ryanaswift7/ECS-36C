#ifndef _TEAMDATA_H_
#define _TEAMDATA_H_

#include <string>
#include <vector>

class TeamData {
 public:
 	TeamData(std::string filename);
 	int numPlayers() const;
 	double winPercentages(int i, int j) const { return winPercentages_[i][j]; }

 private:
	int numPlayers_;
	std::vector<std::vector<double>> winPercentages_;

};  // class TeamData

#endif  // _TEAMDATA_H_
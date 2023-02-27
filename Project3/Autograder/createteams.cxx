#include <fstream>
#include <map>
#include <iostream>
#include "json.hpp"
#include <ostream>
#include <utility>
#include <vector>
#include "priorityqueue.h"
#include "teamdata.h"

//----------BEGIN MAIN FUNCTION------------------
int main(int argc, char** argv) {
     if (argc != 2){
          std::cerr << "Usage: ./createteams.exe <filename.json>" << std::endl;
     }

     TeamData data(argv[1]); // parse team stats from file
     // create heap that holds all teams of 2 (would really be n choose k)
     // where n = numPlayers_ and k = 2, but didn't feel like writing a
     // factorial function and I don't care enough about over-allocating memory
     PriorityQueue pq(data.numPlayers()*data.numPlayers()); 


     for (int i = 0; i < data.numPlayers(); i++){
          for (int j = 0; j < data.numPlayers(); j++){
               if (data.winPercentages(i, j) != 0){ // workaround to avoid isses like not having value for [0,1] but not [1,0]
                    Value val = std::make_pair(i,j);
                    Key k = data.winPercentages(i, j);
                    KeyValuePair kv = std::make_pair(k, val);
                    pq.insert(kv);
               }
          }
     }

     std::vector<std::pair<int, int>> teams_vec(data.numPlayers()/2); // this vector will hold all of the team pairs to output
     std::map<int,std::pair<int,int>> playerMap; // this map will hold individual players (key) and their team pairing (value)
     int n = 0; // this will be the counter for how many teams we have (total will be half numPlayers_)

     // This loop looks at the root of the min heap (which contains keys of how close the win pct is to 50)
     // then checks to see if either of the players on the currently proposed team have already found
     // a better teammate (are either of the players already in the map), then adds the pairing to the
     // list of finalized teams if neither of them have already been paired up
     while (n < (data.numPlayers()/2)){
          KeyValuePair currentMin = pq.removeMin(); // remove min from heap and store it
          // count returns 1(true) if the key is present, and 0(false) if it is not present
          if(!(playerMap.count(currentMin.second.first) || playerMap.count(currentMin.second.second))){
               // if neither of the players are already in the map, finalize them as a team
               playerMap.insert({currentMin.second.first, currentMin.second}); // add first teammate to map
               playerMap.insert({currentMin.second.second, currentMin.second}); // add second teammate to map
               teams_vec[n] = currentMin.second; // add it to the list of team pairings
               ++n; // increase number of teams finalized
          }
     }

     nlohmann::json output; // create a JSON to be output to stdout
     output["teams"] = teams_vec;
     std::cout << output.dump(2) << std::endl;
     return 0;
     
}

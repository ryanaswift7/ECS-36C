#include <iostream>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

//------------------- BEGINNING OF MAIN FUNCTION ----------------

int main(int argc, char** argv) {

     // make sure there's the correct number of arguments
     if (argc != 2){
          std::cerr << "Usage: ./sortedverification <file.json>" << std::endl;
          return -1;
     }

     std::ifstream file(argv[1]); // create input file object

     // make sure file can be opened
     if (!file){
          std::cerr << "Error: cannot open file " << argv[1] << std::endl;
          return -1;
     }

     nlohmann::json data = nlohmann::json::parse(file); // pull the info out of the file
     nlohmann::json j_out; // create JSON to be output
     
     j_out["metadata"]["arraySize"] = data["metadata"]["arraySize"]; // copy arraySize into output JSON
     j_out["metadata"]["numSamples"] = data["metadata"]["numSamples"]; // copy numSamples into output JSON
     j_out["metadata"]["file"] = argv[1]; // include filename

     int inversions = 0; // create counter for number of samples with consecutive inversions
     int len = data["metadata"]["arraySize"]; // get length of samples (only works if all same size)

     for (auto itr = data.begin(); itr != data.end(); ++itr){    // iterate over keys
          bool sampleHasInversion = false; // will change to true if sample has at least 1 inversion

          if (itr.key() != "metadata"){      // only want to deal with samples
               for (int i = 0; i < len-1; i++){       // len-1 because i+1 on the last element is out of bounds
                    if (itr.value()[i] > itr.value()[i+1]){      // see if anything is out of order
                         sampleHasInversion = true;
                         std::stringstream ss_index;
                         ss_index << i;      // create stringstream for index key
                         j_out[itr.key()]["ConsecutiveInversions"][ss_index.str()] = {itr.value()[i], itr.value()[i+1]};
                         j_out[itr.key()]["sample"] = itr.value();
                    }
               }
               
          }
          if (sampleHasInversion){
               inversions++;
          }
     }
     
     j_out["metadata"]["samplesWithInversions"] = inversions;
     file.close(); // close file
     std::cout << j_out.dump(2) << std::endl;
     return 0;
}

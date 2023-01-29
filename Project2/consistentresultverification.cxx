#include <iostream>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <vector>

//------------------------- BEGINNING OF MAIN FUNCTION -------------------------

int main(int argc, char** argv) {

     // make sure correct number of arguments are used
     if (argc != 3){
          std::cerr << "Usage: ./consistentresultverification <file1.json> <file2.json>" << std::endl;
          return -1;
     }

     std::ifstream file1(argv[1]); // create input file object
     // make sure the first file can be opened
     if (!file1){
          std::cerr << "Error: cannot open file " << argv[1] << std::endl;
          return -1;
     }
     nlohmann::json data1 = nlohmann::json::parse(file1); // pull the info out of file1
     file1.close();

     std::ifstream file2(argv[2]);
     // make sure the second file can be opened
     if (!file1){
          std::cerr << "Error: cannot open file " << argv[2] << std::endl;
          return -1;
     }
     nlohmann::json data2 = nlohmann::json::parse(file2); // pull the info out of file2
     file2.close();

     nlohmann::json j_out; // create JSON to be output
     j_out["metadata"]["File1"]["name"] = argv[1]; // include filename for first file
     j_out["metadata"]["File2"]["name"] = argv[2]; // include filename for second file
     j_out["metadata"]["File1"]["arraySize"] = data1["metadata"]["arraySize"]; // copy arraySize into output JSON f1
     j_out["metadata"]["File2"]["arraySize"] = data2["metadata"]["arraySize"]; // copy arraySize into output JSON f2
     j_out["metadata"]["File1"]["numSamples"] = data1["metadata"]["numSamples"]; // copy numSamples into output JSON f1
     j_out["metadata"]["File2"]["numSamples"] = data2["metadata"]["numSamples"]; // copy numSamples into output JSON f2

     int len  = data1["metadata"]["arraySize"]; // only works if all arrays identical size
     int conflicts = 0; // create counter for number of samples with at least one mismatch

     for (auto itr1 = data1.begin(); itr1 != data1.end(); ++itr1){    // iterate over keys in file 1
          for (auto itr2 = data2.begin(); itr2 != data2.end(); ++itr2){    // iterate over keys in file 2
               if ((itr1.key() == itr2.key()) && (itr1.key() != "metadata")){ // only compare same sample numbers across files, exclude metadata
                    bool sampleHasConflict = false; // indicator of whether there is at least one mismatch
                    for (int i = 0; i < len; i++){ // iterate over entire array
                         if (itr1.value()[i] != itr2.value()[i]){ // check if elements at each index are identical
                              std::stringstream ss_index;
                              ss_index << i;
                              j_out[itr1.key()]["Mismatches"][ss_index.str()] = {itr1.value()[i], itr2.value()[i]};
                              sampleHasConflict = true;
                         }
                    }
                    if (sampleHasConflict){ // iterate mismatch counter and add both arrays for the sample to the JSON
                         conflicts++;
                         j_out[itr1.key()][argv[1]] = itr1.value(); 
                         j_out[itr1.key()][argv[2]] = itr2.value();
                    }
               }
          }
     }
     
     j_out["metadata"]["samplesWithConflictingResults"] = conflicts;
     std::cout << j_out.dump(2) << std::endl;
     return 0;
}
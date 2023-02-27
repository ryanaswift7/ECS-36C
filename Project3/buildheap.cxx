#include "json.hpp"
#include "priorityqueue.h"
#include <fstream>
#include <iostream>

//---------------BEGIN MAIN FUNCTION-------------------
int main(int argc, char** argv) {
     if (argc != 2) // make sure exactly 1 file used as argument
          std::cerr << "Usage: ./buildheap.exe <file.json>" << std::endl;
     
     std::ifstream inFile(argv[1]);


     if (!inFile) // make sure the file can be opened
          std::cerr << "Error opening file " << argv[1] << std::endl;

     nlohmann::json df = nlohmann::json::parse(inFile);
     inFile.close();

     const int SIZE = df["metadata"]["maxHeapSize"];
     
     PriorityQueue minHeap(SIZE);

    for (auto itr = df.begin(); itr != df.end(); ++itr){
          if (itr.key() != "metadata"){
               if (itr.value()["operation"] == "insert"){
                    Key newKey = itr.value()["key"]; // insert function takes a Key type parameter
                    minHeap.insert(newKey);
               }
               else if (itr.value()["operation"] == "removeMin"){
                    minHeap.removeMin();
               }
               else{
                    std::cerr << "Invalid operation requested" << std::endl;
                    return 0;
               }
          }
    }

    // Now minHeap should be finished, so create JSON object from it
    nlohmann::json output; // JSON to be output
    output = minHeap.JSON();
    output["metadata"]["maxHeapSize"] = df["metadata"]["maxHeapSize"];
    output["metadata"]["numOperations"] = df["metadata"]["numOperations"];

    std::cout << output.dump(2) << std::endl;
    return 0;

}
//--------------END MAIN FUNCTION----------------------
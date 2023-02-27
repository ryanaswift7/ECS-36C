#include <iostream>
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include "insertionsort.h"
#include "mergesort.h"
#include "quicksort.h"
#include <chrono> // NOLINT (build/c++11)

//------------------ BEGINNING OF MAIN FUNCTION --------------------------------

int main(int argc, char** argv) {

     if (argc != 2){ // ensure correct number of arguments
          std::cerr << "Usage: ./timealgorithms <file.json>" << std::endl;
          return -1;
     }
     std::ifstream inFile(argv[1]); // create input file object
     if (!inFile){ // make sure file cant be opened
          std::cerr << "Error: cannot open file " << argv[1] << std::endl;
     }

     nlohmann::json data = nlohmann::json::parse(inFile); // pull data out of file
     inFile.close(); // close input file

     std::ofstream outFile("TimeAlgosOutput.csv"); // open CSV file for output
    
     // Initialize all of the counters
     int ins_comp = 0;
     int ins_mem = 0;
     int merg_comp = 0;
     int merg_mem = 0;
     int quick_comp = 0;
     int quick_mem = 0;
     
     // set column names
     std::cout << "Sample,InsertionSortTime,InsertionSortCompares,InsertionSortMemaccess,MergeSortTime,MergeSortCompares,MergeSortMemaccess,QuickSortTime,QuickSortCompares,QuickSortMemaccess" << std::endl;
     outFile << "Sample,InsertionSortTime,InsertionSortCompares,InsertionSortMemaccess,MergeSortTime,MergeSortCompares,MergeSortMemaccess,QuickSortTime,QuickSortCompares,QuickSortMemaccess" << std::endl;

     std::vector<int> currentSampleVector = {};

     for (auto itr = data.begin(); itr != data.end(); ++itr) {

          if (itr.key() != "metadata"){

               currentSampleVector = data[itr.key()].get<std::vector<int>>(); // extract num array from sample

               // Reset all counters
               ins_comp = 0;
               ins_mem = 0;
               merg_comp = 0;
               merg_mem = 0;
               quick_comp = 0;
               quick_mem = 0;
               
               //-------------- INSERTION SORT -------------------------------------------
               std::chrono::high_resolution_clock::time_point start_i;
               start_i = std::chrono::high_resolution_clock::now();

               InsertionSort(&currentSampleVector, ins_comp, ins_mem);

               auto end_i = std::chrono::high_resolution_clock::now();
               double elapsed_us_i = std::chrono::duration<double, std::micro>(end_i - start_i).count();

               std::cout << itr.key() << ',' << elapsed_us_i << ',' << ins_comp << ',' << ins_mem << ',';
               outFile << itr.key() << ',' << elapsed_us_i << ',' << ins_comp << ',' << ins_mem << ',';

               //---------------- MERGESORT ------------------------------------------------------

               // Reset vector
               currentSampleVector = data[itr.key()].get<std::vector<int>>(); // extract num array from sample

               std::chrono::high_resolution_clock::time_point start_m;
               start_m = std::chrono::high_resolution_clock::now();

               MergeSort(&currentSampleVector, merg_comp, merg_mem);

               auto end_m = std::chrono::high_resolution_clock::now();
               double elapsed_us_m = std::chrono::duration<double, std::micro>(end_m - start_m).count();

               std::cout << elapsed_us_m << ',' << merg_comp << ',' << merg_mem << ',';
               outFile << elapsed_us_m << ',' << merg_comp << ',' << merg_mem << ',';

               //----------------- QUICKSORT -------------------------------------------------------

               // Reset Vector
               currentSampleVector = data[itr.key()].get<std::vector<int>>(); // extract num array from sample

               std::chrono::high_resolution_clock::time_point start_q;
               start_q = std::chrono::high_resolution_clock::now();

               QuickSort(&currentSampleVector, quick_comp, quick_mem);

               auto end_q = std::chrono::high_resolution_clock::now();
               double elapsed_us_q = std::chrono::duration<double, std::micro>(end_q - start_q).count();

               std::cout << elapsed_us_q << ',' << quick_comp << ',' << quick_mem << std::endl;
               outFile << elapsed_us_q << ',' << quick_comp << ',' << quick_mem << std::endl;
          }

     }

     // Close the output file
     outFile.close();
     return 0;

}
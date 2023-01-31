// Quicksort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include <vector>

void QuickSort(std::vector<int>* numbers, int& comparisons, int& memory_accesses);
void QuickSortRecurse(std::vector<int>* numbers, int i, int k, int& comparisons, int& memory_accesses);
int Partition(std::vector<int>* numbers, int i, int k, int& comparisons, int& memory_accesses);
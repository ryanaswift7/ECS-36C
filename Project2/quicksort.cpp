// Quicksort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include "quicksort.h"


void QuickSort(std::vector<int>* numbers, int& comps, int& mem_acc) {
   QuickSortRecurse(numbers, 0, numbers->size() - 1, comps, mem_acc);
}

void QuickSortRecurse(std::vector<int>* numbers, int i, int k, int& comps, int& mem_acc) {
   int j = 0;
   
   /* Base case: If there are 1 or zero elements to sort,
    partition is already sorted */
   if (i >= k) {
      return;
   }
   
   /* Partition the data within the array. Value j returned
    from partitioning is location of last element in low partition. */
   j = Partition(numbers, i, k, comps, mem_acc);
   
   /* Recursively sort low partition (i to j) and
    high partition (j + 1 to k) */
   QuickSortRecurse(numbers, i, j, comps, mem_acc);
   QuickSortRecurse(numbers, j + 1, k, comps, mem_acc);
   
   return;
}

int Partition(std::vector<int>* numbers, int i, int k, int& comps, int& mem_acc) {
   int l = 0;
   int h = 0;
   int midpoint = 0;
   int pivot = 0;
   int temp = 0;
   bool done = false;
   
   /* Pick middle element as pivot */
   midpoint = i + (k - i) / 2;
   pivot = (*numbers)[midpoint];
   mem_acc++; // midpoint
   
   l = i;
   h = k;
   
   while (!done) {
      
      /* Increment l while numbers[l] < pivot */
      while ((*numbers)[l] < pivot) {
         comps++;
         mem_acc++; // just l, don't count pivot
         ++l;
      }
      comps++; mem_acc++; // account for final access and comparison which breaks loop

      /* Decrement h while pivot < numbers[h] */
      while (pivot < (*numbers)[h]) {
         comps++;
         mem_acc++; // h
         --h;
      }
      comps++; mem_acc++; // accounts for final access and comparison which breaks loop

      /* If there are zero or one elements remaining,
       all numbers are partitioned. Return h */
      if (l >= h) {
         done = true;
      }
      else {
         /* Swap numbers[l] and numbers[h],
          update l and h */
         temp = (*numbers)[l];
         (*numbers)[l] = (*numbers)[h];
         (*numbers)[h] = temp;
         
         mem_acc += 4; // l, h
         ++l;
         --h;
      }
   }
   
   return h;
}
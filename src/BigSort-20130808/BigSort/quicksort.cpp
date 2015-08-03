/* Modified based on External Sort by Dr. David Carlson
 */

#include "globals.h"
#define NUM_SAMPLES 6

inline long long getPivot(TYPE* Buf, long long Lower, long long Upper)
{
   long long length = Upper-Lower+1;
   TYPE samples[NUM_SAMPLES];
   long long sample_idx[NUM_SAMPLES];
   // get random samples
   TYPE min, max, middle;
   long long idx = Lower;
   long long interval = length/(NUM_SAMPLES-1);
   for(int n = 0; n < NUM_SAMPLES; n++)
   {
       sample_idx[n] = idx;
       samples[n] = Buf[sample_idx[n]];
       if (n==0)
       {
         min = max = samples[n];
       } else {
         min = (samples[n] < min) ? samples[n] : min;
         max = (samples[n] > max) ? samples[n] : max;
       }
       idx += interval;
   }
   // get the sample with the middle number
   middle = ((min+max)>>1);
   long long midx;
   unsigned long long mdist;
   for(int n = 0; n < NUM_SAMPLES; n++)
   {
      if (n==0)
      {
        mdist = (samples[n] > middle) ? samples[n] - middle : middle - samples[n];
        midx = sample_idx[n];
      } else {
        TYPE dist = (samples[n] > middle) ? samples[n] - middle : middle - samples[n];
        if (dist < mdist){
            mdist = dist;
            midx = sample_idx[n];
        }
      } 
   }
   return midx;
}

long long Partition(TYPE* Buf, long long Lower, long long Upper)
   {
   long long Left, Right, Pidx;
   TYPE Pivot;
   TYPE temp;
   
   //Pivot = Buf[Lower];
   // Pick the pivot using sampling
   Pidx = getPivot(Buf, Lower, Upper);
   //Pidx = Lower;
   Pivot = Buf[Pidx];
   Buf[Pidx] = Buf[Lower];
   Buf[Lower] = Pivot;

   Left = Lower;
   Right = Upper;

   while (Left < Right)
      {
      // Scan from left, skipping items that belong there. Use case-insensitive compare.
      while (Buf[Left] <= Pivot && (Left < Right))
         Left++;
      // Scan from right, skipping items that belong there.  Use case-insensitive compare.
      while (Buf[Right] > Pivot && (Left <= Right))
         Right--;
      if (Left < Right)
         {
             temp = Buf[Left];
             Buf[Left] = Buf[Right];
             Buf[Right] = temp;
         }
      }

   Buf[Lower] = Buf[Right];
   Buf[Right] = Pivot;
   return Right;  // return the pivot index
   }

void QuickSort(TYPE* Buf, long long Lower, long long Upper, int rank)
   {
   long long PivotIndex;

   if (Lower < Upper)
      {
      PivotIndex = Partition(Buf, Lower, Upper);
      if(PivotIndex-1 > Lower)
        QuickSort(Buf, Lower, PivotIndex - 1, rank);   // sort left side
      if(PivotIndex+1 < Upper)
        QuickSort(Buf, PivotIndex + 1, Upper, rank);   // sort right side
      }
   }


void QuickSortPar(TYPE* Buf, long long Lower, long long Upper, int rank)
   {
   long long PivotIndex;
   if (Upper <= Lower)
      return;

   if (Lower < Upper)
      {
      PivotIndex = Partition(Buf, Lower, Upper);
#pragma omp parallel sections
        {   
#pragma omp section
          {
        QuickSortPar(Buf, Lower, PivotIndex - 1, rank);   // sort left side
          }

#pragma omp section
          {
        QuickSortPar(Buf, PivotIndex + 1, Upper, rank);   // sort right side
          }
        }
      }
   }


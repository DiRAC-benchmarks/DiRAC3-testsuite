/* Modified based on External Sort by Dr. David Carlson
 */

#include "globals.h"

long long PartitionInt(int* Buf, long long Lower, long long Upper)
   {
   long long Left, Right;
   int Pivot;
   int temp;

   Pivot = Buf[Lower];
   Left = Lower;
   Right = Upper;

   while (Left < Right)
      {
      // Scan from left, skipping items that belong there. Use case-insensitive compare.
      while (Buf[Left] <= Pivot && (Left < Upper))
         Left++;
      // Scan from right, skipping items that belong there.  Use case-insensitive compare.
      while (Buf[Right] > Pivot)
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

void QuickSortInt(int* Buf, long long Lower, long long Upper, int rank)
   {
   long long PivotIndex;

   if (Lower < Upper)
      {
      PivotIndex = PartitionInt(Buf, Lower, Upper);
      if(PivotIndex-1 > Lower)
        QuickSortInt(Buf, Lower, PivotIndex - 1, rank);   // sort left side
      if(PivotIndex+1 < Upper)
        QuickSortInt(Buf, PivotIndex + 1, Upper, rank);   // sort right side
      }
   }



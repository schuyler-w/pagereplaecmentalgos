/*
 * Program 4
 * Name: Schuyler Wang
 * cssc1766@edoras.sdsu.edu
 * August 6, 2023
 */

/*
 *  Implementation of FIFO and LRU page replacement algorithm
 *  Please add appropriate level of comments in this file 
 */

#include "vmalgorithm.h"
#include <stdio.h>
#include <time.h>

int *accessPattern;
int ReferenceSZ; 
int FrameNR;
PageFrame memory;

/* Generate an access pattern
 * Example: 3, 2, 2, 1, 1  indicates the page 3, 2, 2, 1, 1 in order
 */
void generateAccessPattern()
{
   int i;
   srand(time(0));
   accessPattern = (int *)malloc( sizeof(int) * AccessPatternLength);   
   printf("The randomized Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++) {
		     accessPattern[i] = rand() % ReferenceSZ;
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");
}

/*
 * Initialize the parameters of simulated memory
 */
void initializePageFrame()
{
   int i;
   memory.PageFrameList = (int *)malloc( sizeof(int)* FrameNR );    // dynamic allocated FrameNR frames to be used in memory
   memory.nextToReplaced =0;          // indicate the new frame to be replaced as 0
   for(i=0; i< FrameNR; i++) {
			memory.PageFrameList[i] = -1;  // initialization to -1 indicating all frames are unused 
   }

}

// Print the pages loaded in memory
void printPageFrame()
{
   int i;
   for(i=0; i< FrameNR; i++) {
			printf("%2d ",memory.PageFrameList[i]);       
   }
   printf("\n");
}

/*
 *  Print the access pattern in order
 */
void printAccessPattern()
{
   int i;
   printf("The Same Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");
}


/*
 * Return: number of the page fault using FIFO page replacement algorithm
 */
int FIFO()
{
    int pageFaults = 0;
    int *queue = (int *)malloc(sizeof(int) * FrameNR); // Queue to keep track of loaded pages
    int queueFront = 0; // Front of the queue

    for (int i = 0; i < AccessPatternLength; i++) {
        int currentPage = accessPattern[i];
        int pageLoaded = 0; // Flag to check if page is already in memory

        // Check if the current page is already in memory
        for (int j = 0; j < FrameNR; j++) {
            if (memory.PageFrameList[j] == currentPage) {
                pageLoaded = 1; // Page found in memory
                break;
            }
        }

        if (!pageLoaded) {
            pageFaults++;
            memory.PageFrameList[queueFront] = currentPage; // Replace the oldest page
            queueFront = (queueFront + 1) % FrameNR; // Move the queue to the front
        }

        // Print the current state of PageFrameList
        printPageFrame();
    }

    free(queue);
    return pageFaults;
}

/*
 * Return: number of the page fault using LRU page replacement algorithm
 */
int LRU()
{
    int pageFaults = 0;
    int *lastUsed = (int *)malloc(sizeof(int) * FrameNR); // Array to keep track of last used pages

    // Initialize the lastUsed array
    for (int i = 0; i < FrameNR; i++) {
        lastUsed[i] = -1; // -1 indicates never used
    }

    for (int i = 0; i < AccessPatternLength; i++) {
        int currentPage = accessPattern[i];
        int pageLoaded = 0; // Flag to check if page is already in memory
        int leastUsedIndex = 0; // Index of the least recently used page

        // Check if the current page is already in memory
        for (int j = 0; j < FrameNR; j++) {
            if (memory.PageFrameList[j] == currentPage) {
                pageLoaded = 1; // Page found in memory
                lastUsed[j] = i; // Update the last used time
                break;
            }
        }

        if (!pageLoaded) {
            pageFaults++;

            // Find the least recently used page to replace
            int minLastUsed = lastUsed[0];
            for (int j = 1; j < FrameNR; j++) {
                if (lastUsed[j] < minLastUsed) {
                    minLastUsed = lastUsed[j];
                    leastUsedIndex = j;
                }
            }

            memory.PageFrameList[leastUsedIndex] = currentPage; // Replace the least recently used page
            lastUsed[leastUsedIndex] = i; // Update the last used time
        }

        // Print the current state of PageFrameList
        printPageFrame();
    }

    free(lastUsed);
    return pageFaults;
}
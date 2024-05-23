## Problem:
For every possible 5×5 A and B patterns, exact matches will be searched, and the coordinate result will be listed at the console.

## Introduction:
I would like to briefly mention the things that I covered within this document. First, I will be explaining the solution I found to the sample image processing problem by explaining my serial and the two different parallelization techniques’ algorithms. Secondly, I will go over how to run the source codes I am sharing and the design of the program. And finally, I will share my performance results that I got with serial version of the algorithm and the parallel versions of the algorithm, with efficiency and speedup metrics.  About the design of the program, the source codes have been written in C++ language. OpenMP library have been used to parallelize the search algorithm. Two different parallelization techniques were used with OpenMP. 

## Path to Solution:

### Solution:
First, I have generated a sample image using Python, to use in my C++ algorithm. An image with repetitions would be nice to see if my search results are correct or not, therefore I have built an image with repetitions that I know.

The image above was used during the tests I have done at C++ side. For example, if you consider the first 5x5 pattern of the image above, I should find first entries of the matching patterns, i.e. the [0,0] points of the patterns.

So, my serial algorithm’s solution is the following to find these matched points:

- Read input image (asking path of the image from user)
- Define integer nRows and nCols as the image’s height and widths, respectively.
- Define patchSize as the desired search pattern’s size.
- For every pixel x from 0 to nRows – patchSize:
    - For every pixel y from 0 to nCols – patchSize:
        - Find pattern 5x5 at point [x,y] and save it to patch variable.
        - Search the whole image once again (x2, y2s) pixel by pixel and at every pixel, extract a new pattern (saved it to patch2 variable) and see if patch’s points exact match with patch2’s points, we break for loops and keep searching from the next point even if there is a single mismatch to speed up the process.
        - If patch == patch2 after checking all entries of the patterns, mark it as matched and store this matched pattern’s initial point [x2,y2] at the vector of points.

The above algorithm is the serial version of the solution. To speed up this serial algorithm with parallelization, I have used OpenMP paradigm.

In my first version, since there is no dependency between rows, I have parallelized my first for loop to divide my jobs to threads row-wise. I.e. If this image is 150x150 and if my thread count is 6, then the first 25 rows will be handled from thread 0, the 2nd 25 rows will be handled from thread 1, the 3rd 25 rows will be handled from thread 2 and so on (See image below).

In my second version of the OpenMP parallelization, I have parallelized my second for loop and distributed the work to the threads equally and column-wise this time. I want to explain my parallel algorithms with also visualizing them with 6 threads on 150x150 images. The algorithms can also be seen from the following image:

[Version 2 Parallelization Image]

As we can see, on version 1, we equally distributed threads to the rows and each thread needs to finish the assigned rows before moving on to the next job (we can follow this from outputs as well). On the other hand, in version 2, column-wise distribution happened to the threads. Threads finish their assigned columns first before moving on to the next columns. Assignment is distributed equally between threads on both versions.

## How to run source codes:

There is only a single main.cpp file which is using “OpenMP” and “OpenCV” libraries. Running the main.cpp file directly after building it is enough to run the search algorithm. In order to run the serial version of the algorithm during the process, comment out its’ function call within the main function. Otherwise, only parallel versions of the algorithms with the number of threads you are giving to it at the start of the program will be running.

Once the program is run successfully after building it, it will start getting inputs from the user and will complete the task by writing results to the given results file name. An example from the console:


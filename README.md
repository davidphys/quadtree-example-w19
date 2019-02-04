Example Quadtree project for physics 141/241
===
Updated Feb 4 2019, David Moore.

This code as-is runs an O(N log(N)) single threaded simulation on 100,000 particles in two dimensions.

Compilation instructions
===

To run, extract all files into a directory. Open up terminal and 
cd into the directory. Then type "make run". This will:
1. Create the "out" directory.
1. Compile main.cpp and other source files into a quadtree executable
2. run ./quadtree, which outputs 30 .bmp files
3. use convert to put the bitmaps into a .gif


How the source code works:
===

My explanation of the Barnes-Hut algorithm is online [at my website here](http://mathandcode.com/2016/02/16/quadtree.html). 
There are some broken links (I built the website 3 years ago and haven't updated it much!) so please email me if you find
anything missing that you need. 

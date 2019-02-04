
#This is the default make target. It will run when we call "make" if any 
#of the files 

quadtree: ImageUtil.h ImageUtil.cpp Physics.h Physics.cpp Vector2.h main.cpp
	#Because this is in c++ we have to work with "g++" instead of "gcc".
	g++ -o quadtree main.cpp Physics.cpp ImageUtil.cpp

#This is the "run" target. It will run when we call "make run".
#It depends on the target "quadtree", so it will make sure that quadtree
#has been built first.
run: quadtree
	#make the "out" directory. If the directory already exists, 
	#this command fails. The "-" prefix says to keep going even if the command fails.
	-mkdir out
	#call the executable
	./quadtree
	#create the .gif
	convert -delay 10 -loop 0 out/image*.bmp animation.gif

#Calling "make clean" will delete the executable, all of the bitmaps,
#and also the animation.gif.
clean:
	-rm quadtree
	-rm -r out
	-rm animation.gif


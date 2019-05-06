Readme.txt
#EE569 Homework Assignment #2
#Date: February 11, 2019
#Name: Wenjun Li
#ID: 8372761120
#EMAIL: wenjunli@usc.edu
# # Compiled on Mac OS with g++# Define compiler and linker
#
1. How to run .cpp?
  Compiling: command line, g++ -o filename filename.cpp
  Running: ./filename input.raw output.raw (no need to input BytesPerPixel)
  (Note: please refer to the comment lines on the top in .cpp file for 		 specific running command)
2. File Submitted in the Package
	Prob.1
	(a) sober edge detector: 
		sobel.cpp
	
	(b) canny edge detector:
		canny.cpp (cannot run in command line, use IDE with OpenCV)
	
	(c) structured edge: 
		spDemo.m (in edges-master folder)	
	
	(c) performance evaluation:
 		edgesDemo.m (in edges-master folder)

	Prob.2
	(a) dithering:
		dithering.cpp
		random.cpp
	
	(b) error diffusion: 
		error_diffusion_fs.cpp
		error_diffusion_JJN.cpp
		error_diffusion_Stucki.cpp

	(c) color halftoning with error diffusion:
		separable_ED.cpp
		MBVQ.cpp


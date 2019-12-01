How to run on linux:

	Step1: make
		After this step, we will get 2 project files named 'juliaset'
		and 'visualization'.
	Step2: mpirun -np 16 juliaset 0.3 -0.4
		The first paramete is #processors.
		The last 2 parameters are the real and imaginary part of 'c'.
		After this step, we will get an output file named 'result.txt'.
	Step3: ./visualization
		This project is used to read the information from the file
		named 'result.txt' and visualize it outputing to a .jpg file
		meanwhile.
## User Guide
* All the programs are in a folder called “programs”. In order to run them:
  1) Move the memory file and the instruction file of the program you want to run to the same location as the source.cpp file
  2) Write its name in the main in the as.read_file (“ “) & as.read_memory (“ “) lines 
* You can find the outputs of the simulated program in a folder named outputs inside the program folder.

## Assumptions

To implement all the RISV32 instructions simulator. Some assumptions and design decisions were made. Including:
1)	All the instructions has to be written in lower case letters.
2)	All the instructions has to be written with no spaces between operands and one space after the instruction name, e.g.  insx op1,op2,op3
3)	In case of the immediate and store instructions, there should be no spaces between operands and there should be one space between the operands and the instruction. E.g. lw t0,0(t1)   or    addi t0,t1,3
4)	The branching labels, like exit: , loop1: , loop2: , …etc. should be written solely in a separate line between the instruction with no space between the label name and “:”.
5)	The branching label must end with “:”
6)	The first index of the memory file should always be the starting address of the program, and it always has 1 as an index.
7)	There are no pseudoinstructions supported. E.g. if you want to use “li” instruction you must use addi or any other instruction from the 40 user level instructions instead.
8)	The program doesn’t tolerate spaces.

## Testing
In the "programs and outputs" directory, you can find test programs and their respective ouputs.

***This is a brief description of some of the attached programs***

### Program 1 

Adds all the elements in a given array with a given size and saves the output in the memory
If you want to change the elements of the array, just modify them in the memory text file and modify the size of the array in the program 1 text file
**Note:**
*	the first element listed in the memory file is the starting address of the program.
*	The starting address of the array in the instructions text must equal the one in the memory text

### Program 2

A non-recursive leaf function implementing Euclid’s algorithm that finds the greatest common divisor (GCD) of two positive integers.
Note: 
*	Although the program doesn’t load or store from the memory. The file named “mem2.txt” has to be compiled with the program because it contains the starting address of the program.

### Program 3 

A recursive non-leaf function implementing Euclid’s algorithm that finds the greatest common divisor (GCD) of two positive integers.

## Bugs and Errors

The submitted simulator has no bugs or errors. It has been tested on all of the six submitted RISCV programs and the output was compatible with Venus RISC-V simulator. However, there were some issues that iterated us while making the project. Including:
1)	The pc counter was sometimes inaccurate, although it is computed in a loop that is controlled by the number of riscv instructions. it was exceeding the number of instructions, and it was making the number of instructions- which is the control variable – increase as well. The problem was solved with a break statement.
2)	If there is no return address given in the simulated instructions. The program will output “unable to find address”

In conclusion, the program outputs were as we expected them to be. 

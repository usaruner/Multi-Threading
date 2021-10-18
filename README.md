# Multi-Threading
  This Repository includes 2 different implementation of threads in c++. The dining philosopher problem and an edge detection algorithm for image processing
## Dining Philosopher
  Refers the scenario where N number of philsopher is seated in a round dining table. Between each philospher is only one fork. 
### Rules
  Each philsopher has k meals to eat <br/>
  A philsopher must have 2 forks to eat the meal. <br/>
  Each philospher may only pick up one fork at the time. <br/>
### Goal 
  finish the meal without deadlock or starvation.
### Solution
  Each diner will attempt to pick up a fork when ready if they can not pick up the fork on the oppsite side they drop their fork. This is repeated until all meals have been eaten. To prevent starvation a delay can be applied after the philsopher finishes a meal. In the program, the threads  represent philsophers, and atomic boolean pointers represent the forks.
## Robert's Cross
  Roberts Cross is a method that filters out edges for image processing. The problem is how to create threads so that the program can run in parallel.
### Equations/Filter
  A filter [[1,0],[0,-1]] and [[0,1],[-1,0]] is convoluted with the photo. resulting is grad_x and grad_y.<br/>
  Goal grad_x and grad_y is put through the mean root square equation grad = sqrt(grad_x^2 +grad_y^2) <br/>
  output[x][y] = grad <= 255 ? grad : 255
### Solution 
  Some solutions that come up were to have and iterative pointer to the pixel that is not done yet. Another solution is predetermine that areas that a thread will work on. In the implementation here the threads are assigned a set number of columns that it will work with.
### Example
  ![inital](/Lab1_Files_export/tc1.pgm)
  ![result](/Lab1_Files_export/tc1.pgm)
  

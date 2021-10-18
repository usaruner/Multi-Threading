# Multi-Threading
  This Repository includes 2 different implementation of threads in c++. The dining philosopher problem and an edge detection algorithm for image processing
## Dining Philosopher
  Refers the scenario where N number of philsopher is seated in a round dining table. Between each philospher is only one fork. 
### Rules
  Each philsopher has k meals to eat <br/>
  A philsopher must have 2 forks to eat the meal. 
  Each philospher may only pick up one fork at the time.
### Goal 
  finish the meal without deadlock or starvation.
### Solution
  Each diner will attempt to pick up a fork when ready if they can not pick up the fork on the oppsite side they drop their fork. This is repeated until all meals have been eaten.
  

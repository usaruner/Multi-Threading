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
  Each diner will attempt to pick up a fork when ready if they can not pick up the fork on the oppsite side they drop their fork. This is repeated until all meals have been eaten. To prevent starvation a delay can be applied after the philsopher finishes a meal.
## Robert's Cross
  Roberts Cross is a method that filters out edges for image processing. 
### Equations/Filter
  A filter [[1,0],[0,-1]] and [[0,1],[-1,0]] is convoluted with the photo. resulting is grad_x and grad_y. grad_x and grad_y is then mean root square equation( $f(x,y) = sqrt[2]{grad_x^2 +grad_y^2} $ )
You can use $$\LaTeX$$ to typeset formulas. A formula can be displayed inline, e.g. $$e=mc^2$$, or as a block:
$$\int_\Omega \nabla u \cdot \nabla v~dx = \int_\Omega fv~dx$$

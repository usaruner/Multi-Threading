#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

typedef struct phil_data{
  int course;
  int phil_num;
  std::atomic<bool> fork; // false - available for use, true - occupied
  std::time_t meal_time[3]; 
}phil_data;

std::time_t start_time;

int num_threads; 


/* ****************Change function below***************** */
void eat_meal(phil_data& phil, std::atomic<bool>& Rfork) {
    /* 3 course meal: Each need to acquire both forks 3 times.
     * First try for fork in front.
     * Then for the one on the right, if not fetched, put the first one back.
     * If both acquired, eat one course.
     * Record time for each meal for the philosophers.
     * Use 'std::this_thread::sleep_for (std::chrono::seconds(1));' to sleep the thread for 1 second.
     */
    bool lprox;
    bool rprox;
    int start;
    int end;
    while (phil.course < 3)
    {
        //std::cout << "in while\n";
        lprox = false;
        rprox = false;
        //std::cout << "phil " << phil.phil_num << "bool: " << &Rfork << std::endl;
        if ((phil.fork).compare_exchange_weak(lprox, true))
        {
            phil.fork = true;
            //std::cout << "phil " << phil.phil_num << " pick up for own fork: " << &phil.fork << std::endl;
            if ((Rfork).compare_exchange_weak(rprox, true))
            {
                Rfork = true;
                //std::cout << "phil " << phil.phil_num << " pick up for own fork: " << &phil.fork << std::endl;
                //std::cout << "phil " << phil.phil_num << " pick up for right fork: " << &Rfork << std::endl;
                time_t now = time(NULL);
                phil.meal_time[phil.course] = now - start_time;
                //std::cout << "phil " << phil.phil_num << " meal " << phil.course << ": started on " << start / CLOCKS_PER_SEC << std::endl;
                //while ((clock()- start)/CLOCKS_PER_SEC < 1) {

                //}
                std::this_thread::sleep_for(std::chrono::seconds(1));
                end = clock();
                //std::cout << "phil " << phil.phil_num <<" meal " << phil.course <<": ended on " << end / CLOCKS_PER_SEC << std::endl;
                phil.course++;
                phil.fork = false;
                Rfork = false;
            }
            else {
                phil.fork = false;
            }
        }
    }
}
/* ****************Change function below***************** */
void dispatch_threads(phil_data* philosophers)
{
/* Create and Join threads here
 * Each thread will represent a philosopher
 */
    std::vector<std::thread> threads;
    //std::thread(eat_meal,std::ref(philosophers[0]),std::ref(philosophers[0].fork));
    for (int i = 0; i < num_threads; i++)
    {
        threads.push_back(std::thread(eat_meal,std::ref(philosophers[i]),std::ref(philosophers[(i+1)%(num_threads)].fork)));
        std::cout << "threads " << i << " created\n";
    }
    
    for(int i = 0; i < num_threads; ++i){
        threads[i].join();
        std::cout << "threads " << i << " joined\n";
    }
    for (int i = 0; i < num_threads; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << "phil " << philosophers[i].phil_num << " meal " << j << ": started on " << philosophers[i].meal_time[j]  << std::endl;
        }
    }
}

int main( int argc, char **argv ){
  int num_philosophers, num_forks;

  if (argc < 2) {
          std::cerr << "Format: " << argv[0] << " <Number of philosophers>" << std::endl;
          return 0;
     }
    
  num_philosophers = num_threads = num_forks = std::atoi(argv[1]);
  phil_data *philosophers = new phil_data[num_threads];
  
  /* Initialize philosophers */
  for( int i = 0; i < num_philosophers; i++ ){
    philosophers[i].phil_num = i;
    philosophers[i].course   = 0;
    philosophers[i].fork     = false;
  }

  /* Syntax to record time. 
  While printing meal times for each philosopher, subtract start_time from recorded values */
  std::time(&start_time);

  /* Launch threads */
  dispatch_threads(philosophers);

  /*******************Print meal times for each philosopher here************/

  return 0;
}
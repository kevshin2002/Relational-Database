//
//  Timer.hpp
//  RGAssignment1
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef Timer_h
#define Timer_h

#include <ctime>
#include <chrono>


class Timer {
protected:
  using clock_t = std::chrono::high_resolution_clock;
  using second_t = std::chrono::duration<double, std::ratio<1> >;
  
  std::chrono::time_point<clock_t>  started;
public:
  
  Timer() : started(clock_t::now()) {}
  
  void reset() {
    started = clock_t::now();
  }
    
  double elapsed() const {
    return std::chrono::duration_cast<second_t>(clock_t::now() - started).count();
  }
};

#endif /* Timer_h */

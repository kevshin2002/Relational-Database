//
//  Timer.hpp
//  Database
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
  using Marker = std::chrono::time_point<clock_t>;
  
  Marker _started;
  
public:
  
  Timer() : _started(now()) {}
  
  Marker started() const {return _started; }
  Marker now() const {return clock_t::now(); }
  Timer& reset() {_started = now(); return *this;}

  double elapsed() const {return elapsed(_started);}
  double elapsed(const Marker &aStart) const {
    return std::chrono::duration_cast<second_t>(clock_t::now()-aStart).count();
  }
  
};

#endif /* Timer_h */

/**
 * Timer : this class can be used to time function calls
 *
 * Richie Steigerwald
 *
 * Copyright 2013 Richie Steigerwald <richie.steigerwald@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/shared_ptr.hpp>

template <typename T>
class timer {
   protected:
   typedef boost::chrono::time_point<boost::chrono::system_clock, boost::chrono::duration<double> > time_point;

   T* p;

   struct deleter {
      timer* d;
      deleter(timer* d): d(d) {};
      void operator()(T* p) {
         d->after(p);
      }
   };

   time_point begin;
   double &d;

   public:
   explicit timer(T* p, double &d) : p(p), d(d) { };

   void before(T* p) {
      begin = boost::chrono::system_clock::now();
   }

   void after(T* p) {
      time_point end = boost::chrono::system_clock::now();
      d += (end-begin).count();
   }

   boost::shared_ptr<T> operator->() {
      before(p);
      return boost::shared_ptr<T>(p,deleter(this));
   }
};

template <typename T> 
timer<T> time(T* p, double &d) {
  return timer<T>(p,d);
}



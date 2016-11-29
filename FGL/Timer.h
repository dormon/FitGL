#pragma once

#include<chrono>
#include<ctime>

template<typename TYPE>
class Timer{
  protected:
    std::chrono::time_point<std::chrono::high_resolution_clock>_start;
    std::chrono::time_point<std::chrono::high_resolution_clock>_last ;
  public:
    Timer(){
      this->reset();
    }
    void reset(){
      this->_start = std::chrono::high_resolution_clock::now();
      this->_last  = this->_start;
    }
    TYPE elapsedFromStart(){
      auto newTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<TYPE> elapsed = newTime - this->_start;
      this->_last = newTime;
      return elapsed.count();
    }
    TYPE elapsedFromLast(){
      auto newTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<TYPE> elapsed = newTime - this->_last;
      this->_last = newTime;
      return elapsed.count();
    }
};

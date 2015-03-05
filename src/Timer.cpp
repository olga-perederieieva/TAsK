#include "Timer.h"

Timer::Timer() : start_(clock()) { 

}; 

Timer::~Timer() {

}; 

void Timer::start() {
	start_ = clock();
}; 

FPType Timer::getTimePassed() const {
	return (static_cast<FPType>(clock() - start_))/ (CLOCKS_PER_SEC);
}; 


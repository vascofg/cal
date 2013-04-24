/*
 * Vehicle.h
 */
#ifndef VEHICLE_H_
#define VEHICLE_H_

class Vehicle {
	unsigned int capacity;
	unsigned int freeSeats;
public:
	unsigned int getCapacity(){
		return capacity;
	}
	int removeFreeSeats(int s)
	{
		if(s>this->freeSeats)
		{
			int tmp = freeSeats;
			freeSeats=0;
			return tmp;
		}
		else{
			this->freeSeats-=s;
			return s;
		}
	}
	void resetFreeSeats()
	{
		this->freeSeats=this->capacity;
	}
	unsigned int getFreeSeats()
	{
		return freeSeats;
	}
	Vehicle(unsigned int capacity) {
		this->capacity = capacity;
		this->freeSeats = capacity;
	}
	void addCapacity(int i){
		this->capacity+=i;
		this->freeSeats+=i;
	}
};

#endif

/*
 * Vehicle.h
 */
#ifndef VEHICLE_H_
#define VEHICLE_H_

class Vehicle {
	unsigned int capacity;
public:
	unsigned int getCapacity(){
		return capacity;
	}
	Vehicle(unsigned int capacity) {
		this->capacity = capacity;
	}
};

#endif

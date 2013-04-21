/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

class Vehicle {
	unsigned int capacity;
public:
	unsigned int getCapacity() {
		return capacity;
	}
	Vehicle(int capacity){
		this.capacity=capacity;
	}
};
#endif

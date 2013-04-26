/*
 * Vehicle.h
 */
#ifndef VEHICLE_H_
#define VEHICLE_H_
/**
 * Classe respons�vel por tratar de opera��es relacionados com o ve�culo.
 */
class Vehicle {
	unsigned int capacity;
	unsigned int freeSeats;
public:
	/**
	 * @return: Capacidade do ve�culo.
	 */
	unsigned int getCapacity(){
		return capacity;
	}
	/**
	 * @return: N�mero de pessoas que entraram no ve�culo.
	 * @param: N�mero total de pessoas no n�.
	 */
	unsigned int removeFreeSeats(unsigned int s)
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
	/**
	 * Reinicializa o valor de lugares livres (igual � capacidade do ve�culo).
	 */
	void resetFreeSeats()
	{
		this->freeSeats=this->capacity;
	}
	/**
	 * @return: N�mero de lugares livres.
	 */
	unsigned int getFreeSeats()
	{
		return freeSeats;
	}
	/**
	 * Constructor.
	 * @param: Capacidade do ve�culo.
	 */
	Vehicle(unsigned int capacity) {
		this->capacity = capacity;
		this->freeSeats = capacity;
	}
	/**
	 * Adds to the vehicle's capacity.
	 * @param: Capacity to add.
	 */
	void addCapacity(int i){
		this->capacity+=i;
		this->freeSeats+=i;
	}
};

#endif

/*
 * Vehicle.h
 */
#ifndef VEHICLE_H_
#define VEHICLE_H_
/**
 * Classe responsável por tratar de operações relacionados com o veículo.
 */
class Vehicle {
	unsigned int capacity;
	unsigned int freeSeats;
public:
	/**
	 * @return: Capacidade do veículo.
	 */
	unsigned int getCapacity(){
		return capacity;
	}
	/**
	 * @return: Número de pessoas que entraram no veículo.
	 * @param: Número total de pessoas no nó.
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
	 * Reinicializa o valor de lugares livres (igual à capacidade do veículo).
	 */
	void resetFreeSeats()
	{
		this->freeSeats=this->capacity;
	}
	/**
	 * @return: Número de lugares livres.
	 */
	unsigned int getFreeSeats()
	{
		return freeSeats;
	}
	/**
	 * Constructor.
	 * @param: Capacidade do veículo.
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

/*
 * 410 Proj 4
 * Ibrahim Toure and Tim Siltz
 * 12/9/2020
 */

#include <string>
#include "stdlib.h"
#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

//ID is just a number used to identify this particular baker
//(used with PRINT statements)
//filename is what waiter reads in orders from
Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order from file_IO
//if return == SUCCESS then anOrder
//contains new order
//otherwise return contains fileIO error
int Waiter::getNext(ORDER &anOrder) {
	int newOrder = myIO.getNext(anOrder);

	if (newOrder == SUCCESS) {
		return SUCCESS;
	} else {
		b_WaiterIsFinished = true;
		return NO_ORDERS;
	}
}

//contains a loop that will get orders from filename one at a time
//then puts them in order_in_Q then signals baker(s) using cv_order_inQ
//so they can be consumed by baker(s)
//when finished exits loop and signals baker(s) using cv_order_inQ that
//it is done using b_WaiterIsFinished
void Waiter::beWaiter() {
	ORDER nextOrder;

	while (getNext(nextOrder) == SUCCESS || b_WaiterIsFinished == false) {
		lock_guard<mutex> lck(mutex_order_inQ);
		order_in_Q.push(nextOrder);
	}
	{
		lock_guard<mutex> lck(mutex_order_inQ);
		b_WaiterIsFinished = true;
	}

	cv_order_inQ.notify_all();
}

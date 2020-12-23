/*
 * 410 Proj 4
 * Ibrahim Toure and Tim Siltz
 * 12/9/2020
 */

#include <mutex>
#include "../includes/baker.h"
#include "../includes/externs.h"

using namespace std;

mutex m;
condition_variable cv;
bool bDone = false;

//ID is just a number used to identify this particular baker
//(used with PRINT statements)
Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

//bake, box and append to anOrder.boxes vector
void Baker::bake_and_box(ORDER &anOrder) {
	int tempDonuts = 0;
	Box box;
	DONUT donut;

	for (int i = 0; i < anOrder.number_donuts; i++) {
		tempDonuts++;
		box.addDonut(donut);

		if (tempDonuts == DOZEN) {
			anOrder.boxes.push_back(box);
			box.clear();
			tempDonuts = 0;
		}
	}

	if (tempDonuts > 0) {
		anOrder.boxes.push_back(box);
	}
}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
void Baker::beBaker() {
	while (true) {
		unique_lock<mutex> ulck(mutex_order_inQ);

		if (order_in_Q.size() == 0 && b_WaiterIsFinished) {
			break;
		}

		while (order_in_Q.size() == 0 && !b_WaiterIsFinished) {
			cv_order_inQ.wait(ulck);
		}

		if (order_in_Q.size() > 0) {
			ORDER order;
			order = order_in_Q.front();
			order_in_Q.pop();

			ulck.unlock();
			bake_and_box(order);

			lock_guard<mutex> lck(mutex_order_outQ);
			order_out_Vector.push_back(order);
		}
	}
}

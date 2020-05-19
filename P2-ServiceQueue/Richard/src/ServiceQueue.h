
#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>



class ServiceQueue {

  private:
  	struct Node 
		{
			int val;
			Node* prev = nullptr;
			Node* next = nullptr;
		};

		int Q_length;
		int Q_currBuzzer;			
		Node* Q_front;				// point to front of queue
		Node* Q_back;					// point to back of queue
    std::vector<int> buzzer_bucket;  				// keeps track of buzzer-IDs that can be reused.  Operates
                                     					// as a stack so buzzer that became reusable most recently is on
                                     					// top (i.e., at back of vector).

		Node** Q_addr_ptr_arr;							// dynamic array for addresses, also used to determine buzzer ID number
		int Q_curr_arr_size;			// current array size for Q_addr

		// resize check array
		Node** resize(Node** arr) {
			int N_size = 2 * Q_curr_arr_size;				// double the array size
			Node** N_arr = new Node*[N_size];

			// initialize new array
			for (int i = 0; i < N_size; i++) {
				N_arr[i] = nullptr;
			}

			// copy entry from old array to new array
			for (int i = 0; i < Q_curr_arr_size; i++) {
				N_arr[i] = arr[i];
			}

			delete arr;										// deallocate old array
			Q_curr_arr_size = N_size;			// set to new size
			return N_arr;									// return new array
		}

  public:

	/**
	 * Constructor
	 * Description: intializes an empty service queue.
	 * 
	 * RUNTIME REQUIREMENT: O(1)
	 *
         * TODO
	 */
		ServiceQueue() {
			Q_length = 0;
			Q_front = nullptr;
			Q_back = nullptr;
			Q_currBuzzer = 0;
			//buzzer_bucket = new std::vector<int>;
			Q_curr_arr_size = 10;				
			Q_addr_ptr_arr = new Node*[10];				// initialize queue array size to 10
		}

	/**
	 * Destructor
	 * Description:  deallocates all memory assciated
	 *   with service queue 
	 *
	 * RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer 
	 *	IDs that have been used during the lifetime of the
	 *	service queue; in general, at any particular instant
	 *	the actual queue length may be less than N_b.
	 *
	 *	[See discussion of "re-using buzzers" below]
	 *
         * TODO
	 */
	~ServiceQueue() {
		Node* next = Q_front->next;

		for (int i = 0; i < Q_length; i++) {
			delete Q_front;
			Q_front = next;

			if (next != nullptr) next = Q_front->next;
		}
		
		//delete buzzer_bucket;
		buzzer_bucket.clear();
		delete Q_addr_ptr_arr;
	}

	/**
	 * Function: snapshot()
         * param:  buzzers is an integer vector passed by ref
	 * Description:  populates buzzers vector with a "snapshot"
         *               of the queue as a sequence of buzzer IDs 
         *
	 *
	 * RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
	 *		length).
	 */
	void snapshot(std::vector<int> &buzzers) {
		buzzers.clear(); 		// you don't know the history of the buzzers vector, so we had better clear it first.
		Node* currNode = Q_front;
		for (int i = 0; i < Q_length; i++) {
			buzzers.push_back(currNode->val);
			currNode = currNode->next;
		}
  }

	/**
	 * Function: length()
	 * Description:  returns the current number of
	 *    entries in the queue.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int  length() {

		return Q_length;
	}

	/**
	 * Function: give_buzzer()
         * Return:   buzzer-ID (integer) assigned to the new customer.
	 * Description:  This is the "enqueue" operation.  For us
	 *    a "buzzer" is represented by an integer (starting
	 *    from zero).  The function selects an available buzzer 
	 *    and places a new entry at the end of the service queue 
	 *    with the selected buzer-ID. 
	 *    This buzzer ID is returned.
	 *    The assigned buzzer-ID is a non-negative integer 
	 *    with the following properties:
	 *
	 *       (1) the buzzer (really it's ID) is not currently 
	 *         taken -- i.e., not in the queue.  (It
	 *         may have been in the queue at some previous
	 *         time -- i.e., buzzer can be re-used).
	 *	  This makes sense:  you can't give the same
	 *	  buzzer to two people!
	 *
	 *       (2) Reusable Buzzers:  A re-usable buzzer is 
	 *	  a buzzer that _was_ in the queue at some previous
	 *	  time, but currently is not.
	 *
         *         REQUIREMENT:  If there is one or more reusable
         *         buzzer, you MUST return one of them; furthermore,
         *         it must be the buzzer that became reusable most 
         *         MOST RECENTLY.
	 *
	 *       (3) if there are no previously-used / reusable buzzers, 
         *         the SMALLEST possible buzzer-ID is used (retrieved from 
         *         inventory).  
	 *	    Properties in this situation (where N is the current
	 *	      queue length):
	 *
	 *		- The largest buzzer-ID used so far is N-1
	 *
	 *		- All buzzer-IDs in {0..N-1} are in the queue
	 *			(in some order).
	 *
	 *		- The next buzzer-ID (from the basement) is N.
	 *
	 *    In other words, you can always get more buzzers (from
	 *    the basement or something), but you don't fetch an
	 *    additional buzzer unless you have to (i.e., no reusable buzzers).
	 *
	 * Comments/Reminders:
	 *
	 *	Rule (3) implies that when we start from an empty queue,
	 *	the first buzzer-ID will be 0 (zero).
	 *
	 *	Rule (2) does NOT require that the _minimum_ reuseable 
	 *	buzzer-ID be used.  If there are multiple reuseable buzzers, 
	 *	any one of them will do.
	 *	
	 *	Note the following property:  if there are no re-useable 
	 *	buzzers, the queue contains all buzzers in {0..N-1} where
	 *       N is the current queue length (of course, the buzzer IDs 
	 *	may be in any order.)
	 *
	 * RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"  
	 *          In other words, if there have been M calls to 
	 *		give_buzzer, the total time taken for those 
	 *		M calls is O(M).
	 *
	 *		An individual call may therefore not be O(1) so long
	 *		as when taken as a whole they average constant time.
	 *
	 */
	int  give_buzzer() {
		Node* currNode = new Node;

		if (buzzer_bucket.empty()) {
			currNode->val = Q_currBuzzer;

			if (Q_length == 0) Q_front = Q_back = currNode;						// the very first one!
			else {
				Q_back->next = currNode;							// push new customer to back of line
				currNode->prev = Q_back;		
				Q_back = currNode;
			}

			// case: queue address array size is equal to the buzzer ID, resize!
			if (Q_currBuzzer == Q_curr_arr_size) Q_addr_ptr_arr = resize(Q_addr_ptr_arr);				// time complexity of this statement will be amortized		
			
			Q_addr_ptr_arr[Q_currBuzzer] = currNode;
			Q_currBuzzer++;
			Q_length++;

			return Q_currBuzzer - 1;					 // return the buzzer ID
		}
		else {
			currNode->val = buzzer_bucket.back();					// reuse the most recent given back buzzer
			buzzer_bucket.pop_back();

			if (Q_length == 0) Q_front = Q_back = currNode;						// the very first one!
			else {
				Q_back->next = currNode;							// push new customer to back of line
				currNode->prev = Q_back;		
				Q_back = currNode;
			}

			Q_length++;
			Q_addr_ptr_arr[currNode->val] = currNode;

			return currNode->val;					 // return the buzzer ID
		}
		 
	}

	/**
	 * function: seat()
	 * description:  if the queue is non-empty, it removes the first 
	 *	 entry from (front of queue) and returns the 
	 *	 buzzer ID.
	 *	 Note that the returned buzzer can now be re-used.
	 *
	 *	 If the queue is empty (nobody to seat), -1 is returned to
	 *	 indicate this fact.
         *
         * Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int seat() {
		// case: queue is empty
		if (Q_length == 0) return -1;

		Node* currNode = Q_front;
		Q_front = Q_front->next;				// point to the new front of queue
		buzzer_bucket.push_back(currNode->val);				// take the seated buzzer ID and push to buzzer bucket for reuse
		//buzzer_bucket->push_back(currNode->val);					

		Q_addr_ptr_arr[currNode->val] == nullptr;					// set address of to-be-deleted to null
		delete currNode;													// delete previous front node
		Q_length--;																// decrement queue size

		return buzzer_bucket.back();
	}


	/**
	 * function: kick_out()
	 *
	 * description:  Some times buzzer holders cause trouble and
	 *		a bouncer needs to take back their buzzer and
	 *		tell them to get lost.
	 *
	 *		Specifially:
	 *
	 *		If the buzzer given by the 2nd parameter is 
	 *		in the queue, the buzzer is removed (and the
	 *		buzzer can now be re-used) and 1 (one) is
	 *		returned (indicating success).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool kick_out(int buzzer) {

		if (Q_addr_ptr_arr[buzzer] == nullptr) return false;  // placeholder
		else {
			buzzer_bucket.push_back(Q_addr_ptr_arr[buzzer]->val);
			Node* prev = Q_addr_ptr_arr[buzzer]->prev;
			Node* next = Q_addr_ptr_arr[buzzer]->next;

			if (prev != nullptr) prev->next = next;
			else Q_front = next;

			if (next != nullptr) next->prev = prev;
			else Q_back = prev;

			//Node* currNode = Q_addr[buzzer];
			Q_addr_ptr_arr[buzzer] = nullptr;
			delete Q_addr_ptr_arr[buzzer];
			Q_length--;

			return true;
		}
	}

	/**
	 * function:  take_bribe()
	 * description:  some people just don't think the rules of everyday
	 *		life apply to them!  They always want to be at
	 *		the front of the line and don't mind bribing
	 *		a bouncer to get there.
	 *
	 *	        In terms of the function:
	 *
	 *		  - if the given buzzer is in the queue, it is 
	 *		    moved from its current position to the front
	 *		    of the queue.  1 is returned indicating success
	 *		    of the operation.
	 *		  - if the buzzer is not in the queue, the queue 
	 *		    is unchanged and 0 is returned (operation failed).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool take_bribe(int buzzer) {
		if (Q_addr_ptr_arr[buzzer] == nullptr) return false;  // placeholder
		else {
			Node* prev = Q_addr_ptr_arr[buzzer]->prev;
			Node* next = Q_addr_ptr_arr[buzzer]->next;
			Node* currNode = Q_addr_ptr_arr[buzzer];

			if (prev != nullptr) prev->next = next;
			else return true;						// buzzer is already in front

			if (next != nullptr) next->prev = prev;
			else Q_back = prev;

			Q_front->prev = currNode;
			currNode->prev = nullptr;
			currNode->next = Q_front;
			Q_front = currNode;
			
			return true;
		}
	}



};   // end ServiceQueue class

#endif


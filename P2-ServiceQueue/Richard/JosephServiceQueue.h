
#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>




class ServiceQueue {

  private:
        /** Your private data members here! 
        *   (you should have NO PUBLIC data members! 
        *                                            
        * Nested private types also go here.  
        * For example, if your design needs some kind of
        *   structure, you would specify it here.
        */ 
		struct Node 
		{
			int val;
			Node* prev = nullptr;
			Node* next = nullptr;
		};

		int T_length;
		Node* T_front;
		Node* T_back;
		std::vector<int>* T_reuse;
	    int T_currBuzzer;
		Node** T_check;
		int T_check_size;

  private:
		Node** resize(Node** arr) 
		{
		// Resize check array
			int nsize = 2 * T_check_size;
			Node** narr = new Node*[nsize];
			// initialize new array
			for (int i = 0; i < nsize; i++) 
			{
				narr[i] = nullptr;
			}
			// copy entry from old array to new array
			for (int i = 0; i < T_check_size; i++) 
			{
				narr[i] = arr[i];
			}
			delete arr;
			T_check_size = nsize;
			return narr;
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
			 T_length = 0;
			 T_front = nullptr;
			 T_back = nullptr;
			 T_reuse = new std::vector<int>;
			 T_currBuzzer = 0;
			 T_check_size = 10;
			 T_check = new Node*[10];
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
			// delete the queue
			Node* nextNode = T_front->next;
			Node* currNode = T_front;
			for (int i = 0; i < T_length; i++) 
			{
				delete currNode;
				currNode = nextNode;
				if (nextNode != nullptr) 
				{
					nextNode = nextNode->next;
				}
			}

			delete T_reuse;
			delete T_check;
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
             buzzers.clear();   // you don't know the history of the 
                                //   buzzers vector, so we had better
                                //   clear it first.
			 Node* currNode = T_front;
			 for (int i = 0; i < T_length; i++) 
			 {
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

             return T_length;   // placeholder

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
			 if (T_reuse->empty()) 
			 {
				 currNode->val = T_currBuzzer;
				 if (T_length == 0) 
				 {
					 T_front = T_back = currNode;
				 }
				 else 
				 {
					 T_back->next = currNode;
					 currNode->prev = T_back;
					 T_back = currNode;
				 }
				 if (T_currBuzzer == T_check_size) 
				 {
				     T_check = resize(T_check);
				 }
				 T_check[T_currBuzzer] = currNode;
				 T_currBuzzer++;
				 T_length++;
				 return T_currBuzzer - 1;
			 }
			 else 
			 {
				 currNode->val = T_reuse->back();
				 T_reuse->pop_back();
				 if (T_length == 0) 
				 {
					 T_front = T_back = currNode;
			     }
				 else 
				 {
					 T_back->next = currNode;
					 currNode->prev = T_back;
					 T_back = currNode;
				 }
				 T_length++;
				 T_check[currNode->val] = currNode;
				 return currNode->val;
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
		   if (T_length == 0) 
		   {
			   return -1;
		   }

		   // case: queue is not empty
		   Node* currNode = T_front;
		   T_front = T_front->next;
		   T_reuse->push_back(currNode->val);
		   T_check[currNode->val] = nullptr;
		   delete currNode;
		   T_length--;

           return T_reuse->back();
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
		   if (T_check[buzzer] == nullptr) 
		   {
			   return false;
		   }
		   else 
		   {
			   T_reuse->push_back(T_check[buzzer]->val);
			   Node* prevNode = T_check[buzzer]->prev;
			   Node* nextNode = T_check[buzzer]->next;
			   if (prevNode != nullptr) 
			   {
				   prevNode->next = nextNode;
			   }
			   else 
			   {
				   T_front = nextNode;
			   }
			   if (nextNode != nullptr) 
			   {
				   nextNode->prev = prevNode;
			   }
			   else 
			   {
				   T_back = prevNode;
			   }
			   Node* currNode = T_check[buzzer];
			   T_check[buzzer] = nullptr;
			   delete currNode;
			   T_length--;
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
		   if (T_check[buzzer] == nullptr) 
		   {
			   return false;
		   }
		   else 
		   {
			   Node* prevNode = T_check[buzzer]->prev;
			   Node* nextNode = T_check[buzzer]->next;
			   Node* currNode = T_check[buzzer];
			   if (prevNode != nullptr) 
			   {
				   prevNode->next = nextNode;
			   }
			   else 
			   {
				   // buzzer is already in front
				   return true;
			   }
			   if (nextNode != nullptr) 
			   {
				   nextNode->prev = prevNode;
			   }
			   else 
			   {
				   T_back = prevNode;
			   }
			   T_front->prev = currNode;
			   currNode->prev = nullptr;
			   currNode->next = T_front;
			   T_front = currNode;
			   return true;
		   }

        }



};   // end ServiceQueue class

#endif


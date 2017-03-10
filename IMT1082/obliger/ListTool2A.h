/* 
|  FILE        :   ListTool2A.h
|
|  AUTHOR      :   Frode Haug, NTNU
|
|  IMPORT      :      None
|  EXPORT      :      None
|
|  DESCRIPTION :   
|          Toolkit for handling of lists (LIFO, FIFO and NUMERIC/TEXT SORTED).
|          For more details:  see "LISTTOOL.HTM" (in norwegian).
|
|
+---------------------------------------------------------------------------+
|                      C H A N G E     H I S T O R Y
+----------+------------------------------------------+---------------------+
|   DATE   |  DESCRIPTION                             |  SIGN
+----------+------------------------------------------+---------------------+
|  020594  |  Initial.                                |  Frode Haug
|  270495  |  Virtual destructor in Element.          |  Frode Haug
|  170497  |  Extended if-test in remove(.....) (2x)  |  Frode Haug
|  040400  |  Some few small cosmetic corrections.    |  Frode Haug
|  220103  |  Updated for/to Standard C++.            |  Frode Haug
|  060116  |  A lot of name changes.                  |  Frode Haug
|          |                                          |
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/       

#if !defined(__LISTTOOL2A_H)           //  Ensures that this file will be
#define __LISTTOOL2A_H                 //    included only ONCE in each file.


			     //  INCLUDE:
#include <iostream>                    //  cout, cin
#include <cstring>                     //  strlen, strcmp

using namespace std;

			     //  ENUM:
enum listtype  { LIFO, FIFO, Sorted }; //  Allowed list-types.


			     //  CLASSES:
class NumElement;                      //  Predeclaration.
class TextElement;                     //  Predeclaration.

			     
class Element  {                       //  Base class 'Element'.
  private:
    char elementType;                  //  Type of element: 'B'(ase), 
				                       //    'N'(umeric) or 'T'(ext).
    friend class NumElement;           //  Only these two classes can 
    friend class TextElement;          //    manipulate 'elementType'.

  public:
    Element()        {  elementType = 'B';    }   //  Sets element type.
    virtual ~Element()  {     }                   //  Virtual, empty destr.
    char getType()  {  return  elementType;  }    //  Return element type.
    virtual int compare (Element* el) 
		     {  el = el; return 0; }   //  Dummy body.
    virtual void display()             //  Displayed only if function is NOT
				                       //   redefined insided derived classes.
		     {  cout << "\nLISTTOOL - WARNING:\n\t'Display()' not "
			         << "implemented inside derived class.";  }
};


class NumElement: public Element  {    //  Base class for all numeric 
  protected:                           //    sorted elements.
    int number;                        //  ID-number.

  public:
    NumElement()                       //  Needs and reads the ID-number.
      {  cout << "\nID-number:  ";  cin >> number;  cin.ignore();
         elementType = 'N';  }

    NumElement(int no)                 //  Created with an ID.
      {  number = no;  elementType = 'N';  }

    virtual int compare(Element* el)   //  Compares own number with  el's.
      {  NumElement* numEl = (NumElement*) el;
   	     int diff = number - numEl->number;
	     if (diff > 0)          return 1;
	     else if  (diff == 0)   return 0;
	     else                   return -1;  }
};


class TextElement: public Element  {   //  Base class for all text/string
  private:
	  void strcpy2(char* s, const char* t)    //  Selvlaget "strcpy"
	    {  while (*t)  *s++ = *t++;   *s = '\0';  }

  protected:                           //    sorted elements.
    char* text;                        //  Pointer to ID-text/string.

  public:
    TextElement()                      //  Needs and reads the ID-name/text.
      {  char id[80];   cout << "\nID-text:  ";   cin.getline(id, 80);
	     text = new char[strlen(id) + 1];  strcpy2(text, id);
	     elementType = 'T';  }

    TextElement(const char* t)  {      //  Created with an ID.
		text = new char[strlen(t) + 1]; strcpy2(text, t); elementType = 'T';
	}

    ~TextElement()                     //  Deletes the text ID.
      {  delete [] text;   }

    virtual int compare(Element* el)   // Compares own name/text with el's.
      {  TextElement* teEl = (TextElement*) el;
	     return  strcmp(text, teEl->text);   }
};



class List  {                          //  The class 'List'.
  private:
    struct Node  {                     //  Internal struct 'node'.
      Element*  liElem;                //  Element from the program that
				                       //    is stored inside THIS node.
      Node*     next;                  //  Nodes are linked together.
    };

    Node*     first;                   //  Pointer to first element/head.
    Node*     last;                    //  Pointer to tail. Only relevant
				                       //    for sorted lists.
    listtype  liType;                  //  'LIFO', 'FIFO' or 'Sorted'.
    int       noInList;                //  Number of elements in the list.

    Node* findPosition(Element* eptr); //  Internal function. Used by
					                   //    several other functions.
  public:
    List();                            //  Constructors.
    List(listtype li);
    ~List();                           //  Destructor.

		//  15 functions available for the user/program
	    //     (some of them are overloaded).
		//  For different type of lists are the following functions relevant:
			                     		//  LIFO/FIFO   Num Sort   Text Sort
    bool isEmpty();                     //      X          X           X 
    int noOfElements();                 //      X          X           X
    bool add(Element* el);              //      X          X           X 
    Element* remove();                  //      X               
    Element* remove(int no);            //                 X
    Element* remove(const char* t);     //                             X
    Element* removeNo(int n);           //                 X           X
    bool destroy();                     //      X   
    bool destroy(int no);               //                 X
    bool destroy(const char* t);        //                             X
    bool inList(int no);                //                 X
    bool inList(const char* t);         //                             X
    bool displayElement(int no);        //      X          X
    bool displayElement(const char* t); //                             X
    void displayList();                 //      X          X           X
}; 


//  DEFINITION OF MEMBERFUNCTIONS:
    //  Searches in a sorted list for the position for insertion of the element
    //  pointed to by 'eptr'. 
    //  RETURNS A POINTER TO THE NODE BEFORE THE RELEVANT POSITION.
List::Node* List::findPosition(Element* eptr)  {
  Node* current = first;    //  Starts at the head.
				            //  As long as the inserted element
				            //    is greater than the next one,
				            //    and the tail is not reached:
  while (eptr->compare(current->next->liElem) > 0  &&  current->next != last)
	current = current->next;       //  Update to next node.
  return current;                  //  Returns relevant pointer.
}


List::List()  {            //  List created without the type.
  cout << "\nLISTTOOL - ERROR:\n\tYou MUST give 'LIFO', "
       << "'FIFO' or 'Sorted' when creating a 'List'.";   
}


List::List(listtype li)  {  //  Legal list is created.
				            //  Variables are initialized.
  liType = li;   first = last = nullptr;   noInList = 0; 
}


List::~List()  {                       //  Destructor to delete the list:
  Node* noptr;
  if (first)  {                        //  If something in the list.
     do  {
       noptr = first;                  //  'noptr' points to the node that
				                       //     soon will be deleted.
       first = first->next;            //  Updates first to the next node.
       delete noptr->liElem;           //  Deletes the content of the node.
       delete noptr;                   //  Deletes the node.
      } while (first);                 //  Proceed until all nodes are removed
     first = last = nullptr;           //  Zeroes the pointers.
   }
}


bool List::isEmpty()  {               //  Returns if list is empty or not:
  return  (noInList == 0); 
}


int List::noOfElements()  {          //  Returns current number of elements
  return  noInList;                  //    in the list.
}

				       
bool List::add(Element* eptr)  {  //  Add "eptr" into the list:
  Node*    newNode;                    //  Pointer to new created node.
  Node*    position;                   //  Pointer to the node BEFORE the
				                       //    position for insertion.
  char litype;                         //  Relevant list type.
  bool added = false;                  //  Adding was successful or not.

  if (eptr)  {                         //  'eptr' REALLY points to something.
     switch (liType)  {
     case LIFO:  newNode = new Node;       //  Creates new list-node.
		 newNode->liElem = eptr;   //  Fills with data. 
		 newNode->next = first;    //  Adds to the beginning.
		 first = newNode;          //  Updates "first"
		 ++noInList;               //  Increments the number.
		 added = true;             //  Adding was successful.
		 break;

     case FIFO:  newNode = new Node;       //  Creates new list-node.
		 newNode->liElem = eptr;   //  Fills with data.
		 newNode->next = nullptr;  //  Initiates next-pointer.
		 if (last)                 //  If list NOT empty:
		    last->next = newNode;  //  Adds to the end. 
		 else                      //  List is empty:
		    first = newNode;       //  Adds as the first one.
		 last = newNode;           //  Updates "last".
		 ++noInList;               //  Increments the number.
		 added = true;             //  Adding was successful.
		 break;

     case Sorted: 
		 litype = eptr->getType();  // Gets the element type.
		 if (litype == 'N'  ||  litype == 'T')  {  // Legal type:
		    if (first == nullptr)  {   //  Empty list:
		       first = new Node;       //  Creates head
		       last = new Node;        //    and tail.
		       first->next = last;     //  Linking them together.
		       last->next = nullptr;   //  Terminating the list.
		       if (litype == 'N')  {   //  Numeric list:
					    //  Fill head/tail:
			  first->liElem = new NumElement(0);
			  last->liElem = new NumElement(0);
	        } else {             //  Text/string list:
					    //  Fill head/tail:
			  first->liElem = new TextElement("");
			  last->liElem = new TextElement("");
		    }
		 }
					    //  Element of same type as those
					    //    those in the list:
		    if (litype ==  first->liElem->getType()) { 
		       newNode = new Node; //  Creates new list-node.
		       newNode->liElem = eptr;  //  Fills with data.
					    //  Finds the position where
					    //    'eptr' will be inserted:
		       position = findPosition(eptr);
					    //  Inserts 'eptr':
		       newNode->next = position->next;
		       position->next = newNode;
		       ++noInList;          //  Increments the number.
		       added = true;        //  Adding was succesful.
		    } else                  //  Element of another type
					                //    than those in the list:
		      cout << "\nLISTTOOL - WARNING:\n\tTrying "
			       << "to add element of wrong type "
			       << "into a sorted list.";
		 }  else                    //  Illegal element type:
		    cout << "\nLISTTOOL - ERROR:\n\tElements in "
			     << "Sorted lists MUST be derived\n\tfrom "
			     << "'NumElement' or 'TextElement'.";
		    break;

     default: cout << "\nLISTTOOL - ERROR:\n\tThis message (1) "
		           << "should never occur !";  break;
     }
  }  else                            //  Function was called with no element:
     cout << "\nLISTTOOL - WARNING:\n\t'Add()' called with no element.";
  return added;                      //  Returning with status of adding.
}

				               //  Returns (if possible) the FIRST 
Element* List::remove()  {     //    element in a LIFO-/FIFO-list:
  Element* eptr = nullptr;              //  Pointer to the removed element. 
  Node*  removeNode;                    //  Node that will be removed.

  if (liType == LIFO  ||  liType == FIFO)  {   // Legal list type:
     if (!isEmpty())  {                 //  Something in the list:
	    eptr = first->liElem;           //  Gets relevant element.
	    removeNode = first;             //  Pointer to soon removed node.
	    first = first->next;            //  Updates the first-pointer.
        if (first == nullptr) last = nullptr; //  The list are now empty.
           delete removeNode;           //  Deletes the first node.
        --noInList;                     //  Decrements the number.
     }
  } else                                //  Illegal use of the function:
    cout << "\nLISTTOOL - WARNING:\n\t'Remove()' shall "
	     << "only be called for LIFO or FIFO lists.";
  return eptr;                          //  Returns element or nullptr.
}

				                  //  Returns (if possible) element with
Element* List::remove(int no)  {  //   ID='no' in a numeric sorted list:
	Element* eptr = nullptr;           //  Pointer to the removed element.
  NumElement* nuptr;                   //  Temporary element with equal ID.
  Node*  removeNode;                   //  Node that will be removed.
  Node*  position;                     //  Pointer to the node BEFORE the
				                       //    possible relevant one. 
  if (first)  {                        //  At least head and tail:
				                       //  Legal list type:
     if (liType == Sorted  &&  first->liElem->getType() == 'N')  {      
	    if (!isEmpty())  {             //  Something in the list:
	       nuptr = new NumElement(no); //  Creates temporary element with
				                       //     equal ID to the searhed one.
	       position = findPosition(nuptr);  //  Finds the position BEFORE
					                   //  the possible node/element.
				                       // The next is the really searched one:
	       if (nuptr->compare(position->next->liElem) == 0)  {
	           removeNode = position->next; //Pointer to the soon removed one.
	           eptr = removeNode->liElem;   // Gets the element.
	           position->next = removeNode->next; // Skips node from the list.
	           delete removeNode;         // Deletes the node.
	           --noInList;                // Decrements the number in the list.
	       }
	      delete nuptr;                   // Deletes the temporary element.
	    }
     } else                               // Illegal use of the function:
       cout << "\nLISTTOOL - WARNING:\n\t'Remove(number)' shall "
	        << "only be called for NUMERIC SORTED lists.";
  }
  return eptr;                            //  Returns element or nullptr.   
}

				                       //  Returns (if possible) element with
Element* List::remove(const char* t) { //    ID='t' in a text sorted list:
  Element* eptr = nullptr;             //  Pointer to the removed element.  
  TextElement* teptr;                  //  Temporary element with equal ID.  
  Node*  removeNode;                   //  Node that will be removed.
  Node*  position;                     //  Pointer to the node BEFORE the
				                       //    possible relevant one.
  if (first)  {                        //  At least head and tail:
				                       //  Legal list type:
     if (liType == Sorted  &&  first->liElem->getType() == 'T')  {
	    if (!isEmpty())  {               //  Something in the list:
	       teptr = new TextElement(t);   // Creates temporary element with
				                         //   equal ID to the searhed one.
	       position = findPosition(teptr);  //  Finds the position BEFORE
			                                //  the possible node/element.
				                       // The next is the really searched one:
	       if (teptr->compare(position->next->liElem) == 0)  {
	          removeNode = position->next; //Pointer to the soon removed one.
	          eptr = removeNode->liElem;  // Gets the element.
	          position->next = removeNode->next; // Skips node from the list.
	          delete removeNode;        //  Deletes the node.
	          --noInList;               //  Decrements the number in the list.
	       }
	       delete teptr;                //  Deletes the temporary element.
	    }
     } else                             //  Illegal use of the function:
       cout << "\nLISTTOOL - WARNING:\n\t'Remove(text)' shall "
	        << "only be called for TEXT/STRING SORTED lists.";
  }
  return eptr;                          //  Returns element or nullptr.   
}

				                   //  Returns (if possible) element no.
Element* List::removeNo(int n)  {  //    'n' in a num/text sorted list:
  Element* eptr = nullptr;            //  Pointer to the removed element.
  Node*  removeNode;                  //  Node that will be removed.
  Node*  position;                    //  Pointer to the node BEFORE the
				                      //    possible relevant one.
  if (liType == Sorted)  {            //  Legal list type:
     if (!isEmpty())  {               //  Something in the list:
	    if (noInList >= n)  {         //  Number 'n' exsists:
	       position = first;          //  Start search at the head.
				       //  Loop to the node BEFORE. 
	       for (int i = 1;  i < n;  i++, position = position->next)   
 	            ;                        //  NOTE: Empty body !
	       removeNode = position->next;  // Pointer to the soon removed one.
	       eptr = removeNode->liElem;    // Gets the element.
	       position->next = removeNode->next; //Skips the node from the list.
	       delete removeNode;            //  Deletes the node.
	       --noInList;                   //  Decrements the number in the list.
	    }
     }
  } else                               //  Illegal use of the function:
    cout << "\nLISTTOOL - WARNING:\n\t'RemoveNo(n)' "
	     << "shall only be called for SORTED lists.";
  return eptr;                         //  Returns element or nullptr.
}

				       
				                  //  Destroys (if possible) the FIRST 
bool List::destroy()  {           //    element in a LIFO-/FIFO-list:
  Element* eptr;                       //  Pointer to the destroyed element.
  bool destroyed = false;              //  Destroying was successful or not.

  if (liType == LIFO  ||  liType == FIFO)  {   //  Legal list type:
     if (!isEmpty())  {                //  Something in the list:
	    eptr = remove();               //  Gets first element.
	    delete eptr;                   //  Delete element.
	    destroyed = true;              //  Destroying was successful.
     }
  } else                               //  Illegal use of function.
    cout << "\nLISTTOOL - WARNING:\n\t'Destroy()' shall "
	     << "only be called for LIFO or FIFO lists.";
  return destroyed;                    //  Return with status of destroying.
}

				       //  Destroys (if possible) the element with ID='no'
                       //    from a numeric sorted list:
bool List::destroy(int no)  { 
  Element* eptr;                       //  Pointer to the destroyed element.
  bool destroyed = false;              //  Destroying was successful or not.

  if (first)  {                        //  At least head and tail:
				                       //  Legal list type:
     if (liType == Sorted  &&  first->liElem->getType() == 'N')  {
     	if (!isEmpty())  {            //  Something in the list:
	       eptr = remove(no);          //  Get (if possible) relevant element.
	       if (eptr)  {                //  Element found:
	          delete eptr;             //  Delete element. 
	          destroyed = true;        //  Destroying was successful.
	       }
	    }
     } else                            //  Illegal use of function.
       cout << "\nLISTTOOL - WARNING:\n\t'Destroy(no)' shall "
	        << "only be called for NUMERIC SORTED lists.";
  }
  return destroyed;                    //  Return with status of destroying.
}
				       //  Destroys (if possible) the element with ID='t'
                       //    from a text/string sorted list:
bool List::destroy(const char* t)  {
  Element* eptr;                       //  Pointer to the destroyed element.
  bool destroyed = false;              //  Destroying was successful or not.

  if (first)  {                        //  At least head and tail:
				                       //  Legal list type:
     if (liType == Sorted  &&  first->liElem->getType() == 'T')  {
	    if (!isEmpty())  {             //  Something in the list:
	       eptr = remove(t);           //  Get (if possible) relevant element.
	       if (eptr)  {                //  Element found:
	          delete eptr;             //  Delete element. 
	          destroyed = true;        //  Destroying was successful.
	       }
	    }
     } else                            //  Illegal use of function.
       cout << "\nLISTTOOL - WARNING:\n\t'Destroy(text)' shall "
	        << "only be called for TEXT/STRING SORTED lists.";
  }
  return destroyed;                    //  Return with status of destroying.
}

				              //  Looks for element with ID='no'
bool List::inList(int no)  {  //    in a numeric sorted list:
  NumElement* neptr;               //  Temporary element with equal ID.
  Node* position;                  //  Pointer to node BEFORE the possible one.
  bool isThere = false;            //  Element found or not. 

  if (first)  {                    //  At least head and tail:
				                   //  Legal list type: 
     if (liType == Sorted  &&  first->liElem->getType() == 'N')  {
    	if (!isEmpty())  {            //  Something in the list:
	       neptr = new NumElement(no);   //  Creates temporary element with
					                     //    equal ID to the searhed one.
	       position = findPosition(neptr);   //  Finds the position BEFORE
					                         //  the possible node/element.
					                 // The next is the really searched one:
	       if (neptr->compare(position->next->liElem) == 0)
	          isThere = true;            //  Searching was successful. 
	       delete neptr;                 //  Deletes the temporary element.
	   }
     } else                              //  Illegal use of the function. 
       cout << "\nLISTTOOL - WARNING:\n\t'InList(no)' shall "
	        << "only be called for NUMERIC SORTED lists.";
  }
  return isThere;                        //  Return the status of the search.
}


				                    //  Looks for element with ID='t'
bool List::inList(const char* t)  { //    in a text/string sorted list:
  TextElement* teptr;              //  Temporary element with equal ID.
  Node* position;                  //  Pointer to node BEFORE the possible one.
  bool isThere = false;            //  Element found or not. 

  if (first)  {                        //  At least head and tail:
				                       //  Legal list type: 
     if (liType == Sorted  &&  first->liElem->getType() == 'T')  {
	    if (!isEmpty())  {             //  Something in the list:
	       teptr = new TextElement(t); //  Creates temporary element with
					                   //    equal ID to the searhed one.
	       position = findPosition(teptr);   //  Finds the position BEFORE
					                         //  the possible node/element.
					                   // The next is the really searched one:
	       if (teptr->compare(position->next->liElem) == 0)
	          isThere = true;            //  Searching was successful. 
	       delete teptr;                 //  Deletes the temporary element.
	    }
     } else                              //  Illegal use of the function. 
       cout << "\nLISTTOOL - WARNING:\n\t'InList(text)' shall "
	        << "only be called for TEXT/STRING SORTED lists.";
  }
  return isThere;                     //  Return the status of the search.
}

				   //  Displays element no.'no' in a LIFO-or FIFO-list, OR 
				   //    displays elementwith ID='no' in a numeric sorted list:
bool List::displayElement(int no)   {   
  NumElement* nuptr;                    //  Temporary element with equal ID.
  Node*  current;                       //  Pointer to relevant node.
  bool displayed = false;               //  Element displayed or not.  

  if (!isEmpty())  {                    //  Something in the list.
     if (liType == LIFO  ||  liType == FIFO)  {    //  Case: LIFO/FIFO:
    	if (noInList >= no)  {          //  Element no.'no' IS there:
	       current = first;             //  Start at the first node.
				                        //  Find relevant node:
	       for (int i = 1;  i < no;  i++, current = current->next)
	            ;                       //  NOTE:  Empty body !
	       current->liElem->display();  //  Display the element. 
	       displayed = true;            //  Displayment was successful.
	    }                               //  Case:  NUMERIC SORTED list:
     } else if (liType == Sorted  &&  first->liElem->getType() == 'N')  {
	     nuptr = new NumElement(no);    // Creates temporary element with
					                    //   equal ID to the searhed one.
	     current = findPosition(nuptr); // Finds the position BEFORE
					                    //   the possible node/element.
	     current = current->next;       //  Update to relevant one.
				                   // The node is the really searched one:
	     if (nuptr->compare(current->liElem) == 0)  {
	        current->liElem->display(); //  Display the element.
	        displayed = true;           //  Displayment was successful.
	     }
	     delete nuptr;                  //  Delete temporary element.
     } else                             //  Illegal use of the function:
       cout << "\nLISTTOOL - WARNING:\n\t'DisplayElement(no)' shall only "
	        << "be called for\n\t LIFO-, FIFO- and NUMERIC SORTED lists.";
  }
  return displayed;                     //  Return status of the displayment.
}

				                           //  Displays element with ID='t' in
bool List::displayElement(const char* t) { //  a TEXT/STRING SORTED list:
  TextElement* teptr;                      //  Temporary element with equal ID.
  Node*  current;                          //  Pointer to relevant node.
  bool displayed = false;                  //  Element displayed or not.  

  if (first)  {                            //  At least head and tail:
				                           //  Legal list type:
     if (liType == Sorted  &&  first->liElem->getType() == 'T')  {
     	if (!isEmpty())  {                 //  Something in the list:
	       teptr = new TextElement(t);     // Creates temporary element with
				                           //    equal ID to the searhed one.
	       current = findPosition(teptr);  //  Finds the position BEFORE
				                           //  the possible node/element.
	       current = current->next;        //  Update to relevant one.
				                      // The node is the really searched one:
	       if (teptr->compare(current->liElem) == 0)  {
	          current->liElem->display();  //  Display the element.
	          displayed = true;            //  Displayment was successful.
	       }
	       delete teptr;                   //  Delete temporary element.
	    }
     } else                                //  Illegal use of the function:
       cout << "\nLISTTOOL - WARNING:\n\t'DisplayElement(text)' "
	        << "shall only be called for TEXT SORTED lists.";
  }
  return displayed;                       //  Return status of the displayment.
}

				               //  Displays the WHOLE content of a
void List::displayList()  {    //    arbitrary list:
  Node* current = first;               //  Pointer to current node.

  if (!isEmpty())  {                   //  Something in the list:
     if (liType == Sorted)             //  If sorted:
	    current = current->next;       //    skip the head.
				                       //  For all nodes/elements:
     for (int i = 1;  i <= noInList;  i++, current = current->next)
	     current->liElem->display();   //  Display element.
  }                                    //  Display number of elements:
  //cout << "\n\tNumber of elements in the list:  " << noInList;
}

#endif

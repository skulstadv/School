#include <iostream>
#include "ListTool2A.h"

using namespace std;

// Maximum stringlength
const int STRLEN = 80;

class Prescription;
class Patient;
class Doctor;

void help();
void readLine(char** src, char* msg);
int readNum(int min, int max, char* msg);
Doctor* findDoctor(char* name);

class Prescription : public Element {
protected:
	// Date prescribed
	int date;
	// Name of drug
	char * name;
	// Amount in milligrams prescribed
	int amount;
public:
	Prescription() {
		date = readNum(1, 10, "Write date prescribed: ");
		readLine(&name, "Write prescription name: ");
		amount = readNum(1, 10000, "Write amount prescribed in milligrams: ");
	}

	~Prescription() {
		delete[] name;
	}

	// Returns true if the date of the prescription is ealier or same as @date parameter
	bool dateBefore(int date) {
		if (date > this->date)
			return true;
		return false;
	}

	int getDate() {
		return date;
	}

	void display() {
		cout << "\t\tPrescription name: " << name << "\n\t\tAmount: " << amount <<
			"\n\t\tDate prescribed: " << date << endl << endl;
	}
};


class Patient :public TextElement {
private:
	List* prescriptions;
public:
	Patient(char* name): TextElement(name) {
		prescriptions = new List(FIFO);
	}

	~Patient() {
		// Removes all elements from prescriptions list and invokes the destructor
		while (prescriptions->removeNo(1) != nullptr) continue;
	}

	Prescription * addPrescription() {
		Prescription * pres = new Prescription();
		prescriptions->add(pres);
		return pres;
	}

	void removeOld(int date) {
		Prescription* p;
		int sum = prescriptions->noOfElements();
		for (int i = 0; i< sum; i++) {
			p = (Prescription*)prescriptions->remove();
			// Only put those after @date back in list
			if (!p->dateBefore(date))
				prescriptions->add(p);
			else
				cout << "Removed old prescription with date: " << p->getDate() << endl;
		}
	}

	void display() {
		cout << "\tPatient name: " << text << "\n\tPrescriptions: " << endl;
		prescriptions->displayList();
	}
};


class Doctor : public TextElement {
protected:
	// Name can be found as text
	char * adr;
	// Telephone number of doctor
	int tel;
	// All patients of this doctor
	List* patients;
public:
	Doctor(char* name) : TextElement(name) {
		readLine(&adr, "Write address of doctor: ");
		tel = readNum(1, 9, "Write telephone number ");
		patients = new List(Sorted);
	}

	~Doctor() {
		// Removes all elements from patients list and invokes the destructor
		while (patients->removeNo(1) != nullptr) continue;
	}

	// Adds prescription to existing patient if it exists or creates a new one
	Patient * addPatient() {
		char* name;
		readLine(&name, "Write patient name: ");
		Patient * pat = (Patient*)patients->remove(name);
		if (pat != nullptr) {
			cout << "Adding presription to existing patient." << endl;
			patients->add(pat);
			return pat;
		}
		cout << "Adding as new patient." << endl;
		pat = new Patient(name);
		patients->add(pat);
		return pat;
	}

	void deleteOld(int date) {
		int items = patients->noOfElements();
		// Something wrong happens when it goes through more than one patient and the program crashes
		Patient* p;
		for (int i = 1; i <= items; i++) {
			p = (Patient*)patients->removeNo(i);
			p->removeOld(date);
			patients->add(p);
		}
	}

	char * getName() {
		return text;
	}

	void displayPatient(char* name) {
		Patient* pat = (Patient*)patients->remove(name);
		if (pat != nullptr)
			pat->display();
	}

	void display() {
		cout << "Doctor name: " << text << "\nAddress: " << adr << "\nTelephone: " << tel << endl;
		patients->displayList();
	}
};


// Global variables
List* doctors;

int main() {
	doctors = new List(Sorted);
	char* input;
	do {
		help();
		readLine(&input, "Make your choice: ");
		*input = toupper(*input);
		switch (*input) {
		case 'R': {
			char* name;
			readLine(&name, "Write name of doctor: ");
			Doctor* doc = (Doctor*)doctors->remove(name);
			if (doc != nullptr){
					cout << "Doctor already registered in list, adding to doctor" << endl;
					doctors->add(doc);
				}
			// We didn't find a doctor with the name and will now create a new one
			else {
				doc = new Doctor(name);
				doctors->add(doc);
			}
			// doc is now either a new doctor or an existing one with matching name
			Patient* pat = doc->addPatient();
			Prescription* pres = pat->addPrescription();
			break;
		}
		case 'L': {
			char* name;
			readLine(&name, "Write name of doctor: ");
			Doctor* doc = (Doctor*)doctors->remove(name);
			if (doc != nullptr) {
				doc->display();
				doctors->add(doc);
			}
			else
				cout << "Could not find specified doctor!" << endl;
			break;
		}
		case 'P': {
			char* name;
			readLine(&name, "Write name of patient: ");
			int elements = doctors->noOfElements();
			for (int i = 1; i <= elements; i++) {
				Doctor* doc = (Doctor*)doctors->removeNo(i);
				doc->displayPatient(name);
				doctors->add(doc);
			}
			break;
		}
		case 'F': {
			int date = readNum(1, 10, "Remove prescriptions for dates earlier than: ");
			int elements = doctors->noOfElements();
			for (int i = 1; i <= elements; i++) {
				Doctor* doc = (Doctor*)doctors->removeNo(i);
				doc->deleteOld(date);
				doctors->add(doc);
			}
			break;
		}
		case 'U': {
			break;
		}
		case 'I': {
			break;
		}
		case 'H': {
			break;
		}
		case 'Q': {
			break;
		}
		default: {
			cout << "Valid input: [R|L|P|F|U|I|H|Q]\n";
			break;
		}
		}
	} while (*input != 'Q');
}

// Displays the help message with available commands
void help() {
	cout << "\n\n\nChoose action: \n\tR New prescription\n\tL Overview of presc" <<
		"riptions\n\tP Overview of specific patient\n\tF Remove all o" <<
		"ld prescriptions\n\tU Write to file\n\tI Read from file\n\tH" <<
		" This help message\n\tQ Exit the program\n";
}

// Reads the next line after printing msg
void readLine(char** src, char* msg) {
	cout << msg;
	char buff[STRLEN];
	cin.getline(buff, STRLEN);
	*src = new char[strlen(buff) + 1];
	strcpy_s(*src, strlen(buff) + 1, buff);
}

// Reads a number in the interval [min, max]
int readNum(int min, int max, char* msg) {
	int input;
	cout << msg << min << "-" << max << ":";
	cin >> input;
	while (input < min || input > max) {
		cout << "Invalid number. Try again: ";
		cin >> input;
	}
	cin.ignore();
	return input;
}

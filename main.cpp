#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

const int MAX_FLIGHTS = 10;
const int MAX_PASSENGERS = 100;
const int MAX_USERS = 50;
const int MAX_BOOKINGS_PER_USER = 10;
// Flight Information
struct Flight {
    string flightNumber;
    string destination;
    int totalSeats;
    int bookedSeats;
};

// Passenger Information
struct Passenger {
    string fname;
    string lname;
    string flightNumber;
    int age;
    string gender;
    string contactNumber;
};



Flight flights[MAX_FLIGHTS];
Passenger passengers[MAX_PASSENGERS];
int flightCount = 0;
int passengerCount = 0;

void loadFlightsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line) && flightCount < MAX_FLIGHTS) {
        istringstream iss(line);
        Flight flight;
        if (!(iss >> flight.flightNumber >> flight.destination >> flight.totalSeats)) {
            cout << "Error: Invalid format in file" << endl;
            continue;
        }
        flight.bookedSeats = 0;
        flights[flightCount++] = flight;
    }
    file.close();
}

void savePassengersToFile(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file " << filename << endl;
        return;
    }

    for (int i = 0; i < passengerCount; ++i) {
        file << passengers[i].fname << " "
            << passengers[i].lname << " "
             << passengers[i].flightNumber << " "
             << passengers[i].age << " "
             << passengers[i].gender << " "
             << passengers[i].contactNumber << endl;
    }
    file.close();
}

void displayAvailableFlights() {
    for (int i = 0; i < flightCount; ++i) {
        cout << "Flight Number: " << flights[i].flightNumber
                  << ", Destination: " << flights[i].destination
                  << ", Available Seats: " << flights[i].totalSeats - flights[i].bookedSeats << endl;
    }
}

void bookSeat() {
    if (passengerCount >= MAX_PASSENGERS) {
        cout << "Cannot add more passengers, maximum limit reached." << endl;
        return;
    }

    string flightNumber;
    cout << "Enter flight number to book: ";
    cin >> flightNumber;

    bool flightFound = false;
    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            flightFound = true;
            if (flights[i].bookedSeats < flights[i].totalSeats) {
                cout << "Enter passenger first name: ";
                cin >> passengers[passengerCount].fname;
                cout << "Enter passenger last name: ";
                cin >> passengers[passengerCount].lname;
                cout << "Enter passenger age: ";
                cin >> passengers[passengerCount].age;
                cout << "Enter passenger gender: ";
                cin >> passengers[passengerCount].gender;
                cout << "Enter passenger phone number: ";
                cin >> passengers[passengerCount].contactNumber;
                passengers[passengerCount].flightNumber = flightNumber;

                flights[i].bookedSeats++;
                passengerCount++;
                cout << "Booking successful for " << passengers[passengerCount - 1].fname << endl;
                savePassengersToFile("passengers.txt");
            } else {
                cout << "No available seats on flight " << flightNumber << endl;
            }
            break;
        }
    }

    if (!flightFound) {
        cout << "Flight not found." << endl;
    }
}

void displayPassengers() {
    for (int i = 0; i < passengerCount; ++i) {
        cout << "Passenger Name: " << passengers[i].fname
                  << ", Passenger last Name: " << passengers[i].lname
                  << ", Flight Number: " << passengers[i].flightNumber
                  << ", Age: " << passengers[i].age
                  << ", Gender: " << passengers[i].gender
                  << ", phone Number: " << passengers[i].contactNumber << endl;
    }
}

void loadPassengersFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line) && passengerCount < MAX_PASSENGERS) {
        istringstream iss(line);
        Passenger passenger;
        if (!(iss >> passenger.fname >>passenger.lname >> passenger.flightNumber >> passenger.age >> passenger.gender >> passenger.contactNumber)) {
            cout << "Error: Invalid format in file" << endl;
            continue;
        }
        passengers[passengerCount++] = passenger;
    }
    file.close();
}

void modifyBooking() {
    string name;
    cout << "Enter the name of the passenger to modify: ";
    cin.ignore(10000, '\n');  // Ignore any leftover newline from previous input
    getline(cin, name);


    bool passengerFound = false;
    for (int i = 0; i < passengerCount; ++i) {
        if (passengers[i].fname == name) {
            passengerFound = true;
            cout << "Enter new flight number: ";
            cin >> passengers[i].flightNumber;
            cout << "Enter new age: ";
            cin >> passengers[i].age;
            cout << "Enter new gender: ";
            cin >> passengers[i].gender;
            cout << "Enter new phone number: ";
            cin >> passengers[i].contactNumber;

            cout << "Modification successful for " << passengers[i].fname << endl;
            savePassengersToFile("passengers.txt");
            break;
        }
    }

    if (!passengerFound) {
        cout << "Passenger not found." << endl;
    }
}

void cancelBooking() {
    string name;
    cout << "Enter the name of the passenger to cancel: ";
    cin.ignore(10000, '\n');  // Ignore any leftover newline from previous input
    getline(cin, name);  // Use getline to read the full name


    bool passengerFound = false;
    for (int i = 0; i < passengerCount; ++i) {
        if (passengers[i].fname == name) {
            passengerFound = true;
            string flightNumber = passengers[i].flightNumber;

            for (int j = i; j < passengerCount - 1; ++j) {
                passengers[j] = passengers[j + 1];
            }
            passengerCount--;

            for (int j = 0; j < flightCount; ++j) {
                if (flights[j].flightNumber == flightNumber) {
                    flights[j].bookedSeats--;
                    break;
                }
            }

            cout << "Cancellation successful for " << name << endl;
            savePassengersToFile("passengers.txt");
            break;
        }
    }

    if (!passengerFound) {
        cout << "Passenger not found." << endl;
    }
}

int main() {
    loadFlightsFromFile("flights.txt");
    loadPassengersFromFile("passengers.txt");

    int choice;
    do {
        cout << "\nFlight Reservation System\n";
        cout << "1. Display Available Flights\n";
        cout << "2. Book Seat\n";
        cout << "3. Display Passengers\n";
        cout << "4. Modify Booking\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayAvailableFlights();
                break;
            case 2:
                bookSeat();
                break;
            case 3:
                displayPassengers();
                break;
            case 4:
                modifyBooking();
                break;
            case 5:
                cancelBooking();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}

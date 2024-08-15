#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

//Room class
class Room {
public:
    int number;
    std::string type;
    double price;
    bool available;

    //Constructor to initialize the room 

    Room(int num, const std::string& tp, double pr) : number(num), type(tp), price(pr), available(true) {}
};

//Customer class
class Customer {
public:
    std::string name;
    std::string checkIn;
    std::string checkOut;

    //Constructor to initialize the customer
    Customer(const std::string& nm, const std::string& in, const std::string& out) : name(nm), checkIn(in), checkOut(out) {}
};

//Booking class
class Booking {
public:
    Room* room;
    Customer* customer;

    //Constructor to initialize the booking
    Booking(Room* rm, Customer* cust) : room(rm), customer(cust) {}
};

//Billing class
class Billing {
private:
    std::map<std::string, double> bills;

public:
    //Method to add a bill
    void addBill(const std::string& name, double amount) {
        if (bills.find(name) != bills.end()) {
            bills[name] += amount;
        } else {
            bills[name] = amount;
        }
    }
    //Method for showing all bills
    void displayBills() {
        for (std::map<std::string, double>::const_iterator it = bills.begin(); it != bills.end(); ++it) {
            std::cout << "Customer: " << it->first << ", Amount: $" << it->second << std::endl;
        }
    }
};

//Hotel class 
class Hotel {
private:
    std::vector<Room> rooms;
    std::vector<Booking> bookings;
    Billing billing;

//Method to initialize the system from the text file
public:
    void initialize(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return;
        }

        int num;
        std::string tp;
        double pr;

        while (inFile >> num >> tp >> pr) {
            rooms.push_back(Room(num, tp, pr));
        }

        inFile.close();
        std::cout << "System initialized from " << filename << std::endl;
    }

    void display() {
        for (std::vector<Room>::const_iterator it = rooms.begin(); it != rooms.end(); ++it) {
            std::cout << "Room Number: " << it->number << ", Type: " << it->type << ", Price: $" << it->price << ", Available: " << (it->available ? "Yes" : "No") << std::endl;
        }
    }

    //Method for checkin
    void checkIn(const std::string& name, const std::string& in, const std::string& out) {
        for (std::vector<Room>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
            if (it->available) {
                it->available = false;
                bookings.push_back(Booking(&(*it), new Customer(name, in, out)));
                billing.addBill(name, it->price);
                std::cout << "Room booked for " << name << std::endl;
                return;
            }
        }
        std::cout << "No available rooms for booking" << std::endl;
    }

    //Method for checkout
    void checkOut(const std::string& name) {
        for (std::vector<Booking>::iterator it = bookings.begin(); it != bookings.end(); ++it) {
            if (it->customer->name == name) {
                it->room->available = true;
                bookings.erase(it);
                std::cout << name << " has checked out" << std::endl;
                return;
            }
        }
        std::cout << "No booking found for " << name << std::endl;
    }


    //Method to show available rooms
    void viewAvailable() {
        for (std::vector<Room>::const_iterator it = rooms.begin(); it != rooms.end(); ++it) {
            if (it->available) {
                std::cout << "Room Number: " << it->number << ", Type: " << it->type << ", Price: $" << it->price << std::endl;
            }
        }
    }

    //Method to show booked rooms
    void viewBooked() {
        for (std::vector<Booking>::const_iterator it = bookings.begin(); it != bookings.end(); ++it) {
            std::cout << "Room Number: " << it->room->number << ", Customer: " << it->customer->name << ", Check-in: " << it->customer->checkIn << ", Check-out: " << it->customer->checkOut << std::endl;
        }
    }

    //Method to show billings
    void viewBilling() {
        billing.displayBills();
    }

    //Method to save the current state of the hotel
    void save(const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Unable to open file " << filename << std::endl;
            return;
        }

        for (std::vector<Room>::const_iterator it = rooms.begin(); it != rooms.end(); ++it) {
            outFile << it->number << " " << it->type << " " << it->price << " " << it->available << std::endl;
        }

        outFile.close();
        std::cout << "Current state saved to " << filename << std::endl;
    }
};

//Main function for the menu of the program
int main() {
    Hotel hotel;
    int choice;
    std::string name, checkIn, checkOut;

    while (true) {
        std::cout << "\nHotel Management System\n";
        std::cout << "1. Initialize System\n";
        std::cout << "2. Display Hotel Description\n";
        std::cout << "3. Check-in Customer\n";
        std::cout << "4. Check-out Customer\n";
        std::cout << "5. View Available Rooms\n";
        std::cout << "6. View Booked Rooms\n";
        std::cout << "7. View Billing System\n";
        std::cout << "8. Save Current State\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            hotel.initialize("initial_data.txt");
            break;
        case 2:
            hotel.display();
            break;
        case 3:
            std::cout << "Enter customer name: ";
            std::cin.ignore(); // Clear the input buffer
            std::getline(std::cin, name);
            std::cout << "Enter check-in date (YYYY-MM-DD): ";
            std::cin >> checkIn;
            std::cout << "Enter check-out date (YYYY-MM-DD): ";
            std::cin >> checkOut;
            hotel.checkIn(name, checkIn, checkOut);
            break;
        case 4:
            std::cout << "Enter customer name: ";
            std::cin.ignore(); // Clear the input buffer
            std::getline(std::cin, name);
            hotel.checkOut(name);
            break;
        case 5:
            hotel.viewAvailable();
            break;
        case 6:
            hotel.viewBooked();
            break;
        case 7:
            hotel.viewBilling();
            break;
        case 8:
            hotel.save("current_state.txt");
            break;
        case 9:
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}

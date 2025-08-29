#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// Structure to store customer information
struct Customer {
    int id;
    string name;
    string address;
    string phone;
    double rate; // per liter rate
};

// Structure to store daily milk entry
struct MilkEntry {
    int customerId;
    string date;
    double morningQty;
    double eveningQty;
    double totalQty;
    double amount;
};

// Function prototypes
void displayMenu();
void addCustomer();
void viewCustomers();
void updateCustomer();
void deleteCustomer();
void addMilkEntry();
void viewDailyEntries();
void generateBill();
void searchEntries();
void saveDataToFile();
void loadDataFromFile();
string getCurrentDate();
double calculateAmount(double quantity, double rate);

// Global vectors to store data
vector<Customer> customers;
vector<MilkEntry> milkEntries;

int main() {
    loadDataFromFile();
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                viewCustomers();
                break;
            case 3:
                updateCustomer();
                break;
            case 4:
                deleteCustomer();
                break;
            case 5:
                addMilkEntry();
                break;
            case 6:
                viewDailyEntries();
                break;
            case 7:
                generateBill();
                break;
            case 8:
                searchEntries();
                break;
            case 9:
                saveDataToFile();
                cout << "Data saved successfully. Exiting...\n";
                break;
            case 10:
                cout << "Exiting without saving...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nPress Enter to continue...";
        cin.get();
        
    } while(choice != 9 && choice != 10);
    
    return 0;
}

void displayMenu() {
    system("cls"); // Clear screen (Windows specific)
    cout << "====================================\n";
    cout << "     MILK DAIRY MANAGEMENT SYSTEM   \n";
    cout << "====================================\n";
    cout << "1. Add New Customer\n";
    cout << "2. View All Customers\n";
    cout << "3. Update Customer Information\n";
    cout << "4. Delete Customer\n";
    cout << "5. Add Milk Entry\n";
    cout << "6. View Daily Entries\n";
    cout << "7. Generate Bill\n";
    cout << "8. Search Entries\n";
    cout << "9. Save and Exit\n";
    cout << "10. Exit Without Saving\n";
    cout << "====================================\n";
}

void addCustomer() {
    Customer newCustomer;
    
    cout << "\n--- Add New Customer ---\n";
    
    // Generate customer ID
    if (customers.empty()) {
        newCustomer.id = 1;
    } else {
        newCustomer.id = customers.back().id + 1;
    }
    
    cout << "Customer ID: " << newCustomer.id << endl;
    
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, newCustomer.name);
    
    cout << "Enter Address: ";
    getline(cin, newCustomer.address);
    
    cout << "Enter Phone: ";
    getline(cin, newCustomer.phone);
    
    cout << "Enter Rate per liter: ";
    cin >> newCustomer.rate;
    
    customers.push_back(newCustomer);
    
    cout << "\nCustomer added successfully!\n";
}

void viewCustomers() {
    if (customers.empty()) {
        cout << "\nNo customers found!\n";
        return;
    }
    
    cout << "\n--- Customer List ---\n";
    cout << "----------------------------------------------------------------------------\n";
    cout << left << setw(8) << "ID" << setw(25) << "Name" << setw(30) << "Address" 
         << setw(15) << "Phone" << setw(10) << "Rate/L" << endl;
    cout << "----------------------------------------------------------------------------\n";
    
    for (const auto& customer : customers) {
        cout << left << setw(8) << customer.id << setw(25) << customer.name 
             << setw(30) << customer.address << setw(15) << customer.phone 
             << setw(10) << fixed << setprecision(2) << customer.rate << endl;
    }
    cout << "----------------------------------------------------------------------------\n";
}

void updateCustomer() {
    int id;
    bool found = false;
    
    cout << "\n--- Update Customer ---\n";
    cout << "Enter Customer ID to update: ";
    cin >> id;
    
    for (auto& customer : customers) {
        if (customer.id == id) {
            found = true;
            
            cout << "\nCurrent Details:\n";
            cout << "Name: " << customer.name << endl;
            cout << "Address: " << customer.address << endl;
            cout << "Phone: " << customer.phone << endl;
            cout << "Rate: " << customer.rate << endl;
            
            cin.ignore();
            cout << "\nEnter New Name (press Enter to keep current): ";
            string newName;
            getline(cin, newName);
            if (!newName.empty()) customer.name = newName;
            
            cout << "Enter New Address (press Enter to keep current): ";
            string newAddress;
            getline(cin, newAddress);
            if (!newAddress.empty()) customer.address = newAddress;
            
            cout << "Enter New Phone (press Enter to keep current): ";
            string newPhone;
            getline(cin, newPhone);
            if (!newPhone.empty()) customer.phone = newPhone;
            
            cout << "Enter New Rate (enter 0 to keep current): ";
            double newRate;
            cin >> newRate;
            if (newRate != 0) customer.rate = newRate;
            
            cout << "\nCustomer information updated successfully!\n";
            break;
        }
    }
    
    if (!found) {
        cout << "Customer with ID " << id << " not found!\n";
    }
}

void deleteCustomer() {
    int id;
    bool found = false;
    
    cout << "\n--- Delete Customer ---\n";
    cout << "Enter Customer ID to delete: ";
    cin >> id;
    
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if (it->id == id) {
            found = true;
            
            // Also remove all milk entries for this customer
            milkEntries.erase(
                remove_if(milkEntries.begin(), milkEntries.end(), 
                    [id](const MilkEntry& entry) { return entry.customerId == id; }),
                milkEntries.end()
            );
            
            customers.erase(it);
            cout << "\nCustomer and all related milk entries deleted successfully!\n";
            break;
        }
    }
    
    if (!found) {
        cout << "Customer with ID " << id << " not found!\n";
    }
}

void addMilkEntry() {
    if (customers.empty()) {
        cout << "\nNo customers available. Please add customers first.\n";
        return;
    }
    
    MilkEntry newEntry;
    
    cout << "\n--- Add Milk Entry ---\n";
    
    // Display customers for reference
    viewCustomers();
    
    cout << "\nEnter Customer ID: ";
    cin >> newEntry.customerId;
    
    // Verify customer exists
    bool customerExists = false;
    double rate = 0.0;
    for (const auto& customer : customers) {
        if (customer.id == newEntry.customerId) {
            customerExists = true;
            rate = customer.rate;
            break;
        }
    }
    
    if (!customerExists) {
        cout << "Customer with ID " << newEntry.customerId << " not found!\n";
        return;
    }
    
    cout << "Enter Date (DD-MM-YYYY) or press Enter for today (" << getCurrentDate() << "): ";
    cin.ignore();
    string dateInput;
    getline(cin, dateInput);
    
    if (dateInput.empty()) {
        newEntry.date = getCurrentDate();
    } else {
        newEntry.date = dateInput;
    }
    
    cout << "Enter Morning Quantity (liters): ";
    cin >> newEntry.morningQty;
    
    cout << "Enter Evening Quantity (liters): ";
    cin >> newEntry.eveningQty;
    
    newEntry.totalQty = newEntry.morningQty + newEntry.eveningQty;
    newEntry.amount = calculateAmount(newEntry.totalQty, rate);
    
    milkEntries.push_back(newEntry);
    
    cout << "\nMilk entry added successfully!\n";
    cout << "Total Quantity: " << newEntry.totalQty << " liters\n";
    cout << "Total Amount: Rs. " << fixed << setprecision(2) << newEntry.amount << endl;
}

void viewDailyEntries() {
    if (milkEntries.empty()) {
        cout << "\nNo milk entries found!\n";
        return;
    }
    
    string date;
    cout << "\n--- View Daily Entries ---\n";
    cout << "Enter Date (DD-MM-YYYY) or press Enter for today (" << getCurrentDate() << "): ";
    cin.ignore();
    getline(cin, date);
    
    if (date.empty()) {
        date = getCurrentDate();
    }
    
    vector<MilkEntry> entriesForDate;
    for (const auto& entry : milkEntries) {
        if (entry.date == date) {
            entriesForDate.push_back(entry);
        }
    }
    
    if (entriesForDate.empty()) {
        cout << "\nNo entries found for date " << date << "!\n";
        return;
    }
    
    cout << "\n--- Milk Entries for " << date << " ---\n";
    cout << "----------------------------------------------------------------------------\n";
    cout << left << setw(8) << "Cust ID" << setw(15) << "Name" << setw(10) << "Morning" 
         << setw(10) << "Evening" << setw(10) << "Total" << setw(12) << "Amount" << endl;
    cout << "----------------------------------------------------------------------------\n";
    
    double totalQty = 0;
    double totalAmount = 0;
    
    for (const auto& entry : entriesForDate) {
        // Find customer name
        string customerName = "Unknown";
        for (const auto& customer : customers) {
            if (customer.id == entry.customerId) {
                customerName = customer.name;
                break;
            }
        }
        
        cout << left << setw(8) << entry.customerId << setw(15) << customerName 
             << setw(10) << fixed << setprecision(2) << entry.morningQty 
             << setw(10) << entry.eveningQty << setw(10) << entry.totalQty 
             << setw(12) << entry.amount << endl;
             
        totalQty += entry.totalQty;
        totalAmount += entry.amount;
    }
    
    cout << "----------------------------------------------------------------------------\n";
    cout << right << setw(43) << "Total: " << setw(10) << totalQty << setw(12) << totalAmount << endl;
    cout << "----------------------------------------------------------------------------\n";
}

void generateBill() {
    if (customers.empty() || milkEntries.empty()) {
        cout << "\nNo data available to generate bill!\n";
        return;
    }
    
    int customerId;
    string startDate, endDate;
    
    cout << "\n--- Generate Bill ---\n";
    viewCustomers();
    
    cout << "\nEnter Customer ID: ";
    cin >> customerId;
    
    // Verify customer exists
    bool customerExists = false;
    string customerName;
    double rate = 0.0;
    for (const auto& customer : customers) {
        if (customer.id == customerId) {
            customerExists = true;
            customerName = customer.name;
            rate = customer.rate;
            break;
        }
    }
    
    if (!customerExists) {
        cout << "Customer with ID " << customerId << " not found!\n";
        return;
    }
    
    cin.ignore();
    cout << "Enter Start Date (DD-MM-YYYY): ";
    getline(cin, startDate);
    
    cout << "Enter End Date (DD-MM-YYYY): ";
    getline(cin, endDate);
    
    // Collect all entries for this customer in date range
    vector<MilkEntry> customerEntries;
    double totalQty = 0;
    double totalAmount = 0;
    
    for (const auto& entry : milkEntries) {
        if (entry.customerId == customerId && entry.date >= startDate && entry.date <= endDate) {
            customerEntries.push_back(entry);
            totalQty += entry.totalQty;
            totalAmount += entry.amount;
        }
    }
    
    if (customerEntries.empty()) {
        cout << "\nNo entries found for customer " << customerName << " between " 
             << startDate << " and " << endDate << "!\n";
        return;
    }
    
    // Display bill
    cout << "\n====================================\n";
    cout << "          MILK DAIRY BILL          \n";
    cout << "====================================\n";
    cout << "Customer ID: " << customerId << "\n";
    cout << "Customer Name: " << customerName << "\n";
    cout << "Bill Period: " << startDate << " to " << endDate << "\n";
    cout << "Rate per liter: Rs. " << fixed << setprecision(2) << rate << "\n";
    cout << "====================================\n";
    cout << left << setw(12) << "Date" << setw(10) << "Morning" 
         << setw(10) << "Evening" << setw(10) << "Total" << setw(12) << "Amount" << endl;
    cout << "------------------------------------\n";
    
    for (const auto& entry : customerEntries) {
        cout << left << setw(12) << entry.date << setw(10) << fixed << setprecision(2) << entry.morningQty 
             << setw(10) << entry.eveningQty << setw(10) << entry.totalQty 
             << setw(12) << entry.amount << endl;
    }
    
    cout << "====================================\n";
    cout << right << setw(32) << "Total Quantity: " << setw(10) << totalQty << " liters\n";
    cout << right << setw(32) << "Total Amount: Rs. " << setw(10) << totalAmount << "\n";
    cout << "====================================\n";
    
    // Option to save bill to file
    cout << "\nDo you want to save this bill to a file? (y/n): ";
    char choice;
    cin >> choice;
    
    if (tolower(choice) == 'y') {
        string filename = "Bill_" + customerName + "_" + startDate + "_to_" + endDate + ".txt";
        ofstream outFile(filename);
        
        if (outFile) {
            outFile << "====================================\n";
            outFile << "          MILK DAIRY BILL          \n";
            outFile << "====================================\n";
            outFile << "Customer ID: " << customerId << "\n";
            outFile << "Customer Name: " << customerName << "\n";
            outFile << "Bill Period: " << startDate << " to " << endDate << "\n";
            outFile << "Rate per liter: Rs. " << fixed << setprecision(2) << rate << "\n";
            outFile << "====================================\n";
            outFile << left << setw(12) << "Date" << setw(10) << "Morning" 
                    << setw(10) << "Evening" << setw(10) << "Total" << setw(12) << "Amount" << endl;
            outFile << "------------------------------------\n";
            
            for (const auto& entry : customerEntries) {
                outFile << left << setw(12) << entry.date << setw(10) << fixed << setprecision(2) << entry.morningQty 
                        << setw(10) << entry.eveningQty << setw(10) << entry.totalQty 
                        << setw(12) << entry.amount << endl;
            }
            
            outFile << "====================================\n";
            outFile << right << setw(32) << "Total Quantity: " << setw(10) << totalQty << " liters\n";
            outFile << right << setw(32) << "Total Amount: Rs. " << setw(10) << totalAmount << "\n";
            outFile << "====================================\n";
            
            outFile.close();
            cout << "Bill saved to file: " << filename << "\n";
        } else {
            cout << "Error saving bill to file!\n";
        }
    }
}

void searchEntries() {
    if (milkEntries.empty()) {
        cout << "\nNo milk entries found!\n";
        return;
    }
    
    int choice;
    cout << "\n--- Search Entries ---\n";
    cout << "1. Search by Customer ID\n";
    cout << "2. Search by Date Range\n";
    cout << "Enter your choice: ";
    cin >> choice;
    
    if (choice == 1) {
        int customerId;
        cout << "Enter Customer ID: ";
        cin >> customerId;
        
        // Verify customer exists
        bool customerExists = false;
        string customerName;
        for (const auto& customer : customers) {
            if (customer.id == customerId) {
                customerExists = true;
                customerName = customer.name;
                break;
            }
        }
        
        if (!customerExists) {
            cout << "Customer with ID " << customerId << " not found!\n";
            return;
        }
        
        vector<MilkEntry> customerEntries;
        for (const auto& entry : milkEntries) {
            if (entry.customerId == customerId) {
                customerEntries.push_back(entry);
            }
        }
        
        if (customerEntries.empty()) {
            cout << "\nNo entries found for customer " << customerName << "!\n";
            return;
        }
        
        cout << "\n--- All Entries for " << customerName << " ---\n";
        cout << "----------------------------------------------------------------------------\n";
        cout << left << setw(12) << "Date" << setw(10) << "Morning" 
             << setw(10) << "Evening" << setw(10) << "Total" << setw(12) << "Amount" << endl;
        cout << "----------------------------------------------------------------------------\n";
        
        double totalQty = 0;
        double totalAmount = 0;
        
        for (const auto& entry : customerEntries) {
            cout << left << setw(12) << entry.date << setw(10) << fixed << setprecision(2) << entry.morningQty 
                 << setw(10) << entry.eveningQty << setw(10) << entry.totalQty 
                 << setw(12) << entry.amount << endl;
                 
            totalQty += entry.totalQty;
            totalAmount += entry.amount;
        }
        
        cout << "----------------------------------------------------------------------------\n";
        cout << right << setw(42) << "Total: " << setw(10) << totalQty << setw(12) << totalAmount << endl;
        cout << "----------------------------------------------------------------------------\n";
        
    } else if (choice == 2) {
        string startDate, endDate;
        cin.ignore();
        cout << "Enter Start Date (DD-MM-YYYY): ";
        getline(cin, startDate);
        
        cout << "Enter End Date (DD-MM-YYYY): ";
        getline(cin, endDate);
        
        vector<MilkEntry> dateRangeEntries;
        for (const auto& entry : milkEntries) {
            if (entry.date >= startDate && entry.date <= endDate) {
                dateRangeEntries.push_back(entry);
            }
        }
        
        if (dateRangeEntries.empty()) {
            cout << "\nNo entries found between " << startDate << " and " << endDate << "!\n";
            return;
        }
        
        cout << "\n--- Entries between " << startDate << " and " << endDate << " ---\n";
        cout << "----------------------------------------------------------------------------\n";
        cout << left << setw(8) << "Cust ID" << setw(15) << "Name" << setw(12) << "Date" 
             << setw(10) << "Morning" << setw(10) << "Evening" << setw(10) << "Total" << setw(12) << "Amount" << endl;
        cout << "----------------------------------------------------------------------------\n";
        
        double totalQty = 0;
        double totalAmount = 0;
        
        for (const auto& entry : dateRangeEntries) {
            // Find customer name
            string customerName = "Unknown";
            for (const auto& customer : customers) {
                if (customer.id == entry.customerId) {
                    customerName = customer.name;
                    break;
                }
            }
            
            cout << left << setw(8) << entry.customerId << setw(15) << customerName 
                 << setw(12) << entry.date << setw(10) << fixed << setprecision(2) << entry.morningQty 
                 << setw(10) << entry.eveningQty << setw(10) << entry.totalQty 
                 << setw(12) << entry.amount << endl;
                 
            totalQty += entry.totalQty;
            totalAmount += entry.amount;
        }
        
        cout << "----------------------------------------------------------------------------\n";
        cout << right << setw(55) << "Total: " << setw(10) << totalQty << setw(12) << totalAmount << endl;
        cout << "----------------------------------------------------------------------------\n";
        
    } else {
        cout << "Invalid choice!\n";
    }
}

void saveDataToFile() {
    // Save customers
    ofstream customerFile("customers.dat");
    if (customerFile) {
        for (const auto& customer : customers) {
            customerFile << customer.id << "," << customer.name << "," 
                         << customer.address << "," << customer.phone << "," 
                         << customer.rate << "\n";
        }
        customerFile.close();
    }
    
    // Save milk entries
    ofstream milkFile("milk_entries.dat");
    if (milkFile) {
        for (const auto& entry : milkEntries) {
            milkFile << entry.customerId << "," << entry.date << "," 
                     << entry.morningQty << "," << entry.eveningQty << "," 
                     << entry.totalQty << "," << entry.amount << "\n";
        }
        milkFile.close();
    }
}

void loadDataFromFile() {
    // Load customers
    ifstream customerFile("customers.dat");
    if (customerFile) {
        string line;
        while (getline(customerFile, line)) {
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 5) {
                Customer customer;
                customer.id = stoi(tokens[0]);
                customer.name = tokens[1];
                customer.address = tokens[2];
                customer.phone = tokens[3];
                customer.rate = stod(tokens[4]);
                customers.push_back(customer);
            }
        }
        customerFile.close();
    }
    
    // Load milk entries
    ifstream milkFile("milk_entries.dat");
    if (milkFile) {
        string line;
        while (getline(milkFile, line)) {
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() == 6) {
                MilkEntry entry;
                entry.customerId = stoi(tokens[0]);
                entry.date = tokens[1];
                entry.morningQty = stod(tokens[2]);
                entry.eveningQty = stod(tokens[3]);
                entry.totalQty = stod(tokens[4]);
                entry.amount = stod(tokens[5]);
                milkEntries.push_back(entry);
            }
        }
        milkFile.close();
    }
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << setw(2) << setfill('0') << ltm->tm_mday << "-"
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << 1900 + ltm->tm_year;
    
    return ss.str();
}

double calculateAmount(double quantity, double rate) {
    return quantity * rate;
}
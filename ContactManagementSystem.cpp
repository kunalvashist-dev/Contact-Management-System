
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <cerrno>
#include <cstring>

using namespace std;

// ==========================================
// CONTACT STRUCTURE
// ==========================================

struct Contact {
    string name;
    string phone;
    string email;
    string address;
};

const string FILE_NAME = "contacts.txt";

vector<Contact> contacts;


// ==========================================
// UTILITY FUNCTIONS
// ==========================================

// Convert string to lowercase for searching
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });

    return str;
}

// Check whether a field is safe for our file format
bool isValidField(const string& value) {
    return !value.empty() &&
           value.find('|') == string::npos &&
           value.find('\n') == string::npos &&
           value.find('\r') == string::npos;
}

// Get non-empty input
string getInput(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);

        if (!isValidField(value)) {
            cout << "Invalid input. Field cannot be empty "
                 << "or contain the | character.\n";
        } else {
            return value;
        }
    }
}

// Validate phone number
bool isValidPhone(const string& phone) {
    int digits = 0;

    for (unsigned char c : phone) {
        if (isdigit(c)) {
            digits++;
        }
        else if (c != '+' && c != '-' &&
                 c != ' ' && c != '(' && c != ')') {
            return false;
        }
    }

    return digits >= 7 && digits <= 15;
}

// Check for duplicate phone numbers
bool phoneExists(const string& phone, int excludeIndex = -1) {
    for (int i = 0; i < static_cast<int>(contacts.size()); i++) {
        if (i != excludeIndex && contacts[i].phone == phone) {
            return true;
        }
    }

    return false;
}

// Get and validate phone number
string getPhone(int excludeIndex = -1) {
    while (true) {
        string phone = getInput("Enter phone number: ");

        if (!isValidPhone(phone)) {
            cout << "Invalid phone number. Enter 7-15 digits.\n";
        }
        else if (phoneExists(phone, excludeIndex)) {
            cout << "This phone number already exists!\n";
        }
        else {
            return phone;
        }
    }
}


// ==========================================
// FILE HANDLING
// ==========================================

// Load contacts from text file
void loadContacts() {
    ifstream file(FILE_NAME);

    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Contact contact;

        if (getline(ss, contact.name, '|') &&
            getline(ss, contact.phone, '|') &&
            getline(ss, contact.email, '|') &&
            getline(ss, contact.address) &&
            isValidField(contact.name) &&
            isValidPhone(contact.phone) &&
            isValidField(contact.email) &&
            isValidField(contact.address) &&
            !phoneExists(contact.phone)) {

            contacts.push_back(contact);
        }
    }

    file.close();
}

// Save all contacts to text file
bool saveContacts() {
    ofstream file(FILE_NAME);

    if (!file.is_open()) {
        cout << "Error: Unable to save contacts! (" << strerror(errno) << ")\n";
        return false;
    }

    for (const Contact& contact : contacts) {
        file << contact.name << "|"
             << contact.phone << "|"
             << contact.email << "|"
             << contact.address << "\n";
    }

    file.close();
    return true;
}


// ==========================================
// DISPLAY CONTACTS
// ==========================================

void displayContact(const Contact& contact) {
    cout << "\n----------------------------------\n";
    cout << "Name    : " << contact.name << "\n";
    cout << "Phone   : " << contact.phone << "\n";
    cout << "Email   : " << contact.email << "\n";
    cout << "Address : " << contact.address << "\n";
    cout << "----------------------------------\n";
}

void viewContacts() {
    if (contacts.empty()) {
        cout << "\nNo contacts available.\n";
        return;
    }

    cout << "\n========== ALL CONTACTS ==========\n";

    for (int i = 0; i < static_cast<int>(contacts.size()); i++) {
        cout << "\nContact #" << i + 1;
        displayContact(contacts[i]);
    }

    cout << "\nTotal Contacts: " << contacts.size() << "\n";
}


// ==========================================
// ADD CONTACT
// ==========================================

void addContact() {
    Contact newContact;

    cout << "\n========== ADD NEW CONTACT ==========\n";

    newContact.name = getInput("Enter name: ");
    newContact.phone = getPhone();
    newContact.email = getInput("Enter email: ");
    newContact.address = getInput("Enter address: ");

    contacts.push_back(newContact);

    if (saveContacts()) {
        cout << "\nContact added successfully!\n";
    }
    else {
        contacts.pop_back();
    }
}


// ==========================================
// SEARCH CONTACT
// ==========================================

void searchContact() {
    if (contacts.empty()) {
        cout << "\nNo contacts available to search.\n";
        return;
    }

    string keyword = getInput("\nEnter name or phone to search: ");
    string searchTerm = toLowerCase(keyword);

    bool found = false;

    for (const Contact& contact : contacts) {
        if (toLowerCase(contact.name).find(searchTerm) != string::npos ||
            contact.phone.find(keyword) != string::npos) {

            displayContact(contact);
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo matching contact found.\n";
    }
}


// ==========================================
// UPDATE CONTACT
// ==========================================

void updateContact() {
    if (contacts.empty()) {
        cout << "\nNo contacts available to update.\n";
        return;
    }

    string phone = getInput("\nEnter phone number of contact to update: ");

    for (int i = 0; i < static_cast<int>(contacts.size()); i++) {
        if (contacts[i].phone == phone) {
            cout << "\nExisting Contact Details:";
            displayContact(contacts[i]);

            Contact oldContact = contacts[i];

            cout << "\nEnter updated details:\n";

            contacts[i].name = getInput("Enter new name: ");
            contacts[i].phone = getPhone(i);
            contacts[i].email = getInput("Enter new email: ");
            contacts[i].address = getInput("Enter new address: ");

            if (saveContacts()) {
                cout << "\nContact updated successfully!\n";
            }
            else {
                contacts[i] = oldContact;
            }

            return;
        }
    }

    cout << "\nContact not found.\n";
}


// ==========================================
// DELETE CONTACT
// ==========================================

void deleteContact() {
    if (contacts.empty()) {
        cout << "\nNo contacts available to delete.\n";
        return;
    }

    string phone = getInput("\nEnter phone number to delete: ");

    for (int i = 0; i < static_cast<int>(contacts.size()); i++) {
        if (contacts[i].phone == phone) {
            cout << "\nContact found:";
            displayContact(contacts[i]);

            string confirmation = getInput(
                "Are you sure you want to delete this contact? (yes/no): "
            );

            if (toLowerCase(confirmation) == "yes") {
                Contact deletedContact = contacts[i];

                contacts.erase(contacts.begin() + i);

                if (saveContacts()) {
                    cout << "\nContact deleted successfully!\n";
                }
                else {
                    contacts.insert(contacts.begin() + i, deletedContact);
                }
            }
            else {
                cout << "\nDeletion cancelled.\n";
            }

            return;
        }
    }

    cout << "\nContact not found.\n";
}


// ==========================================
// SORT CONTACTS
// ==========================================

void sortContacts() {
    if (contacts.empty()) {
        cout << "\nNo contacts available to sort.\n";
        return;
    }

    sort(contacts.begin(), contacts.end(),
        [](const Contact& a, const Contact& b) {
            return toLowerCase(a.name) < toLowerCase(b.name);
        });

    if (saveContacts()) {
        cout << "\nContacts sorted alphabetically!\n";
        viewContacts();
    }
}


// ==========================================
// MAIN MENU
// ==========================================

void displayMenu() {
    cout << "\n";
    cout << "========================================\n";
    cout << "       CONTACT MANAGEMENT SYSTEM        \n";
    cout << "========================================\n";
    cout << "  1. Add New Contact                    \n";
    cout << "  2. View All Contacts                  \n";
    cout << "  3. Search Contact                     \n";
    cout << "  4. Update Contact                     \n";
    cout << "  5. Delete Contact                     \n";
    cout << "  6. Sort Contacts                      \n";
    cout << "  7. Exit                               \n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

int main() {
    loadContacts();

    cout << "\nWelcome to Contact Management System!\n";
    cout << "Loaded " << contacts.size() << " saved contact(s).\n";

    int choice;

    while (true) {
        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                addContact();
                break;

            case 2:
                viewContacts();
                break;

            case 3:
                searchContact();
                break;

            case 4:
                updateContact();
                break;

            case 5:
                deleteContact();
                break;

            case 6:
                sortContacts();
                break;

            case 7:
                cout << "\nThank you for using Contact Management System!\n";
                return 0;

            default:
                cout << "\nInvalid choice! Select between 1 and 7.\n";
        }
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <cstring>
#include <limits> // for numeric_limits

using namespace std;

class Account {
    int accountNumber;
    char name[50];
    char type; // S for Saving, C for Current
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void modify();
    int getAccountNumber() const { return accountNumber; }
    void report() const;
};

void Account::createAccount() {
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer

    cout << "Enter Name: ";
    cin.getline(name, 50);

    cout << "Enter Account Type (S/C): ";
    cin >> type;
    type = toupper(type);

    cout << "Enter Initial Balance: ";
    cin >> balance;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
}

void Account::showAccount() const {
    cout << "Account No.: " << accountNumber << endl;
    cout << "Name: " << name << endl;
    cout << "Type: " << type << endl;
    cout << "Balance: " << balance << endl;
}

void Account::modify() {
    cout << "Modify Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
    cin.getline(name, 50);

    cout << "Modify Type (S/C): ";
    cin >> type;
    type = toupper(type);

    cout << "Modify Balance: ";
    cin >> balance;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
}

void Account::report() const {
    cout << accountNumber << "\t" << name << "\t" << type << "\t" << balance << endl;
}

void writeAccount() {
    Account ac;
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void displayAccount(int accNo) {
    Account ac;
    bool found = false;
    ifstream inFile("accounts.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() == accNo) {
            ac.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "Account not found!" << endl;
}

void modifyAccount(int accNo) {
    Account ac;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof()) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (file && ac.getAccountNumber() == accNo) {
            ac.showAccount();
            cout << "\nEnter New Details:\n";
            ac.modify();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            found = true;
            break;
        }
    }

    file.close();
    if (!found)
        cout << "Account not found!" << endl;
}

void deleteAccount(int accNo) {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() != accNo)
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
        else
            found = true;
    }

    inFile.close();
    outFile.close();
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found)
        cout << "Account Deleted Successfully.\n";
    else
        cout << "Account not found!" << endl;
}

void displayAll() {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    cout << "\nAccount No.\tName\tType\tBalance\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.report();
    }
    inFile.close();
}

int main() {
    int choice;
    int accNo;

    do {
        cout << "\n\n*** BANK MANAGEMENT SYSTEM ***\n";
        cout << "1. Create New Account\n";
        cout << "2. Display Account\n";
        cout << "3. Modify Account\n";
        cout << "4. Delete Account\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "Select Your Option (1-6): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer

        switch (choice) {
            case 1: writeAccount(); break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accNo;
                displayAccount(accNo);
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accNo;
                modifyAccount(accNo);
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accNo;
                deleteAccount(accNo);
                break;
            case 5: displayAll(); break;
            case 6: cout << "Exiting..."; break;
            default: cout << "Invalid option! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}
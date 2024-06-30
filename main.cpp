#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib>

using namespace std;

void setConsoleTitle(const string& title) {
    #ifdef _WIN32
        SetConsoleTitleA(title.c_str());
    #else
        cout << "\033]0;" << title << "\007";
    #endif
}

struct Transaction {
    string transactionId;
    string timestamp;
    double amount;

    Transaction(const string& id, const string& time, double amt)
        : transactionId(id), timestamp(time), amount(amt) {}
};

class Account {
private:
    string accountNumber;
    string accountHolderName;
    double balance;
    vector<Transaction> transactions;


public:
    Account() {}
    Account(const string& accNumber, const string& accName, double initialBalance)
        : accountNumber(accNumber), accountHolderName(accName), balance(initialBalance) {}

    string getAccountNumber() const { return accountNumber; }
    string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        balance += amount;
        recordTransaction("DEPOSIT", amount);
        cout << "Deposited " << amount << " successfully.\n";
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            recordTransaction("WITHDRAWAL", amount);
            cout << "Withdrawn " << amount << " successfully.\n";
            return true;
        } else {
            cout << "Insufficient funds!\n";
            return false;
        }
    }

    void display() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    void saveAccount(ofstream& file) {
        file << accountNumber << " " << accountHolderName << " " << balance << endl;
    }

    void recordTransaction(const string& type, double amount) {
        time_t now = time(0);
        stringstream ss;
        ss << now;
        string transactionId = ss.str();

        string timestamp = ctime(&now);
        timestamp.erase(timestamp.find_last_not_of(" \n\r\t")+1);

        Transaction transaction(transactionId, timestamp, amount);
        transactions.push_back(transaction);

        saveTransaction(transaction);
    }

    void saveTransaction(const Transaction& transaction) {
        ofstream outFile("transactions.txt", ios::app);
        if (outFile) {
            outFile << transaction.transactionId << " "
                    << accountNumber << " "
                    << transaction.timestamp << " "
                    << transaction.amount << endl;
            outFile.close();
        } else {
            cerr << "Error saving transaction to file.\n";
        }
    }

    void loadTransactions() {
        ifstream inFile("transactions.txt");
        if (inFile) {
            string id, accNum, timestamp;
            double amt;
            while (inFile >> id >> accNum >> timestamp >> amt) {
                if (accNum == accountNumber) {
                    Transaction transaction(id, timestamp, amt);
                    transactions.push_back(transaction);
                }
            }
            inFile.close();
        } else {
            cerr << "Error loading transactions from file.\n";
        }
    }

    void displayTransactions() const {
        cout << "Transaction History for Account: " << accountNumber << endl;
        for (const auto& transaction : transactions) {
            cout << "Transaction ID: " << transaction.transactionId << endl;
            cout << "Timestamp: " << transaction.timestamp << endl;
            cout << "Amount: $" << fixed << setprecision(2) << transaction.amount << endl;
            cout << "----------------------------------------\n";
        }
    }
};

void showMainMenu();
void createAccount();
void accessAccount();
void processDeposit(Account& account);
void processWithdraw(Account& account);
void displayTransactions(Account& account);
void clearScreen();
bool accountExists(const string& accNumber);
void loadAccounts();
void saveAccounts();
bool isNumber(const string& str);

vector<Account> accounts;
const string FILENAME = "accounts.txt";

int main() {
    setConsoleTitle("Simple Bank System");
    loadAccounts();

    int choice;
    string choiceInput;
    do {
        showMainMenu();
        cout << "Enter your choice: ";
        cin >> choiceInput;
        cin.ignore();

        if(isNumber(choiceInput)) {
            choice = stoi(choiceInput);
        } else {
            cout << "Invalid choice! Please enter a valid option.\n";
            cin.get();
        }

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                accessAccount();
                break;
            case 3:
                cout << "Exiting program...\n";
                saveAccounts();
                break;
            default:
                cout << "Invalid choice! Please enter a valid option.\n";
        }
    } while (choice != 3);

    return 0;
}

bool isNumber(const string& str) {
    for (char const &c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

void showMainMenu() {
    clearScreen();
    cout << "===== Main Menu =====\n";
    cout << "1. Create an account\n";
    cout << "2. Access an account\n";
    cout << "3. Exit\n";
    cout << "=====================\n";
}

void createAccount() {
    clearScreen();
    string accNumber, accName;
    double initialBalance;

    cout << "Enter account holder's name: ";
    getline(cin, accName);

    cout << "Enter initial balance: $";
    cin >> initialBalance;

    accNumber = "ACC" + to_string(accounts.size() + 1);

    Account newAccount(accNumber, accName, initialBalance);

    accounts.push_back(newAccount);

    cout << "Account created successfully!\n";
    cout << "Your account number is: " << accNumber << endl;

    saveAccounts();

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void accessAccount() {
    clearScreen();
    string accNumber;
    cout << "Enter account number: ";
    getline(cin, accNumber);

    bool found = false;
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getAccountNumber() == accNumber) {
            found = true;
            Account& currentAccount = accounts[i];

            int option;
            do {
                clearScreen();
                currentAccount.display();
                cout << "\n===== Account Menu =====\n";
                cout << "1. Deposit\n";
                cout << "2. Withdraw\n";
                cout << "3. Transaction History\n";
                cout << "4. Back to Main Menu\n";
                cout << "=========================\n";
                cout << "Enter your choice: ";
                cin >> option;
                cin.ignore();

                switch (option) {
                    case 1:
                        processDeposit(currentAccount);
                        break;
                    case 2:
                        processWithdraw(currentAccount);
                        break;
                    case 3:
                        displayTransactions(currentAccount);
                        break;
                    case 4:
                        clearScreen();
                        break;
                    default:
                        clearScreen();
                        cout << "Invalid choice! Please enter a valid option.\n";
                }
            } while (option != 4);

            break;
        }
    }

    if (!found) {
        clearScreen();
        cout << "Account not found!\n";
    }

    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void displayTransactions(Account& account) {
    account.displayTransactions();
    cout << "Press any key to continue...";
    cin.get();
}

void processDeposit(Account& account) {
    double amount;
    cout << "Enter amount to deposit: $";
    cin >> amount;
    account.deposit(amount);
    saveAccounts();
}

void processWithdraw(Account& account) {
    double amount;
    cout << "Enter amount to withdraw: $";
    cin >> amount;
    bool success = account.withdraw(amount);
    if (success) {
        saveAccounts();
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

bool accountExists(const string& accNumber) {
    for (const auto& acc : accounts) {
        if (acc.getAccountNumber() == accNumber) {
            return true;
        }
    }
    return false;
}

void loadAccounts() {
    ifstream inFile(FILENAME);
    if (inFile) {
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string accNumber, accName;
            double balance;
            ss >> accNumber >> accName >> balance;
            Account account(accNumber, accName, balance);
            account.loadTransactions();
            accounts.push_back(account);
        }
        inFile.close();
    }
}

void saveAccounts() {
    ofstream outFile(FILENAME);
    if (outFile) {
        for (auto& acc : accounts) {
            acc.saveAccount(outFile);
        }
        outFile.close();
    } else {
        cerr << "Error saving accounts to file.\n";
    }
}



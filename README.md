# Simple Bank System CLI

A simple Command Line Interface (CLI) banking system implemented in C++.

## Features

- **Create Account**: Create a new bank account with an initial balance.
- **Deposit**: Deposit money into an account.
- **Withdraw**: Withdraw money from an account (if sufficient funds are available).
- **Transaction History**: View the transaction history of an account.
- **Persistent Storage**: Accounts and transaction histories are saved to files (`accounts.txt` and `transactions.txt`).

## Prerequisites

- C++ compiler (e.g., g++, clang++)
- Compatible with Windows and Linux

## Usage

1. **Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/simple-bank-system-cli.git
    cd simple-bank-system-cli
    ```

2. **Compile and run**:
    ```sh
    g++ -o bank_system main.cpp
    ./bank_system
    ```

## Code Overview

### Main Function

Sets console title, loads accounts from file, and presents the main menu.

```cpp
int main() {
    setConsoleTitle("Simple Bank System");
    loadAccounts();
    // Main menu logic...
    return 0;
}
```

Account Class
Manages accounts, including deposits, withdrawals, and transaction history.

```cpp
class Account {
private:
    string accountNumber;
    string accountHolderName;
    double balance;
    vector<Transaction> transactions;

public:
    // Constructors, getters, deposit, withdraw, display, and transaction methods...
};
```

# Functions
- **setConsoleTitle**: Sets console window title.
- **showMainMenu**: Displays main menu options.
- **createAccount**: Creates a new account.
- **accessAccount**: Accesses an existing account.
- **processDeposit**: Handles deposit transactions.
- **processWithdraw**: Handles withdrawal transactions.
- **displayTransactions**: Displays transaction history.
- **clearScreen**: Clears console screen.
- **loadAccounts**: Loads accounts from file.
- **saveAccounts**: Saves accounts to file.
- **isNumber**: Checks if string is a valid number.

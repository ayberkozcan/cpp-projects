#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <regex>

using namespace std;

vector<class BankAccount> registeredAccounts;

class BankAccount {
public:
    string name;
    string surname;
    string birthYear;
    string email;
    string password;
    string accno;
    int balance;
    int limit;

    BankAccount(string n, string s, string b, string e, string p, string a, int ba, int l)
        : name(n), surname(s), birthYear(b), email(e), password(p), accno(a), balance(ba), limit(l) {}

    string getName() const { return name; }
    string getSurname() const { return surname; }
    string getBirthYear() const { return birthYear; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getAccountNumber() const { return accno; }
    int getBalance() const { return balance; }

    void addBalance(int amount) { balance += amount; }
    void withdrawBalance(int amount) { balance -= amount; }
    int getLimit() const { return limit; }
    void changeLimit(int amount) { limit = amount; }
};

void showHomepage(BankAccount& account);
void showSettings(BankAccount& account);

void viewBalance(BankAccount& account);
void manageBalance(BankAccount& account, const string& filename, const string& operation);
void withdrawMoney(BankAccount& account);
void changeLimit(BankAccount& account);
void updateLimit(BankAccount& account, const string& filename, const string& newLimit);

void saveAccount(const BankAccount& account, const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << account.name << " " << account.surname << " "
             << account.birthYear << " " << account.email << " "
             << account.password << " " << account.accno << " "
             << account.balance << account.limit << endl;
        file.close();
    } else {
        cerr << "Error: Could not open file!" << endl;
    }
}

vector<BankAccount> loadAccount(const string& filename) {
    ifstream file(filename);
    vector<BankAccount> accounts;

    if (file.is_open()) {
        string name, surname, birthYear, email, password, accno;
        int balance, limit;

        while (file >> name >> surname >> birthYear >> email >> password >> accno >> balance >> limit) {
            accounts.emplace_back(name, surname, birthYear, email, password, accno, balance, limit);
        }
        file.close();
    } else {
        cerr << "Error: Could not open file!" << endl;
    }

    return accounts;
}

BankAccount* loginControl(const string& acc_no, const string& password, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file!" << endl;
        return nullptr;
    }

    string name, surname, birthYear, email, pwd, account_number;
    int balance, limit;

    while (file >> name >> surname >> birthYear >> email >> pwd >> account_number >> balance >> limit) {
        if (account_number == acc_no && pwd == password) {
            return new BankAccount(name, surname, birthYear, email, pwd, acc_no, balance, limit);
        }
    }

    return nullptr;
}

void showHomepage(BankAccount& account) {
    int action;
    string filename = "accounts.txt";

    while(true) {
        cout << "-------------------\nWelcome "<< account.getName() << "!\n" << endl;
        cout << "\nChoose action below" << endl;
        cout << "(1) View Balance" << endl;
        cout << "(2) Add Money" << endl;
        cout << "(3) Withdraw Money" << endl;
        cout << "(4) View Interest Rates" << endl;
        cout << "(5) Settings" << endl;
        cout << "(6) Quit" << endl;
        cin >> action;

        switch (action)
        {
        case 1:
            viewBalance(account);
            break;
        case 2:
            manageBalance(account, filename, "add");
            break;
        case 3:
            if (account.getBalance() == 0) {
                cout << "\nYour balance is 0!\n" << endl;
            } else {
                manageBalance(account, filename, "withdraw");
            }
            break;
        case 4:
            cout << "\n32 Days: 3%" << endl; 
            cout << "90 Days: 4%" << endl; 
            cout << "180 Days: 4.5%" << endl; 
            cout << "\n(1) Interest Calculator" << endl;
            cout << "(2) Back" << endl;

            cin >> action;

            switch (action)
            {
                case 1: {
                    int day;
                    double money;
                    
                    cout << "Money (amount to deposit): " << endl;
                    cin >> money;

                    while(true) {
                        cout << "Days (32, 90 or 180): " << endl;
                        cin >> day;

                        double interestRate = 0.0;
                        double interest = 0.0;

                        if (day == 32) {
                            interestRate = 0.03;
                            interest = money * interestRate * (day / 365.0);
                            cout << "\nInterest rate for 32 days is 3%" << endl;
                            cout << "Interest earned: " << interest << endl;
                        } 
                        else if (day == 90) {
                            interestRate = 0.04;
                            interest = money * interestRate * (day / 365.0);
                            cout << "\nInterest rate for 90 days is 4%" << endl;
                            cout << "Interest earned: " << interest << endl;
                        } 
                        else if (day == 180) {
                            interestRate = 0.045;
                            interest = money * interestRate * (day / 365.0);
                            cout << "\nInterest rate for 180 days is 4.5%" << endl;
                            cout << "Interest earned: " << interest << endl;
                        }
                        else {
                            cout << "\nInvalid day entered!\n" << endl;
                            continue;
                        }
                        break;
                    }
                    break;
                } 
                case 2:
                    break;
            }
            continue;
        case 5:
            showSettings(account);
            break;
        case 6:
            exit(0);
            break;
        default:
            cout << "Invalid action. Please select a valid option.\n" << endl;
        }
    }
}

void showSettings(BankAccount& account) {
    int action;
    int newLimit;
    string input;

    while(true) {
        cout << "-------------------\nSettings\n" << endl;
        cout << "(1) Withdrawal Limit" << endl;
        cout << "(2) Quit" << endl;

        cin >> action;

        switch (action) {
            case 1:
                while(true) {
                    cout << "\nYour Withdrawal Limit: " << account.getLimit() << endl;
                    cout << "\n(1) Change Limit" << endl;
                    cout << "(2) Quit" << endl;

                    cin >> action;

                    switch (action) {
                        case 1:
                            while(true) {
                                cout << "Enter the limit you want (q to quit): ";
                                cin >> input;

                                if (input == "q") {
                                    break;
                                }

                                try {
                                    newLimit = stoi(input);
                                    if (newLimit > 0 && newLimit < 50000) {
                                        updateLimit(account, "accounts.txt", to_string(newLimit));
                                        cout << "\nWithdrawal limit updated to " << newLimit << endl;
                                        break;
                                    } else {
                                        cout << "\nNew limit must be between 0 and 50.000!\n" << endl;
                                    }
                                } catch (const invalid_argument& e) {
                                    cout << "Invalid input! Please enter a valid number or 'q' to quit." << endl;
                                } catch (const out_of_range& e) {
                                    cout << "Input out of range. Please enter a valid number within the limit." << endl;
                                }
                            }
                            break;
                        case 2:
                            break;
                        default:
                            break;
                    }
                    if (action == 2) break;
                }
            case 2:
                break;
            default:
                break;
        }
        if (action == 2) break;
    }
}

void viewBalance(BankAccount& account) {
    cout << "Balance: " << account.getBalance() << endl;
}

void updateLimit(BankAccount& account, const string& filename, const string& newLimit) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file!" << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool accountFound = false;

    while (getline(file, line)) {
        istringstream iss(line);
        string name, surname, birthYear, email, pwd, account_number;
        int balance, limit;

        if (iss >> name >> surname >> birthYear >> email >> pwd >> account_number >> balance >> limit) {
                if (account_number == account.getAccountNumber()) {
                    accountFound = true;

                    account.changeLimit(stoi(newLimit));

                    limit = stoi(newLimit);

                    line = name + " " + surname + " " + birthYear + " " + email + " " + pwd + " " + account_number + " " + to_string(balance) + " " + to_string(limit);
                }
            }

        lines.push_back(line);
    }

    file.close();

    if (!accountFound) {
            cout << "Account not found!" << endl;
            return;
        }

        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file for writing!" << endl;
            return;
        }

        for (const auto& updatedLine : lines) {
            outFile << updatedLine << endl;
        }

        outFile.close();
}

void manageBalance(BankAccount& account, const string& filename, const string& operation) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file!" << endl;
        return;
    }

    if (operation == "add") {
        vector<string> lines;
        string line;
        bool accountFound = false;

        while (getline(file, line)) {
            istringstream iss(line);
            string name, surname, birthYear, email, pwd, account_number;
            int balance, limit;

            if (iss >> name >> surname >> birthYear >> email >> pwd >> account_number >> balance >> limit) {
                if (account_number == account.getAccountNumber()) {
                    accountFound = true;
                    int money;
                    cout << "\nHow much money you want to add?" << endl;
                    cin >> money;

                    if (money < 0) {
                        cout << "\nThe amount must be valid!" << endl;
                    } else {
                        balance += money;
                        cout << money << " $ added to your balance." << endl;

                        account.addBalance(money);

                        line = name + " " + surname + " " + birthYear + " " + email + " " + pwd + " " + account_number + " " + to_string(balance) + " " + to_string(limit);
                    }
                }
            }

            lines.push_back(line);
        }

        file.close();

        if (!accountFound) {
            cout << "Account not found!" << endl;
            return;
        }

        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file for writing!" << endl;
            return;
        }

        for (const auto& updatedLine : lines) {
            outFile << updatedLine << endl;
        }

        outFile.close();
    }

    else if (operation == "withdraw") {
        vector<string> lines;
        string line;
        bool accountFound = false;

        while (getline(file, line)) {
            istringstream iss(line);
            string name, surname, birthYear, email, pwd, account_number;
            int balance, limit;

            if (iss >> name >> surname >> birthYear >> email >> pwd >> account_number >> balance >> limit) {
                if (account_number == account.getAccountNumber()) {
                    accountFound = true;
                    int money;
                    cout << "\nHow much money you want to withdraw?\nYour withdrawal limit is: " << limit << endl;
                    cin >> money;

                    if (money > balance) {
                        cout << "\nInsufficient balance!" << endl;
                    } else if (money < 0) {
                        cout << "\nThe amount must be valid!" << endl;
                    } else if (money > limit) {
                        cout << "\nThe amount you want to withdraw exceeds your withdrawal limit.\nYou can change your limit in the settings menu!" << endl;
                    } else {
                        balance -= money;
                        cout << money << " $ withdrawn from your balance." << endl;

                        account.withdrawBalance(money);

                        line = name + " " + surname + " " + birthYear + " " + email + " " + pwd + " " + account_number + " " + to_string(balance) + " " + to_string(limit);
                    }
                }
            }

            lines.push_back(line);
        }

        file.close();

        if (!accountFound) {
            cout << "Account not found!" << endl;
            return;
        }

        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file for writing!" << endl;
            return;
        }

        for (const auto& updatedLine : lines) {
            outFile << updatedLine << endl;
        }

        outFile.close();
    }
}

int main() {
    int action;
    int attempt = 3;
    BankAccount* loggedInAccount = nullptr;

    string filename = "accounts.txt";
    vector<BankAccount> registeredAccounts;

    while (true) {
        cout << "Welcome!" << endl;
        cout << "(1) Login" << endl;
        cout << "(2) Create an Account" << endl;

        cin >> action;

        switch(action) {
            case 1: {
                while(true) {
                    if (attempt == 0) {
                        cout << "You have not attempts left!\n\n";
                        break;
                    }
                    
                    string acc_no, password;
                    cout << "Account Number: ";
                    cin >> acc_no;
                    if (acc_no == "q") {
                        break;
                    }
                    
                    cout << "Password: ";
                    cin >> password;
                    if (password == "q") {
                        break;
                    }

                    BankAccount* loggedInAccount = loginControl(acc_no, password, filename);
                    if (loggedInAccount != nullptr) {
                        cout << "Login Successful!" << endl;
                        cout << "Redirecting to homepage..." << endl;
                        system("cls");

                        showHomepage(*loggedInAccount);
                        delete loggedInAccount;
                    } else {
                        cout << "Invalid credentials, please try again! (q to quit)" << endl;
                        attempt -= 1;
                    }
                }
                break;
            }

            case 2: {
                system("cls");
                string name, surname, birthYear, email, password;
                cout << "Create an Account" << endl;
                
                cout << "Name: ";
                while(true) {
                    cin >> name;
                    if (name.length() < 3 || name.length() > 30 || !all_of(name.begin(), name.end(), ::isalpha)) {
                        cout << "Please enter a valid name (between 3 and 30 characters, only letters)!" << endl;
                    } else {
                        break;
                    }
                }
                
                cout << "Surname: ";
                while(true) {
                    cin >> surname;
                    if (surname.length() < 2 || surname.length() > 30 || !all_of(surname.begin(), surname.end(), ::isalpha)) {
                        cout << "Please enter a valid surname (between 2 and 30 characters, only letters)!" << endl;
                    } else {
                        break;
                    }
                }

                cout << "Birth Year: ";
                while(true) {
                    cin >> birthYear;

                    time_t t = time(0);
                    struct tm *now = localtime(&t);

                    int currentYear = 1900 + now->tm_year;

                    if (birthYear.length() != 4 || !all_of(birthYear.begin(), birthYear.end(), ::isdigit) || stoi(birthYear) < 1900 || stoi(birthYear) > currentYear - 18) {
                        cout << "Please enter a valid birth year (4 digits, between 1900 and " << currentYear - 18 << ")!" << endl;
                    } else {
                        break;
                    }
                }

                cout << "Email: ";
                while(true) {
                    cin >> email;

                    regex emailPattern(R"((\w+)(\.\w+)*@(\w+\.)+[a-zA-Z]{2,})");

                    if (email.length() < 5 || email.length() > 30 || !regex_match(email, emailPattern)) {
                        cout << "Please enter a valid email!" << endl;
                    } else {
                        break;
                    }
                }

                cout << "Password: ";
                while(true) {
                    cin >> password;
                    if (password.length() < 5 || password.length() > 20) {
                        cout << "Please enter a valid password (between 5 and 20 characters)!" << endl;
                    } else {
                        break;
                    }
                }

                srand(time(nullptr));
                string accno = to_string(10000 + rand() % 90000);

                BankAccount newAccount(name, surname, birthYear, email, password, accno, 0, 1000);
                registeredAccounts.push_back(newAccount);

                saveAccount(newAccount, filename);

                system("cls");
                cout << "Account created successfully!" << endl;
                cout << "\nAccount Number: " << newAccount.getAccountNumber() << "\n" << endl;
                break;
            }

            default:
                system("cls");
                cout << "Invalid choice, please try again!" << endl;
        }
    }

    return 0;
}

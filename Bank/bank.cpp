#include <iostream>
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
private:
    string name;
    string surname;
    string birthYear;
    string email;
    string password;
    string accno;
    int balance;
    int limit;

public:
    BankAccount(string n, string s, string b, string e, string p, string a)
        : name(n), surname(s), birthYear(b), email(e), password(p), accno(a), balance(0), limit(0) {}

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
void addMoney(BankAccount& account);
void withdrawMoney(BankAccount& account);
void changeLimit(BankAccount& account);

bool loginControl(const string& acc_no, const string& password) {
    for (const auto& account : registeredAccounts) {
        if (account.getAccountNumber() == acc_no && account.getPassword() == password) {
            return true;
        } else {
            return false;
        }
    }
}

void showHomepage(BankAccount& account) {
    int action;

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
            addMoney(account);
            break;
        case 3:
            if (account.getBalance() == 0) {
                cout << "\nYour balance is 0!\n" << endl;
            } else {
                withdrawMoney(account);
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
                case 2: // fix
                    // system("cls");
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
                                        account.changeLimit(newLimit);
                                        cout << "Withdrawal limit updated to " << newLimit << endl;
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

void addMoney(BankAccount& account) {
    int money;
    cout << "How much money you want to add?" << endl;
    cin >> money;
    account.addBalance(money);
    cout << money << " $ added to your balance." << endl;
}

void withdrawMoney(BankAccount& account) {
    int money;
    cout << "How much money you want to withdraw?" << endl;
    cin >> money;

    if (money > account.getBalance()) {
        cout << "Insufficient balance!" << endl;
    } else {
        account.withdrawBalance(money);
        cout << money << " $ withdrawn from your balance." << endl;
    }
}

int main() {
    int action;
    int attempt = 3;
    BankAccount* loggedInAccount = nullptr;

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

                    if (loginControl(acc_no, password)) {
                        for (auto& account : registeredAccounts) {
                            if (account.getAccountNumber() == acc_no) {
                                cout << "Login Successful!" << endl;
                                cout << "Redirecting to homepage..." << endl;
                                system("cls");

                                loggedInAccount = &account;
                                showHomepage(*loggedInAccount);
                                break;
                            }
                        }
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

                BankAccount newAccount(name, surname, birthYear, email, password, accno);
                registeredAccounts.push_back(newAccount);

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

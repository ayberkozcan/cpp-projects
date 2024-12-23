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

public:
    BankAccount(string n, string s, string b, string e, string p, string a)
        : name(n), surname(s), birthYear(b), email(e), password(p), accno(a), balance(0) {}

    string getName() const { return name; }
    string getSurname() const { return surname; }
    string getBirthYear() const { return birthYear; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getAccountNumber() const { return accno; }
    int getBalance() const { return balance; }

    void addBalance(int amount) { balance += amount; }
    void withdrawBalance(int amount) { balance -= amount; }
};

void showHomepage(BankAccount& account);
void viewBalance(BankAccount& account);
void addMoney(BankAccount& account);
void withdrawMoney(BankAccount& account);

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
        cout << "-------------------\nHomepage\n" << endl;
        cout << "\nChoose action below" << endl;
        cout << "(1) View Balance" << endl;
        cout << "(2) Add Money" << endl;
        cout << "(3) Withdraw Money" << endl;
        cout << "(4) Quit" << endl;
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
            withdrawMoney(account);
            break;
        case 4:
            exit(0);
            break;
        default:
            cout << "Invalid action. Please select a valid option.\n" << endl;
        }
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
    BankAccount* loggedInAccount = nullptr;

    while (true) {
        cout << "Welcome!" << endl;
        cout << "(1) Login" << endl;
        cout << "(2) Create an Account" << endl;

        cin >> action;

        switch(action) {
            case 1: {
                string acc_no, password;
                cout << "Account Number: ";
                cin >> acc_no;
                cout << "Password: ";
                cin >> password;

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
                    cout << "Invalid credentials, please try again!" << endl;
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
                    if (name.length() < 3 || name.length() >30) {
                        cout << "Please enter a valid name (between 3 and 30 characters)!" << endl;
                    } else {
                        break;
                    }
                }
                cout << "Surname: ";
                while(true) {
                    cin >> surname;
                    if (surname.length() < 2 || surname.length() >30) {
                        cout << "Please enter a valid surname (between 2 and 30 characters)!" << endl;
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

                    if (stoi(birthYear) < 1900 || stoi(birthYear) > currentYear - 18) {
                        cout << "Please enter a valid birth year (between 1900 and " << currentYear - 18 << ")!" << endl;
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
                    if (password.length() < 5 || password.length() >20) {
                        cout << "Please enter a valid name (between 5 and 20 characters)!" << endl;
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

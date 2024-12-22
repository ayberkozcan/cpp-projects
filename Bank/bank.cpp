#include <iostream>
#include <limits>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>

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
        cout << "-------------------\nHomepage" << endl;
        cout << "Your Balance: " << account.getBalance() << endl;
        cout << "Choose action below" << endl;
        cout << "(1) Add Money" << endl;
        cout << "(2) Withdraw Money" << endl;
        cout << "(3) Quit" << endl;
        cin >> action;

        switch (action)
        {
        case 1:
            addMoney(account);
            break;
        case 2:
            withdrawMoney(account);
            break;
        case 3:
            cout << "Exiting..." << endl;
            exit(0);
            break;
        default:
            cout << "Invalid action. Please select a valid option." << endl;
        }
    }
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
                string name, surname, birthYear, email, password;
                cout << "Create an Account" << endl;
                cout << "Name: ";
                cin >> name;
                cout << "Surname: ";
                cin >> surname;
                cout << "Birth Year: ";
                cin >> birthYear;
                cout << "Email: ";
                cin >> email;
                cout << "Password: ";
                cin >> password;

                srand(time(nullptr));
                string accno = to_string(10000 + rand() % 90000);

                BankAccount newAccount(name, surname, birthYear, email, password, accno);
                registeredAccounts.push_back(newAccount);

                cout << "\nAccount created successfully!" << endl;
                cout << "\nAccount Number: " << newAccount.getAccountNumber() << endl;
                break;
            }

            default:
                cout << "Invalid choice, please try again!" << endl;
        }
    }

    return 0;
}

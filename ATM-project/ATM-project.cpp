#include <iostream>
#include <string>
using namespace std;

const int Max_Attempts = 3;
const int Max_Withdrawal = 2000;
const int Max_Transfer = 10000;
string nameInput;

struct Client {
    string name;
    string pin;
    double balance;
};
string clients_Names[10];
string clients_pins[10];
double clients_Balances[10];
int client_count = 0;

void New_Client();
bool login();
void displayMenu();
void balanceCheck();
void Money_Transfer();
void withdraw();

int main() {
    int option;
    int choice;
    cout << "Welcome to the ATM System!\n";
    while (true) {
        cout << "\n1. Activate New Client\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if (choice == 1)
        {
            New_Client();
        }
        else if (choice == 2)
        {
            if (login()) {
                while (true)
                {
                    displayMenu();
                    cin >> option;
                    switch (option)
                    {
                    case 1:
                        balanceCheck();
                        break;
                    case 2:
                        withdraw();
                        break;
                    case 3:
                        Money_Transfer();
                        break;
                    case 4:
                        cout << "Logged out successfully.\n";
                        return 0;
                    default:
                        cout << "Invalid option. Please try again.\n";
                    }
                }
            }
        }
        else if (choice == 3)
        {
            cout << "Exiting the program. Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}

void New_Client() {
    if (client_count >= 10)
    {
        cout << "Maximum number of clients reached. Cannot add more clients.\n";
        return;
    }

    Client new_client;
    string pin_confirmation;
    cin.ignore();
    cout << "Enter client name: ";
    getline(cin, new_client.name);
    for (int i = 0; i < 10; i++)
    {
        if (new_client.name == clients_Names[i])
        {
            cout << "This username is already in use\n";
            return;
        }
    }
    do
    {
        do
        {
            cout << "Enter 4-digit PIN: ";
            cin >> new_client.pin;
            if (new_client.pin.length() != 4 || !isdigit(new_client.pin[0]))
            {
                cout << "You must enter exactly 4 numeric digits.\n";
            }
        } while (new_client.pin.length() != 4 || !isdigit(new_client.pin[0]));

        cout << "Confirm PIN: ";
        cin >> pin_confirmation;

        if (new_client.pin != pin_confirmation)
        {
            cout << "PINs do not match. Try again.\n";
        }
    } while (new_client.pin != pin_confirmation);

    cout << "Enter initial balance: $";
    cin >> new_client.balance;

    clients_Names[client_count] = new_client.name;
    clients_pins[client_count] = new_client.pin;
    clients_Balances[client_count] = new_client.balance;
    client_count++;

    cout << "Client activated successfully!\n";

}

bool login()
{
    bool userFound = false;
    string pinInput;
    int attempts = 0;
    cin.ignore();
    cout << "Enter your name: ";
    getline(cin, nameInput);

    for (int i = 0; i < client_count; i++)
    {
        if (nameInput == clients_Names[i])
        {
            userFound = true;
            break;
        }
    }

    if (!userFound)
    {
        cout << "There is no user with this username\n";
        cout << userFound;
        return false;
    }
    while (attempts < Max_Attempts)
    {
        cout << "Enter your 4-digit PIN: ";
        cin >> pinInput;

        for (int i = 0; i < client_count; i++)
        {
            if (clients_Names[i] == nameInput && clients_pins[i] == pinInput)
            {
                cout << "Login successful! Welcome, " << clients_Names[i] << ".\n";
                return true;
            }
        }
        attempts++;
        cout << "Invalid name or PIN. " << (Max_Attempts - attempts) << " attempts left.\n";
    }

    cout << "Account locked. Please contact the call center.\n";
    return false;
}

void displayMenu()
{
    cout << "\n1. Balance Check\n";
    cout << "2. Withdraw\n";
    cout << "3. Money Transfer\n";
    cout << "4. Logout\n";
    cout << "Enter your option: ";
}

void balanceCheck()
{
    string client_pin;
    int client_index = -1;
    for (int i = 0; i < client_count; ++i)
    {
        if (clients_Names[i] == nameInput)
        {
            client_index = i;
            break;
        }
    }
    if (client_index == -1)
    {
        cout << "client not found. Please log in again.\n";
        return;
    }
    cout << "Enter pin to confirm:";
    cin >> client_pin;
    if (client_pin == clients_pins[client_index])
    {
        cout << clients_Names[client_index] << " balance: $" << clients_Balances[client_index] << "\n";
    }
    else
    {
        cout << "Pin incorrect....Please try again\n";
        return;
    }
}
void Money_Transfer()
{
    static int transfer_count = 0;

    if (client_count < 2)
    {
        cout << "Not enough clients for money transfer.\n";
        return;
    }

    if (transfer_count >= Max_Attempts)
    {
        cout << "You have reached the maximum transfer limit of " << Max_Attempts << " transfers per day.\n";
        return;
    }

    string receiver_name;
    double amount;
    string sender_pin;

    int sender_index = -1;
    for (int i = 0; i < client_count; ++i)
    {
        if (clients_Names[i] == nameInput)
        {
            sender_index = i;
            break;
        }
    }

    if (sender_index == -1)
    {
        cout << "Sender not found. Please log in again.\n";
        return;
    }

    cout << "Enter receiver's name: ";
    cin >> receiver_name;

    int receiver_index = -1;
    for (int i = 0; i < client_count; ++i)
    {
        if (clients_Names[i] == receiver_name)
        {
            receiver_index = i;
            break;
        }
    }

    if (receiver_index == -1)
    {
        cout << "Receiver not found.\n";
        return;
    }

    cout << "Enter transfer amount: ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Amount must be greater than 0.\n";
        return;
    }

    if (amount > Max_Transfer)
    {
        cout << "Amount exceeds the maximum transfer limit of " << Max_Transfer << ".\n";
        return;
    }

    if (amount > clients_Balances[sender_index])
    {
        cout << "Insufficient balance.\n";
        return;
    }
    cout << "Enter pin to confirm:";
    cin >> sender_pin;
    if (sender_pin == clients_pins[sender_index])
    {
        clients_Balances[sender_index] -= amount;
        clients_Balances[receiver_index] += amount;
        transfer_count++;

        cout << "Transfer successful!\n";
        cout << clients_Names[sender_index] << " new balance: $" << clients_Balances[sender_index] << "\n";
        cout << "Transfers made today: " << transfer_count << " out of " << Max_Attempts << " allowed transfers.\n";
    }
    else
    {
        cout << "Pin incorrect....Please try again\n";
        return;
    }
}
void withdraw()
{
    int amount = 0;
    string client_pin;
    int client_index = -1;
    for (int i = 0; i < client_count; ++i)
    {
        if (clients_Names[i] == nameInput)
        {
            client_index = i;
            break;
        }
    }
    if (client_index == -1)
    {
        cout << "client not found. Please log in again.\n";
        return;
    }
    cout << "Enter the amount you want to withdraw: $";
    cin >> amount;


    if (amount <= 0)
    {
        cout << "Amount must be greater than 0.\n";
        return;
    }

    if (amount > Max_Withdrawal)
    {
        cout << "You can only withdraw up to $2000.\n";
        return;
    }

    if (amount > clients_Balances[client_index])
    {
        cout << "Insufficient balance.\n";
        return;
    }
    if (amount % 50 != 0)
    {
        cout << "Amount must be multiples of 50 ";
        return;
    }
    cout << "Enter pin to confirm:";
    cin >> client_pin;
    if (client_pin == clients_pins[client_index])
    {
        clients_Balances[client_index] -= amount;
        cout << clients_Names[client_index] << " withdrawed amount: $" << amount << "\n";
        cout << clients_Names[client_index] << " new balance: $" << clients_Balances[client_index] << "\n";
    }
    else
    {
        cout << "Pin incorrect....Please try again\n";
        return;
    }
}
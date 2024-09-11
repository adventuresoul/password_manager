#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include<stdexcept>
#include<vector>
#include<random>
#include<algorithm>
#include<sstream>
#include<time.h>

using namespace std;

// Base class for autentication and registration
class BaseServer{
    protected:
        // Authenticates a user based on the username in the "users.txt" file
        bool authenticator(const string& username) {
            ifstream file("users.txt");
            if (!file.is_open()) {
                throw runtime_error("Failed to open users.txt");
            }

            string user = username;
            for (string line; getline(file, line); ) {
                if (line == user) {
                    return true;
                }
            }
            throw runtime_error("User not found");
        }

        // Registers a new user by adding their username to the "users.txt" file
        bool new_user_registration() {
            ofstream file("users.txt", ios::app);
            if (!file.is_open()) {
                throw runtime_error("Failed to open users.txt for writing");
            }

            string username;
            cout << "Enter your username:";
            cin >> username;
            file << username << "\n";
            return true;
        }   
};

// Class for password generation and related functions
class Generator : public BaseServer{
    private:
        // Vectors to store characters for password generation
        vector <string> lower_case = {
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m","n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
        };
        vector <string> upper_case = {
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M","N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
        };
        vector <string> numbers = {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
        };
        vector <string> special_charecters = {
             "!", "@", "#", "$", "%", "^", "&", "*", ":", ";", "~", "=", "/", "|"
        };

        // Functions to get random characters from each category
        string get_lowercase(){
            return lower_case[rand() % lower_case.size()];
        }
        string get_uppercase(){
            return upper_case[rand() % upper_case.size()];
        }
        string get_numbers(){
            return numbers[rand() % numbers.size()];
        }
        string get_special_charecters(){
            return special_charecters[rand() % special_charecters.size()];
        }
    public:
        // Generates a random password and offers to save it to a file
        void password_generator(){
            string password;
            for (int i = 0; i < 12; i++){
                string choose;;
                int choice = rand() % 4;
                switch(choice){
                    case 0:
                        choose = get_lowercase();
                        break;
                    case 1:
                        choose = get_uppercase();
                        break;
                    case 2:
                        choose = get_numbers();
                        break;
                    case 3:
                        choose = get_special_charecters();
                        break;
                }
                password += choose;
            }
            random_shuffle(password.begin(), password.end());
            cout << "Generating password..." << endl;
            cout << password << endl;
            string option;
            cout << "Do you want to save the password? (y/n): ";
            cin >> option;
            if(option == "y"){
                string website;
                cout << "Enter the website name: " << endl;
                cin >> website;
                ofstream file("passwords.csv", ios::app);
                if (file.is_open()){
                    file << website << "," << password << endl;
                    cout << "password saved successfully" << endl;
                    file.close();
                    exit(0);
                }
                else{
                    throw runtime_error("Failed to open passwords.csv for writing");
                }  
            }
            else{
                    cout << "Thank you..." << endl;
                    exit(0);
            }
        }
};  

// Class for password recovery
class Recovery : public Generator{
    public:
        // Recovers a password based on the website name from the "passwords.csv" file
        void recover_password(){
            string website;
            cout << "Enter the website name: "<< endl;
            cin >> website;
            ifstream file("passwords.csv");
            if (!file.is_open()) {
                throw runtime_error("Failed to open passwords.csv");
                return;
            }
            string password;
            bool found = false;
            string line;
            while(getline(file, line)){
                vector <string> row;
                stringstream ss(line);
                string pair;
                while(getline(ss, pair, ',')){
                    row.push_back(pair);
                }

                if(row.size() >= 2 && row[0] == website){
                    cout << "password found! -> " << row[1] << endl;
                    found = true;
                    exit(0);
                    break;
                }
            }
            if (!found){
                string option;
                cout << "password not found!" << endl;
                cout << "Do you want to generate a new password? (y/n): ";
                cin >> option;
                if(option == "y"){
                    password_generator();
                    cout << "Password generated and saved successfully" << endl;
                }
                else{
                    cout << "Thank you..." << endl;
                    exit(0);
                }
            }
        }
};

// Class for error handling and retrying failed operations
class Error_handler : public BaseServer{
    protected:
        // Handles authentication errors by retrying a limited number of times
        bool error_handling_authenticator(int limit){
            while(limit > 0){
                try{
                    string username;
                    cout << "Enter username: " << endl;
                    cin >> username;
                    if(authenticator(username) != false){
                        cout << "Login successful" << endl;
                        return true;
                        break;
                    }
                    else{
                        throw "Invalid username...";
                    }
                }
                catch(...){
                    cout << "Please try again" << endl;
                    limit--;
                }
            }

        }

        // Handles new user registration errors by retrying a limited number of times
        bool error_handling_new_user(int limit){
            while(limit > 0){
                try{
                    if(new_user_registration() != false){
                        cout << "Regestration successful" << endl;
                        return true;
                        break;
                    }
                    else{
                        throw "Invalid username...";
                    }
                }
                catch(...){
                    cout << "Please try again" << endl;
                    limit--;
                }
            }

        }
};

// Main class for the password manager application
class Client : public Recovery, public Error_handler{
    public:
        // Runs the main application loop
        void run_application(){
            cout << "Welcome to password manager: " << endl;
            cout << "-----------------------------" << endl;
            cout << "1. Existing user: " << endl;
            cout << "2. New user: " << endl;
            cout << "3. Exit. " << endl;
            int option;
            while (true){
                cin >> option;
                if(option == 1){
                    cout << "\nExisting user ? please login: " << endl;
                    cout << "--------------------------------" << endl;
                    if(error_handling_authenticator(5) != false){
                        cout << "1. Generate password: " << endl;
                        cout << "2. Recover password: " << endl;
                        cout << "3. Exit. " << endl;
                        int option;
                        while (true){
                            cin >> option;
                            if(option == 1){
                                cout << "Generating password..." << endl;
                                password_generator();
                                break;
                            }
                            else if(option == 2){
                                cout << "Recovering password..." << endl;
                                recover_password();
                                break;
                            }
                            else if(option == 3){
                                cout << "Exiting the application...Thank you" << endl;
                                exit(0);
                            }
                        }
                    }
                    
                }
                else if(option == 2){
                    cout << "\nNew user ? please register: " << endl;
                    cout << "--------------------------------" << endl;
                    if(error_handling_new_user(5) != false){
                        cout << "1. Generate password: " << endl;
                        cout << "2. Recover password: " << endl;
                        cout << "3. Exit. " << endl;
                        int option;
                        while (true){
                            cin >> option;
                            if(option == 1){
                                cout << "Generating password..." << endl;
                                password_generator();
                                break;
                            }
                            else if(option == 2){
                                cout << "Recovering password..." << endl;
                                recover_password();
                                break;
                            }
                            else if(option == 3){
                                cout << "Exiting the application...Thank you" << endl;
                                exit(0);
                            }
                        }
                    }
                }
                else if(option == 3){
                    cout << "Exiting the application...Thank you" << endl;
                    exit(0);
                }
            }
            
        }
};

int main()
{
    srand(time(nullptr));
    Client app;
    app.run_application();
    return 0;
}

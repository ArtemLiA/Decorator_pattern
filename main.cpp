#include <iostream>
#include <algorithm>

//Function for checking if all characters are letters
bool containsOnlyLetters(std::string const &str) {
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos;
}


//Abstract component
class PasswordCheck{
public:
    virtual ~PasswordCheck() = default;
    virtual bool IsCorrect(std::string password) = 0;
    virtual void printConditions() = 0;
};

//Concrete component
class AccessPasswordCheck: public PasswordCheck {
public:
    bool IsCorrect(std::string password) {
        return (password.length() >= 10);
    }
    void printConditions(){
        std::cout << "At least 10 characters" << std::endl;
    }
};

//Abstract decorator
class PasswordCheckDecorator: public PasswordCheck{
protected:
    PasswordCheck* pc = nullptr;
public:
    virtual ~PasswordCheckDecorator(){delete pc;};
};

//Concrete decorator one
class Condition1: public PasswordCheckDecorator{
public:
    explicit Condition1(PasswordCheck* pass_check){
        pc = pass_check;
    }
    void printConditions(){
        std::cout << "Presence of one non-alphabetic character" << std::endl;
        pc->printConditions();
    }
    bool IsCorrect(std::string password){
        return (!containsOnlyLetters(password)) && pc->IsCorrect(password);
    }
};

//Concrete decorator two
class Condition2: public PasswordCheckDecorator{
public:
    explicit Condition2(PasswordCheck* pass_check){
        pc = pass_check;
    }
    void printConditions(){
        std::cout << "The presence of a number in the password" << std::endl;
        pc->printConditions();
    }
    bool IsCorrect(std::string password){
        return std::any_of(password.begin(), password.end(), ::isdigit) && pc->IsCorrect(password);
    }
};

//Concrete decorator three
class Condition3: public PasswordCheckDecorator{
public:
    explicit Condition3(PasswordCheck* pass_check){
        pc = pass_check;
    }
    void printConditions(){
        std::cout << "The presence of upper and lower case letters" << std::endl;
        pc->printConditions();
    }
    bool IsCorrect(std::string password){
        return std::any_of(password.begin(), password.end(), ::isupper) &&
        std::any_of(password.begin(), password.end(), ::islower) &&
        pc->IsCorrect(password);
    }
};


int main() {
    std::string pass1 = "AAbc45*eqwewq=";
    std::string pass2 = "it is a bad password";
    std::string pass3 = "4 5 6 8 7 9 1 3AAAA";
    std::string pass4 = "ABcd78";

    PasswordCheck* password_check1 = new AccessPasswordCheck();
    password_check1 = new Condition1(password_check1);
    password_check1 = new Condition2(password_check1);
    password_check1 = new Condition3(password_check1);

    std::cout << password_check1->IsCorrect(pass1) << std::endl;
    std::cout << password_check1->IsCorrect(pass2) << std::endl;
    std::cout << password_check1->IsCorrect(pass3) << std::endl;
    std::cout << password_check1->IsCorrect(pass4) << std::endl;

    password_check1->printConditions();

    delete password_check1;
    return 0;
}

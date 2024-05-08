#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

// Funkcja do generowania loginu na podstawie podanego slowa
string generateUsername(int length, const string& word) {
    string username = word;
    const string consonants = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
    const string digits = "0123456789";

    srand(time(0));
    for (int i = word.length(); i < length; ++i) {
        if (rand() % 2 == 0) {
            username += consonants[rand() % consonants.length()];
        } else {
            username += digits[rand() % digits.length()];
        }
    }

    return username;
}

// Funkcja do generowania hasla na podstawie wybranych rodzajow znakow
string generatePassword(int length, bool useLowercase, bool useUppercase, bool useSpecialChars) {
    string password;
    string charset = "";

    if (useLowercase) charset += "abcdefghijklmnopqrstuvwxyz";
    if (useUppercase) charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (useSpecialChars) charset += "!@#$%^&*()_+-=[]{}|;:,.<>?";

    srand(time(0));
    for (int i = 0; i < length; ++i) {
        password += charset[rand() % charset.length()];
    }

    return password;
}

// Funkcja do zapisywania loginu i hasla do pliku
void saveCredentials(const string& username, const string& password) {
    ofstream file("credentials.txt", ios::app);
    if (file.is_open()) {
        file << username << " " << password << endl;
        file.close();
    } else {
        cout << "Unable to open file!" << endl;
    }
}

// Funkcja do szyfrowania hasla
string encryptPassword(const string& password) {
    string encryptedPassword = password;

    // Szyfrowanie hasla w terminalu poprzez zastapienie kazdego znaku hashem #
    for (char& c : encryptedPassword) {
        if (c != ' ') {
            c = '#';
        }
    }

    return encryptedPassword;
}

// Funkcja do obliczania sily hasla
int passwordStrength(const string& password) {
    int length = password.length();
    int uppercaseScore = 0;
    int lowercaseScore = 0;
    int specialCharsScore = 0;

    for (char c : password) {
        if (isupper(c)) {
            uppercaseScore = 1;
        } else if (islower(c)) {
            lowercaseScore = 1;
        } else if (!isalnum(c)) {
            specialCharsScore = 1;
        }
    }

    // Bazowa wartosc sily hasla dla hasla o dlugosci 8
    int baseStrength = 6;

    // Korygowanie wartosci sily hasla na podstawie dlugosci i rodzaju znakow
    int lengthCorrection = 0;
    if (length == 7) {
        lengthCorrection = -1;
    } else if (length == 6) {
        lengthCorrection = 1;
    }

    int uppercaseCorrection = uppercaseScore ? 0 : -1;
    int lowercaseCorrection = lowercaseScore ? 0 : -1;
    int specialCharsCorrection = specialCharsScore ? 0 : -1;

    int strength = baseStrength + lengthCorrection + uppercaseCorrection + lowercaseCorrection + specialCharsCorrection;
    return max(1, strength); // Sila hasla nie moze byc mniejsza niz 1
}

// Funkcja do sprawdzania poprawnosci wprowadzanych danych liczbowych
int getValidNumberInput(int min, int max, const string& message) {
    int input;
    bool validInput = false;
    while (!validInput) {
        cout << message;
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cout << "Nieprawidlowa wartosc! Podaj liczbe miedzy " << min << " a " << max << "." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        } else {
            validInput = true;
        }
    }
    return input;
}

// Funkcja do sprawdzania poprawnosci wprowadzanych danych logicznych
bool getValidBoolInput(const string& message) {
    char input;
    bool validInput = false;
    while (!validInput) {
        cout << message;
        cin >> input;
        if (input == '1' || input == '0') {
            validInput = true;
        } else {
            cout << "Nieprawidlowa wartosc! Wpisz 1 dla Tak lub 0 dla Nie." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    return input == '1';
}

int main() {
    const int MIN_USERNAME_LENGTH = 4;
    const int MAX_USERNAME_LENGTH = 16;
    const int MIN_PASSWORD_LENGTH = 6;
    const int MAX_PASSWORD_LENGTH = 32;

    char repeatGeneration;
	
	
    do {
        int usernameLength, passwordLength;
        string word;
        bool useLowercase, useUppercase, useSpecialChars, useRepeat;

        usernameLength = getValidNumberInput(MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH, "Podaj dlugosc loginu (od " + to_string(MIN_USERNAME_LENGTH) + " do " + to_string(MAX_USERNAME_LENGTH) + "): ");

        cout << "Podaj slowo, ktore ma byc czescia loginu: ";
        cin >> word;

        passwordLength = getValidNumberInput(MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH, "Podaj dlugosc hasla (od " + to_string(MIN_PASSWORD_LENGTH) + " do " + to_string(MAX_PASSWORD_LENGTH) + "): ");

        useLowercase = getValidBoolInput("Czy uzywac malych liter? (1 - Tak, 0 - Nie): ");
        useUppercase = getValidBoolInput("Czy uzywac duzych liter? (1 - Tak, 0 - Nie): ");
        useSpecialChars = getValidBoolInput("Czy uzywac znakow specjalnych? (1 - Tak, 0 - Nie): ");

        string username = generateUsername(usernameLength, word);
        string password = generatePassword(passwordLength, useLowercase, useUppercase, useSpecialChars);

        saveCredentials(username, password);

        cout << "Utworzono nastepujace dane logowania:" << endl;
        cout << "Login: " << username << endl;
        cout << "Haslo: ";
        for (int i = 0; i < password.length(); ++i) {
            cout << "#";
        }
        cout << endl;

        // Sila hasla na podstawie hasla z pliku
        ifstream file("credentials.txt");
        string storedPassword;
        if (file.is_open()) {
            string tempUsername;
            while (file >> tempUsername >> storedPassword) {}
            file.close();
        } else {
            cout << "Unable to open file!" << endl;
            return 1;
        }

        cout << "Sila hasla: " << passwordStrength(storedPassword) << " / 10" << endl;
		do{
			
      		cout << ("Czy chcesz wygenerowac kolejne haslo? (T/N): ");
			cin >> repeatGeneration;
			
		}while(repeatGeneration != 'T' && repeatGeneration != 'N');
		
    } while (repeatGeneration == 'T');

    return 0;
}
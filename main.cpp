#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

/**
 * @brief Prosta implementacja SHA1
 */
class SHA1 {
public:
    SHA1() { reset(); }
    void update(const string& s) { update(s.c_str(), s.size()); }
    string final();

private:
    void reset();
    void transform(const unsigned char* data);
    void update(const char* data, size_t len);

    static const size_t blockSize = 64;
    unsigned int h0, h1, h2, h3, h4;
    unsigned char buffer[blockSize];
    size_t bufferLength;
    uint64_t bitLength;
};

void SHA1::reset() {
    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
    h4 = 0xC3D2E1F0;
    bufferLength = 0;
    bitLength = 0;
}

void SHA1::update(const char* data, size_t len) {
    bitLength += len * 8;
    while (len--) {
        buffer[bufferLength++] = *data++;
        if (bufferLength == blockSize) {
            transform(buffer);
            bufferLength = 0;
        }
    }
}

void SHA1::transform(const unsigned char* data) {
    unsigned int a, b, c, d, e, f, k, temp;
    unsigned int w[80];

    for (int i = 0; i < 16; ++i)
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);

    for (int i = 16; i < 80; ++i)
        w[i] = (w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]) << 1;

    a = h0;
    b = h1;
    c = h2;
    d = h3;
    e = h4;

    for (int i = 0; i < 80; ++i) {
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        temp = (a << 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = b << 30;
        b = a;
        a = temp;
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
}

string SHA1::final() {
    unsigned char hash[20];
    size_t i = bufferLength;

    if (bufferLength < 56) {
        buffer[i++] = 0x80;
        while (i < 56)
            buffer[i++] = 0x00;
    } else {
        buffer[i++] = 0x80;
        while (i < 64)
            buffer[i++] = 0x00;
        transform(buffer);
        memset(buffer, 0, 56);
    }

    bitLength += bufferLength * 8;
    buffer[63] = bitLength;
    buffer[62] = bitLength >> 8;
    buffer[61] = bitLength >> 16;
    buffer[60] = bitLength >> 24;
    buffer[59] = bitLength >> 32;
    buffer[58] = bitLength >> 40;
    buffer[57] = bitLength >> 48;
    buffer[56] = bitLength >> 56;
    transform(buffer);

    for (i = 0; i < 4; ++i) {
        hash[i] = (h0 >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (h1 >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (h2 >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (h3 >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (h4 >> (24 - i * 8)) & 0x000000ff;
    }

    stringstream ss;
    for (i = 0; i < 20; ++i)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}

/**
 * @brief Funkcja do generowania loginu na podstawie podanego slowa
 * 
 * @param length Dlugosc loginu
 * @param word Slowo, ktore ma byc czescia loginu
 * @return string Wygenerowany login
 */
string generateUsername(int length, const string& word) {
    string username = word;
    const string consonants = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
    const string digits = "0123456789";

    for (int i = word.length(); i < length; ++i) {
        if (rand() % 2 == 0) {
            username += consonants[rand() % consonants.length()];
        } else {
            username += digits[rand() % digits.length()];
        }
    }

    return username;
}

/**
 * @brief Funkcja do generowania hasla na podstawie wybranych rodzajow znakow
 * 
 * @param length Dlugosc hasla
 * @param useLowercase Czy uzywac malych liter
 * @param useUppercase Czy uzywac duzych liter
 * @param useSpecialChars Czy uzywac znakow specjalnych
 * @param useDigits Czy uzywac cyfr
 * @return string Wygenerowane haslo
 */
string generatePassword(int length, bool useLowercase, bool useUppercase, bool useSpecialChars, bool useDigits) {
    string password;
    string charset = "";

    if (useLowercase) charset += "abcdefghijklmnopqrstuvwxyz";
    if (useUppercase) charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (useSpecialChars) charset += "!@#$%^&*()_+-=[]{}|;:,.<>?";
    if (useDigits) charset += "0123456789";

    if (charset.empty()) {
        cout << "Musisz wybrac przynajmniej jeden rodzaj znakow!" << endl;
        return "";
    }

    for (int i = 0; i < length; ++i) {
        password += charset[rand() % charset.length()];
    }

    return password;
}

/**
 * @brief Funkcja do zapisywania loginu i hasla do pliku
 * 
 * @param username Wygenerowany login
 * @param password Wygenerowane haslo
 */
void saveCredentials(const string& username, const string& password) {
    SHA1 sha1;
    sha1.update(password);
    string hashedPassword = sha1.final();

    ofstream file("credentials.txt", ios::app);
    if (file.is_open()) {
        file << username << " " << hashedPassword << endl;
        file.close();
    } else {
        cout << "Nie mozna otworzyc pliku!" << endl;
    }
}

/**
 * @brief Funkcja do obliczania sily hasla
 * 
 * @param password Haslo do oceny
 * @return int Sila hasla w skali 1-10
 */
int passwordStrength(const string& password) {
    int length = password.length();
    int uppercaseScore = 0;
    int lowercaseScore = 0;
    int specialCharsScore = 0;
    int digitsScore = 0;

    for (char c : password) {
        if (isupper(c)) {
            uppercaseScore = 1;
        } else if (islower(c)) {
            lowercaseScore = 1;
        } else if (!isalnum(c)) {
            specialCharsScore = 1;
        } else if (isdigit(c)) {
            digitsScore = 1;
        }
    }

    // Bazowa wartosc sily hasla dla hasla o dlugosci 8
    int baseStrength = 6;

    // Korygowanie sily hasla na podstawie dlugosci
    baseStrength += (length > 8) ? 2 : -2;

    return baseStrength + uppercaseScore + lowercaseScore + specialCharsScore + digitsScore;
}

/**
 * @brief Funkcja do pobierania poprawnej wartosci liczbowej z podanego zakresu
 * 
 * @param min Minimalna wartosc
 * @param max Maksymalna wartosc
 * @param message Wiadomosc wyswietlana uzytkownikowi
 * @return int Poprawna wartosc liczbowa
 */
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

/**
 * @brief Funkcja do pobierania poprawnej wartosci logicznej
 * 
 * @param message Wiadomosc wyswietlana uzytkownikowi
 * @return bool Poprawna wartosc logiczna
 */
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

/**
 * @brief Funkcja do ustawienia ziarna dla rand() na podstawie uzytkownika
 */
void setRandomSeed() {
    string seed;
    cout << "Podaj ziarno do generowania losowych wartosci: ";
    cin >> seed;
    unsigned int hash = 0;
    for (char c : seed) {
        hash = hash * 101 + c;
    }
    srand(hash);
}

/**
 * @brief Funkcja do sprawdzania, czy uzytkownik chce kontynuowac
 * 
 * @return bool Czy uzytkownik chce kontynuowac
 */
bool askToRepeat() {
    return getValidBoolInput("Czy chcesz wygenerowac kolejne dane logowania? (1 - Tak, 0 - Nie): ");
}

/**
 * @brief Funkcja glowna programu
 * 
 * @return int Kod wyjscia programu
 */
int main() {
    const int MIN_USERNAME_LENGTH = 4;
    const int MAX_USERNAME_LENGTH = 16;
    const int MIN_PASSWORD_LENGTH = 6;
    const int MAX_PASSWORD_LENGTH = 32;

    setRandomSeed();

    do {
        int usernameLength = 0, passwordLength;
        string word;
        bool useLowercase, useUppercase, useSpecialChars, useDigits;

        bool generateUsernameOption = getValidBoolInput("Czy chcesz wygenerowac login? (1 - Tak, 0 - Nie): ");
        if (generateUsernameOption) {
            usernameLength = getValidNumberInput(MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH, "Podaj dlugosc loginu (od " + to_string(MIN_USERNAME_LENGTH) + " do " + to_string(MAX_USERNAME_LENGTH) + "): ");

            cout << "Podaj slowo, ktore ma byc czescia loginu: ";
            cin >> word;
        }

        passwordLength = getValidNumberInput(MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH, "Podaj dlugosc hasla (od " + to_string(MIN_PASSWORD_LENGTH) + " do " + to_string(MAX_PASSWORD_LENGTH) + "): ");

        useLowercase = getValidBoolInput("Czy uzywac malych liter? (1 - Tak, 0 - Nie): ");
        useUppercase = getValidBoolInput("Czy uzywac duzych liter? (1 - Tak, 0 - Nie): ");
        useSpecialChars = getValidBoolInput("Czy uzywac znakow specjalnych? (1 - Tak, 0 - Nie): ");
        useDigits = getValidBoolInput("Czy uzywac cyfr? (1 - Tak, 0 - Nie): ");

        string username = generateUsernameOption ? generateUsername(usernameLength, word) : "";
        string password = generatePassword(passwordLength, useLowercase, useUppercase, useSpecialChars, useDigits);

        if (password.empty()) continue;

        saveCredentials(username, password);

        cout << "Utworzono nastepujace dane logowania:" << endl;
        if (generateUsernameOption) {
            cout << "Login: " << username << endl;
        }
        cout << "Haslo: " << password << endl;

        cout << "Sila hasla: " << passwordStrength(password) << " / 10" << endl;

    } while (askToRepeat());

    return 0;
}
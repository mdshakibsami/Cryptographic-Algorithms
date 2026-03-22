#include <iostream>
#include <string>
#include <cctype>

using namespace std;

/*
    Finds gcd and coefficients x, y such that:
              a*x + b*y = gcd(a, b)
*/
int extendedGCD(int a, int b, int &x, int &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int gcd = extendedGCD(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}

// Finds inverse of key under mod 26
int findModularInverse(int key, int mod)
{
    int x, y;
    int gcd = extendedGCD(key, mod, x, y);

    if (gcd != 1)
        return -1;

    return (x % mod + mod) % mod;
}

// Validates user key input
bool isValidKeyInput(string keyInput)
{
    if (keyInput.empty())
        return false;

    // If single letter
    if (keyInput.length() == 1 && isalpha(keyInput[0]))
        return true;

    // If number (all digits)
    for (char c : keyInput)
    {
        if (!isdigit(c))
            return false;
    }

    return true;
}

// Converts valid key input into integer
int convertKeyInput(string keyInput)
{
    if (keyInput.length() == 1 && isalpha(keyInput[0]))
    {
        return toupper(keyInput[0]) - 'A';
    }

    return stoi(keyInput);
}

// Encrypt plaintext
string encryptText(string text, int key)
{
    string result = "";
    key = key % 26;

    for (char ch : text)
    {
        if (isupper(ch))
            result += char((ch - 'A') * key % 26 + 'A');
        else if (islower(ch))
            result += char((ch - 'a') * key % 26 + 'a');
        else
            result += ch;
    }

    return result;
}

// Decrypt ciphertext
string decryptText(string text, int key)
{
    string result = "";

    int keyInverse = findModularInverse(key, 26);

    if (keyInverse == -1)
    {
        cout << "Error: Key has no modular inverse.\n";
        return "";
    }

    for (char ch : text)
    {
        if (isupper(ch))
            result += char((ch - 'A') * keyInverse % 26 + 'A');
        else if (islower(ch))
            result += char((ch - 'a') * keyInverse % 26 + 'a');
        else
            result += ch;
    }

    return result;
}

//  Main Function
int main()
{
    string text, keyInput;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter key (number or single letter): ";
    cin >> keyInput;

    // Validate key input
    if (!isValidKeyInput(keyInput))
    {
        cout << "Invalid key! Use only a number or a single letter.\n";
        return 0;
    }

    int key = convertKeyInput(keyInput);

    // Validate mathematical key condition
    if (findModularInverse(key, 26) == -1)
    {
        cout << "Invalid key! Key must be co-prime with 26.\n";
        return 0;
    }

    string encrypted = encryptText(text, key);
    cout << "\nEncrypted Text: " << encrypted << endl;

    string decrypted = decryptText(encrypted, key);
    cout << "Decrypted Text: " << decrypted << endl;

    return 0;
}
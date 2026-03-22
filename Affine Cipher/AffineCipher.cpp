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

// Validates user key input (single letter or number)
bool isValidKeyInput(string keyInput)
{
    if (keyInput.empty())
        return false;

    if (keyInput.length() == 1 && isalpha(keyInput[0]))
        return true;

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

// Encrypt plaintext using Affine Cipher
string encryptText(string text, int a, int b)
{
    string result = "";
    a = a % 26;
    b = b % 26;

    for (char ch : text)
    {
        if (isupper(ch))
            result += char((a * (ch - 'A') + b) % 26 + 'A');
        else if (islower(ch))
            result += char((a * (ch - 'a') + b) % 26 + 'a');
        else
            result += ch;
    }

    return result;
}

// Decrypt ciphertext using Affine Cipher
string decryptText(string text, int a, int b)
{
    string result = "";

    int aInverse = findModularInverse(a, 26);

    if (aInverse == -1)
    {
        cout << "Error: Key 'a' has no modular inverse.\n";
        return "";
    }

    b = b % 26;

    for (char ch : text)
    {
        if (isupper(ch))
            result += char((aInverse * ((ch - 'A' - b + 26) % 26)) % 26 + 'A');
        else if (islower(ch))
            result += char((aInverse * ((ch - 'a' - b + 26) % 26)) % 26 + 'a');
        else
            result += ch;
    }

    return result;
}

// Main Function
int main()
{
    string text, aInput, bInput;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter key 'a' (multiplicative, number or single letter): ";
    cin >> aInput;

    cout << "Enter key 'b' (additive, number or single letter): ";
    cin >> bInput;

    // Validate key inputs
    if (!isValidKeyInput(aInput) || !isValidKeyInput(bInput))
    {
        cout << "Invalid key! Use only a number or a single alphabet letter for each key.\n";
        return 0;
    }

    int a = convertKeyInput(aInput);
    int b = convertKeyInput(bInput);

    // Validate mathematical condition for a
    if (findModularInverse(a, 26) == -1)
    {
        cout << "Invalid key 'a'! Must be co-prime with 26.\n";
        return 0;
    }

    // Validate range for b
    if (b < 0 || b > 25)
    {
        cout << "Invalid key 'b' range! Must be between 0 and 25.\n";
        return 0;
    }

    string encrypted = encryptText(text, a, b);
    cout << "\nEncrypted Text: " << encrypted << endl;

    string decrypted = decryptText(encrypted, a, b);
    cout << "Decrypted Text: " << decrypted << endl;

    return 0;
}
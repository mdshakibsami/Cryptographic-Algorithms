#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Encrypt plaintext using Additive Cipher
string encrypt(string text, int key)
{
    string result = "";
    key = key % 26;

    for (int i = 0; i < text.length(); i++)
    {
        char ch = text[i];

        if (isupper(ch))
            result += char((ch - 'A' + key) % 26 + 'A');
        else if (islower(ch))
            result += char((ch - 'a' + key) % 26 + 'a');
        else
            result += ch;
    }

    return result;
}

// Decrypt ciphertext using Additive Cipher
string decrypt(string text, int key)
{
    string result = "";
    key = key % 26;

    for (int i = 0; i < text.length(); i++)
    {
        char ch = text[i];

        if (isupper(ch))
            result += char((ch - 'A' - key + 26) % 26 + 'A');
        else if (islower(ch))
            result += char((ch - 'a' - key + 26) % 26 + 'a');
        else
            result += ch;
    }

    return result;
}

// Convert key input to integer
int getKeyValue(string keyInput)
{
    if (keyInput.length() == 1 && isalpha(keyInput[0]))
        return toupper(keyInput[0]) - 'A';

    if (keyInput.length() > 0)
        return stoi(keyInput);

    return -1; // invalid case
}

// Validate key input

bool isValidKey(string keyInput)
{
    if (keyInput.empty())
        return false;

    if (keyInput.length() == 1 && isalpha(keyInput[0]))
        return true;

    for (char ch : keyInput)
    {
        if (!isdigit(ch))
            return false;
    }

    return true;
}

int main()
{
    string plaintext, keyInput;

    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    cout << "Enter key (number or single letter): ";
    cin >> keyInput;

    // Validate key
    if (!isValidKey(keyInput))
    {
        cout << "Invalid key! Use only a number or a single alphabet letter.\n";
        return 0;
    }

    int key = getKeyValue(keyInput);

    if (key < 0 || key > 25)
    {
        cout << "Invalid key range! Must be between 0 to 25.\n";
        return 0;
    }

    string ciphertext = encrypt(plaintext, key);

    cout << "\nEncrypted text: " << ciphertext << endl;
    cout << "Decrypted text: " << decrypt(ciphertext, key) << endl;

    return 0;
}
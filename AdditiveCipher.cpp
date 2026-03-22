#include <iostream>
#include <string>
#include <cctype> // for isalpha, isupper, islower
using namespace std;

/**
 * NOTE: Encryption: C = (P + k) mod 26         => (ch - 'A' + key + 26) % 26 + 'A'
 * NOTE: Decryption: P = (C - k + 26) mod 26    => (ch - 'A' - key + 26) % 26 + 'A'
 * NOTE: Number ✅, Character ✅, String(Not Additive Cipher) ❌
 */

/*
    Function: encrypt
    Purpose : Encrypt plaintext using Additive Cipher
    Input   : text (plaintext), key (shift value)
    Output  : encrypted ciphertext
*/

string encrypt(string text, int key)
{
    string result = "";

    // Normalize key so it stays within 0–25
    key = key % 26;

    // Loop through each character of the plaintext
    for (int i = 0; i < text.length(); i++)
    {
        char ch = text[i];

        // If character is uppercase letter
        if (isupper(ch))
        {
            /*
                Convert letter to 0–25 index
                Add the key
                Use mod 26 to wrap around alphabet
                Convert back to character
            */
            result += char((ch - 'A' + key) % 26 + 'A');
        }

        // If character is lowercase letter
        else if (islower(ch))
        {
            result += char((ch - 'a' + key) % 26 + 'a');
        }

        // If character is number or symbol
        else
        {
            // Leave unchanged
            result += ch;
        }
    }

    return result;
}

/*
    Function: decrypt
    Purpose : Decrypt ciphertext using Additive Cipher
*/
string decrypt(string text, int key)
{
    string result = "";

    // Normalize key
    key = key % 26;

    for (int i = 0; i < text.length(); i++)
    {
        char ch = text[i];

        if (isupper(ch))
        {
            /*
                Subtract the key instead of adding it
                Add 26 before mod to avoid negative values
            */
            result += char((ch - 'A' - key + 26) % 26 + 'A');
        }

        else if (islower(ch))
        {
            result += char((ch - 'a' - key + 26) % 26 + 'a');
        }

        else
        {
            // Numbers and symbols remain unchanged
            result += ch;
        }
    }

    return result;
}

/*
    Convert key input (string) to integer shift value
    Supports:
    - number (e.g., "3")
    - character (e.g., "D" or "d")
*/
int getKeyValue(string keyInput)
{
    // Case 1: If input is a single alphabet character
    if (keyInput.length() == 1 && isalpha(keyInput[0]))
    {
        char ch = toupper(keyInput[0]); // Convert to uppercase
        return ch - 'A';                // Convert to 0–25
    }

    // Case 2: If input is a number
    else
    {
        return stoi(keyInput); // Convert string → int
    }
}

/*
    Main Function
*/
int main()
{
    string plaintext;
    string keyInput;

    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    cout << "Enter key (number or single letter): ";
    cin >> keyInput;

    cin.ignore(); // Clear newline from input buffer

    int key = getKeyValue(keyInput);

    string ciphertext = encrypt(plaintext, key);

    cout << "\nEncrypted text: " << ciphertext << endl;
    cout << "Decrypted text: " << decrypt(ciphertext, key) << endl;

    return 0;
}
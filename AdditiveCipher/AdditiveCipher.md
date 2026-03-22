# Additive Cipher (Caesar Cipher) Guide for Beginners

## 1. Cryptography in Simple Words

Cryptography means protecting messages by changing them into a form that others cannot easily read.

- Plaintext: normal readable message
- Ciphertext: encrypted unreadable message

## 2. What Is an Additive Cipher?

In an additive cipher, every letter is shifted forward by a fixed number called a key.

Example with key 3:

- A becomes D
- B becomes E
- X becomes A (wraps around)

So:

- Plaintext: HELLO
- Key: 3
- Ciphertext: KHOOR

## 3. Math Behind the Cipher

Use alphabet positions where A is 0, B is 1, ..., Z is 25.

Encryption:

```text
C = (P + k) mod 26
```

Decryption:

```text
P = (C - k + 26) mod 26
```

`mod 26` keeps the result inside the alphabet range.

## 4. Valid Key Input Rules

| Input Type       | Example | Allowed |
| ---------------- | ------- | ------- |
| Number           | 5       | Yes     |
| Single letter    | D       | Yes     |
| Multiple letters | abc     | No      |
| Symbols          | @#      | No      |

If a single letter is used as key, it is converted as:

- A to 0
- B to 1
- D to 3

## 5. Function-by-Function Explanation

### 5.1 Function: encrypt

What it does:

- Converts plaintext into ciphertext by shifting each alphabetic character by key.

Code:

```cpp
string encrypt(string text, int key)
{
	// Store encrypted output here
	string result = "";
	// Normalize key so large values still work (e.g., 29 -> 3)
	key = key % 26;

	// Process each character one by one
	for (int i = 0; i < text.length(); i++)
	{
		char ch = text[i];

		if (isupper(ch))
			// Convert A-Z to 0-25, shift, wrap with mod, convert back to A-Z
			result += char((ch - 'A' + key) % 26 + 'A');
		else if (islower(ch))
			// Same logic for a-z
			result += char((ch - 'a' + key) % 26 + 'a');
		else
			// Keep spaces, digits, and symbols unchanged
			result += ch;
	}

	// Return the final encrypted text
	return result;
}
```

Important lines:

- `key = key % 26;` keeps key inside 0 to 25.
- `(ch - 'A' + key) % 26 + 'A'` shifts uppercase letters.
- `(ch - 'a' + key) % 26 + 'a'` shifts lowercase letters.
- Non-letters are copied unchanged.

### 5.2 Function: decrypt

What it does:

- Converts ciphertext back into plaintext.

Code:

```cpp
string decrypt(string text, int key)
{
	// Store decrypted output here
	string result = "";
	// Keep key inside valid alphabet range
	key = key % 26;

	// Process each character
	for (int i = 0; i < text.length(); i++)
	{
		char ch = text[i];

		if (isupper(ch))
			// Reverse shift for uppercase letters, +26 avoids negative values
			result += char((ch - 'A' - key + 26) % 26 + 'A');
		else if (islower(ch))
			// Reverse shift for lowercase letters
			result += char((ch - 'a' - key + 26) % 26 + 'a');
		else
			// Keep non-alphabetic characters unchanged
			result += ch;
	}

	// Return restored plaintext
	return result;
}
```

Important lines:

- `- key` reverses the shift.
- `+ 26` avoids negative values before modulo.

### 5.3 Function: getKeyValue

What it does:

- Converts key input text to an integer.

Code:

```cpp
int getKeyValue(string keyInput)
{
	if (keyInput.length() == 1 && isalpha(keyInput[0]))
		// Convert letter key to number (A->0, B->1, ..., Z->25)
		return toupper(keyInput[0]) - 'A';

	if (keyInput.length() > 0)
		// Convert numeric string key (e.g., "5") to integer 5
		return stoi(keyInput);

	return -1; // invalid case
}
```

Important lines:

- `toupper(keyInput[0]) - 'A'` turns a letter into its alphabet index.
- `stoi(keyInput)` converts numeric string to integer.
- `return -1` handles invalid empty input.

### 5.4 Function: isValidKey

What it does:

- Checks whether key input format is acceptable.

Code:

```cpp
bool isValidKey(string keyInput)
{
	// Empty key is invalid
	if (keyInput.empty())
		return false;

	// Single alphabet letter is valid
	if (keyInput.length() == 1 && isalpha(keyInput[0]))
		return true;

	// Otherwise, every character must be a digit
	for (char ch : keyInput)
	{
		if (!isdigit(ch))
			return false;
	}

	// All checks passed
	return true;
}
```

Important lines:

- `keyInput.empty()` prevents empty key.
- `isalpha(...)` allows a single letter key.
- Loop with `isdigit(...)` ensures full numeric key.

### 5.5 Function: main

What it does:

- Drives the complete program: input, validation, encryption, decryption, output.

Code:

```cpp
int main()
{
	string plaintext, keyInput;

	// Read full line so spaces are included
	cout << "Enter plaintext: ";
	getline(cin, plaintext);

	// Read key as string to support both number and letter input
	cout << "Enter key (number or single letter): ";
	cin >> keyInput;

	// Validate key
	if (!isValidKey(keyInput))
	{
		cout << "Invalid key! Use only a number or a single alphabet letter.\n";
		return 0;
	}

	// Convert validated key into integer form
	int key = getKeyValue(keyInput);

	// Additive cipher key must be between 0 and 25
	if (key < 0 || key > 25)
	{
		cout << "Invalid key range! Must be between 0 to 25.\n";
		return 0;
	}

	// Encrypt plaintext
	string ciphertext = encrypt(plaintext, key);

	// Show encrypted and decrypted results
	cout << "\nEncrypted text: " << ciphertext << endl;
	cout << "Decrypted text: " << decrypt(ciphertext, key) << endl;

	// Program finished successfully
	return 0;
}
```

Important lines:

- `isValidKey(...)` rejects invalid format early.
- `if (key < 0 || key > 25)` ensures valid additive range.
- Final two output lines demonstrate both encryption and successful decryption.

## 6. Error Handling Behavior

Invalid format example:

```text
abc
```

Program output:

```text
Invalid key! Use only a number or a single alphabet letter.
```

Invalid range example:

```text
30
```

Program output:

```text
Invalid key range! Must be between 0 to 25.
```

## 7. Key Concepts You Learn Here

- Modular arithmetic with 26 letters
- Character to number conversion using ASCII offsets
- Input validation
- Encryption and decryption as reverse processes

## 8. Quick Summary

Additive cipher is an easy first cipher to learn. It shifts each letter by a fixed key and uses modulo 26 so values stay inside A to Z. With proper key validation, the program can safely encrypt and decrypt text.

# Multiplicative Cipher Guide for Beginners

## 1. Cryptography in Simple Words

Cryptography is the process of hiding a readable message (plaintext) into an unreadable form (ciphertext), then recovering it when needed.

## 2. What Is a Multiplicative Cipher?

A multiplicative cipher changes each letter by multiplying its alphabet index with a key under modulo 26.

- A is 0, B is 1, ..., Z is 25
- Example operation: newValue = (oldValue x key) mod 26

Important rule:

- The key must be co-prime with 26, otherwise decryption will fail.

## 3. Math Behind the Cipher

Encryption:

```text
C = (P x key) mod 26
```

Decryption:

```text
P = (C x keyInverse) mod 26
```

Where keyInverse is the modular inverse of key.

## 4. Input Rules

| Input Type       | Example | Allowed |
| ---------------- | ------- | ------- |
| Number           | 5       | Yes     |
| Single letter    | D       | Yes     |
| Multiple letters | abc     | No      |
| Symbols          | @#      | No      |

## 5. Function-by-Function Explanation

### 5.1 Function: extendedGCD

What it does:

- Computes gcd and numbers x, y such that:

```text
a*x + b*y = gcd(a, b)
```

Code:

```cpp
int extendedGCD(int a, int b, int &x, int &y)
{
	// Base case: gcd(a, 0) = a
	if (b == 0)
	{
		// Coefficients for base case equation: a*1 + 0*0 = a
		x = 1;
		y = 0;
		return a;
	}

	// Recursive step on smaller pair (b, a % b)
	int x1, y1;
	int gcd = extendedGCD(b, a % b, x1, y1);

	// Back-substitute to compute x and y for current a, b
	x = y1;
	y = x1 - (a / b) * y1;

	// Return gcd(a, b)
	return gcd;
}
```

Important lines:

- Base case `if (b == 0)` ends recursion.
- Recursive call reduces problem size using `a % b`.
- Back-substitution updates x and y.

Why this matters:

- Needed to find modular inverse for decryption.

### 5.2 Function: findModularInverse

What it does:

- Finds inverse value such that:

```text
(key x inverse) mod 26 = 1
```

Code:

```cpp
int findModularInverse(int key, int mod)
{
	// x and y satisfy: key*x + mod*y = gcd(key, mod)
	int x, y;
	int gcd = extendedGCD(key, mod, x, y);

	// Inverse exists only when key and mod are co-prime
	if (gcd != 1)
		return -1;

	// Make inverse positive within range [0, mod-1]
	return (x % mod + mod) % mod;
}
```

Important lines:

- `if (gcd != 1) return -1;` means no inverse exists.
- `(x % mod + mod) % mod` ensures positive inverse.

### 5.3 Function: isValidKeyInput

What it does:

- Validates key format before conversion.

Code:

```cpp
bool isValidKeyInput(string keyInput)
{
	// Reject empty input
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

	// Valid numeric key string
	return true;
}
```

Important lines:

- Empty check avoids invalid states.
- Single alphabet key is allowed.
- Loop with `isdigit` enforces numeric-only multi-char key.

### 5.4 Function: convertKeyInput

What it does:

- Converts valid key input into integer form.

Code:

```cpp
int convertKeyInput(string keyInput)
{
	if (keyInput.length() == 1 && isalpha(keyInput[0]))
	{
		// Convert letter key to index (A->0, B->1, ..., Z->25)
		return toupper(keyInput[0]) - 'A';
	}

	// Convert numeric string key to integer
	return stoi(keyInput);
}
```

Important lines:

- `toupper(keyInput[0]) - 'A'` maps letter to 0 to 25.
- `stoi(keyInput)` converts numeric key.

### 5.5 Function: encryptText

What it does:

- Encrypts plaintext characters using multiplication and modulo.

Code:

```cpp
string encryptText(string text, int key)
{
	// Stores encrypted output
	string result = "";
	// Reduce large key values into 0-25
	key = key % 26;

	// Encrypt each character
	for (char ch : text)
	{
		if (isupper(ch))
			// Map A-Z to 0-25, multiply by key, wrap with mod, map back
			result += char((ch - 'A') * key % 26 + 'A');
		else if (islower(ch))
			// Same transformation for lowercase letters
			result += char((ch - 'a') * key % 26 + 'a');
		else
			// Keep spaces and symbols unchanged
			result += ch;
	}

	// Return ciphertext
	return result;
}
```

Important lines:

- `(ch - 'A') * key % 26 + 'A'` transforms uppercase letters.
- Lowercase branch handles small letters similarly.
- Non-letters are preserved.

### 5.6 Function: decryptText

What it does:

- Decrypts by multiplying with the modular inverse of the key.

Code:

```cpp
string decryptText(string text, int key)
{
	// Stores decrypted output
	string result = "";

	// Compute multiplicative inverse of key under mod 26
	int keyInverse = findModularInverse(key, 26);

	// If inverse does not exist, decryption cannot proceed
	if (keyInverse == -1)
	{
		cout << "Error: Key has no modular inverse.\n";
		return "";
	}

	// Decrypt each character using inverse key
	for (char ch : text)
	{
		if (isupper(ch))
			// Reverse multiplicative transformation for uppercase letters
			result += char((ch - 'A') * keyInverse % 26 + 'A');
		else if (islower(ch))
			// Reverse multiplicative transformation for lowercase letters
			result += char((ch - 'a') * keyInverse % 26 + 'a');
		else
			// Keep non-letters unchanged
			result += ch;
	}

	// Return recovered plaintext
	return result;
}
```

Important lines:

- `int keyInverse = findModularInverse(key, 26);` is the core decryption requirement.
- If inverse does not exist, function returns safely.

### 5.7 Function: main

What it does:

- Controls complete flow from input to final output.

Code:

```cpp
int main()
{
	string text, keyInput;

	// Read full text including spaces
	cout << "Enter text: ";
	getline(cin, text);

	// Read key as string to support both letter and number input
	cout << "Enter key (number or single letter): ";
	cin >> keyInput;

	// Validate key input
	if (!isValidKeyInput(keyInput))
	{
		cout << "Invalid key! Use only a number or a single letter.\n";
		return 0;
	}

	// Convert validated key to integer
	int key = convertKeyInput(keyInput);

	// Validate mathematical key condition
	if (findModularInverse(key, 26) == -1)
	{
		cout << "Invalid key! Key must be co-prime with 26.\n";
		return 0;
	}

	// Encrypt and display ciphertext
	string encrypted = encryptText(text, key);
	cout << "\nEncrypted Text: " << encrypted << endl;

	// Decrypt and display recovered plaintext
	string decrypted = decryptText(encrypted, key);
	cout << "Decrypted Text: " << decrypted << endl;

	// Program completed successfully
	return 0;
}
```

Important lines:

- Input validation prevents conversion errors.
- Co-prime check ensures decryptable key.
- Final print confirms encryption and decryption cycle.

## 6. Error Handling Behavior

Invalid key format:

```text
abc
```

Output:

```text
Invalid key! Use only a number or a single letter.
```

Invalid mathematical key example:

```text
13
```

Output:

```text
Invalid key! Key must be co-prime with 26.
```

## 7. Key Concepts You Learn Here

- Modular arithmetic
- Co-prime numbers and modular inverse
- Extended Euclidean algorithm
- Character shifting with ASCII offsets
- Input validation for safe programs

## 8. Quick Summary

Multiplicative cipher encrypts letters by multiplication under modulo 26. To decrypt, the key must have a modular inverse, so choosing a co-prime key is essential. The program is structured into small functions, making each step easier to understand and maintain.

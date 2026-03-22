# Affine Cipher Guide for Beginners

## 1. Cryptography in Simple Words

Cryptography is the process of hiding a readable message (plaintext) into an unreadable form (ciphertext), then recovering it when needed.

## 2. What Is an Affine Cipher?

An Affine Cipher combines both multiplication and addition to encrypt letters. It uses two keys: one for multiplication (like the multiplicative cipher) and one for addition (like the additive cipher). This makes it stronger than using just one method.

## 3. Math Behind the Cipher

Encryption:

```text
C = (a * P + b) mod 26
```

Decryption:

```text
P = a_inv * (C - b) mod 26
```

Where:
- P is the plaintext letter (0-25)
- C is the ciphertext letter (0-25)
- a is the multiplicative key (must be co-prime with 26)
- b is the additive key (0-25)
- a_inv is the modular inverse of a

## 4. Input Rules

| Input Type | Example | Allowed |
|------------|---------|---------|
| Number | 5 | Yes |
| Single letter | D | Yes |
| Multiple letters | abc | No |
| Symbols | @# | No |

Both keys 'a' and 'b' follow the same rules. 'a' must be co-prime with 26 (e.g., 1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25).

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

- Encrypts plaintext characters using both multiplication and addition.

Code:

```cpp
string encryptText(string text, int a, int b)
{
	// Stores encrypted output
	string result = "";
	// Reduce large key values into 0-25
	a = a % 26;
	b = b % 26;

	// Encrypt each character
	for (char ch : text)
	{
		if (isupper(ch))
			// Map A-Z to 0-25, multiply by a, add b, wrap with mod, map back
			result += char((a * (ch - 'A') + b) % 26 + 'A');
		else if (islower(ch))
			// Same transformation for lowercase letters
			result += char((a * (ch - 'a') + b) % 26 + 'a');
		else
			// Keep spaces and symbols unchanged
			result += ch;
	}

	// Return ciphertext
	return result;
}
```

Important lines:

- `(a * (ch - 'A') + b) % 26 + 'A'` applies the affine transformation.
- Lowercase branch handles small letters similarly.
- Non-letters are preserved.

### 5.6 Function: decryptText

What it does:

- Decrypts by reversing the affine transformation using the inverse of a.

Code:

```cpp
string decryptText(string text, int a, int b)
{
	// Stores decrypted output
	string result = "";

	// Compute multiplicative inverse of a under mod 26
	int aInverse = findModularInverse(a, 26);

	// If inverse does not exist, decryption cannot proceed
	if (aInverse == -1)
	{
		cout << "Error: Key 'a' has no modular inverse.\n";
		return "";
	}

	// Reduce b to valid range
	b = b % 26;

	// Decrypt each character using inverse key
	for (char ch : text)
	{
		if (isupper(ch))
			// Reverse affine transformation for uppercase letters
			result += char((aInverse * ((ch - 'A' - b + 26) % 26)) % 26 + 'A');
		else if (islower(ch))
			// Reverse affine transformation for lowercase letters
			result += char((aInverse * ((ch - 'a' - b + 26) % 26)) % 26 + 'a');
		else
			// Keep non-letters unchanged
			result += ch;
	}

	// Return recovered plaintext
	return result;
}
```

Important lines:

- `int aInverse = findModularInverse(a, 26);` is the core decryption requirement.
- If inverse does not exist, function returns safely.

### 5.7 Function: main

What it does:

- Controls complete flow from input to final output.

Code:

```cpp
int main()
{
	string text, aInput, bInput;

	// Read full text including spaces
	cout << "Enter text: ";
	getline(cin, text);

	// Read key a as string to support both letter and number input
	cout << "Enter key 'a' (multiplicative, number or single letter): ";
	cin >> aInput;

	// Read key b as string
	cout << "Enter key 'b' (additive, number or single letter): ";
	cin >> bInput;

	// Validate key inputs
	if (!isValidKeyInput(aInput) || !isValidKeyInput(bInput))
	{
		cout << "Invalid key! Use only a number or a single alphabet letter for each key.\n";
		return 0;
	}

	// Convert validated keys to integers
	int a = convertKeyInput(aInput);
	int b = convertKeyInput(bInput);

	// Validate mathematical key condition for a
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

	// Encrypt and display ciphertext
	string encrypted = encryptText(text, a, b);
	cout << "\nEncrypted Text: " << encrypted << endl;

	// Decrypt and display recovered plaintext
	string decrypted = decryptText(encrypted, a, b);
	cout << "Decrypted Text: " << decrypted << endl;

	// Program completed successfully
	return 0;
}
```

Important lines:

- Input validation prevents conversion errors.
- Co-prime check for a ensures decryptable key.
- Range check for b.
- Final print confirms encryption and decryption cycle.

## 6. Error Handling Behavior

Invalid key format:

```text
abc
```

Output:

```text
Invalid key! Use only a number or a single alphabet letter for each key.
```

Invalid mathematical key for a:

```text
13
```

Output:

```text
Invalid key 'a'! Must be co-prime with 26.
```

Invalid range for b:

```text
30
```

Output:

```text
Invalid key 'b' range! Must be between 0 and 25.
```

## 7. Key Concepts You Learn Here

- Modular arithmetic
- Co-prime numbers and modular inverse
- Extended Euclidean algorithm
- Combining multiplication and addition for encryption
- Input validation for safe programs

## 8. Quick Summary

Affine cipher combines multiplicative and additive ciphers for stronger encryption. It uses two keys: a for multiplication (must be co-prime with 26) and b for addition. Decryption requires the modular inverse of a. The program validates inputs and demonstrates both encryption and decryption.
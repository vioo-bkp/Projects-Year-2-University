## IOCLA HW2 - Strings
Mateescu Viorel-Cristian
Time Solved: 5 days

# 1. One Time Pad
	The One Time Pad algorithm encrypts a message by doing xor between each character from the message and its corresponding character from the key(the key and the message to be encrypted have the same length). 
	In order to do this each character(byte) from the message, respectively from the key is moved to the al, respectively the bl registers. These two register are XOR-ed and the resulted character(byte) is added 
	to the encrypted message.

# 2. Caesar Cipher
	The Caesar Cipher algorithm implies the circular shift of each alphabetical character from the message to be encrpted a certain number of times indicated by the encryption key. Circular shifting means that 
	lowercase/uppercase letters will still be lowercase/uppercase after shifting. The non alphabetical characters will not be encrypted, they will be the same as before. In order to do this each character(byte) 
	from the message is moved to the al register and it's checked whether it is an uppercase, a lowercase or a non alphabetical character. Each character's value is reduced to the range [0, 25] by subtracting 
	'A'/'a' for uppercase/lowercase from it and the key's value is added to it. This value is divided by 26('z' - 'a' + 1). By adding to the remainder the value we subtracted from al earlier we obtain the 
	character's encryption. In the end, each character is moved to the encrypted message, whether it has been encrypted(alphabetical character) or not(non alphabetical character).

# 3. Vigenere Cipher
	The Vigenere Cipher algoritm implies the circular shift of each alphabetical character from the message to be encrypted a certain number of times indicated by the index of the corresponding character from 
	the encryption key('A' -> index 0, 'B' ->  index 1 etc.). A few steps of this algorithm are similar to the ones from the Caesar Cipher algorithm. Circular shifting means that lowercase/uppercase letters will 
	still be lowercase/uppercase after shifting. The non alphabetical characters will not be encrypted, they will be the same as before. First, the characters from the key are reduced to the range [0, 25] by 
	subtracting 'A' from each. In order to encrypth the message each character(byte) from it is moved to the al register and it's checked whether it is an uppercase, a lowercase or a non alphabetical character. 
	Each character's value is reduced to the range [0, 25] by subtracting 'A'/'a' for uppercase/lowercase from it and the key's corresponding character's value is added to it. This value is divided by 26('z' - 
	'a' + 1). By adding to the remainder the value we subtracted from al earlier we obtain the character's encryption. In the end, each character is moved to the encrypted message, whether it has been encrypted(
	alphabetical character) or not(non alphabetical character).

# 4. StrStr
	The StrStr algorithm checks whether a substring is found in a string(finding the needle in the haystack). We are looping through haystack's characters(bytes). If the current character of the haystack matches 
	the first character from the needle we start looping through needle's characters. As long as the characters match we move to the next character from the haystack, respectively from the needle and keep on 
	checking whether if they match anymore or not. If two non matching characters are found the index for the needle is reset to 0. If we reach the end of the needle then we found a match for the substring(
	needle) in the string(haystack) and we move to [edi] the index of the first match. We are only looping through the first (haystack_legth - needle_length) characters of the haystack in order not to search for 
	the needle in the haystack if there aren't as many characters left in the haystack as in the needle. So if we reach the (haystack_legth - needle_length + 1)'th character from the haytack we can't find a 
	match anymore and we have to move to [edi] the haystack's length + 1.

# 5. Binary to Hexadecimal
	The Binary to Hexadecimal algorithm converts a binary sequence to it's hexadecimal value, as a string.
	The strategy is:
    	- Take the binary sequence;
    	- Divide the binary digits into groups of 4(starting from right to left);
    	- Convert each group of 4 binary digits(bits) to one hexadecimal digit. 
    First we find out how many characters we'll have after grouping the bits by dividing the sequence's length by 4 and checking if there are any bits left apart(if there are we increase the quotient by one). 
    Start making pairs of 4 bits. If the current bit is 1 we have to add it to the pair's value(if it's the first bit we add 2^0, for the second one 2^1 etc), but if the bit is 0 it doesn't affect the pair's 
    value. We do this by setting ebx to 1 and left shifting it each time we move to the next bit in pair. When ebx is 16 then the pair is complete and we can convert it's value to a number if it's lower than 
    10(1 -> '1' etc.), or to a letter if it's between 10 and 15(10 -> 'A' etc.). The pair's value is added to hexa_value. After all the bits from the sequence were looped we check if there was any incomplete 
    pair of digits and add it to hexa_value. In the end '\n' is added at the end of hexa_value.
    
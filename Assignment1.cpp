/*
*   Keshawn Jones
*  November 3, 2017
*   CSCI 3341
*   Prof. Hashemi
*
*   Assignment 1 Multi-Threading Using pthread
*/

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <climits>

using namespace std;

string input, message1, message2, message3; // global vars
int order[3]; // Holds order of the strings in the input

void* sift(void*);
bool validate(string);
void* decode(void*);
void* fencef(void*);
void* hillf(void*);
void* pinnaclef(void*);
string toUpperCase(string);
string matrixMul(string, string, int, int);
string toString(int);
int toInt(string);
char toChar(int);


int main() {

	cout << "\n\\* Assignment 1\n *For Professor Ray Hashemi\n *CSCI 3341\n *By: Keshawn Jones\n *November 3rd, 2017 \n*\\";

	int result;
	pthread_t sifter;
	void * sifterptr;

	result = pthread_create(&sifter, NULL, &sift, NULL);
	if (result) {
		cout << "Error creating thread \"sifter\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_join(sifter, &sifterptr);
	if (result) {
		cout << "Error joining thread \"sifter\"\n";
		exit(EXIT_FAILURE);
	}

	return 0;
}

void* sift(void*) {
	int result, attempts = 0;
	bool cont = true;

	while (cont) {
		input = "";
		string a, b, c;
		cout << "\n\n*******************************************************\n\n";
		cout << "Please enter a string to be evaluated\n";
		cout << "To exit this program, enter EXIT\n";
		getline(cin, input);
		cout << "\n";
		if (input == "EXIT") {
			cout << "Good-Bye. Logging out\n\n";
			cont = false;
			break;
		}
		else {
			cout << "You entered: " << input;
			bool valid = validate(input);
			if (!valid) {
				attempts++;
				if (attempts < 3) cout << input << " is an invalid string.\nAttempts Remaining: " << 3 - attempts << "\n";
				else {
					cout << "Maximum number of attempts reached. System is exiting\n";
					cont = false; break;
				}
			}

			else {

				cout << "\n*******************************************************\n";
				attempts = 0;

				pthread_t decoder;
				void * decoderptr;

				result = pthread_create(&decoder, NULL, &decode, NULL);
				if (result) {
					cout << "Error creating thread \"decoder\"\n";
					exit(EXIT_FAILURE);
				}

				result = pthread_join(decoder, &decoderptr);
				if (result) {
					cout << "Error joining thread \"decoder\"\n";
					exit(EXIT_FAILURE);
				}
			}
		}

		cout << "\nThe final decoded message is: ";

		for (int i = 0; i < 3; i++) {
			switch (order[i]) {
			case 1: cout << message1 << " "; break;
			case 2: cout << message2 << " "; break;
			case 3: cout << message3 << " "; break;
			default: cout << "ERROR:" << " ";
			}
		}
	}
}

bool validate(string s) {
	bool a = false, b = false, w = false;
	int j = 0;
	order[0] = order[1] = order[2] = 0;

	for (unsigned int i = 0; i < s.length(); i++) {
		char c = s.at(i);
		if (c == '*') { // 1 asterisk
			if (s.at(i + 1) == '*') { // 2 asterisk
				if (s.at(i + 2) == '*') { // 3 asterisk
					if (s.at(i + 3) == '*' || w) { // Too many asterisks or already 3 asterisks in string
						return false;
					}

					else {
						w = true;
						order[j] = 3;
						j++;
						i += 2;
					}
				}
				else if (b) return false; // already 2 asterisks in string

				else {
					b = true;
					order[j] = 2;
					j++;
					i += 1;
				}
			}
			else if (a) return false; // already 1 asterisk in string
			else {
				a = true;
				order[j] = 1;
				j++;
			}
		}
	}

	if (a && b && w) return true;
	return false;
}

void* decode(void*) {
	int j = 0;
	message1 = message2 = message3 = "";
	
	for (unsigned int i = 0; i < input.length();) { // Break input into 3 strings
		char c = input.at(i);
		if (c == '*') {
			int k = i + order[j];
			char x = input.at(k);
			while (x != '*' && k < input.length()) {
				
				switch (order[j]) {
				case 1:
					message1 = message1 + x; break;
				case 2:
					message2 = message2 + x; break;
				case 3:
					message3 = message3 + x; break;
				default:
					cout << "Error in decoding. Exiting\n";
					exit(EXIT_FAILURE);
				}

				k++;
				try {
					x = input.at(k);
				}
				catch (exception e) { // For index out of bounds
					break;
				}
			}

			i = k;
			j++;
		}
		else i++;
	}

	int result;
	pthread_t fence, hill, pinnacle;
	void *fenceptr, *hillptr, *pinnacleptr;

	result = pthread_create(&fence, NULL, &fencef, NULL);
	if (result) {
		cout << "Error creating thread \"fence\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_create(&hill, NULL, &hillf, NULL);
	if (result) {
		cout << "Error creating thread \"hill\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_create(&pinnacle, NULL, &pinnaclef, NULL);
	if (result) {
		cout << "Error creating thread \"pinnacle\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_join(fence, &fenceptr);
	if (result) {
		cout << "Error joining thread \"fence\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_join(hill, &hillptr);
	if (result) {
		cout << "Error joining thread \"hill\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_join(pinnacle, &pinnacleptr);
	if (result) {
		cout << "Error joining thread \"pinnacle\"\n";
		exit(EXIT_FAILURE);
	}

}

void* fencef(void*) {
	char c = message1.at(0);
	string num, mess;
	if (c < '1' || c > '9') { //Must begin with a digit, or is invalid
		cout << "Invalid string found in fence thread\n";
		return 0;
	}

	num = "";
	int i = 0;
	while (c >= '1' && c <= '9' && i < message1.length()) { // Search through all numbers in the string
		if (num.find(c) != -1) { // If a digit is repeated, is invalid
			cout << "Invalid string found in fence thread\n";
			return 0;
		}

		num = num + message1.at(i);
		i++;
		c = message1.at(i);
	}

	int largest = 0;
	for (int j = 0; j < num.length(); j++) {
		int temp = (int)num.at(j) - 48;
		if (temp > largest) largest = temp;
	}

	if (num.length() != largest) {
		cout << "Invalid string found in fence thread\n";
		return 0;
	}

	while (i < message1.length()) { // Get message to be decoded
		mess = mess + message1.at(i);
		i++;
	}

	const int cols = num.length();
	const int rows = (mess.length() / num.length());
	char matrix[rows][cols];
	int k = 0;

	for (int j = 0; j < cols; j++) { // Set up array 
		for (i = 0; i < rows; i++) {
			matrix[i][j] = mess.at(k);
			k++;
		}
	}

	k = 0;

	char output[rows][cols];
	while (k < cols) { // Create Output Matrix
		int j = (int)num.at(k) - 49;

		for (i = 0; i < rows; i++) {
			output[i][k] = matrix[i][j];
		}
		k++;
	}

	string ret = "";
	for (i = 0; i < rows; i++) { // print decoded message
		for (int j = 0; j < cols; j++) {
			ret = ret + output[i][j];
		}
	}

	cout << "Fence thread returns: " << ret << "\n";
	message1 = ret;
	return 0;
}

void* hillf(void*) {
	char c = message2.at(0);
	message2 = toUpperCase(message2);
	if (c < 'A' || c > 'Z') { // If does not begin with number, string is invalid
		cout << "Invalid string found in hill thread\n";
		return 0;
	}

	string sec1 = "";
	int i = 0;
	while (c >= 'A' && c <= 'Z' && i < message2.length()) { // Search for letters in section 1
		sec1 = sec1 + c;
		i++;
		c = message2.at(i);
	}
	
	if (sec1.length() % 2 != 0 ) { // if non-even amount of characters in section 1, string is invalid
		cout << "Invalid string found in fence thread\n";
		return 0;
	}

	string temp;
	int sec2[2][2] = { {INT_MIN, INT_MIN}, {INT_MIN, INT_MIN} }; // For testing purpose
	int j, k;

	for (j = 0; j < 2 && i < message2.length(); j++) { // Get digits for sec 2;
		for (k = 0; k < 2 && i < message2.length(); k++) {
			if (c < '0' || c > '9') { //if c is not a number...
				if (c != ' ') { // ...and c is not a space, string is invalid
					cout << "Invalid string found in hill thread\n";
					return 0;
				}
			}


			if (c == ' ') { //if c is a space, increment
				i++;
				try {
					do {
						c = message2.at(i);
						if (c >= '0' && c <= '9') break;
					} while (i < message2.length());
				}
				catch (exception e) { // For index out of bounds
					break;
				}
			}

			if (c >= '0' && c <= '9') { // c is a number. Add to matrix
				temp = "";
				while ((c >= '0' && c <= '9') && i < message2.length()) {
					temp = temp + c;
					i++;
					try {
						c = message2.at(i);
					}
					catch (exception e) { // For index out of bounds
						break;
					}
				}

				sec2[j][k] = toInt(temp);
			}
		}	
	}

	if (sec2[1][1] == INT_MIN) { // If the last value is Int_MIN, then there must be less than 4 digits, and string is invalid
		cout << "Invalid string found in hill thread\n";
		return 0;
	}

	string sec2Str = "";
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			sec2Str = sec2Str + toString(sec2[x][y]) + " ";
		}
	}

	string ret = matrixMul(sec2Str, sec1, 2, 1);

	cout << "Hill thread returns: " << ret << "\n";
	message2 = ret;
	return 0;

}

void* pinnaclef(void*) {
	char c = message3.at(0);
	message3 = toUpperCase(message3);
	if (c < 'A' || c > 'Z') { // If does not begin with number, string is invalid
		cout << "Invalid string found in pinnacle thread\n";
		return 0;
	}

	string sec1 = "";
	int i = 0;
	while (c >= 'A' && c <= 'Z' && i < message3.length()) { // Search for letters in section 1
		sec1 = sec1 + c;
		i++;
		c = message3.at(i);
	}

	if (sec1.length() % 3 != 0) { // if non-divisible by 3 amount of characters in section 1, string is invalid
		cout << "Invalid string found in pinnacle thread\n";
		return 0;
	}

	string temp;
	int sec2[3][3] = { { INT_MIN, INT_MIN },{ INT_MIN, INT_MIN },{ INT_MIN, INT_MIN } }; // For testing purpose
	int j, k;

	for (j = 0; j < 3 && i < message3.length(); j++) { // Get digits for sec 2;
		for (k = 0; k < 3 && i < message3.length(); k++) {
			if (c < '0' || c > '9') { //if c is not a number...
				if (c != ' ') { // ...and c is not a space, string is invalid
					cout << "Invalid string found in pinnacle thread\n";
					return 0;
				}
			}


			if (c == ' ') { //if c is a space, increment
				i++;
				try {
					do {
						c = message3.at(i);
						if (c >= '0' && c <= '9') break;
					} while (i < message3.length());
				}
				catch (exception e) { // For index out of bounds
					break;
				}
			}

			if (c >= '0' && c <= '9') { // c is a number. Add to matrix
				temp = "";
				while ((c >= '0' && c <= '9') && i < message3.length()) {
					temp = temp + c;
					i++;
					try {
						c = message3.at(i);
					}
					catch (exception e) { // For index out of bounds
						break;
					}
				}

				sec2[j][k] = toInt(temp);
			}
		}
	}

	if (sec2[1][1] == INT_MIN) { // If the last value is Int_MIN, then there must be less than 4 digits, and string is invalid
		cout << "Invalid string found in pinnacle thread\n";
		return 0;
	}

	string sec2Str = "";
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			sec2Str = sec2Str + toString(sec2[x][y]) + " ";
		}
	}

	string ret = matrixMul(sec2Str, sec1, 3, 1);

	cout << "Pinnacle thread returns: " << ret << "\n";
	message3 = ret;
	return 0;
}

string toUpperCase(string s) {
	string ret = "";
	for (int i = 0; i < s.length(); i++) {
		if (s.at(i) >= 'a' && s.at(i) <= 'z') {
			ret = ret + (char)(s.at(i) - 32);
		}
		else {
			ret = ret + (char)(s.at(i));
		}
	}

	return ret;
}

string matrixMul(string a, string b, int rows, int cols) {
	int A[rows][rows];
	int B[rows][cols];
	int C[rows][cols];
	char c;
	string temp;
	string ret = "";

	int i = 0;
	for (int k = 0; k < rows && i < a.length(); k++) { // Set matrix A
		for (int j = 0; j < rows && i < a.length(); j++) {
			char c = a.at(i);
			temp = "";
			while (c != ' ') {
				temp = temp + c;
				i++;
				try {
					c = a.at(i);
				}
				catch (exception e) { // For index out of bounds
					break;
				}
			}

			A[k][j] = toInt(temp);
			i++;
		}
	}

	i = 0;
	do { // Set matrix B
		for (int k = 0; k < rows; k++) {
			for (int j = 0; j < cols; j++) {
				c = b.at(i);
				int t = ((int)c - 65);
				B[k][j] = t;
				i++;
			}
		}

		for (int x = 0; x < rows; x++) { //initialize C to 0 matrix
			for (int y = 0; y < cols; y++) {
				C[x][y] = 0;
			}

		}
		for (int x = 0; x < rows; x++) { // Multiply A and B
			for (int y = 0; y < cols; y++) {
				for (int z = 0; z < rows; z++) {
					C[x][y] += A[x][z] * B[z][y];
				}
			}
		}

		for (int x = 0; x < rows; x++) {
			for (int y = 0; y < cols; y++) {
			}
		}

		for (int x = 0; x < rows; x++) { // mod 26
			for (int y = 0; y < cols; y++) {
				C[x][y] = C[x][y] % 26;
				ret = ret + toChar(C[x][y]);
			}
		}
	} while (i < b.length());

	return ret;
}

string toString(int a) {
	stringstream in;
	in << a;
	return in.str();
}

int toInt(string s) {
	stringstream in(s);
	int x = 0;

	in >> x;
	//cout << x;
	return x;
}

char toChar(int a) {
	int b;
	switch (a) {
	case 0: b = 'A'; break;
	case 1: b = 'B'; break;
	case 2: b = 'C'; break;
	case 3: b = 'D'; break;
	case 4: b = 'E'; break;
	case 5: b = 'F'; break;
	case 6: b = 'G'; break;
	case 7: b = 'H'; break;
	case 8: b = 'I'; break;
	case 9: b = 'J'; break;
	case 10: b = 'K'; break;
	case 11: b = 'L'; break;
	case 12: b = 'M'; break;
	case 13: b = 'N'; break;
	case 14: b = 'O'; break;
	case 15: b = 'P'; break;
	case 16: b = 'Q'; break;
	case 17: b = 'R'; break;
	case 18: b = 'S'; break;
	case 19: b = 'T'; break;
	case 20: b = 'U'; break;
	case 21: b = 'V'; break;
	case 22: b = 'W'; break;
	case 23: b = 'X'; break;
	case 24: b = 'Y'; break;
	case 25: b = 'Z'; break;
	default: b = '_';
	}

	return b;
}

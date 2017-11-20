/*
*   Keshawn Jones
*	November 28, 2017
*   CSCI 3341
*   Prof. Hashemi
*
*   Assignment 2 Use of Semaphores
*
*	This program creates two threads, a producer and consumer,
*	which work in parallel to create and consume characters based on information
*	provided in the prompt
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
#include <unistd.h>
#include <semaphore.h>

using namespace std;

sem_t empty, full, mutex;
vector<char> buffer;
int n;

void* produce(void*);
void* consume(void*);
void consumeHelper(char);
char toChar(int);
char closestVowel(string, char);
int dist(string, char, char);
int toInt(string);


int main()
{
	cout << "\n\\* Assignment 2\n *For Professor Ray Hashemi\n *CSCI 3341\n *By: Keshawn Jones\n *November 28th, 2017 \n*\\\n";
	cout << "\n\n*******************************************************\n\n";
	cout << "How many total characters would you like to evaluate? (Please enter integer values only)\n";
   
	string in;

	pthread_t producer, consumer;
	void *producerptr, *consumerptr;
	int result, buffers;
	getline(cin, in);
	n = toInt(in);

	//Semaphore initialization
	result = sem_init(&empty, 0, 2);
	if (result) {
		cout << "Error creating semaphore \"empty\"\n";
		exit(EXIT_FAILURE);
	}

	result = sem_init(&full, 0, 0);
	if (result) {
		cout << "Error creating semaphore \"full\"\n";
		exit(EXIT_FAILURE);
	}

	result = sem_init(&mutex, 0, 1);
	if (result) {
		cout << "Error creating semaphore \"mutex\"\n";
		exit(EXIT_FAILURE);
	}

	//Thread initialization
	result = pthread_create(&producer, NULL, &produce, NULL);
	if (result) {
		cout << "Error creating thread \"producer\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_create(&consumer, NULL, &consume, NULL);
	if (result) {
		cout << "Error creating thread \"consumer\"\n";
		exit(EXIT_FAILURE);
	}

	//Thread Joining
	result = pthread_join(producer, &producerptr);
	if (result) {
		cout << "Error joining thread \"producer\"\n";
		exit(EXIT_FAILURE);
	}

	result = pthread_join(consumer, &consumerptr);
	if (result) {
		cout << "Error joining thread \"consumer\"\n";
		exit(EXIT_FAILURE);
	}

	//Semaphore Destrucion
	result = sem_destroy(&empty);
	if (result) {
		cout << "Error destroying semaphore \"empty\"\n";
		exit(EXIT_FAILURE);
	}

	result = sem_destroy(&full);
	if (result) {
		cout << "Error destroying semaphore \"full\"\n";
		exit(EXIT_FAILURE);
	}

	result = sem_destroy(&mutex);
	if (result) {
		cout << "Error destroying semaphore \"mutex\"\n";
		exit(EXIT_FAILURE);
	}

	return 0;
}

void* produce(void*) {
	char c;
	int result;
	for (int i = 0; i < n; i++) {
		c = toChar((rand() % 26)); // Generate a random character
		cout << "Producer: " << c << "\n";

		result = sem_wait(&empty);
		if (result) {
			cout << "Producer: Error waiting in semaphore \"empty\"\n";
			exit(EXIT_FAILURE);
		}

		result = sem_wait(&mutex);
		if (result) {
			cout << "Producer: Error waiting in semaphore \"mutex\"\n";
			exit(EXIT_FAILURE);
		}

		buffer.push_back(c);

		result = sem_post(&mutex);
		if (result) {
			cout << "Producer: Error signaling in semaphore \"mutex\"\n";
			exit(EXIT_FAILURE);
		}

		result = sem_post(&full);
		if (result) {
			cout << "Producer: Error signaling in semaphore \"full\"\n";
			exit(EXIT_FAILURE);
		}
	}
}

void* consume(void*) {
	bool even = false;
	int result;
	for (int i = 0; i < n; i++) {
		result = sem_wait(&full);
		if (result) {
			cout << "Consumer: Error waiting in semaphore \"full\"\n";
			exit(EXIT_FAILURE);
		}

		result = sem_wait(&mutex);
		if (result) {
			cout << "Consumer: Error waiting in semaphore \"mutex\"\n";
			exit(EXIT_FAILURE);
		}
		char c = buffer.front();
		buffer.erase(buffer.begin());

		result = sem_post(&mutex);
		if (result) {
			cout << "Consumer: Error signaling in semaphore \"mutex\"\n";
			exit(EXIT_FAILURE);
		}

		result = sem_post(&empty);
		if (result) {
			cout << "Consumer: Error signaling in semaphore \"empty\"\n";
			exit(EXIT_FAILURE);
		}

		if (!even) {
			cout << "Consumer: " << c << "\n";
			even = true;
		}

		else {
			consumeHelper(c);
			even = false;
		}
	}
}

void consumeHelper(char c) {
	string S = "", T = "";
	int d1, d2, maxd;
	char v1, v2, fartherv;

	v1 = closestVowel("Left", c);
	v2 = closestVowel("Right", c);

	d1 = dist("Left", v1, c);
	d2 = dist("Right", v1, c);

	if (d1 < d2) {
		maxd = d2;
		fartherv = v2;
	}

	else {
		maxd = d1;
		fartherv = v1;
	}

	S = S + v1 + c + v2;
	T = T + fartherv + S + fartherv;

	//cout << "Consumer: " << c << ", " << v1 << ", " << v2 << ", " << T << ", " << d1 << ", " << d2 << ", " << maxd << "\n";
	cout << "Consumer: " << c << v1 << v2 << T << d1 << d2 << maxd << "\n";
}

char toChar(int a) {
	char b;
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

char closestVowel(string s, char c) {
	bool isVowel = false;
	char temp = c;
	while (!isVowel) {
		if (s == "Left") temp = (char)temp - 1;
		else temp = (char)temp + 1;
			
		if (temp < 'A') temp = 'Z';
		if (temp > 'Z') temp = 'A';

		if (temp == 'A' || temp =='E' || temp =='I' 
			|| temp == 'O' || temp == 'U') {
			isVowel = true;
		}
	}
	
	return temp;
}

int dist(string s, char v, char c) {
	int d = 0;
	bool isVowel = false;
	char temp = c;
	while (!isVowel) {
		if (s == "Left") temp = (char)temp - 1;
		else temp = (char)temp + 1;

		if (temp < 'A') temp = 'Z';
		if (temp > 'Z') temp = 'A';

		if (temp == 'A' || temp == 'E' || temp == 'I'
			|| temp == 'O' || temp == 'U') {
			isVowel = true;
		}

		d++;
	}

	return d;
}

int toInt(string s) {
	stringstream in(s);
	int x = 0;

	in >> x;
	//cout << x;
	return x;
}


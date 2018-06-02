#include <iostream>
#include <string>
#include <fstream>
using namespace std;

string getkey();
unsigned char nextk();
void swap(unsigned char* s, unsigned int i, unsigned int j);
unsigned int getdec(char a);
void wordCheck(string text);


unsigned int i, j = 0;
unsigned char S[256];
unsigned char T[256];

int main() {

	ifstream checkFile;
	checkFile.open("rc4Input.txt");
	string fileCheck;
	getline(checkFile, fileCheck);
	cout << "File Detected. Displaying Text:" << endl << endl;
	cout << fileCheck << endl << endl;
	wordCheck(fileCheck);
	checkFile.close();

	int choice;
	cout << endl << "Do you wish to encrypt(1) or decrypt(0)? ";
	cin >> choice;
	cout << endl;

	string strkey;
	strkey = getkey();
	unsigned char* key;
	key = new unsigned char[strkey.length()];

	for (int k = 0; k < strkey.length(); k++) {
		key[k] = strkey[k];
	}

	//initialization
	for (i = 0; i <= 255; i++) {
		S[i] = i;
		T[i] = key[i % (strkey.length())];
	}

	j = 0;
	for (i = 0; i <= 255; i++) {
		j = ((j + S[i] + T[i]) % 256);
		swap(S, i, j);
	}
	///////////////////////////////////

	//Stream generation
	i = 0, j = 0;
	for (int k = 0; k < 3072; k++) {
		nextk();
	}


	/////////////////////////////////////////////////
	if (choice == 1) {
		ifstream inFile;
		inFile.open("rc4Input.txt");
		string strtext;
		getline(inFile, strtext);
		unsigned char* text;
		text = new unsigned char[strtext.length()];
		for (int k = 0; k < strtext.length(); k++) {
			text[k] = strtext[k];
		}
		///////////////////////////////////////
		ofstream outFile;
		outFile.open("rc4Input.txt");
		unsigned char x;
		for (int k = 0; k < strtext.length(); k++) {
			unsigned char tempk = nextk();
			unsigned char tester = (text[k] ^ tempk);
			if (tester < 16) {
				outFile << std::hex << '0' << ((text[k] ^ tempk)) << " ";
				cout << std::hex << '0' << ((text[k] ^ tempk)) << " ";
			}
			else {
				outFile << std::hex << ((text[k] ^ tempk)) << " ";
				cout << std::hex << ((text[k] ^ tempk)) << " ";
			}
		}
	}
	/////////////////////////////////////

	////////////////////////////////////
	else if (choice == 0) {

		ifstream inputFile;
		inputFile.open("rc4Input.txt");
		unsigned char* cipher;
		int count = 0;
		string ciphertext;
		getline(inputFile, ciphertext);
		//cout << ciphertext << "!" << endl;
		//cout << "ciphertext" << ciphertext.length();
		cipher = new unsigned char[(ciphertext.length() / 3)];
		count = 0;
		for (int k = 0; k < (ciphertext.length() / 3); k++) {
			int x, y;
			if (ciphertext[count] != ' ' && ciphertext[count + 1] != ' ') {
				x = getdec(ciphertext[count]);
				y = getdec(ciphertext[count + 1]);
				count += 3;
				cipher[k] = ((x * 16) + y);
			}
			else if (ciphertext[count] != ' ') {
				y = getdec(ciphertext[count]);
				cipher[k] = (y);
				count += 2;
			}
			else {
				count++;
			}

		}

		///////////////////////////////////////
		ofstream outputFile;
		outputFile.open("rc4Input.txt");
		string output;
		for (int k = 0; k < (ciphertext.length() / 3); k++) {
			unsigned char m = (cipher[k] ^ nextk());
			output += m;
		}
		cout << output << endl << "Text saved to file." << endl;
		outputFile << output;
	}
	else {
		cout << "Invalid input" << endl;
	}
	//////////////////////////////////////
	cout << endl << endl;
	system("pause");
	return 0;
}

string getkey() {
	string inpt;
	cout << "Please enter key (5-32 bytes): ";
	cin >> inpt;
	cout << endl;
	if ((inpt.length() >= 5) && (inpt.length() <= 32)) {
		return inpt;
	}
	else {
		getkey();
	}
}
////////////////////////////////////////////////////////////////
void swap(unsigned char* s, unsigned int i, unsigned int j) {

	unsigned char temp;
	temp = s[i];
	s[i] = s[j];
	s[j] = temp;
}
////////////////////////////////////////////
unsigned char nextk() {
	i = (i + 1) % 256;
	j = (j + S[i]) % 256;
	swap(S, i, j);
	return S[(S[i] + S[j]) % 256];
}
///////////////////////////////////////////
unsigned int getdec(char a) {
	unsigned int c;
	if ((a >= 'a') && (a <= 'z')) {
		c = a - 87;
		return c;
	}
	else if ((a >= '0') && (a <= '9')) {
		c = a - 48;
		return c;
	}
	else {
		c = 0;
		return c;
	}
}

void wordCheck(string text) {
	string commonWords[] = { " the "," be "," to "," of "," and "," in "," that "," have "," I " };
	for (int i = 0; i < sizeof(commonWords); i++) {
		if (text.find(commonWords[i]) != std::string::npos) {
			cout << "Plaintext has been detected.";
			cout << endl;
			break;
		}
		else if (i < sizeof(commonWords) - 1) {
			cout << "No plaintext detected. Possibly encrypted." << endl;
			break;
		}
	}
}
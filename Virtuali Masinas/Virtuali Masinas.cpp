#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;


char reg[16];
int Ry, Rx;
bool nullFlag, endFlag;

void INC(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] += 0x01;
}
void DEC(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] -= 0x01;
}
void MOV(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Ry] = reg[Rx];
}
void MOVC(char &memory) {
	reg[0] = memory;
}
void LSL(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] *= 0x02;
}
void LSR(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] /= 0x02;
}
void JMP(char &memory, int &i) {
	int jump = memory;
	i += jump - 2; // nes main funkcijoj - i - po ivikdymo padideja 2;
}
void JZ(char &memory, int &i) {
	if (nullFlag) {
		int jump = memory;
		i += jump - 2;
	}
}
void JNZ(char &memory, int &i) {
	if (!nullFlag) {
		int jump = memory;
		i += jump - 2;
	}
}
void JFE(char &memory, int &i) {
	if (endFlag) {
		int jump = memory;
		i += jump - 2;
	}
}
void RET() {
	//add break to the if statement
}
void ADD(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] += reg[Ry];
}
void SUB(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] -= reg[Ry];
}
void XOR(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] = reg[Rx] ^ reg[Ry];
}
void OR(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] = reg[Rx] | reg[Ry];
}
void IN(char &memory, std::ifstream &data) {
	if (!endFlag) {
		Rx = memory & 0x0F;
		data.read(&reg[Rx], 1);
	}
	if (data.eof()) endFlag = true;
}
void OUT(char &memory, std::ofstream &rez) {
	Rx = memory & 0x0F;
	rez.write(&reg[Rx], 1);
}

int main()
{
	std::ifstream data;
	std::ofstream rez;
	rez.open("rez.txt", std::ios::binary);
	char reader;
	char memory[256];
	//-------------
	data.open("q1_encr.txt", std::ios::binary);
	if (!data.is_open()) {
		std::cout << "failed to open data file \n";
		std::cin.ignore();
	}
	nullFlag = false;
	endFlag = false;
	//------------------
	std::ifstream decryptor("decryptor.bin", std::ios::binary);
	int numb_of_instructions = 0;
	while (!decryptor.eof()) {
		decryptor.read(&memory[numb_of_instructions], 1);
		numb_of_instructions++;
	}
	int i = 0;
	char temp;
	while (true) {
		temp = memory[i];
		if (temp == 0x01) INC(memory[i + 1]); // +
		if (temp == 0x02) DEC(memory[i + 1]); // + 
		if (temp == 0x03) MOV(memory[i + 1]); // +
		if (temp == 0x04) MOVC(memory[i + 1]); // +
		if (temp == 0x05) LSL(memory[i + 1]); // +
		if (temp == 0x06) LSR(memory[i + 1]); // +
		if (temp == 0x07) JMP(memory[i + 1], i); // +
		if (temp == 0x08) JZ(memory[i + 1], i); // + -- buvo iskviesta kelis kart nors siame darbe neturetu buti naudota (suolio neatliko ne karto)
		if (temp == 0x09) JNZ(memory[i + 1], i); // + ? - nenaudota
		if (temp == 0x0A) JFE(memory[i + 1], i); // +
		if (temp == 0x0B) break; // + ? if its all it should do 
		if (temp == 0x0C) ADD(memory[i + 1]); // + kind a working i guess? just not called in meaningful moments
		if (temp == 0x0D) SUB(memory[i + 1]); // +
		if (temp == 0x0E) XOR(memory[i + 1]); // +
		if (temp == 0x0F) OR(memory[i + 1]); // +
		if (temp == 0x10) IN(memory[i + 1], data); // +
		if (temp == 0x11) OUT(memory[i + 1], rez); // +
		i += 2;
	}
	rez.close();
	data.close();
	decryptor.close();
	return 0;
}
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

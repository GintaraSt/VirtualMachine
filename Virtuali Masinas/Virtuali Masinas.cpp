#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;


char reg[16];
int Ry, Rx;
bool nullFlag, endFlag;

//increase registry X by 1
void INC(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] += 0x01;
}
//decreases registry X by 1
void DEC(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] -= 0x01;
}
//copys registry Y to  X
void MOV(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Ry] = reg[Rx];
}
//copies const to registry 0
void MOVC(char &memory) {
	reg[0] = memory;
}
//Moves registry X to left by 1 bite
void LSL(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] *= 0x02;
}
//Moves registry X to right by 1 bite
void LSR(char &memory) {
	Rx = memory & 0x0F;
	reg[Rx] /= 0x02;
}
//jumps to relative  address by adding const to i (i - current position of memory array)
void JMP(char &memory, int &i) {
	int jump = memory;
	i += jump - 2; // nes main funkcijoj - i - po ivikdymo padideja 2;
}
//jumps if nullFlag - true
void JZ(char &memory, int &i) {
	if (nullFlag) {
		int jump = memory;
		i += jump - 2;
	}
}
//jumps if nullFlag - false
void JNZ(char &memory, int &i) {
	if (!nullFlag) {
		int jump = memory;
		i += jump - 2;
	}
}
//jumps if endFlag - true
void JFE(char &memory, int &i) {
	if (endFlag) {
		int jump = memory;
		i += jump - 2;
	}
}
//returns from virtual machine - replaced with break statement in main()
void RET() {
	//add break to the if statement
}
//adds registry X to registry Y and writes result to Y
void ADD(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] += reg[Ry];
}
//subdivides registry Y from registry X and writes result to Y
void SUB(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] -= reg[Ry];
}
//XOR operation for each bit seperately - result is left in registry X
void XOR(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] = reg[Rx] ^ reg[Ry];
}
//OR operation for each bit seperately - result is left in registry X
void OR(char &memory) {
	Rx = memory & 0x0F;
	Ry = (memory & 0xF0) / 0x10;
	reg[Rx] = reg[Rx] | reg[Ry];
}
//Reads 1 byte from file and sets endFlag to true if end of file was reached
void IN(char &memory, std::ifstream &data) {
	if (!endFlag) {
		Rx = memory & 0x0F;
		data.read(&reg[Rx], 1);
	}
	if (data.eof()) endFlag = true;
}
//Writes registry X data to the file
void OUT(char &memory, std::ofstream &rez) {
	Rx = memory & 0x0F;
	rez.write(&reg[Rx], 1);
}

int main()
{
	std::ifstream data;
	std::ofstream rez;
	rez.open("rez.txt", std::ios::binary);
	char memory[256]; //stores decryptor.bin data
	//-------------
	data.open("q1_encr.txt", std::ios::binary);
	if (!data.is_open()) {
		std::cout << "failed to open data file \n";
		std::cin.ignore();
	}
	nullFlag = false; // not used
	endFlag = false;
	//------------------
	std::ifstream decryptor("decryptor.bin", std::ios::binary);
	int numb_of_instructions = 0;
	while (!decryptor.eof()) {
		decryptor.read(&memory[numb_of_instructions], 1);
		numb_of_instructions++;
	}
	int i = 0; //current position in memory array
	char temp;
	//runs virtual machine until RET() function should be called (instead of RET uses simple break;
	while (true) {
		temp = memory[i];
		if (temp == 0x01) INC(memory[i + 1]); 
		if (temp == 0x02) DEC(memory[i + 1]); 
		if (temp == 0x03) MOV(memory[i + 1]); 
		if (temp == 0x04) MOVC(memory[i + 1]);
		if (temp == 0x05) LSL(memory[i + 1]); 
		if (temp == 0x06) LSR(memory[i + 1]);
		if (temp == 0x07) JMP(memory[i + 1], i);
		if (temp == 0x08) JZ(memory[i + 1], i);
		if (temp == 0x09) JNZ(memory[i + 1], i); 
		if (temp == 0x0A) JFE(memory[i + 1], i);
		if (temp == 0x0B) break;
		if (temp == 0x0C) ADD(memory[i + 1]);
		if (temp == 0x0D) SUB(memory[i + 1]); 
		if (temp == 0x0E) XOR(memory[i + 1]); 
		if (temp == 0x0F) OR(memory[i + 1]);
		if (temp == 0x10) IN(memory[i + 1], data);
		if (temp == 0x11) OUT(memory[i + 1], rez); 
		i += 2;
	}
	rez.close();
	data.close();
	decryptor.close();
	return 0;
}

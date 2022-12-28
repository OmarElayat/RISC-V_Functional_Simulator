#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include<algorithm>
#include <cstdlib>
#include<sstream>
#include<format>
#include<set>
#include <bitset>

using namespace std;

// Define the registers of the simulator

int Registers[32];
string Reg_num[32] = {"x0", "ra","sp","gp","tp","t0","t1","t2","s0","s1","a0","a1","a2","a3","a4","a5","a6","a7","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11","t3","t4","t5","t6"};

map<string, int> mymap;

#define x0 Registers[0]
#define ra Registers[1]
#define sp Registers[2]
#define gp Registers[3]

// Declare all the used instructions in vectors of their type

vector <string> I_type = { "addi","slti","sltiu","xori","ori","andi","slli" ,"srli" ,"srai","ecall","ebreak" ,"fence" ,"fence.i" };
vector <string> I_type_offset = { "lb", "lh", "lw","lbu","lhu", "jalr" };
vector <string> R_type = { "add", "sub", "sll", "slt", "sltu", "xor", "srl", "sra", "or", "and" };
vector <string> S_type = { "sb","sh","sw" };
vector <string> B_type = { "beq","bne","blt", "bge","bltu","bgeu" };
vector <string> U_type = { "lui","auipc" };
vector <string> J_type = { "jal" };



int mapping(string name) {
	// The function is used to map the Registers to their index number
	// Parameters :
	//      name: name of the register
	//	Out : the number of the register in Registers array

	map<string, int> mymap;

	mymap["zero"] = 0;
	mymap["ra"] = 1;
	mymap["sp"] = 2;
	mymap["gp"] = 3;
	mymap["tp"] = 4;
	mymap["t0"] = 5;
	mymap["t1"] = 6;
	mymap["t2"] = 7;
	mymap["s0"] = 8;
	mymap["s1"] = 9;
	mymap["a0"] = 10;
	mymap["a1"] = 11;
	mymap["a2"] = 12;
	mymap["a3"] = 13;
	mymap["a4"] = 14;
	mymap["a5"] = 15;
	mymap["a6"] = 16;
	mymap["a7"] = 17;
	mymap["s2"] = 18;
	mymap["s3"] = 19;
	mymap["s4"] = 20;
	mymap["s5"] = 21;
	mymap["s6"] = 22;
	mymap["s7"] = 23;
	mymap["s8"] = 24;
	mymap["s9"] = 25;
	mymap["s10"] = 26;
	mymap["s11"] = 27;
	mymap["t3"] = 28;
	mymap["t4"] = 29;
	mymap["t5"] = 30;
	mymap["t6"] = 31;
	return mymap.at(name);
}


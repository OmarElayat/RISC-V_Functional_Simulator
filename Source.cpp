// writing on a text file
#include "Header.h"

class assembler {
	// Assembler is the class representing the simulators with methods representing actions of the simuator
	int count=0;                              // Number of instructions in File
	string* lines;							  // Instruction file lines
	int pc=0;								  // program counter
	map<string, int> looping_inst_map;		  // map for instruction labels with their address
	map<int,int> MemoryIn_map;                // map of the memory used with their value

public:
	 assembler() 
		 // The function is constructor to the assembler class
	// Parameters : None
	//	Out : None
	 {
		for (int i = 0; i < 32; i++)
		{
			Registers[i] = 0;
		}
		sp = 2147479548;
	}

	void print_Memory_map()
	{    //printing Memory Map for debugging
		cout << "KEY\tELEMENT\n";
		for (auto itr = MemoryIn_map.begin(); itr != MemoryIn_map.end(); ++itr) {
			cout << itr->first << '\t' << itr->second << '\n';
		}
	}
	
	void printmap_loop() {
		//printing label map for debugging
		cout << "KEY\tELEMENT\n";
		for (auto itr = looping_inst_map.begin(); itr != looping_inst_map.end(); ++itr) {
			cout << itr->first << '\t' << itr->second << '\n';
		}
	}

	void execute(int pci) {  
		// This function defines command type from instructions and call compute with operands 
		// Parameters :
		//			pci: number of the current program counter to count on
	    //	Out : None
	string line;
	for (pc= pci; pc < count; pc++)
	{
		line = lines[pc];
		cout << pc << endl;
		if (line[line.length() - 1] != ':') {
			string deli = " ";
			string instruct = line.substr(0, line.find(deli));
			string operands = line.substr(line.find(deli) + 1, line.length());
			//cout << operands << endl << instruct << endl;
			if ((instruct == "ecall") || (instruct == "ebreak") || (instruct == "fence") || (instruct == "fence.i")) {
				break;
			}
			else
			{
				cout << "pc before ex" << pc << endl;
				compute(instruct, operands);
			}
			if (pc == count) {
				break;
			}
			if (Registers[0]) {
				Registers[0] = 0;
			}
			write_mem();
			write_reg();
		}
	}
}
	


	void print() {
		// This function prints current registers values
		// Parameters : None
		//	Out : None

		cout << "My Registers" << endl;
		for (int i = 0; i < 31; i++) {
			cout << "Decimal:" << dec << Registers[i] << endl;
			cout << "Hex: 0x" << hex << Registers[i] << endl;
			bitset<32> b(Registers[i]);
			cout << "Binary:"  << b << endl;

		}
		cout << "pc" << pc;
	}
	void read_file(string filename) {     
		// This function creates an array of strings and each string represents an instruction with its index 
		// Parameters :
		//			filename: name of instruction file
		//	Out : None
		string line;
		count = 0;
		ifstream myfile(filename);
		ifstream myfile2(filename);
		if (myfile.is_open())
		{

			while (getline(myfile, line)) {
				count++;
			}
			myfile.close();
		}

		if (myfile2.is_open()) {
			lines = new string[count];
			int i = 0;
			while (getline(myfile2, line)) {
				lines[i] = line;

				if (lines[i][lines[i].length() - 1] == ':') {
					string label = line.substr(0, line.find(':'));
					
					//cout << label << endl;

					//string label = line.substr(0, line.length() - 1);
					string ii = to_string(i);
					looping_inst_map.insert({ label,i });   //need to push in a vector with a label 
					looping_inst_map.insert({ ii,i });
				
				}
				i++;
			}
			myfile2.close();
			cout << "file readed successfully..." << endl;
		}
		else cout << "Unable to open instrucion file";
	}


	void read_memory(string memoryfile) {
		// This function reads the content of the memory file and save it  
		// Parameters :
		//			filename: name of memory file
		//	Out : None
		string linemem;
		ifstream myfilemem(memoryfile);
		if (myfilemem.is_open())
		{
			
			while (getline(myfilemem, linemem)) {
				string address = linemem.substr( 0,linemem.find(' '));
				string value = linemem.substr(linemem.find(' ')+1, linemem.find('\n'));
				int address2 = stoi(address);
				int value2 = stoi(value);
				MemoryIn_map.insert({ address2,value2 });
				cout << address << endl<< value << endl;
			}
			cout << " memory file" << endl;
			myfilemem.close();
		}
		else { cout << "cannot open memory file" << endl; }
	}


private:
	void compute(string ins_type, string operands) {
		if (in_array(ins_type, R_type)) {
			string rd1 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rd = mapping(rd1);
			string rs11 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs1 = mapping(rs11);
			string rs21 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs2 = mapping(rs21);
			cout << "rs1: " << rs1 << "  " << "rs2: " << rs2 << "  " << "rd: " << rd << endl;
			R_type_compute(ins_type, rd, rs1, rs2);
		}
		else if (in_array(ins_type, I_type)) {

			string rd1 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rd = mapping(rd1);

			string rs11 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs1 = mapping(rs11);

			string immedstring = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int immediate = stoi(immedstring);
			cout << "rs1: " << rs1 << "  " << "imm: " << immediate << "  " << "rd: " << rd << endl;
			I_type_compute(ins_type, rd, rs1, immediate);
		}
		else if (in_array(ins_type, I_type_offset)) {

			string rs11 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs1 = mapping(rs11);

			string immedstring = operands.substr(0, operands.find("("));
			operands = operands.substr(operands.find("(") + 1, operands.length());
			int immediate = stoi(immedstring);

			string rs21 = operands.substr(0, operands.find(")"));
			operands = operands.substr(operands.find(")") + 1, operands.length());
			int rs2 = mapping(rs21);
			cout << "rs1: " << rs1 << "  " << "imm: " << immediate << "  " << "rs2: " << rs2 << endl;
			I_type_offset_compute(ins_type, rs1, rs2, immediate);
		}
		else if (in_array(ins_type, S_type)) {

			string rs11 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs1 = mapping(rs11);

			string immedstring = operands.substr(0, operands.find("("));
			operands = operands.substr(operands.find("(") + 1, operands.length());
			int immediate = stoi(immedstring);

			string rs21 = operands.substr(0, operands.find(")"));
			operands = operands.substr(operands.find(")") + 1, operands.length());
			int rs2 = mapping(rs21);
			cout << "rs1: " << rs1 << "  " << "imm: " << immediate << "  " << "rs2: " << rs2 << endl;
			S_type_compute(ins_type, rs1, rs2, immediate);
		}
		else if (in_array(ins_type, B_type)) {

			string rs11 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs1 = mapping(rs11);
			string rs21 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rs2 = mapping(rs21);

			string imm_address = operands.substr(0, operands.length());
			operands = operands.substr(operands.find(",") + 1, operands.length());
			//int imm_address_hexa = Address_mapping(imm_address);
			
			cout << "rs1: " << rs1 << "  " << "rs2: " << rs2 << "  " << "imm: " << imm_address << endl;
			cout << "values:  "<< Registers[rs1] << "    " << Registers[rs2] << endl;
			B_type_compute(ins_type, rs1, rs2, imm_address);
		}
		else if (in_array(ins_type, U_type)) {
			

			string rd1 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rd = mapping(rd1);

			string immedstring = operands.substr(0, operands.length());
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int immediate = stoi(immedstring);
			
			cout << "imm: " << immediate << "  " << "rd: " << rd << endl;

			U_type_compute(ins_type, rd, immediate);
		}
		else if (in_array(ins_type, J_type)) {
		
			string rd1 = operands.substr(0, operands.find(","));
			operands = operands.substr(operands.find(",") + 1, operands.length());
			int rd = mapping(rd1);

			string imm_address_label = operands.substr(0, operands.length());
			operands = operands.substr(operands.find(",") + 1, operands.length());
						
			//cout << "rd: " << rd << "  " << "imm: " << imm_address_label << endl;

			J_type_compute(rd, imm_address_label);
		}
		else {
			cout << "da5l instruction 3edl ya 3el2" << endl;
		}
	}
	
	void jump_to_address(string address_label) {    
		// This function takes the label of the branch and jumps to it
		// Parameters :
		//			address_label: label that want the program to jump to
		//	Out : None

		if (looping_inst_map[address_label]) {
			execute(looping_inst_map[address_label] + 1);
		}
		else {
			cout << "pc in jump to address" << pc << endl;
			cout << "unable to find branching address" << endl;
		}
	}
	bool in_array(string target, vector<string> array1) {
		// This function defines if string exists in string vector of instruction types or not
		// Parameters :
		//			target: the string to search for
		//			array1: the vector to search in
		//	Out : boolean whether the target exists in array1 or not 
		return find(array1.begin(), array1.end(), target) != array1.end();
	}

	void R_type_compute(string type, int rd, int rs1, int rs2) {
		if (type == "add") {
			Registers[rd] = Registers[rs1] + Registers[rs2];
		}
		else if (type == "sub") {
			Registers[rd] = Registers[rs1] - Registers[rs2];
		}
		else if (type == "sll") {
			Registers[rd] = Registers[rs1] << Registers[rs2];
		}
		else if (type == "slt") {
			if (Registers[rs1] < Registers[rs2]) {
				Registers[rd] = 1;
			}
			else
				Registers[rd] = 0;
		}
		else if (type == "sltu") {
			unsigned int r1 = Registers[rs1];
			unsigned int r2 = Registers[rs2];
			if (r1 < r2) {
				Registers[rd] = 1;
			}
			else
				Registers[rd] = 0;
		}
		else if (type == "xor") {
			Registers[rd] = Registers[rs1] ^ Registers[rs2];
		}
		else if (type == "srl") {
			unsigned int r1 = Registers[rs1];
			unsigned int r2 = Registers[rs2];
			Registers[rd] = Registers[rs1] >> Registers[rs2];
		}
		else if (type == "sra") {
			Registers[rd] = Registers[rs1] >> Registers[rs2];
		}
		else if (type == "or") {
			Registers[rd] = Registers[rs1] | Registers[rs2];
		}
		else if (type == "and") {
			Registers[rd] = Registers[rs1] & Registers[rs2];
		}
		else {
			cout << "R-type instruction doesn't exist" << endl;
		}
	}
	void I_type_compute(string type, int rd, int rs1, int imm) {
		if (type == "addi") {
			Registers[rd] = Registers[rs1] + imm;
		}
		else if (type == "slti") {
			if (Registers[rs1] < imm) {
				Registers[rd] = 1;
			}
			else {
				Registers[rd] = 0;
			}
		}
		else if (type == "sltiu") {
			unsigned int r1 = Registers[rs1];
			unsigned int im_uns = imm;
			if (r1 < im_uns) {
				Registers[rd] = 1;
			}
			else {
				Registers[rd] = 0;
			}
		}
		else if (type == "xori") {
			Registers[rd] = Registers[rs1] ^ imm;
		}
		else if (type == "ori") {
			Registers[rd] = Registers[rs1] | imm;
		}
		else if (type == "andi") {
			Registers[rd] = Registers[rs1] & imm;
		}
		else if (type == "slli") {
			Registers[rd] = Registers[rs1] << imm;
		}
		else if (type == "srli") {
			unsigned int r1 = Registers[rs1];
			unsigned int imm_uns = imm;
			Registers[rd] = r1 >> imm_uns;
		}
		else if (type == "srai") {
			Registers[rd] = Registers[rs1] >> imm;
		}
		else
			cout << "invalid B-type computation" << endl;
	}
	void I_type_offset_compute(string type, int rd, int rs1, int imm) {
		if (type == "lw") {
			//load word
			Registers[rd] = MemoryIn_map[imm+ Registers[rs1]];
		}
		else if (type == "lbu") {
			//load bite 
			int destination = MemoryIn_map[imm + Registers[rs1]];
			Registers[rd] = destination & 0x000000ff;
		}
		else if (type == "lhu") {
			//load bite 
			int destination = MemoryIn_map[imm + Registers[rs1]];
			Registers[rd] = destination & 0x0000ffff;
		}
		else if (type == "lb") {
			int destination = MemoryIn_map[imm + Registers[rs1]];
			destination = destination & 0x000000ff;
			if (destination & 0x80000000) {
				destination = destination | 0xffffff00; 
			}
				Registers[rd] = destination;
		}
		else if (type == "lh") {
			int destination = MemoryIn_map[imm + Registers[rs1]];
			destination = destination & 0x0000ffff;
			if (destination & 0x80000000) {
				destination = destination | 0xffff0000;
			}
			Registers[rd] = destination;
		}
		else if (type == "jalr") {
			Registers[rd] = pc + 1;
			string x = to_string(Registers[rs1]);
			jump_to_address(x);
		}
		
	}
	void S_type_compute(string type, int rs1, int rs2, int imm) {
		if (type == "sw") {
			if (MemoryIn_map[imm + Registers[rs2]]) {
				MemoryIn_map[imm + Registers[rs2]] = Registers[rs1];
			}
			else {
				MemoryIn_map.insert({ imm + Registers[rs2], Registers[rs1] });
			}
		}
		else if (type == "sh") {
			int destination = Registers[rs1];
			destination = destination & 0x0000ffff;
			if (destination & 0x80000000) {
				destination = destination | 0xffff0000;
			}
			if (MemoryIn_map[imm + Registers[rs2]]) {
				MemoryIn_map[imm + Registers[rs2]] = destination;
			}
			else {
				MemoryIn_map.insert({ imm + Registers[rs2], destination });
			}
		}
		else if (type == "sb") {
			int destination = Registers[rs1];
			destination = destination & 0x000000ff;
			if (destination & 0x80000000) {
				destination = destination | 0xffffff00;
			}
			if (MemoryIn_map[imm + Registers[rs2]]) {
				MemoryIn_map[imm + Registers[rs2]] = destination;
			}
			else {
				MemoryIn_map.insert({ imm + Registers[rs2], destination });
			}
		}
	}
	void B_type_compute(string type, int rs1, int rs2, string address_name) {
		// beq bne blt bge bltu bgeu
		if (type == "beq") {
			//equal
			if (Registers[rs1] == Registers[rs2]) {
				jump_to_address(address_name);
			}
		}
		else if (type == "bne") {
			// not equal
			if (Registers[rs1] != Registers[rs2]) {
				jump_to_address(address_name);
			}
		}
		else if (type == "blt") {
			// less than
			if (Registers[rs1] < Registers[rs2]) {
				jump_to_address(address_name);
			}
		}
		else if (type == "bge") {
			// greater than or equal
			cout << "greater than or equal" << endl;
			if (Registers[rs1] >= Registers[rs2]) {
				jump_to_address(address_name);
			}
		}
		else if (type == "bltu") {
			//less than unsigned
			unsigned int rs1unsign = Registers[rs1];
			unsigned int rs2unsign = Registers[rs2];
			if (rs1unsign < rs2unsign) {
				jump_to_address(address_name);
			}
		}
		else if (type == "bgeu") {
			//greater than or equal unsigned
			unsigned int rs1unsign = Registers[rs1];
			unsigned int rs2unsign = Registers[rs2];
			if (rs1unsign >= rs2unsign) {
				jump_to_address(address_name);
			}
		}
		else {
			cout << "invalid B-type computation" << endl;
		}

	}
	void U_type_compute(string type, int rd, int imm) {
		if (type == "lui") {
			// Load Upper Immediate
			Registers[rd] = imm & 0xfffff000;
		}
		else if (type == "auipc") {
			Registers[rd] = (imm & 0xfffff000) + (pc * 4) + MemoryIn_map[0001];
		}
	}
	void J_type_compute(int rd, string address_name) {
		Registers[rd] = pc + 1;
		jump_to_address(address_name);
	}

	void write_reg() {
		ofstream outfile;
		outfile.open("RegisterOutput.txt", ios::app);
		if (outfile.is_open()) {
			outfile << "................................." << endl;
			for (int i = 0; i < 32; i++) {
				outfile << Reg_num[i] << "  Decimal: " << dec << Registers[i] << endl;
				outfile << "    Hex: 0x" << hex << Registers[i] << endl;
				bitset<32> b(Registers[i]);
				outfile << "    Binary: " << b << endl;
			}
			int pcindex = (pc * 4) + MemoryIn_map[0001];
			outfile << "pc" << "  Decimal: " << dec << pcindex << endl;
			outfile << "    Hex: 0x" << hex << pcindex << endl;
			bitset<32> b(pcindex);
			outfile << "    Binary: " << b << endl;
			outfile.close();
		}
		else
			cout << "cann't open the output file";
	}
	void write_mem() {
		ofstream outfile;
		outfile.open("MemoryOutput.txt", ios::app);
		if (outfile.is_open()) {
			outfile << "................................." << endl;
			for (auto itr = MemoryIn_map.begin(); itr != MemoryIn_map.end(); ++itr) {
				outfile << dec << itr->first << '\t' << "  Decimal: " << dec << itr->second << '\n';
				outfile << '\t' << "  Hex: 0x" << hex << itr->second << endl;
				bitset<32> b(itr->second);
				outfile << '\t' << "  Binary: " << b << endl;
			}
			outfile.close();
		}
		else
			cout << "cann't open the output file";
	}

};


int main() {

	assembler as;

	as.read_file("prog1.txt");
	as.read_memory("mem1.txt");
	as.execute(0);
	as.print();
	//as.write_reg();
	//as.write_mem();
	return 0;
}
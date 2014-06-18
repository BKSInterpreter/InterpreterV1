#include "systemc.h"
#include <string>
//#include <fstream>
#include <iostream>

#include "sp_ram.cpp"
//#include "adder.cpp"

//#include "shared_parameters.h"

//I would very much like to implement a less hackish system
//for parsing the input strings.
int parse(std::string lng){
	if(lng == OW_NOP){
		return OP_NOP;
	}
	if(lng == OW_ABS){
		return OP_ABS;
	}
	if(lng == OW_ADD){
		return OP_ADD;
	}
	if(lng == OW_SUB){
		return OP_SUB;
	}
	if(lng == OW_AND){
		return OP_AND;
	}
	if(lng == OW_ROR){
		return OP_OR;
	}
	if(lng == OW_XOR){
		return OP_XOR;
	}
	if(lng == OW_NOR){
		return OP_NOR;
	}
	if(lng == OW_SLL){
		return OP_SLL;
	}
	if(lng == OW_SRL){
		return OP_SRL;
	}
	if(lng == OW_SRA){
		return OP_SRA;
	}
	if(lng == OW_INT_SMUL){
		return OP_INT_SMUL;
	}
	if(lng == OW_INT_UMUL){
		return OP_INT_UMUL;
	}
	if(lng == OW_INT_FSMUL){
		return OP_INT_FSMUL;
	}
	if(lng == OW_INT_FUMUL){
		return OP_INT_FUMUL;
	}
	cout << "Error. Opstring not recognized.";
	return 0;
}

int sc_main(int argc, char* argv []){
	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
	
	if(argc != 3){
		cout << "Basic adder and memory simulation.\n";
		cout << "Argument 1: Location of instruction file.\n";
		cout << "Argument 2: Location of data file.\n";
		return 0;
	}
	
	const char * ramfile = argv[2];
	
	//Now that we have the size of our ram, we can actually construct
	//the modules we will be simulating.
	
	sc_signal<sc_lv<DATA_WIDTH> > address1;
	sc_signal<sc_lv<DATA_WIDTH> > address2;
	sc_signal<sc_lv<DATA_WIDTH> > opA;
	sc_signal<sc_lv<DATA_WIDTH> > opB;
	sc_signal<sc_lv<DATA_WIDTH> > alu_op_result;
	sc_signal<sc_lv<DATA_WIDTH*2> > mul_op_result;
	sc_signal<sc_lv<INSTRUCTION_WIDTH> > opcode;
	sc_signal<bool> overflow;
	sc_signal<bool> enable_adder;
	sc_signal<bool> enable_ram;
	sc_signal<bool> clock;

	alu Adder("ADDER");
		Adder.alu_op_result(alu_op_result);
		Adder.mul_op_result(mul_op_result);
		Adder.opcode(opcode);
		Adder.opA(opA);
		Adder.opB(opB);
		Adder.overflow(overflow);
		Adder.enable_adder(enable_adder);
		Adder.clock(clock);

	sp_ram Memory("MEM", ramfile);
		Memory.opA(opA);
		Memory.opB(opB);
		Memory.address1(address1);
		Memory.address2(address2);
		Memory.enable_ram(enable_ram);
		Memory.clock(clock);

	//Set up monitoring.
	sc_trace_file *wave = sc_create_vcd_trace_file("mem_and_add");
	sc_trace(wave, alu_op_result, "alu_op_result");
	sc_trace(wave, mul_op_result, "mul_op_result");
	sc_trace(wave, opA, "opA");
	sc_trace(wave, opB, "opB");
	sc_trace(wave, overflow, "overflow");
	sc_trace(wave, enable_adder, "enable_adder");
	sc_trace(wave, enable_ram, "enable_ram");
	sc_trace(wave, clock, "clock");
	sc_trace(wave, address1, "address1");
	sc_trace(wave, address2, "address2");
		
	sc_start(1);

	//Open the instructions file.
	const char * instfile = argv[1];
	std::fstream instructions_list;
	instructions_list.open(instfile);
	if(!(instructions_list.is_open())){
		cout << "ERROR: instruction file " << instfile;
		cout << " does not exist";
		cout << " or cannot be opened.\n";
		return 0;
	}

	//Turn us on...
	enable_ram = true;
	enable_adder = true;

	//Read and begin executing instructions.
	std::string ln;
	while(true){
		std::string ln;
		getline(instructions_list, ln);
		if(instructions_list.eof()){
			break;
		}
		
		std::string opstr = ln.substr(0,
		ln.find_first_of(" ")).c_str();
		
		int add1 = std::stoi(ln.substr(ln.find_first_of(" ") + 1,
		ln.find_first_of(",")).c_str());
		
		int add2 = std::stoi(ln.substr(ln.find_first_of(",")+1,
		ln.length()-1).c_str());
		
		address1.write(add1);
		address2.write(add2);
		opcode.write(parse(opstr));
		
		clock.write(0);
		sc_start(1);
		clock.write(1);
		sc_start(1);
	}
	clock.write(0);
	sc_start(1);
	clock.write(1);
	sc_start(1);
	sc_close_vcd_trace_file(wave);
}

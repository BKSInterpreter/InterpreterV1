#include "systemc.h"
#include "shared_parameters.h"
SC_MODULE(alu){
	sc_in<sc_lv<INSTRUCTION_WIDTH> > opcode;
	sc_out<sc_lv<DATA_WIDTH> > alu_op_result;
	sc_out<sc_lv<DATA_WIDTH*2> > mul_op_result;
	sc_in<sc_lv<DATA_WIDTH> > opA;
	sc_in<sc_lv<DATA_WIDTH> > opB;
	sc_out<bool> overflow;
	sc_in<bool> enable_alu;
	sc_in<bool> enable_mul;
	
	sc_in_clk clock;

	SC_CTOR(alu){
		SC_METHOD(think);
		sensitive << clock.pos();
	}
	
	void think(){	
		int code = opcode.read().to_uint();
		//cout << "\n" << code << ", " << OP_INT_SMUL << "\n";
		switch(code){
			case OP_NOP:
				if(enable_alu.read()){
					nop();
				}
				break;
			case OP_ABS:
				if(enable_alu.read()){
					abs();
				}
				break;
			case OP_ADD:
				if(enable_alu.read()){
					add();
				}
				break;
			case OP_SUB:
				if(enable_alu.read()){
					sub();
				}
				break;
			case OP_AND:
				if(enable_alu.read()){
					land();
				}
				break;
			case OP_OR:
				if(enable_alu.read()){
					lor();
				}
				break;
			case OP_XOR:
				if(enable_alu.read()){
					lxor();
				}
				break;
			case OP_NOR:
				if(enable_alu.read()){
					lnor();
				}
				break;
			case OP_SLL:
				if(enable_alu.read()){
					sll();
				}
				break;
			case OP_SRL:
				if(enable_alu.read()){
					srl();
				}
				break;
			case OP_SRA:
				if(enable_alu.read()){
					sra();
				}
				break;
			case OP_INT_SMUL:
				if(enable_mul.read()){
					signmul();
				}
				break;
			case OP_INT_UMUL:
				if(enable_mul.read()){
					unsignmul();
				}
				break;
			case OP_INT_FSMUL:
				if(enable_mul.read()){
					fsignmul();
				}
				break;
			case OP_INT_FUMUL:
				if(enable_mul.read()){
					funsignmul();
				}
				break;
		}
	}
	
	void nop(){
		cout << alu_op_result.read().to_int();
		cout << "\n";
		overflow.write(false);
		cout << opA.read().to_int() << " _ ";
		cout << opB.read().to_int() << " = ";
		alu_op_result.write(opA.read());
	}
	
	void abs(){
		cout << opA.read().to_int() << " || ";
		cout << opB.read().to_int() << " = ";
		int tmp = std::abs(opA.read().to_int());
		overflow.write(false);
		alu_op_result.write(tmp);
		cout << alu_op_result.read().to_int();
		cout << "\n";
	}
	
	void add(){
	cout << opA.read().to_int() << " + ";
		cout << opB.read().to_int() << " = ";
		int tmp = opA.read().to_int() + opB.read().to_int();
		if(tmp > (pow(2, DATA_WIDTH-1) - 1) || tmp < -1 * pow(2, DATA_WIDTH-1)){
			overflow.write(true);
			alu_op_result.write(tmp);
		}
		else{
			overflow.write(false);
			alu_op_result.write(tmp);
		}
		cout << alu_op_result.read().to_int();
		cout << "\n";
	}
	
	void sub(){
		cout << opA.read().to_int() << " - ";
		cout << opB.read().to_int() << " = ";
		int tmp = opA.read().to_int() - opB.read().to_int();
		if(tmp > (pow(2, DATA_WIDTH-1) - 1) || tmp < -1 * pow(2, DATA_WIDTH-1)){
			overflow.write(true);
			alu_op_result.write(tmp);
		}
		else{
			overflow.write(false);
			alu_op_result.write(tmp);
		}
		cout << alu_op_result.read().to_int();
		cout << "\n";
	}
	
	void land(){
		cout << opA.read() << " & ";
		cout << opB.read() << " = ";
		overflow.write(false);
		alu_op_result = opA.read() & opB.read();
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void lor(){
		cout << opA.read() << " | ";
		cout << opB.read() << " = ";
		overflow.write(false);
		alu_op_result = opA.read() | opB.read();
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void lxor(){
		cout << opA.read() << " ^ ";
		cout << opB.read() << " = ";
		overflow.write(false);
		alu_op_result = opA.read() ^ opB.read();
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void lnor(){
		cout << opA.read() << " !| ";
		cout << opB.read() << " = ";
		overflow.write(false);
		alu_op_result = ~(opA.read() | opB.read());
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void sll(){
		cout << opA.read() << " << ";
		cout << opB.read() << " = ";
		overflow.write(false);
		int tmp = opA.read().to_int() << opB.read().to_int();
		//Going to redo these later so that sizes greater than the c++ int work.
		alu_op_result.write(tmp);
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void srl(){
		cout << opA.read() << " >> ";
		cout << opB.read() << " = ";
		overflow.write(false);
		int tmp = opA.read().to_int() >> opB.read().to_int();
		//Going to redo these later so that sizes greater than the c++ int work.
		alu_op_result.write(tmp);
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void sra(){//Arithmetic right shifts are
	//a little bit bizarre, so I'm doing this
	//manually (i.e. not with ints).
		cout << opA.read() << " >>> ";
		cout << opB.read() << " = ";
		overflow.write(false);
		sc_lv<DATA_WIDTH> tmp = opA.read();
		for(int i = 0; i < opB.read().to_int(); i++){
			for(int i = 0; i < DATA_WIDTH -1; i ++){
				tmp[i+1] = tmp[i];
			}
		}
		alu_op_result.write(tmp);
		cout << alu_op_result.read();
		cout << "\n";
	}
	
	void signmul(){
		cout << opA.read().to_int() << " * ";
		cout << opB.read().to_int() << " = ";
		overflow.write(false);
		int tmp = opA.read().to_int() * opB.read().to_int();
		mul_op_result.write(tmp);
		cout << alu_op_result.read().to_int();
		cout << "\n";
	}
	
	void unsignmul(){
		cout << opA.read().to_uint() << " |*| ";
		cout << opB.read().to_uint() << " = ";
		overflow.write (false);
		unsigned int tmp = opA.read().to_uint() * opB.read().to_uint();
		mul_op_result.write(tmp);
		cout << alu_op_result.read().to_uint();
		cout << "\n";
	}
	
	void fsignmul(){
		cout << opA.read().to_int() << " .* ";
		cout << opB.read().to_int() << " = ";
		overflow.write(false);
		int tmp = (opA.read().to_int() * opB.read().to_int()) >> 15;
		mul_op_result.write(tmp);
		cout << alu_op_result.read().to_int();
		cout << "\n";
	}
	
	void funsignmul(){
		cout << opA.read().to_int() << " |.*| ";
		cout << opB.read().to_int() << " = ";
		overflow.write(false);
		unsigned int tmp = (opA.read().to_uint() * opB.read().to_uint()) >> 15;
		mul_op_result.write(tmp);
		cout << alu_op_result.read().to_uint();
		cout << "\n";
	}
};

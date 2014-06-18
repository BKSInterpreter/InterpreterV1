#include "systemc.h"
#include "shared_parameters.h"
SC_MODULE(alu){
	sc_in<sc_lv<INSTRUCTION_WIDTH> > opcode;
	sc_out<sc_lv<DATA_WIDTH> > output;
	sc_in<sc_lv<DATA_WIDTH> > opA;
	sc_in<sc_lv<DATA_WIDTH> > opB;
	sc_out<bool> overflow;
	sc_in<bool> enable_adder;
	
	sc_in_clk clock;

	SC_CTOR(alu){
		SC_METHOD(think);
		sensitive << clock.pos();
	}
	
	void think(){
		if(enable_adder){
			unsigned int code = opcode.read().to_uint();
			cout << "\n" << code << "\n";
			switch(code){
				case 0:
					nop();
					break;
				case 1:
					abs();
					break;
				case 10:
					add();
					break;
				case 11:
					sub();
					break;
				case 4:
					land();
					break;
				case 5:
					lor();
					break;
				case 14:
					lxor();
					break;
				case 15:
					lnor();
					break;
				case 8:
					sll();
					break;
				case 9:
					srl();
					break;
			}
		}
	}
	
	void nop(){
		overflow.write(false);
		output.write(opA.read());
	}
	
	void abs(){
		int tmp = std::abs(opA.read().to_int());
		overflow.write(false);
		output.write(tmp);
	}
	
	void add(){
		int tmp = opA.read().to_int() + opB.read().to_int();
		if(tmp > (pow(2, DATA_WIDTH-1) - 1) || tmp < -1 * pow(2, DATA_WIDTH-1)){
			overflow.write(true);
			output.write(tmp);
		}
		else{
			overflow.write(false);
			output.write(tmp);
		}
	}
	
	void sub(){
		int tmp = opA.read().to_int() - opB.read().to_int();
		if(tmp > (pow(2, DATA_WIDTH-1) - 1) || tmp < -1 * pow(2, DATA_WIDTH-1)){
			overflow.write(true);
			output.write(tmp);
		}
		else{
			overflow.write(false);
			output.write(tmp);
		}
	}
	
	void land(){
		overflow.write(false);
		output = opA.read() & opB.read();
	}
	
	void lor(){
		overflow.write(false);
		output = opA.read() | opB.read();
	}
	
	void lxor(){
		overflow.write(false);
		output = opA.read() ^ opB.read();
	}
	
	void lnor(){
		overflow.write(false);
		output = ~(opA.read() | opB.read());
	}
	
	void sll(){
		overflow.write(false);
		int tmp = opA.read().to_int() << opB.read().to_int();
		//Going to redo these later so that sizes greater than the c++ int work.
		output.write(tmp);
	}
	
	void srl(){
		overflow.write(false);
		int tmp = opA.read().to_int() >> opB.read().to_int();
		//Going to redo these later so that sizes greater than the c++ int work.
		output.write(tmp);
	}
};

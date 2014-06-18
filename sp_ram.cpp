#include "systemc.h"
#include <string>
#include <iostream>
//#include "shared_parameters.h"
#include "alu.cpp"
//Addressable ram of variable
//size, currently can only be
//"populated" from a text file
//so is essentially read-only.

SC_MODULE(sp_ram){

	sc_in<bool> clock;
	sc_in<bool> enable_ram;
	sc_in<sc_lv<DATA_WIDTH> > address1;
	sc_out<sc_lv<DATA_WIDTH> > opA;
	sc_in<sc_lv<DATA_WIDTH> > address2;
	sc_out<sc_lv<DATA_WIDTH> > opB;
	
	SC_HAS_PROCESS(sp_ram);

	public:
	void write(int address, int value){
		//Since this is extremely temporary,
		//there isn't much error handling or
		//durability. I'll just let the top deal
		//with writing to non-extant addresses.
		values[address] = value;
	}

	sp_ram(sc_module_name name_, std::string filename) : sc_module(name_){
	//Constructor accepts a name and a size for the ram, default 32 ints.
		std::fstream numbers_list;
		numbers_list.open(filename);
		if(!(numbers_list.is_open())){
			cout << "ERROR: memory file ";
			cout << filename << " does not exist";
			cout << " or cannot be opened.\n";
			size = 0;
		}
	
		else{
			std::string tmp;//A more rigorous way to determine ram size.
			getline(numbers_list, tmp);
			size = std::stoi(tmp);
		}
		
		SC_METHOD(access);
		sensitive << clock.pos();
		values = new int [size];
		//Begin with the ram filled with 0s. Later on we could
		//throw some kind of warning or error, or implement
		//randomly-generated garbage.
		for(int i = 0; i < size; i ++){
			values[i] = 0;
		}
		
		while(true){
			//Based on the string-parsing
			//code in the CWRUCutter waypoint finder.
			//Therefore, probably a bit more
			//complicated than it needs to be.
			std::string ln;
			getline(numbers_list, ln);
			if(numbers_list.eof()){
				break;
			}
			int address = std::stoi(ln.substr(0,
			ln.find_first_of(" ")).c_str());
			int value = std::stoi(ln.substr(ln.find_first_of(" ")+1,
			ln.length()-1).c_str());
			if(address < size){
				this->write(address, value);
			}
			else{
				cout << "Index " << address;
				cout << " is out of bounds!\n";
			}
		}
		numbers_list.close();
	}
	
	private:
		int* values;
		int size;
	
	void access(){
		if(enable_ram.read()){
			opA.write(values[address1.read().to_int()]);
			opB.write(values[address2.read().to_int()]);
		}
	}
};

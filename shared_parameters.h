#include "systemc.h"

//Can't do this at runtime ATM, but want to change that later.
const int DATA_WIDTH = 32;

const int INSTRUCTION_WIDTH = 4;

//Instruction words.
//Would later like to implement as a more efficient
//form, but for now this will do.
const std::string OW_NOP = "_";
const std::string OW_ABS = "||";
const std::string OW_ADD = "+";
const std::string OW_SUB = "-";
const std::string OW_AND = "&";
const std::string OW_ROR =  "|";
const std::string OW_XOR = "^";
const std::string OW_NOR = "!|";
const std::string OW_SLL = "<<";
const std::string OW_SRL = ">>";
const std::string OW_SRA = ">>>";

const std::string OW_INT_SMUL = "*";
const std::string OW_INT_UMUL = "|*|";
const std::string OW_INT_FSMUL = ".*";
const std::string OW_INT_FUMUL = "|.*|";

//Same thing with opcodes.
enum{
	OP_NOP,
	OP_ABS,
	OP_ADD,
	OP_SUB,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_NOR,
	OP_SLL,
	OP_SRL,
	OP_SRA,
	OP_INT_SMUL,
	OP_INT_UMUL,
	OP_INT_FSMUL,
	OP_INT_FUMUL
};

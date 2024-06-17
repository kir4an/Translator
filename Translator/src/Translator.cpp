#include "Translator.h"
#include <ranges>

LL::LL(std::istream &stream,
	   const std::string &Graph = "",
	   const std::string &Atoms = "",
	   const std::string &Asm = "") : lexer{stream} {

	_graph = Graph;
	_atomsInput = Atoms;
	_asmOut = Asm;

	clearStream(_graph);
	clearStream(_atomsInput);
	clearStream(_asmOut);
}

// Function to say if your MiniC code contains Error
bool LL::checkCorrectSyntax() {

	std::cout << "   Checked code" << std::endl;
	std::cout << "=====================" << std::endl << std::endl;

    getToken();
	graphIterator = states.begin() + 1;

	stackFromContext.push(currentContext);

	bool result = StmtList();
	if (result && it->first == "eof") {
		CodeIsChecked = true;
		CodeIsCorrect = true;

		std::cout << "  Syntax Correct! " << std::endl;


		return true;
	} else {
		CodeIsChecked = true;
		CodeIsCorrect = false;

        std::cout << "--------------------------" << std::endl << std::endl;
		std::cout << "Input error!" << std::endl;
		return false;
	}
}


void LL::printGraph() {

	if (_graph.empty()) {
		std::cout << "Name for GraphOutput file is not set" << std::endl;
		return;
	}

	if (!CodeIsChecked) {
		if (!checkCorrectSyntax()) return;
	}

	if (!CodeIsCorrect) return;

	_printGraph();
    std::cout << "--------------------------" << std::endl << std::endl;
	std::cout << "    Graph Printed!" << std::endl;

}

// Function to print Atoms of your code
void LL::printAtoms() {

	if (_atomsInput.empty()) {
		std::cout << "Name for AtomsOutput file is not set" << std::endl;
		return;
	}

	if (!CodeIsChecked) {
		if (!checkCorrectSyntax()) return;
	}

	if (!CodeIsCorrect) return;

	if (!_printAtoms()) return;
    std::cout << "--------------------------" << std::endl << std::endl;
	std::cout << "    Atoms Printed!" << std::endl;
    std::cout << "--------------------------" << std::endl << std::endl;
}

// Function to generate ASM i8080 code
void LL::printASMCode() {

	if (_asmOut.empty()) {
		std::cout << "Name for ASMOutput file is not set" << std::endl;
		return;
	}

	if (!CodeIsChecked) {
		if (!checkCorrectSyntax()) return;
	}

	if (!CodeIsCorrect) return;

	if (!_printASMCode()) return;

	std::cout << "  ASM print!" << std::endl;
    std::cout << "--------------------------" << std::endl << std::endl;
}


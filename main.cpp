#include "Translator.h"

int main() {
	std::fstream INPUT_PATH("../InputCode.minic");
	const std::string GRAPH_PATH = "../GraphExpr.graph";
	const std::string ATOMS_PATH = "../ResultAtom.atom";
	const std::string ASM_PATH = "../ASMcode.asm";

	LL Translator(INPUT_PATH, GRAPH_PATH, ATOMS_PATH, ASM_PATH);

    Translator.checkCorrectSyntax();
    Translator.printGraph();
	Translator.printAtoms();
	Translator.printASMCode();
}

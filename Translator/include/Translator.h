#ifndef TVSU_TRANSLATOR_TRANSLATOR_H
#define TVSU_TRANSLATOR_TRANSLATOR_H

#include "Lexer.h"
#include <stack>
#include <iostream>
#include <fstream>

using FT = std::pair<bool, std::string>;

struct Atom {
	std::string context;
	std::string text;
	std::string first;
	std::string second;
	std::string third;
};

struct VarOrFunc {
	std::string name;
	std::string scope;
	std::string type;
	std::string init;
	std::string kind;
    std::string length;
	int cnt;
};

class LL {
public:
	// Constructor.
	explicit LL(std::istream&, const std::string&, const std::string&, const std::string&);

	// Function to say if your MiniC code contains Error
	bool checkCorrectSyntax();

	// Function to print graph of your code
	void printGraph();

	// Function to print Atoms of your code
	void printAtoms();

	// Function to generate ASM i8080 code
	void printASMCode();

private:

	// context vector
	std::stack<std::string> stackFromContext;
    std::stack<std::string> paramStack;

	// Checker Check
	bool CodeIsChecked = false;
	bool CodeIsCorrect = false;
	bool AtomsGenerated = false;
	bool SemanticError = false;

	// clears files
	void clearStream(const std::string &);

	// Private function to print Atoms
	bool _printAtoms();
	bool createAtomsResult();

	// Private function to print ASM i8080 code
	bool _printASMCode();

	// Private function to print Output Graph
	void _printGraph();

	// ASM i8080 helpful functions
	void loadOp(const std::string &, int);
	void saveOp(const std::string &, int);

	// variables for (MUL)
	std::string MUL_LABEL_START;
	std::string MUL_LABEL_END;
	std::string MUL_LABEL_ZERO;

	// ASM i8080 translating functions
	void MOV(const Atom &);
	void LBL(const Atom &);
	void JMP(const Atom &);
	void ADD(const Atom &);
	void SUB(const Atom &);
	void OR(const Atom &);
	void AND(const Atom &);
	void EQ(const Atom &);
	void NE(const Atom &);
	void GT(const Atom &);
	void LT(const Atom &);
	void GE(const Atom &);
	void LE(const Atom &);
	void NOT(const Atom &);
	void MUL(const Atom &);
	void IN(const Atom &);
	void OUT(const Atom &);

	// Not implemented, because we don't have good i8080 emulator
	// You can do it yourself if you want to though
	void CALL(const Atom &);
	void PARAM(const Atom &);
	void RET(const Atom &);

	// Entry point holder. If not found = "NoEntry"
	std::string entryPoint = "NoEntry";
	std::string currentContext = "-1";

	// Needful variables
	std::string _atomsInput; // path to Atoms output file
	std::string _asmOut; // path to ASM i8080 output file
	std::string _graph; // path to output graph output file
	std::ofstream basicStream; // path to the input file

	std::vector<Atom> atoms; // Contains all the atoms we generated
	std::vector<VarOrFunc> sortedAtomsVector; // contains sorted list of variables that we will need to allocate

	int LabelCnt = 1; // Variable to know which labels we already used
	int NewVarCnt = 1; // Variable to create new tempVariables

	std::map<std::string, std::vector<VarOrFunc>> AtomicMap; // Map: Context -> all the (functions or variables)
	int AtomicMapCnt = 0; // Amount of... idk... I forgot :(

	std::vector<std::string> outputVector; // Vector that stores all the strings of output graph
	std::vector<int> states; // states to understand which string to generate for graph
	std::vector<int>::iterator graphIterator = states.begin(); // Iterator of my graph vector

	std::vector<Token> lexemVector; // All the Tokens from Lexer we store here
	std::vector<Token>::iterator it = lexemVector.begin(); // Iterator for that vector

	int outVecCnt = 0; // The shift, if I remember correctly

	// Translation to Atoms Language
	std::string newLabel();

	std::string alloc(const std::string &scope);

	std::string
	addVar(const std::string &name, const std::string &scope, const std::string &type, const std::string &init = "0");

	std::string addFunc(const std::string &name, const std::string &type, const std::string &length = "0");

	std::string checkVar(const std::string &name);

	std::string checkFunc(const std::string &name, const std::string &len);

	void createAtom(const std::string &context, const std::string &text, const std::string &first,
                    const std::string &second, const std::string &third);


	void getNextState(const int &a);
	void returnPrevNode();
	void createString(const std::string &TextToAdd);


	Lexer lexer;
	void getToken();


	bool StmtList();
	bool Stmt();
	bool DeclareStmt();
	bool DeclareStmtList(const std::string &q, const std::string &r);
	bool AppointCallOp();
	bool AppointOrCall();
	bool AppointList(const std::string &name);
	bool WhileOp();
	bool ForOp();
	bool ForInit();
	FT ForExp();
	bool ForLoop();
	bool IfOp();
	bool ElsePart();
	bool SwitchOp();
	bool Cases(const std::string &p, const std::string &end);
	bool CasesList(const std::string &p, const std::string &end, const std::string &def);
	FT ACase(const std::string &p, const std::string &end);
	bool InOp();
	bool OutOp();
	bool OutOpList();
	FT Type();
	bool DeclareVarList(const std::string &type);
	bool InitVar(const std::string &r, const std::string &s);
	FT ParamList();
	FT ParamListList();

	// Expression checker
	FT Expr();
	FT Expr7();
	FT Expr7List(const std::string &funcID);
	FT Expr6();
	FT Expr6List(const std::string &funcID);
	FT Expr5();
	FT Expr5List(const std::string &funcID);
	FT Expr4();
	FT Expr4List(const std::string &funcID);
	FT Expr3();
	FT Expr3List(const std::string &funcID);
	FT Expr2();
	FT Expr1();
	FT Expr1List(const std::string &funcID);
	FT ArgList();
	FT ArgListList();
};

#endif //TVSU_TRANSLATOR_TRANSLATOR_H

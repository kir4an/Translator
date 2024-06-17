#include "Translator.h"

// Private function to print ASM i8080 code
bool LL::_printASMCode() {
	if (SemanticError) return false;
	if (!createAtomsResult()) return false;

	if (entryPoint == "NoEntry") {

		std::cout << "##########################" << std::endl;
		std::cout << "      No Entry Point!     " << std::endl;
		std::cout << "##########################" << std::endl << std::endl;

		return false;
	}

	basicStream.open(_asmOut);

	basicStream << "ORG 8000H" << std::endl << std::endl;

	for (const auto &i: sortedAtomsVector) {
		if (i.kind == "var" && i.scope == "-1") {
			if (i.type == "kwint") {
				basicStream << "int_dev_" + i.name << ": db ";
				if (i.init.empty()) basicStream << "0" << std::endl;
				else basicStream << i.init << "" << std::endl;
			} else {
				basicStream << "char_dev_" + i.name << ": db ";
				if (i.init.empty()) basicStream << "0" << std::endl;
				else basicStream << (int) i.init[0] << std::endl;
			}
		}
	}

	basicStream << std::endl << "ORG 0" << std::endl << std::endl;
    basicStream << "LXI H, 0" << std::endl;
    basicStream << "SPHL" << std::endl;
    basicStream << "PUSH B" << std::endl;
    basicStream << "CALL main" << std::endl;
    basicStream << "END" << std::endl << std::endl;
    basicStream << "@ MULT ..." << std::endl;
    basicStream << "@ DIV ..." << std::endl;
    basicStream << "@ PRINT ..." << std::endl << std::endl;

    std::string prevCont = "-1";

	for (const auto &atom: atoms) {

        if (prevCont != atom.context) {
            std::string name;
            int n = 0;
            int m = 0;

            for (const auto &i : AtomicMap["-1"]) {
                if (i.cnt == stoi(atom.context)) {
                    name = i.name;
                    n = stoi(i.length);
                    break;
                }
            }

            basicStream << name << ":" << std::endl << std::endl;

            for (const auto &i : sortedAtomsVector) {
                if (i.scope == atom.context && i.kind == "var") m++;
            }

            for (int i = 0; i < m - n; i++) {
                basicStream << "PUSH B" << std::endl;
            }

            basicStream << std::endl;

            prevCont = atom.context;
        }

		// Atom translation functions
		if (atom.text == "MOV") MOV(atom); // done
		else if (atom.text == "LBL") LBL(atom); // done
		else if (atom.text == "JMP") JMP(atom); // done
		else if (atom.text == "ADD") ADD(atom); // done
		else if (atom.text == "SUB") SUB(atom); // done
		else if (atom.text == "OR") OR(atom); // done
		else if (atom.text == "AND") AND(atom); // done
		else if (atom.text == "EQ") EQ(atom); // done
		else if (atom.text == "NE") NE(atom); // done
		else if (atom.text == "GT") GT(atom); // done
		else if (atom.text == "LT") LT(atom); // done
		else if (atom.text == "GE") GE(atom); // done
		else if (atom.text == "LE") LE(atom); // done
		else if (atom.text == "NOT") NOT(atom); // done
		else if (atom.text == "MUL") MUL(atom); // done

		else if (atom.text == "IN") IN(atom);  // done
		else if (atom.text == "OUT") OUT(atom);  // done

        // It is really complicated
		else if (atom.text == "CALL") CALL(atom);  // partially done
		else if (atom.text == "PARAM") PARAM(atom);  // done
		else if (atom.text == "RET") RET(atom);  // partially done
	}

    basicStream.close();
	return true;
}

// ASM i8080 helpful functions
void LL::loadOp(const std::string &atom, int shift = 0) {
	if (atom.starts_with('\'')) {

        std::string oper;

        auto iter = sortedAtomsVector.rbegin();
        while (("'" + std::to_string(iter->cnt) + "'") != atom) {
            shift += 2;
            iter++;
        }

        oper = iter->type.substr(2, iter->type.size()) + "_dev_" + iter->name;

        if (iter->scope == "-1") {
            basicStream << "LDA " + oper << std::endl;
        } else {
            basicStream << "LXI H, " << shift << std::endl;
            basicStream << "DAD SP" << std::endl;
            basicStream << "MOV A, M" << std::endl;
        }
	} else {
		basicStream << "MVI A, " << atom << std::endl;
	}
}

void LL::saveOp(const std::string &atom, int shift = 0) {

    std::string oper;

    auto iter = sortedAtomsVector.rbegin();
    while (("'" + std::to_string(iter->cnt) + "'") != atom) {
        shift += 2;
        iter++;
    }

    oper = iter->type.substr(2, iter->type.size()) + "_dev_" + iter->name;

    if (iter->scope == "-1") {
        basicStream << "STA " + oper << std::endl;
    } else {
        basicStream << "LXI H, " << shift << std::endl;
        basicStream << "DAD SP" << std::endl;
        basicStream << "MOV M, A" << std::endl;
    }
}

// ASM i8080 translation functions
void LL::MOV(const Atom &atom) {
	loadOp(atom.first);
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::LBL(const Atom &atom) {
	basicStream << atom.third + ":" << std::endl;
	basicStream << std::endl;
}

void LL::JMP(const Atom &atom) {
	basicStream << "JMP " + atom.third << std::endl;
	basicStream << std::endl;
}

void LL::ADD(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "ADD B" << std::endl;
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::SUB(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "SUB B" << std::endl;
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::OR(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "ORA B" << std::endl;
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::AND(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "ANA B" << std::endl;
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::EQ(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "CMP B" << std::endl;
	basicStream << "JZ " + atom.third << std::endl << std::endl;
}

void LL::NE(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "CMP B" << std::endl;
	basicStream << "JNZ " + atom.third << std::endl << std::endl;
}

void LL::GT(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "CMP B" << std::endl;
	basicStream << "JP " + atom.third << std::endl << std::endl;
}

void LL::LT(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "CMP B" << std::endl;
	basicStream << "JM " + atom.third << std::endl << std::endl;
}

void LL::GE(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "CMP B" << std::endl;
	basicStream << "JP " + atom.third << std::endl;
	basicStream << "JZ " + atom.third << std::endl << std::endl;
}

void LL::LE(const Atom &atom) {
	loadOp(atom.second);
	basicStream << "MOV B, A" << std::endl;
	loadOp(atom.first);
	basicStream << "CMP B" << std::endl;
	basicStream << "JM " + atom.third << std::endl;
	basicStream << "JZ " + atom.third << std::endl << std::endl;
}

void LL::NOT(const Atom &atom) {
	loadOp(atom.first);
	basicStream << "CMA" << std::endl;
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::MUL(const Atom &atom) {
	MUL_LABEL_START = std::to_string(LabelCnt++);
	MUL_LABEL_END = std::to_string(LabelCnt++);

	// Start
	loadOp(atom.second);
	basicStream << "MOV C, A" << std::endl;

	loadOp(atom.first);
	basicStream << "MOV B, A" << std::endl;

	loadOp("0");

	basicStream << "CMP C" << std::endl;
	basicStream << "JZ L" + MUL_LABEL_END << std::endl;

	basicStream << "JMP L" + MUL_LABEL_START << std::endl;
	basicStream << std::endl;

	// Multiplication
	basicStream << "L" + MUL_LABEL_START + ":" << std::endl;
	basicStream << "ADD B" << std::endl;

	basicStream << "DCR C" << std::endl;
	basicStream << "JZ L" + MUL_LABEL_END << std::endl;

	basicStream << "JMP L" + MUL_LABEL_START << std::endl << std::endl;

	// End
	basicStream << "L" + MUL_LABEL_END + ":" << std::endl;
	saveOp(atom.third);
}

void LL::IN(const Atom &atom) {
	basicStream << "IN 0" << std::endl;
	saveOp(atom.third);
	basicStream << std::endl;
}

void LL::OUT(const Atom &atom) {
	loadOp(atom.third);
	 basicStream << "OUT 1" << std::endl << std::endl;
}

void LL::CALL(const Atom &atom) {
    /* (CALL, p ,, r)
     * а) PUSH B                  ; Выделяем место под возвращаемое значение
     * б) PUSH B
     *    ...                     ; n раз - выделяем место под аргументы
     *    PUSH B
     * в) Для i = 1 до n:
     *      - снимаем операнд Op с программного стека (paramStack)
     *      - loadOp(Op^(+2n + 2)) # loadOp(std::to_string(stoi(Op) + 2 * n + 2)) # только для локальных переменных
     * г) CALL p
     * д) POP B
     *    ...                     ; n раз - очищаем стек
     *    POP B
     * е) POP B                   ; снимаем результат
     * ж) MOV A, C
     *    saveOp(r)               ; сохраняем результат в r
     * */

    basicStream << "PUSH B" << std::endl;

    int n;
    std::string fnToCall;
    for (const auto &i : AtomicMap["-1"]) {
        if (i.cnt == stoi(atom.first.substr(1, atom.first.size() - 2))) {
            n = stoi(i.length);
            fnToCall = i.name;
            break;
        }
    }

    for (int i = 0; i < n; i++) {
        basicStream << "PUSH B" << std::endl;
    }

    basicStream << std::endl;

    // TODO: FIX
    for (int i = 0; i < n; i++) {
        std::string Op = paramStack.top();

        loadOp(Op, 2 * i + 2);

        paramStack.pop();

        basicStream << std::endl;
    }

    basicStream << std::endl << "CALL " << fnToCall << std::endl << std::endl;

    for (int i = 0; i < n; i++) {
        basicStream << "POP B" << std::endl;
    }

    basicStream << "POP B" << std::endl << std::endl;
    basicStream << "MOV A, C" << std::endl << std::endl;
    saveOp(atom.third);
}

void LL::PARAM(const Atom &atom) {
	paramStack.push(atom.third);
}

// TODO: Fix
void LL::RET(const Atom &atom) {

    loadOp(atom.third);

    int m;

    auto iterator = AtomicMap["-1"].begin();
    while (std::to_string(iterator->cnt) != atom.context) {
        iterator++;
    }

    int n = stoi(iterator->length);
    int cnt = 0;

    for (const auto &a: AtomicMap[atom.context]) {
        if (a.kind == "var") cnt++;
    }

    m = cnt - n;


    // TODO: Generate res
    int res = m * 2 + 2;

    for (int i = 0; i < m; i++) {
        basicStream << "POP B" << std::endl;
    }

    basicStream << std::endl << "RET" << std::endl << std::endl;
}
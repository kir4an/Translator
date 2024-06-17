#include "Translator.h"
#include <algorithm>


void LL::_printGraph() {
	basicStream.open(_graph);

	basicStream << "StmtList" << std::endl;

	for (auto &i: outputVector) {
		basicStream << i << std::endl;
	}

	basicStream.close();
}


bool LL::createAtomsResult() {
	if (AtomsGenerated) return true;

	if (!atoms.empty() && atoms.front().text == "ERROR") {
		AtomsGenerated = true;
		SemanticError = true;

		std::cout << "      Semantic Error!     " << std::endl;

		return false;
	}

	for (const auto &i: AtomicMap) {
		for (const auto &j: i.second) {
			sortedAtomsVector.emplace_back(j);
		}
	}

	struct {
		bool operator()(VarOrFunc &a, VarOrFunc &b) const { return a.cnt < b.cnt; };
	} customLess;
	std::sort(sortedAtomsVector.begin(), sortedAtomsVector.end(), customLess);

	for (const auto &o: sortedAtomsVector) {
		if (o.name == "main") entryPoint = std::to_string(o.cnt);
	}

	AtomsGenerated = true;

	return true;
}


void LL::clearStream(const std::string &stream) {
	if (stream.empty()) return;
	basicStream.open(stream);
	basicStream.clear();
	basicStream.close();
}



void LL::createString(const std::string &TextToAdd) {
	std::string _text;

	for (auto i = states.begin(); i != states.end(); ++i) {
		if (i == states.end() - 1) {
			if (*i == 1) {
				_text += "├";
			} else {
				_text += "└";
			}

			break;
		}

		if (*i == 1) {
			_text += "│ ";
		} else {
			_text += " ";
		}
	}

	outVecCnt++;
	outputVector.push_back(_text + TextToAdd);
}


std::string LL::newLabel() {
	return std::to_string(LabelCnt++);
}

std::string LL::alloc(const std::string &scope) {
	std::string temp = addVar("$T" + std::to_string(NewVarCnt++), scope, "kwint", "0");
	return "'" + temp + "'";
}

std::string
LL::addVar(const std::string &name, const std::string &scope, const std::string &type, const std::string &init) {
	if (AtomicMap.count(scope)) {
		for (const auto &i: AtomicMap[scope]) {
			if (i.name == name) {
				return "Error";
			}
		}
	}

	VarOrFunc temp = {name, scope, type, init, "var", "-", AtomicMapCnt++};
	AtomicMap[scope].emplace_back(temp);

	return std::to_string(temp.cnt);
}

std::string LL::addFunc(const std::string &name, const std::string &type, const std::string &length) {
	for (const auto &i: AtomicMap["-1"]) {
		if (i.name == name) {
			return "ERROR";
		}
	}

	VarOrFunc temp = {name, "-1", type, "-", "func", length.empty() ? "0" : length, AtomicMapCnt++};
	AtomicMap["-1"].emplace_back(temp);

	return std::to_string(temp.cnt);
}

std::string LL::checkVar(const std::string &name) {

    std::string result;
    std::stack<std::string> _shadowStack;

    while (!stackFromContext.empty()) {
        for (const auto &i: AtomicMap[stackFromContext.top()]) {
            if (i.name == name && i.kind == "var") {
                result = "'" + std::to_string(i.cnt) + "'";
                goto end;
            } else if (i.name == name && i.type != "var") {
                result = "ERROR";
                goto end;
            }
        }

        _shadowStack.push(stackFromContext.top());
        stackFromContext.pop();
    }

    end:

    while (!_shadowStack.empty()) {
        stackFromContext.push(_shadowStack.top());
        _shadowStack.pop();
    }

    if (result == "ERROR" || result.empty()) {
        return "ERROR";
    } else {
        return result;
    }
}

std::string LL::checkFunc(const std::string &name, const std::string &len) {
	for (const auto &i: AtomicMap["-1"]) {
		if (i.name == name && i.kind == "func" && i.length == len) return "'" + std::to_string(i.cnt) + "'";
		else if (i.name == name && i.type != "func") return "ERROR";
	}

	return "ERROR";
}

void LL::createAtom(const std::string &context = "", const std::string &text = "", const std::string &first = "",
                    const std::string &second = "", const std::string &third = "") {
	if (atoms.size() == 1 && atoms[0].text == "ERROR") return;

	if (context == "ERROR" || text == "ERROR" || first == "ERROR" || second == "ERROR" || third == "ERROR") {
		atoms.clear();
		atoms.emplace_back("SYSTEM", "ERROR", "ERROR", "ERROR", "ERROR");
		return;
	}

	Atom atom = {context, text, first, second, third};
	atoms.emplace_back(atom);
}

// Syntax analyzer and graph creating functions
void LL::getToken() {
	if (lexemVector.empty() || it == lexemVector.end() - 1) {
		lexemVector.emplace_back(lexer.getNextToken());
		it = lexemVector.end() - 1;
		return;
	}

	it++;
}


void LL::getNextState(const int &state) {
	states.emplace_back(state);
	graphIterator = states.end() - 1;
}


void LL::returnPrevNode() {
	graphIterator = (graphIterator > states.begin()) ? graphIterator - 1 : states.begin();
	states.pop_back();
}

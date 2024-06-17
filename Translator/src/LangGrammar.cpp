#include "Translator.h"

// MiniC grammar check
bool LL::StmtList() {
	if (it->first == "eof") return true;

	auto _temp = it;

    getNextState(1);
    createString("Stmt");

	bool result = Stmt();
	if (!result) {
		outputVector.pop_back();
        returnPrevNode();
        returnPrevNode();
		return true;
	}

    getNextState(0);
    createString("StmtList");

	bool tailResult = StmtList();
	if (!tailResult) return false;

    returnPrevNode();
	return true;
}

bool LL::Stmt() {
	if (it->first == "eof") return false;

	if (it->first == "kwint" || it->first == "kwchar") {
        getNextState(0);
        createString("DeclareStmt");

		if (!DeclareStmt()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "semicolon") {
        getToken();

        getNextState(0);
        createString("semicolon");

        returnPrevNode();
        returnPrevNode();
		return true;
	}

	if (stackFromContext.top() == "-1") return false;

	if (it->first == "id") {
        getNextState(0);
        createString("AppointCallOp");

		if (!AppointCallOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "kwwhile") {
        getToken();

        getNextState(0);
        createString("kwwhile WhileOp");

		if (!WhileOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "kwfor") {
        getToken();

        getNextState(0);
        createString("kwfor ForOp");

		if (!ForOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "kwif") {
        getToken();

        getNextState(0);
        createString("kwif IfOp");

		if (!IfOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "kwswitch") {
        getToken();

        getNextState(0);
        createString("kwswitch SwitchOp");

		if (!SwitchOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "kwin") {
        getToken();

        getNextState(0);
        createString("kwin InOp");

		if (!InOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "kwout") {
        getToken();

        getNextState(0);
        createString("kwout OutOp");

		if (!OutOp()) return false;

        returnPrevNode();
		return true;
	}

	if (it->first == "lbrace") {
        getToken();

        getNextState(1);
        createString("lbrace StmtList");

		bool result = StmtList();
		if (!result) return false;

		if (it->first != "rbrace") return false;
        getToken();

        getNextState(0);
        createString("rbrace");

        returnPrevNode();
        returnPrevNode();
		return true;
	}

	if (it->first == "kwreturn") {
        getToken();

        getNextState(1);
        createString("kwreturn E");

		auto EResult = Expr();
		if (!EResult.first) return false;

        createAtom(stackFromContext.top(), "RET", "", "", EResult.second);


		if (it->first != "semicolon") return false;
        getToken();

        getNextState(0);
        createString("semicolon");

        returnPrevNode();
        returnPrevNode();
		return true;
	}

    if (it->first == "opinc") {
        getToken();

        if (it->first != "id") return false;

        getNextState(0);
        createString("opinc " + it->second);

        auto q = checkVar(it->second);
        createAtom(stackFromContext.top(), "ADD", q, "1", q);

        getToken();

        returnPrevNode();
        returnPrevNode();
        return true;
    }

    if (it->first == "opdec") {
        getToken();

        if (it->first != "id") return false;

        getNextState(0);
        createString("opdec " + it->second);

        auto q = checkVar(it->second);
        createAtom(stackFromContext.top(), "SUB", q, "1", q);

        getToken();

        returnPrevNode();
        returnPrevNode();
        return true;
    }

	return false;
}

bool LL::DeclareStmt() {

    getNextState(1);
    createString("Type");

	auto TypeResult = Type();
	if (!TypeResult.first) return false;

	if (it->first != "id") return false;
	auto temp = it->second;
    getToken();

    getNextState(0);
    createString(temp + " DeclareStmt'");

	if (!DeclareStmtList(TypeResult.second, temp)) return false;

    returnPrevNode();
	return true;
}

bool LL::DeclareStmtList(const std::string &type, const std::string &name) {
	if (it->first == "eof") return false;

	if (it->first == "lpar") {
		if (stackFromContext.top() != "-1") return false;
        getToken();

        getNextState(1);
        createString("lpar ParamList");

		std::string TC = addFunc(name, type);
		stackFromContext.push(TC);

		FT ParamListRes = ParamList();
		if (!ParamListRes.first) return false;

		for (auto &i : AtomicMap["-1"]) {
			if (std::to_string(i.cnt) == stackFromContext.top()) {
				i.length = ParamListRes.second;
			}
		}

		if (it->first != "rpar") return false;
        getToken();

		if (it->first != "lbrace") return false;
        getToken();

        getNextState(1);
        createString("rpar lbrace StmtList");

        if (!StmtList()) return false;

		if (it->first != "rbrace") return false;
        getToken();

        createAtom(stackFromContext.top(), "RET", "", "", "0");
        stackFromContext.pop();

        getNextState(0);
        createString("rbrace");

        returnPrevNode();
        returnPrevNode();
		return true;
	} else if (it->first == "opassign") {
        getToken();
		auto _temp = it->second;

		if (it->first == "num") {
			std::string newVar = "'" + addVar(name, stackFromContext.top(), type, _temp) + "'";

			if (newVar == "'Error'") return false;

            getToken();

            getNextState(1);
            createString("opassign " + _temp + " DeclareVarList");

			auto DecVarListRes = DeclareVarList(type);
			if (!DecVarListRes) return false;

			if (it->first != "semicolon") return false;
            getToken();

            getNextState(0);
            createString("semicolon");

            returnPrevNode();
            returnPrevNode();
			return true;
		} else if (it->first == "char") {
			std::string newVar = "'" + addVar(name, stackFromContext.top(), type, _temp) + "'";

			if (newVar == "'Error'") return false;

            getToken();

            getNextState(1);
            createString("opassign " + _temp + "DeclareVarList");

			auto DecVarListRes = DeclareVarList(type);
			if (!DecVarListRes) return false;

			if (it->first != "semicolon") return false;
            getToken();

            getNextState(0);
            createString("semicolon");

            returnPrevNode();
            returnPrevNode();
			return true;
		} else {
			return false;
		}
	} else {
		std::string _temp = addVar(name, stackFromContext.top(), type);

		if (_temp == "'Error'") return false;

        getNextState(1);
        createString("DeclareVarList");

		if (!DeclareVarList(type)) return false;

		if (it->first != "semicolon") return false;
        getToken();

        getNextState(0);
        createString("semicolon");

        returnPrevNode();
        returnPrevNode();
		return true;
	}
}

bool LL::AppointCallOp() {

    getNextState(1);
    createString("AppointOrCall");

	if (!AppointOrCall()) return false;

	if (it->first != "semicolon") return false;
    getToken();

    getNextState(0);
    createString("semicolon");

    returnPrevNode();
    returnPrevNode();
	return true;
}

bool LL::AppointOrCall() {
	if (it->first != "id") return false;
	auto temp = it->second;
    getToken();

    getNextState(0);
    createString(temp + " AppointOrCall'");

	if (!AppointList(temp)) return false;

    returnPrevNode();
	return true;
}

bool LL::AppointList(const std::string &name) {
	if (it->first == "opassign") {
        getToken();

        getNextState(0);
        createString("opassign E");

		auto ERes = Expr();
		if (!ERes.first) return false;

		auto r = checkVar(name);
        createAtom(stackFromContext.top(), "MOV", ERes.second, "", r);

        returnPrevNode();
		return true;
	}

    if (it->first == "opinc") {
        getToken();

        getNextState(0);
        createString("opinc");

        auto s = checkVar(name);
        auto r = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "MOV", s, "", r);
        createAtom(stackFromContext.top(), "ADD", s, "1", s);

        returnPrevNode();
        returnPrevNode();
        return true;
    }

    if (it->first == "opdec") {
        getToken();

        getNextState(0);
        createString("opdec");

        auto s = checkVar(name);
        auto r = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "MOV", s, "", r);
        createAtom(stackFromContext.top(), "SUB", s, "1", s);

        returnPrevNode();
        returnPrevNode();
        return true;
    }

	if (it->first == "opmulassign") {
        getToken();

        getNextState(0);
        createString("opmulassign E");

		auto ERes = Expr();
		if (!ERes.first) return false;

		auto r = checkVar(name);
		auto s = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "MUL", r, ERes.second, s);
        createAtom(stackFromContext.top(), "MOV", s, "", r);

        returnPrevNode();
		return true;
	}

	if (it->first == "opplusassign") {
        getToken();

        getNextState(0);
        createString("opplusassign E");

		auto ERes = Expr();
		if (!ERes.first) return false;

		auto r = checkVar(name);
		auto s = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "ADD", r, ERes.second, s);
        createAtom(stackFromContext.top(), "MOV", s, "", r);

        returnPrevNode();
		return true;
	}

	if ((*it).first == "opminusassign") {
        getToken();

        getNextState(0);
        createString("opminusassign E");

		auto ERes = Expr();
		if (!ERes.first) return false;

		auto resultCheck = checkVar(name);
		auto allocResult = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "SUB", resultCheck, ERes.second, allocResult);
        createAtom(stackFromContext.top(), "MOV", allocResult, "", resultCheck);

        returnPrevNode();
		return true;
	}

	if (it->first == "lpar") {
        getToken();

        getNextState(1);
        createString("lpar ArgList");

		auto ArgListRes = ArgList();
		if (!ArgListRes.first) return false;

		if (it->first != "rpar") return false;
        getToken();

        getNextState(0);
        createString("rpar");

		auto q = checkFunc(name, ArgListRes.second);
		auto r = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "CALL", q, "", r);

        returnPrevNode();
        returnPrevNode();
		return true;
	}

	return false;
}

bool LL::WhileOp() {

	auto l1 = newLabel();
	auto l2 = newLabel();

    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l1);

	if (it->first != "lpar") return false;
    getToken();

    getNextState(1);
    createString("lpar E");

	auto ERes = Expr();
	if (!ERes.first) return false;

    createAtom(stackFromContext.top(), "EQ", ERes.second, "0", "L" + l2);

	if (it->first != "rpar") return false;
    getToken();

    getNextState(0);
    createString("rpar Stmt");

	if (!Stmt()) return false;

    createAtom(stackFromContext.top(), "JMP", "", "", "L" + l1);
    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l2);

    returnPrevNode();
	return true;
}

bool LL::ForOp() {

	auto l1 = newLabel();
	auto l2 = newLabel();
	auto l3 = newLabel();
	auto l4 = newLabel();

	if (it->first != "lpar") return false;
    getToken();

    getNextState(1);
    createString("lpar ForInit");

	if (!ForInit()) return false;

    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l1);

    getNextState(1);
    createString("semicolon ForExp");

	FT ForExpRes = ForExp();
	if (!ForExpRes.first) return false;

    createAtom(stackFromContext.top(), "EQ", ForExpRes.second, "0", "L" + l4);
    createAtom(stackFromContext.top(), "JMP", "", "", "L" + l3);
    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l2);

    getNextState(1);
    createString("semicolon ForLoop");

	if (!ForLoop()) return false;

    createAtom(stackFromContext.top(), "JMP", "", "", "L" + l1);
    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l3);

    getNextState(0);
    createString("rpar Stmt");

	if (!Stmt()) return false;

    createAtom(stackFromContext.top(), "JMP", "", "", "L" + l2);
    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l4);

    returnPrevNode();
	return true;
}

bool LL::ForInit() {
	if (it->first == "kwint") {

        getNextState(1);
        createString("kwint");
        returnPrevNode();

        getToken();

		if (it->first != "id") return false;

		std::string _temp = addVar(it->second, stackFromContext.top(), "kwint");
		if (_temp == "Error") return false;

		AtomicMap[stackFromContext.top()].pop_back();
		AtomicMapCnt--;

		addVar(it->second, stackFromContext.top(), "kwint");

        getNextState(0);
        createString("AppointOrCall");

		if (!AppointOrCall()) return false;
	}

	else if (it->first == "id") {
        getNextState(0);
        createString("AppointOrCall");

		if (!AppointOrCall()) return false;
	}

	if (it->first != "semicolon") return false;
    getToken();

    returnPrevNode();
	return true;
}

FT LL::ForExp() {
	bool oneFlag = false;
	std::string _temp;

	if (it->first == "lpar" || it->first == "opdec" || it->first == "opinc" || it->first == "num" || it->first == "char" || it->first == "id") {
        getNextState(0);
        createString("E");

		FT ERes = Expr();

		if (!ERes.first) oneFlag = true;
		else oneFlag = false;

		_temp = ERes.second;
	}

	if (it->first == "semicolon") {
        getToken();

        returnPrevNode();
		if (oneFlag) return {true, "1"};
		return {true, _temp};
	}

	return {false, ""};
}

bool LL::ForLoop() {
	if (it->first == "opinc") {
        getToken();

		if (it->first != "id") return false;
		std::string _temp = it->second;
        getToken();

		auto p = checkVar(_temp);
        createAtom(stackFromContext.top(), "ADD", p, "1", p);

		if (it->first != "rpar") return false;
        getToken();

        getNextState(0);
        createString("opinc " + _temp);

        returnPrevNode();
        returnPrevNode();
		return true;
	}

	if (it->first == "opdec") {
        getToken();

		if (it->first != "id") return false;
		std::string _temp = it->second;
        getToken();

		auto p = checkVar(_temp);
        createAtom(stackFromContext.top(), "SUB", p, "1", p);

		if (it->first != "rpar") return false;
        getToken();

        getNextState(0);
        createString("opdec " + _temp);

        returnPrevNode();
        returnPrevNode();
		return true;
	}

	if (it->first == "id") {
        getNextState(0);
        createString("AppointOrCall");

		if (!AppointOrCall()) return false;

		if (it->first != "rpar") return false;
        getToken();

        returnPrevNode();
		return true;
	}

	if (it->first != "rpar") return false;
    getToken();

    returnPrevNode();
	return true;
}

bool LL::IfOp() {
	if (it->first != "lpar") return false;
    getToken();

    getNextState(1);
    createString("lpar E");

	auto ERes = Expr();
	if (!ERes.first) return false;

	if (it->first != "rpar") return false;
    getToken();

	auto l1 = newLabel();
    createAtom(stackFromContext.top(), "EQ", ERes.second, "0", "L" + l1);

    getNextState(1);
    createString("rpar Stmt");

	if (!Stmt()) return false;

	auto l2 = newLabel();
    createAtom(stackFromContext.top(), "JMP", "", "", "L" + l2);
    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l1);

    getNextState(0);
    createString("ElsePart");

	if (it->first == "kwelse") {
		if (!ElsePart()) return false;

        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l2);

        returnPrevNode();
		return true;
	}

    createAtom(stackFromContext.top(), "LBL", "", "", "L" + l2);

    returnPrevNode();
    returnPrevNode();
	return true;
}

bool LL::ElsePart() {

    getToken();

    getNextState(0);
    createString("kwelse Stmt");

	if (!Stmt()) return false;

    returnPrevNode();
	return true;
}

bool LL::SwitchOp() {
	if (it->first != "lpar") return false;
    getToken();

    getNextState(1);
    createString("lpar E");

	auto ERes = Expr();
	if (!ERes.first) return false;

	if (it->first != "rpar") return false;
    getToken();

	if (it->first != "lbrace") return false;
    getToken();

    getNextState(1);
    createString("rpar lbrace Cases");

	auto end = newLabel();
	if (!Cases(ERes.second, end)) return false;

	if (it->first != "rbrace") return false;
    getToken();

    createAtom(stackFromContext.top(), "LBL", "", "", "L" + end);

    getNextState(0);
    createString("rbrace");

    returnPrevNode();
    returnPrevNode();
	return true;
}

bool LL::Cases(const std::string &p, const std::string &end) {
    getNextState(1);
    createString("ACase");

	auto ACaseRes = ACase(p, end);
	if (!ACaseRes.first) return false;

    getNextState(0);
    createString("Cases'");

	if (!CasesList(p, end, ACaseRes.second)) return false;

    returnPrevNode();
	return true;
}

bool LL::CasesList(const std::string &p, const std::string &end, const std::string &def) {

	if (it->first == "kwcase" || it->first == "kwdefault") {
        getNextState(1);
        createString("ACase");

		FT ACaseRes = ACase(p, end);
		if (!ACaseRes.first) return false;

		if ((stoi(def) >= 0) && (stoi(ACaseRes.second) >= 0)) {
            createAtom("SYSTEM", "ERROR", "ERROR", "ERROR", "ERROR");
		}

		auto maxDef = stoi(def) < stoi(ACaseRes.second) ? ACaseRes.second : def;

        getNextState(0);
        createString("Cases'");

		if (!CasesList(p, end, maxDef)) return false;

        returnPrevNode();
		return true;
	}

	std::string q = stoi(def) >= 0 ? def : end;
    createAtom(stackFromContext.top(), "JMP", "", "", "L" + q);

    returnPrevNode();
	return true;
}

FT LL::ACase(const std::string &p, const std::string &end) {
	if (it->first == "kwcase") {
        getToken();

		if (it->first != "num") return {false, ""};
		std::string _temp = it->second;
        getToken();

		std::string next = newLabel();
        createAtom(stackFromContext.top(), "NE", p, _temp, "L" + next);

		if (it->first != "colon") return {false, ""};
        getToken();

        getNextState(0);
        createString("kwcase " + _temp + "colon StmtList");

		if (!StmtList()) return {false, ""};

        createAtom(stackFromContext.top(), "JMP", "", "", "L" + end);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + next);

        returnPrevNode();
		return {true, "-1"};
	}

	if (it->first == "kwdefault") {
        getToken();

		if (it->first != "colon") return {false, ""};
        getToken();

		auto next = newLabel();
		auto def = newLabel();

        createAtom(stackFromContext.top(), "JMP", "", "", "L" + next);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + def);

        getNextState(0);
        createString("kwdefault colon StmtList");

		if (!StmtList()) return {false, ""};

        createAtom(stackFromContext.top(), "JMP", "", "", "L" + end);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + next);

        returnPrevNode();
		return {true, def};
	}

	return {false, ""};
}

bool LL::InOp() {
	if (it->first != "id") return false;
	std::string _temp = it->second;
    getToken();

	if (it->first != "semicolon") return false;
    getToken();

	auto p = checkVar(_temp);
    createAtom(stackFromContext.top(), "IN", "", "", p);

    getNextState(0);
    createString(_temp + " semicolon");

    returnPrevNode();
    returnPrevNode();
	return true;
}

bool LL::OutOp() {

    getNextState(1);
    createString("OutOp'");

	if (!OutOpList()) return false;

	if (it->first != "semicolon") return false;
    getToken();

    getNextState(0);
    createString("semicolon");

    returnPrevNode();
    returnPrevNode();
	return true;
}

bool LL::OutOpList() {
	if (it->first == "str") {
		std::string _temp = it->second;
        getToken();

        getNextState(0);
        createString("\"" + _temp + "\"");

        createAtom(stackFromContext.top(), "OUT", "", "", "\"" + _temp + "\"");
        returnPrevNode();
	} else {

        getNextState(0);
        createString("E");

		auto ERes = Expr();
		if (!ERes.first) return false;

        createAtom(stackFromContext.top(), "OUT", "", "", ERes.second);
	}

    returnPrevNode();
	return true;
}

FT LL::Type() {
	if (it->first == "eof") return {false, ""};

	if (it->first == "kwint" || it->first == "kwchar") {

		auto temp = it->first;
        getToken();

        getNextState(0);
        createString(temp);

        returnPrevNode();
        returnPrevNode();
		return {true, temp};
	}

	return {false, ""};
}

bool LL::DeclareVarList(const std::string &type) {
	if (it->first == "eof") return false;

	if (it->first == "comma") {
        getToken();

		if (it->first != "id") return false;
		auto temp = it->second;

        getNextState(1);
        createString("comma " + it->second + " InitVar");

        getToken();

		if (!InitVar(type, temp)) return false;

        getNextState(0);
        createString("DeclareVarList");

		if (!DeclareVarList(type)) return false;
	}

    returnPrevNode();
	return true;
}

bool LL::InitVar(const std::string &r, const std::string &s) {
	if (it->first == "eof") return false;

	if (it->first == "opassign") {
        getToken();

		if (it->first == "num" || it->first == "char") {

			std::string newVar = "'" + addVar(s, stackFromContext.top(), r, it->second) + "'";

			if (newVar == "'Error'") return false;

            getNextState(0);
            createString("opassign " + it->second);

            createAtom(stackFromContext.top(), "MOV", it->second, "", newVar);

            getToken();

            returnPrevNode();
            returnPrevNode();
			return true;
		}

		return false;
	}

	std::string _temp = addVar(s, stackFromContext.top(), r);

	if (_temp == "'Error'") return false;

    returnPrevNode();
	return true;
}

FT LL::ParamList() {
	if (it->first == "eof") return {false, ""};

	if (it->first == "kwint" || it->first == "kwchar") {
        getNextState(1);
        createString("Type");

		auto TypeResult = Type();

		if (it->first != "id") return {false, ""};
		std::string _temp = addVar(it->second, stackFromContext.top(), TypeResult.second);

		if (_temp == "'Error'") return {false, ""};

        getNextState(0);
        createString(it->second + " ParamList'");

        getToken();

		FT ParLLRes = ParamListList();
		if (!ParLLRes.first) return {false, ""};

        returnPrevNode();
		return {true, std::to_string(stoi(ParLLRes.second) + 1)};
	}

    returnPrevNode();
	return {true, "0"};
}

FT LL::ParamListList() {
	if (it->first == "eof") return {false, ""};

	if (it->first == "comma") {
        getToken();

		if (it->first == "kwint" || it->first == "kwchar") {
            getNextState(1);
            createString("comma Type");

			auto TypeResult = Type();

			if (it->first != "id") return {false, ""};
			std::string _temp = addVar(it->second, stackFromContext.top(), TypeResult.second);

			if (_temp == "'Error'") return {false, ""};

            getNextState(0);
            createString(it->second + " ParamList'");

            getToken();

			FT ParLLRes = ParamListList();
			if (!ParLLRes.first) return {false, ""};

            returnPrevNode();
			return {true, std::to_string(stoi(ParLLRes.second) + 1)};
		}
	}

    returnPrevNode();
	return {true, "0"};
}

#include "Translator.h"

// Expressions check
FT LL::Expr() {

    getNextState(0);
    createString("E7");

	auto E7Result = Expr7();
	if (!E7Result.first) return {false, ""};

    returnPrevNode();
	return {true, E7Result.second};
}

FT LL::Expr7() {

    getNextState(1);
    createString("E6");

	auto E6Result = Expr6();
	if (!E6Result.first) return {false, ""};

    getNextState(0);
    createString("E7'");

	auto E7ListResult = Expr7List(E6Result.second);
	if (!E7ListResult.first) return {false, ""};

    returnPrevNode();
	return {true, E7ListResult.second};
}

FT LL::Expr7List(const std::string &funcID) {
	if (it->first == "opor") {
        getToken();

        getNextState(1);
        createString("opor E6");

		auto E6Result = Expr6();
		if (!E6Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "OR", funcID, E6Result.second, s);

        getNextState(0);
        createString("E7'");

		auto E7ListResult = Expr7List(s);
		if (!E7ListResult.first) return {false, ""};

        returnPrevNode();
		return {true, E7ListResult.second};
	}

    returnPrevNode();
	return {true, funcID};
}

FT LL::Expr6() {
    getNextState(1);
    createString("E5");

	auto E5Result = Expr5();
	if (!E5Result.first) return {false, ""};

    getNextState(0);
    createString("E6'");

	auto E6ListResult = Expr6List(E5Result.second);
	if (!E6ListResult.first) return {false, ""};

    returnPrevNode();
	return {true, E6ListResult.second};
}

FT LL::Expr6List(const std::string &funcID) {
	if (it->first == "opand") {
        getToken();

        getNextState(1);
        createString("opand E5");

		auto E5Result = Expr5();
		if (!E5Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "AND", funcID, E5Result.second, s);

        getNextState(0);
        createString("E6'");

		auto E6ListResult = Expr6List(s);
		if (!E6ListResult.first) return {false, ""};

        returnPrevNode();
		return {true, E6ListResult.second};
	}

    returnPrevNode();
	return {true, funcID};
}

FT LL::Expr5() {

    getNextState(1);
    createString("E4");

	auto E4Result = Expr4();
	if (!E4Result.first) return {false, ""};

    getNextState(0);
    createString("E5'");

	auto E5ListResult = Expr5List(E4Result.second);
	if (!E5ListResult.first) return {false, ""};

    returnPrevNode();
	return {true, E5ListResult.second};
}

FT LL::Expr5List(const std::string &funcID) {
	if (it->first == "opeq") {
        getToken();

        getNextState(0);
        createString("opeq E4");

		auto E4Result = Expr4();
		if (!E4Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
		auto l = newLabel();

        createAtom(stackFromContext.top(), "MOV", "1", "", s);
        createAtom(stackFromContext.top(), "EQ", funcID, E4Result.second, "L" + l);
        createAtom(stackFromContext.top(), "MOV", "0", "", s);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l);

        returnPrevNode();
		return {true, s};

	} else if (it->first == "opne") {
        getToken();

        getNextState(0);
        createString("opne E4");

		auto E4Result = Expr4();
		if (!E4Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
		auto l = newLabel();

        createAtom(stackFromContext.top(), "MOV", "1", "", s);
        createAtom(stackFromContext.top(), "NE", funcID, E4Result.second, "L" + l);
        createAtom(stackFromContext.top(), "MOV", "0", "", s);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l);

        returnPrevNode();
		return {true, s};
	} else if (it->first == "ople") {
        getToken();

        getNextState(0);
        createString("ople E4");

		auto E4Result = Expr4();
		if (!E4Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
		auto l = newLabel();

        createAtom(stackFromContext.top(), "MOV", "1", "", s);
        createAtom(stackFromContext.top(), "LE", funcID, E4Result.second, "L" + l);
        createAtom(stackFromContext.top(), "MOV", "0", "", s);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l);

        returnPrevNode();
		return {true, s};
	} else if (it->first == "opgt") {
        getToken();

        getNextState(0);
        createString("opgt E4");

		auto E4Result = Expr4();
		if (!E4Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
		auto l = newLabel();

        createAtom(stackFromContext.top(), "MOV", "1", "", s);
        createAtom(stackFromContext.top(), "GT", funcID, E4Result.second, "L" + l);
        createAtom(stackFromContext.top(), "MOV", "0", "", s);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l);

        returnPrevNode();
		return {true, s};
	} else if (it->first == "opge") {
        getToken();

        getNextState(0);
        createString("opge E4");

		auto E4Result = Expr4();
		if (!E4Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
		auto l = newLabel();

        createAtom(stackFromContext.top(), "MOV", "1", "", s);
        createAtom(stackFromContext.top(), "GE", funcID, E4Result.second, "L" + l);
        createAtom(stackFromContext.top(), "MOV", "0", "", s);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l);

        returnPrevNode();
		return {true, s};
	} else if (it->first == "oplt") {
        getToken();

        getNextState(0);
        createString("oplt E4");

		auto E4Result = Expr4();
		if (!E4Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
		auto l = newLabel();

        createAtom(stackFromContext.top(), "MOV", "1", "", s);
        createAtom(stackFromContext.top(), "LT", funcID, E4Result.second, "L" + l);
        createAtom(stackFromContext.top(), "MOV", "0", "", s);
        createAtom(stackFromContext.top(), "LBL", "", "", "L" + l);

        returnPrevNode();
		return {true, s};
	}

    returnPrevNode();
	return {true, funcID};
}

FT LL::Expr4() {

    getNextState(1);
    createString("E3");

	auto E3Result = Expr3();
	if (!E3Result.first) return {false, ""};

    getNextState(0);
    createString("E4'");

	auto E4ListResult = Expr4List(E3Result.second);
	if (!E4ListResult.first) return {false, ""};

    returnPrevNode();
	return {true, E4ListResult.second};
}

FT LL::Expr4List(const std::string &funcID) {
	if (it->first == "opplus") {
        getToken();

        getNextState(1);
        createString("opplus E3");

		auto E3Result = Expr3();
		if (!E3Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "ADD", funcID, E3Result.second, s);

        getNextState(0);
        createString("E4'");

		auto E4ListResult = Expr4List(s);
		if (!E4ListResult.first) return {false, ""};

        returnPrevNode();
		return {true, E4ListResult.second};

	} else if (it->first == "opminus") {
        getToken();

        getNextState(1);
        createString("opminus E3");

		auto E3Result = Expr3();
		if (!E3Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "SUB", funcID, E3Result.second, s);

        getNextState(0);
        createString("E4'");

		auto E4ListResult = Expr4List(s);
		if (!E4ListResult.first) return {false, ""};

        returnPrevNode();
		return {true, E4ListResult.second};
	}

    returnPrevNode();
	return {true, funcID};
}

FT LL::Expr3() {

    getNextState(1);
    createString("E2");

	auto E2Result = Expr2();
	if (!E2Result.first) return {false, ""};

    getNextState(0);
    createString("E3'");

	auto E3ListResult = Expr3List(E2Result.second);
	if (!E3ListResult.first) return {false, ""};

    returnPrevNode();
	return {true, E3ListResult.second};

}

FT LL::Expr3List(const std::string &funcID) {
	if (it->first == "opmul") {
        getToken();

        getNextState(1);
        createString("opmul E2");

		auto E2Result = Expr2();
		if (!E2Result.first) return {false, ""};

		auto s = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "MUL", funcID, E2Result.second, s);

        getNextState(0);
        createString("E3'");

		auto E3ListResult = Expr3List(s);
		if (!E3ListResult.first) return {false, ""};

        returnPrevNode();
		return {true, E3ListResult.second};
	}

    returnPrevNode();
	return {true, funcID};
}

FT LL::Expr2() {
	if (it->first == "opnot") {
        getToken();

        getNextState(0);
        createString("opnot E1");

		auto E1Result = Expr1();
		if (!E1Result.first) return {false, ""};

		auto r = alloc(stackFromContext.top());
        createAtom(stackFromContext.top(), "NOT", E1Result.second, "", r);

        returnPrevNode();
		return {true, r};
	} else {

        getNextState(0);
        createString("E1");

		auto E1Result = Expr1();
		if (!E1Result.first) return {false, ""};

        returnPrevNode();
		return {true, E1Result.second};
	}
}

FT LL::Expr1() {
	if (it->first == "opdec") {
        getToken();

		if (it->first != "id") return {false, ""};

		auto q = checkVar(it->second);
        createAtom(stackFromContext.top(), "SUB", q, "1", q);

        getNextState(0);
        createString("opdec " + it->second);
        getToken();

        returnPrevNode();
        returnPrevNode();
		return {true, q};
	} else if (it->first == "opinc") {
        getToken();

		if (it->first != "id") return {false, ""};

		auto q = checkVar(it->second);
        createAtom(stackFromContext.top(), "ADD", q, "1", q);

        getNextState(0);
        createString("opinc " + it->second);
        getToken();

        returnPrevNode();
        returnPrevNode();
		return {true, q};
	} else if (it->first == "lpar") {
        getToken();

        getNextState(1);
        createString("lpar E");

		auto EResult = Expr();
		if (!EResult.first) return {false, ""};

        getNextState(0);
        createString("rpar");

		if (it->first != "rpar") return {false, ""};
        getToken();

        returnPrevNode();
        returnPrevNode();
		return {true, EResult.second};
	} else if (it->first == "num") {
		auto val = it->second;

        getNextState(0);
        createString(val);
        getToken();

        returnPrevNode();
        returnPrevNode();
		return {true, val};
	} else if (it->first == "char") {
		auto val = it->second;

        getNextState(0);
        createString(val);
        getToken();

        returnPrevNode();
        returnPrevNode();
		return {true, val};
	} else if (it->first == "id") {
		auto r = it->second;

        getNextState(0);
        createString(r + " E1'");

        getToken();

		auto E1ListResult = Expr1List(r);
		if (!E1ListResult.first) return {false, ""};

        returnPrevNode();
		return {true, E1ListResult.second};
	}

	return {false, ""};
}

FT LL::Expr1List(const std::string &funcID) {
	if (it->first == "lpar") {
        getToken();

        getNextState(1);
        createString("lpar ArgList");

		auto ArgListResult = ArgList();
		if (!ArgListResult.first) return {false, ""};

        getNextState(0);
        createString("rpar");

		if (it->first != "rpar") return {false, ""};
        getToken();

		auto s = checkFunc(funcID, ArgListResult.second);
		auto r = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "CALL", s, "", r);

        returnPrevNode();
        returnPrevNode();
		return {true, r};
	}

	if (it->first == "opinc") {
        getToken();

		auto s = checkVar(funcID);
		auto r = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "MOV", s, "", r);
        createAtom(stackFromContext.top(), "ADD", s, "1", s);

        getNextState(0);
        createString("opinc");
        returnPrevNode();
        returnPrevNode();
		return {true, r};
	}

	if (it->first == "opdec") {
        getToken();

		auto s = checkVar(funcID);
		auto r = alloc(stackFromContext.top());

        createAtom(stackFromContext.top(), "MOV", s, "", r);
        createAtom(stackFromContext.top(), "SUB", s, "1", s);

        getNextState(0);
        createString("opdec");
        returnPrevNode();
        returnPrevNode();
		return {true, r};
	}

	auto q = checkVar(funcID);

    returnPrevNode();
	return {true, q};
}

FT LL::ArgList() {
	if (it->first == "lpar" || it->first == "opdec" || it->first == "opinc" || it->first == "num" || it->first == "char" || it->first == "id") {

        getNextState(1);
        createString(it->first + " E");

		FT ERes = Expr();
		if (!ERes.first) return {false, ""};

        createAtom(stackFromContext.top(), "PARAM", "", "", ERes.second);

        getNextState(0);
        createString("ArgList'");

		FT ArgListListResult = ArgListList();
		if (!ArgListListResult.first) return {false, ""};

        returnPrevNode();
		return {true, std::to_string(stoi(ArgListListResult.second) + 1)};
	}

    returnPrevNode();
	return {true, "0"};
}

FT LL::ArgListList() {
	if (it->first == "comma") {
        getToken();

		if (it->first == "lpar" || it->first == "opdec" || it->first == "opinc" || it->first == "num" || it->first == "char" ||
		    it->first == "id") {

            getNextState(1);
            createString("comma E");

			auto ERes = Expr();
			if (!ERes.first) return {false, ""};

            createAtom(stackFromContext.top(), "PARAM", "", "", ERes.second);

            getNextState(0);
            createString("ArgList'");

			FT ArgListListResult = ArgListList();
			if (!ArgListListResult.first) return {false, ""};

            returnPrevNode();
			return {true, std::to_string(stoi(ArgListListResult.second) + 1)};
		} else {
			return {false, ""};
		}
	}

    returnPrevNode();
	return {true, "0"};
}

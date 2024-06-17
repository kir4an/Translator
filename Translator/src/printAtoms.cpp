#include "Translator.h"

// Private function to print Atoms
bool LL::_printAtoms() {

	if (!createAtomsResult()) return false;

	basicStream.open(_atomsInput);
	basicStream << "Context: Atom\n" << std::endl;

	for (const auto &i: atoms) {
		basicStream << i.context << ": ("
		         << i.text << ","
		         << i.first << ","
		         << i.second << ","
		         << i.third << ")" << std::endl;
	}

	basicStream << "\n=============================================\nName : Code : Class : Type : Init : Length : Scope \n\n";

	for (const auto &i: sortedAtomsVector) {
		basicStream << i.name << " : '" << i.cnt << "' : " << i.kind << " : " << i.type << " : " << i.init << " : "
                 << i.length << " : " << i.scope << std::endl;
	}

	basicStream.close();

	return true;
}
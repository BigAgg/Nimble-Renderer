#include "NimbleHelpers.h"

#include <iostream>

namespace strings {
	bool ends_with(const std::string& value, const std::string& ending) {
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}
}

namespace logging {
	void log(const std::string &type, const std::string &msg) {
		std::cout << type << ":\t" << msg << "\n";
	}
}

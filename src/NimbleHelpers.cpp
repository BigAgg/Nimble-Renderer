#include "NimbleHelpers.h"

namespace strings {
	bool ends_with(const std::string& value, const std::string& ending) {
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}
}

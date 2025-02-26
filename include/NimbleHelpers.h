#ifndef NIMBLEHELPERS_H
#define NIMBLEHELPERS_H

#include <string>

namespace strings {
	bool ends_with(const std::string& value, const std::string& ending);
}

namespace logging {
	void log(const std::string &type, const std::string &msg);
}

#endif
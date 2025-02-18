#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <string>
#include <fstream>
#include <chrono>
#include <format>
#include <filesystem>

namespace logger {
	inline std::string getTimestamp() {
		const auto now = std::chrono::system_clock::now();
		return std::format("{:%d-%m-%Y %H:%M:%OS}", now);
	}

	inline void log(std::string from, std::string msg) {
		std::string outstr = "\t[" + from + "] " + msg + "\n";
		outstr = getTimestamp() + outstr;
		std::cout << outstr;
	}

	inline void deletelog() { std::remove("run.log"); }

	inline void backuplog(bool crash = false) {
		if (crash) {
			if (std::filesystem::exists("run.log")) {
				std::filesystem::create_directories("./crashes/");
				int count = 0;
				for (const auto& entry : std::filesystem::directory_iterator("logs/")) {
					count++;
				}
				std::string outStr = "crash_" + std::to_string(count) + ".log";
				std::filesystem::copy_file("run.log", "./crashes/" + outStr);
			}
			return;
		}
		if (std::filesystem::exists("run.log")) {
			std::filesystem::create_directories("logs/");
			int count = 0;
			for (const auto& entry : std::filesystem::directory_iterator("logs/")) {
				count++;
			}
			std::string outStr = "run_" + std::to_string(count) + ".log";
			std::filesystem::copy_file("run.log", "logs/" + outStr);
		}
	}

	inline void startlogging(std::fstream& file, bool backupOld = true) {
		if (!backupOld)
			deletelog();
		else {
			backuplog();
		}

		file.open("run.log", std::ios::out);
		if (!file.is_open() || !file.good())
			return;

		std::streambuf* stream_buffer_file = file.rdbuf();
		std::cout.rdbuf(stream_buffer_file);
	}

	inline void startlogging_c(FILE& f, bool backupOld = true) {
		if (backupOld)
			backuplog();
		f = *freopen("run.log", "w", stdout);
	}
} // namespace logger

#endif

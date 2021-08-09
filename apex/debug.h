#pragma once
#include "sdk.h"
#include <chrono>
#include <ctime>
//GOAL: Create a lasting debug message sdk
namespace debug
{
	void printVar(int num) {
		printf("(%d)", num);
	}

	void printVar(float flt) {
		printf("(%.2f)", flt);
	}

	void printVar(double dbl) {
		printf("(%f)", dbl);
	}

	void printVar(long lng) {
		printf("(%l)", lng);
	}

	void printVar(long long lng) {
		printf("(%l)", lng);
	}

	void printVar(bool b) {
		printf("(%s)", b ? "true" : "false");
	}

	void printVar(const char* cstr) {
		printf( "(%s)", cstr);
	}

	void printVar(std::string str) {
		printf("%s", str.c_str());
	}

	void printVar(Vector3 vec) {
		printf("(%.2f, %.2f, %.2f)", vec.x, vec.y, vec.z);
	}

	void printVar(Vector4 vec) {
		printf("(%.2f, %.2f, %.2f, %.2f)", vec.w, vec.x, vec.y, vec.z);
	}

	void printVar(VMatrix mat) {
		

	}

	void printVar(Color col) {
		printf("(%d, %d, %d, %d)", col.r, col.g, col.b, col.a);
	}

	void print(std::string message, ...) {

		std::string output = "";
		//find time as a string
		auto time = [&] {
			std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
			std::time_t start_time = std::chrono::system_clock::to_time_t(now);
			char timedisplay[100];
			struct tm buf;
			errno_t err = localtime_s(&buf, &start_time);
			if (std::strftime(timedisplay, sizeof(timedisplay), "[%H:%M:%S]", &buf))
				return std::string(timedisplay);
			else
				return std::string("[error]");
		};

		//Arg nonsense

		//recursively print args

		printf("\n"); //newline
	}
}
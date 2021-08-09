#pragma once
#include "stdafx.h"
#include "memory.h"
#define read(address, type) pMem->Rpm<type>(address);
#define write(address, val, type) pMem->Wpm<type>(address, val);
template<typename T>
class Offset {
public:
	Offset() {}
	Offset(uint64_t address, uint64_t offset) {
		this->base = address;
		this->offset = offset;
	};
	T fromCache() {
		if (!saved) {
			return get();
		}
		return this->cached;
	}
	T get() {
		this->cached = read(base + offset, T);
		this->saved = true;
		return this->cached;
	}
	void set(T newValue) {
		//printf("Address: %llu, size: %llu\n", base + offset, sizeof(T));
		write(base + offset, newValue, T)
	}
	bool initialized() {
		return (base != NULL);
	}
private:
	T cached;
	bool saved = false;
	uint64_t base = NULL;
	uint64_t offset = NULL;
};
#undef read
#undef write
#define OFFSET(name, type, offset) private: Offset<type> _##name; public: Offset<type>* name() { if (!this->_##name.initialized()) { this->_##name = Offset<type>(this->base, offset);} return &_##name; }
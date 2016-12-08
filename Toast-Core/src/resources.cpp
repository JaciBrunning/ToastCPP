#include "toast/resources.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"

#include <map>
#include <vector>

#ifndef OS_WIN
#include <unistd.h>
#endif

using namespace Toast;

std::map<std::string, std::string> _file_map;
std::map<std::string, Resources::Resource> _res;

void Resources::link_trx_file(std::string name, std::string file) {
	_file_map[name] = file;
}

void ls_trx_here() {
	std::vector<std::string> rootfiles = Filesystem::ls(".");
	for (auto file : rootfiles) {
		if (ends_with(file, ".trx")) {
			_file_map[Filesystem::basename(file)] = file;
		}
	}
}

#include <iostream>

Resources::Resource *Resources::get_resource_file(std::string module, std::string name) {
	std::string combo = module + "_" + name;
	if (_res.count(combo) != 0) return &_res.at(combo);

	if (_file_map.count(module) == 0) {
		ls_trx_here();
		if (_file_map.count(module) == 0) {
			return nullptr;
		}
	}
	std::string filename = _file_map.at(module);
	FILE *fp = fopen(filename.c_str(), "r");
	rewind(fp);
	int map_size = 0;
	fscanf(fp, "%d:", &map_size);
	char _fn[128];
	for (int i = 0; i < map_size; i++) {
		Resources::Resource trx;
		fscanf(fp, "%[^;];%d@%d;", _fn, &trx.size, &trx._trx_offset);		// Name;Size;Offset
		trx.file = filename;
		_res[module + "_" + std::string(_fn)] = trx;
	}
	long _tell = ftell(fp);
	fclose(fp);
	for (auto it = _res.begin(); it != _res.end(); it++) {
		if (it->second.file == filename) {
			it->second.offset = it->second._trx_offset + _tell;
		}
	}
	if (_res.count(combo) != 0) return &_res.at(combo);
	return nullptr;								// Resource File not Present
}

char read_buf[4096];

std::string Resources::read_resource(Resources::Resource *trx) {
	std::string str = "";
	FILE *fp = trx->open();
	if (trx->size < 4096) {
		fread(read_buf, 1, trx->size, fp);
		str.append(read_buf, trx->size);
		trx->close(fp);
		return str;
	} else {
		char *buffer = (char *)malloc(trx->size);
		fread(buffer, 1, trx->size, fp);
		str.append(buffer, trx->size);
		free(buffer);
		trx->close(fp);
		return str;
	}
}

int Resources::read_resource(Resources::Resource *trx, char *buffer) {
	FILE *fp = trx->open();
	fread(buffer, 1, trx->size, fp);
	trx->close(fp);
	return trx->size;
}

FILE *Resources::Resource::open() {
	FILE *ptr = fopen(this->file.c_str(), "rb");
	this->seek(ptr);
	return ptr;
}

void Resources::Resource::seek(FILE *fp) {
	fseek(fp, this->offset, SEEK_SET);
}

void Resources::Resource::close(FILE *fp) {
	fclose(fp);
}

std::string Resources::get_and_read(std::string module, std::string resource) {
	return Resources::read_resource(Resources::get_resource_file(module, resource));
}
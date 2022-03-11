#ifndef SET_RADIUS_IN_FS_H
#define SET_RADIUS_IN_FS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
const std::string radStr = "radius";

void setRadius(int radius, std::string inPath, std::string outPath) {
	std::fstream f(inPath, std::ios::in);
	if (!f.is_open()) {
		std::cout<<"open file: "<<inPath<<" fail!"<<std::endl;
		f.close();
		return;
	}
	auto ss = std::ostringstream{};
	ss << f.rdbuf();
	std::string shader = ss.str();
	f.close();

	std::fstream out(outPath, std::ios::out);
	if (!out.is_open()) {
		std::cout<<"open file: "<<outPath<<" fail!"<<std::endl;
		out.close();
		return;
	}
	while(true) {
		size_t pos = shader.rfind(radStr);
		if (pos == shader.npos)
			break;
		shader.replace(pos, radStr.size(), std::to_string(radius));
	}
	out<<shader<<std::endl;
	out.close();
}
#endif
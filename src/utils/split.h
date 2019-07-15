#ifndef SPLIT_H_
#define SPLIT_H_

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace utils{
	using namespace std;

	vector<string> &split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}
}



#endif /* SPLIT_H_ */

#include "split.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

namespace utils {
	using namespace std;


	vector<vector<string>> csv(string fileName) {
			vector<vector<string> > csv_values;

			ifstream file(fileName, ios::in);

			if (file) {

				string line;

				while (getline(file,line)) {

					// store array of values
					csv_values.push_back(split(line,','));
				}
			} else {
				cerr << "Error: Unable to open file " << fileName << endl;
				return {};
			}

			return csv_values;
		}
}

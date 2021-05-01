#include <iostream>
#include "dictpara.h"
#include "array.h"
#include <time.h>
#include <vector>

typedef Array<DictPara> Container;
class Badwordclass {
	std::ofstream BadWords;
public:
	Badwordclass(const char* FileName) {
		BadWords.open(FileName);
		if (!BadWords)std::cout << "Cant open File :" + std::string(FileName);
	}
	std::ofstream& getFile() { return BadWords; }
	void inFile(const DictPara& X) { BadWords << X; }
	~Badwordclass() {
		BadWords.close();
	}
};

bool check(Badwordclass &Y,DictPara& X) {
	const char* sup = X.get_word().str(); 
	while (*sup != '\0') {
		if (*sup == ' ') {
			Y.inFile(X);
			return 1;
		}
		++sup;
	}
	if (X.get_word() == '\0') {
		Y.inFile(X);
		return 1;
	}
	else {
		const char* p = X.get_word().str();
		for (; *p != '\0'; p++) {
			int i = 0;
			if (*p == ' ') {
				i++;
				if (i == 2) {
					Y.inFile(X);
					return 1;
				}
			}
		}
	}
	return 0;
}
void Merge(Badwordclass &bad,const DictPara X , Container &A) {
		char* p = X.get_word().str_2();
		std::vector<char*> supVec = { p };
		int count = 1;
		char* sup;
		while (*p != '\0') {
			if (*p == ',') {
				*p = '\0';
				if (*(p - 1) == ' ')*(p - 1) = '\0';
				if (*(p + 1) == ' ')supVec.push_back(p + 2);
				else { supVec.push_back(p++); }
				count++;
			}
			p++;
		}
		for (int i = 0; i < supVec.size(); i++) {
			DictPara Y = DictPara(supVec[i], X.get_translate().str());
			if (!check(bad,Y))A.push_back(Y);
		}
}
int main()
{
	char fname[] = "en_ru.txt";
	Badwordclass File("bad_words.txt");
	try {
		std::ifstream inFile(fname);
		if (!inFile) throw stroka("Can not open dictionary ") + fname;
		DictPara X;
		Container Dictionary;
		int counter = 0;
		long tic = time(NULL);
		while (inFile.peek() != EOF) {
			inFile >> X;
			Merge(File,X, Dictionary);
			++counter;
		}
		inFile.close();
		for (int i = 0; i < Dictionary.size(); i++) {
			std::cout << Dictionary[i];
		}
		std::cout << "Reading from file: " << time(NULL) - tic << " sec" << std::endl;
		std::cout << "Total: " << Dictionary.size() << std::endl;
		std::cout << "Capacity: " << Dictionary.capacity() << std::endl;

		Dictionary.sort();
		std::ofstream outFile("ru-en.txt");
		for (auto pos = Dictionary.begin(); pos != Dictionary.end(); ++pos) // Array<DictPara>::iterator
			 outFile << *pos;

	}
	catch (const stroka error) {
		std::cout << "Error: " << error << std::endl;
	}
	catch (const char* error) {
		std::cout << "Error: " << error << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error" << std::endl;
	}

	return 0;
}


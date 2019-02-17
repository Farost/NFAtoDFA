// Цепочки вида Z=1^+1*01(11+01)^
// ^ - положительная итерация (от 1 до inf)
// * - итерация от 0 до inf

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

static std::string alphabet[36] = { "S", "A", "B", "C", "D", "E", "F", "H", "G", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "T", "U", "V", "W", "X", "Y", "Z" };

class FSMChain {
private:
	std::vector<std::string> letts;
	std::vector<std::string> letters;
	std::vector<std::string> states;
	unsigned letuk = 0;
	std::vector<std::vector<std::string>> table;
	void PushBackTable();
	int FindLetIndex(std::string str);
	
public:
	FSMChain();
	FSMChain(std::string string);
	//FSMChain(FSMChain *fsmchain);
	~FSMChain() { letts.clear();  }
	std::string chain;
	

	void PrintLetts();
	int CheckChain();
	int OptimizeChain();
	int AnalyzeChain();
	int CreateTable();
	void CreateLetters();
	void PrintTable();
	void PrintLetters();
	void PrintStates();

};

FSMChain::FSMChain() {
	chain = "";
}

FSMChain::FSMChain(std::string string) {
	chain = string;
}


void FSMChain::PrintLetts() {
	if (!letts.size()) std::cout << "Letts are empty." << std::endl;
	else for (unsigned i = 0; i < letts.size(); ++i) std::cout << letts[i] << std::endl;
}

int FSMChain::CheckChain() {
	if (!chain.length()) return 0;
	int bopen = 0, bclose = 0;
	unsigned id = 0;

	for (; id < chain.length(); ++id) {
		switch (chain[id]) {
		case '0':

			break;

		case '1':

			break;

		case '(':
			++bopen;
			if (id == chain.length() - 1)
				return -8;       // Ошибка: незавершенная операция в конце выражения

			break;

		case ')':
			if (id == 0)
				return -4;       // Ошибка: операция от пустоты
			++bclose;
			if (bopen < bclose) return -3;     // Ошибка: закрывающих скобок больше, чем открывающих
			if (chain[id - 1] == '(')
				return -6;       // Ошибка: пустые скобки
			if (chain[id - 1] == '+')
				return -7;       // Ошибка: незавершенный "+"

			break;

		case '^':
			if (id == 0)
				return -4;       // Ошибка: операция от пустоты
			if (chain[id - 1] == '(' or chain[id - 1] == '+' or chain[id - 1] == '*' or chain[id - 1] == '^')
				return -5;       // Ошибка: некорректный цикл (после (, +, * или ^))


			break;

		case '*':
			if (id == 0)
				return -4;       // Ошибка: операция от пустоты
			if (chain[id - 1] == '(' or chain[id - 1] == '+' or chain[id - 1] == '*' or chain[id - 1] == '^')
				return -5;       // Ошибка: некорректный цикл (после (, +, * или ^))


			break;

		case '+':
			if (id == 0) return -4;       // Ошибка: операция от пустоты
			else
				if (chain[id - 1] == '(' or chain[id - 1] == ')')
					return -2;       // Ошибка: символ "+" после скобки
			if (id == chain.length() - 1)
				return -8;       // Ошибка: незавершенная операция в конце выражения

			

			break;

		default:
			return -1;               // Ошибка: символ вне алфавита операций и букв
		}
	}

	if (bopen != bclose) return -9; // Ошибка: не соблюден баланс скобок

	return 1;

}

int FSMChain::OptimizeChain() {
	int num = 0, dont = 0, inside = 0, outside = 0;
	std::string::iterator uk1 = chain.begin(), uk2 = chain.begin();
	for (unsigned i = 0; i < chain.length() && !num; ++i) {
		if (chain[i] == '(') {
			uk1 = chain.begin() + i;
			inside = 0;
		}
		else if (chain[i] == '+') {
			++inside;
		}
		else if (chain[i] == ')') {
			uk2 = chain.begin() + i;
			if (chain.length() != i + 1)
				if (chain[i + 1] == '*' || chain[i + 1] == '^')
					++outside;
			if (!outside && !inside) {
				chain.erase(uk2);
				chain.erase(uk1);
				++num;
			}
		}
	}
	return num;
}

int FSMChain::AnalyzeChain() {
	if (!chain.length()) return -1;
	unsigned id = 0, dont = 0, dont2 = 0;
	int bopen = 0, bclose = 0;
	std::string::iterator uk1 = chain.begin(), uk2 = chain.begin();

	
	for (; id < chain.length(); ++id) {
		switch (chain[id]) {
		case '0':
			dont2 = 0;
			if (chain.length() != id + 1 && !dont)
				if (chain[id + 1] == '^' || chain[id + 1] == '*') {
					dont2 = 1;
					++id;
					std::string str(chain.begin() + id - 1, chain.begin() + id + 1);
					letts.push_back(str);
				}
					

			if (!dont && !dont2) {
				std::string str(1, chain[id]);
				letts.push_back(str);
			}

			break;

		case '1':
			dont2 = 0;
			if (chain.length() != id + 1 && !dont)
				if (chain[id + 1] == '^' || chain[id + 1] == '*') {
					dont2 = 1;
					++id;
					std::string str(chain.begin() + id - 1, chain.begin() + id + 1);
					letts.push_back(str);
				}


			if (!dont && !dont2) {
				std::string str(1, chain[id]);
				letts.push_back(str);
			}


			break;

		case '(':
			++bopen;
			if (bopen == bclose + 1) {
				uk1 = chain.begin() + id;
				dont = 1;
			}
			

			break;

		case ')':
			++bclose;
			if (bopen == bclose) {
				uk2 = chain.begin() + id;
				if (chain.length() != id + 1) {
					if (chain[id + 1] != '^' && chain[id + 1] != '*')
						dont = 0;
				}
				else dont = 0;
			}

			if (!dont) {
				std::string str(uk1, uk2 + 1);
				letts.push_back(str);
			}

			break;

		case '^':
			if (chain[id - 1] == ')' && bopen == bclose) {
				++uk2;
				dont = 0;
			}

			if (!dont) {
				std::string str(uk1, uk2 + 1);
				letts.push_back(str);
			}
			

			break;

		case '*':
			if (chain[id - 1] == ')' && bopen == bclose) {
				++uk2;
				dont = 0;
			}

			if (!dont) {
				std::string str(uk1, uk2 + 1);
				letts.push_back(str);
			}


			break;

		case '+':
			if (!dont) {
				std::string str(1, chain[id]);
				letts.push_back(str);
			}

			break;

		default:
			return -2;
		}

	}
	
	/*for (unsigned i = 0; i < letts.size(); ++i) 
		if (letts[i][0] == '(' && letts[i][letts[i].length() - 1] == ')') {
			letts[i].erase(letts[i].begin());
			letts[i].erase(letts[i].end() - 1);
		} */

	

	return 0;
}

void FSMChain::CreateLetters() {
	letters.push_back("0");
	letters.push_back("1");
	letters.push_back("e");
	letters.push_back("END?");
	letuk = 2;
	unsigned short k = 0;

	for (unsigned i = 0; i < letts.size(); ++i) 
		if (letts[i] != "+") {
			k = 0;
			for (unsigned j = 0; j < letters.size(); ++j)
				if (letters[j] == letts[i]) ++k;
			if (!k) 
				letters.insert(letters.begin() + letuk++, letts[i]);
		}
}

void FSMChain::PushBackTable() {
	states.push_back(alphabet[states.size()]);
	std::vector<std::string> str;
	for (unsigned i = 0; i < letters.size(); ++i) {
		str.push_back("_");
	}
	table.push_back(str);
}

int FSMChain::FindLetIndex(std::string str) {
	for (unsigned i = 0; i < letters.size(); ++i)
		if (letters[i] == str) return i;
	return -1;
}

int FSMChain::CreateTable() {
	if (!letts.size()) return 0;
	CreateLetters();
	PushBackTable();

	unsigned beg = 0, curr = 0;
	for (unsigned i = 0; i < letts.size(); ++i) {
		if (curr > 35) return -1;
		if (letts[i] != "+") {
			PushBackTable();
			int id = FindLetIndex(letts[i]);
			if (table[curr][id] == "_")
				table[curr][id] = states[states.size() - 1];
			else table[curr][id].append(states[states.size() - 1]);

			curr = states.size() - 1;
		}
		else {
			table[curr][table[curr].size() - 1] = "1";
			curr = 0;
		}
	}
	table[curr][table[curr].size() - 1] = "1";
	return 1;
}

void FSMChain::PrintLetters() {
	std::cout << "   ";
	for (unsigned i = 0; i < letters.size(); ++i)
		std::cout << letters[i] << "| ";
	std::cout << std::endl;
}

void FSMChain::PrintStates() {
	for (unsigned i = 0; i < states.size(); ++i)
		std::cout << states[i] << " ";
	std::cout << std::endl;
}

void FSMChain::PrintTable() {
	if (!table.size()) std::cout << "Table is empty." << std::endl;
	else {
		PrintLetters();
		for (unsigned i = 0; i < states.size(); ++i) {
			std::cout << states[i] << "| ";
			for (unsigned j = 0; j < letters.size(); ++j) {
				std::cout << table[i][j];
				for (int k = 0; k < letters[j].size() - table[i][j].size(); ++k)
					std::cout << " ";
				std::cout << "| ";
			}
			std::cout << std::endl;
		}
	}
}


void MainTest();

int main()
{
	int again = 1;
	do {
		FSMChain MyChain;
		int k = 0;
		std::cout << "Do you want the main test (1) or your expression (0)?" << std::endl;
		std::cin >> k;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (k == 1) MainTest();
		else {

		

		do {
			std::cout << "Give me the expression. For letters use only 0 and 1." << std::endl;
			std::getline(std::cin, MyChain.chain);

			k = MyChain.CheckChain();
			if (k == 1)
				std::cout << "All is good." << std::endl;
			else std::cout << "ERROR!" << std::endl;
		} while (k != 1);


		int m = 0;
		do {
			k = MyChain.OptimizeChain();
			if (k) ++m;
		} while (k);
		if (m) std::cout << "We've optimized your chain: " << MyChain.chain << std::endl;

		std::cout << "Now i will analyze your chain:" << std::endl;
		MyChain.AnalyzeChain();
		MyChain.PrintLetts();

		std::cout << "----------------------" << std::endl;
		if (MyChain.CreateTable() < 1) {
			std::cout << "ERROR in making Table." << std::endl;
		}
		else {
			MyChain.PrintTable();
			std::cout << "----------------------" << std::endl;
		}


		std::cout << "Again? (1/0)" << std::endl;
		std::cin >> again;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	} while (again);


	std::cout << "Okay, Good Bye!" << std::endl;
	system("Pause");
    return 0;
}


void MainTest() {
	int k;
	std::string str[6] = { "1^+1*01(11+01)^", "1^+1*00(00+11)*", "(0+1)*(1+00)(0+1)*", "(11+01)*(1*+1*01)", "(0+1)*+00+(01+10)", "11+(0+10)(10)*" };
	for (int i = 0; i < 6; ++i) {
		FSMChain MyChain(str[i]);
		std::cout << "CHAIN #" << i + 1 << ": " << MyChain.chain << std::endl;
		k = MyChain.CheckChain();
		if (k != 1) std::cout << "ERROR IN THE CHAIN #" << i + 1 << "!" << std::endl;
		int m = 0;
		do {
			k = MyChain.OptimizeChain();
			if (k) ++m;
		} while (k);
		if (m) std::cout << "Optimized chain #" << i + 1 << ": " << MyChain.chain << std::endl;

		std::cout << "Analyzing chain #" << i + 1 << ":" << std::endl;
		MyChain.AnalyzeChain();
		MyChain.PrintLetts();
		std::cout << "----------------------" << std::endl;
		if (MyChain.CreateTable() < 1) {
			std::cout << "ERROR in making Table." << std::endl;
		}
		else {
			MyChain.PrintStates();
			MyChain.PrintTable();
			std::cout << "----------------------" << std::endl;
		}
	}
	
}
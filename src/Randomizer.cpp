/**
	MIT License

	Copyright (c) 2018 Lord Alzer Casiño

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

/**
	Electronic Raffle Randomizer for ICpEP Assembly
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <windows.h>
#include <conio.h>

void start();
void readList();
void genrand(unsigned lower, unsigned upper);
bool ask_for_roll();
void roll();

std::vector<std::string> list;
unsigned int index;
volatile int state;

const char *filename = "list.txt";   // Name of file to be read.

std::mt19937 eng;

int main()
{
	SetConsoleOutputCP(1252);
	SetConsoleCP(1252);
	std::random_device rd;
	eng.seed(rd());
	readList();
	const int entries = list.size();
	while (true)
	{
		int choice;
		system("cls");
		std::cout << "*************************************" << std::endl;
		std::cout << "*          ICpEP Randomizer         *" << std::endl;
		std::cout << "*************************************" << std::endl;
		std::cout << "There are " << entries << " entries." << std::endl;
		std::cout << "Press 1 to start; press 0 to exit." << std::endl;
		std::cin >> choice; std::cin.get();
		switch (choice)
		{
		case 0:
			return 0;
		case 1:
			start();
			break;
		default:
			std::cout << "Wrong choice. Try again." << std::endl;
			break;
		}
	}
}

void start()
{
	do
	{
		state = 1;
		if (list.empty())
		{
			std::cout << "List is empty." << std::endl;
			system("pause");
			return;
		}
		roll();
	} while (ask_for_roll());
}

void roll()
{
	std::thread roll(genrand, 0, list.size() - 1);
	std::cout << "Press any key to stop rolling." << std::endl;
	_getch();
	state = 0;
	roll.join();
	std::cout << "**********************************************" << std::endl;
	std::cout << "  Congratulations! You won! " << list[index] << std::endl;
	std::cout << "**********************************************" << std::endl;
	list.erase(list.begin() + index);
	system("pause");
}

bool ask_for_roll()
{
	while (true)
	{
		char choice;
		std::cout << "Do you want to roll again? (Y/N): ";
		std::cin >> choice; std::cin.get(); std::cout << std::endl;
		switch (choice)
		{
		case 'Y':
		case 'y':
			return true;
		case 'N':
		case 'n':
			return false;
		default:
			std::cout << "Wrong choice." << std::endl;
			break;
		}
	}
}

void readList()
{
	std::string temp;
	std::ifstream stream;
	stream.open(filename);
	if (stream.is_open())
	{
		while (!stream.eof())
		{
			std::getline(stream, temp);
			if (temp.empty())
				continue;
			list.push_back(temp);
			temp = "";
		}
	}
	stream.close();
}

void genrand(unsigned lower, unsigned upper)
{
	std::uniform_int_distribution<> distr(lower, upper);
	while (state)
	{
		index = distr(eng);
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}
		
}

#include <boost/regex.hpp>
#include <conio.h>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>

#include "MyContainer.hpp"

#define MIN_COUNT_OF_THREADS 1
#define MAX_COUNT_OF_THREADS 64

bool stopAll = false;
std::mutex mymutex;

void threadFunction(MyContainer< int > &cont, size_t id)
{
	while (true)
	{
		int item = rand() % 1000;
		MyElement< int > el(id, item);

		mymutex.lock();		
		cont.add(el);
		mymutex.unlock();
				
		std::this_thread::sleep_for( std::chrono::milliseconds( id * 10 + rand() % 100));	// depends on id

		if (stopAll)
			return;
	}
}

void printStatistics(MyContainer< int > &cont, size_t countOfThreads)
{
	for (size_t id = 0; id < countOfThreads; ++id)
	{
		std::cout << "ID: " << id << "\t COUNT: " << cont.getCountOfElementsById(id) << std::endl;
	}

	std::cout << "TOTAL SIZE: " << cont.getCurrentSize() << "\t FIXED MAXIMUM: " << cont.getFixedMaximum() << std::endl;
}

bool areArgsAppropriate(int argc, char* argv[])
{
	if ((argc <= 2)||(argc > 3))
	{
		std::cout << "Incorrect quantity of input args!" << std::endl;
		return(false);
	}
	else
	{
		boost::regex xRegEx("[1-9][0-9]*");
		if (!(boost::regex_match(argv[1], xRegEx)) | !(boost::regex_match(argv[2], xRegEx)))
		{
			std::cout << "Incorrect parameters!" << std::endl;
			_getch();
			return(false);
		}
		else 
		{
			int x = atoi(argv[1]);// y = atoi(argv[2]);
			if ( (x <= MIN_COUNT_OF_THREADS) || (x > MAX_COUNT_OF_THREADS))
			{
				std::cout << "X must be in inteval (" << MIN_COUNT_OF_THREADS << ", " << MAX_COUNT_OF_THREADS << "]" << std::endl;
				return(false);
			}
		}
	}
	return(true);
}

int main(int argc, char* argv[])
{	

	if (!areArgsAppropriate(argc, argv))
		return(1);

	size_t x = atoi(argv[1]), y = atoi(argv[2]);

	std::cout << "NUMBER OF THREADS: " << x << "\t MAXIMUM SIZE: " << y << "\n\nTo stop threads press any key...\n" << std::endl;

	MyContainer< int > container(y);
	auto rcont = std::ref(container);

	std::vector< std::thread > t;

	for (size_t i = 0; i < x; ++i)
	{
		t.push_back(std::thread(threadFunction, rcont, i));
	}
		
	_getch(); // waiting for keystroke

	stopAll = true; // stop all threads

	for (size_t i = 0; i < x; ++i) // waiting for the completion of all threads
	{
		t.at(i).join();
	}

	printStatistics(rcont, x);

	std::cout << "Enter any key to exit...";
	_getch();

	return(0);
}
#pragma once
#include <iostream>
class MyException
{
private:
	std::string message;
public:
	MyException() = default;
	MyException(std::string message) : message{ message } {};
	void afisare() { std::cout << this->message; };
	~MyException() = default;
};


#include "WeightMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	WeightMap ThetaTau;
	cout << "Epsilon Class Size: " << ThetaTau.size() << endl;	
	ThetaTau.enroll("Nathan Tsai", 4.5);
	ThetaTau.enroll("Jane Lee", 7.5);
	ThetaTau.enroll("Andrew Arifin", 2.5);
	ThetaTau.enroll("Derrick Benson", 3.5);
	ThetaTau.enroll("Damian Gutierrez", 1.25);
	ThetaTau.enroll("Shushanik Stepanyan", 1.25);
	ThetaTau.enroll("Michelle Lee", 3);
	ThetaTau.enroll("YY Wan", 2);
	ThetaTau.enroll("Max Zhu", 4.5);
	ThetaTau.enroll("Frederick Kennedy", 2.5);
	ThetaTau.enroll("Gary Bui", 1.5);

	ThetaTau.print();

	cout << "Passed all tests." << endl;
}
#include <iostream>

using namespace std;
int main(int argc, char** argv) {
	string linia;
	int i;
	linia= system("taskkill /FI \"IMAGENAME eq notepad.exe\" /T /F");
	gcount();
	cout << "linia: " <<linia<< endl;
	system("pause");
	return 0;
}

#include "PolygonHeader.h"
using namespace std;



int main() {
	
	//Create vector storage for shapes
	vector<Polygon*> shapestorage;

	//Call menu function
	TopMenu(shapestorage);

	//Once exited from menu function, delete shapes
	system("cls");
	//Delete each shape in storage vector
	for (auto iterator = shapestorage.begin(); iterator < shapestorage.end(); iterator++) {
		delete *iterator;
	}
	shapestorage.clear(); //Clear vector
	cout << "Stored shapes deleted." << endl;
	cout << "Number of stored shapes: " << shapestorage.size() << endl;
	cout << "Program exiting.";
	cin.get();

	return 0;
}
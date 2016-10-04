#include "PolygonHeader.h"
using namespace std;
	
//Validation checks----------------------------------------------------

// Checks user has inputted valid coordinates
Coordinates coordinatevalidate(string input) {

	bool valid{ true };
	const int error_flag{ -1 };
	Coordinate z;
	
	//Input to try
	try {
		//Find comma position
		unsigned int comma{ input.find_first_of(",") }; 

		//Throw flag if comma in wrong position
		if (comma == 0 || comma == -1) { 
			valid = false; 
			throw error_flag;
		}

		//Extract (X,Y) values for co-ordinate from before/after comma
		z.X = stod(input.substr(0, comma)); 
		z.Y = stod(input.substr(comma + 1));

	}

	//Catch exceptions
	//Not in form of double,double
	catch (int flag) {
		if (flag = error_flag) {
			while (!valid) {
				cout << "Enter valid input: ";
				cin >> input;

				//Find comma position
				unsigned int comma{ input.find_first_of(",") };
				if ((comma != 0) && (comma != -1)) {
					//Extract (X,Y) values for co-ordinate from before/after comma
					z.X = stod(input.substr(0, comma));
					z.Y = stod(input.substr(comma + 1));
					valid = true;
					break;
				}
			}
		}
	}

	//Cannot allocate new memory
	catch (bad_alloc memFail) {
		cout << "Memory allocation failure. Enter valid input: ";		
		cin >> input;

		//Find comma position
		unsigned int comma{ input.find_first_of(",") };

		//Extract (X,Y) values for co-ordinate from before/after comma
		z.X = stod(input.substr(0, comma)); 
		z.Y = stod(input.substr(comma + 1));
	}

	//Not in double format
	catch (exception) {
		cout << "Enter valid input: ";
		cin >> input;

		//Find comma position
		unsigned int comma{ input.find_first_of(",") };

		//Extract (X,Y) values for co-ordinate from before/after comma
		z.X = stod(input.substr(0, comma));
		z.Y = stod(input.substr(comma + 1));
	}

	Coordinates coordinatevalidated(z);
	return coordinatevalidated;
}

//Template function creates number between mininum and maximum values
template <class T> T GetNum(T min, T max) {
	T Num;
	string sNum;

	while (true) {
		cin.clear();
		//Take input and turn into stream to check
		getline(cin, sNum);
		stringstream stream(sNum);
		if (stream >> Num && !(stream >> sNum) && Num >= min && Num <= max) {
			return Num;
		}

		cin.clear();
		cout << endl << "Enter valid number. Try again: ";
	}	
}

//Menu functions------------------------------------------------

//Add a shape, choose between general polygon or manual input, choose sides
Polygon* AddShape() {
	Polygon* temp{ NULL }; //Always return value

	//Get vertices of shape, must be between 3 and 1000
	cout << "Polygon creation: Enter number of vertices (3 to 20): ";
	int vertices{ GetNum(3, 20) };

	//Decide on whether to generate co-ordinates or manually enter
	cout << "Create (g)eneral polygon or (m)anually input co-ordinates: ";
	string choice;
	cin >> choice;
	
	while (choice != "g" && choice != "m") {
		cout << "Enter g or m: ";
		cin >> choice;
	}

	//Generate general polygon
	if (choice == "g") {
		cout << "Enter shape radius from centre to points: ";
		cin.ignore();
		double radius{ GetNum(1e-14, 1e14) };

		cout << endl << "General polygon created about origin with radius " << radius << ": " << endl;
		
		//Initialise class dependent on chosen vertices
		if (vertices == 3) { temp = new Triangle(AddGeneralPolygon<Triangle>(3, radius)); }
		if (vertices == 4) { temp = new Rectangle(AddGeneralPolygon<Rectangle>(4, radius)); }
		if (vertices == 5) { temp = new Pentagon(AddGeneralPolygon<Pentagon>(5, radius)); }
		if (vertices == 6) { temp = new Hexagon(AddGeneralPolygon<Hexagon>(6, radius)); }
		if (vertices > 6) { temp = new GeneralPolygon(AddGeneralPolygon<GeneralPolygon>(vertices, radius)); }
	}

	//Generate polygon from co-ordinates
	if (choice == "m") {
		list<Coordinates*> inputcoords;

		//Push co-ordinates back onto list
		for (int i = 0; i < vertices; i++) {
			cout << "Enter co-ordinate " << i + 1 << ", in form x,y: ";
			string tempcoord; cin >> tempcoord;
			//Allocate a new co-ordinates for input after validation
			inputcoords.push_back(new Coordinates(coordinatevalidate(tempcoord)));
		}
		
		assert(IsShape(inputcoords)); //Assert all co-ordinates different
		
		//Initialise class dependent on chosen vertices
		if (vertices == 3) { 
			//Perform boolean check on co-ordinates for isoceles, create triangle class depending on bool return
			if (IsIsoceles(inputcoords)) { temp = new Isoceles(AddManualPolygon<Isoceles>(3, inputcoords)); }
			else { temp = new Triangle(AddManualPolygon<Triangle>(3, inputcoords)); }
		}
		if (vertices == 4) { temp = new Rectangle(AddManualPolygon<Rectangle>(4, inputcoords)); }
		if (vertices == 5) { temp = new Pentagon(AddManualPolygon<Pentagon>(5, inputcoords)); }
		if (vertices == 6) { temp = new Hexagon(AddManualPolygon<Hexagon>(6, inputcoords)); }
		if (vertices > 6) { temp = new GeneralPolygon(AddManualPolygon<GeneralPolygon>(vertices, inputcoords)); }
		cin.get();
	}

	return temp;
}

//Manipulation choice, enter a manipulation and show manipulated co-ords
void Manipulation(Polygon* pol) {
	cout << "Choose manipulation: 1. Translation 2. Rescale 3. Rotation" << endl;

	//Select choice
	int choice{ GetNum(1, 3) };

	//Switch cases for choice entered
	//Get input of parameters for chosen manipulation, then point to that function
	switch (choice) {
		//Translation
	case 1: {
		cout << "Enter co-ordinates of translation in form x,y: ";
		string coords;
		cin >> coords;
		Coordinates translatecoords{ coordinatevalidate(coords) };
		pol->translate(translatecoords.GetX(), translatecoords.GetY());
		break;
	}
		//Rescaling
	case 2: {
		cout << "Enter position of enlargement in form x,y: ";
		string coords;
		cin >> coords;
		Coordinates rescalecoords{ coordinatevalidate(coords) };

		cout << "Enter scale of enlargement: ";
		cin.ignore();
		double scale{ GetNum(1e-14, 1e14) };
		pol->resize(rescalecoords.GetX(), rescalecoords.GetY(), scale);
		break;
	}
		//Rotation, through centre or point
	case 3: {
		cout << "Enter angle of rotation: ";
		double angle{ GetNum(-1e14, 1e14) };
		cout << "Rotate through shape (c)entre or a (p)oint: ";

		string choice;
		cin >> choice;

		while (choice != "c" && choice != "p") {
			cout << "Enter c or p: ";
			cin >> choice;
		}

		if (choice == "c") {
			pol->rotate_centre(angle);
		}
		else {
			cout << "Enter point of rotation in form x,y: ";
			string coords;
			cin >> coords;
			Coordinates rotatecoords{ coordinatevalidate(coords) };
			pol->rotate(angle, rotatecoords.GetX(), rotatecoords.GetY());
		}			
		break;
	}
	}

	//Print co-ordinates post-manipulation
	cout << "Shape: ";
	pol->DisplayName();
	cout << endl << "Co-ordinates: ";
	pol->PrintCoordinates();
	cin.get();
	cin.get();
}

//Print out all shape info
void PrintLibrary(vector<Polygon*> polygons) {
	for (unsigned int i{ 0 }; i < polygons.size(); i++) {
		//Iterate through vector of shapes, display info for each one
		cout << "Shape " << i + 1 << ": ";	
		polygons[i]->DisplayName();
		cout << ": ";
		polygons[i]->PrintCoordinates();
		cout << endl;
	}
	
}

//Program menu
void TopMenu(vector<Polygon*> shapestorage) {
	
	//Menu choices
	system("cls");
	cout << "What would you like to do:" << endl;
	cout << "1. Add shape" << endl;
	cout << "2. Manipulate shape" << endl;
	cout << "3. Test for overlap" << endl;
	cout << "4. View shape library" << endl;	
	cout << "5. Exit" << endl;

	//Select choice
	int choice{ GetNum(1, 5) };

	system("cls");

	//Switch cases for each menu choice
	switch (choice) {
		//Add shape
	case 1: {
		//Add shape to storage
		shapestorage.push_back(AddShape());

		//Display shape
		int shapeselect{ static_cast<int>(shapestorage.size()) - 1 };
		cout << "Shape: ";
		shapestorage[shapeselect]->DisplayName();
		cout << endl << "Co-ordinates: ";
		shapestorage[shapeselect]->PrintCoordinates();

		//Wait for enter then return to top menu
		cin.get();
		TopMenu(shapestorage);
		break;
	}
			//Manipulate shape
	case 2: {
		//Check at least 1 shape
		if (shapestorage.size() == 0) {
			cout << "No shapes to manipulate";
			cin.get();
			TopMenu(shapestorage);
			break;
		}

		cout << "Which shape to manipulate:" << endl;
		PrintLibrary(shapestorage);

		int manipchoice{ GetNum(1, int(shapestorage.size())) };

		//Clear screen
		system("cls");

		//Display shape data before manipulating
		cout << "Shape: ";
		shapestorage[manipchoice - 1]->DisplayName();
		cout << endl << "Co-ordinates: ";
		shapestorage[manipchoice - 1]->PrintCoordinates();
		cout << endl;
		Manipulation(shapestorage[manipchoice - 1]);
		
		TopMenu(shapestorage);
		break;
	}
		//Test for overlap
	case 3: {
		//Check at least 1 shape
		if (shapestorage.size() == 0) {
			cout << "No shapes to test overlap";
			cin.get();
			TopMenu(shapestorage);
			break;
		}

		cout << "Testing for overlap (any interior points between shapes)." << endl;
		PrintLibrary(shapestorage);
		cout << "Select shape 1: ";
		int overlapchoice1{ GetNum(1, int(shapestorage.size())) };
		cout << "Select shape 2: ";
		int overlapchoice2{ GetNum(1, int(shapestorage.size())) };

		//Check bool if one shape inside the other, and reverse to see if other shape inside the first
		bool overlaptest{ interiorpoints(shapestorage[overlapchoice1 - 1], shapestorage[overlapchoice2 - 1]) };
		bool overlaptest2{ interiorpoints(shapestorage[overlapchoice2 - 1], shapestorage[overlapchoice1 - 1]) };

		if (overlaptest || overlaptest2) {
			cout << "Overlap detected between shapes " << overlapchoice1 << " and " << overlapchoice2;
		}
		else {
			cout << "No overlap detected between shapes " << overlapchoice1 << " and " << overlapchoice2;
		}
		cin.get();
		TopMenu(shapestorage);
		break;
	}
		//Display shape library
	case 4: {
		//Check at least 1 shape
		if (shapestorage.size() == 0) {
			cout << "No shapes to display";
			cin.get();			
			TopMenu(shapestorage);
			break;
		}

		cout << "Number of stored shapes: " << shapestorage.size() << endl;
		PrintLibrary(shapestorage);
		cin.get();
		TopMenu(shapestorage);
		break;
	}
		//Exit from menu
	case 5: {

		return;
	}
	}
}

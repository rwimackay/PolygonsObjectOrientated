#ifndef POLYGONHEADER_H
#define POLYGONHEADER_H
#include<vector>
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<sstream>
#include<cassert>
#include<set>
#include<list>
#include<memory>

using namespace std;

//Define pi
const double pi{ 3.14159265358979323846264338327950288419716939937510582 };

//Structure for a co-ordinate that forms the basis of the co-ordinates class
struct Coordinate { 
	double X, Y; //(X,Y) components for co-ordinate

	//Overload < operator for a lexicographical comparison
	friend bool operator <(Coordinate const& lhs, Coordinate const& rhs)
	{
		return lhs.X < rhs.X || lhs.X == rhs.X && lhs.Y < rhs.Y;		
	}
};

//Co-ordinates class
class Coordinates 
{
	
private:
	Coordinate z;

public:

	//Constructors
	Coordinates() { z.X = z.Y = 0; }
	Coordinates(Coordinate zin) { z.X = zin.X; z.Y = zin.Y; }
	Coordinates(double xin, double yin) { z.X = xin; z.Y = yin; }

	//Destructor
	~Coordinates() {}

	//Accessors
	double GetX() { return z.X; }
	double GetY() { return z.Y; }

	//Set co-ordinates
	void SetX(double xin) { z.X = xin; } 
	void SetY(double yin) { z.Y = yin; } 

	//Overload of input and output
	friend ostream & operator<<(ostream &os, Coordinates* &coords);
	friend istream & operator>>(istream &is, Coordinates* &coords); 

};

//Prototype Functions
double round(double number, int decimals);
bool IsIsoceles(list<Coordinates*> shape);
bool IsShape(list<Coordinates*> shape);
void printcoordinates(list<Coordinates*> shape);
Coordinates coordinatevalidate(string input);

//Prototype manipulations
void shapetranslate(double x, double y, list<Coordinates*> shape);
void shaperesize(double x, double y, double scale, list<Coordinates*> shape);
void shaperotate(double angle, double x, double y, list<Coordinates*> shape);
void shaperotate_centre(double angle, list<Coordinates*> shape);

//Polygon ABC
class Polygon
{

public:

	list<Coordinates*> CoordinateList; //List of co-ordinates to act on

	//Destructor
	virtual ~Polygon() {}

	//Pure virtual functions
	virtual void DisplayName() const = 0; //Display info on polygon
	virtual list<Coordinates*> GetCoordinates() = 0; //Access co-ordinates of shape
	virtual void PrintCoordinates() = 0; //Print co-ordinates

	//Manipulation functions
	virtual void translate(double x, double y) = 0; //Translate by vector (x,y)
	virtual void resize(double x, double y, double scale) = 0; //Resize shape by scale centred on (x,y)
	virtual void rotate(double angle, double x, double y) = 0; //Rotate shape by angle through point, by matrix multiplication of rotation matrix
	virtual void rotate_centre(double angle) = 0; //Rotate shape through centre		
	
};

//Prototype menu functions
void TopMenu(vector<Polygon*> shapestorage);
Polygon* AddShape();
void PrintLibrary(vector<Polygon*> polygons);
void Manipulation(Polygon* pol);
double vectorangle(Coordinates* a, Coordinates* b, Coordinates* c);
bool interiorpoints(Polygon* pol1, Polygon* pol2);

//Derived Shape Classes
class Triangle : public Polygon
{
protected:
	list<Coordinates*> CoordinateList;
	static const int vertices{ 3 };

public:

	//Constructors
	Triangle() { CoordinateList = list<Coordinates*>(vertices); }
	Triangle(list<Coordinates*> vin) {
		assert(vin.size() == vertices); //Assert correct number of sides
		CoordinateList = vin;	
	}

	//Destructors
	~Triangle() { CoordinateList.clear(); }

	//Functions
	list<Coordinates*> GetCoordinates() { return CoordinateList; }
	void PrintCoordinates() { printcoordinates(CoordinateList); }
	void DisplayName() const {		
		cout << "Triangle";
	}
	
	//Manipulations
	void translate(double x, double y) { shapetranslate(x, y, CoordinateList); }
	void resize(double x, double y, double scale) { shaperesize(x, y, scale, CoordinateList); }
	void rotate(double angle, double x, double y) { shaperotate(angle, x, y, CoordinateList); }
	void rotate_centre(double angle) { shaperotate_centre(angle, CoordinateList); }
		
};

class Isoceles : public Triangle
{
public:

	//Constructors
	Isoceles() { CoordinateList = list<Coordinates*>(vertices); }
	Isoceles(list<Coordinates*> vin) {
		assert(vin.size() == vertices); //Assert correct number of sides
		assert(IsIsoceles(vin)); //Assert isoceles check
		CoordinateList = vin;
	} 

	//Destructors
	~Isoceles() { CoordinateList.clear(); }

	//Function override
	void DisplayName() const {
		cout << "Isoceles Triangle";
	}

};

class Rectangle : public Polygon
{
protected:
	list<Coordinates*> CoordinateList;
	static const int vertices{ 4 };

public:

	//Constructors
	Rectangle() { CoordinateList = list<Coordinates*>(vertices); }
	Rectangle(list<Coordinates*> vin) {
		assert(vin.size() == vertices); //Assert correct number of sides
		CoordinateList = vin;
	}

	//Destructors
	~Rectangle(){ CoordinateList.clear(); }

	//Functions
	list<Coordinates*> GetCoordinates() { return CoordinateList; }
	void PrintCoordinates() { printcoordinates(CoordinateList); }
	void DisplayName() const {
		cout << "Rectangle";
	}

	//Manipulations
	void translate(double x, double y) { shapetranslate(x, y, CoordinateList); }
	void resize(double x, double y, double scale) { shaperesize(x, y, scale, CoordinateList); }
	void rotate(double angle, double x, double y) { shaperotate(angle, x, y, CoordinateList); }
	void rotate_centre(double angle) { shaperotate_centre(angle, CoordinateList); }
	
};

class Pentagon : public Polygon
{
protected:
	list<Coordinates*> CoordinateList;
	static const int vertices{ 5 };

public:

	//Constructors
	Pentagon() { CoordinateList = list<Coordinates*>(vertices); }
	Pentagon(list<Coordinates*> vin) {
		assert(vin.size() == vertices); //Assert correct number of sides
		CoordinateList = vin;
	}

	//Destructors
	~Pentagon(){ CoordinateList.clear(); }

	//Functions
	list<Coordinates*> GetCoordinates() { return CoordinateList; }
	void PrintCoordinates() { printcoordinates(CoordinateList); }	
	void DisplayName() const {
		cout << "Pentagon";
	}

	//Manipulations
	void translate(double x, double y) { shapetranslate(x, y, CoordinateList); }
	void resize(double x, double y, double scale) { shaperesize(x, y, scale, CoordinateList); }
	void rotate(double angle, double x, double y) { shaperotate(angle, x, y, CoordinateList); }
	void rotate_centre(double angle) { shaperotate_centre(angle, CoordinateList); }
	
};

class Hexagon : public Polygon
{
protected:
	list<Coordinates*> CoordinateList;
	static const int vertices{ 6 };

public:

	//Constructors
	Hexagon() { CoordinateList = list<Coordinates*>(vertices); }
	Hexagon(list<Coordinates*> vin) {
		assert(vin.size() == vertices); //Assert correct number of sides
		CoordinateList = vin;
	}

	//Destructors
	~Hexagon(){ CoordinateList.clear(); }

	//Functions
	list<Coordinates*> GetCoordinates() { return CoordinateList; }
	void PrintCoordinates() { printcoordinates(CoordinateList); }	
	void DisplayName() const {
		cout << "Hexagon";
	}

	//Manipulations
	void translate(double x, double y) { shapetranslate(x, y, CoordinateList); }
	void resize(double x, double y, double scale) { shaperesize(x, y, scale, CoordinateList); }
	void rotate(double angle, double x, double y) { shaperotate(angle, x, y, CoordinateList); }
	void rotate_centre(double angle) { shaperotate_centre(angle, CoordinateList); }

};

class GeneralPolygon : public Polygon
{
protected:
	list<Coordinates*> CoordinateList;
	int vertices;

public:

	//Constructors
	GeneralPolygon() { CoordinateList = list<Coordinates*>(vertices); }
	GeneralPolygon(list<Coordinates*> vin) {
		vertices = vin.size();
		CoordinateList = vin;
	}

	//Destructors
	~GeneralPolygon(){ CoordinateList.clear(); }
	
	//Functions
	list<Coordinates*> GetCoordinates() { return CoordinateList; }
	void PrintCoordinates() { printcoordinates(CoordinateList); }
	void DisplayName() const {
		cout << "General Polygon: " << vertices << " vertices";
	}

	//Manipulations
	void translate(double x, double y) { shapetranslate(x, y, CoordinateList); }
	void resize(double x, double y, double scale) { shaperesize(x, y, scale, CoordinateList); }
	void rotate(double angle, double x, double y) { shaperotate(angle, x, y, CoordinateList); }
	void rotate_centre(double angle) { shaperotate_centre(angle, CoordinateList); }
	
};

//Template function creates general polygon
template <class T> T AddGeneralPolygon(int vertices, double radius) {
	list<Coordinates*> temp; //Temporary vector to hold co-ordinates
	
	for (int i{ 0 }; i < vertices; i++) {
		//Create co-ordinate of (x,y)
		double x{ radius * cos((2 * pi * i) / vertices) };
		double y{ radius * sin((2 * pi * i) / vertices) };
		temp.push_back(new Coordinates(x, y));
	}

	//Round to 2 decimal places and set
	for_each(temp.begin(), temp.end(), [](Coordinates* coords) { //Apply lambda function to temp vector elements
		//Point to functions for each coords, to set (x,y) co-ordinates to 2 d.p
		coords -> SetX(round(coords -> GetX(), 2));
		coords -> SetY(round(coords -> GetY(), 2) );
	} );

	T tempshape(temp); //Create shape of class T
	return tempshape;
}

//Template function creates polygon with manual input of co-ordinate list
template <class T> T AddManualPolygon(int vertices, list<Coordinates*> inputcoords) {
	T tempshape(inputcoords); 
	return tempshape; 
}

#endif
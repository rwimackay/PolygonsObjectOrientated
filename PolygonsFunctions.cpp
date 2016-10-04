#include "PolygonHeader.h"
using namespace std;

//Functions file

//Friend functions for operator overloading of co-ordinates
ostream & operator<<(ostream &os, Coordinates* &coords) {
	//Output in form (x,y)
	os << "(" << coords->GetX() << "," << coords->GetY() << ")";
	return os;
}

istream & operator>>(istream &is, Coordinates* &coords){
	string input;
	is >> input;
	//Create co-ordinates from valid input function
	Coordinates validinput = coordinatevalidate(input);
	//Set input co-ordinates to above
	coords->SetX(validinput.GetX());
	coords->SetY(validinput.GetY());
	return is;
}

//Functions that act on co-ordinate lists (polygons). Iterate through each co-ordinate and apply a lambda function of the manipulation

//Print co-ordinates
void printcoordinates(list<Coordinates*> shape) {
	for_each(shape.begin(), shape.end(), [](Coordinates* coords) {
		cout << coords;
	});
}

//Manipulations
//Translation by a vector (x,y)
void shapetranslate(double x, double y, list<Coordinates*> shape) {
	for_each(shape.begin(), shape.end(), [&](Coordinates* coords) {
		coords->SetX(coords->GetX() + x);
		coords->SetY(coords->GetY() + y);
	});
}

//Rescale through point by scale
void shaperesize(double x, double y, double scale, list<Coordinates*> shape) {
	for_each(shape.begin(), shape.end(), [&](Coordinates* coords) {
		coords->SetX(scale * ((coords->GetX()) - x) + x);
		coords->SetY(scale * ((coords->GetY()) - y) + y);
	});
}

//Rotation through a point (x,y)
void shaperotate(double angle, double x, double y, list<Coordinates*> shape) {
	
	double radians{ angle * pi / 180 };
	for_each(shape.begin(), shape.end(), [&](Coordinates* coords) {
		//Translate point to the origin
		shapetranslate(-x, -y, shape);		
		//Rotate through origin by 2x2 rotation matrix
		double x1{ coords->GetX() };
		double y1{ coords->GetY() };
		double xprime{ x1*cos(radians) - y1*sin(radians) };
		double yprime{ x1*sin(radians) + y1*cos(radians) };
		coords->SetX(round(xprime, 2));
		coords->SetY(round(yprime, 2));
		//Translate point back
		shapetranslate(x, y, shape);
	});
}

//Find centre of co-ordinates
Coordinates centre(list<Coordinates*> shape) {
	double xsum{ 0 };
	double ysum{ 0 };
	for_each(shape.begin(), shape.end(), [&](Coordinates* coords) {
		double x1{ coords->GetX() };
		double y1{ coords->GetY() };
		xsum += x1;
		ysum += y1;
	});
	//Sum each co-ordinates x,y components then average
	unsigned int n{ shape.size() };
	xsum = xsum / n;
	ysum = ysum / n;
	Coordinates centre(xsum, ysum);
	return centre;
}

//Rotation through centre
void shaperotate_centre(double angle, list<Coordinates*> shape) {
	Coordinates centrepoint(centre(shape));
	double x{ centrepoint.GetX() };
	double y{ centrepoint.GetY() };
	shaperotate(angle, x, y, shape);
}

//Boolean checks for shapes

//Return Pythagorean distance between 2 co-ordinates
double pythagdistance(Coordinate a, Coordinate b) {
	double distance{ pow( pow(a.X - b.X, 2.0) + pow(a.Y - b.Y, 2.0), 0.5) };
	return distance;
}

//Check if a triangle is isoceles
//Uses a set to check how many unique distances between co-ordinates in a set, returns true if 2 sides equal
bool IsIsoceles(list<Coordinates*> shape) {
	assert(shape.size() == 3);
	vector<Coordinates*> shapeconvert(begin(shape), end(shape));
	vector<Coordinate> coordinatecheck(shapeconvert.size());
	set<double> distances;

	//Convert to Coordinate form to compare
	for (unsigned int i{ 0 }; i < shape.size(); i++) {
		coordinatecheck[i].X = shapeconvert[i]->GetX();
		coordinatecheck[i].Y = shapeconvert[i]->GetY();		
	}
	
	//Find distances between each co-ordinates and add to set
	for (int i{ 0 }; i < coordinatecheck.size(); i++) {
		if (i == coordinatecheck.size() - 1) {
			distances.insert(pythagdistance(coordinatecheck[i], coordinatecheck[0]));
		}
		else {
			distances.insert(pythagdistance(coordinatecheck[i], coordinatecheck[i + 1]));
		}		
	}	
	
	//If 2 equal sides then set size is 2, return true
	return (distances.size() == 2) ? true : false;
}

//Checks whether all co-ordinates on shape different
bool IsShape(list<Coordinates*> shape) {
	//Get vector of co-ords from list, convert into co-ordinate structure that can be compared
	set<Coordinate> setcheck;

	//Insert co-ordinate structures into set (able to do since overload struct < operator)
	for_each(shape.begin(), shape.end(), [&](Coordinates* coords) {
		Coordinate z;
		z.X = coords->GetX();
		z.Y = coords->GetY();		
		setcheck.insert(z);
	});

	//Check if set size is equal to number of co-ordinates. If not then have same co-ordinates for shape list and not a shape
	return (setcheck.size() == shape.size()) ? true : false;	
}

//Miscellaneous functions

//Round number to a certain decimal places
double round(double number, int decimals) {
	double round{ roundf(number * pow(10, decimals)) / pow(10, decimals) };
	return round;
}

//Compute angle from point b->a->c
double vectorangle(Coordinates* a, Coordinates* b, Coordinates* c) {

	//Calculate dot product and magnitude of vectors from point a->b and point a->c
	double badotca{ (a->GetX() - b->GetX())*(a->GetX() - c->GetX()) + (a->GetY() - b->GetY())*(a->GetY() - c->GetY()) };
	double magnitudeba{ pow(pow(a->GetX() - b->GetX(), 2.0) + pow(a->GetY() - b->GetY(), 2.0), 0.5) };
	double magnitudeca{ pow(pow(a->GetX() - c->GetX(), 2.0) + pow(a->GetY() - c->GetY(), 2.0), 0.5) };

	//Calculate angle inbetween vectors
	double angle{ acos(badotca / (magnitudeba * magnitudeca)) };
	while (angle > pi) { angle -= 2 * pi; }
	while (angle < -pi) { angle += 2 * pi; }

	return angle;
}

//Check if one shape has points inside another. Assumes that the vertex of each shape is given by consecutive co-ordinates in list and then last point to first
bool interiorpoints(Polygon* pol1, Polygon* pol2) {
	
	//Get vector of co-ordinates for each shape
	list<Coordinates*> list1 = pol1->GetCoordinates();
	list<Coordinates*> list2 = pol2->GetCoordinates();
	vector<Coordinates*> shape1(begin(list1), end(list1));
	vector<Coordinates*> shape2(begin(list2), end(list2));

	//Keep track of overlaps
	int points{ 0 };

	//Iterate through each point in shape 1 to see if it is inside shape 2
	//If point not inside shape, each iteration i will have one point that has an angle of greater than pi
	for (unsigned int i{ 0 }; i < shape1.size(); i++) {

		//Keep track of sum of angles for each point
		double anglesum{ 0. }; 
		for (unsigned int j{ 0 }; j < shape2.size(); j++) {
			if (j == shape2.size() - 1) {
				double intangle{ vectorangle(shape1[i], shape2[j], shape2[0]) };				
				anglesum += intangle;
			}
			else {
				double intangle{ vectorangle(shape1[i], shape2[j], shape2[j + 1]) };				
				anglesum += intangle;
			}
			
			//A point is inside a polygon if the sum of the angles to each pair of points is 2pi
			//Point is inside other polygon, add to overlap
			if (anglesum >= pi) { points++; }
		}
	}
	
	//If overlap, no point will have added to overlap
	return (points == 0) ? false : true;	
}
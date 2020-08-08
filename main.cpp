#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include<vector>

using namespace std;
sf::RenderWindow window(sf::VideoMode(800, 800), "Traffic Simulator");

typedef enum{
corner_bottomleft,
corner_bottomright,
corner_topleft,
corner_topright,
cross,
straight_horizontal,
straight_vertical,
t_bottom,
t_left,
t_right,
t_top,
}tRoadTileType;

typedef enum{
teleport,
smooth,
}tVehicleType;

typedef enum{
green,
red,
}tLightState;

class RoadTile{
	float x, y;
	bool originSet;
	sf::Texture texture;
	sf::Sprite sprite;
	tRoadTileType type;
	
  public:
  	int row, col;
    RoadTile (tRoadTileType t, int r, int c){
    	type = t;
    	row = r;
    	col = c;
	} 
	     
    void Draw();
};

void RoadTile::Draw(){
	// Type is an integer, we combine it with other strings, so with this way we won't need to use any other if else statements
	auto result = "images/roadpieces/" + std::to_string(type)+ ".png";
	if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	// We choose our tiles to be 160 pixels so we, multiply our row and cols with 160
	int x = row*160;
	int y = col*160;
	
	// we scaled our sprites to have 160 pixel size
	sprite.setScale(160.0f/239.0f, 160.0f/239.0f);
	
	sprite.setTexture(texture);

	sprite.setPosition(x, y);
	
	window.draw(sprite);
}

class Waypoint{ 
	int next1, next2, next3, row, col, i_x, i_y;
	bool originSet;
	sf::Texture texture;
	
  public:
    Waypoint (int d, int r, int c, int ix, int iy, int n1, int n2, int n3)
	{
		// We decided not to use just inner index number instead we used relative
		// x and y values directly with respect to main tile.
    	direction = d;
		row = r;
    	col = c;
    	next1 = n1;
    	next2 = n2;
    	next3 = n3;
    	i_x = ix;
    	i_y = iy;
	} 
	
	sf::Sprite sprite;
	int direction;
	bool isOccupied;
	bool isRed;
	bool isStop;
    int getNext();
    void getPosition(float&, float&, float&);
    void Draw();
};

int Waypoint::getNext(){
	int result;
	// This function checks how many possible next directions (Even though there is 2 at maximum, there could be 3 so the maximum is 3 in our function)
	// then, it choose one of them and then return it
	
	if (next2 == -1){
		return next1;
	}
	
	else if (next3 == -1){
		result = rand() % 2;
		if(result == 0){
			return next1;
		}
		else if (result == 1){
			return next2;
		}
	}
	
	else{
		result = rand() % 3;
		if(result == 0){
			return next1;
		}
		else if (result == 1){
			return next2;
		}
		else if (result == 2){
			return next3;
		}
	}
	
}

void Waypoint::getPosition(float& x, float& y, float& dir){
	// Changes the values of x,y and dir values by reference.
	sf::Vector2f position = sprite.getPosition();
	x = position.x;
	y = position.y;
	dir = direction;
}

void Waypoint::Draw(){
	// Instead of using 4 different sprites, we only use 1 of them and rotating it with the given
	// direction value
	auto result = "images/waypoints/right.png";
	if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	
	// scaled it to have 100 pixels size
	sprite.setScale(100.0f/239.0f, 100.0f/239.0f);
	sprite.setTexture(texture);
	
	// we again multiply it with 160 since our tiles have 160 size
	// then we also scale the given index x,y coordinates in pdf to have correct places
	sprite.setPosition(row*160.0f + i_x * 160.0f/239.0f, col*160.0f + i_y * 160.0f/239.0f);
	sprite.setRotation(direction);
	window.draw(sprite);
}

Waypoint wp[48]{
		Waypoint(270, 0, 0, 118, 218, 1, -1, -1),
		Waypoint(0, 0, 0, 218, 121, 2, -1, -1),
		Waypoint(0, 1, 0, 20, 121, 3, -1, -1),
		Waypoint(0, 1, 0, 172, 121, 4, -1, -1),
		Waypoint(0, 2, 0, 20, 121, 5, 6, -1),
		Waypoint(90, 2, 0, 118, 218, 13, -1, -1),
		Waypoint(0, 2, 0, 218, 121, 7, -1, -1),
		Waypoint(0, 3, 0, 20, 121, 8, -1, -1),
		Waypoint(0, 3, 0, 172, 121, 9, -1, -1),
		Waypoint(0, 4, 0, 20, 121, 10, -1, -1),
		Waypoint(90, 4, 0, 118, 218, 15, -1, -1),
		Waypoint(270, 0, 1, 118, 20, 0, -1, -1),
		Waypoint(270, 0, 1, 118, 172, 11, -1, -1),
		Waypoint(90, 2, 1, 118, 20, 14, -1, -1),
		Waypoint(90, 2, 1, 118, 172, 23, -1, -1),
		Waypoint(90, 4, 1, 118, 20, 16, -1, -1),
		Waypoint(90, 4, 1, 118, 172, 29, -1, -1),
		Waypoint(270, 0, 2, 118, 20, 12, -1, -1),
		Waypoint(270, 0, 2, 118, 218, 17, 19, -1),
		Waypoint(0, 0, 2, 218, 121, 20, -1, -1),
		Waypoint(0, 1, 2, 20, 121, 21, -1, -1),
		Waypoint(0, 1, 2, 172, 121, 22, -1, -1), //21
		Waypoint(0, 2, 2, 20, 121, 25, -1, -1),
		Waypoint(90, 2, 2, 118, 20, 25, -1, -1),
		Waypoint(180, 2, 2, 218, 121, 25, -1, -1),
		Waypoint(90, 2, 2, 118, 218, 33, -1, -1),
		Waypoint(180, 3, 2, 20, 121, 24, -1, -1),
		Waypoint(180, 3, 2, 172, 121, 26, -1, -1),
		Waypoint(180, 4, 2, 20, 121, 27, -1, -1), 
		Waypoint(90, 4, 2, 118, 20, 28, 30, -1),
		Waypoint(90, 4, 2, 118, 218, 35, -1, -1), //30
		Waypoint(270, 0, 3, 118, 20, 18, -1, -1),
		Waypoint(270, 0, 3, 118, 172, 31, -1, -1),
		Waypoint(90, 2, 3, 118, 20, 34, -1, -1),
		Waypoint(90, 2, 3, 118, 172, 42, -1, -1),
		Waypoint(90, 4, 3, 118, 20, 36, -1, -1),
		Waypoint(90, 4, 3, 118, 172, 47, -1, -1),
		Waypoint(270, 0, 4, 118, 20, 32, -1, -1),
		Waypoint(180, 0, 4, 218, 121, 37, -1, -1),
		Waypoint(180, 1, 4, 20, 121, 38, -1, -1),
		Waypoint(180, 1, 4, 172, 121, 39, -1, -1),
		Waypoint(180, 2, 4, 20, 121, 40, -1, -1),
		Waypoint(90, 2, 4, 118, 20, 41, -1, -1),
		Waypoint(180, 2, 4, 218, 121, 41, -1, -1),
		Waypoint(180, 3, 4, 20, 121, 43, -1, -1),
		Waypoint(180, 3, 4, 172, 121, 44, -1, -1),
		Waypoint(180, 4, 4, 20, 121, 45, -1, -1),
		Waypoint(90, 4, 4, 118, 20, 46, -1, -1)
		};

class Vehicle{ 
  public:
  	float x=-1, y=-1, prevDir=0;
	sf::Texture texture;
	sf::Sprite sprite;
	Waypoint *prevWP;
	bool moved;
	bool first=true;
  	Waypoint *currentWp;
    Vehicle (Waypoint& swp){
    	currentWp = &swp;
	} 
	    
    virtual void Move ();
    // We made out move function virtual
};

	void Vehicle::Move(){
}

class Car : public Vehicle{ 
	int carNum;
	public:
	// Starting waypoiny and car type is taken as parameter in constructor
	Car(Waypoint& wp, int carN) : Vehicle(wp), carNum(carN)
     {}
	
    void Move();
};

	void Car::Move(){
	// Move the car to the target x and y values and with dir value rotation.
	// Addition to the previous function we added nextPoint parameter to check if the next waypoint is empty
	// and also carNum to load different images for cars
	
	// All the necessary parameters are called below rather than taking them as parameters.
	float testX, testY, testDir;
	currentWp->getPosition(testX, testY, testDir);
	int newX = (int)testX;
	int newY = (int)testY;
	int dir = (int)testDir;

	if (first){
		prevWP = currentWp;
		first=false;
	}
	
	auto result = "images/vehicles/car" + std::to_string(carNum)+ ".png";
	if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	
	// We choose our cor to have 110 pixels size
	sprite.setScale(110.0f/239.0f, 110.0f/239.0f);
	sprite.setTexture(texture);
	
	// If next waypoint is not empty we keep the current coordinates of the car
	// If empty we move to the var and return a bool variable with True
	if(currentWp->isOccupied || prevWP->isRed){
		sprite.setPosition(x, y);
		sprite.setRotation(prevDir);
		window.draw(sprite);
		moved = false;
	}
	else{
		prevWP->isOccupied=false;
		sprite.setPosition(newX, newY);
		sprite.setRotation(dir);
		window.draw(sprite);
		moved = true;
		x = newX;
		y = newY;
		prevDir = dir;
		prevWP = currentWp;
		currentWp->isOccupied = true;
	}
	
	// Next waypoint is found here.
	if (moved){
			currentWp = &wp[currentWp->getNext()];
		}
}

class Bus : public Vehicle{ 
	public:
	    int stopCounter = 0;
	    int routeIndex;
	    int stopCount = 0;
	    vector<int> route;
	    
	    Bus(Waypoint& wp, int start) : Vehicle(wp), routeIndex(start)
     {}
    void Move();
    void addStop(int);
};

	void Bus::addStop(int wpNum){
		// Stop count variable is used to keep the numbers of waypoints in elements.
		// Id of waypoints are push to route vector.
		stopCount++;
		route.push_back(wpNum);
	}
	
	
	void Bus::Move(){
	// Move the car to the target x and y values and with dir value rotation.
	// Addition to the previous function we added nextPoint parameter to check if the next waypoint is empty
	// and also carNum to load different images for cars
	
	float testX, testY, testDir;
	currentWp->getPosition(testX, testY, testDir);
	int newX = (int)testX;
	int newY = (int)testY;
	int dir = (int)testDir;
	
	if (first){
		prevWP = currentWp;
		first=false;
	}
	
	auto result = "images/vehicles/bus.png";
	if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	
	// We choose our cor to have 110 pixels size
	sprite.setScale(110.0f/239.0f, 110.0f/239.0f);
	sprite.setTexture(texture);
	
	// If next waypoint is not empty we keep the current coordinates of the car
	// If empty we move to the car and return a bool variable with True
	if(currentWp->isOccupied || prevWP->isRed){
		sprite.setPosition(x, y);
		sprite.setRotation(prevDir);
		window.draw(sprite);
		moved = false;
	}
	else{
		prevWP->isOccupied=false;
		sprite.setPosition(newX, newY);
		sprite.setRotation(dir);
		window.draw(sprite);
		moved = true;
		x = newX;
		y = newY;
		prevDir = dir;
		prevWP = currentWp;
		currentWp->isOccupied = true;
	}
	
	// In addition to previous stop conditions we add an extra stop condition
	// for stops.
	if(prevWP->isStop){
		sprite.setPosition(x, y);
		sprite.setRotation(prevDir);
		window.draw(sprite);
		moved = false;
		stopCounter++;
		if (stopCounter==10){
			prevWP->isOccupied=false;
			sprite.setPosition(newX, newY);
			sprite.setRotation(dir);
			window.draw(sprite);
			moved = true;
			x = newX;
			y = newY;
			prevDir = dir;
			prevWP = currentWp;
			currentWp->isOccupied = true;
			stopCounter = 0;
		}
	}
	
	if (moved){
			if (routeIndex == (stopCount-1)){
				routeIndex = 0;
			}
			else {
				routeIndex++;
			}
			currentWp = &wp[route[routeIndex]];
		}
}

class TrafficLight{ 
	float x, y, dir;
	tLightState tState;
	Waypoint *wl;
	sf::Texture texture;
	sf::Sprite sprite;
	
  public:
    TrafficLight (Waypoint &wp, tLightState state)
	{
		// Constructor also takes waypoint to control the car movements
		wl = &wp;
    	sf::Vector2f position = wp.sprite.getPosition();
    	x = position.x;
    	y = position.y;
    	tState = state;
	} 
	     
	TrafficLight *next;
    void getPosition(float&, float&, float&);
    void Draw(int, int, int);
    tLightState getState();
    void setState(tLightState);
};

tLightState TrafficLight::getState(){
	return tState;
}

void TrafficLight::setState(tLightState newState){
	// Changes the state of light and waypoint assigned to it.
	tState = newState;
	if (newState == 0){
		wl->isRed=false;
	}
	else if (newState == 1){
		wl->isRed=true;
	}
}

void TrafficLight::getPosition(float& x, float& y, float& d){
	// Returns the position of light
	sf::Vector2f position = sprite.getPosition();
	x = position.x;
	y = position.y;
	d = dir;
}

void TrafficLight::Draw(int xOff,int yOff, int direc){
	// By checking the state of light its sprite is loaded
	// And also, to control its position properly offset values are taken to the function
	dir = direc;
	if (tState == 0){
		auto result = "images/trafficlights/green.png";
		if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	}
	else{
		auto result = "images/trafficlights/red.png";
		if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	}
	// scaled it to have 100 pixels size
	sprite.setScale(100.0f/239.0f, 100.0f/239.0f);
	sprite.setTexture(texture);
	

	sprite.setPosition(x+xOff, y+yOff);
	sprite.setRotation(dir);
	window.draw(sprite);
}

class TrafficLightGroup{ 
	TrafficLight *head, *greenLight, *tail;
	char tankut[8] = {'T', 'A', 'N', 'K', 'U', 'T', '<', '3'};
	int time = 0, duration;
	
  public:
    TrafficLightGroup(float dur, TrafficLight &l)
	{
		head = &l;
		greenLight = head;
    	duration = dur;
    	tail = head;
	} 
	     
    void Add(TrafficLight&);
    void Simulate(int);
};

void TrafficLightGroup::Add(TrafficLight &newLight){
	// NewLight is added to the end of the group and its next becomes head
	tail->next = &newLight;
	newLight.next = head;
	tail = &newLight;
}

void TrafficLightGroup::Simulate(int timestep){
	// Every frame time gets bigger and when it is equal to duration
	// current greenLight will become red and current greenLight's next will become next greenLight
	time = time + timestep;
	if (time == duration){
		greenLight->setState(red);
		greenLight = greenLight->next;
		greenLight->setState(green);
		time = 0;
	}
}

class BusStop{ 
	public:
	sf::Texture texture;
	sf::Sprite sprite;
	
    BusStop(){} 
	     
    void Draw(Waypoint&);
};

	void BusStop::Draw(Waypoint& wp){
	// By checking the state of light its sprite is loaded
	// And also, to control its position properly offset values are taken to the function
	int x,y,dir,xOff,yOff;
	wp.isStop = true;
	
	auto result = "images/busstop/busstop.png";
		if (!texture.loadFromFile(result))
	{
		cout << "Could not find the image file" << endl;
	}
	// scaled it to have 100 pixels size
	dir = wp.direction;
	if (dir == 0 || dir == 180){
		xOff = -10;
		yOff = 32;	
	}
	if (dir == 90){
		xOff = -43;
		yOff = 0;	
	}
	if (dir == 270){
		xOff = 35;
		yOff = 0;	
	}
	sf::Vector2f position = wp.sprite.getPosition();
    x = position.x;
    y = position.y;
	

    	
	sprite.setScale(100.0f/239.0f, 100.0f/239.0f);
	sprite.setTexture(texture);
	

	sprite.setPosition(x+xOff, y+yOff);
	sprite.setRotation(0);
	window.draw(sprite);
}

int main()
{
	//sf::RenderWindow window(sf::VideoMode(800, 800), "Traffic Simulator");
	
	// In this section we first, choose tile type from tRoadTile enum
	// Then we create our tile object with the chosen type
	tRoadTileType t01 = corner_topleft;
	RoadTile tile01(t01, 0, 0);
	tRoadTileType t02 = straight_vertical;
	RoadTile tile02(t02, 0, 1);
	tRoadTileType t03 = t_left;
	RoadTile tile03(t03, 0, 2);
	tRoadTileType t04 = straight_vertical;
	RoadTile tile04(t04, 0, 3);
	tRoadTileType t05 = corner_bottomleft;
	RoadTile tile05(t05, 0, 4);
	tRoadTileType t21= straight_horizontal;
	RoadTile tile21(t21, 1, 0);
	tRoadTileType t06 = t_top;
	RoadTile tile06(t06, 2, 0);
	tRoadTileType t07 = straight_horizontal;
	RoadTile tile07(t07, 3, 0);
	tRoadTileType t08 = corner_topright;
	RoadTile tile08(t08, 4, 0);
	tRoadTileType t09 = straight_horizontal;
	RoadTile tile09(t09, 1, 4);
	tRoadTileType t10 = t_bottom;
	RoadTile tile10(t10, 2, 4);
	tRoadTileType t11 = straight_horizontal;
	RoadTile tile11(t11, 3, 4);
	tRoadTileType t12 = corner_bottomright;
	RoadTile tile12(t12, 4, 4);
	tRoadTileType t13 = straight_vertical;
	RoadTile tile13(t13, 4, 3);
	tRoadTileType t14 = t_right;
	RoadTile tile14(t14, 4, 2);
	tRoadTileType t15 = straight_vertical;
	RoadTile tile15(t15, 4, 1);
	tRoadTileType t16 = cross;
	RoadTile tile16(t16, 2, 2);
	tRoadTileType t17 = straight_vertical;
	RoadTile tile17(t17, 2, 1);
	tRoadTileType t18 = straight_vertical;
	RoadTile tile18(t18, 2, 3);
	tRoadTileType t19 = straight_horizontal;
	RoadTile tile19(t19, 1, 2);
	tRoadTileType t20 = straight_horizontal;
	RoadTile tile20(t20, 3, 2);
	
	// waypoint (int d, int r, int c, int ix, int iy, int n1, int n2, int n3)
	// We created an object array to have easier access to all our objects
	// This allowed us to use directly returned integer value of Waypoint::getNext() Function
	
	// We choose the desired movement type for the vehicle (choose smooth for smooth movement)
	tVehicleType vehicle = teleport;
	// Create car object here.
	Car car(wp[6],1);
	Car car2(wp[25],2);
	Car car3(wp[12],3);
	Car car4(wp[29],4);
	Car car5(wp[34],5);
	Car car6(wp[47],6);
	
	Bus bus1(wp[1], 0);
	Bus bus2(wp[10], 8);
	
	BusStop bs1;
	BusStop bs2;
	BusStop bs3;
	BusStop bs4;
	BusStop bs5;
	
	for (int i = 0; i < 48; i++){
			wp[i].Draw();
		}
	
	TrafficLight l1(wp[22], green);
	TrafficLight l2(wp[23], green);
	TrafficLight l3(wp[24], green);
	TrafficLight l4(wp[42], green);
	TrafficLight l5(wp[43], green);
	
	l2.setState(red);
	l3.setState(red);
	l5.setState(red);
	
	TrafficLightGroup g1(20, l1);
	g1.Add(l2);
	g1.Add(l3);
	TrafficLightGroup g2(20, l4);
	g2.Add(l5);
	
	// Add Route
	bus1.addStop(1);
	bus1.addStop(2);
	bus1.addStop(3);
	bus1.addStop(4);
	bus1.addStop(6);
	bus1.addStop(7);
	bus1.addStop(8);
	bus1.addStop(9);
	bus1.addStop(10);
	bus1.addStop(15);
	bus1.addStop(16);
	bus1.addStop(29);
	bus1.addStop(30);
	bus1.addStop(35);
	bus1.addStop(36);
	bus1.addStop(47);
	bus1.addStop(46);
	bus1.addStop(45);
	bus1.addStop(44);
	bus1.addStop(43);
	bus1.addStop(41); //43
	bus1.addStop(40);
	bus1.addStop(39);
	bus1.addStop(38);
	bus1.addStop(37);
	bus1.addStop(32);
	bus1.addStop(31);
	bus1.addStop(18);
	bus1.addStop(17);
	bus1.addStop(12);
	bus1.addStop(11);
	bus1.addStop(0);
	
	
	bus2.addStop(1);
	bus2.addStop(2);
	bus2.addStop(3);
	bus2.addStop(4);
	bus2.addStop(6);
	bus2.addStop(7);
	bus2.addStop(8);
	bus2.addStop(9);
	bus2.addStop(10);
	bus2.addStop(15);
	bus2.addStop(16);
	bus2.addStop(29);
	bus2.addStop(30);
	bus2.addStop(35);
	bus2.addStop(36);
	bus2.addStop(47);
	bus2.addStop(46);
	bus2.addStop(45);
	bus2.addStop(44);
	bus2.addStop(43);
	bus2.addStop(41); //43
	bus2.addStop(40);
	bus2.addStop(39);
	bus2.addStop(38);
	bus2.addStop(37);
	bus2.addStop(32);
	bus2.addStop(31);
	bus2.addStop(18);
	bus2.addStop(17);
	bus2.addStop(12);
	bus2.addStop(11);
	bus2.addStop(0);
	
	while (window.isOpen()) //This is the main loop, the simulation should take place within this loop
	{
	// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			window.close();
		}
		window.clear(sf::Color::White);
		
		// Draw tiles on window
		tile01.Draw();
		tile02.Draw();
		tile03.Draw();
		tile04.Draw();
		tile05.Draw();
		tile21.Draw();
		tile06.Draw();
		tile07.Draw();
		tile08.Draw();
		tile09.Draw();
		tile10.Draw();
		tile11.Draw();
		tile12.Draw();
		tile13.Draw();
		tile14.Draw();
		tile15.Draw();
		tile16.Draw();
		tile17.Draw();
		tile18.Draw();
		tile19.Draw();
		tile20.Draw();
		
		bs1.Draw(wp[31]);
		bs2.Draw(wp[4]);
		bs3.Draw(wp[36]);
		bs4.Draw(wp[17]);
		bs5.Draw(wp[41]);

		for (int i = 0; i < 48; i++){
			wp[i].Draw();
		}
		// (window, x, y, direction)
		
		g1.Simulate(1);
		g2.Simulate(1);
				
		l1.Draw(30, 30, 90);
		l2.Draw(-30, 25, 180);
		l3.Draw(-25, 40, 270);
		l4.Draw(-30, 25, 180);
		l5.Draw(-25, 40, 270);

		// Move car to desired coordinates
		car.Move();
		car2.Move();
		car3.Move();
		car4.Move();
		car5.Move();
		car6.Move();
		
		bus1.Move();
		bus2.Move();

		window.display();
		
		// You may change the speed of the car
		float speed = 0.2f;
		sf::sleep(sf::seconds(speed));
	}
}


//this is a class that contains Objects 
#ifndef ROOM
#define ROOM

#include "Objects.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct dimensionsRoom
{
	int xSize = 0;// height
	int ySize = 0;// width
};


class Room
{
protected:
	string roomID;
	dimensionsRoom size;
	vector <vector <char> > mapRoom;// map of room WITHOUT exterior walls. 
	vector < Object* > objRoom;// container for all objects (represented by pointers) within Room. 

public:
	Room()// will not be in final implementation
	{
		size.xSize = 5;// used to test
		size.ySize = 5;// used to test
		objRoom;
		roomTemplate(size);
	}

	Room(string file)// explicit constructor
	{
		fstream fin;
		fin.open(file);
		if (fin.is_open())
		{
			cout << "File opened successfully!\n";// only needed during testing
			int targetInfo;
			dimensionsRoom tempI;
			string input, tempS;

			/* this block assigns roomID */
			getline(fin, input);
			for (targetInfo = 0; input[targetInfo] != ':'; targetInfo++) {} // loops until ':' reached
			targetInfo += 2;
			for (; targetInfo < input.size(); targetInfo++) // loops until end of line
			{
				tempS += input[targetInfo];
			}
			roomID = tempS;// assigns room ID

			/* this block assigns Room dimensions */
			getline(fin, input);
			for (targetInfo = 0; input[targetInfo] != ':'; targetInfo++) {} // loops until ':' reached
			targetInfo += 2;
			for (; !isspace(input[targetInfo]); targetInfo++) // loosp until a space is found
			{
				tempI.xSize = (tempI.xSize * 10) + ((int)input[targetInfo] - 48); // reduces ASCII value to integer after typecasting
			}
			targetInfo++;
			for (; targetInfo < input.size(); targetInfo++) // loops until end of line
			{
				tempI.ySize = (tempI.ySize * 10) + ((int)input[targetInfo] - 48); // reduces ASCII value to integer after typecasting
			}
			setSize(tempI); // assigns dimensions of Room

			/* this block assigns Objects information */
			while (!fin.eof())
			{
				coordinateObject tempC;
				tempS = "";

				/* this block finds the object name */
				getline(fin, input);
				for (targetInfo = 0; !isspace(input[targetInfo]); targetInfo++) {} // loops until a space is found
				targetInfo += 1;
				for (; input[targetInfo] != ':'; targetInfo++)  // loops until ':' reached
				{
					tempS += input[targetInfo];
				}

				/* this block assigns the coordinates of the object */
				targetInfo += 2;
				for (; !isspace(input[targetInfo]); targetInfo++) // loops until a space is found
				{
					tempC.row = (tempC.row * 10) + ((int)input[targetInfo] - 48); // reduces ASCII value to digit after typecasting
				}
				targetInfo++;
				for (; targetInfo < input.size(); targetInfo++) // loops until end of line
				{
					tempC.col = (tempC.col * 10) + ((int)input[targetInfo] - 48); // reduces ASCII value to digit after typecasting
				}

				/* this block creates the object and pushes it back into vector objRoom */
				if (tempS == "door")
				{
					Door *door = new Door;
					(*door).setLocation(tempC);
					objRoom.push_back(door);
				}
				else if(tempS == "window")
				{
					Window *window = new Window;
					(*window).setLocation(tempC);
					objRoom.push_back(window);
				}
				else if(tempS == "stairs")
				{
					Stairs *stairs = new Stairs;
					(*stairs).setLocation(tempC);
					objRoom.push_back(stairs);
				}
				else if(tempS == "obstruction")
				{
					Obstruction *obstruction = new Obstruction;
					(*obstruction).setLocation(tempC);
					objRoom.push_back(obstruction);
				}
				else if(tempS == "entity")
				{
					Entity *entity = new Entity;
					(*entity).setLocation(tempC);
					objRoom.push_back(entity);
				}
			}
			roomTemplate(size);
		}
		else cout << "Input file error!\n";

		fin.close();
	}

	void setSize(dimensionsRoom & item)
	{
		size.xSize = item.xSize;
		size.ySize = item.ySize;
	}

	dimensionsRoom getSize() const // possibly useful for fullMap
	{
		return size;
	}

	vector <vector <char> > getRoom() const
	{
		return mapRoom;
	}

	void roomTemplate(dimensionsRoom & item)
	{
		//cout << item.xSize << endl << item.ySize << endl; // testline!!!!!!!!!!!!!!!!!!!!!
		for (int x = 0; x < item.xSize; x++)
			// creates initial 2-d vector filled with '.'
		{
			vector <char> temp;
			for (int y = 0; y < item.ySize; y++)
			{
				temp.push_back('.');
			}
			mapRoom.push_back(temp);
		}
		for (int x = 0; x < objRoom.size(); x++)
		{
			coordinateObject temp = (*objRoom[x]).getLocation();
			mapRoom[temp.row][temp.col] = (*objRoom[x]).getSymbol();
		}
	}

	friend ostream & operator <<(ostream & out, Room item)
		//called by Database.h
		//needs to account for objects, and "shift" doors and windows outward into the walls. 
	{
		for (int i = 0; i < item.size.ySize + 2; i++)
		{
			out << "# ";
		}
		out << endl;
		
		for (int x = 0; x < item.size.xSize; x++)
		{
			out << "# ";
			for (int y = 0; y < item.size.ySize; y++)
			{
				out << item.mapRoom[x][y] << " ";
			}
			out << "#" << endl;
		}
		for (int i = 0; i < item.size.ySize + 2; i++)
		{
			out << "# ";
		}
		out << endl;
		
		return out;
	}
};
#endif
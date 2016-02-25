/*#include <iostream>
#include <string>
using namespace std;*/

// Landmark class declaration 
class Landmark
{
  public:
	Landmark(string name);
	virtual ~Landmark();
	// not virtual because every derived class has a name given in the Constructor
	string name() const;
	// pure virtual because every derived class has a different color and icon
	virtual string color() const = 0;
	virtual string icon() const = 0;
  private:
	string m_name;
};

// Landmark class implementation
Landmark::Landmark(string name)
	: m_name(name)
{}

Landmark::~Landmark()
{}

string Landmark::name() const
{
	return m_name;
}

// Hotel class declaration
class Hotel : public Landmark
{ 
  public:
	Hotel(string name);
	virtual ~Hotel();
	virtual string color() const;
	virtual string icon() const;
  private:
};

// Hotel class implementation
Hotel::Hotel(string name)
	: Landmark(name)
{}

Hotel::~Hotel()
{
	cout << "Destroying the hotel " << name() << "." << endl;
}

string Hotel::color() const
{
	return "yellow";
}

string Hotel::icon() const
{
	return "bed";
}

// Restaurant class declaration
class Restaurant : public Landmark
{
  public:
	Restaurant(string name, int capacity);
	virtual ~Restaurant();
	virtual string color() const;
	virtual string icon() const;
  private:
	int m_capacity;
};

// Restaurant class implementation
Restaurant::Restaurant(string name, int capacity)
	: Landmark(name), m_capacity(capacity)
{
}

Restaurant::~Restaurant()
{
	cout << "Destroying the restaurant " << name() << "." << endl;
}

string Restaurant::color() const
{
	return "yellow";
}

string Restaurant::icon() const
{
	if (m_capacity < 40)
		return "small knife/fork";
	else 
		return "large knife/fork";
}

// Hospital class declaration
class Hospital : public Landmark
{
  public:
	  Hospital(string name);
	  virtual ~Hospital();
	  virtual string color() const;
	  virtual string icon() const;
  private:
};

// Hospital class implementation
Hospital::Hospital(string name)
	: Landmark(name)
{
}

Hospital::~Hospital()
{
	cout << "Destroying the hospital " << name() << "." << endl;
}

string Hospital::color() const
{
	return "blue";
}

string Hospital::icon() const
{
	return "H";
}

/* display and main functions
void display(const Landmark* lm)
{
	cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
		<< lm->name() << "." << endl;
}

int main()
{
	Landmark* landmarks[4];
	landmarks[0] = new Hotel("Westwood Rest Good");
	// Restaurants have a name and seating capacity.  Restaurants with a
	// capacity under 40 have a small knife/fork icon; those with a capacity
	// 40 or over have a large knife/fork icon.
	landmarks[1] = new Restaurant("Bruin Bite", 30);
	landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
	landmarks[3] = new Hospital("UCLA Medical Center");

	cout << "Here are the landmarks." << endl;
	for (int k = 0; k < 4; k++)
		display(landmarks[k]);

	// Clean up the landmarks before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete landmarks[k];
}*/
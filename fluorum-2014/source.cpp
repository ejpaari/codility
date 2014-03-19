#include <vector>

using std::vector;

struct City
{
	int value;			// How many cities will be visited if going to this city.
	bool visited;		// Is the city visited.
	vector<int> links;	// Links to other cities.
};

vector<City> graph;
vector<int> leafs;

void createGraph(const vector<int> &T)
{
	graph.resize(T.size());

	// Initialize graph values and add links to between cities.
	for (unsigned int i = 0; i < T.size(); ++i)
	{
		graph[i].visited = false;
		graph[i].value = 0;

		if (T[i] != static_cast<int>(i))
		{
			graph[i].links.push_back(T[i]);
			graph[T[i]].links.push_back(i);
		}
	}
}

void findLeafs(int startCity)
{
	// If there is only one link then the city must be a leaf. Leafs are added in ascending order.
	// Start city should not be added because it is visited by default.
	for (unsigned int i = 0; i < graph.size(); ++i)
	{
		if (graph[i].links.size() == 1 && i != static_cast<unsigned int>(startCity))
		{
			leafs.push_back(i);
		}
	}
}

void findCityValues(int city, int &lastCity, int &value)
{
	// Find the value from city to all the "child cities" (if the graph is considered as tree).
	graph[city].value = value;

	for (unsigned int i = 0; i < graph[city].links.size(); ++i)
	{
		if (graph[city].links[i] != lastCity)
		{
			findCityValues(graph[city].links[i], city, ++value);
		}
	}

	--value;
}

int getLeafWithGreatestValue(int &greatestValue, int &leafIndex)
{
	int city = 0;

	// Go through all the leafs and find the greatest value. 
	// Leafs are in ascending order so there is no need to separately check situations where values are equal (lower city should be selected).
	for (unsigned int i = 0; i < leafs.size(); ++i)
	{
		if (graph[leafs[i]].value > greatestValue)
		{
			greatestValue = graph[leafs[i]].value;
			city = leafs[i];
			leafIndex = i;
		}
	}

	return city;
}

void updateCityValues(int city, int lastCity, int &startValue)
{
	// Go through all the "child cities" of the given city.
	for (unsigned int i = 0; i < graph[city].links.size(); ++i)
	{
		int nextCity = graph[city].links[i];

		// Don't go backwards or already visited cities.
		if (lastCity != nextCity && graph[nextCity].value != 0)
		{
			// If the value is lower it means that the city is in the travelled route (set value to 0).
			if (graph[nextCity].value < graph[city].value)
			{
				updateCityValues(nextCity, city, startValue);				
			}
			// Find new values for cities that are along the route but not visited.
			else
			{
				findCityValues(nextCity, city, ++startValue);
			}
		}
	}
	
	graph[city].value = 0;
}

vector<int> solution(int K, vector<int> &T)
{
	createGraph(T);
	findLeafs(K);
	graph[K].visited = true;

	int lastCity = K;
	int value = 0;
	findCityValues(K, lastCity, value);

	vector<int> mySolution;
	mySolution.push_back(K);

	int greatestValue = 0;
	int startValue = 0;
	int city = 0;
	int leafIndex = 0;

	while (leafs.size() > 0)
	{
		city = getLeafWithGreatestValue(greatestValue, leafIndex);
		
		// Small optimization: There are only leafs left and they should be visited in ascending order.
		if (greatestValue == 1)
		{
			for (unsigned int i = 0; i < leafs.size(); ++i)
			{
				mySolution.push_back(leafs[i]);				
			}
			break;
		}

		mySolution.push_back(city);

		updateCityValues(city, city, startValue);
		leafs.erase(leafs.begin() + leafIndex);

		greatestValue = 0;
	}

	return mySolution;
}
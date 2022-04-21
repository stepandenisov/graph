#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

template<class TVertex, class TEdge>
class Graph
{
private:
	struct destination {
		int index;
		TEdge edge;
		TVertex destination_vertex;
	};

	struct vertex {
		int index;
		TVertex source_vertex;
		vector<destination> list_of_destinations;
	};

	vector<vertex> table;

public:
	int index(const TVertex& v)
	{
		for (int i = 0; i < table.size; i++)
		{
			if (table.at(i).source_vertex == v) { return table.at(i).index; }
		}
		return -1;
	}

	void add_vertex(const TVertex& v)
	{
		if (index(v) > 0)
		{
			vertex av;
			av.index = table.size;
			av.source_vertex = v;
			table.push_back(&av);
		}
		throw "This vertex is already exists!";
	}

	void add_edge(const TVertex& source, const TVertex& dest, const TEdge& edge)
	{
		int index_source = index(source);
		int index_destination = index(destination);
		if (index_source * index_destination < 0) { throw "The vetrtex is not exists"; }
		destination new_dest;
		new_dest.index = table.at(index_source).list_of_destinations.size;
		new_dest.destination_vertex = destination;
		new_dest.edge = edge;
		table.at(index_source).list_of_destinations.push_back(new_dest);
	}

	void del_vertex(const TVertex& target)
	{
		int index_of_target = index(target);
		if (index_of_target > 0)
		{
			for (int i = 0; i < index_of_target; i++)
			{
				for (int j = 0; j < table.at(i).list_of_destinations.size; j++)
				{
					if (table.at(i).list_of_destinations.at(j).destination_vertex == target)
					{
						for (int k = j; k < table.at(i).list_of_destinations.size - 1; k++)
						{
							table.at(i).list_of_destinations.at(k).destination_vertex = table.at(i).list_of_destinations.at(k + 1).destination_vertex;
						}
						table.at(i).list_of_destinations.pop_back();
					}
				}
			}
			for (int i = index_of_target + 1; i < table.size; i++)
			{
				for (int j = 0; j < table.at(i).list_of_destinations.size; j++)
				{
					if (table.at(i).list_of_destinations.at(j).destination_vertex == target)
					{
						for (int k = j; k < table.at(i).list_of_destinations.size - 1; k++)
						{
							table.at(i).list_of_destinations.at(k).destination_vertex = table.at(i).list_of_destinations.at(k + 1).destination_vertex;
						}
						table.at(i).list_of_destinations.pop_back();
					}
				}
			}
			for (int i = index_of_target; i < table.size-1; i++)
			{
				table.at(i) = table.at(i + 1);
			}
			table.pop_back();
		}
		throw "This vertex is not exists!";
	}

	void del_edge(const TVertex& source, const TVertex& dest)
	{
		int index_source = index(source);
		int index_destination = index(destination);
		if (index_source * index_destination < 0) { throw "The vetrtex is not exists"; }
		for (int i = 0; i < table.at(index_source).list_of_destinations.size; i++)
		{
			if (table.at(index_source).list_of_destinations.at(i).destination == dest)
			{
				for (int j = i; j < table.at(index_source).list_of_destinations.size - 1; j++)
				{
					table.at(index_source).list_of_destinations.at(j).destination_vertex = table.at(index_source).list_of_destinations.at(j + 1).destination_vertex;
				}
				table.at(index_source).list_of_destinations.pop_back();
			}
		}
	}
};

int main()
{
	return 0;
}




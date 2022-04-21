#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

template<class TVertex, class TEdge>
class Graph
{
private:
	struct destination {
		unsigned index;
		TEdge edge;
		TVertex destination_vertex;
	};

	struct vertex {
		unsigned index;
		TVertex source_vertex;
		vector<destination> list_of_destinations;
	};

	vector<vertex> table;

public:
	int index(const TVertex& v)
	{
		for (unsigned i = 0; i < table.size(); i++)
		{
			if (table.at(i).source_vertex == v) { return table.at(i).index; }
		}
		return -1;
	}

	void add_vertex(const TVertex& v)
	{
		if (index(v) < 0)
		{
			vertex av;
			av.index = table.size();
			av.source_vertex = v;
			table.push_back(av);
		}
		else { throw "This vertex is already exists!"; }
	}

	void add_edge(const TVertex& source, const TVertex& dest, const TEdge& edge)
	{
		int index_source = index(source);
		int index_destination = index(dest);
		if ((index_source < 0) || (index_destination < 0)) { return; }//throw "The vetrtex is not exists"; }
		else
		{
			destination new_dest;
			new_dest.index = table.at(index_source).list_of_destinations.size();
			new_dest.destination_vertex = dest;
			new_dest.edge = edge;
			table.at(index_source).list_of_destinations.push_back(new_dest);
		}
	}

	void del_vertex(const TVertex& target)
	{
		int index_of_target = index(target);
		if (index_of_target >= 0)
		{
			for (unsigned i = 0; i < unsigned(index_of_target); i++)
			{
				for (unsigned j = 0; j < table.at(i).list_of_destinations.size(); j++)
				{
					if (table.at(i).list_of_destinations.at(j).destination_vertex == target)
					{
						if (table.at(i).list_of_destinations.size() > 1)
						{
							for (unsigned k = j; k < table.at(i).list_of_destinations.size() - 1; k++)
							{
								table.at(i).list_of_destinations.at(k).destination_vertex = table.at(i).list_of_destinations.at(k + 1).destination_vertex;
							}
						}
						table.at(i).list_of_destinations.pop_back();
					}
				}
			}
			for (unsigned i = unsigned(index_of_target)+1; i < table.size(); i++)
			{
				for (unsigned j = 0; j < table.at(i).list_of_destinations.size(); j++)
				{
					if (table.at(i).list_of_destinations.at(j).destination_vertex == target)
					{
						if (table.at(i).list_of_destinations.size() > 1)
						{
							for (unsigned k = j; k < table.at(i).list_of_destinations.size() - 1; k++)
							{
								table.at(i).list_of_destinations.at(k).destination_vertex = table.at(i).list_of_destinations.at(k + 1).destination_vertex;
							}
						}
						table.at(i).list_of_destinations.pop_back();
					}
				}
			}
			for (unsigned i = index_of_target; i < table.size()-1; i++)
			{
				table.at(i) = table.at(i + 1);
				table.at(i).index--;
			}
			table.pop_back();
			return;
		}
		throw "This vertex is not exists!";
	}

	void del_edge(const TVertex& source, const TVertex& dest)
	{
		int index_source = index(source);
		int index_destination = index(dest);
		if (index_source * index_destination < 0) { throw "The vetrtex is not exists"; }
		for (unsigned i = 0; i < table.at(index_source).list_of_destinations.size(); i++)
		{
			if (table.at(index_source).list_of_destinations.at(i).destination_vertex == dest)
			{
				for (unsigned j = i; j < table.at(index_source).list_of_destinations.size() - 1; j++)
				{
					table.at(index_source).list_of_destinations.at(j).destination_vertex = table.at(index_source).list_of_destinations.at(j + 1).destination_vertex;
				}
				table.at(index_source).list_of_destinations.pop_back();
			}
		}
	}

	void print()
	{
		for (unsigned i = 0; i < table.size(); i++)
		{
			cout << "From ";
			cout << table.at(i).source_vertex;
			cout << " to ";
			for (unsigned j = 0; j < table.at(i).list_of_destinations.size(); j++)
			{
				cout << table.at(i).list_of_destinations.at(j).destination_vertex;
				cout << " by ";
				cout << table.at(i).list_of_destinations.at(j).edge;
				cout << " and to ";
			}
			cout << endl;
		}
	}

	struct u {
		unsigned color = 0;
		vertex cur;
		TVertex prev;
		vector<u> dest_list;
		unsigned d;
	};

	void width_search()
	{
		vector<u> U;
		for (unsigned i = 0; i < table.size(); i++)
		{
			u tmp;
			tmp.cur = table.at(i);
			//tmp.prev = table.at(i).source_vertex;
			tmp.d = NAN;
			U.push_back(tmp);
		}
		for (unsigned i = 0; i < U.size(); i++)
		{
			if (U.at(i).color == 0) { ws(U.at(i), U); }
		}
	}

	void ws(u& s, vector<u>& U)
	{
		queue<u> Q;
		s.color = 1;
		s.d = 0;
		Q.push(s);
		while (!Q.empty())
		{
			u a = Q.front();
			Q.pop();
			cout << endl << "from ";
			cout << a.cur.source_vertex;
			cout << " to ";
			for (unsigned i = 0; i < a.cur.list_of_destinations.size(); i++)
			{
				unsigned id = index(a.cur.list_of_destinations.at(i).destination_vertex);
				cout << U.at(id).cur.source_vertex << " and ";
				if (U.at(id).color == 0)
				{
					U.at(id).color = 1;
					U.at(id).d = a.d + 1;
					U.at(id).prev = a.cur.source_vertex;
					Q.push(U.at(id));
				}
			}
			cout << endl;
			U.at(index(a.cur.source_vertex)).color = 2;
		}
	}
};

int main()
{
	Graph<int, int> g;
	g.add_vertex(1);
	g.add_vertex(2);
	g.add_vertex(3);
	g.add_vertex(4);
	g.add_edge(2, 4, 6);
	g.add_edge(1, 4, 5);
	g.add_edge(4, 1, 8);
	g.add_edge(3, 2, 7);
	g.add_edge(1, 2, 8);
	g.add_edge(3, 4, 8);
	g.print();
	g.width_search();
	return 0;
}




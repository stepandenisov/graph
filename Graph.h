#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

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

	struct color_vertex {
		unsigned color = 0;
		vertex current;
		TVertex previous;
		unsigned step;
	};

	void width_search()
	{
		vector<color_vertex> U;
		for (unsigned i = 0; i < table.size(); i++)
		{
			color_vertex tmp;
			tmp.current = table.at(i);
			tmp.previous = table.at(i).source_vertex;
			tmp.step = NAN;
			U.push_back(tmp);
		}
		for (unsigned i = 0; i < U.size(); i++)
		{
			if (U.at(i).color == 0) { ws(U.at(i), U); }
		}		
	}

	void ws(color_vertex& s, vector<color_vertex>& U)
	{
		queue<color_vertex> Q;
		s.color = 1;
		s.step = 0;
		Q.push(s);
		while (!Q.empty())
		{
			color_vertex a = Q.front();
			Q.pop();
			//cout << endl << "from ";
			//cout << a.cur.source_vertex;
			//cout << " to ";
			for (unsigned i = 0; i < a.current.list_of_destinations.size(); i++)
			{
				unsigned id = index(a.current.list_of_destinations.at(i).destination_vertex);
				//cout << U.at(id).cur.source_vertex << " and ";
				if (U.at(id).color == 0)
				{
					U.at(id).color = 1;
					U.at(id).step = a.step + 1;
					U.at(id).previous = a.current.source_vertex;
					Q.push(U.at(id));
				}
			}
			//cout << endl;
			U.at(index(a.current.source_vertex)).color = 2;
		}
	}

	struct Way {
		vector<TVertex> way;
		TEdge length;
	};

	void Dijkstra(TVertex source, TVertex dest)
	{
		unsigned st = index(source);
		unsigned dt = index(dest);
		unsigned n = table.size();
		vector<vector<TEdge>> w(table.size());
		for (unsigned i = 0; i < table.size(); i++)
		{
			for (unsigned j = 0; j < table.size(); j++)
			{
				w[i].push_back(INT_MAX);
			}
		}
		for (unsigned i = 0; i < table.size(); i++)
		{
			for (unsigned j = 0; j < table.at(i).list_of_destinations.size(); j++)
			{
				unsigned dest_index = index(table.at(i).list_of_destinations.at(j).destination_vertex);
				w[i][dest_index] = table.at(i).list_of_destinations.at(j).edge;
			}
		}
		vector<vector<TVertex>> res;
		res.resize(table.size());
		unsigned f = 0;
		for (int i = 0; i < n; i++)
		{
			res[i].resize(table.size());
			res[i][0] = (table.at(st).source_vertex);
		}
		vector<bool> visited(table.size());
		vector<TEdge> D(table.size());
		for (int i = 0; i < n; i++)
		{
			D[i] = w[st][i];
			visited[i] = false;
		}
		D[st] = 0;
		int index = 0, u = 0;
		for (int i = 0; i < n; i++)
		{
			int min = INT_MAX;
			for (int j = 0; j < n; j++)
			{
				if (!visited[j] && D[j] < min)
				{
					min = D[j];
					index = j;
				}
			}
			u = index;
			visited[u] = true;
			for (int j = 0; j < n; j++)
			{
				if (!visited[j] && w[u][j] != INT_MAX && D[u] != INT_MAX && (D[u] + w[u][j] < D[j]))
				{
					res[j] = res[u];
					res[j].push_back((table.at(u).source_vertex));
					f++;
					//res[j].push_back((table.at(j).source_vertex));
					D[j] = D[u] + w[u][j];
				}
			}
			//f = 1;
		}
		//cout << "Стоимость пути из начальной вершины до остальных(Алгоритм Дейкстры):\t\n";
		for (int i = 0; i < n; i++)
		{
			if (D[i] != INT_MAX)
				cout << source << " -> " << table.at(i).source_vertex << " = " << D[i] << endl;
			else
				cout << source << " -> " << table.at(i).source_vertex << " = " << "error" << endl;
		}
		cout << endl;
		for (unsigned i = 0; i < res.size(); i++)
		{
			for (unsigned j = 0; j < res[i].size(); j++)
				cout << res[i][j] << "->";
			cout << endl;
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
	g.add_vertex(5);
	g.add_edge(1, 2, 1);
	g.add_edge(2, 3, 2);
	g.add_edge(3, 4, 2);
	g.add_edge(4, 5, 2);
	g.add_edge(4, 1, 5);
	g.add_edge(3, 2, 7);
	g.add_edge(1, 2, 8);
	g.add_edge(3, 4, 8);
	g.add_edge(4, 5, 9);
	g.print();
	g.width_search();
	g.Dijkstra(1, 5);
	return 0;
}




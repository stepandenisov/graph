#include "Graph.h"

struct City {
private:
	int count_of_citizens;
	string name;

public:
	City(const int c = 0, const string n= "") :count_of_citizens(c), name(n) {}
	City(const City& c)
	{
		this->count_of_citizens = c.count_of_citizens;
		this->name = c.name;
	}
	friend bool operator==(const City& lhs, const City& rhs)
	{
		return (lhs.name == rhs.name);
	}
	friend bool operator!=(const City& lhs, const City& rhs)
	{
		return !operator==(lhs, rhs);
	}
	friend std::ostream& operator<<(std::ostream& out, City& c)
	{
		cout << c.name;
		return out;
	}
};

struct Road {
private:
	bool federal;
	bool toll;
	int length;
public:
	Road(bool f=false, bool t=false, int l=0): federal(f), toll(t), length(l) {}
	operator double()
	{
		return length;
	}
	Road operator=(const int& v)
	{
		this->federal = false;
		this->toll = false;
		this->length = v;
		return *this;
	}
	Road (const Road& v)
	{
		this->federal = v.federal;
		this->toll = v.toll;
		this->length = v.length;
	}
	friend Road operator+(const Road& lhs, const Road &rhs)
	{
		return Road(lhs.federal || rhs.federal, lhs.toll || rhs.toll, lhs.length + rhs.length);

	}
	friend std::ostream& operator<<(std::ostream& out, Road& r)
	{
		cout << r.length << " (is toll: " << r.toll << ", is federal: " << r.federal << ")";
		return out;
	}
};

int main()
{
	Graph<City, Road> g;
	City Samara(1000000, "Samara"), Togliatti(7000000, "Togliatti"), Zhigulevsk(40000, "Zhigulevsk"), Syzran(100000, "Syzran"), Kurumoch(700, "Kurumoch"), Moscow(10000000, "Moscow");
	Road SaTo(true, false, 70), ToZh(true, false, 15), ToSy(false, false, 100), 
		ToKu(true, false, 30), KuSa(true, false, 30), ZhSy(false, false, 80), 
		SaMo(true, false, 1000), ToMo(true, false, 900), SyMo(false, false, 800);
	g.add_vertex(Samara);
	g.add_vertex(Togliatti);
	g.add_vertex(Zhigulevsk);
	g.add_vertex(Syzran);
	g.add_vertex(Kurumoch);
	g.add_vertex(Moscow);
	g.add_edge(Samara, Togliatti, SaTo);
	g.add_edge(Togliatti, Zhigulevsk, ToZh);
	g.add_edge(Togliatti, Syzran, ToSy);
	g.add_edge(Togliatti, Kurumoch, ToKu);
	g.add_edge(Kurumoch, Samara, KuSa);
	g.add_edge(Togliatti, Samara, SaTo);
	g.add_edge(Zhigulevsk, Syzran, ZhSy);
	g.add_edge(Samara, Moscow, SaMo);
	g.add_edge(Togliatti, Moscow, ToMo);
	g.add_edge(Syzran, Moscow, SyMo);
	g.print();
	g.dijkstra(Togliatti, Samara);
	g.dijkstra(Togliatti, Syzran);
	g.dijkstra(Samara, Syzran);
	//g.del_vertex(Zhigulevsk);
	//g.del_edge(Togliatti, Moscow);
	//g.del_edge(Togliatti, Syzran);
	//g.print();
	g.dijkstra(Samara, Moscow);
	return 0;
}
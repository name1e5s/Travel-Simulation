#include <city_graph.h>
#include <fstream>
#include <iostream>
#include <sstream>

CityGraph::CityGraph() {
  this->init();
  this->print_edge();
}

CityGraph::~CityGraph() {
  for (int i = 0; i < city_num; i++) {
    if (city[i].first_transport) {
      Transport *tmp = nullptr;

      while (city[i].first_transport) {
        tmp = city[i].first_transport;
        city[i].first_transport = tmp->next_transport;
        delete tmp;
        tmp = nullptr;
      }
    }
  }
}

// Find (and insert) cities by name
int CityGraph::find_city(string name) {
  auto res = city_index.find(name);

  if (res != city_index.end())
    return res->second;
  else {
    city_index.insert(std::make_pair(name, city_num));
    index_city.insert(std::make_pair(city_num, name));
    city_num++;
    return city_num - 1;
  }
}

void CityGraph::add_edge(string name, transport_t type, int source, int dest,
                         int start, int duration, int price) {
  auto edge = new Transport(name, type, source, dest, start, duration, price);

  // Find the cheapest route bewteen two cities.
  if (cheapest_price[source][dest] > price) {
    cheapest_price[source][dest] = price;

    cheapest_route[source][dest].clear();
    cheapest_route[source][dest].push_back(*edge);
  }

  // Add edge to the graph
  if (!city[source].first_transport) {
    city[source].first_transport = edge;
    std::cout << "Added " << edge->tran_name;
  } else {
    edge->next_transport = city[source].first_transport;
    city[source].first_transport = edge;
  }
}

// Print the whole graph.
// For debug.
void CityGraph::print_edge() {
  for (auto &i : city) {
    if (i.first_transport) {
      Transport *t = i.first_transport;
      while (t) {
        std::cout << t->tran_name << " " << t->tran_type << " "
                  << index_city[t->source_city] << index_city[t->dest_city]
                  << std::endl;
        t = t->next_transport;
      }
    }
  }
}

// Read the data from file.
// Data format: <start time(by hours)> <duration(by hours)> <price><end
// of line> For example:
//  D21 0 Beijing Jilin 7 8 286
// The cities appear in the file will be convert to integer automatically.
// Converting between json and the format described above is handled by a
// python script.
void CityGraph::init(const char *path) {
  std::ifstream input(path);
  city_num = 0;

  for (int i = 0; i < MAX_VERT; i++) {
    for (int j = 0; j < MAX_VERT; j++) {
      if (i != j) {
        cheapest_price[i][j] = INFTY;
      } else {
        cheapest_price[i][j] = 0;
      }
      cheapest_route[i][j].resize(0); // CLear all
    }
    city[i].first_transport = nullptr;
  }

  string getline;
  while (std::getline(input, getline)) {
    std::stringstream word(getline);
    string edge_name;
    int tran_type;
    string begin_city, dest_city;
    int start_time, duration;
    int price;
    word >> edge_name >> tran_type >> begin_city >> dest_city >> start_time >>
        duration >> price;
    int b_city, e_city;
    b_city = find_city(begin_city);
    e_city = find_city(dest_city);
    transport_t tp;
    switch (tran_type) {
    case 0:
      tp = TYPE_TRAIN;
      break;
    case 1:
      tp = TYPE_FLIGHT;
      break;
    case 2:
      tp = TYPE_BUS;
      break;
    default:
      tp = TYPE_TRAIN;
      break;
    }
    add_edge(edge_name, tp, b_city, e_city, start_time, duration, price);
  }
}

// Pre-process the given data and compute the cheapest price using
// the floyd algorighm.
void CityGraph::floyd() {
  for (int k = 0; k < city_num; k++)
    for (int i = 0; i < city_num; i++)
      for (int j = 0; j < city_num; j++)
        if (cheapest_price[i][j] >
            cheapest_price[i][k] + cheapest_price[k][j]) {
          cheapest_price[i][j] = cheapest_price[i][k] + cheapest_price[k][j];
          cheapest_route[i][j].clear();
          cheapest_route[i][j].insert(cheapest_route[i][j].begin(),
                                      cheapest_route[k][j].begin(),
                                      cheapest_route[k][j].end());
          cheapest_route[i][j].insert(cheapest_route[i][j].begin(),
                                      cheapest_route[i][k].begin(),
                                      cheapest_route[i][k].end());
        }
}
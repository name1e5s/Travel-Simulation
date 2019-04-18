#include <city_graph.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>

using std::deque;

CityGraph::CityGraph() {
  this->init();
  this->print_edge();
  this->floyd();
  this->spfa();
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
int CityGraph::find_city(const string &name) {
  auto res = city_index.find(name);

  if (res != city_index.end())
    return res->second;
  else {
    city_index.insert(std::make_pair(name, city_num));
    index_city.insert(std::make_pair(city_num, name));
    city[city_num].city_name = name;
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

// Compute total time of a route.
int CityGraph::compute_time(const vector<Transport> &trans, int start_time) {
  int temp = start_time;
  for (const auto &x : trans)
    temp = pick_tran(temp, &(x));
  return temp - start_time;
}

// Returns cheapest price of the given middle city sequence.
int CityGraph::get_cheapest_price(const vector<int> &city_seq,
                                  const Traveller &t) const {
  if (t.middle_city_index.empty()) {
    return cheapest_price[t.source_city_index][t.dest_city_index];
  } else {
    int total_price = 0;
    for (int i = 0; i < city_seq.size() - 1; i++) {
      total_price += cheapest_price[city_seq[i]][city_seq[i + 1]];
    }
  }
}

// Returns fastest time of the given middle city sequence.
int CityGraph::get_fastest_price(const vector<int> &city_seq,
                                 const Traveller &t,
                                 const int begin_time) const {
  int temp, i;

  if (t.middle_city_index.empty()) {
    return begin_time + (fastest_time[t.source_city_index][t.dest_city_index]
                                     [begin_time % 24] -
                         begin_time % 24);
  } else {
    temp = begin_time +
           (fastest_time[t.source_city_index][city_seq[0]][begin_time % 24] -
            (begin_time % 24));

    for (i = 0; i < city_seq.size() - 1; i++)
      temp +=
          fastest_time[city_seq[i]][city_seq[i + 1]][temp % 24] - (temp % 24);
    return temp + (fastest_time[city_seq[i]][t.dest_city_index][temp % 24] -
                   (temp % 24));
  }
}

// Returns end time when we pick the Transport EDGE at
// begin_time. End time may be larger than 24. The function should be const.
int CityGraph::pick_tran(int begin_time, const Transport *edge) const {
  if (begin_time % 24 <= edge->start_time) {
    return begin_time + (edge->start_time - begin_time % 24) + edge->duration;
  } else { // Go next day
    return begin_time + (edge->start_time - begin_time % 24) + edge->duration +
           24;
  }
}

//////////////////////////////////////////////////////////////////
///                    The Algorithms                          ///
/////////////////////////////////////////////////////////////////

// Pre-process the given data and compute the cheapest price using
// the floyd algorithm.
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

// Pre-process to find the most fast route between ANY two
// cities.
void CityGraph::spfa() {
  for (int i = 0; i < city_num; ++i)
    for (int time = 0; time < 24; ++time) {
      deque<int> deque;
      bool used[MAX_VERT] = {false};

      deque.push_back(i);
      used[i] = true;

      while (!deque.empty()) {
        int node = deque.front();
        Transport *temp = city[node].first_transport;

        while (temp) {
          if (fastest_time[i][temp->dest_city][time] >
              pick_tran(fastest_time[i][node][time], temp)) {
            fastest_time[i][temp->dest_city][time] =
                pick_tran(fastest_time[i][node][time], temp);

            fastest_route[i][temp->dest_city][time].clear();
            fastest_route[i][temp->dest_city][time].insert(
                fastest_route[i][temp->dest_city][time].begin(),
                fastest_route[i][node][time].begin(),
                fastest_route[i][node][time].end());
            fastest_route[i][temp->dest_city][time].push_back(*temp);

            if (!used[temp->dest_city]) {
              used[temp->dest_city] = true;
              deque.push_back(temp->dest_city);
            }
          }
          temp = temp->next_transport;
        }
        used[node] = false;
        deque.pop_front();
      }
    }
}
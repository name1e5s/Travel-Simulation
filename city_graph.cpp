#include <QDebug>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <city_graph.h>
#include <map>
#include <random>

using std::deque;
using std::shuffle;

#define HIGHTEMP 11451400
#define MIDTEMP 7
#define LOWTEMP (1.00001)

/**
 * @brief Construct a new City Graph:: City Graph object.
 * 
 * In the constructor, the config file is to be read, the
 * city index it updated, and the pre-process function is
 * called.
 */
CityGraph::CityGraph() {
  this->init_cities();
  qDebug() << "Init cities done.";
  this->init();
  qDebug() << "Init cities done.";
  this->floyd();
  qDebug() << "Init cities done.";
  this->spfa();
  qDebug() << "Init cities done.";
}

/**
 * @brief Destroy the City Graph:: City Graph object.
 * 
 * In the destructor, all edges is freed.
 */
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

/**
 * @brief Find (and insert) cities by name.
 * 
 * @param name City name
 * @return int The index of the city
 */
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

/**
 * @brief Add a new edge to the graph.
 * 
 * @param name Transport name
 * @param type Type of the transport, e.g. TYPE_TRAIN, TYPE_FLIGHT
 * @param source Source city index
 * @param dest Destnation city index
 * @param start Start time
 * @param duration Duration of the transport
 * @param price Price of the transport
 */
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
  } else {
    edge->next_transport = city[source].first_transport;
    city[source].first_transport = edge;
  }
}

/**
 * @brief Update city index.
 * 
 */
void CityGraph::init_cities() {
  city_num = 0;
  this->find_city("BJ");
  this->find_city("TJ");
  this->find_city("SH");
  this->find_city("CQ");
  this->find_city("SJZ");
  this->find_city("TY");
  this->find_city("ZZ");
  this->find_city("CS");
  this->find_city("WH");
  this->find_city("HEB");
  this->find_city("CC");
  this->find_city("SY");
  this->find_city("CD");
  this->find_city("KM");
  this->find_city("GY");
  this->find_city("XA");
  this->find_city("LZ");
  this->find_city("GZ");
  this->find_city("NN");
  this->find_city("NJ");
  this->find_city("HZ");
  this->find_city("FZ");
  this->find_city("JN");
  this->find_city("NC");
  this->find_city("HF");
  this->find_city("HHHT");
  this->find_city("LS");
  this->find_city("WLMQ");
  this->find_city("YC");
  this->find_city("XN");
  this->find_city("HK");
}

/**
 * @brief Initialize the graph.
 * 
 * Read the data from file.
 * Data format: start time(by hours) duration(by hours) price end
 * of line For example:
 *  D21 0 Beijing Jilin 7 8 286
 * The cities appear in the file will be convert to integer automatically.
 * Converting between json and the format described above is handled by a
 * python script.
 * 
 * @param path File path
 */
void CityGraph::init(const char *path) {
  std::ifstream input(path);

  for (int i = 0; i < MAX_VERT; i++) {
    for (int j = 0; j < MAX_VERT; j++) {
      if (i != j) {
        cheapest_price[i][j] = INFTY;
        for (int k = 0; k < 24; k++)
          fastest_time[i][j][k] = INFTY;
      } else {
        cheapest_price[i][j] = 0;
        for (int k = 0; k < 24; k++)
          fastest_time[i][j][k] = k;
      }
      cheapest_route[i][j].resize(0); // Clear all
      for (int k = 0; k < 24; k++)
        fastest_route[i][j][k].resize(0); // Clear all
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
    default:
      tp = TYPE_TRAIN;
      break;
    }
    add_edge(edge_name, tp, b_city, e_city, start_time, duration, price);
  }
}

/**
 * @brief Compute time for a given plan.
 * 
 * @param plan Plan sequence
 * @param begin_time Start time of the plan
 * @return int Total time
 */
int CityGraph::compute_time(const vector<Transport> &plan, int begin_time) {
  int t = begin_time;
  for (const auto &i : plan)
    t = pick_tran(t, &i);
  return t - begin_time;
}

/**
 * @brief Compute price of a given path.
 * 
 * @param plan Plan sequence
 * @return int Total price
 */
int CityGraph::compute_price(const vector<Transport> &plan) {
  int t = 0;
  for (const auto &i : plan)
    t += i.price;
  return t;
}

/**
 * @brief Compute the fastest time of the given middle city sequence.
 * 
 * @param city_seq The middle city sequence, often provided by the SA algorithm
 * @param t The traveller
 * @param begin_time Start time of the plan
 * @return int The fastest time of the given sequence.
 */
int CityGraph::compute_time(const vector<int> &city_seq, const Traveller &t,
                            const int begin_time) const {
  int temp;
  uint i;
  if (t.middle_city_index.empty()) {
    return (
        fastest_time[t.source_city_index][t.dest_city_index][begin_time % 24] -
        begin_time % 24);
  } else {
    temp = begin_time +
           (fastest_time[t.source_city_index][city_seq[0]][begin_time % 24] -
            (begin_time % 24));

    for (i = 0; i < city_seq.size() - 1; i++)
      temp +=
          fastest_time[city_seq[i]][city_seq[i + 1]][temp % 24] - (temp % 24);
    return temp +
           (fastest_time[city_seq[i]][t.dest_city_index][temp % 24] -
            (temp % 24)) -
           begin_time;
  }
}


/**
 * @brief Compute the cheapest price of the given middle city sequence.
 * 
 * @param city_seq The middle city sequence, often provided by the SA algorithm
 * @param t The traveller
 * @return int The cheapest price of the given middle city sequence
 */
int CityGraph::compute_price(const vector<int> &city_seq,
                             const Traveller &t) const {
  if (t.middle_city_index.empty()) {
    return cheapest_price[t.source_city_index][t.dest_city_index];
  } else {
    int total_price = 0;
    for (uint i = 0; i < city_seq.size() - 1; i++) {
      total_price += cheapest_price[city_seq[i]][city_seq[i + 1]];
    }
    return total_price;
  }
}

/**
 * @brief Guess the price of the given middle city sequence.
 * 
 * @param t The traveller
 * @return int The price.
 */
int CityGraph::compute_expected_price(const Traveller &t) {
  int guess_price_1 = compute_price(simulated_annealing(LOWTEMP, t), t);

  int guess_price_2 = compute_price(t.middle_city_index, t);

  if (guess_price_1 > guess_price_2)
    std::swap(guess_price_1, guess_price_2);

  return (guess_price_2 * 2) - guess_price_1;
}

/**
 * @brief Guess the time used of the given middle city sequence.
 * 
 * @param t The traveller
 * @param begin_time The start time of the plan
 * @return int The total time
 */
int CityGraph::compute_expected_time(Traveller &t, int begin_time) {
  auto guess_time_1 = compute_time(t.middle_city_index, t, begin_time);
  auto tmp_sa = simulated_annealing(LOWTEMP, begin_time, t);
  auto tmp = get_fastest_route(t, tmp_sa, begin_time);
  auto guess_time_2 = compute_time(tmp_sa, t, begin_time);

  if (guess_time_1 > guess_time_2)
    std::swap(guess_time_1, guess_time_2);

  return ((guess_time_2 * 2) - guess_time_1) * 2;
}

/**
 * @brief Returns end time when we pick the Transport EDGE at begin_time.
 * 
 * @param begin_time Start time of the plan
 * @param edge The selected edge
 * @return int End time, may be larger than 24. The function should be const
 */
int CityGraph::pick_tran(int begin_time, const Transport *edge) const {
  if (begin_time % 24 <= edge->start_time) {
    return begin_time + (edge->start_time - begin_time % 24) + edge->duration;
  } else { // Go next day
    return begin_time + (edge->start_time - begin_time % 24) + edge->duration +
           24;
  }
}

/**
 * @brief Returns the cheapest route.
 * 
 * @param t The traveller
 * @param sa_result The middle city sequence, provided by SA algorithm
 * @return vector<Transport> The route.
 */
vector<Transport> CityGraph::get_cheapest_route(Traveller &t,
                                                const vector<int> &sa_result) {
  vector<Transport> result;
  uint i;

  if (t.middle_city_index.size() == 0)
    result.assign(
        cheapest_route[t.source_city_index][t.dest_city_index].begin(),
        cheapest_route[t.source_city_index][t.dest_city_index].end());

  else {
    result.insert(result.begin(),
                  cheapest_route[t.source_city_index][sa_result[0]].begin(),
                  cheapest_route[t.source_city_index][sa_result[0]].end());

    for (i = 0; i < sa_result.size() - 1; i++)
      result.insert(result.end(),
                    cheapest_route[sa_result[i]][sa_result[i + 1]].begin(),
                    cheapest_route[sa_result[i]][sa_result[i + 1]].end());

    result.insert(result.end(),
                  cheapest_route[sa_result[i]][t.dest_city_index].begin(),
                  cheapest_route[sa_result[i]][t.dest_city_index].end());
  }

  return result;
}

/**
 * @brief Returns the cheapest route.
 * 
 * @param t The traveller
 * @return vector<Transport> The route
 */
vector<Transport> CityGraph::get_cheapest_route(Traveller &t) {
  return get_cheapest_route(t, simulated_annealing(HIGHTEMP, t));
}

/**
 * @brief Returns the fastest route at given begin_time.
 * 
 * @param t The traveller
 * @param sa_result The middle city sequence, provided by SA algorithm
 * @param leave_time The start time of the plan
 * @return vector<Transport> The result route
 */
vector<Transport> CityGraph::get_fastest_route(Traveller &t,
                                               const vector<int> &sa_result,
                                               int leave_time) {
  uint i;
  vector<Transport> result;
  int temp = leave_time;

  if (sa_result.empty()) {
    result.assign(
        fastest_route[t.source_city_index][t.dest_city_index][leave_time]
            .begin(),
        fastest_route[t.source_city_index][t.dest_city_index][leave_time]
            .end());
  }

  else {
    result.insert(
        result.begin(),
        fastest_route[t.source_city_index][sa_result[0]][temp].begin(),
        fastest_route[t.source_city_index][sa_result[0]][temp].end());
    temp = fastest_time[t.source_city_index][sa_result[0]][temp] % 24;

    for (i = 0; i < sa_result.size() - 1; i++) {
      result.insert(result.end(),
                    fastest_route[sa_result[i]][sa_result[i + 1]][temp].begin(),
                    fastest_route[sa_result[i]][sa_result[i + 1]][temp].end());
      temp = fastest_time[sa_result[i]][sa_result[i + 1]][temp] % 24;
    }

    result.insert(result.end(),
                  fastest_route[sa_result[i]][t.dest_city_index][temp].begin(),
                  fastest_route[sa_result[i]][t.dest_city_index][temp].end());
  }

  return result;
}

/**
 * @brief Returns the fastest route.
 * 
 * @param t The traveller
 * @return vector<Transport> The route
 */
vector<Transport> CityGraph::get_fastest_route(Traveller &t) {
  vector<Transport> result;
  int best_time = 0x3f3f3f3f;
  for (int i = 0; i < 24; i++) {
    auto tmp_sa = simulated_annealing(HIGHTEMP, i, t);
    auto tmp = get_fastest_route(t, tmp_sa, i);
    auto tmp_time = compute_time(tmp_sa, t, i);
    if (tmp_time < best_time) {
      t.leave_time = i;
      result = tmp;
      best_time = tmp_time;
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////
///                    The Algorithms                          ///
/////////////////////////////////////////////////////////////////

/**
 * @brief Pre-process the given data and compute the cheapest price using
 * the floyd algorithm.
 * 
 */
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

/**
 * @brief Pre-process to find the most fast route between ANY two
 * cities.
 * 
 */
void CityGraph::spfa() {
  for (int i = 0; i < city_num; ++i)
    for (int time = 0; time < 24; ++time) {
      deque<int> deque;
      bool used[MAX_VERT] = {};

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

/**
 * @brief Accept rate function for SA algorithm.
 * 
 * @param new_energy 
 * @param old_energy 
 * @param temperature 
 * @return double The accept rate
 */
static inline double accept_rate(const int &new_energy, const int &old_energy,
                                 const double &temperature) {
  return old_energy > new_energy ? exp((old_energy - new_energy) / temperature)
                                 : 1.0f;
}

vector<int> CityGraph::simulated_annealing(const double initial_temperature,
                                           const Traveller &t) {
  int new_energy = 0;
  int old_energy = 0;
  double temperature = initial_temperature;
  int min_price;
  vector<int> temp_plan;

  for (auto x : t.middle_city_index)
    temp_plan.push_back(x);

  vector<int> best_plan(temp_plan);
  std::random_device rd;
  std::mt19937 g(rd());
  shuffle(temp_plan.begin(), temp_plan.end(), g);

  if (temp_plan.empty())
    return best_plan;
  else {
    min_price = compute_price(temp_plan, t);
    old_energy = min_price;
    while (temperature > 1 && temp_plan.size() != 1) {
      vector<int> new_temp = temp_plan;

      int pos1 = g() % temp_plan.size();
      int pos2 = g() % temp_plan.size();
      while (pos1 == pos2)
        pos2 = g() % temp_plan.size();

      std::swap(new_temp[pos1], new_temp[pos2]);
      new_energy = compute_price(temp_plan, t);
      if (accept_rate(new_energy, old_energy, temperature) >
          (double)g() / g.max()) {
        temp_plan.assign(new_temp.begin(), new_temp.end());
        old_energy = new_energy;
      }

      if (old_energy < min_price) {
        min_price = old_energy;
        best_plan.assign(temp_plan.begin(), temp_plan.end());
      }
      temperature *= 1 - 0.004114514;
    }
    return best_plan;
  }
}

/**
 * @brief The simulated annealing algorithm for finding the 
 * fastest middle city sequence.
 * 
 * @param initial_temperature 
 * @param leave_time 
 * @param t The traveller
 * @return vector<int> The new middle city sequence
 */
vector<int> CityGraph::simulated_annealing(const double initial_temperature,
                                           const int leave_time,
                                           const Traveller &t) {
  int new_energy = 0;
  int old_energy = 0;
  double temperature = initial_temperature;
  int min_time;
  vector<int> temp_plan;

  for (auto x : t.middle_city_index)
    temp_plan.push_back(x);

  vector<int> best_plan(temp_plan);
  std::random_device rd;
  std::mt19937 g(rd());
  shuffle(temp_plan.begin(), temp_plan.end(), g);

  if (temp_plan.empty())
    return best_plan;
  else {
    min_time = compute_time(temp_plan, t, leave_time);
    old_energy = min_time;
    while (temperature > 1 && temp_plan.size() != 1) {
      vector<int> new_temp = temp_plan;

      int pos1 = g() % temp_plan.size();
      int pos2 = g() % temp_plan.size();
      while (pos1 == pos2)
        pos2 = g() % temp_plan.size();

      std::swap(new_temp[pos1], new_temp[pos2]);
      new_energy = compute_time(temp_plan, t, leave_time);
      if (accept_rate(new_energy, old_energy, temperature) >
          (double)g() / g.max()) {
        temp_plan.assign(new_temp.begin(), new_temp.end());
        old_energy = new_energy;
      }

      if (old_energy < min_time) {
        min_time = old_energy;
        best_plan.assign(temp_plan.begin(), temp_plan.end());
      }
      temperature *= 1 - 0.004114514;
    }
    return best_plan;
  }
}

/**
 * @brief The bounded DFS algorithm for the third strategy.
 * 
 * @param t The traveller
 * @param money Current money
 * @param begin_time The leave time of the plan
 * @param time Current time
 * @param depth Depth limit, if the depth is 0, then this route is deprecated
 */
void CityGraph::dfs(Traveller &t, int money, int begin_time, int time,
                    int depth) {
  if ((t.current_city_index == t.dest_city_index) &&
      (t.middle_city_index.empty())) {
    if ((min_cost > money) && (time <= t.time_limit)) {
      min_cost = money;
      best_route = curr_route;
      return;
    } else {
      return;
    }
  }

  if (depth == 0 || time + compute_expected_time(t, time) > t.time_limit ||
      money + compute_expected_price(t) > min_cost) {
    return;
  }

  int now = t.current_city_index;
  Transport *temp = city[t.current_city_index].first_transport;

  while (temp != nullptr) {
    t.current_city_index = temp->dest_city;
    curr_route.push_back(*temp);

    int city_idx = t.get_middle_index(t.current_city_index);
    if (city_idx >= 0) {
      t.middle_city_index.erase(t.middle_city_index.begin() + city_idx);
      dfs(t, money + temp->price, begin_time,
          pick_tran(time + begin_time, temp) - begin_time, depth - 1);
      t.current_city_index = now;
      t.middle_city_index.insert(t.middle_city_index.begin() + city_idx,
                                 temp->dest_city);
    } else
      dfs(t, money + temp->price, begin_time,
          pick_tran(time + begin_time, temp) - begin_time, depth - 1);

    t.current_city_index = now;
    curr_route.pop_back();
    temp = temp->next_transport;
  }
}

/**
 * @brief The entry point of the algorithm module.
 * 
 * This function reads user input from the traveller class
 * and the generate result.
 *
 * @param t The traveller
 */
void CityGraph::get_route(Traveller &t) {
  if (t.plan_type == TYPE_CHEAP)
    t.plan_result = get_cheapest_route(t);
  else if (t.plan_type == TYPE_FAST)
    t.plan_result = get_fastest_route(t);
  else {
    best_route.clear();
    auto temp = get_cheapest_route(t);
    if (compute_time(temp, t.leave_time) < t.time_limit)
      t.plan_result = temp;
    else {
      auto sa = simulated_annealing(MIDTEMP, t);
      if (compute_time(sa, t, t.leave_time) - t.leave_time <= t.time_limit) {
        best_route = get_fastest_route(t, sa, t.leave_time);
        min_cost = 0;
        for (auto &i : best_route)
          min_cost += i.price;
        dfs(t, 0, t.leave_time, 0, 5);
      }
      t.plan_result = best_route;
      auto fastest_result = get_fastest_route(
          t, simulated_annealing(HIGHTEMP, t.leave_time, t), t.leave_time);
      if (t.plan_result.empty() ||
          compute_time(t.plan_result, t.leave_time) > t.time_limit ||
          compute_price(fastest_result) < compute_price(t.plan_result)) {
        t.plan_result = fastest_result;
      }
    }
  }
}

﻿#ifndef CITY_GRAPH
#define CITY_GRAPH
// Basic definitions
#define MAX_VERT 50
#define INFTY 0x3f3f3f3f

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

/**
 * @brief Define the type of the trips.
 * 
 */
typedef enum { TYPE_TRAIN, TYPE_FLIGHT } transport_t;

/**
 * @brief Define users' requests type.
 * 
 */
typedef enum { TYPE_CHEAP, TYPE_FAST, TYPE_CHEAP_LIMITED } planning_t;


/**
 * @brief Record the basic information of a public transport.
 * Usually used as an edge of the adjacent list in out graph.
 * 
 */
struct Transport {
  Transport() {
    tran_name = "";
    tran_type = TYPE_TRAIN;
    source_city = 0;
    dest_city = 0;
    start_time = 0;
    duration = 0;
    price = 0;
    next_transport = nullptr;
  }

  Transport(const Transport &transport) {
    tran_name = transport.tran_name;
    tran_type = transport.tran_type;
    source_city = transport.source_city;
    dest_city = transport.dest_city;
    start_time = transport.start_time;
    duration = transport.duration;
    price = transport.price;
    next_transport = nullptr;
  }

  Transport(string name, transport_t type, int source, int dest, int start,
            int duration, int price) {
    this->tran_name = name;
    this->tran_type = type;
    this->source_city = source;
    this->dest_city = dest;
    this->start_time = start;
    this->duration = duration;
    this->price = price;
    this->next_transport = nullptr;
  }

  string tran_name;
  transport_t tran_type;
  int source_city, dest_city;
  // Note: in out system, we use hours.
  int start_time, duration;
  // All integer price, "xxx.5" will be "xxx".
  int price;

  Transport *next_transport;
};

/**
 * @brief Determine the result type
 * 
 */
typedef enum { ROUTE, CITY } result_t;

/**
 * @brief Record the result begin hour and basic info.
 * 
 */
struct ResultNode {
  int beginHour;
  result_t resultType;
  // For CITY
  int waitCityIndex;
  // For ROUTE
  Transport transport;
};

/**
 * @brief Result for log.
 * 
 */
struct PlanResult {
  int startHour;
  vector<ResultNode> result;
};

/**
 * @brief Record the city name and transports start from
 * this city. Usually as a vertex of the graph.
 * 
 */
struct City {
  string city_name;
  Transport *first_transport;
};

/**
 * @brief  Store user's request and our results, the
 * simulation is also handled by this struct.
 * Also some temp variable is stored here.
 * 
 */
struct Traveller {
  planning_t plan_type;
  int source_city_index, dest_city_index;
  int current_city_index;
  vector<int> middle_city_index;
  int time_limit;
  int leave_time; // For TYPE_CHEAP_LIMITED
  vector<Transport> plan_result;
  int time_all, price_all;
  // void run_simulation();
  int get_middle_index(int city_idx) {
    for (uint32_t i = 0; i < middle_city_index.size(); ++i)
      if (middle_city_index[i] == city_idx)
        return i;
    return -1;
  }
};

/**
 * @brief The City Graph. Records ALL the useful data.
 * 
 */
class CityGraph {
public:
  CityGraph();
  ~CityGraph();
  void get_route(Traveller &);
  int compute_price(const vector<Transport> &);
  int compute_time(const vector<Transport> &, int);
  map<int, string> index_city;
  map<string, int> city_index;

private:
  void init();
  void init_cities();
  void add_edge(string, transport_t, int, int, int, int, int);
  int find_city(const string &);
  int compute_price(const vector<int> &, const Traveller &) const;
  int compute_time(const vector<int> &, const Traveller &t, int) const;
  int compute_expected_price(const Traveller &t);
  int compute_expected_time(Traveller &t, int begin_time);
  int pick_tran(int, const Transport *) const;
  void floyd();
  void spfa();

  vector<int> simulated_annealing(const double, const Traveller &);
  vector<int> simulated_annealing(const double, const int, const Traveller &);

  City city[MAX_VERT];
  int city_num;

  // To find the cheapest route.
  // In our program, the cheapest route between
  // ANY two cities will be pre-generated by FLOYD
  // algorithm. Obviously, the time complexity will be
  // $O(v^3)$($v$ for vertex).
  int cheapest_price[MAX_VERT][MAX_VERT];
  vector<Transport> cheapest_route[MAX_VERT][MAX_VERT];
  vector<Transport> get_cheapest_route(Traveller &, const vector<int> &);
  vector<Transport> get_cheapest_route(Traveller &);

  // To find the most fast route.
  // In our program, the most fast route bewteen ANY
  // two city will be pre-generated by SPFA algorithm,
  // which time complexity is $O(Ce)$($e$ for edge).
  // The end time fastest_time may be larger than 23,
  // because the journey may take more than one day.
  int fastest_time[MAX_VERT][MAX_VERT][24];
  vector<Transport> fastest_route[MAX_VERT][MAX_VERT][24];
  vector<Transport> get_fastest_route(Traveller &, const vector<int> &, int);
  vector<Transport> get_fastest_route(Traveller &);

  int min_cost;
  int best_route_time;
  vector<Transport> best_route, curr_route;
  void dfs(Traveller &, int, int, int, int);
};
#endif

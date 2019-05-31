#include <city_graph.h>
#include <iostream>
#include <vector>

int main() {
  CityGraph cityGraph;
  std::cout << "Hello, World!" << std::endl;
    Traveller t;
    t.plan_type = TYPE_CHEAP_LIMITED;
    t.leave_time = 12;
    t.source_city_index = 0;
    t.dest_city_index = 2;
    t.time_limit = 10;
    /*
    t.middle_city_index.push_back(5);
    t.middle_city_index.push_back(16);
    t.middle_city_index.push_back(28);
     */
    cityGraph.get_route(t);
    std::cout << cityGraph.compute_price(t.middle_city_index , t);
  return 0;
}
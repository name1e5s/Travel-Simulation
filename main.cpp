#include <city_graph.h>
#include <iostream>
#include <vector>

int main() {
  CityGraph cityGraph;
  std::cout << "Hello, World!" << std::endl;
    Traveller t;
    t.plan_type = TYPE_CHEAP_LIMITED;
    t.leave_time = 14;
    t.source_city_index = 0;
    t.dest_city_index = 28;
    t.time_limit = 9;
    t.middle_city_index.push_back(15);
    cityGraph.get_route(t);
    std::cout << cityGraph.compute_price(t.plan_result) << std::endl;
  return 0;
}

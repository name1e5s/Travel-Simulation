#include "graph_handler.h"
#include <QDebug>

GraphHandler::GraphHandler(QObject *parent) : QObject(parent), m_cityGraph() {
  m_traveller.plan_type = TYPE_CHEAP;
}

int GraphHandler::beginYear() const { return m_beginYear; }

void GraphHandler::setBeginYear(int beginYear) { m_beginYear = beginYear; }

int GraphHandler::beginMonth() const { return m_beginMonth; }

void GraphHandler::setBeginMonth(int beginMonth) { m_beginMonth = beginMonth; }

int GraphHandler::beginDate() const { return m_beginDate; }

void GraphHandler::setBeginDate(int beginDate) { m_beginDate = beginDate; }

QVector<int> GraphHandler::middleCity() const {
  return QVector<int>::fromStdVector(m_traveller.middle_city_index);
}
void GraphHandler::setMiddleCity(const QVector<int> &middleCity) {
  // DO NOT WRITE HERE
}

void GraphHandler::appendMiddleCity(int value) {
  m_traveller.middle_city_index.push_back(value);
}

void GraphHandler::clearMiddleCity() { m_traveller.middle_city_index.clear(); }

int GraphHandler::leaveTime() const { return m_traveller.leave_time; }

void GraphHandler::setLeaveTime(int leaveTime) {
  m_traveller.leave_time = leaveTime;
}

int GraphHandler::timeLimit() const { return m_traveller.time_limit; }

void GraphHandler::setTimeLimit(int timeLimit) {
  m_traveller.time_limit = timeLimit;
}

int GraphHandler::planType() const { return m_traveller.plan_type; }

void GraphHandler::setPlanType(int planType) {
  switch (planType) {
  case 0:
    m_traveller.plan_type = TYPE_CHEAP;
    break;
  case 1:
    m_traveller.plan_type = TYPE_FAST;
    break;
  default:
    m_traveller.plan_type = TYPE_CHEAP_LIMITED;
    break;
  }
}

QVector<int> GraphHandler::citySequence() const { return m_citySequence; }

void GraphHandler::setCitySequence(const QVector<int> &citySequence) {
  // DO NOT WRITE, PLEASE
}

int GraphHandler::sourceCity() const { return m_traveller.source_city_index; }

void GraphHandler::setSourceCity(int sourceCity) {
  m_traveller.source_city_index = sourceCity;
}

int GraphHandler::destCity() const { return m_traveller.dest_city_index; }

void GraphHandler::setDestCity(int destCity) {
  m_traveller.dest_city_index = destCity;
}

void GraphHandler::generateResult() {
  m_cityGraph.get_route(m_traveller);
  generateCitySequence();
  generateTotalPrice();
  generateTotalTime();
}

void GraphHandler::generateCitySequence() {
  m_citySequence.clear();
  m_tranName.clear();
  if (!m_traveller.plan_result.empty()) {
    m_citySequence.push_back(m_traveller.plan_result[0].source_city);
    for (auto x : m_traveller.plan_result) {
      m_citySequence.push_back(x.dest_city);
      m_tranName.push_back(QString::fromStdString(x.tran_name));
    }
  }
}

void GraphHandler::generateTotalPrice() {
  m_totalPrice = m_cityGraph.compute_price(m_traveller.plan_result);
  emit totalPriceChanged();
}

void GraphHandler::generateTotalTime() {
  switch (m_traveller.plan_type) {
  case TYPE_CHEAP:
    m_totalTime = m_cityGraph.compute_time(
        m_traveller.plan_result, m_traveller.plan_result[1].start_time);
    break;
  default:
    m_totalTime = m_cityGraph.compute_time(m_traveller.plan_result,
                                           m_traveller.leave_time);
    break;
  }
  emit totalTimeChanged();
}

int GraphHandler::totalPrice() const { return m_totalPrice; }

void GraphHandler::setTotalPrice(int totalPrice) { m_totalPrice = totalPrice; }

int GraphHandler::totalTime() const { return m_totalTime; }

void GraphHandler::setTotalTime(int totalTime) { m_totalTime = totalTime; }

QVector<QString> GraphHandler::tranName() const { return m_tranName; }

void GraphHandler::setTranName(const QVector<QString> &tranName) {
  m_tranName = tranName;
}

#include "graph_handler.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QFile>

static int currentTime;
static int log_i;
static QDate logDate;
static QVector<QString> logFile;

GraphHandler::GraphHandler(QObject *parent) : QObject(parent), m_cityGraph() {
  m_traveller.plan_type = TYPE_CHEAP;
  m_simulateTimer.setInterval(5000);
  m_simulateTimer.setSingleShot(false);
  QObject::connect(&m_simulateTimer, &QTimer::timeout, this,
                   &GraphHandler::printNewLog);
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
  qDebug() << "APPEND " << value;
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
    m_totalTime =
        m_traveller.plan_result.empty()
            ? 0
            : m_cityGraph.compute_time(m_traveller.plan_result,
                                       m_traveller.plan_result[0].start_time);
    break;
  default:
    m_totalTime = m_cityGraph.compute_time(m_traveller.plan_result,
                                           m_traveller.leave_time);
    break;
  }
  emit totalTimeChanged();
}

void GraphHandler::generatePlanResult() {
  m_planResult.result.clear();
  if (m_traveller.plan_result.empty())
    return;

  switch (m_traveller.plan_type) {
  case TYPE_CHEAP:
    m_planResult.startHour = m_traveller.plan_result[0].start_time;
    break;
  default:
    m_planResult.startHour = m_traveller.leave_time;
    break;
  }

  logFile.clear();
  logFile.push_back("================================");
  logFile.push_back(QDateTime::currentDateTime().toString(
      "     yyyy-MM-dd hh:mm:ss.zzz    "));
  logFile.push_back("(C) 2019 Travel-Simulation Group");
  logFile.push_back("================================");
  logFile.push_back("用户输入");
  logFile.push_back(
      QString("起始城市: %1")
          .arg(QString::fromStdString(
              m_cityGraph.index_city[m_traveller.source_city_index])));
  logFile.push_back(
      QString("目标城市: %1")
          .arg(QString::fromStdString(
              m_cityGraph.index_city[m_traveller.dest_city_index])));
  logFile.push_back(QString("策略类型: %1").arg(m_traveller.plan_type));
  logFile.push_back("必经城市:");
  for (auto x : m_traveller.middle_city_index) {
    logFile.push_back(QString("    %1").arg(
        QString::fromStdString(m_cityGraph.index_city[x])));
  }
  logFile.push_back("================================");
  logFile.push_back("结果概要");
  logFile.push_back(QString("开始时间: %1").arg(m_planResult.startHour));
  logFile.push_back(QString("耗时: %1").arg(m_totalTime));
  logFile.push_back(QString("价格: %1").arg(m_totalPrice));
  logFile.push_back(
      QString("路线数目: %1").arg(m_traveller.plan_result.size()));
  logFile.push_back("================================");
  logFile.push_back("详细日志");

  currentTime = m_planResult.startHour; // Initialize time.
  logDate = QDate(m_beginYear, m_beginMonth, m_beginDate);
  log_i = 0;
  // Wait at source city?
  if (m_planResult.startHour != m_traveller.plan_result[0].start_time) {
    ResultNode temp;
    temp.beginHour = m_planResult.startHour;
    temp.resultType = CITY;
    temp.waitCityIndex = m_traveller.source_city_index;
    m_planResult.result.push_back(temp);
  }

  ResultNode t;
  int tmpHour = m_traveller.plan_result[0].start_time - m_planResult.startHour;
  t.beginHour =
      m_planResult.startHour + (tmpHour >= 0 ? tmpHour : 24 + tmpHour);
  t.resultType = ROUTE;
  t.transport = m_traveller.plan_result[0];
  m_planResult.result.push_back(t);

  for (size_t i = 1; i < m_traveller.plan_result.size(); i++) {
    if ((m_planResult.result.back().beginHour +
         m_traveller.plan_result[i - 1].duration) %
            24 !=
        m_traveller.plan_result[i].start_time) {
      // Wait here.
      t.beginHour = (m_planResult.result.back().beginHour +
                     m_traveller.plan_result[i - 1].duration);
      t.resultType = CITY;
      t.waitCityIndex = m_planResult.result.back().transport.dest_city;
      m_planResult.result.push_back(t);
    }
    int tmpHour;
    if (m_planResult.result.back().resultType == CITY)
      tmpHour = m_traveller.plan_result[i].start_time -
                (m_planResult.result.back().beginHour % 24);
    else
      tmpHour = (m_traveller.plan_result[i].start_time) % 24 -
                (m_planResult.result.back().beginHour +
                 m_planResult.result.back().transport.duration) %
                    24;
    int h = (tmpHour >= 0 ? tmpHour : 24 + tmpHour);
    int b = m_planResult.result.back().beginHour;
    t.beginHour = b + h;
    if (m_planResult.result.back().resultType != CITY)
      t.beginHour += m_planResult.result.back().transport.duration;
    t.resultType = ROUTE;
    t.transport = m_traveller.plan_result[i];
    m_planResult.result.push_back(t);
  }
}

int GraphHandler::totalPrice() const { return m_totalPrice; }

void GraphHandler::setTotalPrice(int totalPrice) { m_totalPrice = totalPrice; }

int GraphHandler::totalTime() const { return m_totalTime; }

void GraphHandler::setTotalTime(int totalTime) { m_totalTime = totalTime; }

QVector<QString> GraphHandler::tranName() const { return m_tranName; }

void GraphHandler::setTranName(const QVector<QString> &tranName) {
  m_tranName = tranName;
}

static QString getTranName(transport_t tran) {
  if (tran == TYPE_TRAIN)
    return "火车";
  else
    return "航班";
}

void GraphHandler::printNewLog() {

  if (m_planResult.result.empty()) {
    m_simulateTimer.stop();
    emit simulationDone();
    return;
  }

  QDate realLogDate = logDate.addDays(currentTime / 24);
  QString routeString =
      realLogDate.toString("yyyy-MM-dd") + " " +
      QString::number(currentTime % 24) + "时 " +
      QString::fromStdString(m_planResult.result[log_i].transport.tran_name) +
      " " + "次" + getTranName(m_planResult.result[log_i].transport.tran_type) +
      " 从 %1 到 %2";
  QString cityString = realLogDate.toString("yyyy-MM-dd") + " " +
                       QString::number(currentTime % 24) + "时 " + "滞留于 %1";
  if (currentTime == m_planResult.startHour + m_totalTime) {
    logUpdated("于 " + realLogDate.toString("yyyy-MM-dd") + " " +
                   QString::number(currentTime % 24) + "时 到达终点 %1",
               m_planResult.result.back().transport.dest_city, 0, 1);
    m_simulateTimer.stop();
    emit simulationDone();
    return;
  }

  if (m_planResult.result[log_i].resultType == CITY) {
    logUpdated(cityString, m_planResult.result[log_i].waitCityIndex, 0, 1);
  } else {
    logUpdated(routeString, m_planResult.result[log_i].transport.source_city,
               m_planResult.result[log_i].transport.dest_city, 2);
  }
  currentTime++;
  if (m_planResult.result.size() != log_i + 1 &&
      currentTime == m_planResult.result[log_i + 1].beginHour) {
    log_i++;
  }
}

void GraphHandler::runSimulation() {
  generatePlanResult();
  printNewLog();
  m_simulateTimer.start();
}

void GraphHandler::receiveNewLog(QString string) { logFile.push_back(string); }

void GraphHandler::saveLog() {
  QFile fileOut("./Travel-Simulation.log");
  fileOut.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream streamFileOut(&fileOut);
  streamFileOut.setCodec("UTF-8");
  streamFileOut.setGenerateByteOrderMark(true);
  for (auto x : logFile)
    streamFileOut << x << "\n";
  streamFileOut.flush();
  fileOut.close();
}

void GraphHandler::getTimeAndTrans() {
    for(uint i = 0; i < uint(m_tranName.size()); i++){
        m_timeAndTrans.append(m_traveller.plan_result[i].start_time);
        m_timeAndTrans.append(m_traveller.plan_result[i].duration);
        m_timeAndTrans.append(m_traveller.plan_result[i].tran_type);
    }
}

QVector<int> GraphHandler::timeAndTrans() const { return m_timeAndTrans; }

void GraphHandler::setTimeAndTrans(const QVector<int> &timeAndTrans) {  }

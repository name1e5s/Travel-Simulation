#ifndef GRAPH_HANDLER_H
#define GRAPH_HANDLER_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <city_graph.h>

class GraphHandler : public QObject {
  Q_OBJECT
  Q_PROPERTY(int beginYear READ beginYear WRITE setBeginYear)
  Q_PROPERTY(int beginMonth READ beginMonth WRITE setBeginMonth)
  Q_PROPERTY(int beginDate READ beginDate WRITE setBeginDate)
  Q_PROPERTY(int sourceCity READ sourceCity WRITE setSourceCity)
  Q_PROPERTY(int destCity READ destCity WRITE setDestCity)
  Q_PROPERTY(int planType READ planType WRITE setPlanType)
  Q_PROPERTY(int leaveTime READ leaveTime WRITE setLeaveTime)
  Q_PROPERTY(int timeLimit READ timeLimit WRITE setTimeLimit)
  Q_PROPERTY(QVector<int> middleCity READ middleCity WRITE setMiddleCity)
  Q_PROPERTY(QVector<int> citySequence READ citySequence WRITE setCitySequence)
  Q_PROPERTY(QVector<QString> tranName READ tranName WRITE setTranName)
  Q_PROPERTY(
      int totalTime READ totalTime WRITE setTotalTime NOTIFY totalTimeChanged)
  Q_PROPERTY(int totalPrice READ totalPrice WRITE setTotalPrice NOTIFY
                 totalPriceChanged)
  Q_PROPERTY(QVector<int> timeAndTrans READ timeAndTrans WRITE setTimeAndTrans)
public:
  explicit GraphHandler(QObject *parent = nullptr);

  Q_INVOKABLE void generateResult();

  Q_INVOKABLE void appendMiddleCity(int value);
  Q_INVOKABLE void clearMiddleCity();
  Q_INVOKABLE void runSimulation();
  Q_INVOKABLE void saveLog();
  Q_INVOKABLE void receiveNewLog(QString string);
  Q_INVOKABLE void getTimeAndTrans();

  int beginYear() const;
  void setBeginYear(int beginYear);

  int beginMonth() const;
  void setBeginMonth(int beginMonth);

  int beginDate() const;
  void setBeginDate(int beginDate);

  QVector<int> middleCity() const;
  void setMiddleCity(const QVector<int> &middleCity);

  int leaveTime() const;
  void setLeaveTime(int leaveTime);

  int timeLimit() const;
  void setTimeLimit(int timeLimit);

  int planType() const;
  void setPlanType(int planType);

  QVector<int> citySequence() const;
  void setCitySequence(const QVector<int> &citySequence);

  int sourceCity() const;
  void setSourceCity(int sourceCity);

  int destCity() const;
  void setDestCity(int destCity);

  QVector<QString> tranName() const;
  void setTranName(const QVector<QString> &tranName);

  int totalTime() const;
  void setTotalTime(int totalTime);

  int totalPrice() const;
  void setTotalPrice(int totalPrice);

  QVector<int> timeAndTrans() const;
  void setTimeAndTrans(const QVector<int> &timeAndTrans);

signals:
  void totalTimeChanged();
  void totalPriceChanged();
  void simulationDone();
  void logUpdated(QString logInfo, int src_1, int src_2, int type);

public slots:
  void printNewLog();

private:
  void generateCitySequence();
  void generateTotalTime();
  void generateTotalPrice();
  void generatePlanResult();
  int m_beginYear, m_beginMonth, m_beginDate;
  int m_totalTime, m_totalPrice;
  CityGraph m_cityGraph;
  Traveller m_traveller;
  PlanResult m_planResult;
  QTimer m_simulateTimer;
  QVector<int> m_citySequence;
  QVector<QString> m_tranName;
  QVector<int> m_timeAndTrans;
};

#endif // GRAPH_HANDLER_H

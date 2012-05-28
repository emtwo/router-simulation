/**
 * M/D/1 Router Queue Simulation.
 *
 * Packets arrive at a Markovian(M) distribution interval.
 * Packets depart at a deterministic(D), constant interval.
 * Packets are only routed through 1 server.
 *
 **/
#include <math.h>
#include <iostream>
#include <queue>
#include "random.h"

using namespace std;

int T_ARRIVAL;
int T_DEPARTURE;
int numPacketsInQueue = 0;
int packetNum = 0;
const double LAMBDA = 100; // Avg # of packets generated/arrived per second
const int TICKS = 1000000;
const int TICKS_TIL_DEPARTURE = 10000;
queue<int> q;

double generateMicrosecondX(double U) {
  return (-1 / LAMBDA) * log(1 - U) * 1000000;
}

void arrival(int t) {
  if (t != T_ARRIVAL) {
    return;
  }

  cout << "New packet will be: " << packetNum << endl;
  q.push(packetNum++);
  T_ARRIVAL += generateMicrosecondX(genrand());
}

void departure(int t) {
  if (q.size() == 0 || t < T_DEPARTURE) {
    return;
  }

  cout << "Packet " << q.front() << " is departing." << endl;
  q.pop();
  T_DEPARTURE = t + TICKS_TIL_DEPARTURE;
}

void start_simulation(int ticks) {
  for (int t = 0; t < ticks; t++) {
    usleep(1);
    arrival(t);
    departure(t);
  }
}

void compute_performances() {
}

int main() {
  sgenrand(4357);
  T_ARRIVAL = generateMicrosecondX(genrand());
  T_DEPARTURE = T_ARRIVAL;

  cout << "Arrival Tick: " << T_ARRIVAL << endl;
  cout << "Departure Tick: " << T_DEPARTURE << endl;
  cout << "Total Ticks: " << TICKS << endl;

  start_simulation(TICKS);
  compute_performances();
}
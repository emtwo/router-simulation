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

// Constants:
const long TICKS = 6000000000;
const double LAMBDA = 100;     // Avg # of packets generated/arrived per second
const double LENGTH = 1;       // Length of packet in bits
const double C = 1;            // Transmittion rate of the output link (bits per second)

// Output variables.
double numPackets = 0;         // Sum of packets in every tick.
double totalTime = 0;          // Sum of time taken from arrival to departure for every packet.
double totalIdleTicks = 0;     // Total num ticks with server doing nothing.
//const double p;              // Utilization of the queue

// Other.
int T_ARRIVAL;                 // Time for next packet arrival.
int T_DEPARTURE;               // Time for next packet departure.
int T_TRANSMIT;                // Time for next packet transmit.
int packetNum = 0;             // Packet data.
queue<int> q;                  // Router queue.
queue<int> elapsedTime;        // A queue of the arrival times of each packet.

double generateMicrosecondX() {
  return (-1 / LAMBDA) * log(1 - genrand()) * 1000000;
}

void arrival(int t) {
  if (t != T_ARRIVAL) {
    return;
  }

  cout << "New packet will be: " << packetNum << endl;
  q.push(packetNum++);
  elapsedTime.push(t);
  T_ARRIVAL += generateMicrosecondX();
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
  T_ARRIVAL = generateMicrosecondX();
  T_DEPARTURE = T_ARRIVAL;

  cout << "Arrival Tick: " << T_ARRIVAL << endl;
  cout << "Departure Tick: " << T_DEPARTURE << endl;
  cout << "Total Ticks: " << TICKS << endl;

  start_simulation(TICKS);
  compute_performances();
}
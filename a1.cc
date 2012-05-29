/**
 * M/D/1/K Router Queue Simulation.
 *
 * Packets arrive at a Markovian(M) distribution interval.
 * Packets depart at a deterministic(D), constant interval.
 * Packets are only routed through 1 server.
 * Buffer is bounded with a size K.
 *
 **/
#include <math.h>
#include <iostream>
#include <queue>
#include <iomanip>
#include "random.h"

using namespace std;

#define INFINITE -1

// Constants:
const unsigned long TICKS = 5000000000;
const double LAMBDA = 100;        // Avg # of packets arrived per second
const double LENGTH = 2000;       // Length of packet in bits
const double C = 1000000;         // Transmittion rate of the output link (bits per second)
const int K = INFINITE;                  // Max size of router queue.

const int SERVICE_TIME = LENGTH * 1000 / C;

// Output variables.
double numPackets = 0;            // Sum of packets in every tick.
double totalTime = 0;             // Sum of time taken from arrival to departure for every packet.
double totalIdleTicks = 0;        // Total num ticks with server doing nothing.
double totalPacketsLost = 0;      // Total num of packets lost;
double totalNumPackets = 0;       // Total number of packets attempting arrival.
//const double p;                 // Utilization of the queue

// Other.
unsigned long T_ARRIVAL;          // Time for next packet arrival.
unsigned long T_DEPARTURE;        // Time for next packet departure.
unsigned long T_TRANSMIT;         // Time for next packet transmit.
unsigned long packetNum = 0;      // Packet data.
queue<int> q;                     // Router queue.
queue<unsigned long> elapsedTime; // A queue of the arrival times of each packet.

double generateMicrosecondX() {
  // Multiply to convert to microseconds.
  // Add 1 to ensure generated number is > 0. 
  return (-1.0 / LAMBDA) * log(1.0 - genrand()) * 1000000.0 + 1.0;
}

void arrival(unsigned long t) {
  if (t != T_ARRIVAL) {
    return;
  }

  //cout << "New packet will be: " << packetNum << endl;
  if (q.size() >= K && K != INFINITE) {
    totalPacketsLost++;
    return;
  }
  totalNumPackets++;

  q.push(packetNum++);
  elapsedTime.push(t);
  T_ARRIVAL += generateMicrosecondX();
}

void departure(unsigned long t) {
  // Server is idle only when the queue is empty.
  if (q.size() == 0) {
    totalIdleTicks++;
  }

  if (t == T_TRANSMIT) {
    //cout << "Packet " << q.front() << " is departing." << endl;
    q.pop();

    // Add the time taken from arrival to end of transmission.
    totalTime += (t-elapsedTime.front());
    elapsedTime.pop();
  }

  // There are items in the queue but nothing is scheduled to transmit yet.
  if (q.size() != 0 && t >= T_TRANSMIT) {
    T_DEPARTURE = t;
    T_TRANSMIT = T_DEPARTURE + SERVICE_TIME;
  }
}

void start_simulation(unsigned long ticks) {
  for (unsigned long t = 0; t < ticks; ++t) {
    numPackets += q.size();
    arrival(t);
    departure(t);
    //if (t % 1000000 == 0) cout << "tick: " << t << endl;
  }
}

void compute_performances() {
  cout << fixed << setprecision(10);
  cout << "E[N]: " << numPackets / TICKS << " (avg. # packets in queue per tick)" << endl;
  cout << "E[T]: " << totalTime / TICKS << " (avg. # ticks from arrival to end of transmission)" << endl;
  cout << "Pidle: " << (totalIdleTicks / TICKS) * 100 << " %" << endl;
  if (K != INFINITE) {
    cout << "Ploss: " << (totalPacketsLost / totalNumPackets) * 100 << " %" << endl; 
  }
}

int main() {
  sgenrand(1359084);
  T_ARRIVAL = generateMicrosecondX();
  T_DEPARTURE = T_ARRIVAL;
  T_TRANSMIT = T_DEPARTURE + SERVICE_TIME;

  /*double ro;
  cin >> ro;

  LAMBDA = ((double) ro) * SERVICE_TIME;*/

  cout << "Arrival Tick: " << T_ARRIVAL << endl;
  cout << "Departure Tick: " << T_DEPARTURE << endl;
  cout << "Total Ticks: " << TICKS << endl;

  start_simulation(TICKS);
  compute_performances();
}
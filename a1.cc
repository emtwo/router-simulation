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
unsigned long TICKS; //number of microseconds to run the simulation for
double LAMBDA; // Avg # of packets arrived per second
unsigned long LENGTH; // Length of packet in bits
double C; // Transmittion rate of the output link (bits per second)
int K = INFINITE; // Max size of router queue.

int SERVICE_TIME;

// Output variables.
unsigned long sumPacketsPerTick = 0;            // Sum of packets in every tick.
unsigned long totalTime = 0;             // Sum of time taken from arrival to departure for every packet.
unsigned long totalIdleTicks = 0;        // Total num ticks with server doing nothing.
unsigned long totalPackets = 0;       // Total number of packets attempting arrival.
unsigned long totalPacketsLost = 0;      // Total num of packets lost;
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

  totalPackets++;
  if (q.size() >= K && K != INFINITE) {
    totalPacketsLost++;
    return;
  }

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

void compute_performances(unsigned long ticks) {
  cout << fixed << setprecision(10);
  cout << "At tick: " << ticks << endl;
  cout << "E[N]: " << (double) sumPacketsPerTick / ticks << " (avg. # packets in queue per tick)" << endl;
  cout << "E[T]: " << (double) totalTime / totalPackets << " (avg. # ticks from arrival to end of transmission)" << endl;
  cout << "Pidle: " << ((double) totalIdleTicks / ticks) * 100 << " %" << endl;
  if (K != INFINITE) {
    cout << "Ploss: " << ((double) totalPacketsLost / totalPackets) * 100 << " %" << endl; 
  }
}

void start_simulation(unsigned long ticks) {
  for (unsigned long t = 0; t < ticks; ++t) {
    sumPacketsPerTick += q.size();
    arrival(t);
    departure(t);

    /*if (t % 1000000 == 0) {
      compute_performances(t);
    }*/
  }
}

int main() {
  TICKS = 5000000000;
  LAMBDA = 100;
  LENGTH = 2000;
  C = 1000000; 
  K = INFINITE;
  
  SERVICE_TIME = LENGTH * 1000 / C;

  sgenrand(1359084);
  T_ARRIVAL = generateMicrosecondX();
  T_DEPARTURE = T_ARRIVAL;
  T_TRANSMIT = T_DEPARTURE + SERVICE_TIME;

  /*double ro;
  cin >> ro;

  LAMBDA = ro * C / LENGTH;

  cout << "LAMBDA is " << LAMBDA << endl;*/

  cout << "Arrival Tick: " << T_ARRIVAL << endl;
  cout << "Departure Tick: " << T_DEPARTURE << endl;
  cout << "Total Ticks: " << TICKS << endl;

  start_simulation(TICKS);
  compute_performances(TICKS);
}

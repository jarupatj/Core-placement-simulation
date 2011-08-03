#pragma once

class Cost {
   public:
      Cost();
      ~Cost();

      //function
      void init(double alpha, double beta, double gamma, double theta);
      double initCost(const int** bandwidth, const int** latency, const vector<Core> core, const int LINK_LATENCY, const Network& network); 

   private:
      //variable
      double alpha, beta, gamma, theta;
      double cost;
      double compaction, dilation, slack, proximity, utilization;

      //function
      double compactionCost(const int** bandwidth, const vector<Core> core);
      double dilationCost(const int** bandwidth, const int** latency, const vector<Core> core, const int LINK_LATENCY, const Network& network); 
      double slackCost(const int** latency, const vector<Core> core, const int LINK_LATENCY); 
      double proximityCost(const int** bandwidth, const vector<Core> core); 
      double utilizationCost(const int** bandwidth, const vector<Core> core, const Network& network); 
      int getHops(Coordinate a, Coordinate b);

};

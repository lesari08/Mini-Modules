/* 

@file stream_merger.hpp
@brief A class that merges multiple input streams of integers into a single sorted output stream.
This class provides functionality to merge multiple input streams of integers into a single sorted output stream.
The input streams are represented as vectors of pairs, where each pair contains an integer value and the time
at which the value was added to the stream. The output stream is produced by continuously popping the minimum
value from a min heap that is formed by merging the input streams. The class provides methods to add input streams
to the merger and to get the next element from the merged stream.

*/

#pragma once 

#include <iostream>
#include <vector>
#include <utility>
#include <queue>

using namespace std;

class StreamMerger
{
public:

    using IntPair = pair<int,int>;

    struct Info
    {
        int value;
        int time;

        bool operator>(const Info& obj) const
        {
            return this->time > obj.time; 
        }

        bool operator<(const Info& obj) const
        {
            return this->time < obj.time; 
        }

        Info(int v, int t) : value(v), time(t){}
    };

    // given N sorted streams, merge the streams a 
    // single list 
    void mergeStreams(vector<vector<IntPair>> streams)
    {
        for(auto s : streams)
        {   
            addToStream(s);
        }
    }


    // return the next pair from the
    // sorted list 
    IntPair getNextElement()
    {
        if(minHeap.empty())
            return {};

        auto pr = minHeap.top(); 
        minHeap.pop();

        int curr_value = pr.value;
        pr.value -=prevReturnedValue;

        prevReturnedValue = curr_value;

        return {pr.value, pr.time};
    }


    StreamMerger() 
        : prevReturnedValue(0), minHeap(), no_insertions(true){}
    
private:

    priority_queue<Info, vector<Info>, std::greater<Info>> minHeap;
    int prevReturnedValue; 
    bool no_insertions;

    // helper method called by mergeStreams
    void addToStream(const vector<IntPair>& stream)
    {
        int prev = 0;
        for(const auto& pr : stream)
        {
            int delta = pr.first;
            int time = pr.second;
            int val = prev + delta;
            minHeap.push(Info(val, time));
            prev = val;
        }
    }
};


void runTestStreamMerger()
{
    int size = 6;
    vector<vector<pair<int,int>>> streams{
        {{10,0}, {-5,6}},
        {{17,2}},
        {{15,1},{5,4},{1,8}}
        };

        StreamMerger sm;
        sm.mergeStreams(streams);

        vector<pair<int,int>> results;
        for(int i = 0; i <size; i++)
        {
            results.push_back(sm.getNextElement());
        }

        for(auto pr : results)
        {
            cout<<"["<<pr.first<<" "<<pr.second<<"]"<<endl;
        }
    /*
    Expected Results
    [10 0]
    [5 1]
    [2 2]
    [3 4]
    [-15 6]
    [16 8]
    */
}
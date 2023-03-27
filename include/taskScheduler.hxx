/**

@file TaskScheduler.hxx

@brief This file simulates a library for scheduling tasks based on their dependencies and duration.
This library provides functionality for scheduling a set of tasks based on their dependencies
and duration. The user provides a graph of dependencies as an unordered map where each key
represents a task and each value is a vector of the task's dependencies. In addition, the user
provides a map where each key is a task and each value is the duration of the task. The library
outputs a vector of strings representing the order in which the tasks should be executed, with
each string representing a set of tasks that can be executed in parallel.
Example usage:
std::unordered_mapstd::string,int taskTime {{"A",1}, {"B",2}, {"C",7}, {"D", 3}, {"E",3}};
std::unordered_map<std::string,std::vectorstd::string> graph;
graph.insert({"B", {"A"}});
graph.insert({"C", {"A"}});
graph.insert({"D", {"A", "B"}});
graph.insert({"E", {"C", "B"}});
std::vectorstd::string result = TaskScheduler::scheduler(taskTime , graph);
TaskScheduler::print(result);
Output:
1 A
2 C B
3 C D
2 B
3 E

Note: This library is still in development and not all features are fully implemented or tested.
*/
#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits.h>



namespace TaskScheduler
{
    template<typename T>
    void print(const std::vector<T>& steps)
    {
        for(const auto& step : steps)
            cout<<step<<"\n";
    }

    template<typename keyType>
    struct Maps
    {
        std::unordered_map<keyType,int> inEdges;
        std::unordered_map<keyType,std::vector<keyType>> adjList;
    };

    Maps<std::string> CreateAdjListAndIncomingEdgesMap(const std::unordered_map<std::string,std::vector<std::string>>& inputGraph)
    { 
        std::unordered_map<std::string, int> inEdges;
        std::unordered_map<std::string, std::vector<std::string>> graph;

        for(const auto&[dep, neighbors] : inputGraph)
        {
            for(const auto& neighbor : neighbors)
            {
                graph[neighbor].push_back(dep);
                inEdges[dep]++;
                
                if(inEdges.find(neighbor) == inEdges.end())
                    inEdges.insert({neighbor, 0});
            }
        }

        Maps<std::string> maps;
        maps.inEdges = inEdges;
        maps.adjList = graph;
        return maps;
    }

    template<typename keyType>
    int getMinTime(std::queue<keyType>& que, const std::unordered_map<keyType,int>& taskToTimeMap)
    {
        int minTime = INT_MAX;
        
        for(int i = que.size()-1; i >=0; --i)
        {
            std::string curr = que.front(); que.pop();
            
            const auto& keyVal = taskToTimeMap.find(curr);
            minTime = min(minTime, keyVal->second);
            
            que.push(curr);
        }
        
        return minTime;
    }


    template<typename keyType>
    std::vector<keyType> scheduler(unordered_map<keyType,int> taskToTimeMap, std::unordered_map<keyType, std::vector<keyType>>& inputGraph)
    {
        std::vector<keyType> result;

        Maps<keyType> maps = CreateAdjListAndIncomingEdgesMap(inputGraph);
        auto inEdges = maps.inEdges;
        auto graph = maps.adjList;

        queue<keyType> ready_tasks;
        for(const auto& [name, num_in_edges] : inEdges)
        {
            if(num_in_edges == 0)
                ready_tasks.push(name);
        }
        
        while(!ready_tasks.empty())
        {
            int minTime = getMinTime(ready_tasks,taskToTimeMap);
            std::string order(std::to_string(minTime) + ',');
        
            for(int i = ready_tasks.size()-1; i >=0; i--)
            {
                keyType curr = ready_tasks.front(); 
                ready_tasks.pop();
            
                order+=curr + ',';
                
                taskToTimeMap[curr]-=minTime;
            
               
                if(taskToTimeMap[curr] == 0)
                {
                    // task is finished. 
                    // Update neighbors

                    for(const std::string& neighbor : graph[curr])
                    {
                        --inEdges[neighbor];
                        if(inEdges[neighbor] == 0)
                            ready_tasks.push(neighbor);
                    }
                }
                else 
                {
                    // task is not finished, 
                    // push task back into queue

                    ready_tasks.push(curr);
                }
            }
            
            order.pop_back();
            result.push_back(order);
        }
        
        return result;
    }

    // TODO
    // Move the run() methods below into 
    // a test suite
    void run()
    {
        std::unordered_map<std::string,int> taskTime {{"A",1}, {"B",2}, {"C",7}, {"D", 3}, {"E",3}};
    
        std::unordered_map<std::string,std::vector<std::string>> graph;
        graph.insert({"B", {"A"}});
        graph.insert({"C", {"A"}});
        graph.insert({"D", {"A", "B"}});
        graph.insert({"E", {"C", "B"}});
        std::vector<std::string> result = scheduler(taskTime , graph);
        print(result);
        /*
        Expect 
        1 A
        2 C b 
        3 C D 
        2 c
        3 E 
        */
       
    }


    void run2()
    {
        std::unordered_map<std::string,int> taskTime {{"A",1}, {"B",2}, {"C",3}, {"D", 3}, {"E",3}};
    
        std::unordered_map<std::string,std::vector<std::string>> graph;
        graph.insert({"B", {"A"}});
        graph.insert({"C", {"A"}});
        graph.insert({"D", {"A", "B"}});
        graph.insert({"E", {"C", "B"}});
        std::vector<std::string> result = scheduler(taskTime , graph);
        print(result);

        /*
        Expect
        1 A
        2 cb
        1 c d 
        2 e d 
        1 e
        */
    }
}
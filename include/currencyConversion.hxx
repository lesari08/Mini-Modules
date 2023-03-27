#pragma once

#include <bitset>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

/*
Approaches: Time using ints
-DFS: |V| + |E| == V^2
-DFS w/ memo
-Dijsktra native
-dijkstra w/ heap
-floyd warshall
-bitset?
*/

namespace Solutions
{
    double currencyExchangeDfs(vector<string> names, vector<vector<double>> matrix, string source, string destination);
    

    void runCX()
    {
        vector<string> currences{"USD", "CAD","EUR","JPY"};
        vector<vector<double>> matrix{
            {1,1.3,1,6.49},
            {0.72,1,0.9,5.5},
            {1.1,1.1,1,7.3},
            {0.18,0.2,0.136,1}
        };

        string source = "USD";
        string destination="JPY";
        cout<< currencyExchangeDfs(currences, matrix, source, destination)<<endl;
    }

    void dfs(int currency, int destination, double currRate, double& maxRate, vector<bool>& path, const vector<vector<double>>& matrix)
    {
        path[currency] = true;

        if(currency == destination)
        {
            maxRate = max(currRate, maxRate);
        }
        else
        {
            for(int i = 0; i < matrix.front().size(); i++)
            {
                if(path[i] == false)
                    dfs(i, destination, currRate * matrix[currency][i], maxRate,path, matrix);
                
            }
        }
        path[currency] = false;
    }

    //O(|V| + |E| == V^2)
    double currencyExchangeDfs(vector<string> names, vector<vector<double>> matrix, string source, string destination)
    {
        int sourceIdx = 0;
        int destinationIdx = 0;
        for(int i = 0; i < names.size(); i++)
        {
            if(names[i] == source)
                sourceIdx = i;
            else if(names[i] == destination)
                destinationIdx = i;
        }

        vector<bool> path(names.size());
        double maxRate{};
        double startRate = 1.0;
        
        dfs(sourceIdx, destinationIdx, startRate, maxRate,path, matrix);
      
        cout<<endl;
        return maxRate;
    }



};


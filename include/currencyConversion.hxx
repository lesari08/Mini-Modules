/*
    @file currencyConversion.hxx
    This file contains methods that simulate currency exchange. Given a matrix of exchange rates for various currencies,
    find the maximum exchange rate from a starting currency to a destination currency. The constraints are that while searching
    for the best exchange rate, a currency cannot be exchanged more than once.
*/

#pragma once

#include <iostream>
#include <vector>


namespace exchange
{
    double currencyExchangeDfs(
        std::vector<std::string_view> names, 
        std::vector<std::vector<double>> matrix, 
        std::string_view source, 
        std::string_view destination);


    void dfs(
        int currency, 
        int destination, 
        double currRate, 
        double& maxRate, 
        std::vector<bool>& path,
        const std::vector<std::vector<double>>& matrix
    )
    {
        path[currency] = true;

        if(currency == destination)
        {
            maxRate = std::max(currRate, maxRate);
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

    /**
    * @brief Performs currency exchange calculations using Depth-First Search algorithm.
    *
    * @param names std::vector of currency names in string format.
    * @param matrix Matrix containing currency conversion rates.
    * @param source Name of the source currency.
    * @param destination Name of the target currency.
    * @return double Returns the highest conversion rate from the source to the destination currency.
    */
    // TODO Implement improved algorithm (perharps bellman-held-karp?)
    double currencyExchangeDfs(
        std::vector<std::string_view> names, 
        std::vector<std::vector<double>> matrix, 
        std::string_view source, 
        std::string_view destination)
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

        std::vector<bool> path(names.size());
        double maxRate = 0.0;
        double startRate = 1.0;

        dfs(sourceIdx, destinationIdx, startRate, maxRate,path, matrix);
      
        return maxRate;
    }


    // TODO
    // Move test function into a unit test suite
    void runCX()
    {
        std::vector<std::string_view> currences{"USD", "CAD","EUR","JPY"};
        std::vector<std::vector<double>> matrix{
            {1,1.3,1,6.49},
            {0.72,1,0.9,5.5},
            {1.1,1.1,1,7.3},
            {0.18,0.2,0.136,1}
        };

        std::string_view source = "USD";
        std::string_view destination="JPY";

        std::cout<< currencyExchangeDfs(currences, matrix, source, destination)<<"\n";
    }

};


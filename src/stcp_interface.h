#ifndef STCP_INTERFACE_H
#define STCP_INTERFACE_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <deque>

namespace stcp
{
    // Constants and global helper functions
    constexpr double kEps{1e-12};
    constexpr double kNegInf{-std::numeric_limits<double>::infinity()};

    inline double logSumExp(const std::vector<double> &xs)
    {
        if (xs.empty())
        {
            throw std::runtime_error("Empty vector is not allowed for the logSumExp function.");
        }
        double max_x = *std::max_element(xs.begin(), xs.end());
        double sum_exp{0.0};
        for (auto &x : xs)
        {
            sum_exp += std::exp(x - max_x);
        }

        return log(sum_exp) + max_x;
    }

    // stcp interface classes
    class IBaselineIncrement
    {
    public:
        virtual double computeLogBaseValue(const double &x) = 0;
        // Exponential baseline can be computed
        // by using x_bar = 1/n * sum_{i=1}^n x_i and n values as inputs.
        // General baseline may be computed by s/v and v as inputs.
        // This method must take n as a double rather than integer for generality.
        virtual double computeLogBaseValueByAvg(const double &x_bar, const double &n)  = 0;

        virtual ~IBaselineIncrement() {}
    };

    class ILogLRIncrement
    {
    public:
        virtual double computeLogBaseValue(const double &x) = 0;
        // Unrestricted mle for h1 parameter
        virtual void updateH1MLE(double &h_1_mle, const double &x, const int &n) = 0; 
        // MaxLLR must be computed under H1 restriction.
        virtual double computeMaxLLR(const double &h_1_mle, const int &n) = 0; 
        
        virtual ~ILogLRIncrement() {}
    };

    class IGeneralE
    {
    public:
        virtual double getLogValue() = 0;
        virtual void reset() = 0;
        virtual void updateLogValue(const double &x) = 0;
        virtual void updateLogValueByAvg(const double &x_bar, const double &n)  = 0;

        virtual ~IGeneralE() {}
    };

    class IStcp
    {
    public:
        virtual double getLogValue() = 0;
        virtual double getThreshold() = 0;

        virtual bool isStopped() = 0;
        virtual double getTime() = 0; // Use double for generality
        virtual double getStoppedTime() = 0; // Use double for generality

        virtual void reset() = 0;

        virtual void updateLogValue(const double &x) = 0;
        virtual void updateLogValues(const std::vector<double> &xs) = 0;
        virtual void updateLogValuesUntilStop(const std::vector<double> &xs) = 0;
    
        virtual void updateLogValueByAvg(const double &x_bar, const double &n) = 0;
        virtual void updateLogValuesByAvgs(const std::vector<double> &x_bars, const std::vector<double> &ns) = 0;
        virtual void updateLogValuesUntilStopByAvgs(const std::vector<double> &x_bars, const std::vector<double> &ns) = 0;

        // For visualization, IStcp support update and return updated history
        virtual double updateAndReturnHistory(const double &x) = 0;
        virtual std::vector<double> updateAndReturnHistories(const std::vector<double> &xs) = 0;

        virtual double updateAndReturnHistoryByAvg(const double &x_bar, const double &n) = 0;
        virtual std::vector<double> updateAndReturnHistoriesByAvgs(const std::vector<double> &x_bars, const std::vector<double> &ns) = 0;

        virtual ~IStcp() {}
    };
} // End of namespace stcp
#endif

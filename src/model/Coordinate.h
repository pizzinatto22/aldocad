#ifndef COORDINATE_H
#define COORDINATE_H

#include <array>

template <unsigned N>
class Coordinate {
    public:
        template <typename ...T> 
        Coordinate(double first, T... next):
            values_{{first, next...}}
        {}

        std::array<double, N> values()
        {
        	return values_;
        }

        double &operator()(unsigned x)
        {
          if (x < N) return values_[x];
          else throw std::range_error("Tried to access out of range element");
        }


    private:
        std::array<double, N> values_;
};

#endif

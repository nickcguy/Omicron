//
// Created by Guy on 06/08/2017.
//

#ifndef OMICRONRENDER_FILTEREDVECTOR_HPP
#define OMICRONRENDER_FILTEREDVECTOR_HPP

#include <vector>
#include <functional>

namespace Omicron {


    template <typename T>
    class FilteredVector : public std::vector<T> {

        // TODO optimise
    public:
        inline FilteredVector Filter(std::function<bool(T)> filter) {
            FilteredVector<T> vec;
            for(T item : *this) {
                if(filter(item))
                    vec.push_back(item);
            }
            return vec;
        }

    };

}

#endif //OMICRONRENDER_FILTEREDVECTOR_HPP

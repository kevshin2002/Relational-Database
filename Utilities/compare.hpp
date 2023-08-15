//
//  compare.hpp
//  Created by rick gessner on 4/30/21.
//

#ifndef compare_hpp
#define compare_hpp

#include <sstream>
#include <type_traits>
  
  //Equal ------------------------------------------
namespace ECE141 {
    template <typename T>
    inline bool isEqual(const T& arg1, const T& arg2) {
        return arg1 == arg2;
    }

    inline bool isEqual(const std::string& aStr1, const std::string& aStr2) {
        return aStr1 == aStr2;
    }

    template <typename T1, typename T2>
    typename std::enable_if<std::is_convertible<T2, T1>::value, bool>::type
        inline    isEqual(const T1& arg1, const T2& arg2) {
        return static_cast<T1>(arg2) == arg1;
    }

    // SFINAE
    template <typename T1, typename T2>
    typename std::enable_if<!std::is_convertible<T2, T1>::value, bool>::type
        inline    isEqual(const T1& arg1, const T2& arg2) {
        return false;
    }

    template <typename T1>
    inline bool isEqual(const T1& arg1, const std::string& aStr) {
        std::stringstream temp;
        temp << arg1;
        std::string theStr = temp.str();
        bool theResult = theStr == aStr;
        return theResult;
    }

    inline bool isEqual(const std::string& aStr, const bool& arg2) {
        return isEqual(arg2, aStr);
    }

    inline bool isEqual(const std::string& aStr, const int& arg2) {
        return isEqual(arg2, aStr);
    }

    inline bool isEqual(const std::string& aStr, const double& arg2) {
        return isEqual(arg2, aStr);
    }

    // Less than ------------------------------------------


    // Greater than ------------------------------------------


    //--------- feel free to add more operators... ------

}

#endif /* compare_h */

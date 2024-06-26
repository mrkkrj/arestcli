//
// some utilities to replace boost (mrkkrj)
//

#include <algorithm> 
#include <cctype>
#include <locale>

namespace utilities
{
    
    // source: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
    // ------

    // trim from start (in place)
    inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    // trim from end (in place)
    inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string& s) {
        rtrim(s);
        ltrim(s);
    }

    // trim from start (copying)
    inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }


    // source: https://stackoverflow.com/a/4119881
    // ------

    template <class StringT1, class StringT2>
    bool iequals(const StringT1& a, const StringT2& b)
    {
        return std::equal(std::begin(a), std::end(a),
            std::begin(b), std::end(b),
            [](char a, char b) {
                return tolower(a) == tolower(b);
            });
    }

#if 0
    template <class StringT1>
    bool iequals(const StringT1& a, const char* b)
    {
        return std::equal(std::begin(a), std::end(a),
            b, b + strlen(b) - 1, // OPEN TODO::: hack!!!
            [](char a, char b) {
                return tolower(a) == tolower(b);
            });
    }
#endif

}

/****************************************************************************/
/// @file    TplConvert.h
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    Sun, 09 Jun 2002
/// @version $Id$
///
// Some conversion methods (from strings to other)
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2001-2015 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef TplConvert_h
#define TplConvert_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <cmath>
#include <limits>
#include <algorithm>
#include <utils/common/UtilExceptions.h>
#include <utils/common/StdDefs.h>
#include <utils/geom/Position.h>    // PABLO #1916


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * TplConvert
 * Some methods that convert 0-terminated char-arrays of any type into
 * other values (numerical/boolean) or into std::strings
 */
class TplConvert {
public:
    /** converts a 0-terminated char-type array into std::string
        throws an EmptyData - exception if the given pointer is 0 */
    template<class E>
    static inline std::string _2str(const E* const data) {
        return _2str(data, getLength(data));
    }


    /** converts a 0-terminated char array into std::string
        throws an EmptyData - exception if the given pointer is 0 */
    static inline std::string _2str(const char* const data) {
        if (data == 0) {
            throw EmptyData();
        }
        return std::string(data);
    }


    /** converts a char-type array into std::string considering the given
            length
        throws an EmptyData - exception if the given pointer is 0 */
    template<class E>
    static inline std::string _2str(const E* const data, unsigned length) {
        if (data == 0) {
            throw EmptyData();
        }
        if (length == 0) {
            return "";
        }
        char* buf = new char[length + 1];
        unsigned i = 0;
        for (i = 0; i < length; i++) {
            if ((int) data[i] > 255) {
                buf[i] = 63; // rudimentary damage control, replace with '?'
            } else {
                buf[i] = (char) data[i];
            }
        }
        buf[i] = 0;
        std::string ret = buf;
        delete[] buf;
        return ret;
    }


    /** converts a char array into std::string considering the given
            length
        throws an EmptyData - exception if the given pointer is 0 */
    static inline std::string _2str(const char* const data, unsigned length) {
        if (data == 0) {
            throw EmptyData();
        }
        return std::string(data, length);
    }


    /** converts a char-type array into the integer value described by it
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain an integer */
    template<class E>
    static int _2int(const E* const data) {
        long long int result = _2long(data);
        if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min()) {
            throw NumberFormatException();
        }
        return (int)result;
    }


    /** converts a string into the integer value described by it
        by calling the char-type converter, which
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain an integer */
    static int _str2int(const std::string& sData) {
        return _2int(sData.c_str());
    }


    /** converts a char-type array with a hex value into the integer value described by it
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain an integer */
    template<class E>
    static int _hex2int(const E* const data) {
        long long int result = _hex2long(data);
        if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min()) {
            throw NumberFormatException();
        }
        return (int)result;
    }

    /** converts a string with a hex value into the integer value described by it
        by calling the char-type converter, which
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain an integer */
    static int _strHex2int(const std::string& sData) {
        return _hex2int(sData.c_str());
    }


    /** converts a char-type array into the long value described by it
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain a long */
    template<class E>
    static long long int _2long(const E* const data) {
        if (data == 0 || data[0] == 0) {
            throw EmptyData();
        }
        long long int sgn = 1;
        unsigned i = 0;
        if (data[0] == '+') {
            i++;
        }
        if (data[0] == '-') {
            i++;
            sgn = -1;
        }
        long long int ret = 0;
        for (; data[i] != 0; i++) {
            ret *= 10;
            // !!! need to catch overflows
            char akt = (char) data[i];
            if (akt < '0' || akt > '9') {
                throw NumberFormatException();
            }
            ret += akt - 48;
        }
        if (i == 0) {
            throw EmptyData();
        }
        return ret * sgn;
    }


    /** converts a char-type array with a hex value into the long value described by it
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain a long */
    template<class E>
    static long long int _hex2long(const E* const data) {
        if (data == 0 || data[0] == 0) {
            throw EmptyData();
        }
        long long int sgn = 1;
        unsigned i = 0;
        if (data[0] == '+') {
            i++;
        }
        if (data[0] == '-') {
            i++;
            sgn = -1;
        }
        if (data[i] == '#') { // for html color codes
            i++;
        }
        if (data[i] == '0' && (data[i + 1] == 'x' || data[i + 1] == 'X')) {
            i += 2;
        }
        long long int ret = 0;
        for (; data[i] != 0; i++) {
            ret *= 16;
            // !!! need to catch overflows
            char akt = (char) data[i];
            if (akt >= '0' && akt <= '9') {
                ret += akt - '0';
            } else if (akt >= 'A' && akt <= 'F') {
                ret += akt - 'A' + 10;
            } else if (akt >= 'a' && akt <= 'f') {
                ret += akt - 'a' + 10;
            } else {
                throw NumberFormatException();
            }
        }
        if (i == 0) {
            throw EmptyData();
        }
        return ret * sgn;
    }


    /** converts a char-type array into the SUMOReal value described by it
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain a SUMOReal */
    template<class E>
    static SUMOReal _2SUMOReal(const E* const data) {
        if (data == 0 || data[0] == 0) {
            throw EmptyData();
        }
        int i = 0;
        SUMOReal sgn = 1;
        if (data[0] == '+') {
            i++;
        }
        if (data[0] == '-') {
            i++;
            sgn = -1;
        }
        // we try to parse it as a long long int storing the decimal point pos
        int pointPos = -1;
        int digits = std::numeric_limits<long long int>::digits10;
        long long int ret = 0;
        for (; data[i] != 0 && data[i] != 'e' && data[i] != 'E'; i++) {
            char akt = (char) data[i];
            if (akt < '0' || akt > '9') {
                if (pointPos < 0 && (akt == '.' || akt == ',')) {
                    pointPos = i;
                    continue;
                }
                throw NumberFormatException();
            }
            digits--;
            if (digits >= 0) { // we skip the digits which don't fit into long long int
                ret = ret * 10 + akt - 48;
            }
        }
        int exponent = digits >= 0 ? 0 : -digits;
        if (pointPos != -1) {
            exponent += pointPos - i + 1;
        }
        // check what has happened - end of string or exponent
        if (data[i] == 0) {
            return ret * sgn * (SUMOReal) pow(10.0, exponent);
        }
        // now the exponent
        try {
            return ret * sgn * (SUMOReal) pow(10.0, _2int(data + i + 1) + exponent);
        } catch (EmptyData&) {
            // the exponent was empty
            throw NumberFormatException();
        }
    }


    /** converts a string into the SUMOReal value described by it
        by calling the char-type converter, which
        throws an EmptyData - exception if the given string is empty
        throws a NumberFormatException - exception when the string does
            not contain a SUMOReal */
    static SUMOReal _str2SUMOReal(const std::string& sData) {
        return _2SUMOReal(sData.c_str());
    }


    /** converts a 0-terminated char-type array into the boolean value
            described by it
        returns true if the data* is one of the following (case insensitive):
            '1', 'x', 'true', 'yes', 'on'
        returns false if the data* is one of the following (case insensitive):
            '0', '-', 'false', 'no', 'off'
        throws an EmptyData - exception if the given string is empty or 0 pointer
        throws a BoolFormatException in any other case
    */
    template<class E>
    static bool _2bool(const E* const data) {
        if (data == 0 || data[0] == 0) {
            throw EmptyData();
        }
        std::string s = _2str(data);
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s == "1" || s == "yes" || s == "true" || s == "on" || s == "x" || s == "t") {
            return true;
        } else if (s == "0" || s == "no" || s == "false" || s == "off" || s == "-" || s == "f") {
            return false;
        } else {
            throw BoolFormatException();
        }
    }

    /** converts a string into the bool value described by it       // PABLO #1916
    by calling the char-type converter, which                       // PABLO #1916
    throws an EmptyData - exception if the given string is empty    // PABLO #1916
    throws a BoolFormatException - exception when the string does   // PABLO #1916
    not contain a bool */                                           // PABLO #1916
    static bool _str2bool(const std::string& sData) {               // PABLO #1916
        return _2bool(sData.c_str());                               // PABLO #1916
    }                                                               // PABLO #1916

    // convert a string into Position in the three axis (X, Y Z)                                                        // PABLO #1916
    template<class E>                                                                                                   // PABLO #1916
    static Position _2Position(const E* const data) {                                                                   // PABLO #1916
        if (data == 0 || data[0] == 0) {                                                                                // PABLO #1916
            throw EmptyData();                                                                                          // PABLO #1916
        }                                                                                                               // PABLO #1916
        std::string s = _2str(data);                                                                                    // PABLO #1916
        std::string x, y, z;                                                                                            // PABLO #1916
        while(!s.empty() && s.back() != ' ') {                                                                          // PABLO #1916
            if((s.back() >= '0' && s.back() <= '9') || (s.back() == '-') || (s.back() == '+') || (s.back() == '.')) {   // PABLO #1916
                z.push_back(s.back());                                                                                  // PABLO #1916
                s.pop_back();                                                                                           // PABLO #1916
            }                                                                                                           // PABLO #1916
            else                                                                                                        // PABLO #1916
                throw PositionFormatException();                                                                        // PABLO #1916
        }                                                                                                               // PABLO #1916
        s.pop_back();                                                                                                   // PABLO #1916
        while(!s.empty() && s.back() != ' ') {                                                                          // PABLO #1916
            if((s.back() >= '0' && s.back() <= '9') || (s.back() == '-') || (s.back() == '+') || (s.back() == '.')) {   // PABLO #1916
                y.push_back(s.back());                                                                                  // PABLO #1916
                s.pop_back();                                                                                           // PABLO #1916
            }                                                                                                           // PABLO #1916
            else                                                                                                        // PABLO #1916
                throw PositionFormatException();                                                                        // PABLO #1916
        }                                                                                                               // PABLO #1916
        s.pop_back();                                                                                                   // PABLO #1916
        while(!s.empty() && s.back() != ' ') {                                                                          // PABLO #1916
            if((s.back() >= '0' && s.back() <= '9') || (s.back() == '-') || (s.back() == '+') || (s.back() == '.')) {   // PABLO #1916
                x.push_back(s.back());                                                                                  // PABLO #1916
                s.pop_back();                                                                                           // PABLO #1916
            }                                                                                                           // PABLO #1916
            else                                                                                                        // PABLO #1916
                throw PositionFormatException();                                                                        // PABLO #1916
        }                                                                                                               // PABLO #1916
        std::reverse(x.begin(), x.end());                                                                               // PABLO #1916
        std::reverse(y.begin(), y.end());                                                                               // PABLO #1916
        std::reverse(z.begin(), z.end());                                                                               // PABLO #1916
        return Position(_str2SUMOReal(x), _str2SUMOReal(y), _str2SUMOReal(z));                                          // PABLO #1916
    }                                                                                                                   // PABLO #1916

    /** convert a string into Position in the three axis (X, Y Z)       // PABLO #1916
    by calling the Position-type converter, which                       // PABLO #1916
    throws an EmptyData - exception if the given string is empty        // PABLO #1916
    throws a PositionFormatException - exception when the string does   // PABLO #1916
    not contain a Position */                                           // PABLO #1916
    static Position _str2Position(const std::string& sData) {           // PABLO #1916
        return _2Position(sData.c_str());                               // PABLO #1916
    }                                                                   // PABLO #1916

    // conversion methods not throwing an exception
    /** converts a 0-terminated char-type array into std::string
        returns the default value if the data is empty */
    template<class E>
    static std::string _2strSec(const E* const data,
                                const std::string& def) {
        return _2strSec(data, getLength(data), def);
    }


    /** converts a 0-terminated char-type array into the integer value
            described by it
        returns the default value if the data is empty */
    template<class E>
    static int _2intSec(const E* const data, int def) {
        if (data == 0 || data[0] == 0) {
            return def;
        }
        return _2int(data);
    }


    /** converts a 0-terminated char-type array into the long value
            described by it
        returns the default value if the data is empty */
    template<class E>
    static long long int _2longSec(const E* const data, long def) {
        if (data == 0 || data[0] == 0) {
            return def;
        }
        return _2long(data);
    }


    /** converts a 0-terminated char-type array into the SUMOReal value
            described by it
        returns the default value if the data is empty */
    template<class E>
    static SUMOReal _2SUMORealSec(const E* const data, SUMOReal def) {
        if (data == 0 || data[0] == 0) {
            return def;
        }
        return _2SUMOReal(data);
    }


    /** converts a 0-terminated char-type array into the SUMOReal value
            described by it
        returns true if the data* is one of the following (case insensitive):
            '1', 'x', 'true', 'yes', 'on'
        returns false if the data* is one of the following (case insensitive):
            '0', '-', 'false', 'no', 'off'
        returns the default value if the data is empty */
    template<class E>
    static bool _2boolSec(const E* const data, bool def) {
        if (data == 0 || data[0] == 0) {
            return def;
        }
        return _2bool(data);
    }


    /** converts a char-type array into std::string considering
            the given length
        returns the default value if the data is empty */
    template<class E>
    static std::string _2strSec(const E* const data, int length,
                                const std::string& def) {
        if (data == 0 || length == 0) {
            return def;
        }
        return _2str(data, length);
    }


    /** returns the length of the string (the position of the 0-character) */
    template<class E>
    static unsigned getLength(const E* const data) {
        if (data == 0) {
            return 0;
        }
        unsigned i = 0;
        while (data[i] != 0) {
            i++;
        }
        return i;
    }

};


#endif

/****************************************************************************/

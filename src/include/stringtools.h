/***************************************************************************
                          stringtools.h  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001-2023 by Andre Simon
    email                : a.simon@mailbox.org
 ***************************************************************************/


/*
This file is part of Highlight.

Highlight is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Highlight is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Highlight.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <string>
#include <vector>
#include <sstream>

/// Contains methods for string manipulation

namespace StringTools
{

/** Change Keyword case */
enum KeywordCase {
    CASE_UNCHANGED, ///< do not alter case
    CASE_LOWER,     ///< convert to lower case
    CASE_UPPER,     ///< convert to upper case
    CASE_CAPITALIZE ///< convert first character to upper case
};

/** Change character case of strings
    \param s input string
    \param kcase case modification indicator
    \return modified string
*/
std::string change_case ( const std::string & s,
                     const KeywordCase kcase = CASE_LOWER ) throw();

/** Trim string (remove whitespace)
   \param value String
   \return string trimmed on the right
*/
std::string trimRight ( const std::string &value );

std::string trim(const std::string& s);

/** Split string and return items separated by a delimiter
    \param s string containing tokens
    \param delim Token delimiter
    \return vector containing found tokens */
std::vector<std::string> splitString ( const std::string& s, unsigned char delim );

/** Convert string to a numeric value of the given type
    \param val variable of specified type which will contain the numeric value
    \param s string containing a number
    \param f format specifier function (IO manipulator)
    \return true if successful */
template <class T>
bool str2num ( T &val, const std::string& s, std::ios_base& ( *f ) ( std::ios_base& ) )
{
    std::istringstream iss ( s );
    return ! ( iss >> f >> val ).fail();
}

int calcWeight(const std::string&s);

std::string getPathAcronym(const std::string&path, char delim);

bool endsWith(std::string const & value, std::string const & ending);

int utf8_strlen(const std::string& str);
}

#endif

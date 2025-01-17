/***************************************************************************
                        PreFormatter.cpp  -  description
                             -------------------
    begin                : Mo Jan 03 2005
    copyright            : (C) 2005-2023 by Andre Simon
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


#ifndef PreFormatter_H
#define PreFormatter_H

#include <string>
#include <set>

namespace highlight
{

/** \brief Class which provides intelligent line wrapping.
* @author Andre Simon
*/

class PreFormatter
{
public:

    PreFormatter();

    ~PreFormatter();

    /**
     Set wrapping mode
     \param wrap set to true if long lines should be wrapped
    */
    void setWrap ( bool wrap )
    {
        wrapLines = wrap;
    }

    /**
     Replace tabs by spaces
     \param replTabs set to true if tabs should be replaced by spaces
    */
    void setReplaceTabs ( bool replTabs )
    {
        replaceTabs = replTabs;
    }

    /**
     \return True if current line can be wrapped again
    */
    bool hasMoreLines();

    /**
     Sets new line to be wrapped
     \param newline New line
    */
    void setLine ( const std::string & newline );

    /**
     The method will indent function calls and statements
     \return Next line
    */
    std::string getNextLine();

    /**
     \return True if lines following open braces should be indented
    */
    bool indentCode();

    /**
      Maximum line length
      \param maxlength max. length of output lines
    */
    void setWrapLineLength ( unsigned int maxlength );

    /**
      Indentation mode
      \param indentAfterOpenBraces set true if lines should be indented after braces
    */
    void setWrapIndentBraces ( bool indentAfterOpenBraces=true );

    /**
      Number of spaces
      \param num number of spaces which replace a tab
    */
    void setNumberSpaces ( unsigned int num );

    /**
      \return true if preformatting is enabled
    */
    bool isEnabled()
    {
        return wrapLines || replaceTabs;
    }

    bool getReplaceTabs()
    {
        return replaceTabs;
    }

    /**
      reset preformatting state to use the object with new input data
    */
    void reset ()
    {
        lineNumber=0;
        wrappedLines.clear();
    }

    /**
     \param lineNumber line number
      \return true if input line linenumber was wrapped
    */
    bool isWrappedLine ( int lineNumber )
    {
        return wrappedLines.count ( lineNumber );
    }

private:

    unsigned int maxLineLength{ 80 };

    std::string line, wsPrefix;
    unsigned int index{ 0 };
    unsigned int numberSpaces{ 0 };
    unsigned int lineNumber{ 0 };
    size_t wsPrefixLength;
    bool hasMore{ false }, indentAfterOpenBraces{ true };
    bool redefineWsPrefix{ false };
    bool wrapLines{ false }, replaceTabs{ false };

    std::set<int> wrappedLines;

    static const std::string LB_CHARS;
    static const std::string WS_CHARS;
    static const std::string INDENT_MARKERS;

};

}

#endif

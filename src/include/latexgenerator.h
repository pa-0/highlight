/***************************************************************************
                          latexgenerator.h  -  description
                             -------------------
    begin                : Mit Jul 24 2002
    copyright            : (C) 2002-2023 by Andre Simon
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


#ifndef LATEXGENERATOR_H
#define LATEXGENERATOR_H

#include <string>
#include <iostream>
#include <sstream>

#include "codegenerator.h"
#include "version.h"
#include "charcodes.h"


namespace highlight
{

/**
   \brief This class generates LaTeX.

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class LatexGenerator : public highlight::CodeGenerator
{
public:
    LatexGenerator();
    ~LatexGenerator();

    /** set replace quotes flag
       \param b flag
    */
    void setLATEXReplaceQuotes ( bool b )
    {
        replaceQuotes = b;
    }

    /** set disable babel shorthand flag
       \param b flag
    */
    void setLATEXNoShorthands ( bool b )
    {
        disableBabelShortHand = b;
    }

    /** set pretty symbols flag
       \param b flag
    */
    void setLATEXPrettySymbols ( bool b )
    {
        prettySymbols = b;
    }

    /** set Beamer mode  flag
       \param b flag
    */
    void setLATEXBeamerMode ( bool b )
    {
       beamerMode = b;
       newLineTag = beamerMode ? "\n\n" : "\\\\\n";
       longLineTag = "\\hspace*{\\fill}" + newLineTag;
    }

private:

    /** prints document header
     */
    std::string getHeader();

    /** Prints document footer*/
    std::string getFooter();

    /** Prints document body*/
    void printBody();

    /** initialize tags in specific format according to colouring information provided in DocumentStyle */
    void initOutputTags();

    std::string styleDefinitionCache;
    std::string longLineTag;

    /** \return escaped character*/
    virtual std::string maskCharacter ( unsigned char );

    /**\return text formatting attributes in LaTeX format */
    std::string getAttributes ( const std::string & elemName,
                                const ElementStyle & elem );

    /** test if double quotes should be replaced by \dq{} */
    bool replaceQuotes{ false };

    /** test if Babel shorthand for " should be disabled */
    bool disableBabelShortHand{ false };

    /** test if symbols like <,>,{,},~ should be replaced by nicer definitions */
    bool prettySymbols{ false };

    bool beamerMode{ false };

    std::string getNewLine();

    std::string getStyleDefinition();

    std::string getKeywordOpenTag ( unsigned int styleID );
    std::string getKeywordCloseTag ( unsigned int styleID );
};

}

#endif

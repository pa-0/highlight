/***************************************************************************
                          TexGenerator.h  -  description
                             -------------------
    begin                : Mit Jul 24 2002
    copyright            : (C) 2002-2023 by Andrï¿½Simon
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


#ifndef TEXGENERATOR_H
#define TEXGENERATOR_H

#include <string>

#include "codegenerator.h"


namespace highlight
{

/**
   \brief This class generates TeX.

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class TexGenerator : public highlight::CodeGenerator
{
public:

    TexGenerator();
    ~TexGenerator();

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

    std::string getStyleDefinition();

    /** \return escaped character*/
    virtual std::string maskCharacter ( unsigned char );

    /**\return text formatting attributes in RTF format */
    std::string getAttributes ( const std::string & elemName, const ElementStyle & elem );

    /** @param styleID current style ID
        @return matching sequence to begin a new element formatting*/
    std::string getKeywordOpenTag ( unsigned int styleID );

    /** @param styleID current style ID
        @return matching  sequence to stop element formatting*/
    std::string getKeywordCloseTag ( unsigned int styleID );

};

}

#endif

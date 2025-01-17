/***************************************************************************
                          elementstyle.h  -  description
                             -------------------
    begin                : Son Nov 10 2002
    copyright            : (C) 2002-2024 by Andre Simon
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


#ifndef ELEMENTSTYLE_H
#define ELEMENTSTYLE_H

#include <string_view>


#include "stylecolour.h"

namespace highlight
{

/** \brief The class stores the basic text formatting properties.

* @author Andre Simon
*/

class ElementStyle
{
public:

    /** Constructor
        \param col Style colour
        \param b Bold flag
        \param i Italic flag
        \param u Underline flag */
    ElementStyle ( const Colour& col, bool b, bool i, bool u );

    /** Constructor
         \param elementStyleString String with formatting information (eg "00 aa ff bold") */
    ElementStyle ( const std::string & elementStyleString );

    /** Constructor */
    ElementStyle();

    /**copy constructor */
    ElementStyle ( const ElementStyle &other )
    {
        colour = other.getColour();
        bold = other.isBold();
        italic = other.isItalic();
        underline = other.isUnderline();
        customOverride = other.getCustomOverride();
        customAttribute = other.getCustomAttribute();
    }

    /** operator overloading */
    ElementStyle& operator= ( const ElementStyle &other )
    {
        colour = other.getColour();
        bold = other.isBold();
        italic = other.isItalic();
        underline = other.isUnderline();
        customOverride = other.getCustomOverride();
        customAttribute = other.getCustomAttribute();

        return *this;
    }

    ~ElementStyle();

    /** initialize object
        \param elementStyleString String which contains formatting attributes
               (Format: "color attr" where
                color can be HTML hex notation or a hex RGB tuple (ie "#2244ff" or "22 44 ff")
                attr can be a combination of "italic, "bold" and "underline")
    */
    void set ( const std::string &elementStyleString );

    /** \return True if italic */
    bool isItalic() const;

    /** \return True if bold */
    bool isBold() const;

    /** \return True if underline */
    bool isUnderline() const;

    /** \param b set italic flag */
    void setItalic ( bool b );

    /** \param b set bold flag */
    void setBold ( bool b );

    /** \param b set underline flag */
    void setUnderline ( bool b );

    /** \return Element colour */
    Colour getColour() const;

    /** \param col colour of this element */
    void setColour (const Colour& col );

    std::string getCustomAttribute() const;

    [[deprecated("Replaced by getCustomAttribute")]]
    std::string getCustomStyle() const {
        return getCustomAttribute();
    }

    void setCustomAttribute( std::string_view style);

    [[deprecated("Replaced by setCustomAttribute")]]
    void setCustomStyle( std::string_view style){
        setCustomAttribute(style);
    }

    bool getCustomOverride() const;

    void setCustomOverride(bool override);

private:
    Colour colour;
    bool bold, italic, underline, customOverride;
    std::string customAttribute;
};

}

#endif

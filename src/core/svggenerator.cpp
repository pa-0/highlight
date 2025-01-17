/***************************************************************************
                     svggenerator.cpp  -  description
                             -------------------
    begin                : Mo 23.06.2008
    copyright            : (C) 2008-2023 by Andre Simon
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


#include <string>
#include <sstream>

#include "version.h"
#include "svggenerator.h"

using std::ostringstream;
using std::string;

namespace highlight
{

SVGGenerator::SVGGenerator()
    : CodeGenerator ( SVG )
{
    spacer = initialSpacer = "&#160;";
    maskWs=true;
    newLineTag = "\n";
    styleCommentOpen="/*";
    styleCommentClose="*/";
}

SVGGenerator::~SVGGenerator() {}

void SVGGenerator::initOutputTags()
{
    openTags.emplace_back("" );
    openTags.push_back ( getOpenTag ( STY_NAME_STR ) );
    openTags.push_back ( getOpenTag ( STY_NAME_NUM ) );
    openTags.push_back ( getOpenTag ( STY_NAME_SLC ) );
    openTags.push_back ( getOpenTag ( STY_NAME_COM ) );
    openTags.push_back ( getOpenTag ( STY_NAME_ESC ) );
    openTags.push_back ( getOpenTag ( STY_NAME_DIR ) );
    openTags.push_back ( getOpenTag ( STY_NAME_DST ) );
    openTags.push_back ( getOpenTag ( STY_NAME_LIN ) );
    openTags.push_back ( getOpenTag ( STY_NAME_SYM ) );
    openTags.push_back ( getOpenTag ( STY_NAME_IPL ) );
    openTags.push_back ( getOpenTag ( STY_NAME_ERR ) );
    openTags.push_back ( getOpenTag ( STY_NAME_ERM ) );

    closeTags.emplace_back("" );
    for (unsigned int i=1; i<NUMBER_BUILTIN_STATES; i++ ) {
        closeTags.emplace_back("</tspan>" );
    }
}

string SVGGenerator::getStyleDefinition()
{
    if (disableStyleCache || styleDefinitionCache.empty() ) {
        ostringstream os;
        string tspan("tspan.");
        if ( includeStyleDef ) {
            os << "<style type=\"text/css\">\n"
               << "<![CDATA[\n";
        }
        os << "rect { fill:#"
           << ( docStyle.getBgColour().getRed ( SVG ) )
           << ( docStyle.getBgColour().getGreen ( SVG ) )
           << ( docStyle.getBgColour().getBlue ( SVG ) )
           << "; } \n";
        os << "g { font-size: " << getBaseFontSize();
        os << "; font-family: " << getBaseFont() << "; white-space: pre;}\n";
        os << getAttributes ( "text", docStyle.getDefaultStyle() )
           << getAttributes ( tspan+STY_NAME_NUM, docStyle.getNumberStyle() )
           << getAttributes ( tspan+STY_NAME_ESC, docStyle.getEscapeCharStyle() )
           << getAttributes ( tspan+STY_NAME_STR, docStyle.getStringStyle() )
           << getAttributes ( tspan+STY_NAME_DST, docStyle.getPreProcStringStyle() )
           << getAttributes ( tspan+STY_NAME_SLC, docStyle.getSingleLineCommentStyle() )
           << getAttributes ( tspan+STY_NAME_COM, docStyle.getCommentStyle() )
           << getAttributes ( tspan+STY_NAME_DIR, docStyle.getPreProcessorStyle() )
           << getAttributes ( tspan+STY_NAME_SYM, docStyle.getOperatorStyle() )
           << getAttributes ( tspan+STY_NAME_IPL, docStyle.getInterpolationStyle() )
           << getAttributes ( tspan+STY_NAME_LIN, docStyle.getLineStyle() );

           os << getAttributes ( tspan+STY_NAME_ERM, docStyle.getErrorMessageStyle() );
           os << getAttributes ( tspan+STY_NAME_ERR, docStyle.getErrorStyle() );

        for ( const auto &[first, second] : docStyle.getKeywordStyles() ) {
            os << getAttributes ( tspan+first, second );
        }

        if ( includeStyleDef ) {
            os << "]]>\n"
               << "</style>";
        }
        styleDefinitionCache=os.str();
    }
    return styleDefinitionCache;
}


string SVGGenerator::getAttributes ( const string & elemName,
                                     const ElementStyle & elem )
{
    ostringstream s;

    if ( !elemName.empty() ) {
        s << elemName << " { ";
    }

    if (!elem.getCustomOverride()) {
        s   << "fill:#"
            << ( elem.getColour().getRed ( HTML ) )
            << ( elem.getColour().getGreen ( HTML ) )
            << ( elem.getColour().getBlue ( HTML ) )
            << ( elem.isBold() ?     "; font-weight:bold" :"" )
            << ( elem.isItalic() ?   "; font-style:italic" :"" )
            << ( elem.isUnderline() ? "; text-decoration:underline" :"" );
    }

    string customStyle(elem.getCustomAttribute());

    if (!customStyle.empty()) {
        if (!elem.getCustomOverride()) {
            s << "; ";
        }
        s << customStyle;
    }

    if ( !elemName.empty() ) {
        s << "; }\n" ;
    }
    return s.str();
}

string  SVGGenerator::getOpenTag ( const string& styleName )
{
    return "<tspan class=\""+styleName+"\">";
}

string SVGGenerator::getHeader()
{
    ostringstream header;
    header << "<?xml version=\"1.0\"";
    if ( encodingDefined() ) {
        header << " encoding=\"" << encoding << "\"";
    }
    header << "?>\n";
    if ( !includeStyleDef ) {
        header  << R"(<?xml-stylesheet type="text/css" href=")"
                << getStyleOutputPath()
                << "\"?>\n";
    }
    header << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.2//EN\" "
           << "\"http://www.w3.org/Graphics/SVG/1.2/DTD/svg12.dtd\">\n";
    header << R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.2" )"
           << R"(baseProfile="full" xml:space="preserve")";
    if ( width.size() ) header << " width=\""<<width<<"\"";
    if ( height.size() ) header << " height=\""<<height<<"\"";
    //viewBox=\"0 0 800 600\"
    header << ">\n<desc>" << docTitle << "</desc>\n";
    if ( includeStyleDef ) {
        header << "<defs>\n";
        header << getStyleDefinition();
        header << "\n</defs>\n";
    }
    return header.str();
}

void SVGGenerator::printBody()
{
    *out << "<g>\n<rect x=\"0\" y=\"0\" width=\"100%\" height=\"100%\"/>"; // rect: background color
    int fontSize=0;
    StringTools::str2num<int> ( fontSize, getBaseFontSize(), std::dec );
    *out << "\n<text x=\"10\" y=\""<<fontSize*2<<"\">";
    processRootState();
    *out << "</text>\n</g>\n";
}


string SVGGenerator::getFooter()
{
    ostringstream os;
    os <<"</svg>\n";

    if (!omitVersionComment) {
        os  << "<!-- SVG generated by Highlight "
            << highlight::Info::getVersion()
            << ", "
            << highlight::Info::getWebsite()
            <<" -->\n";
    }
    return os.str();
}

string SVGGenerator::maskCharacter ( unsigned  char c )
{
    switch ( c ) {
    case ' ' :
        return spacer;
        break;
    case '<' :
        return "&lt;";
        break;
    case '>' :
        return "&gt;";
        break;
    case '&' :
        return "&amp;";
        break;
    case '\"' :
        return "&quot;";
        break;
    default:
        return string ( 1, c );
    }
}

string SVGGenerator::getKeywordOpenTag ( unsigned int styleID )
{
    return getOpenTag ( currentSyntax->getKeywordClasses() [styleID] );
}

string SVGGenerator::getKeywordCloseTag ( unsigned int styleID )
{
    return "</tspan>";
}

string SVGGenerator::getNewLine()
{
    ostringstream ss;

    printSyntaxError(ss);

    if ( lineNumber>1 ) {
        int fontSize=0;
        StringTools::str2num<int> ( fontSize, getBaseFontSize(), std::dec );
        ss<< "</text>\n<text x=\"10\" y=\""<< ( lineNumber*fontSize*2 ) <<"\">";
    }
    return ss.str();
}

void SVGGenerator::setSVGSize ( const string& w, const string& h )
{
    width=w;
    height=h;
}

}

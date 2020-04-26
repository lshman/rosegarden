/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A sequencer and musical notation editor.
    Copyright 2000-2018 the Rosegarden development team.
    See the AUTHORS file for more details.

    This file is Copyright 2003
        Mark Hymers         <markh@linuxfromscratch.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef RG_COLOURMAP_H
#define RG_COLOURMAP_H

#include <QColor>

#include <utility>
#include <map>
#include <string>

// Default Segment color.
#define COLOUR_DEF_R 255
#define COLOUR_DEF_G 234
#define COLOUR_DEF_B 182 

namespace Rosegarden 
{


/**
 * ColourMap is our table which maps the unsigned integer keys stored in
 *  segments to both a QColor and a String containing the 'name'
 */

class ColourMap
{
public:
    // Functions:

    /**
     * Initialises an ColourMap with a default element set to
     * whatever COLOUR_DEF_X defines the colour to be (see the source file)
     */
    ColourMap();
    /**
     * Initialises an ColourMap with a default element set to
     * the value of the QColor passed in.
     */
    ColourMap(const QColor& input);
    ~ColourMap();

    /**
     * Returns the QColor associated with the item_num passed in.  Note that
     * if the item_num doesn't represent a valid item, the routine returns
     * the value of the Default colour.  This means that if somehow some of
     * the Segments get out of sync with the ColourMap and have invalid
     * colour values, they'll be set to the Composition default colour.
     */
    QColor getColourByIndex(unsigned int item_num) const;

    /**
     * Returns the string associated with the item_num passed in.  If the
     * item_num doesn't exist, it'll return "" (the same name as the default
     * colour has - for internationalization reasons).
     */
    std::string getNameByIndex(unsigned int item_num) const;

    /**
     * If item_num exists, this routine deletes it from the map.
     */
    bool deleteItemByIndex(unsigned int item_num);

    /**
     * This routine adds a Colour using the lowest possible index.
     */
    bool addItem(QColor colour, std::string name);

    /**
     * This routine adds a Colour using the given id.  ONLY FOR USE IN
     * rosexmlhandler.cpp
     */
    bool addItem(QColor colour, std::string name, unsigned int id);

    /**
     * If the item with item_num exists and isn't the default, this
     * routine modifies the string associated with it
     */
    bool modifyNameByIndex(unsigned int item_num, std::string name);

    /**
     * If the item with item_num exists, this routine modifies the 
     * Colour associated with it
     */
    bool modifyColourByIndex(unsigned int item_num, QColor colour);

    /**
     * If both items exist, swap them.  
     */
    bool swapItems(unsigned int item_1, unsigned int item_2);

//    void replace(ColourMap &input);

    typedef std::map<unsigned /* item_num */,
                     std::pair<QColor, std::string /* name */> > MapType;

    std::string toXmlString(std::string name) const;

    MapType colours;

private:
    // unused
    MapType::const_iterator begin();
    MapType::const_iterator end();
    unsigned int size() const;

};

}

#endif

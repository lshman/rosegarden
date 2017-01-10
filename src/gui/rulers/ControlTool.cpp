/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A MIDI and audio sequencer and musical notation editor.
    Copyright 2000-2017 the Rosegarden development team.

    Other copyrights also apply to some parts of this work.  Please
    see the AUTHORS file and individual file headers for details.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/


#include "ControlTool.h"
#include "misc/Debug.h"
#include "misc/Strings.h"

#include "ControlRuler.h"
#include "ControlItem.h"
#include "ControlMouseEvent.h"
#include <iostream>

namespace Rosegarden
{

ControlTool::ControlTool(QString /* rcFileName */, QString menuName, ControlRuler *ruler) :
    BaseTool(menuName, ruler),
    m_ruler(ruler),
    m_overItem(false)
{
}

void
ControlTool::createMenu()
{
}

}

/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */
/*
  Rosegarden
  A sequencer and musical notation editor.
  Copyright 2020 the Rosegarden development team.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.  See the file
  COPYING included with this distribution for more information.
*/

#pragma once


#include "base/MidiProgram.h"  // For MidiByte


namespace Rosegarden
{


class MappedEvent;


/// Support for the Korg nanoKONTROL2 control surface.
/**
 * This only supports the nanoKONTROL2 in "CC" mode.  To reset the nanoKONTROL2
 * to factory defaults (removing any custom programming) and switch to CC
 * mode:
 *
 *   Hold down the PREV TRACK, NEXT TRACK, and CYCLE buttons and then
 *   connect the nanoKONTROL2 to USB.
 */
class KorgNanoKontrol2
{
public:
    KorgNanoKontrol2();

    /// Handle MappedEvent's from the external controller port.
    void processEvent(const MappedEvent *event);

private:
    // Current 8-channel page.
    unsigned m_page;

    void processFader(MidiByte controlNumber, MidiByte value);
    void processKnob(MidiByte controlNumber, MidiByte value);
    void processSolo(MidiByte controlNumber);
    void processMute(MidiByte controlNumber);
    void processRecord(MidiByte controlNumber);

};


}

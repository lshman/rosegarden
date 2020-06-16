/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A sequencer and musical notation editor.
    Copyright 2000-2020 the Rosegarden development team.
    See the AUTHORS file for more details.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef RG_MIDITYPES_H
#define RG_MIDITYPES_H

#include "Exception.h"
#include "MidiProgram.h"  // For MidiByte
#include "PropertyName.h"
#include "TimeT.h"

#include <string>

// Internal representation of some very MIDI-specific event types
// that fall clearly outside of NotationTypes and still require
// representation.


namespace Rosegarden 
{


class Event;


class MIDIValueOutOfRange : public Exception {
public:
    MIDIValueOutOfRange(std::string name) :
        Exception("Value of " + name + " out of byte range") { }
    MIDIValueOutOfRange(std::string name, std::string file, int line) :
        Exception("Value of " + name + " out of byte range", file, line) { }
};

//////////////////////////////////////////////////////////////////////

namespace PitchBend
{
    extern const std::string EventType;
    constexpr int EventSubOrdering = -5;

    extern const PropertyName MSB;
    extern const PropertyName LSB;

    /// Returned Event is on heap; caller takes responsibility for ownership.
    Event *makeEvent(timeT absoluteTime, MidiByte msb, MidiByte lsb);
}

//////////////////////////////////////////////////////////////////////

namespace Controller
{
    extern const std::string EventType;
    constexpr int EventSubOrdering = -5;

    extern const PropertyName NUMBER;
    extern const PropertyName VALUE;

    /// Returned Event is on heap; caller takes responsibility for ownership.
    Event *makeEvent(timeT absoluteTime, MidiByte number, MidiByte value);
}

//////////////////////////////////////////////////////////////////////

namespace KeyPressure
{
    extern const std::string EventType;
    constexpr int EventSubOrdering = -5;

    extern const PropertyName PITCH;
    extern const PropertyName PRESSURE;

    /// Returned event is on heap; caller takes responsibility for ownership
    Event *makeEvent(timeT absoluteTime, MidiByte pitch, MidiByte pressure);
}

//////////////////////////////////////////////////////////////////////

class ChannelPressure
{
public:
    static const std::string EventType;
    static const int EventSubOrdering;

    static const PropertyName PRESSURE;

    /// Event -> Data
    ChannelPressure(const Event &event);
    MidiByte getPressure() const { return m_pressure; }

    /// Data -> Event
    ChannelPressure(MidiByte pressure);
    /// Returned event is on heap; caller takes responsibility for ownership
    Event *getAsEvent(timeT absoluteTime) const;

private:
    MidiByte m_pressure;
};

//////////////////////////////////////////////////////////////////////

class ProgramChange
{
public:
    static const std::string EventType;
    static const int EventSubOrdering;

    static const PropertyName PROGRAM;

    /// Event -> Data
    ProgramChange(const Event &event);
    MidiByte getProgram() const { return m_program; }

    /// Data -> Event
    ProgramChange(MidiByte program);
    /// Returned event is on heap; caller takes responsibility for ownership
    Event *getAsEvent(timeT absoluteTime) const;

private:
    MidiByte m_program;
};

//////////////////////////////////////////////////////////////////////

class SystemExclusive
{
public:
    static const std::string EventType;
    static const int EventSubOrdering;

    struct BadEncoding : public Exception {
        BadEncoding() : Exception("Bad SysEx encoding") { }
    };

    static const PropertyName DATABLOCK;

    // Event -> Data
    SystemExclusive(const Event &event);
    // Playback (MappedEvent) uses this.
    // SimpleEventEditDialog uses this for the data and the length.
    std::string getRawData() const { return m_rawData; }
    // SimpleEventEditDialog uses this.
    std::string getHexData() const { return toHex(m_rawData); }

    // Data -> Event
    SystemExclusive(std::string rawData);
    /// Returned event is on heap; caller takes responsibility for ownership
    Event *getAsEvent(timeT absoluteTime) const;

    static std::string toHex(std::string rawData);

private:
    std::string m_rawData;
};


}


#endif

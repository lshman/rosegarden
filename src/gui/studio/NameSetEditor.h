/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*- vi:set ts=8 sts=4 sw=4: */

/*
    Rosegarden
    A MIDI and audio sequencer and musical notation editor.
    Copyright 2000-2018 the Rosegarden development team.

    Other copyrights also apply to some parts of this work.  Please
    see the AUTHORS file and individual file headers for details.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef RG_NAMESETEDITOR_H
#define RG_NAMESETEDITOR_H

#include "gui/widgets/LineEdit.h"

#include <QGroupBox>
#include <QString>
#include <QStringList>

class QFrame;
class QGridLayout;
class QLabel;
class QPushButton;
class QToolButton;
class QTreeWidgetItem;

#include <vector>


namespace Rosegarden
{

class BankEditorDialog;

/// A tabbed editor for large lists of names.
/**
 * MidiProgramsEditor and MidiKeyMappingEditor derive from this to provide
 * the program and key mapping lists in the BankEditorDialog.
 */
class NameSetEditor : public QGroupBox
{
    Q_OBJECT

public slots:
    /// Handler for changes to text in any of the line edit widgets.
    /**
     * Connected to LineEdit::textChanged() for all line edits.
     *
     * This is virtual because NameSetEditor does the connect and wants
     * the derived version.
     */
    virtual void slotNameChanged(const QString &) = 0;

    /// Handler for presses of any of the key map buttons.
    /**
     * Connected to QToolButton::clicked() for all key map buttons.
     *
     * This is virtual because NameSetEditor does the connect and wants
     * the derived version.
     */
    virtual void slotKeyMapButtonPressed() = 0;

    /// Handler for presses of the numbering base (0/1) button.
    /**
     * Connected to QPushButton::clicked() for the numbering base button.
     *
     * ??? rename: slotToggleNumberingBase()
     */
    void slotToggleInitialLabel();

protected:
    NameSetEditor(BankEditorDialog *bankEditor,
                  QString title,
                  QWidget *parent,
                  const char *name,
                  QString headingPrefix = "",
                  bool showKeyMapButtons = false);

    QToolButton *getKeyMapButton(int n) { return m_keyMapButtons[n]; }
    const QToolButton *getKeyMapButton(int n) const { return m_keyMapButtons[n]; }

    /// Parent
    BankEditorDialog *m_bankEditor;

    QFrame *m_mainFrame;
    QGridLayout *m_mainLayout;

    QLabel *m_librarian;
    QLabel *m_librarianEmail;

    std::vector<LineEdit *> m_names;
    QStringList m_completions;

private:
    // ??? rename: m_numberingBase
    QPushButton *m_initialLabel;

    /// Numbers to the left of each line edit.
    std::vector<QLabel *> m_labels;
    /// Key map buttons between the labels and the line edits.
    std::vector<QToolButton *> m_keyMapButtons;
};


}

#endif

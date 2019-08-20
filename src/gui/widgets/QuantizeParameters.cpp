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

#define RG_MODULE_STRING "[QuantizeParameters]"

#include "QuantizeParameters.h"

#include "misc/Debug.h"
#include "misc/Strings.h"  // qStrToBool() etc...
#include "misc/ConfigGroups.h"  // *ConfigGroup names
#include "base/Quantizer.h"
#include "base/BasicQuantizer.h"
#include "base/LegatoQuantizer.h"
#include "base/NotationQuantizer.h"
#include "gui/editors/notation/NotationStrings.h"
#include "gui/editors/notation/NotePixmapFactory.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>


namespace Rosegarden
{


QuantizeParameters::QuantizeParameters(QWidget *parent,
                                       QuantizerType defaultQuantizer,
                                       bool showNotationOption) :
        QFrame(parent),
        m_standardQuantizations(BasicQuantizer::getStandardQuantizations())
{
    const bool inNotation = (defaultQuantizer == Notation);

    m_settings.beginGroup(
            inNotation ? NotationQuantizeConfigGroup :
                         GridQuantizeConfigGroup);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setSpacing(5);
    setContentsMargins(5, 5, 5, 5);
    setLayout(m_mainLayout);

    // Quantizer box
    QGroupBox *quantizerBox = new QGroupBox(tr("Quantizer"));
    QGridLayout *qbLayout = new QGridLayout;
    quantizerBox->setLayout(qbLayout);
    m_mainLayout->addWidget(quantizerBox);

    qbLayout->addWidget(new QLabel(tr("Quantizer type:"), quantizerBox), 0, 0);
    m_quantizerType = new QComboBox(quantizerBox);
    m_quantizerType->addItem(tr("Grid quantizer"));
    m_quantizerType->addItem(tr("Legato quantizer"));
    m_quantizerType->addItem(tr("Heuristic notation quantizer"));
    qbLayout->addWidget(m_quantizerType, 0, 1);

    m_quantizeNotation = new QCheckBox(
            tr("Quantize for notation only (leave performance unchanged)"),
            quantizerBox);
    m_quantizeNotation->setChecked(qStrToBool(m_settings.value(
            "quantizenotationonly", inNotation)));
    qbLayout->addWidget(m_quantizeNotation, 1, 0, 1, 2);

    // ??? Always false.  Only caller always sets this to false.
    if (!showNotationOption)
        m_quantizeNotation->hide();


    // Notation parameters box
    m_notationBox = new QGroupBox( tr("Notation parameters"));
    QGridLayout *nbLayout = new QGridLayout;
    nbLayout->setSpacing(3);
    m_notationBox->setLayout(nbLayout);
    m_mainLayout->addWidget(m_notationBox);

    // Base grid unit
    nbLayout->addWidget(new QLabel(tr("Base grid unit:"), m_notationBox), 1, 0);
    m_notationBaseGridUnit = new QComboBox(m_notationBox);
    nbLayout->addWidget(m_notationBaseGridUnit, 1, 1);

    // Complexity
    nbLayout->addWidget(new QLabel(tr("Complexity:"), m_notationBox), 0, 0);
    m_complexity = new QComboBox(m_notationBox);
    m_complexity->addItem(tr("Very high"));
    m_complexity->addItem(tr("High"));
    m_complexity->addItem(tr("Normal"));
    m_complexity->addItem(tr("Low"));
    m_complexity->addItem(tr("Very low"));
    m_complexity->setCurrentIndex(m_settings.value(
            "quantizesimplicity", 13).toInt() - 11);
    nbLayout->addWidget(m_complexity, 0, 1);

    // Tuplet level
    nbLayout->addWidget(new QLabel(tr("Tuplet level:"), m_notationBox), 2, 0);
    m_tupletLevel = new QComboBox(m_notationBox);
    m_tupletLevel->addItem(tr("None"));
    m_tupletLevel->addItem(tr("2-in-the-time-of-3"));
    m_tupletLevel->addItem(tr("Triplet"));
    m_tupletLevel->addItem(tr("Any"));
    m_tupletLevel->setCurrentIndex(m_settings.value(
            "quantizemaxtuplet", 3).toInt() - 1);
    nbLayout->addWidget(m_tupletLevel, 2, 1);

    // Permit counterpoint
    m_permitCounterpoint = new QCheckBox(tr("Permit counterpoint"), m_notationBox);
    m_permitCounterpoint->setChecked(qStrToBool(m_settings.value(
            "quantizecounterpoint", "false" )));
    nbLayout->addWidget(m_permitCounterpoint, 3, 0, 1, 1);


    // Grid parameters box
    m_gridBox = new QGroupBox( tr("Grid parameters"));
    QGridLayout *gbLayout = new QGridLayout;
    gbLayout->setSpacing(3);
    m_gridBox->setLayout(gbLayout);
    m_mainLayout->addWidget(m_gridBox);

    // Base grid unit
    gbLayout->addWidget(new QLabel(tr("Base grid unit:"), m_gridBox), 0, 0);
    m_gridBaseGridUnit = new QComboBox(m_gridBox);
    gbLayout->addWidget(m_gridBaseGridUnit, 0, 1);

    // Swing
    m_swingLabel = new QLabel(tr("Swing:"), m_gridBox);
    gbLayout->addWidget(m_swingLabel, 1, 0);
    m_swing = new QComboBox(m_gridBox);
    gbLayout->addWidget(m_swing, 1, 1);

    // Iterative amount
    m_iterativeAmountLabel = new QLabel(tr("Iterative amount:"), m_gridBox);
    gbLayout->addWidget(m_iterativeAmountLabel, 2, 0);
    m_iterativeAmount = new QComboBox(m_gridBox);
    gbLayout->addWidget(m_iterativeAmount, 2, 1);

    // Quantize durations
    m_quantizeDurations = new QCheckBox(
            tr("Quantize durations as well as start times"), m_gridBox);
    m_quantizeDurations->setChecked(qStrToBool(m_settings.value(
            "quantizedurations", "false")));
    gbLayout->addWidget(m_quantizeDurations, 3, 0, 1, 1);


    // After quantization box
    QGroupBox *afterQuantizationBox =
            new QGroupBox(tr("After quantization"), this);
    QGridLayout *pbLayout = new QGridLayout;
    pbLayout->setSpacing(3);
    afterQuantizationBox->setLayout(pbLayout);
    m_mainLayout->addWidget(afterQuantizationBox);

    // Re-beam
    m_rebeam = new QCheckBox(tr("Re-beam"), afterQuantizationBox);
    m_rebeam->setChecked(qStrToBool(m_settings.value(
            "quantizerebeam", "true")));

    // Add articulations
    m_addArticulations = new QCheckBox(
            tr("Add articulations (staccato, tenuto, slurs)"),
            afterQuantizationBox);
    m_addArticulations->setChecked(qStrToBool(m_settings.value(
            "quantizearticulate", "true")));

    // Tie notes at barlines
    m_tieNotesAtBarlines = new QCheckBox(
            tr("Tie notes at barlines etc"), afterQuantizationBox);
    m_tieNotesAtBarlines->setChecked(qStrToBool(m_settings.value(
            "quantizemakeviable", "false")));

    // Split-and-tie overlapping chords
    m_splitAndTie = new QCheckBox(
            tr("Split-and-tie overlapping chords"), afterQuantizationBox);
    m_splitAndTie->setChecked(qStrToBool(m_settings.value(
            "quantizedecounterpoint", "false")));

    pbLayout->addWidget(m_rebeam, 0, 0);
    pbLayout->addWidget(m_addArticulations, 1, 0);
    pbLayout->addWidget(m_tieNotesAtBarlines, 2, 0);
    pbLayout->addWidget(m_splitAndTie, 3, 0);


    // Initialize widgets

    // ??? Move these up closer to where the widgets are created.

    QPixmap noMap = NotePixmapFactory::makeToolbarPixmap("menu-no-note");

    timeT gridBaseGridUnit = m_settings.value(
            "gridBaseGridUnit",
            static_cast<int>(
                Note(Note::Demisemiquaver).getDuration())).toInt();
    timeT notationBaseGridUnit = m_settings.value(
            "notationBaseGridUnit",
            static_cast<int>(
                Note(Note::Demisemiquaver).getDuration())).toInt();

    // For each standard quantization
    for (unsigned int i = 0; i < m_standardQuantizations.size(); ++i) {

        timeT time = m_standardQuantizations[i];
        timeT error = 0;

        QPixmap pmap = NotePixmapFactory::makeNoteMenuPixmap(time, error);
        QString label;
        if (error == 0)
            label = NotationStrings::makeNoteMenuLabel(time, false, error);

        if (error == 0) {
            m_gridBaseGridUnit->addItem(pmap, label);
            m_notationBaseGridUnit->addItem(pmap, label);
        } else {
            // ??? We never end up in here since we are iterating through
            //     the standard quantizations.  We can probably remove this.
            m_gridBaseGridUnit->addItem(noMap, QString("%1").arg(time));
            m_notationBaseGridUnit->addItem(noMap, QString("%1").arg(time));
        }

        // Found it?  Select it.
        if (m_standardQuantizations[i] == gridBaseGridUnit) {
            m_gridBaseGridUnit->setCurrentIndex(
                    m_gridBaseGridUnit->count() - 1);
        }
        if (m_standardQuantizations[i] == notationBaseGridUnit) {
            m_notationBaseGridUnit->setCurrentIndex(
                    m_notationBaseGridUnit->count() - 1);
        }
    }

    int swing = m_settings.value("quantizeswing", 0).toInt();

    for (int i = -100; i <= 200; i += 10) {
        m_swing->addItem(i == 0 ? tr("None") : QString("%1%").arg(i));

        // Found it?  Select it.
        if (i == swing)
            m_swing->setCurrentIndex(m_swing->count() - 1);
    }

    int iterativeAmount = m_settings.value("quantizeiterate", 100).toInt();

    for (int i = 10; i <= 100; i += 10) {
        m_iterativeAmount->addItem(
                i == 100 ? tr("Full quantize") : QString("%1%").arg(i));

        // Found it?  Select it.
        if (i == iterativeAmount)
            m_iterativeAmount->setCurrentIndex(m_iterativeAmount->count() - 1);
    }

    int quantizerType = m_settings.value(
            "quantizetype",
            (defaultQuantizer == Notation) ? 2 :
                    (defaultQuantizer == Legato) ? 1 : 0).toInt();

    switch (quantizerType) {
    case 0:  // grid
        m_gridBox->show();
        m_swingLabel->show();
        m_swing->show();
        m_iterativeAmountLabel->show();
        m_iterativeAmount->show();
        m_notationBox->hide();
        m_quantizeDurations->show();
        m_quantizerType->setCurrentIndex(0);
        break;
    case 1:  // legato
        m_gridBox->show();
        m_swingLabel->hide();
        m_swing->hide();
        m_iterativeAmountLabel->hide();
        m_iterativeAmount->hide();
        m_notationBox->hide();
        m_quantizeDurations->hide();
        m_quantizerType->setCurrentIndex(1);
        break;
    case 2:  // notation
        m_gridBox->hide();
        m_notationBox->show();
        m_quantizerType->setCurrentIndex(2);
        break;
    }

    connect(m_quantizerType, SIGNAL(activated(int)), SLOT(slotTypeChanged(int)));

}

Quantizer *
QuantizeParameters::getQuantizer()
{
    //!!! Excessive duplication with
    // EventQuantizeCommand::makeQuantizer in editcommands.cpp

    int type = m_quantizerType->currentIndex();
    timeT unit = 0;

    if (type == 0 || type == 1) {
        unit = m_standardQuantizations[m_gridBaseGridUnit->currentIndex()];
    } else {
        unit = m_standardQuantizations[m_notationBaseGridUnit->currentIndex()];
    }

    Quantizer *quantizer = nullptr;

    int swing = m_swing->currentIndex();
    swing *= 10;
    swing -= 100;

    int iterate = m_iterativeAmount->currentIndex();
    iterate *= 10;
    iterate += 10;

    if (type == 0) {

        if (m_quantizeNotation->isChecked()) {
            quantizer = new BasicQuantizer
                        (Quantizer::RawEventData,
                         Quantizer::NotationPrefix,
                         unit, m_quantizeDurations->isChecked(),
                         swing, iterate);
        } else {
            quantizer = new BasicQuantizer
                        (Quantizer::RawEventData,
                         Quantizer::RawEventData,
                         unit, m_quantizeDurations->isChecked(),
                         swing, iterate);
        }
    } else if (type == 1) {
        if (m_quantizeNotation->isChecked()) {
            quantizer = new LegatoQuantizer
                        (Quantizer::RawEventData,
                         Quantizer::NotationPrefix, unit);
        } else {
            quantizer = new LegatoQuantizer
                        (Quantizer::RawEventData,
                         Quantizer::RawEventData,
                         unit);
        }
    } else {

        NotationQuantizer *nq;

        if (m_quantizeNotation->isChecked()) {
            nq = new NotationQuantizer();
        } else {
            nq = new NotationQuantizer
                 (Quantizer::RawEventData,
                  Quantizer::RawEventData);
        }

        nq->setUnit(unit);
        nq->setSimplicityFactor(m_complexity->currentIndex() + 11);
        nq->setMaxTuplet(m_tupletLevel->currentIndex() + 1);
        nq->setContrapuntal(m_permitCounterpoint->isChecked());
        nq->setArticulate(m_addArticulations->isChecked());

        quantizer = nq;
    }

    m_settings.setValue("quantizetype", type);
    m_settings.setValue("gridBaseGridUnit", static_cast<unsigned long long>(
            m_standardQuantizations[m_gridBaseGridUnit->currentIndex()]));
    m_settings.setValue("notationBaseGridUnit", static_cast<unsigned long long>(
            m_standardQuantizations[m_notationBaseGridUnit->currentIndex()]));
    m_settings.setValue("quantizeswing", swing);
    m_settings.setValue("quantizeiterate", iterate);
    m_settings.setValue("quantizenotationonly",
                       m_quantizeNotation->isChecked());
    if (type == 0) {
        m_settings.setValue("quantizedurations",
                           m_quantizeDurations->isChecked());
    } else {
        m_settings.setValue("quantizesimplicity",
                           m_complexity->currentIndex() + 11);
        m_settings.setValue("quantizemaxtuplet",
                           m_tupletLevel->currentIndex() + 1);
        m_settings.setValue("quantizecounterpoint",
                           m_permitCounterpoint->isChecked());
    }
    m_settings.setValue("quantizerebeam", m_rebeam->isChecked());
    m_settings.setValue("quantizearticulate", m_addArticulations->isChecked());
    m_settings.setValue("quantizemakeviable", m_tieNotesAtBarlines->isChecked());
    m_settings.setValue("quantizedecounterpoint", m_splitAndTie->isChecked());

    return quantizer;
}

void
QuantizeParameters::slotTypeChanged(int index)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum));
    parentWidget()->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

    if (index == 0) {
        m_gridBox->show();
        m_swingLabel->show();
        m_swing->show();
        m_iterativeAmountLabel->show();
        m_iterativeAmount->show();
        m_quantizeDurations->show();
        m_notationBox->hide();
    } else if (index == 1) {
        m_gridBox->show();
        m_swingLabel->hide();
        m_swing->hide();
        m_iterativeAmountLabel->hide();
        m_iterativeAmount->hide();
        m_quantizeDurations->hide();
        m_notationBox->hide();
    } else {
        m_gridBox->hide();
        m_notationBox->show();
    }

    adjustSize();
    parentWidget()->adjustSize();
}

}

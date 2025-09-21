#include "PianoWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMap>
#include <QString>
#include <QVector> // Added for QVector

PianoWidget::PianoWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(120);
}

void PianoWidget::setNotesToHighlight(const std::vector<std::string>& notes)
{
    m_highlightedNotes.clear();
    for (size_t i = 0; i < notes.size(); ++i) {
        m_highlightedNotes.append(QString::fromStdString(notes[i]));
    }
    update(); // Trigger a repaint
}

void PianoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int whiteKeyCount = 14; // Draw two octaves
    const float whiteKeyWidth = (float)width() / whiteKeyCount;
    const float whiteKeyHeight = height();
    const float blackKeyWidth = whiteKeyWidth * 0.6f;
    const float blackKeyHeight = whiteKeyHeight * 0.6f;

    const QColor bassHighlightColor = QColor(100, 149, 237); // Cornflower Blue
    const QColor noteHighlightColor = QColor(173, 216, 230); // Light Blue
    const QColor whiteKeyColor = Qt::white;
    const QColor blackKeyColor = Qt::black;

    // Get the bass note for special highlighting
    QString bassNote;
    if (!m_highlightedNotes.isEmpty()) {
        bassNote = m_highlightedNotes.first();
    }
  
    // Map white key index to note name for two octaves
    const QVector<QString> whiteKeyNotes = {
        "C", "D", "E", "F", "G", "A", "B", // First octave
        "C", "D", "E", "F", "G", "A", "B"  // Second octave
    };

    // Map black key start position (as a multiple of white key width) to note name for two octaves
    const QMap<float, QString> blackKeyPositions = {
        {0.7f, "C#"}, {1.7f, "D#"}, {3.7f, "F#"}, {4.7f, "G#"}, {5.7f, "A#"}, // First octave
        {7.7f, "C#"}, {8.7f, "D#"}, {10.7f, "F#"}, {11.7f, "G#"}, {12.7f, "A#"} // Second octave
    };
    const QMap<int, QString> allNotesIndex = {
        {0, "C"}, {1, "C#"}, {2, "D"}, {3, "D#"}, {4, "E"}, {5, "F"}, {6, "F#"},
        {7, "G"}, {8, "G#"}, {9, "A"}, {10, "A#"}, {11, "B"},
    };
    //create empty set to record highlighted notes exsisting in the piano
    QSet<QString> highlightSet;

    // find bass note index at allNotesIndex
    int bassIndex = allNotesIndex.key(bassNote, -1);

    // draw white root
    for (int i = 0; i < whiteKeyCount; ++i) {
        QRectF keyRect(i * whiteKeyWidth, 0, whiteKeyWidth, whiteKeyHeight);
        if (whiteKeyNotes[i] == bassNote) {
            painter.setBrush(bassHighlightColor);
            highlightSet.insert(bassNote);
            painter.setPen(Qt::darkGray);
            painter.drawRect(keyRect);
            break;
        } 
    }
    // Draw other white keys 
    for (int i = 0; i < whiteKeyCount; ++i) {
        QRectF keyRect(i * whiteKeyWidth, 0, whiteKeyWidth, whiteKeyHeight);

        int noteValue = allNotesIndex.key(whiteKeyNotes[i], -1);
     
        if (i >= 7) {
            noteValue += 12; // Adjust for second octave
        }
        if (whiteKeyNotes[i] == bassNote && noteValue == bassIndex) {
            continue; // Skip already drawn bass note
        }
        if (m_highlightedNotes.contains(whiteKeyNotes[i]) && !highlightSet.contains(whiteKeyNotes[i]) && noteValue > bassIndex) {
                highlightSet.insert(whiteKeyNotes[i]);
                painter.setBrush(noteHighlightColor);
            
        } else {
            painter.setBrush(whiteKeyColor);
        }
        painter.setPen(Qt::darkGray);
        painter.drawRect(keyRect);
    }
    // draw black root
    for (auto it = blackKeyPositions.constBegin(); it != blackKeyPositions.constEnd
(); ++it) {
        QRectF keyRect(it.key() * whiteKeyWidth, 0, blackKeyWidth, blackKeyHeight);
        if (it.value() == bassNote) {
            painter.setBrush(bassHighlightColor);
            highlightSet.insert(bassNote);
            painter.setPen(Qt::black); 
            painter.drawRect(keyRect);
            break;
        } 
    }
    // Draw other black keys on top

    int counter = 0;
    for (auto it = blackKeyPositions.constBegin(); it != blackKeyPositions.constEnd(); ++it) {


        QRectF keyRect(it.key() * whiteKeyWidth, 0, blackKeyWidth, blackKeyHeight);
        int noteValue = allNotesIndex.key(it.value(), -1);
        if (counter >= 5) {
            noteValue += 12; // Adjust for second octave
        }

        if (it.value() == bassNote && noteValue == bassIndex) {
            continue; // Skip already drawn bass note
        }
        if (m_highlightedNotes.contains(it.value()) && !highlightSet.contains(it.value()) && noteValue > bassIndex) {
                highlightSet.insert(it.value());
                painter.setBrush(noteHighlightColor);
            
        } else {
            painter.setBrush(blackKeyColor);
        }
       
        painter.setPen(Qt::black); 
        painter.drawRect(keyRect);
        counter++;
    }
}
   


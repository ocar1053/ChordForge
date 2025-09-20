#include "Chord.h"
#include <algorithm> // For std::find

// Defines the 12 chromatic notes
const std::vector<std::string> Chord::allNotes = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

// Defines the intervals for major and minor chords from the root note
// Major: Root, Major Third (4 semitones), Perfect Fifth (7 semitones)
// Minor: Root, Minor Third (3 semitones), Perfect Fifth (7 semitones)
const std::map<std::string, std::vector<int>> Chord::chordFormulas = {
    {"Major", {0, 4, 7}},
    {"Minor", {0, 3, 7}},
    {"Diminished", {0, 3, 6}},
    {"Augmented", {0, 4, 8}},
    {"Dominant 7th", {0, 4, 7, 10}},
    {"Major 7th", {0, 4, 7, 11}},
    {"Minor 7th", {0, 3, 7, 10}}
};

Chord::Chord(const std::string& rootNote, const std::string& chordType)
    : m_rootNote(rootNote), m_chordType(chordType)
{
    calculateNotes();
}

void Chord::calculateNotes()
{
    m_notes.clear();
    auto it = std::find(allNotes.begin(), allNotes.end(), m_rootNote);
    if (it == allNotes.end()) {
        // Root note not found, handle error or do nothing
        return;
    }

    int rootIndex = std::distance(allNotes.begin(), it);

    const auto& formula = chordFormulas.find(m_chordType);
    if (formula == chordFormulas.end()) {
        // Chord type not found
        return;
    }

    for (int interval : formula->second) {
        m_notes.push_back(allNotes[(rootIndex + interval) % 12]);
    }
}

std::vector<std::string> Chord::getNotes() const
{
    return m_notes;
}

std::vector<std::string> Chord::getInversion(unsigned int inversionNumber) const
{
    if (m_notes.empty() || inversionNumber == 0) {
        return m_notes;
    }

    std::vector<std::string> inversion = m_notes;
    for (unsigned int i = 0; i < inversionNumber; ++i) {
        if (inversion.empty()) break;
        // Move the bottom note to the end of the vector (conceptually an octave higher)
        inversion.push_back(inversion.front());
        inversion.erase(inversion.begin());
    }
    return inversion;
}

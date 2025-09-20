#ifndef CHORD_H
#define CHORD_H

#include <string>
#include <vector>
#include <map>

class Chord
{
public:
    // Constructor that takes a root note and chord type
    Chord(const std::string& rootNote, const std::string& chordType);

    // Returns the notes of the chord in root position
    std::vector<std::string> getNotes() const;

    // Returns a specific inversion of the chord
    std::vector<std::string> getInversion(unsigned int inversionNumber) const;

    // A map to store all notes in chromatic order for calculation
    static const std::vector<std::string> allNotes;
    // A map to define chord structures by intervals
    static const std::map<std::string, std::vector<int>> chordFormulas;

private:
    void calculateNotes();

    std::string m_rootNote;
    std::string m_chordType;
    std::vector<std::string> m_notes;
};

#endif // CHORD_H

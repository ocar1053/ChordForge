#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <vector>
#include <string>
#include <numeric> // for std::accumulate

#include "Chord.h"

// Helper function to join a vector of strings with a separator
std::string join(const std::vector<std::string>& elements, const std::string& separator) {
    if (elements.empty()) {
        return "";
    }
    return std::accumulate(std::next(elements.begin()), elements.end(), elements[0],
        [&separator](const std::string& a, const std::string& b) {
            return a + separator + b;
        });
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QMainWindow window;
    window.setWindowTitle("ChordTool");

    // Main widget and layout
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Input Controls ---
    QHBoxLayout *inputLayout = new QHBoxLayout();
    QComboBox *rootNoteCombo = new QComboBox(centralWidget);
    QComboBox *chordTypeCombo = new QComboBox(centralWidget);

    // Populate combos
    // Use the static member from the Chord class
    const std::vector<std::string>& allNotes = Chord::allNotes;
    for (const std::string& note : allNotes) {
        rootNoteCombo->addItem(QString::fromStdString(note));
    }
    chordTypeCombo->addItem("Major");
    chordTypeCombo->addItem("Minor");

    inputLayout->addWidget(new QLabel("Root Note:", centralWidget));
    inputLayout->addWidget(rootNoteCombo);
    inputLayout->addWidget(new QLabel("Chord Type:", centralWidget));
    inputLayout->addWidget(chordTypeCombo);
    mainLayout->addLayout(inputLayout);

    // --- Output Displays ---
    QLabel *rootPosLabel = new QLabel("Root Position: ", centralWidget);
    QLabel *firstInvLabel = new QLabel("1st Inversion: ", centralWidget);
    QLabel *secondInvLabel = new QLabel("2nd Inversion: ", centralWidget);
    mainLayout->addWidget(rootPosLabel);
    mainLayout->addWidget(firstInvLabel);
    mainLayout->addWidget(secondInvLabel);
    mainLayout->addStretch(); // Pushes everything to the top

    // --- Logic ---
    auto updateChords = [&]() {
        std::string rootNote = rootNoteCombo->currentText().toStdString();
        std::string chordType = chordTypeCombo->currentText().toStdString();

        Chord currentChord(rootNote, chordType);

        std::vector<std::string> rootNotes = currentChord.getNotes();
        std::vector<std::string> firstInvNotes = currentChord.getInversion(1);
        std::vector<std::string> secondInvNotes = currentChord.getInversion(2);

        rootPosLabel->setText("Root Position: " + QString::fromStdString(join(rootNotes, " - ")));
        firstInvLabel->setText("1st Inversion: " + QString::fromStdString(join(firstInvNotes, " - ")));
        secondInvLabel->setText("2nd Inversion: " + QString::fromStdString(join(secondInvNotes, " - ")));
    };

    // Connect signals to the update function
    QObject::connect(rootNoteCombo, &QComboBox::currentIndexChanged, updateChords);
    QObject::connect(chordTypeCombo, &QComboBox::currentIndexChanged, updateChords);

    // Initial calculation
    updateChords();

    window.setCentralWidget(centralWidget);
    window.resize(450, 250);
    window.show();

    return app.exec();
}

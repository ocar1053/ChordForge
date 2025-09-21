#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <vector>
#include <string>
#include <numeric> // for std::accumulate

#include "Chord.h"
#include "PianoWidget.h"

// QSS Stylesheet for Dark Theme
const char* qss = R"(
    QWidget {
        background-color: #2E2E2E;
        color: #E0E0E0;
        font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
    }

    QLabel {
        font-size: 14px;
        font-weight: bold;
        padding-top: 10px;
    }

    /* Style for the result labels */
    #ResultLabel {
        font-size: 16px;
        padding: 5px;
        color: #A9D0F5; /* Light blue for emphasis */
        font-weight: normal;
    }

    QPushButton {
        background-color: #4F4F4F;
        border: 1px solid #444;
        border-radius: 4px;
        padding: 8px;
        font-size: 13px;
    }

    QPushButton:hover {
        background-color: #5A5A5A;
    }

    QPushButton:pressed {
        background-color: #444444;
    }

    QPushButton:checked {
        background-color: #4A6984; /* Blue for selection */
        border: 1px solid #5A98D1;
        color: #FFFFFF;
    }
)";

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
    app.setStyleSheet(qss); // Apply the dark theme

    QMainWindow window;
    window.setWindowTitle("ChordForge");

    // Main widget and layout
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15); // Add some spacing between elements

    // --- Input Controls ---
    mainLayout->addWidget(new QLabel("Root Note:"));
    QGridLayout *noteLayout = new QGridLayout();
    QButtonGroup *noteGroup = new QButtonGroup(centralWidget);
    const auto& allNotes = Chord::allNotes;
    for (int i = 0; i < allNotes.size(); ++i) {
        QPushButton *button = new QPushButton(QString::fromStdString(allNotes[i]));
        button->setCheckable(true);
        noteLayout->addWidget(button, i / 6, i % 6);
        noteGroup->addButton(button, i);
    }
    noteGroup->button(0)->setChecked(true);
    mainLayout->addLayout(noteLayout);

    mainLayout->addWidget(new QLabel("Chord Type:"));
    QGridLayout *typeLayout = new QGridLayout();
    QButtonGroup *typeGroup = new QButtonGroup(centralWidget);
    const std::vector<std::string> chordTypes = {"Major", "Minor", "Diminished", "Augmented", "Dominant 7th", "Major 7th", "Minor 7th"};
    int typeId = 0;
    for (const auto& type : chordTypes) {
        QPushButton *button = new QPushButton(QString::fromStdString(type));
        button->setCheckable(true);
        typeLayout->addWidget(button, typeId / 4, typeId % 4);
        typeGroup->addButton(button, typeId++);
    }
    typeGroup->button(0)->setChecked(true);
    mainLayout->addLayout(typeLayout);

    // --- Piano Display Selector ---
    mainLayout->addWidget(new QLabel("Piano Display:"));
    QHBoxLayout* inversionLayout = new QHBoxLayout();
    QButtonGroup* inversionGroup = new QButtonGroup(centralWidget);
    QPushButton* btnRoot = new QPushButton("Root");
    QPushButton* btnInv1 = new QPushButton("1st Inv");
    QPushButton* btnInv2 = new QPushButton("2nd Inv");
    QPushButton* btnInv3 = new QPushButton("3rd Inv");
    btnRoot->setCheckable(true);
    btnInv1->setCheckable(true);
    btnInv2->setCheckable(true);
    btnInv3->setCheckable(true);
    inversionGroup->addButton(btnRoot, 0);
    inversionGroup->addButton(btnInv1, 1);
    inversionGroup->addButton(btnInv2, 2);
    inversionGroup->addButton(btnInv3, 3);
    inversionLayout->addWidget(btnRoot);
    inversionLayout->addWidget(btnInv1);
    inversionLayout->addWidget(btnInv2);
    inversionLayout->addWidget(btnInv3);
    btnRoot->setChecked(true);
    mainLayout->addLayout(inversionLayout);

    // --- Piano Display ---
    PianoWidget *pianoWidget = new PianoWidget(centralWidget);
    mainLayout->addWidget(pianoWidget);

    // --- Output Displays ---
    QLabel *rootPosLabel = new QLabel("Root Position: ", centralWidget);
    QLabel *firstInvLabel = new QLabel("1st Inversion: ", centralWidget);
    QLabel *secondInvLabel = new QLabel("2nd Inversion: ", centralWidget);
    QLabel *thirdInvLabel = new QLabel("3rd Inversion: ", centralWidget);
    
    rootPosLabel->setObjectName("ResultLabel");
    firstInvLabel->setObjectName("ResultLabel");
    secondInvLabel->setObjectName("ResultLabel");
    thirdInvLabel->setObjectName("ResultLabel");

    mainLayout->addWidget(rootPosLabel);
    mainLayout->addWidget(firstInvLabel);
    mainLayout->addWidget(secondInvLabel);
    mainLayout->addWidget(thirdInvLabel);
    mainLayout->addStretch();

    // --- Logic ---
    auto updateChords = [&]() {
        int noteId = noteGroup->checkedId();
        if (noteId == -1) return;
        std::string rootNote = allNotes[noteId];

        int typeId = typeGroup->checkedId();
        if (typeId == -1) return;
        std::string chordType = typeGroup->button(typeId)->text().toStdString();

        Chord currentChord(rootNote, chordType);

        std::vector<std::string> rootNotes = currentChord.getNotes();
        std::vector<std::string> firstInvNotes = currentChord.getInversion(1);
        std::vector<std::string> secondInvNotes = currentChord.getInversion(2);
        std::vector<std::string> thirdInvNotes = currentChord.getInversion(3);

        // Update piano based on inversion selection
        int inversionSelection = inversionGroup->checkedId();
        std::vector<std::string> notesToHighlight;
        switch(inversionSelection) {
            case 0: notesToHighlight = rootNotes; break;
            case 1: notesToHighlight = firstInvNotes; break;
            case 2: notesToHighlight = secondInvNotes; break;
            case 3: notesToHighlight = thirdInvNotes; break;
        }
        pianoWidget->setNotesToHighlight(notesToHighlight);

        // Update text labels
        rootPosLabel->setText("Root Position: " + QString::fromStdString(join(rootNotes, " - ")));
        firstInvLabel->setText("1st Inversion: " + QString::fromStdString(join(firstInvNotes, " - ")));
        secondInvLabel->setText("2nd Inversion: " + QString::fromStdString(join(secondInvNotes, " - ")));

        // Show/hide 3rd inversion button and label
        if (rootNotes.size() == 4) {
            thirdInvLabel->setText("3rd Inversion: " + QString::fromStdString(join(thirdInvNotes, " - ")));
            thirdInvLabel->setVisible(true);
            btnInv3->setVisible(true);
        } else {
            thirdInvLabel->setVisible(false);
            btnInv3->setVisible(false);
            // If 3rd inv is selected but no longer available, default to root
            if (inversionGroup->checkedId() == 3) {
                btnRoot->setChecked(true);
            }
        }
    };

    // Connect signals to the update function
    QObject::connect(noteGroup, &QButtonGroup::idClicked, updateChords);
    QObject::connect(typeGroup, &QButtonGroup::idClicked, updateChords);
    QObject::connect(inversionGroup, &QButtonGroup::idClicked, updateChords);

    // Initial calculation
    updateChords();

    window.setCentralWidget(centralWidget);
    window.resize(520, 600); // Adjusted size for new buttons
    window.show();

    return app.exec();
}

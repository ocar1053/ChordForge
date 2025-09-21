#ifndef PIANOWIDGET_H
#define PIANOWIDGET_H

#include <QWidget>
#include <vector>
#include <string>
#include <QSet>
#include <QString>

class PianoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PianoWidget(QWidget *parent = nullptr);

public slots:
    void setNotesToHighlight(const std::vector<std::string>& notes);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QString> m_highlightedNotes;

    static const QVector<QString> whiteKeyNotes;
    static const QMap<float, QString> blackKeyPositions;
};

#endif // PIANOWIDGET_H

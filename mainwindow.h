#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>

#include "CharacterSave.h"


class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void ChooseFile();
	void UpdateSaveFiles();
	void ClearSaveFiles();
	void LoadSaveFiles(const QString& i_directory);

    QLineEdit* m_saveFolderEdit;
	QLabel* m_noCharacterLabel;
	QTabWidget* m_charactersTabWidget;
	QList<CharacterSave*> m_charactersSaves;
};
#endif // MAINWINDOW_H

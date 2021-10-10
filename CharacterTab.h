#ifndef CHARACTERTAB_H
#define CHARACTERTAB_H

#include <QWidget>
#include <QPushButton>
#include <QTabWidget>

#include "CharacterSave.h"

class CharacterTab : public QWidget
{
public:
	explicit CharacterTab(CharacterSave* i_character, QTabWidget* i_parrentTabWidget, QWidget *parent = nullptr);
private:
	void ReviveCharacter();

	QPushButton* m_reviveButton;
	CharacterSave* m_characterSave;
	QTabWidget* m_parrentTabWidget;
};

#endif // CHARACTERTAB_H

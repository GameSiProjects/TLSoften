#include "CharacterTab.h"

#include <QVBoxLayout>
#include <QLabel>

CharacterTab::CharacterTab(CharacterSave* i_character, QTabWidget* i_parrentTabWidget, QWidget *parent)
	: QWidget(parent)
	, m_characterSave(i_character)
	, m_parrentTabWidget(i_parrentTabWidget)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setAlignment(Qt::AlignTop);

	//Character save display
	QLabel* saveNameLabel = new QLabel("Save name: " + m_characterSave->saveFileName());
	QLabel* classNameLabel = new QLabel("Class: " + m_characterSave->className());
	QLabel* characterNameLabel = new QLabel("Character name: " + m_characterSave->characterName());

	mainLayout->addWidget(saveNameLabel);
	mainLayout->addWidget(classNameLabel);
	mainLayout->addWidget(characterNameLabel);

	m_reviveButton = new QPushButton("Revive");
	connect(m_reviveButton, &QPushButton::released, this, &CharacterTab::ReviveCharacter);
	mainLayout->addWidget(m_reviveButton);
	m_reviveButton->setVisible(i_character->isDead());
	//

	setLayout(mainLayout);
}

void CharacterTab::ReviveCharacter()
{
	m_characterSave->Revive();
	m_reviveButton->setVisible(m_characterSave->isDead());
	m_parrentTabWidget->setTabText(m_parrentTabWidget->currentIndex(), m_characterSave->characterName());
}

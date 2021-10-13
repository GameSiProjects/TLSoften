#include "CharacterSave.h"

#include <QFile>
#include <QFileInfo>
#include <QSaveFile>

CharacterSave::CharacterSave(const QString& i_savePath)
    : m_savePath(i_savePath)
{
	LoadData(i_savePath);
}

void CharacterSave::LoadData(const QString &i_savePath)
{
	QFile file(i_savePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	m_saveFileName = QFileInfo(file.fileName()).fileName();

	m_saveData = file.readAll();

	const QChar k_emptyChar = QChar(0);
	const QChar k_checkSymbolChar = QChar(32);
	QString className;
	QChar stopCharacter = QChar(222);
	unsigned int dataIndex = 0;

	//Get class
	QChar dataView = m_saveData.at(dataIndex);
	while (true)
	{
		dataView = QChar(m_saveData.at(dataIndex));
		if (dataView == stopCharacter)
		{
			break;
		}
		else if (dataView.isLetter())
		{
			className += dataView;
		}
		dataIndex++;
	}
	m_className = className;
	//

	//Get character name
	dataIndex += 70;
	size_t nameCharacterCount = m_saveData.at(dataIndex);
	QString characterName;
	for (int i = 0; i < nameCharacterCount; i++)
	{
		dataIndex += 2;
		dataView = QChar(m_saveData.at(dataIndex));
		characterName += dataView;
	}
	m_characterName = characterName;
	//

	//Check live or dead
	dataIndex += 104;
	m_isDead = QChar(m_saveData.at(dataIndex)) == k_emptyChar
			&& QChar(m_saveData.at(dataIndex + 1)) == k_emptyChar;
	m_deadStatusIndex = dataIndex;
	//

	file.close();
}

const QString &CharacterSave::saveFileName() const
{
    return m_saveFileName;
}

const QString &CharacterSave::className() const
{
    return m_className;
}

const QString &CharacterSave::characterName() const
{
	return m_characterName;
}

bool CharacterSave::isDead() const
{
	return m_isDead;
}

void CharacterSave::Revive()
{
	//Modify save data
	m_saveData.replace(m_deadStatusIndex, 2, "HC");
	//Save as new file
	QString newSaveName = GetNewUniqueFileName(m_savePath, ".new");
	QSaveFile file(newSaveName);
	file.open(QIODevice::WriteOnly);
	file.write(m_saveData);
	file.commit();
	//Rename the old file
	QFile oldSaveFile(m_savePath);
	QString desiredNameForOldFile = GetNewUniqueFileName(m_savePath, ".old");
	oldSaveFile.rename(desiredNameForOldFile);
	//Rename the new file
	QFile newSaveFile(newSaveName);
	newSaveFile.rename(m_savePath);

	m_isDead = false;
}

QString CharacterSave::GetNewUniqueFileName(QString& i_oldName, QString i_suffix)
{
	QString desiredName = i_oldName + i_suffix;
	unsigned int renameNewFileAttemp = 0;
	while (QFile::exists(desiredName))
	{
		desiredName = i_oldName + QString::number(renameNewFileAttemp) + i_suffix;
		renameNewFileAttemp++;
	}
	return desiredName;
}

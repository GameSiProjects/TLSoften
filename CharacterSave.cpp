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
	const QChar k_checkSymbolChar = QChar(31);
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
	dataIndex += 72;
	stopCharacter = QChar(34);
	QString characterName;
	bool finishGetName = false;
	while (true)
	{
		dataView = QChar(m_saveData.at(dataIndex));
		if (finishGetName)
		{
			if (dataView > k_checkSymbolChar && dataView != stopCharacter)
			{
				break;
			}
		}
		else if (dataView == k_emptyChar && m_saveData.at(dataIndex - 1) == k_emptyChar)
		{
			finishGetName = true;
		}
		else if (dataView > k_checkSymbolChar)
		{
			characterName += dataView;
		}

		dataIndex++;
	}
	m_characterName = characterName;
	//

	//Check live or dead
	dataIndex += 94;
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
	QSaveFile file(GetNewUniqueFileName(m_savePath, "new"));
	file.open(QIODevice::WriteOnly);
	file.write(m_saveData);
	file.commit();
	//Rename the old file
	QFile oldSaveFile(m_savePath);
	QString desiredNameForOldFile = GetNewUniqueFileName(m_savePath, "old");
	oldSaveFile.rename(desiredNameForOldFile);
	//Rename the new file
	QFile newSaveFile(m_savePath + "new");
	newSaveFile.rename(m_savePath);

	m_isDead = false;
}

QString CharacterSave::GetNewUniqueFileName(QString& i_oldName, QString i_suffix)
{
	QString desiredName = i_oldName + i_suffix;
	unsigned int renameNewFileAttemp = 0;
	while (QFile::exists(desiredName))
	{
		desiredName += renameNewFileAttemp;
		renameNewFileAttemp++;
	}
	return desiredName;
}
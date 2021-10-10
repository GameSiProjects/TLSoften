#ifndef CHARACTERSAVE_H
#define CHARACTERSAVE_H

#include <QString>

class CharacterSave
{
public:
	static const int CLASS_NAME_OFFSET = 32;
	CharacterSave(const QString& i_savePath);

	const QString &saveFileName() const;

	const QString &className() const;

	const QString &characterName() const;

	bool isDead() const;

	void Revive();

private:
	void LoadData(const QString& i_savePath);
	QString GetNewUniqueFileName(QString& i_oldName, QString i_suffix);

    QString m_savePath;
    QString m_saveFileName;
    QString m_className;
	QString m_characterName;
	bool m_isDead;
	int m_deadStatusIndex;
	QByteArray m_saveData;
};
#endif // CHARACTERSAVE_H

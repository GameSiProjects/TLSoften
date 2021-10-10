#include "mainwindow.h"
#include "CharacterTab.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProcessEnvironment>
#include <QFileDialog>
#include <QDirIterator>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QWidget * centralWidget = new QWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setAlignment(Qt::AlignTop);

	//Save folder UI
	QGroupBox* saveFolderGroupBox = new QGroupBox(tr("Save folder"));
	QHBoxLayout* saveFolderLayout = new QHBoxLayout();

	QString defaultSaveFilePath = QProcessEnvironment::systemEnvironment().value("APPDATA", "");
	defaultSaveFilePath += "\\runic games\\torchlight\\save";
	m_saveFolderEdit = new QLineEdit(defaultSaveFilePath);
	m_saveFolderEdit->setDisabled(true);

	QPushButton* chooseFolderButton = new QPushButton("Choose folder");
	connect(chooseFolderButton, &QPushButton::released, this, &MainWindow::ChooseFile);

	saveFolderLayout->addWidget(m_saveFolderEdit);
	saveFolderLayout->addWidget(chooseFolderButton);

	saveFolderGroupBox->setLayout(saveFolderLayout);
	saveFolderGroupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	mainLayout->addWidget(saveFolderGroupBox);
	//

	//Characters saves modifying UI
	QGroupBox* charactersGroupBox = new QGroupBox(tr("Characters"));
	QVBoxLayout* charactersLayout = new QVBoxLayout();
	m_noCharacterLabel = new QLabel("No character found in " + m_saveFolderEdit->text());


	m_charactersTabWidget = new QTabWidget(charactersGroupBox);
	//m_charactersTabWidget->setTabShape()

	charactersLayout->addWidget(m_noCharacterLabel);
	charactersLayout->addWidget(m_charactersTabWidget);

	charactersGroupBox->setLayout(charactersLayout);
	mainLayout->addWidget(charactersGroupBox);
	//

	centralWidget->setLayout(mainLayout);
	this->setCentralWidget(centralWidget);

	UpdateSaveFiles();
}

MainWindow::~MainWindow()
{
	ClearSaveFiles();
}

void MainWindow::ChooseFile()
{
	QFileDialog chooseFileDialog(this);
	QFileInfo currentPathInfo = QFileInfo(m_saveFolderEdit->text());
	if (currentPathInfo.isFile())
	{
		chooseFileDialog.setDirectory(currentPathInfo.dir());
	}
	else
	{
		chooseFileDialog.setDirectory(currentPathInfo.filePath());
	}
	chooseFileDialog.setFileMode(QFileDialog::Directory);
	chooseFileDialog.setOption(QFileDialog::DontUseNativeDialog, true);

	connect(&chooseFileDialog, &QFileDialog::currentChanged,this,[&](const QString & str)
	{
		QFileInfo info(str);
		if(info.isFile())
		{
			chooseFileDialog.setFileMode(QFileDialog::ExistingFile);
			chooseFileDialog.setNameFilter(tr("*.svt"));
		}
		else if(info.isDir())
		{
			chooseFileDialog.setFileMode(QFileDialog::Directory);
		}
	});

	if (chooseFileDialog.exec())
	{
		if (chooseFileDialog.selectedFiles().size() > 0)
		{
			m_saveFolderEdit->setText(chooseFileDialog.selectedFiles().at(0));
		}
		else
		{
			m_saveFolderEdit->setText(chooseFileDialog.directory().absolutePath());
		}
		UpdateSaveFiles();
	}
}

void MainWindow::UpdateSaveFiles()
{
	ClearSaveFiles();
	LoadSaveFiles(m_saveFolderEdit->text());

	if (m_charactersSaves.size() > 0)
	{
		foreach (CharacterSave* save, m_charactersSaves)
		{
			QString tabName = save->characterName();
			if (save->isDead())
			{
				tabName += " (Dead)";
			}

			m_charactersTabWidget->addTab(new CharacterTab(save, m_charactersTabWidget), tabName);
		}
		m_noCharacterLabel->setVisible(false);
		m_charactersTabWidget->setVisible(true);
	}
	else
	{
		m_noCharacterLabel->setVisible(true);
		m_charactersTabWidget->setVisible(false);
	}
}

void MainWindow::ClearSaveFiles()
{
	while(m_charactersTabWidget->count() != 0)
	{
		m_charactersTabWidget->removeTab(0);
	}
	qDeleteAll(m_charactersSaves);
	m_charactersSaves.clear();
}

void MainWindow::LoadSaveFiles(const QString& i_directory)
{
	if (QFileInfo(i_directory).isFile())
	{
		CharacterSave* save = new CharacterSave(i_directory);
		m_charactersSaves.push_back(save);
	}
	else
	{
		QDirIterator it(i_directory, QStringList() << "*.svt", QDir::Files, QDirIterator::Subdirectories);
		while (it.hasNext())
		{
			CharacterSave* save = new CharacterSave(it.next());
			m_charactersSaves.push_back(save);
		}
	}
}

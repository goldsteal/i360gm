#ifndef UIMAIN_H
#define UIMAIN_H

#include <QtGui/QMainWindow>
#include <QDir>

#include "ui_uiMain.h"
#include "ui_uiAbout.h"

#include "Xbox/Game.h"
#include "xgdf/Raw.h"
#include "GameList.h"
#include "Loader.h"
#include "XGDF/Iso.h"

class Main : public QMainWindow
{
	Q_OBJECT

public:
	Main(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Main();


	Ui::MainClass *getUi(){return &ui;}
	void refreshDir(QString directory, GameList *list, bool keep = false);

	//Data
	signals:

	public slots:
		void slotOnClickList(const QModelIndex &current, const QModelIndex &previous);
		void saveDot();
		void extractIso();
		void setGamePath();
		void checkHashCollision();
		void extractFile();
		void calculateMemory();
		void reportIntline9();

		//Loader
		void progressTotalIsos(uint isos);
		void progressIso(QString iso);
		void loaderDone(vector<Game*> *games, uint vptr);

		//Extraction
		void fileExtractedSuccess(QString name);
		void bytesWritten(uint bytes);
		void setTree(QTreeWidgetItem *item);
		void fileExtracted(QString name, uint size);
		void isoExtracted(Iso *iso);

	//Log
	void addLog(QString log);

private:
	void readNameDb();
	void walkDot(QString &trace, FileNode *&node);

	//Helper functions

	//Helper data
	GameList *_source, *_external;
	Loader *_loader;

	QString _lastDotPath;
	QString _lastIsoPath;
	QString _gamePath;
	QString _filePath;

	//Settings
	QString sExternalPath, sSourcePath;

	//Forms
	Ui::MainClass ui;
	Ui::About uiAbout;
	QDialog *about;
};
#endif // UIMAIN_H

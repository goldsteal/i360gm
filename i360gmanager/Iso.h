#ifndef PACKET_H
#define PACKET_H

#include <QVariant>
#include <QAbstractListModel>
#include <QPixmap>
#include <QString>
#include <QFile>
#include <QBool>
#include <QBrush>

#include "xbox.h"
#include "FileNode.h"

#include <vector>
#include <map>

using std::map;
using std::vector;

class Iso
{
public:
	Iso();
	~Iso();

	//Qt data passing
	QVariant getField(int column);
	
	//Set and get functions
	QString getPath();
	QString getIso();
	bool setPath(QString path);

	//Iso functions
	bool isDefaultXex();
	bool isValidMedia();
	vector<XboxFileInfo*> getFiles(bool refresh = false);
	XboxFileInfo* getFile(char* name, int length);
	

private:
	//Helper functions
	void cleanupIso();
	void walkFile(uint offset);
	void makeTree(void *sector, uint offset, FileNode *&node);

	//Helper data
	QString _path;
	map<XboxFileInfo*, void*> _sectors;
	vector<XboxFileInfo*> _files;
	FileNode *_rootFile;

	//Iso data
	int _isoHandle;
	char _isValidMedia;
	char _isValidXex;
	XboxMedia _xboxMedia;
	XboxFileInfo *_defaultXex;
	uint _offset;
};

#endif
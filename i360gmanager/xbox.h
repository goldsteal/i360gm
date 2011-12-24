#ifndef XBOX_H
#define XBOX_H

#define SECTOR_SIZE 2048
#define MAX_SECTOR 10240000
#define GAME_SECTOR 32
#define GLOBAL_LSEEK_OFFSET 0xFD90000ul
#define XGD3_LSEEK_OFFSET 0x2080000ul
#define MEDIA_MAGIC_BYTE "MICROSOFT*XBOX*MEDIA"
#define MEDIA_MAGIC_BYTE_SIZE 20
#define XEX_MAGIC_BYTE 0x32584558 //XEX2
#define XEX_FILE "default.xex"
#define XEX_FILE_SIZE 11
#ifndef MAX_PATH
	#define MAX_PATH 255
#endif

struct XboxFileInfoStruct;

#pragma pack(1)
typedef struct
{
	uchar magic[MEDIA_MAGIC_BYTE_SIZE];
	uint rootSector;
	uint rootSize;
	
	bool isValidMedia(int iso, uint &offset)
	{
		offset = 0;
		readMedia(iso);
		if(!isXboxMedia())
		{
			//Check for XDG2 disc
			offset = GLOBAL_LSEEK_OFFSET;
			readMedia(iso, offset);
			if(!isXboxMedia())
			{
				//Check for XDG3 disc
				offset = XGD3_LSEEK_OFFSET;
				readMedia(iso, offset);
				if(!isXboxMedia())
				{
					return false;
				}
			}
		}
		return true;
	}

	void readMedia(int iso, uint offset = 0)
	{
		_lseeki64(iso, GAME_SECTOR*SECTOR_SIZE+offset, SEEK_SET);
		_read(iso, (char*)this, sizeof(XboxMedia));
	}

	bool isXboxMedia()
	{
		if(magic == NULL) return false;
		return (memcmp(magic, MEDIA_MAGIC_BYTE, MEDIA_MAGIC_BYTE_SIZE) == 0);
	}

	unsigned __int64 getRootAddress(uint video)
	{
		return ((unsigned __int64)rootSector*SECTOR_SIZE)+video;
	}

} XboxMedia;

typedef struct XboxFileInfoStruct
{
	short flag1;
	short flag2;
	uint sector;
	uint size;
	uchar type;
	uchar length;
	uchar name[MAX_PATH];		//This is nasty! But there is no other way.....


	XboxFileInfoStruct* load(char *pointer)
	{
		return (XboxFileInfoStruct*)pointer;
	}

	unsigned __int64 getAddress(uint offset)
	{
		return ((unsigned __int64)sector*SECTOR_SIZE)+offset;
	}

	uint getStructSize()
	{
		uint structSize = sizeof(XboxFileInfo)-MAX_PATH+length;
		uint padding = 0;

		for(int i = length; i < MAX_PATH; i++)
		{
			if((uchar)name[i] != 0xFF)
				break;
			padding++;

			if(padding > 32)
				return MAX_SECTOR;
		}

		return structSize + padding;
	}

	bool isEqual(char* _name, int _length)
	{
		if(length == _length)   //Speed up
			if(_strnicmp((char*)name, _name, _length) == 0)
				return true;
		return false;
	}

} XboxFileInfo;
#pragma pack()

#endif
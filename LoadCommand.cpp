#pragma once
#include"Command.h"

void LoadCommand::ReadFileStruct(FileStruct* filestruct, ifstream& fin)
{
	size_t len = 0;

	char* Readcontent;
	string tempcontent;

	//读大小
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetSize(atoi(tempcontent.c_str()));
	delete[]Readcontent;

	//读名字
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetName(tempcontent);
	delete[]Readcontent;

	//读类型
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetPath(tempcontent);
	delete[]Readcontent;

	//读创建时间
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetCreateTime(tempcontent);
	delete[]Readcontent;

	//读修改时间
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetModifyTime(tempcontent);
	delete[]Readcontent;
}
void LoadCommand::ReadFile(File* targetfile, ifstream& fin)
{
	ReadFileStruct(targetfile, fin);
	size_t len;
	char* Readcontent;
	fin.read((char*)&len, sizeof(size_t));
	//len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	targetfile->SetContent(Readcontent, len);
	//delete[]Readcontent;
	Readcontent = nullptr;
}
void LoadCommand::ReadDir(Dir* targetdir, ifstream& fin)
{
	ReadFileStruct(targetdir, fin);
	size_t len;
	char* Readcontent;
	string tempcontent;

	//读大小
	fin.read((char*)&len, sizeof(size_t));
	len++;
	//没出错
	if (fin.good() && !fin.eof())
	{
		Readcontent = new char[len];
		fin.read(Readcontent, len);
		tempcontent = Readcontent;
		delete[]Readcontent;
		//子结构
		if (tempcontent == "++++++++++")
		{

			while (1)
			{
				fin.read((char*)&len, sizeof(size_t));
				len++;
				Readcontent = new char[len];
				fin.read(Readcontent, len);
				tempcontent = Readcontent;
				delete[]Readcontent;
				//文件
				if (tempcontent == "//////////")
				{
					File* newfile = new File("temp", targetdir->GetPath(), targetdir);
					ReadFile(newfile, fin);
					targetdir->FileOrDirAdd(newfile);
				}
				//目录
				else if (tempcontent == "**********")
				{
					Dir* newdir = new Dir("temp", targetdir->GetPath(), targetdir);
					ReadDir(newdir, fin);
					targetdir->FileOrDirAdd(newdir);
				}
				else
				{
					break;
				}
			}

		}

		else
		{
			//cout << tempcontent << "|3304" << endl;
			return;
		}
	}
}

void LoadCommand::Execute(VirtualDisk* virtualdisk)
{

	string dirpath, filename;
	//要写的内容
	string writecontent;
	ifstream filein;
	//如果是绝对路径
	if (IsAbsolutePath(this->CommandOrder[1]))
	{
		filein.open(this->CommandOrder[1], ios::in | ios::binary);
	}
	else
	{
		filein.open(VirtualDisk::RealSavePath + this->CommandOrder[1], ios::in | ios::binary);
	}
	if (!filein.is_open())
	{
		Error::CannotFindPathError(this->CommandOrder[1]);
		return;
	}
	Dir* rootdir = virtualdisk->GetRootDir();
	//全部删除,rootdir本身还在
	Dir::DeleteFileOfDirRecursion(rootdir);
	Dir::DeleteEmptyDirRecursion(rootdir);

	ReadDir(rootdir, filein);
	filein.close();
}
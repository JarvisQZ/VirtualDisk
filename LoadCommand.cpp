#pragma once
#include"Command.h"

void LoadCommand::ReadFileStruct(FileStruct* filestruct, ifstream& fin)
{
	size_t len = 0;

	char* Readcontent;
	string tempcontent;

	//����С
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetSize(atoi(tempcontent.c_str()));
	delete[]Readcontent;

	//������
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetName(tempcontent);
	delete[]Readcontent;

	//������
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetPath(tempcontent);
	delete[]Readcontent;

	//������ʱ��
	fin.read((char*)&len, sizeof(size_t));
	len++;
	Readcontent = new char[len];
	fin.read(Readcontent, len);
	tempcontent = Readcontent;
	filestruct->SetCreateTime(tempcontent);
	delete[]Readcontent;

	//���޸�ʱ��
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

	//����С
	fin.read((char*)&len, sizeof(size_t));
	len++;
	//û����
	if (fin.good() && !fin.eof())
	{
		Readcontent = new char[len];
		fin.read(Readcontent, len);
		tempcontent = Readcontent;
		delete[]Readcontent;
		//�ӽṹ
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
				//�ļ�
				if (tempcontent == "//////////")
				{
					File* newfile = new File("temp", targetdir->GetPath(), targetdir);
					ReadFile(newfile, fin);
					targetdir->FileOrDirAdd(newfile);
				}
				//Ŀ¼
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
	//Ҫд������
	string writecontent;
	ifstream filein;
	//����Ǿ���·��
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
	//ȫ��ɾ��,rootdir������
	Dir::DeleteFileOfDirRecursion(rootdir);
	Dir::DeleteEmptyDirRecursion(rootdir);

	ReadDir(rootdir, filein);
	filein.close();
}
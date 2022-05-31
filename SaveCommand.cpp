#pragma once
#include"Command.h"

void SaveCommand:: WriteFileStruct(FileStruct* filestruct, ofstream& fout)
{
	size_t len, addtaillen;
	string tempcontent;

	//��ȡ��С
	tempcontent = to_string(filestruct->GetSize());
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3048" << tempcontent<< "|"<<endl;
	fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
	fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���

	//��ȡ����
	tempcontent = filestruct->GetName();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3053" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
	fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���

	//��ȡ·��
	tempcontent = filestruct->GetPath();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3059" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
	fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���

	//��ȡ����ʱ��
	tempcontent = filestruct->GetCreateTime();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3067" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
	fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���
	//��ȡ�޸�ʱ��
	tempcontent = filestruct->GetModifyTime();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3073" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
	fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���

}

void SaveCommand::WriteFile(File* targetfile, ofstream& fout)
{
	WriteFileStruct(targetfile, fout);
	//�ӽṹ��־
	char* tempcontent = targetfile->GetContent();
	size_t len = targetfile->GetSize();
	size_t addtaillen = len + 1;
	fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
	fout.write(tempcontent, addtaillen);  //��string���������д���ļ���

}
void SaveCommand::WriteDir(Dir* targetdir, ofstream& fout)
{
	WriteFileStruct(targetdir, fout);
	size_t len, addtaillen;
	string tempcontent;

	//��Ϊ��
	map<string, FileStruct*> fileordir = targetdir->GetFileOrDir();
	if (fileordir.size() != 0)
	{
		//�ӽṹ��ʼ��־
		tempcontent = "++++++++++";
		len = tempcontent.length();
		addtaillen = len + 1;
		fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
		fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���


		for (auto filestruct : fileordir)
		{
			if (filestruct.second->GetFType() == "Dir")
			{
				//Ŀ¼��ʶ��
				tempcontent = "**********";
				len = tempcontent.length();
				addtaillen = len + 1;
				fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
				fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���

				WriteDir(dynamic_cast<Dir*>(filestruct.second), fout);
			}
			else
			{
				//�ļ���ʶ��
				tempcontent = "//////////";
				len = tempcontent.length();
				addtaillen = len + 1;
				fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
				fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���

				WriteFile(dynamic_cast<File*>(filestruct.second), fout);
			}
		}
		//�ӽṹ������־
		tempcontent = "----------";
		len = tempcontent.length();
		addtaillen = len + 1;
		fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
		fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���
	}
	else
	{
		//�ӽṹ������־
		tempcontent = "----------";
		len = tempcontent.length();
		addtaillen = len + 1;
		fout.write((char*)&len, sizeof(size_t)); //��string����ĳ���д���ļ���
		fout.write(tempcontent.data(), addtaillen);  //��string���������д���ļ���
	}
}
void SaveCommand::Execute(VirtualDisk* virtualdisk)
{

	string dirpath, filename;
	//Ҫд������
	string writecontent;
	ofstream fileout;
	//����Ǿ���·��
	if (IsAbsolutePath(this->CommandOrder[1]))
	{
		fileout.open(this->CommandOrder[1], ios::out | ios::binary);
	}
	else
	{
		fileout.open(VirtualDisk::RealSavePath + this->CommandOrder[1], ios::out | ios::binary);
	}
	if (!fileout.is_open())
	{
		Error::CannotFindPathError(this->CommandOrder[1]);
		return;
	}
	Dir* rootdir = virtualdisk->GetRootDir();
	WriteDir(rootdir, fileout);
	fileout.close();
}
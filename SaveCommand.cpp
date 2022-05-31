#pragma once
#include"Command.h"

void SaveCommand:: WriteFileStruct(FileStruct* filestruct, ofstream& fout)
{
	size_t len, addtaillen;
	string tempcontent;

	//获取大小
	tempcontent = to_string(filestruct->GetSize());
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3048" << tempcontent<< "|"<<endl;
	fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
	fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中

	//获取名字
	tempcontent = filestruct->GetName();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3053" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
	fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中

	//获取路径
	tempcontent = filestruct->GetPath();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3059" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
	fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中

	//获取创建时间
	tempcontent = filestruct->GetCreateTime();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3067" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
	fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中
	//获取修改时间
	tempcontent = filestruct->GetModifyTime();
	len = tempcontent.length();
	addtaillen = len + 1;
	//cout << len << "|3073" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
	fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中

}

void SaveCommand::WriteFile(File* targetfile, ofstream& fout)
{
	WriteFileStruct(targetfile, fout);
	//子结构标志
	char* tempcontent = targetfile->GetContent();
	size_t len = targetfile->GetSize();
	size_t addtaillen = len + 1;
	fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
	fout.write(tempcontent, addtaillen);  //将string对象的内容写入文件中

}
void SaveCommand::WriteDir(Dir* targetdir, ofstream& fout)
{
	WriteFileStruct(targetdir, fout);
	size_t len, addtaillen;
	string tempcontent;

	//不为空
	map<string, FileStruct*> fileordir = targetdir->GetFileOrDir();
	if (fileordir.size() != 0)
	{
		//子结构开始标志
		tempcontent = "++++++++++";
		len = tempcontent.length();
		addtaillen = len + 1;
		fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
		fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中


		for (auto filestruct : fileordir)
		{
			if (filestruct.second->GetFType() == "Dir")
			{
				//目录标识符
				tempcontent = "**********";
				len = tempcontent.length();
				addtaillen = len + 1;
				fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
				fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中

				WriteDir(dynamic_cast<Dir*>(filestruct.second), fout);
			}
			else
			{
				//文件标识符
				tempcontent = "//////////";
				len = tempcontent.length();
				addtaillen = len + 1;
				fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
				fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中

				WriteFile(dynamic_cast<File*>(filestruct.second), fout);
			}
		}
		//子结构结束标志
		tempcontent = "----------";
		len = tempcontent.length();
		addtaillen = len + 1;
		fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
		fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中
	}
	else
	{
		//子结构结束标志
		tempcontent = "----------";
		len = tempcontent.length();
		addtaillen = len + 1;
		fout.write((char*)&len, sizeof(size_t)); //将string对象的长度写入文件中
		fout.write(tempcontent.data(), addtaillen);  //将string对象的内容写入文件中
	}
}
void SaveCommand::Execute(VirtualDisk* virtualdisk)
{

	string dirpath, filename;
	//要写的内容
	string writecontent;
	ofstream fileout;
	//如果是绝对路径
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
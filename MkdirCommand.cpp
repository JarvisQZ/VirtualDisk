#pragma once
#include"Command.h"

bool MkdirCommand::IsCommandCorrect()
{

	//判断参数是否合法
	if (this->CommandOrder.size() != 2)
	{
		Help::HelpMkdirCommand();
		return false;
	}
	else
	{
		// 以 / 分割参数,this->CommandOrder存了两个值得时候，第二个为路径
		vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

		//判断路径名称是否合法  是否包含特殊字符，比如   *a/b/c   a/*b/c   C:/*a/b/c  ../*a/b/c  ./a/b/c
		for (auto name : parapath)
		{
			if (name == "C:")
				continue;
			//路径中包含了 空  a//b/c
			if (name == "" && name != parapath[0])
			{
				Error::PathFormatError();
				return false;
			}
			if (!FileHelper::IsContainIllegalChar(name))
			{
				Error::NameError();
				return false;
			}
		}
		return true;
	}
}

void MkdirCommand::Execute(VirtualDisk* virtualdisk)
{

	// 以 / 分割参数,this->CommandOrder存了两个值得时候，第二个为路径
	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  绝对路径  或者 mkdir  /a/b/c  以 /开头
		{
			if (i == parapath.size() - 1)
			{
				Error::VisitRootError();
				return;
			}
			//获取根目录对象
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  以 .. 开头的命令
		{
			if (i == parapath.size() - 1)
			{
				Error::CreateFatherSelfError();
				return;
			}
			//获取当前目录，之后变为父目录
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			if (i == parapath.size() - 1)
			{
				Error::CreateSelfError();
				return;
			}
			continue;
		}
		else
		{
			//目录不存在,就新建目录
			if (containdirandfile.find(parapath[i]) == containdirandfile.end())
			{
				//如果是根目录下的文件或文件夹，路径是C:
				Dir* NewDir;
				if (searchdir == VirtualDisk::GetRootDir())
				{
					NewDir = new Dir(parapath[i], searchdir->GetPath(), searchdir);
				}
				else
				{
					//如果不是根路径下的目录，路径是父目录那一级
					NewDir = new Dir(parapath[i], searchdir->GetPath() + "/" + searchdir->GetName(), searchdir);
				}

				//新建目录并且添加进map中，并且增加修改时间
				searchdir->FileOrDirAdd(parapath[i], NewDir);
				searchdir = NewDir;
				containdirandfile = searchdir->GetFileOrDir();
				//delete NewDir;

			}
			//在map里找到了名字，还得判断是文件还是目录
			else
			{
				//如果是文件,报错,不能新建与文件重名的目录
				if (containdirandfile[parapath[i]]->GetFType() == "File")
				{
					Error::DirNameSaveAsFileError();
					return;
				}
				//如果是目录
				else
				{
					//找到了目录的名字，新建目录的名字和它不同
					if (i != parapath.size() - 1)
					{
						//获取改目录并类型转化为目录类型
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
					else
					{
						Error::DirSameNameError(searchdir->GetPath(), parapath[i]);
						return;
					}
				}
			}
		}
	}
}
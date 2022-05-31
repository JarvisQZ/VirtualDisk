#pragma once
#include"Command.h"

bool TouchCommand::IsCommandCorrect()
{


	//判断参数是否合法
	if (this->CommandOrder.size() != 2)
	{
		Help::HelpTouchCommand();
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

void TouchCommand::Execute(VirtualDisk* virtualdisk)
{

	// 以 / 分割参数,this->CommandOrder存了两个值得时候，第二个为路径
	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  绝对路径  或者 touch  /a/b/c  以 /开头
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
			//文件名字前的目录    比如要创建c    a/b/c   这里是a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::TouchNoDirFileError(searchdir->GetPath() + "/" + parapath[i]);
					return;
				}
				else
				{
					//找到了名字但是它是文件
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
						return;
					}
					else
					{
						//获取改目录并类型转化为目录类型
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
				}
			}
			else
			{
				//没有文件叫这个名字,新建文件
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					string path;
					if (searchdir == virtualdisk->GetRootDir())
					{
						path = searchdir->GetPath();
					}
					else
					{
						path = searchdir->GetPath() + "/" + searchdir->GetName();
					}
					File* NewFile = new File(parapath[i], path, searchdir);
					searchdir->FileOrDirAdd(parapath[i], NewFile);
					return;
				}
				else
				{
					//找到了同名的目录
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						Error::FileNameSaveAsDirError();
						return;
					}
					//修改时间
					else
					{
						dynamic_cast<File*>(containdirandfile[parapath[i]])->SetModifyTime();
					}
				}
			}
		}
	}
}
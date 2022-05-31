#pragma once
#include"Command.h"

bool RenameCommand::IsCommandCorrect()
{

	//判断参数是否合法
	if (this->CommandOrder.size() != 3)
	{
		Help::HelpRenameCommand();
		return false;
	}
	// 以 / 分割参数,this->CommandOrder存了两个值得时候，第二个为路径,第三个为新名称
	if (!FileHelper::IsContainIllegalChar(this->CommandOrder[2]))
	{
		Error::NameError();
		return false;
	}
	return true;
}

void RenameCommand::Execute(VirtualDisk* virtualdisk)
{

	// 以 / 分割参数,this->CommandOrder存了两个值得时候，第二个为路径,第三个为新名称
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
				Error::VisitRootError();
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
				Error::CannotVisitError();
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
					Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
					return;
				}
				else
				{
					//找到了名字但是它是文件
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
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
				//没有文件叫这个名字,报错
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindFileError(searchdir->GetPath(), parapath[i]);
					return;
				}
				else
				{
					//新名字已经存在了
					if (containdirandfile.find(this->CommandOrder[2]) != containdirandfile.end())
					{
						if (containdirandfile[this->CommandOrder[2]]->GetFType() == "Dir")
						{
							Error::RenameSaveAsDirError(this->CommandOrder[2]);

						}
						else
						{
							Error::RenameSaveAsFileError(this->CommandOrder[2]);
						}
						return;
					}
					//先把自己名字改了
					containdirandfile[parapath[i]]->SetName(this->CommandOrder[2]);
					//新名字不存在,新名字指针指向旧名字
					searchdir->FileOrDirAdd(this->CommandOrder[2], containdirandfile[parapath[i]]);
					//删除旧名字的指针指向
					searchdir->RemoveFromMap(parapath[i]);

				}
			}
		}
	}
}
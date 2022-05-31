#pragma once
#include"Command.h"

void CdCommand::Execute(VirtualDisk* virtualdisk)
{

	// 以 / 分割参数,this->CommandOrder存了两个值得时候，第二个为路径
	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (size_t i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  绝对路径  或者 cd  /a/b/c  以 /开头
		{
			//获取根目录对象
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  以 .. 开头的命令
		{
			//获取当前目录，之后变为父目录
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			continue;
		}
		else
		{
			//目录不存在,报错
			if (containdirandfile.find(parapath[i]) == containdirandfile.end())
			{
				Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
				return;

			}
			//在map里找到了名字，还得判断是文件还是目录
			if (containdirandfile.find(parapath[i]) != containdirandfile.end())
			{
				//如果是文件,报错,说明没有这个目录
				if (containdirandfile[parapath[i]]->GetFType() == "File")
				{
					Error::CannotFindPathError(containdirandfile[parapath[i]]->GetPath() + "/" + parapath[i]);
					return;
				}
				//如果是目录
				else
				{
					//获取改目录并类型转化为目录类型
					searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
					containdirandfile = searchdir->GetFileOrDir();
				}
			}
		}
	}
	virtualdisk->SetLocalDir(searchdir);
}
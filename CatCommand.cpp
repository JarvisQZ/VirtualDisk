#pragma once
#include"Command.h"

void CatCommand:: Execute(VirtualDisk* virtualdisk)
{

	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (size_t i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))   //  C:/a/b/c  绝对路径  或者 del  /a/b/c  以 /开头
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
			//文件名字前的目录        a/b/c   这里是a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					//根目录
					if (searchdir->GetName() == "")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
					}
					else
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

					}
					return;
				}
				else
				{
					//找到了名字,它是文件，报错
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						//根目录
						if (searchdir->GetName() == "")
						{
							Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
						}
						else
						{
							Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

						}
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
				//没有文件或者目录叫这个名字,报错
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
					return;
				}
				else
				{
					//找到了同名的目录,不是文件，报错
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
						return;
					}
					//如果是文件，如果不为空输出内容
					else
					{
						File* file = dynamic_cast<File*>(containdirandfile[parapath[i]]);
						file->PrintContent();
					}
				}
			}
		}
	}
}
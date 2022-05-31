#pragma once
#include"Command.h"

bool DelCommand::IsCommandCorrect()
{

	//判断参数是否合法
	if (this->CommandOrder.size() == 1)
	{
		Help::HelpDelCommand();
		return false;
	}
	else if (this->CommandOrder.size() == 2)
	{
		this->PathSet.emplace_back(this->CommandOrder[1]);
		return true;
	}
	//参数大于等于三个
	else
	{
		//有三个参数时，第二个是/s 表示参数而不是文件或者目录的名字
		if (this->CommandOrder[1] == "/s")
		{
			//递归删除文件
			this->Recursion = true;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
		}
		//如果没有/s 之后都是路径
		else
		{
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 1, this->CommandOrder.end());
		}

	}

	return true;
}

void DelCommand::SpecialPathExecute(vector<string> parapath, int index, Dir* targetdir)
{
	if (index == parapath.size() - 1)
	{
		//如果需要递归删除
		if (this->Recursion)
		{
			Dir::DeleteFileOfDirRecursion(targetdir);
		}
		//不递归只删除当前目录下文件
		else
		{
			targetdir->DeleteFileFromDir();
		}
	}
}

void DelCommand::DeleteFileInPath(VirtualDisk* virtualdisk, string path)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))   //  C:/a/b/c  绝对路径  或者 del  /a/b/c  以 /开头
		{
			//获取根目录对象
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
			//如果输入了 C:
			this->SpecialPathExecute(parapath, i, searchdir);
		}
		else if (parapath[i] == "..")  //  ../a/b/c  以 .. 开头的命令
		{
			//获取当前目录，之后变为父目录
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
			//如果输入了 .
			this->SpecialPathExecute(parapath, i, searchdir);
		}
		else if (parapath[i] == ".")
		{
			if (i != parapath.size() - 1)
			{
				continue;
			}
			//如果输入了 .
			this->SpecialPathExecute(parapath, i, searchdir);
		}
		else
		{
			//文件名字前的目录        a/b/c   这里是a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
					return;
				}
				else
				{
					//找到了名字,它是文件，报错
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
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
					//找到了同名的目录,递归删除该目录下所有文件，根据参数
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						//先变换目录到path上的目录
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
						//如果需要递归删除
						if (this->Recursion)
						{
							Dir::DeleteFileOfDirRecursion(searchdir);
						}
						//不递归只删除当前目录下文件
						else
						{
							searchdir->DeleteFileFromDir();
						}
					}
					//如果是文件，就删除
					else
					{
						//删除对象
						delete containdirandfile[parapath[i]];
						//指向空
						containdirandfile[parapath[i]] = nullptr;
						//从map中移除,只是临时的这个删除了，真正的还没有删除,因为它不是指针
						containdirandfile.erase(parapath[i]);
						searchdir->RemoveFromMap(parapath[i]);
					}
				}
			}
		}
	}
}
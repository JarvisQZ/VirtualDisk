#pragma once
#include"Command.h"

//判断语句是否合法
bool DirCommand::IsCommandCorrect()
{


	//无参数
	if (this->CommandOrder.size() == 1)
	{
		// 因为默认执行模式是 0所以不做改变
		return true;
	}
	//一个参数
	else if (this->CommandOrder.size() == 2)
	{
		//只带/ad参数，不带路径
		if (this->CommandOrder[1] == "/ad")
		{
			this->ExecType = 2;
			return true;
		}
		//只带/s参数，不带路径
		else if (this->CommandOrder[1] == "/s")
		{
			this->ExecType = 3;
			return true;
		}
		//只带路径，不带参数
		else
		{
			this->ExecType = 1;
			this->PathSet.emplace_back(this->CommandOrder[1]);
			return true;
		}
	}
	//多个参数
	else
	{

		if (this->CommandOrder.size() == 3)
		{
			//如果是命令 dir /s /ad  或 dir /ad /s
			if ((this->CommandOrder[1] == "/ad" && (this->CommandOrder[2] == "/s")) || (this->CommandOrder[1] == "/s" && (this->CommandOrder[2] == "/ad")))
			{
				this->ExecType = 6;
				return true;
			}
		}
		else
		{
			//如果是命令 dir /s /ad  path (path) ... 或 dir /ad /s path (path) ...
			if ((this->CommandOrder[1] == "/ad" && (this->CommandOrder[2] == "/s")) || (this->CommandOrder[1] == "/s" && (this->CommandOrder[2] == "/ad")))
			{
				this->ExecType = 7;
				this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 3, this->CommandOrder.end());
				return true;
			}
		}

		// 命令 dir /ad path (path)
		if (this->CommandOrder[1] == "/ad")
		{
			this->ExecType = 5;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
			return true;
		}
		// 命令 dir /s path (path)
		else if (this->CommandOrder[1] == "/s")
		{
			this->ExecType = 4;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
			return true;
		}
		// 命令 dir path1 path2
		else
		{
			this->ExecType = 1;
			//插入路径集合中去
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 1, this->CommandOrder.end());
			return true;
		}
	}
	return false;
}

Dir* DirCommand::GetPathDir(VirtualDisk* virtualdisk, string path)
{
	vector<string> parapath = FileHelper::GetSplitPathName(path);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  绝对路径  或者   /a/b/c  以 /开头
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
				if (searchdir->GetName() == "")
				{
					Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
				}
				else
				{
					Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
				}
				return nullptr;

			}
			//在map里找到了名字，还得判断是文件还是目录
			if (containdirandfile.find(parapath[i]) != containdirandfile.end())
			{
				//如果是文件,报错,说明没有这个目录
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
					return nullptr;
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
	return searchdir;
}

//合法则执行语句
void DirCommand::Execute(VirtualDisk* virtualdisk)
{

	//只输出此级目录下的文件和目录
	if (this->ExecType == 0)
	{
		this->LocalDir->PrintAllFileAndDir();
	}
	//输出path路径下的文件和目录
	else if (this->ExecType == 1)
	{
		for (auto path : this->PathSet)
		{
			Dir* tempdir = this->GetPathDir(virtualdisk, path);
			if (tempdir == nullptr)
			{
				return;
			}
			cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " 的目录" << endl;
			tempdir->PrintAllFileAndDir();
			//cout << "            " << tempdir->GetNumOfFiles() << "个文件" << "           " << sizeof(tempdir) << "字节"<<endl;
		}
	}
	// 输出当前目录下的目录 /ad
	else if (this->ExecType == 2)
	{
		this->LocalDir->PrintAllDir();
	}
	// 输出目录及其所有子目录下所有文件
	else if (this->ExecType == 3)
	{
		Dir::PrintAllFileAndDirRecursion(this->LocalDir);
	}
	// 执行dir 带参数/s path
	else if (this->ExecType == 4)
	{
		for (auto path : this->PathSet)
		{
			Dir* tempdir = this->GetPathDir(virtualdisk, path);
			if (tempdir == nullptr)
			{
				return;
			}
			//cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " 的目录" << endl;
			Dir::PrintAllFileAndDirRecursion(tempdir);
		}
	}
	else if (this->ExecType == 5)
	{
		for (auto path : this->PathSet)
		{
			Dir* tempdir = this->GetPathDir(virtualdisk, path);
			if (tempdir == nullptr)
			{
				return;
			}
			cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " 的目录" << endl;
			tempdir->PrintAllDir();
		}
	}
	else if (this->ExecType == 6)
	{
		Dir::PrintAllDirRecursion(this->LocalDir);

	}
	else
	{
		for (auto path : this->PathSet)
		{
			Dir* tempdir = this->GetPathDir(virtualdisk, path);
			if (tempdir == nullptr)
			{
				return;
			}
			//cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " 的目录" << endl;
			Dir::PrintAllDirRecursion(tempdir);
		}
	}
}
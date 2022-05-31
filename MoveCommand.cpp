#pragma once
#include"Command.h"

bool MoveCommand::IsCommandCorrect()
{

	//判断参数是否合法

	if (this->CommandOrder.size() == 3)
	{
		this->SrcPath = this->CommandOrder[1];
		this->DstPath = this->CommandOrder[2];
	}
	else if (this->CommandOrder.size() == 4)
	{
		if (this->CommandOrder[1] != "/y")
		{
			Help::HelpMoveCommand();
			return false;
		}
		this->Cover = true;
		this->SrcPath = this->CommandOrder[2];
		this->DstPath = this->CommandOrder[3];
	}
	else
	{
		Help::HelpMoveCommand();
		return false;
	}
	return true;
}
//给定一个路径，返回他的所在的目录对象   type是true的时候，取最后一级目录，false的时候不取
// move path1 path2      move   a/b/c/d   e/f/g/h    对于sec来说 a/b/c/d都需要存在 对于 dstpath 来说  e/f/g 存在 h 不要求存在 
Dir* MoveCommand::GetPathDir(VirtualDisk* virtualdisk, string path, bool type)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	if (!type)
	{
		//对于dstpath来说只判断前几级目录是否存在
		parapath.pop_back();
	}
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//绝对路径
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  绝对路径  或者   /a/b/c  以 /开头
		{
			if (i == parapath.size() - 1)
			{
				Error::VisitRootError();
				return nullptr;
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
				return nullptr;
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
				return nullptr;
			}
			continue;
		}
		else
		{
			//如果是最后一级
			if (i == parapath.size() - 1)
			{
				//目录不存在,报错
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
					return nullptr;

				}
				//在map里找到了名字，不管文件还是目录都返回它上一级
				else
				{

					//如果是文件,转换为File类之后获取父目录
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						if (type)
						{
							//现在就是文件父目录，直接返回
							return searchdir;
						}
						else
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
					}
					//如果是目录
					else
					{
						if (type)
						{
							//获取父目录并类型转化为目录类型
							searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]])->GetFatherDir();
						}
						else
						{
							searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						}

					}
					containdirandfile = searchdir->GetFileOrDir();
					return searchdir;
				}
			}
			//不是最后一级，全是目录名才行   a/b/c/d/r/f   a,b,c,d,r 全部得是目录
			else
			{
				//目录不存在,报错
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
	}
	return searchdir;
}

void MoveCommand::Execute(VirtualDisk* virtualdisk)
{
	//获取
	Dir* srcdir = this->GetPathDir(virtualdisk, this->SrcPath, true);
	Dir* dstdir = this->GetPathDir(virtualdisk, this->DstPath, false);
	//如果为空说明有错
	if (srcdir == nullptr || dstdir == nullptr) { return; }

	vector<string> parapath = FileHelper::GetSplitPathName(this->SrcPath);
	//取最后的名字
	string srcname = parapath[parapath.size() - 1];
	parapath = FileHelper::GetSplitPathName(this->DstPath);
	//取最后的名字
	string dstname = parapath[parapath.size() - 1];
	//if (srcdir == dstdir ) 
	//{ 
	//	map<string, FileStruct*> dirsofsrcdir = srcdir->GetFileOrDir();
	//	if (srcname == dstname)
	//	{
	//		dirsofsrcdir[srcname]->SetModifyTime();
	//	}
	//	else {
	//		//添加
	//		dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
	//		//删除
	//		srcdir->RemoveFromMap(srcname);
	//	}
	//	return;
	//}

	//如果加了 /y 参数 ,不经过询问
	if (this->Cover == true)
	{
		map<string, FileStruct*> dirsofsrcdir = srcdir->GetFileOrDir();
		map<string, FileStruct*> dirsofdstdir = dstdir->GetFileOrDir();
		//重命名了
		if (srcname != dstname)
		{
			//修改时间改变
			dirsofsrcdir[srcname]->SetModifyTime();
		}
		//修改路径
		if (dstdir == virtualdisk->GetRootDir())
		{
			dirsofsrcdir[srcname]->SetPath(dstdir->GetPath());
		}
		else
		{
			dirsofsrcdir[srcname]->SetPath(dstdir->GetPath() + "/" + dstdir->GetName());
		}

		//命名
		dirsofsrcdir[srcname]->SetName(dstname);
		//如果移动的是目录，需要将目录下的所有文件和子目录的路径做修改
		if (dirsofsrcdir[srcname]->GetFType() == "Dir")
		{
			Dir::ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dirsofsrcdir[srcname]));
			//父目录也得修改
			dirsofsrcdir[srcname]->SetFatherDir(dstdir);
		}
		//添加
		dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
		//删除
		srcdir->RemoveFromMap(srcname);
	}
	else
	{
		map<string, FileStruct*> dirsofsrcdir = srcdir->GetFileOrDir();
		map<string, FileStruct*> dirsofdstdir = dstdir->GetFileOrDir();
		//已经存在同名文件或目录
		string input;
		//移动到的目录里有同名文件
		if (dirsofdstdir.find(dstname) != dirsofdstdir.end())
		{
			cout << "  覆盖" << dirsofdstdir[dstname]->GetPath() + "/" + dstname << "吗?(Yes/No):" << endl;
			getline(cin, input);
			FileHelper::RemoveSpace(input);
			if (input == "Yes" || input == "Y" || input == "y" || input == "yes")
			{
				//修改路径
				if (dstdir == virtualdisk->GetRootDir())
				{
					dirsofsrcdir[srcname]->SetPath(dstdir->GetPath());
				}
				else
				{
					dirsofsrcdir[srcname]->SetPath(dstdir->GetPath() + "/" + dstdir->GetName());
				}
				//如果移动的是目录，需要将目录下的所有文件和子目录的路径做修改
				if (dirsofsrcdir[srcname]->GetFType() == "Dir")
				{
					Dir::ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dirsofsrcdir[srcname]));
					//父目录也得修改
					dirsofsrcdir[srcname]->SetFatherDir(dstdir);
				}
				else
				{
					//文件要改名字和时间
					dirsofsrcdir[srcname]->SetName(dstname);
					dirsofsrcdir[srcname]->SetModifyTime();
				}

				//添加
				dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
				//删除
				srcdir->RemoveFromMap(srcname);
			}
			else
			{
				cout << "   撤销移动   " << endl;
			}
		}
		//移动到的文件没有同名文件
		else
		{

			//重命名了
			if (srcname != dstname)
			{
				//修改时间改变
				dirsofsrcdir[srcname]->SetModifyTime();
			}
			//修改路径
			if (dstdir == virtualdisk->GetRootDir())
			{
				dirsofsrcdir[srcname]->SetPath(dstdir->GetPath());
			}
			else
			{
				dirsofsrcdir[srcname]->SetPath(dstdir->GetPath() + "/" + dstdir->GetName());
			}
			//命名
			dirsofsrcdir[srcname]->SetName(dstname);
			//如果移动的是目录，需要将目录下的所有文件和子目录的路径做修改
			if (dirsofsrcdir[srcname]->GetFType() == "Dir")
			{
				Dir::ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dirsofsrcdir[srcname]));
				//父目录也得修改
				dirsofsrcdir[srcname]->SetFatherDir(dstdir);
			}
			//添加
			dstdir->FileOrDirAdd(dstname, dirsofsrcdir[srcname]);
			//删除
			srcdir->RemoveFromMap(srcname);
		}
	}
}
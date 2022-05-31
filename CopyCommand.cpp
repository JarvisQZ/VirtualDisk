#pragma once
#include"Command.h"

bool CopyCommand::IsCommandCorrect()
{

	//判断参数是否合法
	if (this->CommandOrder.size() != 3)
	{
		Help::HelpCopyCommand();
		return false;
	}
	else
	{

		//如果第一个参数以@开头，那么就是真实路径
		if (this->CommandOrder[1][0] == '@')
		{
			//存储该目录下所有文件
			vector<string> allfile;
			//去除@符号
			string realpath = this->CommandOrder[1].substr(1, this->CommandOrder[1].size() - 1);


			// 通配符模式
			string regex;

			size_t pos = realpath.find_last_of('/');
			//说明路径里面没有 /  那就在当前目录下，则realpath就是regex
			if (pos == string::npos)
			{
				regex = realpath;
				//从当前目录查找，当前目录肯定存在不做判断
				getFileNames(".", allfile);
			}
			else
			{
				//目录名带路径 最后是带有 /的     C:/a/b/
				this->TargetDir = realpath.substr(0, pos + 1);

				//文件通配符名
				regex = realpath.substr(pos + 1, realpath.size() - 1);
				//从目标路径查找,目标路径不存在就报错
				if (!getFileNames(realpath.substr(0, pos), allfile))
				{
					//磁盘中没有该目录
					Error::CannotFindPathError(realpath.substr(0, pos));
					return false;
				}

			}

			for (auto filename : allfile)
			{
				//如果匹配成功
				if (WildcardMatching(filename, regex))
				{
					this->TargetFile.emplace_back(filename);
				}
			}

			//检查加入的文件个数，如果一个都没有匹配上，报错
			if (TargetFile.size() == 0)
			{
				Error::NoFileMatchedError();
				return false;
			}

			//第二个参数以@开头，
			if (this->CommandOrder[2][0] == '@')
			{
				// 1 真实磁盘拷贝到真实磁盘（磁盘到内存,内存到磁盘）
				this->CopyType = 1;
			}
			else
			{
				// 2 真实磁盘拷贝到虚拟磁盘（磁盘到内存）
				this->CopyType = 2;
			}
		}
		else
		{
			if (this->CommandOrder[2][0] == '@')
			{
				// 3 虚拟磁盘拷贝到真实磁盘（内存到磁盘）
				this->CopyType = 3;
			}
			else
			{
				// 4 虚拟磁盘拷贝到虚拟磁盘（内存到内存）
				this->CopyType = 4;
			}
		}

	}
	return true;
}

//获取磁盘目录下的所有文件的名字，返回状态
bool CopyCommand::getFileNames(string path, vector<string>& files)
{
	//文件句柄
	//注意：如果此处是long类型，实测运行中会报错访问异常
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	//string p;p.assign(path).append("/*").c_str()
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,跳过
			//如果不是,把文件绝对路径存入vector中
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				continue;
			}
			else
			{
				files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	else
	{
		//失败
		return false;
	}
	return true;
}

//通配符匹配
bool CopyCommand::WildcardMatching(string path, string inputpath)
{
	size_t m = path.size();
	size_t n = inputpath.size();
	vector<vector<size_t>> dp(m + 1, vector<size_t>(n + 1));
	dp[0][0] = true;
	for (size_t i = 1; i <= n; ++i) {
		if (inputpath[i - 1] == '*') {
			dp[0][i] = true;
		}
		else {
			break;
		}
	}
	for (size_t i = 1; i <= m; ++i) {
		for (size_t j = 1; j <= n; ++j) {
			if (inputpath[j - 1] == '*') {
				dp[i][j] = dp[i][j - 1] | dp[i - 1][j];
			}
			else if (inputpath[j - 1] == '?' || path[i - 1] == inputpath[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1];
			}
		}
	}
	return dp[m][n];
}

//返回目录指针，如果type=true，则path必须是一个文件夹路径，如果type=false，给定的path是一个文件路径，返回文件父目录
Dir* CopyCommand::GetPathDir(VirtualDisk* virtualdisk, string path, bool type)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	if (!type)
	{
		//只判断前几级目录是否存在
		parapath.pop_back();
	}
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (size_t  i = 0; i < parapath.size(); i++)
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
					//如果是文件,报错
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
						//获取目录对象返回
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
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

//根据路径判断是目录还是文件，或者没有找到
// 0 Error 未找到
// 1 FIle 文件
// 2 Dir 目录
int CopyCommand::FileOrDirBaseOnPath(string Path)
{
	struct stat s;
	if (stat(Path.c_str(), &s) == 0)
	{
		//存在的目录
		if (s.st_mode & S_IFREG)
		{
			return 1;
		}
		//文件
		else
		{
			return 2;
		}
	}
	//不存在
	else
	{
		return 0;
	}
}

void CopyCommand::Execute(VirtualDisk* virtualdisk)
{
	//获取将这些文件拷贝到的目录，如果TargetFile里面只有一个文件，那么copy path1 path2 中的path2是一个文件名,返回父目录
	// 如果TargetFile有多个文件，那么path2是一个目录名

	string reapath = this->CommandOrder[2].substr(1, this->CommandOrder[2].size());
	// 1 真实磁盘拷贝到真实磁盘（磁盘到内存,内存到磁盘）
	if (this->CopyType == 1)
	{
		//拷贝一个文件到另一个已有的文件,直接覆盖
		if (this->TargetFile.size() == 1)
		{
			string dirfile;

			//目录
			if (FileOrDirBaseOnPath(reapath) == 2)
			{
				dirfile = reapath + "/" + this->TargetFile[0];
			}
			else
			{
				dirfile = reapath;
			}
			//文件内容缓存
			char* buffer;
			size_t size;
			//记录文件字节大小
			ifstream fin(this->TargetDir + this->TargetFile[0], ios::in | ios::binary | ios::ate);
			size = static_cast<size_t>(fin.tellg()) + 1;
			try {
				buffer = new char[size];
			}
			catch (exception e)
			{
				Error::MemoryMallocError();
				return;
			}
			memset(buffer, 0, size);
			fin.seekg(fin.beg);
			fin.read(buffer, size);
			//cout << size << "大小" << buffer << "|" << endl;
			fin.close();

			//写文件
			ofstream fout(dirfile, ios::out | ios::binary);
			fout.write(buffer, size);
			fout.close();
			delete[]buffer;
			buffer = nullptr;
		}
		//多个文件
		else
		{

			//目录
			if (FileOrDirBaseOnPath(reapath) == 2)
			{

				for (auto filename : this->TargetFile)
				{
					//文件内容缓存
					char* buffer;
					size_t size;
					//记录文件字节大小
					ifstream fin(this->TargetDir + this->TargetFile[0], ios::in | ios::binary | ios::ate);
					size = static_cast<size_t>(fin.tellg()) + 1;
					try {
						buffer = new char[size];
					}
					catch (exception e)
					{
						Error::MemoryMallocError();
						return;
					}
					memset(buffer, 0, size);
					fin.seekg(fin.beg);
					fin.read(buffer, size);
					//cout << size << "大小" << buffer << "|" << endl;
					fin.close();

					//写文件
					ofstream fout(reapath + "/" + filename, ios::out | ios::binary);
					fout.write(buffer, size);
					fout.close();
					delete[] buffer;
					buffer = nullptr;
				}
			}
			//文件,只用最后一个文件覆盖
			else
			{
				//文件内容缓存
				char* buffer;
				size_t size;
				//记录文件字节大小
				ifstream fin(this->TargetDir + this->TargetFile[this->TargetFile.size() - 1], ios::in | ios::binary | ios::ate);
				size = static_cast<size_t>(fin.tellg()) + 1;
				try {
					buffer = new char[size];
				}
				catch (exception e)
				{
					Error::MemoryMallocError();
					return;
				}
				memset(buffer, 0, size);
				fin.seekg(fin.beg);
				fin.read(buffer, size);
				//cout << size << "大小" << buffer << "|" << endl;
				fin.close();

				//写文件
				ofstream fout(reapath, ios::out | ios::binary);
				fout.write(buffer, size);
				fout.close();
				delete[] buffer;
				buffer = nullptr;
			}

		}

	}
	// 2 真实磁盘拷贝到虚拟磁盘（磁盘到内存）
	else if (this->CopyType == 2)
	{
		Dir* DstDir;
		//获取拷贝到哪个目录的指针
		if (this->TargetFile.size() == 1)
		{
			string newfilename;
			DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], false);
			vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[2]);
			newfilename = parapath[parapath.size() - 1];
			if (DstDir == nullptr) { return; }
			map<string, FileStruct*> containdirandfile = DstDir->GetFileOrDir();
			//存在名为 Targetfile 这个的文件或目录
			if (containdirandfile.find(newfilename) != containdirandfile.end())
			{
				if (containdirandfile[newfilename]->GetFType() == "Dir")
				{
					DstDir = dynamic_cast<Dir*>(containdirandfile[newfilename]);
					newfilename = this->TargetFile[0];
				}
			}

			//如果 copy path1 path2 中的2已经存在并且为文件，报错不执行，如果为目录，将文件放在该目录下，名字不变

			//文件内容缓存
			char* buffer;
			size_t size;
			//size_t a;
			
			//记录文件字节大小
			ifstream in(this->TargetDir + this->TargetFile[0], ios::in | ios::binary | ios::ate);
			size = static_cast<size_t>(in.tellg()) + 1;
			//cout << size << endl;
			try {
				buffer = new char[size];
			}
			catch (exception e)
			{
				Error::MemoryMallocError();
				return;
			}
			memset(buffer, 0, size);
			in.seekg(in.beg);
			in.read(buffer, size);
			//cout << size << "大小" << buffer << "|" << endl;
			in.close();

			string path;
			if (DstDir == virtualdisk->GetRootDir())
			{
				path = DstDir->GetPath();
			}
			else
			{
				path = DstDir->GetPath() + "/" + DstDir->GetName();
			}
			//新建文件
			File* newfile = new File(newfilename, path, DstDir);
			//将指针指向buffer所指向的内容
		
			newfile->SetContent(buffer,size);
			//采取覆盖的方式
			DstDir->FileOrDirAdd(newfilename, newfile, true);
			//delete[] buffer;
			buffer = nullptr;
		}
		else
		{
			DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], true);
			if (DstDir == nullptr) { return; }
			//文件内容缓存
			char* buffer;
			//记录文件字节大小
			size_t size;
			string path;

			//打开每一个文件，将它按字节读到内存
			for (auto file : this->TargetFile)
			{

				ifstream in(this->TargetDir + file, ios::in | ios::binary | ios::ate);
				size = static_cast<size_t>(in.tellg()) + 1;
				try {
					buffer = new char[size];
				}
				catch (exception e)
				{
					Error::MemoryMallocError();
					return;
				}
				memset(buffer, 0, size);
				in.seekg(in.beg);

				in.read(buffer, size);
				in.close();

				if (DstDir == virtualdisk->GetRootDir())
				{
					path = DstDir->GetPath();
				}
				else
				{
					path = DstDir->GetPath() + "/" + DstDir->GetName();
				}
				//新建文件
				File* newfile = new File(file, path, DstDir);
				//将指针指向buffer所指向的内容

				

				newfile->SetContent(buffer, size);
				//采取覆盖的方式
				DstDir->FileOrDirAdd(file, newfile, true);
				//delete[] buffer;
				buffer = nullptr;
			}
		}

	}

	else
	{
		//如果不是以@开头，虚拟磁盘里的路径
		//存储该目录下所有文件
		vector<string> allfile;
		string dirpath = this->CommandOrder[1];
		// 通配符模式
		string regex;
		Dir* SrcDir;
		size_t pos = dirpath.find_last_of('/');
		//说明路径里面没有 /  那就在当前目录下，dirpath =VirtualDisk::localdir
		if (pos == string::npos)
		{
			regex = dirpath;
			//从当前目录查找，当前目录肯定存在不做判断
			SrcDir = virtualdisk->GetLocalDir();
			SrcDir->GetAllFileOfDir(allfile);
		}
		else
		{
			//目录名带路径 最后是带有 /的     C:/a/b/
			this->TargetDir = dirpath.substr(0, pos + 1);
			//获取源目录对象
			SrcDir = GetPathDir(virtualdisk, this->TargetDir, true);
			//如果有错
			if (SrcDir == nullptr) { return; }

			//获取文件名字
			SrcDir->GetAllFileOfDir(allfile);
			//文件通配符名
			regex = dirpath.substr(pos + 1, dirpath.size() - 1);
		}
		for (auto filename : allfile)
		{
			//如果匹配成功
			if (WildcardMatching(filename, regex))
			{
				this->TargetFile.emplace_back(filename);
			}
		}
		if (this->TargetFile.size() == 0)
		{
			Error::NoMatchedFile();
			return;
		}
		// 3 虚拟磁盘拷贝到真实磁盘（内存到磁盘）
		if (this->CopyType == 3)
		{
			//拷贝一个文件到另一个已有的文件,直接覆盖
			if (this->TargetFile.size() == 1)
			{
				string dirfile;

				//目录
				if (FileOrDirBaseOnPath(reapath) == 2)
				{
					dirfile = reapath + "/" + this->TargetFile[0];
				}
				//文件
				else
				{
					dirfile = reapath;
				}
				//文件内容缓存
				char* tempcontent = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[0]])->GetContent();
				size_t size = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[0]])->GetSize();


				//写文件
				ofstream fout(dirfile, ios::out | ios::binary);
				fout.write(tempcontent, size);
				fout.close();

			}
			//多个文件
			else
			{

				//目录
				if (FileOrDirBaseOnPath(reapath) == 2)
				{
					map<string, FileStruct*> sourcefileanddir = SrcDir->GetFileOrDir();
					char* tempcontent;
					size_t size;
					for (auto filename : this->TargetFile)
					{
						tempcontent = dynamic_cast<File*>(sourcefileanddir[filename])->GetContent();
						size = dynamic_cast<File*>(sourcefileanddir[filename])->GetSize();


						//写文件
						ofstream fout(reapath + "/" + filename, ios::out | ios::binary);
						fout.write(tempcontent, size);
						fout.close();

					}
				}
				//文件,只用最后一个文件覆盖
				else
				{
					//文件内容缓存
					char* tempcontent = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[this->TargetFile.size() - 1]])->GetContent();
					size_t size = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[this->TargetFile.size() - 1]])->GetSize();


					//写文件
					ofstream fout(reapath, ios::out | ios::binary);
					fout.write(tempcontent, size);
					fout.close();

				}
			}
		}
		// 4 虚拟磁盘拷贝到虚拟磁盘（内存到内存）
		else
		{
			Dir* DstDir;
			//获取拷贝到哪个目录的指针
			if (this->TargetFile.size() == 1)
			{
				string newfilename;
				DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], false);
				if (DstDir == nullptr) { return; }
				vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[2]);
				newfilename = parapath[parapath.size() - 1];
				map<string, FileStruct*> sourcefileanddir = DstDir->GetFileOrDir();

				//存在名为 Targetfile 这个的文件或目录
				if (sourcefileanddir.find(newfilename) != sourcefileanddir.end())
				{
					if (sourcefileanddir[newfilename]->GetFType() == "File")
					{
						Error::FileNameAlreadyExistError(newfilename);
						return;
					}
					else
					{
						DstDir = dynamic_cast<Dir*>(sourcefileanddir[newfilename]);
						newfilename = this->TargetFile[0];
					}

				}
				//新建文件
				File* newfile = new File(dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[0]]));
				//设置新路径
				newfile->SetPath(DstDir->GetPath() + "/" + DstDir->GetName());
				//设置父目录
				newfile->SetFatherDir(DstDir);
				//设置名字，防止重命名了这边没有改变
				newfile->SetName(newfilename);
				//修改时间
				newfile->SetModifyTime();
				//采取覆盖方式
				DstDir->FileOrDirAdd(newfilename, newfile, true);

			}
			else
			{
				DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], true);
				if (DstDir == nullptr) { return; }
				//复制
				map<string, FileStruct*> sourcefileanddir = SrcDir->GetFileOrDir();
				for (auto file : this->TargetFile)
				{
					//新建文件
					File* newfile = new File(dynamic_cast<File*>(sourcefileanddir[file]));
					//设置新路径
					newfile->SetPath(DstDir->GetPath() + "/" + DstDir->GetName());
					//设置父目录
					newfile->SetFatherDir(DstDir);
					//修改时间
					newfile->SetModifyTime();
					//采取覆盖方式
					DstDir->FileOrDirAdd(file, newfile, true);
				}
			}
		}




	}
}
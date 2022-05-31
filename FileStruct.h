#pragma once
#include"head.h"
#include"FileHelper.h"
#include"Error.h"
//命令模式
//接收者类
//FileStruct
//Dir
//File
//文件和文件夹基类
class FileStruct
{
protected:
	//大小
	size_t Size = 0;
	string Name;
	string FType;
	string Path;
	//父目录
	FileStruct* FatherDir = nullptr;
	//string Mklink;
	string CreateTime;
	string ModifyTime;
public:
	//默认构造函数，子类构造函数调用，为下面字段赋值
	FileStruct()
	{
		this->CreateTime = FileHelper::GetNowTimeToString();
		this->ModifyTime = CreateTime;
	}
	//析构函数
	virtual ~FileStruct()
	{
		FatherDir = nullptr;
		cout << "FileStruct 的析构函数" << endl;
	}

	size_t GetSize()
	{
		return this->Size;
	}
	void SetSize(size_t size)
	{
		this->Size = size;
	}
	//获取文件或文件夹名
	string GetName()
	{
		return this->Name;
	}
	void SetCreateTime(string CreateTime)
	{
		this->CreateTime = CreateTime;
	}
	string GetCreateTime()
	{
		return this->CreateTime;
	}
	string GetModifyTime()
	{
		return this->ModifyTime;
	}
	//设置修改时间
	void SetModifyTime()
	{
		this->ModifyTime = FileHelper::GetNowTimeToString();
	}
	//设置修改时间
	void SetModifyTime(string ModifyTime)
	{
		this->ModifyTime = ModifyTime;
	}
	//获取路径
	string GetPath()
	{
		return this->Path;
	}
	void SetPath(string Path)
	{
		this->Path = Path;
	}
	//获取类型
	string GetFType()
	{
		return this->FType;
	}
	//修改父目录指针
	void SetFatherDir(FileStruct* fatherdir)
	{
		this->FatherDir = fatherdir;
	}
	FileStruct* GetFatherDir()
	{
		return this->FatherDir;
	}
	//设置文件或文件夹名
	void SetName(string Name)
	{
		this->Name = Name;
	}
	

};


//文件夹
class Dir :public FileStruct
{
private:
	map<string, FileStruct*> FileOrDir;
public:
	Dir()
	{
		this->Name = "";
		this->FType = "Dir";
		this->Path = "";
		this->FatherDir = nullptr;
	}

	Dir(string Name, string Path, Dir* FatherDir)
	{
		this->Name = Name;
		this->FType = "Dir";
		this->Path = Path;
		if (FatherDir == nullptr)
		{
			this->FatherDir = this;
		}
		else
		{
			this->FatherDir = FatherDir;
		}
	}

	~Dir()
	{
		//DeleteFileAndDirFromDir();
		//FatherDir = nullptr;
		map<string, FileStruct*>().swap(this->FileOrDir);
		cout << "Dir 的析构函数" << endl;
	}

	//获取父目录
	Dir* GetFatherDir()
	{
		return dynamic_cast<Dir*>(this->FatherDir);
	}



	//目录中新增文件或目录
	void FileOrDirAdd(string Name, FileStruct* New)
	{
		//文件夹不存在 进行创建，修改当前文件夹 修改时间字段
		this->FileOrDir[Name] = New;
		this->ModifyTime = FileHelper::GetNowTimeToString();
		this->CountDirSizeUpward();
		//}
	}
	//载入的时候需要的函数
	void FileOrDirAdd(FileStruct* New)
	{
		this->FileOrDir[New->GetName()] = New;
	}
	//采取覆盖的方式
	void FileOrDirAdd(string Name, FileStruct* New, bool Coverold)
	{

		map<string, FileStruct* >::iterator key = this->FileOrDir.find(Name);
		//文件或者目录已经存在
		if (key != this->FileOrDir.end())
		{
			if (Coverold)
			{
				cout << "将覆盖文件" << this->FileOrDir[Name]->GetPath() + "/" + this->FileOrDir[Name]->GetName() << endl;
				//删除旧的对象，释放指针
				delete this->FileOrDir[Name];
				this->FileOrDir[Name] = nullptr;
				this->FileOrDir.erase(key++);
				//文件夹不存在 进行创建，修改当前文件夹 修改时间字段
				this->FileOrDir[Name] = New;
				this->ModifyTime = FileHelper::GetNowTimeToString();
			}
			else
			{
				cout << "取消此次操作" << endl;
			}

		}
		else
		{
			//文件夹不存在 进行创建，修改当前文件夹 修改时间字段
			this->FileOrDir[Name] = New;
			this->ModifyTime = FileHelper::GetNowTimeToString();
		}
		this->CountDirSizeUpward();
	}

	//删除文件
	void DeleteFileFromDir()
	{
		//只删除文件
		for (map<string, FileStruct*>::iterator it = this->FileOrDir.begin(); it != this->FileOrDir.end();)
		{
			if (it->second->GetFType() == "File")
			{
				//指向父目录的指针置空
				it->second->SetFatherDir(nullptr);
				//删除对象
				delete it->second;
				//指向空
				it->second = nullptr;
				//从map中移除
				this->FileOrDir.erase(it++);

			}
			else { it++; }
		}
		this->CountDirSizeUpward();
	}

	//删除文件和目录，删除对象
	void DeleteFileAndDirFromDir()
	{
		//删除文件和目录
		for (map<string, FileStruct*>::iterator it = this->FileOrDir.begin(); it != this->FileOrDir.end();)
		{
			//指向父目录的指针置空
			it->second->SetFatherDir(nullptr);
			//删除对象
			delete it->second;
			//指向空
			it->second = nullptr;
			//从map中移除
			this->FileOrDir.erase(it++);
		}
		this->CountDirSizeUpward();
	}

	void RemoveFromMap(string Name)
	{
		//删除目录或文件,并没有真正删除对象，只是从map中移除了
		//删除此目录下的目标文件夹并修改当前文件夹的 修改时间字段
		this->FileOrDir.erase(this->FileOrDir.find(Name));
		this->ModifyTime = FileHelper::GetNowTimeToString();
		this->CountDirSizeUpward();
	}
	//递归删除某个目录下的所有文件,没有删除目录
	void static DeleteFileOfDirRecursion(Dir* targetdir)
	{
		for (map<string, FileStruct*>::iterator dir = targetdir->FileOrDir.begin(); dir != targetdir->FileOrDir.end();)
		{
			//递归
			if (dir->second->GetFType() == "Dir")
			{
				//转换为Dir类型之后递归打印
				DeleteFileOfDirRecursion(dynamic_cast<Dir*>(dir->second));
				dir++;
			}
			//是文件
			else
			{
				//指向父目录的指针置空
				dir->second->SetFatherDir(nullptr);
				//删除对象
				delete dir->second;
				//指向空
				dir->second = nullptr;
				//从map中移除
				targetdir->FileOrDir.erase(dir++);
			}
		}

		targetdir->CountDirSizeUpward();

	}

	//递归删除目录结构
	void static DeleteEmptyDirRecursion(Dir* targetdir)
	{
		for (map<string, FileStruct*>::iterator dir = targetdir->FileOrDir.begin(); dir != targetdir->FileOrDir.end();)
		{
			//递归
			if (dir->second->GetFType() == "Dir")
			{
				if (dynamic_cast<Dir*>(dir->second)->FileOrDir.size() != 0)
				{
					DeleteEmptyDirRecursion(dynamic_cast<Dir*>(dir->second));
				}
				else
				{
					//指向父目录的指针置空
					dir->second->SetFatherDir(nullptr);
					//删除对象
					delete dir->second;
					//指向空
					dir->second = nullptr;
					//从map中移除
					targetdir->FileOrDir.erase(dir++);
				}
			}
			
		}
	}

	//输出该目录下的所有文件和文件夹
	void PrintAllFileAndDir()
	{
		size_t filenum = 0;
		size_t allfilesize = 0;
		cout << std::setw(20) << "创建时间" << std::setw(20) << "修改时间" << std::setw(10) << "类型" << std::setw(10) << "大小" << std::setw(10) << "名称" << endl;
		cout << std::setw(20) << this->CreateTime << std::setw(20) << this->ModifyTime << std::setw(10) << this->FType << std::setw(10) << this->Size << std::setw(10) << "." << endl;

		if (this->FatherDir != this)
		{
			cout << std::setw(20) << this->FatherDir->GetCreateTime() << std::setw(20) << this->FatherDir->GetModifyTime() << std::setw(10) << this->FatherDir->GetFType() << std::setw(10) << "         " << std::setw(10) << ".." << endl;
		}
		for (auto f : this->FileOrDir)
		{
			if (f.second->GetFType() == "File")
			{
				filenum++;
				allfilesize += f.second->GetSize();
			}
			cout << std::setw(20) << f.second->GetCreateTime() << std::setw(20) << f.second->GetModifyTime() << std::setw(10) << f.second->GetFType() << std::setw(10) << f.second->GetSize() << std::setw(10) << f.second->GetName() << endl;
		}
		cout << "            " << filenum << "个文件" << "           " << allfilesize << "字节" << endl;
	}

	//只打印目录,可以和上面函数合并的，暂且不和
	void PrintAllDir()
	{
		cout << std::setw(20) << "创建时间" << std::setw(20) << "修改时间" << std::setw(10) << "类型" << std::setw(10) << "大小" << std::setw(10) << "名称" << endl;
		cout << std::setw(20) << this->CreateTime << std::setw(20) << this->ModifyTime << std::setw(10) << this->FType << std::setw(10) << this->Size << std::setw(10) << "." << endl;

		if (this->FatherDir != this)
		{
			cout << std::setw(20) << this->FatherDir->GetCreateTime() << std::setw(20) << this->FatherDir->GetModifyTime() << std::setw(10) << this->FatherDir->GetFType() << std::setw(10) << "          " << std::setw(10) << ".." << endl;
		}
		for (auto f : this->FileOrDir)
		{
			if (f.second->GetFType() == "Dir")
			{
				cout << std::setw(20) << f.second->GetCreateTime() << std::setw(20) << f.second->GetModifyTime() << std::setw(10) << f.second->GetFType() << std::setw(10) << f.second->GetSize() << std::setw(10) << f.second->GetName() << endl;
			}
		}
		cout << "            0个文件" << "           " << 0 << "字节" << endl;
	}

	//递归打印 参数目录下的所有文件和目录
	void static PrintAllFileAndDirRecursion(Dir* targetdir)
	{
		cout << endl;
		cout << endl << "  " << targetdir->GetPath() + "/" + targetdir->GetName() << " 的目录" << endl;
		targetdir->PrintAllFileAndDir();
		for (auto dir : targetdir->GetFileOrDir())
		{
			//递归
			if (dir.second->GetFType() == "Dir")
			{
				//转换为Dir类型之后递归打印
				PrintAllFileAndDirRecursion(dynamic_cast<Dir*>(dir.second));
			}
		}
	}

	//递归打印 参数目录下的所有目录
	void static PrintAllDirRecursion(Dir* targetdir)
	{

		cout << endl;
		cout << endl << "  " << targetdir->GetPath() + "/" + targetdir->GetName() << " 的目录" << endl;
		targetdir->PrintAllDir();
		for (auto dir : targetdir->GetFileOrDir())
		{
			//递归
			if (dir.second->GetFType() == "Dir")
			{
				//转换为Dir类型之后递归打印
				PrintAllDirRecursion(dynamic_cast<Dir*>(dir.second));
			}
		}
	}

	//递归修改该目录下的所有目录和文件路径
	void static ChangeAllFileAndDirPathRecursion(Dir* targetdir)
	{

		//目录修改路径
		//targetdir->SetPath(targetdir->GetFatherDir()->GetPath()+"/"+ targetdir->GetFatherDir()->GetName());
		for (auto dir : targetdir->GetFileOrDir())
		{
			//递归
			dir.second->SetPath(targetdir->GetPath() + "/" + targetdir->GetName());
			if (dir.second->GetFType() == "Dir")
			{
				//转换为Dir类型之后递归修改路径
				ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dir.second));
			}

		}
	}

	//返回此目录下的子目录和文件map
	map<string, FileStruct*> GetFileOrDir()
	{
		return this->FileOrDir;
	}

	//获取目录下所有的文件名
	void GetAllFileOfDir(vector<string>& files)
	{
		for (auto name : this->FileOrDir)
		{
			if (name.second->GetFType() == "File")
			{
				files.emplace_back(name.first);
			}
		}
	}

	//向上修改目录大小
	void CountDirSizeUpward()
	{
		Dir* dirptr = this;
		Dir* oldone = nullptr;
		while (dirptr != oldone)
		{
			size_t size = 0;
			for (auto f : dirptr->GetFileOrDir())
			{
				size += f.second->GetSize();
			}
			dirptr->SetSize(size);
			oldone = dirptr;
			dirptr = dynamic_cast<Dir*>(dirptr->GetFatherDir());
		}
	}

	
};


//文件
class File :public FileStruct
{
private:
	char* Content = nullptr;
public:
	File(string Name, string Path, Dir* FatherDir)
	{
		this->Name = Name;
		this->FType = "File";
		this->Path = Path;
		this->FatherDir = FatherDir;
	}

	//拷贝构造函数
	File(const File* obj)
	{
		cout << "拷贝构造" << endl;
		this->Name = obj->Name;
		this->FType = obj->FType;
		this->Path = obj->Path;
		this->FatherDir = obj->FatherDir;
		//this->Mklink = obj->Mklink;
		this->Size = obj->Size;
		this->CreateTime = obj->CreateTime;
		this->ModifyTime = obj->ModifyTime;

		try {
			this->Content = new char[this->Size];
		}
		catch (exception e)
		{
			Error::MemoryMallocError();
			return;
		}
		memcpy(this->Content, obj->Content, this->Size);
	}
	~File()
	{
		delete[] this->Content;
		cout << "File 的析构函数" << endl;
	}
	//获取文件内容
	char* GetContent()
	{
		return this->Content;
	}
	
	void PrintContent()
	{
	
		if (this->Content != nullptr)
		{
			cout << this->Content << endl;
		}
	}


	void SetContent(char* Content,size_t Size)
	{

		this->Size = Size;
		this->Content = Content;
	}
};
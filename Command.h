#pragma once
#include"head.h"
#include"FileStruct.h"
#include"VirtualDisk.h"
#include"Error.h"
#include"FileHelper.h"
#include"Help.h"
//命令模式
//抽象命令类
class Command
{
protected:
	//指令
	vector<string> CommandOrder;
	Dir* LocalDir = nullptr;
public:
	Command()
	{
		this->LocalDir = nullptr;
	}

	Command(Dir* LocalDir, vector<string> CommandOrder)
	{
		this->LocalDir = LocalDir;
		this->CommandOrder = CommandOrder;
	}
	virtual ~Command()
	{
		vector<string>().swap(this->CommandOrder);
		LocalDir = nullptr;
	}

	//判断命令是否合法
	virtual bool IsCommandCorrect() = 0;
	//执行命令
	virtual void Execute(VirtualDisk* virtualdisk) = 0;

	//是否退出?
	virtual bool IsQuit() { return false; }

};

//命令模式
//具体命令类
//列出当前目录下的目录和文件
//针对目录
class DirCommand :public Command
{
private:
	/*  说明     列出文件和文件夹                   | 命令格式                 | 执行语句
	*  0 执行dir 不带参数                           | dir                      | dir .
	*  1 执行dir 带参数path                         | dir path                 | dir path
	*  2 执行dir 带参数/ad                          | dir /ad                  | dir /ad .
	*  3 执行dir 带参数/s                           | dir /s                   | dir /s .
	*  4 执行dir 带参数/s path                      | dir /s path              | dir /s path      支持名叫s的目录
	*  5 执行dir 带参数/ad path                     | dir /ad path             | dir /ad path     支持名叫ad的目录
	*  6 执行dir 带参数/s /ad  或 /ad /s            | dir /s /ad               | dir /s /ad .
	*  7 执行dir 带参数/ad /s path或者/s /ad path   | dir /s /ad path          | dir /ad /s path  支持名叫/ad /s 的目录
	*
	*
	*/
	int ExecType;
	//多路径命令
	vector<string> PathSet;
public:
	//执行命令的路径
	//DirCommand() {}
	DirCommand(Dir* dir, vector<string> CommandOrder)
	{

		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
		this->ExecType = 0;
	}
	~DirCommand()
	{
		vector<string>().swap(this->PathSet);
	}
	//判断语句是否合法
	bool IsCommandCorrect();

	Dir* GetPathDir(VirtualDisk* virtualdisk, string path);

	//合法则执行语句
	void Execute(VirtualDisk* virtualdisk);
};

//创建目录命令
class MkdirCommand :public Command
{
public:
	MkdirCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect();

	void Execute(VirtualDisk* virtualdisk);
};


//切换目录命令
class CdCommand :public Command
{
public:
	CdCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}

	bool IsCommandCorrect()
	{
		
		//判断参数是否合法
		if (this->CommandOrder.size() != 2)
		{
			Help::HelpCdCommand();
			return false;
		}
		return true;
	}

	void Execute(VirtualDisk* virtualdisk);
};


//拷贝文件命令
class CopyCommand :public Command
{
private:
	//目标目录
	string TargetDir = "";
	//拷贝的类型,
	// 1 真实磁盘拷贝到真实磁盘（磁盘到内存,内存到磁盘）
	// 2 真实磁盘拷贝到虚拟磁盘（磁盘到内存）
	// 3 虚拟磁盘拷贝到真实磁盘（内存到磁盘）
	// 4 虚拟磁盘拷贝到虚拟磁盘（内存到内存）

	int CopyType=0;
	//需要拷贝的文件
	vector<string>  TargetFile;
	
public:
	CopyCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	~CopyCommand()
	{
		vector<string>().swap(this->TargetFile);
	}
	bool IsCommandCorrect();

	//获取磁盘目录下的所有文件的名字，返回状态
	bool getFileNames(string path, vector<string>& files);

	//通配符匹配
	bool WildcardMatching(string path, string inputpath);

	//返回目录指针，如果type=true，则path必须是一个文件夹路径，如果type=false，给定的path是一个文件路径，返回文件父目录
	Dir* GetPathDir(VirtualDisk* virtualdisk, string path, bool type);

	//根据路径判断是目录还是文件，或者没有找到
	// 0 Error 未找到
	// 1 FIle 文件
	// 2 Dir 目录
	int FileOrDirBaseOnPath(string Path);

	void Execute(VirtualDisk* virtualdisk);
};

//新建文件命令
class TouchCommand :public Command
{
public:
	TouchCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect();

	void Execute(VirtualDisk* virtualdisk);
};


//删除文件或者目录命令
class DelCommand :public Command
{
private:
	//多路径命令
	vector<string> PathSet;
	//默认不递归
	bool Recursion = false;
public:
	DelCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	~DelCommand()
	{
		vector<string>().swap(this->PathSet);
	}
	bool IsCommandCorrect();

	void SpecialPathExecute(vector<string> parapath, int index, Dir* targetdir);

	void DeleteFileInPath(VirtualDisk* virtualdisk, string path);
	void Execute(VirtualDisk* virtualdisk)
	{
		for (auto path : this->PathSet)
		{
			this->DeleteFileInPath(virtualdisk, path);
		}
	}
};


//删除目录命令

class RdCommand :public Command
{
private:
	//多路径命令
	vector<string> PathSet;
	//默认不递归
	bool Recursion = false;
public:
	RdCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	~RdCommand()
	{
		vector<string>().swap(this->PathSet);
	}

	bool IsCommandCorrect();

	void SpecialPathExecute(Dir* searchdir, map<string, FileStruct*> containdirandfile);

	void DeleteFileInPath(VirtualDisk* virtualdisk, string path);

	void Execute(VirtualDisk* virtualdisk)
	{
		for (auto path : this->PathSet)
		{
			DeleteFileInPath(virtualdisk, path);
		}
	}
};

//重命名命令

class RenameCommand :public Command
{
public:
	RenameCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}

	bool IsCommandCorrect();

	void Execute(VirtualDisk* virtualdisk);
};


//移动命令
class MoveCommand :public Command
{
private:
	bool Cover = false;
	string SrcPath;
	string DstPath;

public:
	MoveCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
		//默认不覆盖
		this->Cover = false;
	}

	bool IsCommandCorrect();
	//给定一个路径，返回他的所在的目录对象   type是true的时候，取最后一级目录，false的时候不取
	// move path1 path2      move   a/b/c/d   e/f/g/h    对于sec来说 a/b/c/d都需要存在 对于 dstpath 来说  e/f/g 存在 h 不要求存在 
	Dir* GetPathDir(VirtualDisk* virtualdisk, string path, bool type);

	void Execute(VirtualDisk* virtualdisk);
};


//链接命令
class MklinkCommand :public Command
{
public:
	MklinkCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect() { return false; }
	void Execute(VirtualDisk* virtualdisk) {}
};



//查看文本文件内容命令
class CatCommand :public Command
{
public:
	CatCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect()
	{
		
		if (this->CommandOrder.size() != 2)
		{
			Help::HelpCatCommand();
			return false;
		}
		return true;
	}
	void Execute(VirtualDisk* virtualdisk);
};


//保存虚拟磁盘
class SaveCommand :public Command
{
public:
	SaveCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect()
	{
		
		if (this->CommandOrder.size() != 2)
		{
			Help::HelpSaveCommand();
			return false;
		}
		return true;
	}
	bool IsAbsolutePath(string Path)
	{
		if (Path.size() > 2 && Path[1] == ':') { return true; }
		return false;
	}

	void WriteFileStruct(FileStruct* filestruct, ofstream& fout);
	void WriteFile(File* targetfile, ofstream& fout);
	void WriteDir(Dir* targetdir, ofstream& fout);
	void Execute(VirtualDisk* virtualdisk);
};



//载入虚拟磁盘
class LoadCommand :public Command
{
public:
	LoadCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect()
	{
		
		if (this->CommandOrder.size() != 2)
		{
			Help::HelpLoadCommand();
			return false;
		}
		return true;
	}
	bool IsAbsolutePath(string Path)
	{
		if (Path.size() > 2 && Path[1] == ':') { return true; }
		return false;
	}
	void ReadFileStruct(FileStruct* filestruct, ifstream& fin);
	void ReadFile(File* targetfile, ifstream& fin);
	void ReadDir(Dir* targetdir, ifstream& fin);

	void Execute(VirtualDisk* virtualdisk);
};



//帮助命令
class HelpCommand :public Command
{
public:
	HelpCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect()
	{
		
		if (this->CommandOrder.size() != 2)
		{
			Help::HelpHelpCommand();
			return false;
		}
		return true;
	}
	void Execute(VirtualDisk* virtualdisk)
	{
		
		if (this->CommandOrder[1] == "dir") { Help::HelpDirCommand(); }
		else if (this->CommandOrder[1] == "mkdir") { Help::HelpMkdirCommand(); }
		else if (this->CommandOrder[1] == "rd") { Help::HelpRdCommand(); }
		else if (this->CommandOrder[1] == "cd") { Help::HelpCdCommand(); }
		else if (this->CommandOrder[1] == "touch") { Help::HelpTouchCommand(); }
		else if (this->CommandOrder[1] == "del") { Help::HelpDelCommand(); }
		else if (this->CommandOrder[1] == "copy") { Help::HelpCopyCommand(); }
		else if (this->CommandOrder[1] == "ren") { Help::HelpRenameCommand(); }
		else if (this->CommandOrder[1] == "move") { Help::HelpMoveCommand(); }
		else if (this->CommandOrder[1] == "mklink") { Help::HelpMklinkCommand(); }
		else if (this->CommandOrder[1] == "save") { Help::HelpSaveCommand(); }
		else if (this->CommandOrder[1] == "load") { Help::HelpLoadCommand(); }
		else if (this->CommandOrder[1] == "cat") { Help::HelpCatCommand(); }
		else if (this->CommandOrder[1] == "cls") { Help::HelpClsCommand(); }
		else { Help::HelpHelpCommand(); }
	}
};


//链接命令
class ClsCommand :public Command
{
public:
	ClsCommand(Dir* dir, vector<string> CommandOrder)
	{
		this->CommandOrder = CommandOrder;
		this->LocalDir = dir;
	}
	bool IsCommandCorrect()
	{
		
		if (this->CommandOrder.size() != 1)
		{
			Help::HelpClsCommand();
			return false;
		}
		return true;
	}
	void Execute(VirtualDisk* virtualdisk)
	{
		system("cls");
	}
};


//退出
class QuitCommand :public Command
{
public:
	QuitCommand() {}
	bool IsCommandCorrect() { return false; }
	void Execute(VirtualDisk* virtualdisk) {}
	bool IsQuit() { return true; }
};
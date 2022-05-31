#pragma once
#include"head.h"
#include"FileStruct.h"
#include"VirtualDisk.h"
#include"Error.h"
#include"FileHelper.h"
#include"Help.h"
//����ģʽ
//����������
class Command
{
protected:
	//ָ��
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

	//�ж������Ƿ�Ϸ�
	virtual bool IsCommandCorrect() = 0;
	//ִ������
	virtual void Execute(VirtualDisk* virtualdisk) = 0;

	//�Ƿ��˳�?
	virtual bool IsQuit() { return false; }

};

//����ģʽ
//����������
//�г���ǰĿ¼�µ�Ŀ¼���ļ�
//���Ŀ¼
class DirCommand :public Command
{
private:
	/*  ˵��     �г��ļ����ļ���                   | �����ʽ                 | ִ�����
	*  0 ִ��dir ��������                           | dir                      | dir .
	*  1 ִ��dir ������path                         | dir path                 | dir path
	*  2 ִ��dir ������/ad                          | dir /ad                  | dir /ad .
	*  3 ִ��dir ������/s                           | dir /s                   | dir /s .
	*  4 ִ��dir ������/s path                      | dir /s path              | dir /s path      ֧������s��Ŀ¼
	*  5 ִ��dir ������/ad path                     | dir /ad path             | dir /ad path     ֧������ad��Ŀ¼
	*  6 ִ��dir ������/s /ad  �� /ad /s            | dir /s /ad               | dir /s /ad .
	*  7 ִ��dir ������/ad /s path����/s /ad path   | dir /s /ad path          | dir /ad /s path  ֧������/ad /s ��Ŀ¼
	*
	*
	*/
	int ExecType;
	//��·������
	vector<string> PathSet;
public:
	//ִ�������·��
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
	//�ж�����Ƿ�Ϸ�
	bool IsCommandCorrect();

	Dir* GetPathDir(VirtualDisk* virtualdisk, string path);

	//�Ϸ���ִ�����
	void Execute(VirtualDisk* virtualdisk);
};

//����Ŀ¼����
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


//�л�Ŀ¼����
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
		
		//�жϲ����Ƿ�Ϸ�
		if (this->CommandOrder.size() != 2)
		{
			Help::HelpCdCommand();
			return false;
		}
		return true;
	}

	void Execute(VirtualDisk* virtualdisk);
};


//�����ļ�����
class CopyCommand :public Command
{
private:
	//Ŀ��Ŀ¼
	string TargetDir = "";
	//����������,
	// 1 ��ʵ���̿�������ʵ���̣����̵��ڴ�,�ڴ浽���̣�
	// 2 ��ʵ���̿�����������̣����̵��ڴ棩
	// 3 ������̿�������ʵ���̣��ڴ浽���̣�
	// 4 ������̿�����������̣��ڴ浽�ڴ棩

	int CopyType=0;
	//��Ҫ�������ļ�
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

	//��ȡ����Ŀ¼�µ������ļ������֣�����״̬
	bool getFileNames(string path, vector<string>& files);

	//ͨ���ƥ��
	bool WildcardMatching(string path, string inputpath);

	//����Ŀ¼ָ�룬���type=true����path������һ���ļ���·�������type=false��������path��һ���ļ�·���������ļ���Ŀ¼
	Dir* GetPathDir(VirtualDisk* virtualdisk, string path, bool type);

	//����·���ж���Ŀ¼�����ļ�������û���ҵ�
	// 0 Error δ�ҵ�
	// 1 FIle �ļ�
	// 2 Dir Ŀ¼
	int FileOrDirBaseOnPath(string Path);

	void Execute(VirtualDisk* virtualdisk);
};

//�½��ļ�����
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


//ɾ���ļ�����Ŀ¼����
class DelCommand :public Command
{
private:
	//��·������
	vector<string> PathSet;
	//Ĭ�ϲ��ݹ�
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


//ɾ��Ŀ¼����

class RdCommand :public Command
{
private:
	//��·������
	vector<string> PathSet;
	//Ĭ�ϲ��ݹ�
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

//����������

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


//�ƶ�����
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
		//Ĭ�ϲ�����
		this->Cover = false;
	}

	bool IsCommandCorrect();
	//����һ��·���������������ڵ�Ŀ¼����   type��true��ʱ��ȡ���һ��Ŀ¼��false��ʱ��ȡ
	// move path1 path2      move   a/b/c/d   e/f/g/h    ����sec��˵ a/b/c/d����Ҫ���� ���� dstpath ��˵  e/f/g ���� h ��Ҫ����� 
	Dir* GetPathDir(VirtualDisk* virtualdisk, string path, bool type);

	void Execute(VirtualDisk* virtualdisk);
};


//��������
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



//�鿴�ı��ļ���������
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


//�����������
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



//�����������
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



//��������
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


//��������
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


//�˳�
class QuitCommand :public Command
{
public:
	QuitCommand() {}
	bool IsCommandCorrect() { return false; }
	void Execute(VirtualDisk* virtualdisk) {}
	bool IsQuit() { return true; }
};
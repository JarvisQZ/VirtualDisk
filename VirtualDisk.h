#pragma once
#include"head.h"
#include"FileStruct.h"

//虚拟磁盘类  单例模式
class VirtualDisk
{
private:
	VirtualDisk(Dir* dir)
	{
		this->NowDir = dir;
	}
	static VirtualDisk* virtualdisk;
	//string VirtualPath;
	//string RealSavePath;
	//根目录
	static Dir* RootDir;
	//当前路径
	Dir* NowDir;
public:
	static const string VirtualPath;
	static const string RealSavePath;
	static VirtualDisk* GetVirtualDisk()
	{
		if (virtualdisk == nullptr)
		{
			cout << "创建虚拟磁盘" << endl;
			//this->NowDir=new Dir(FileHelper::VirtualPath)
			Dir* root = new Dir("", VirtualDisk::VirtualPath, nullptr);
			RootDir = root;
			virtualdisk = new VirtualDisk(root);
			//一开始当前路径等于根路径
		}
		return virtualdisk;
	}


	Dir* GetLocalDir()
	{
		return this->NowDir;
	}

	void SetLocalDir(Dir* localdir)
	{
		this->NowDir = localdir;
	}

	static Dir* GetRootDir()
	{
		return RootDir;
	}
	~VirtualDisk()
	{
		delete RootDir;
		RootDir = nullptr;
		virtualdisk = nullptr;
		cout << "析构函数" << endl;
	}
};

#pragma once
#include"head.h"
#include"FileStruct.h"

//���������  ����ģʽ
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
	//��Ŀ¼
	static Dir* RootDir;
	//��ǰ·��
	Dir* NowDir;
public:
	static const string VirtualPath;
	static const string RealSavePath;
	static VirtualDisk* GetVirtualDisk()
	{
		if (virtualdisk == nullptr)
		{
			cout << "�����������" << endl;
			//this->NowDir=new Dir(FileHelper::VirtualPath)
			Dir* root = new Dir("", VirtualDisk::VirtualPath, nullptr);
			RootDir = root;
			virtualdisk = new VirtualDisk(root);
			//һ��ʼ��ǰ·�����ڸ�·��
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
		cout << "��������" << endl;
	}
};

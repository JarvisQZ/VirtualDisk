#pragma once
#include"Command.h"

//�ж�����Ƿ�Ϸ�
bool DirCommand::IsCommandCorrect()
{


	//�޲���
	if (this->CommandOrder.size() == 1)
	{
		// ��ΪĬ��ִ��ģʽ�� 0���Բ����ı�
		return true;
	}
	//һ������
	else if (this->CommandOrder.size() == 2)
	{
		//ֻ��/ad����������·��
		if (this->CommandOrder[1] == "/ad")
		{
			this->ExecType = 2;
			return true;
		}
		//ֻ��/s����������·��
		else if (this->CommandOrder[1] == "/s")
		{
			this->ExecType = 3;
			return true;
		}
		//ֻ��·������������
		else
		{
			this->ExecType = 1;
			this->PathSet.emplace_back(this->CommandOrder[1]);
			return true;
		}
	}
	//�������
	else
	{

		if (this->CommandOrder.size() == 3)
		{
			//��������� dir /s /ad  �� dir /ad /s
			if ((this->CommandOrder[1] == "/ad" && (this->CommandOrder[2] == "/s")) || (this->CommandOrder[1] == "/s" && (this->CommandOrder[2] == "/ad")))
			{
				this->ExecType = 6;
				return true;
			}
		}
		else
		{
			//��������� dir /s /ad  path (path) ... �� dir /ad /s path (path) ...
			if ((this->CommandOrder[1] == "/ad" && (this->CommandOrder[2] == "/s")) || (this->CommandOrder[1] == "/s" && (this->CommandOrder[2] == "/ad")))
			{
				this->ExecType = 7;
				this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 3, this->CommandOrder.end());
				return true;
			}
		}

		// ���� dir /ad path (path)
		if (this->CommandOrder[1] == "/ad")
		{
			this->ExecType = 5;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
			return true;
		}
		// ���� dir /s path (path)
		else if (this->CommandOrder[1] == "/s")
		{
			this->ExecType = 4;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
			return true;
		}
		// ���� dir path1 path2
		else
		{
			this->ExecType = 1;
			//����·��������ȥ
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
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  ����·��  ����   /a/b/c  �� /��ͷ
		{
			//��ȡ��Ŀ¼����
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  �� .. ��ͷ������
		{
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			continue;
		}
		else
		{
			//Ŀ¼������,����
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
			//��map���ҵ������֣������ж����ļ�����Ŀ¼
			if (containdirandfile.find(parapath[i]) != containdirandfile.end())
			{
				//������ļ�,����,˵��û�����Ŀ¼
				if (containdirandfile[parapath[i]]->GetFType() == "File")
				{
					//��Ŀ¼
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
				//�����Ŀ¼
				else
				{
					//��ȡ��Ŀ¼������ת��ΪĿ¼����
					searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
					containdirandfile = searchdir->GetFileOrDir();
				}
			}
		}
	}
	return searchdir;
}

//�Ϸ���ִ�����
void DirCommand::Execute(VirtualDisk* virtualdisk)
{

	//ֻ����˼�Ŀ¼�µ��ļ���Ŀ¼
	if (this->ExecType == 0)
	{
		this->LocalDir->PrintAllFileAndDir();
	}
	//���path·���µ��ļ���Ŀ¼
	else if (this->ExecType == 1)
	{
		for (auto path : this->PathSet)
		{
			Dir* tempdir = this->GetPathDir(virtualdisk, path);
			if (tempdir == nullptr)
			{
				return;
			}
			cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " ��Ŀ¼" << endl;
			tempdir->PrintAllFileAndDir();
			//cout << "            " << tempdir->GetNumOfFiles() << "���ļ�" << "           " << sizeof(tempdir) << "�ֽ�"<<endl;
		}
	}
	// �����ǰĿ¼�µ�Ŀ¼ /ad
	else if (this->ExecType == 2)
	{
		this->LocalDir->PrintAllDir();
	}
	// ���Ŀ¼����������Ŀ¼�������ļ�
	else if (this->ExecType == 3)
	{
		Dir::PrintAllFileAndDirRecursion(this->LocalDir);
	}
	// ִ��dir ������/s path
	else if (this->ExecType == 4)
	{
		for (auto path : this->PathSet)
		{
			Dir* tempdir = this->GetPathDir(virtualdisk, path);
			if (tempdir == nullptr)
			{
				return;
			}
			//cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " ��Ŀ¼" << endl;
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
			cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " ��Ŀ¼" << endl;
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
			//cout << endl << "  " << tempdir->GetPath() + "/" + tempdir->GetName() << " ��Ŀ¼" << endl;
			Dir::PrintAllDirRecursion(tempdir);
		}
	}
}
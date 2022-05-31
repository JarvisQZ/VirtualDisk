#pragma once
#include"Command.h"

bool TouchCommand::IsCommandCorrect()
{


	//�жϲ����Ƿ�Ϸ�
	if (this->CommandOrder.size() != 2)
	{
		Help::HelpTouchCommand();
		return false;
	}
	else
	{
		// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��
		vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

		//�ж�·�������Ƿ�Ϸ�  �Ƿ���������ַ�������   *a/b/c   a/*b/c   C:/*a/b/c  ../*a/b/c  ./a/b/c
		for (auto name : parapath)
		{
			if (name == "C:")
				continue;
			//·���а����� ��  a//b/c
			if (name == "" && name != parapath[0])
			{
				Error::PathFormatError();
				return false;
			}
			if (!FileHelper::IsContainIllegalChar(name))
			{
				Error::NameError();
				return false;
			}
		}
		return true;
	}
}

void TouchCommand::Execute(VirtualDisk* virtualdisk)
{

	// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��
	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  ����·��  ���� touch  /a/b/c  �� /��ͷ
		{
			if (i == parapath.size() - 1)
			{
				Error::VisitRootError();
				return;
			}
			//��ȡ��Ŀ¼����
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  �� .. ��ͷ������
		{
			if (i == parapath.size() - 1)
			{
				Error::CreateFatherSelfError();
				return;
			}
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			if (i == parapath.size() - 1)
			{
				Error::CreateSelfError();
				return;
			}
			continue;
		}
		else
		{
			//�ļ�����ǰ��Ŀ¼    ����Ҫ����c    a/b/c   ������a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::TouchNoDirFileError(searchdir->GetPath() + "/" + parapath[i]);
					return;
				}
				else
				{
					//�ҵ������ֵ��������ļ�
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
						return;
					}
					else
					{
						//��ȡ��Ŀ¼������ת��ΪĿ¼����
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
				}
			}
			else
			{
				//û���ļ����������,�½��ļ�
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					string path;
					if (searchdir == virtualdisk->GetRootDir())
					{
						path = searchdir->GetPath();
					}
					else
					{
						path = searchdir->GetPath() + "/" + searchdir->GetName();
					}
					File* NewFile = new File(parapath[i], path, searchdir);
					searchdir->FileOrDirAdd(parapath[i], NewFile);
					return;
				}
				else
				{
					//�ҵ���ͬ����Ŀ¼
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						Error::FileNameSaveAsDirError();
						return;
					}
					//�޸�ʱ��
					else
					{
						dynamic_cast<File*>(containdirandfile[parapath[i]])->SetModifyTime();
					}
				}
			}
		}
	}
}
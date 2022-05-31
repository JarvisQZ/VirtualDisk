#pragma once
#include"Command.h"

bool RenameCommand::IsCommandCorrect()
{

	//�жϲ����Ƿ�Ϸ�
	if (this->CommandOrder.size() != 3)
	{
		Help::HelpRenameCommand();
		return false;
	}
	// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��,������Ϊ������
	if (!FileHelper::IsContainIllegalChar(this->CommandOrder[2]))
	{
		Error::NameError();
		return false;
	}
	return true;
}

void RenameCommand::Execute(VirtualDisk* virtualdisk)
{

	// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��,������Ϊ������
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
				Error::VisitRootError();
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
				Error::CannotVisitError();
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
					Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
					return;
				}
				else
				{
					//�ҵ������ֵ��������ļ�
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
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
				//û���ļ����������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindFileError(searchdir->GetPath(), parapath[i]);
					return;
				}
				else
				{
					//�������Ѿ�������
					if (containdirandfile.find(this->CommandOrder[2]) != containdirandfile.end())
					{
						if (containdirandfile[this->CommandOrder[2]]->GetFType() == "Dir")
						{
							Error::RenameSaveAsDirError(this->CommandOrder[2]);

						}
						else
						{
							Error::RenameSaveAsFileError(this->CommandOrder[2]);
						}
						return;
					}
					//�Ȱ��Լ����ָ���
					containdirandfile[parapath[i]]->SetName(this->CommandOrder[2]);
					//�����ֲ�����,������ָ��ָ�������
					searchdir->FileOrDirAdd(this->CommandOrder[2], containdirandfile[parapath[i]]);
					//ɾ�������ֵ�ָ��ָ��
					searchdir->RemoveFromMap(parapath[i]);

				}
			}
		}
	}
}
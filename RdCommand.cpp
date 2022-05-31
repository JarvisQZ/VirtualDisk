#pragma once
#include"Command.h"

bool RdCommand::IsCommandCorrect()
{

	//�жϲ����Ƿ�Ϸ�
	if (this->CommandOrder.size() == 1)
	{
		Help::HelpRdCommand();
		return false;
	}
	else if (this->CommandOrder.size() == 2)
	{
		this->PathSet.emplace_back(this->CommandOrder[1]);
		return true;
	}
	//�������ڵ�������
	else
	{
		//����������ʱ���ڶ�����/s ��ʾ�����������ļ�����Ŀ¼������
		if (this->CommandOrder[1] == "/s")
		{
			//�ݹ�ɾ���ļ�
			this->Recursion = true;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
		}
		//���û��/s ֮����·��
		else
		{
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 1, this->CommandOrder.end());
		}

	}

	return true;
}

void RdCommand::SpecialPathExecute(Dir* searchdir, map<string, FileStruct*> containdirandfile)
{
	if (this->Recursion)
	{
		//ɾ��Ŀ¼�������ļ���Ŀ¼���������Լ�
		Dir::DeleteFileOfDirRecursion(searchdir);
		Dir::DeleteEmptyDirRecursion(searchdir);
		string name = searchdir->GetName();
		searchdir = dynamic_cast<Dir*>(searchdir->GetFatherDir());
		containdirandfile = searchdir->GetFileOrDir();
		//ɾ��Ŀ¼����
		delete containdirandfile[name];
		//ָ���
		containdirandfile[name] = nullptr;
		//��map���Ƴ�
		searchdir->RemoveFromMap(name);
	}
	else
	{
		//�ж��Ƿ�Ϊ��
		if (containdirandfile.size() != 0)
		{
			Error::DeleteDirNotEmptyError();
		}
		else
		{
			string name = searchdir->GetName();
			searchdir = dynamic_cast<Dir*>(searchdir->GetFatherDir());
			containdirandfile = searchdir->GetFileOrDir();
			//ɾ��Ŀ¼����
			delete containdirandfile[name];
			//ָ���
			containdirandfile[name] = nullptr;
			//��map���Ƴ�
			searchdir->RemoveFromMap(name);
		}
	}
}

void RdCommand::DeleteFileInPath(VirtualDisk* virtualdisk, string path)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  ����·��  ���� del  /a/b/c  �� /��ͷ
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
				Error::DeleteDirNotEmptyError();
				return;
			}
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			if (i != parapath.size() - 1)
			{
				continue;
			}
			else
			{
				//�����ڵ�ǰĿ¼,���ܽ���ɾ�������򱨴�
				if (searchdir != virtualdisk->GetLocalDir())
				{
					SpecialPathExecute(searchdir, containdirandfile);
				}
				else
				{
					Error::CannotVisitError();
					return;
				}
			}

		}
		else
		{
			//�ļ�����ǰ��Ŀ¼        a/b/c   ������a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
					return;
				}
				else
				{
					//�ҵ�������,�����ļ�������
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
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
				//û���ļ�����Ŀ¼���������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindDirError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
					return;
				}
				else
				{
					//�ҵ���ͬ����Ŀ¼,�ݹ�ɾ����Ŀ¼�������ļ������ݲ���
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						//��ȡ��Ŀ¼������ת��ΪĿ¼����
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
						//��ǰĿ¼����Ҫɾ����Ŀ¼��ִ��ɾ��
						if (searchdir != virtualdisk->GetLocalDir())
						{
							SpecialPathExecute(searchdir, containdirandfile);
						}
						else
						{
							Error::CannotVisitError();
							return;
						}
					}
					//������ļ����ͱ���
					else
					{
						Error::InvalidDirNameError();
						return;
					}
				}
			}
		}
	}
}


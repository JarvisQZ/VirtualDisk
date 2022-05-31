#pragma once
#include"Command.h"

bool CopyCommand::IsCommandCorrect()
{

	//�жϲ����Ƿ�Ϸ�
	if (this->CommandOrder.size() != 3)
	{
		Help::HelpCopyCommand();
		return false;
	}
	else
	{

		//�����һ��������@��ͷ����ô������ʵ·��
		if (this->CommandOrder[1][0] == '@')
		{
			//�洢��Ŀ¼�������ļ�
			vector<string> allfile;
			//ȥ��@����
			string realpath = this->CommandOrder[1].substr(1, this->CommandOrder[1].size() - 1);


			// ͨ���ģʽ
			string regex;

			size_t pos = realpath.find_last_of('/');
			//˵��·������û�� /  �Ǿ��ڵ�ǰĿ¼�£���realpath����regex
			if (pos == string::npos)
			{
				regex = realpath;
				//�ӵ�ǰĿ¼���ң���ǰĿ¼�϶����ڲ����ж�
				getFileNames(".", allfile);
			}
			else
			{
				//Ŀ¼����·�� ����Ǵ��� /��     C:/a/b/
				this->TargetDir = realpath.substr(0, pos + 1);

				//�ļ�ͨ�����
				regex = realpath.substr(pos + 1, realpath.size() - 1);
				//��Ŀ��·������,Ŀ��·�������ھͱ���
				if (!getFileNames(realpath.substr(0, pos), allfile))
				{
					//������û�и�Ŀ¼
					Error::CannotFindPathError(realpath.substr(0, pos));
					return false;
				}

			}

			for (auto filename : allfile)
			{
				//���ƥ��ɹ�
				if (WildcardMatching(filename, regex))
				{
					this->TargetFile.emplace_back(filename);
				}
			}

			//��������ļ����������һ����û��ƥ���ϣ�����
			if (TargetFile.size() == 0)
			{
				Error::NoFileMatchedError();
				return false;
			}

			//�ڶ���������@��ͷ��
			if (this->CommandOrder[2][0] == '@')
			{
				// 1 ��ʵ���̿�������ʵ���̣����̵��ڴ�,�ڴ浽���̣�
				this->CopyType = 1;
			}
			else
			{
				// 2 ��ʵ���̿�����������̣����̵��ڴ棩
				this->CopyType = 2;
			}
		}
		else
		{
			if (this->CommandOrder[2][0] == '@')
			{
				// 3 ������̿�������ʵ���̣��ڴ浽���̣�
				this->CopyType = 3;
			}
			else
			{
				// 4 ������̿�����������̣��ڴ浽�ڴ棩
				this->CopyType = 4;
			}
		}

	}
	return true;
}

//��ȡ����Ŀ¼�µ������ļ������֣�����״̬
bool CopyCommand::getFileNames(string path, vector<string>& files)
{
	//�ļ����
	//ע�⣺����˴���long���ͣ�ʵ�������лᱨ������쳣
	intptr_t hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	//string p;p.assign(path).append("/*").c_str()
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����
			//�������,���ļ�����·������vector��
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
		//ʧ��
		return false;
	}
	return true;
}

//ͨ���ƥ��
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

//����Ŀ¼ָ�룬���type=true����path������һ���ļ���·�������type=false��������path��һ���ļ�·���������ļ���Ŀ¼
Dir* CopyCommand::GetPathDir(VirtualDisk* virtualdisk, string path, bool type)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	if (!type)
	{
		//ֻ�ж�ǰ����Ŀ¼�Ƿ����
		parapath.pop_back();
	}
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (size_t  i = 0; i < parapath.size(); i++)
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
			//��������һ��
			if (i == parapath.size() - 1)
			{
				//Ŀ¼������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
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
				//��map���ҵ������֣������ļ�����Ŀ¼����������һ��
				else
				{
					//������ļ�,����
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
						//��ȡĿ¼���󷵻�
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
				}
			}
			//�������һ����ȫ��Ŀ¼������   a/b/c/d/r/f   a,b,c,d,r ȫ������Ŀ¼
			else
			{
				//Ŀ¼������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
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
	}
	return searchdir;
}

//����·���ж���Ŀ¼�����ļ�������û���ҵ�
// 0 Error δ�ҵ�
// 1 FIle �ļ�
// 2 Dir Ŀ¼
int CopyCommand::FileOrDirBaseOnPath(string Path)
{
	struct stat s;
	if (stat(Path.c_str(), &s) == 0)
	{
		//���ڵ�Ŀ¼
		if (s.st_mode & S_IFREG)
		{
			return 1;
		}
		//�ļ�
		else
		{
			return 2;
		}
	}
	//������
	else
	{
		return 0;
	}
}

void CopyCommand::Execute(VirtualDisk* virtualdisk)
{
	//��ȡ����Щ�ļ���������Ŀ¼�����TargetFile����ֻ��һ���ļ�����ôcopy path1 path2 �е�path2��һ���ļ���,���ظ�Ŀ¼
	// ���TargetFile�ж���ļ�����ôpath2��һ��Ŀ¼��

	string reapath = this->CommandOrder[2].substr(1, this->CommandOrder[2].size());
	// 1 ��ʵ���̿�������ʵ���̣����̵��ڴ�,�ڴ浽���̣�
	if (this->CopyType == 1)
	{
		//����һ���ļ�����һ�����е��ļ�,ֱ�Ӹ���
		if (this->TargetFile.size() == 1)
		{
			string dirfile;

			//Ŀ¼
			if (FileOrDirBaseOnPath(reapath) == 2)
			{
				dirfile = reapath + "/" + this->TargetFile[0];
			}
			else
			{
				dirfile = reapath;
			}
			//�ļ����ݻ���
			char* buffer;
			size_t size;
			//��¼�ļ��ֽڴ�С
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
			//cout << size << "��С" << buffer << "|" << endl;
			fin.close();

			//д�ļ�
			ofstream fout(dirfile, ios::out | ios::binary);
			fout.write(buffer, size);
			fout.close();
			delete[]buffer;
			buffer = nullptr;
		}
		//����ļ�
		else
		{

			//Ŀ¼
			if (FileOrDirBaseOnPath(reapath) == 2)
			{

				for (auto filename : this->TargetFile)
				{
					//�ļ����ݻ���
					char* buffer;
					size_t size;
					//��¼�ļ��ֽڴ�С
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
					//cout << size << "��С" << buffer << "|" << endl;
					fin.close();

					//д�ļ�
					ofstream fout(reapath + "/" + filename, ios::out | ios::binary);
					fout.write(buffer, size);
					fout.close();
					delete[] buffer;
					buffer = nullptr;
				}
			}
			//�ļ�,ֻ�����һ���ļ�����
			else
			{
				//�ļ����ݻ���
				char* buffer;
				size_t size;
				//��¼�ļ��ֽڴ�С
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
				//cout << size << "��С" << buffer << "|" << endl;
				fin.close();

				//д�ļ�
				ofstream fout(reapath, ios::out | ios::binary);
				fout.write(buffer, size);
				fout.close();
				delete[] buffer;
				buffer = nullptr;
			}

		}

	}
	// 2 ��ʵ���̿�����������̣����̵��ڴ棩
	else if (this->CopyType == 2)
	{
		Dir* DstDir;
		//��ȡ�������ĸ�Ŀ¼��ָ��
		if (this->TargetFile.size() == 1)
		{
			string newfilename;
			DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], false);
			vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[2]);
			newfilename = parapath[parapath.size() - 1];
			if (DstDir == nullptr) { return; }
			map<string, FileStruct*> containdirandfile = DstDir->GetFileOrDir();
			//������Ϊ Targetfile ������ļ���Ŀ¼
			if (containdirandfile.find(newfilename) != containdirandfile.end())
			{
				if (containdirandfile[newfilename]->GetFType() == "Dir")
				{
					DstDir = dynamic_cast<Dir*>(containdirandfile[newfilename]);
					newfilename = this->TargetFile[0];
				}
			}

			//��� copy path1 path2 �е�2�Ѿ����ڲ���Ϊ�ļ�������ִ�У����ΪĿ¼�����ļ����ڸ�Ŀ¼�£����ֲ���

			//�ļ����ݻ���
			char* buffer;
			size_t size;
			//size_t a;
			
			//��¼�ļ��ֽڴ�С
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
			//cout << size << "��С" << buffer << "|" << endl;
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
			//�½��ļ�
			File* newfile = new File(newfilename, path, DstDir);
			//��ָ��ָ��buffer��ָ�������
		
			newfile->SetContent(buffer,size);
			//��ȡ���ǵķ�ʽ
			DstDir->FileOrDirAdd(newfilename, newfile, true);
			//delete[] buffer;
			buffer = nullptr;
		}
		else
		{
			DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], true);
			if (DstDir == nullptr) { return; }
			//�ļ����ݻ���
			char* buffer;
			//��¼�ļ��ֽڴ�С
			size_t size;
			string path;

			//��ÿһ���ļ����������ֽڶ����ڴ�
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
				//�½��ļ�
				File* newfile = new File(file, path, DstDir);
				//��ָ��ָ��buffer��ָ�������

				

				newfile->SetContent(buffer, size);
				//��ȡ���ǵķ�ʽ
				DstDir->FileOrDirAdd(file, newfile, true);
				//delete[] buffer;
				buffer = nullptr;
			}
		}

	}

	else
	{
		//���������@��ͷ������������·��
		//�洢��Ŀ¼�������ļ�
		vector<string> allfile;
		string dirpath = this->CommandOrder[1];
		// ͨ���ģʽ
		string regex;
		Dir* SrcDir;
		size_t pos = dirpath.find_last_of('/');
		//˵��·������û�� /  �Ǿ��ڵ�ǰĿ¼�£�dirpath =VirtualDisk::localdir
		if (pos == string::npos)
		{
			regex = dirpath;
			//�ӵ�ǰĿ¼���ң���ǰĿ¼�϶����ڲ����ж�
			SrcDir = virtualdisk->GetLocalDir();
			SrcDir->GetAllFileOfDir(allfile);
		}
		else
		{
			//Ŀ¼����·�� ����Ǵ��� /��     C:/a/b/
			this->TargetDir = dirpath.substr(0, pos + 1);
			//��ȡԴĿ¼����
			SrcDir = GetPathDir(virtualdisk, this->TargetDir, true);
			//����д�
			if (SrcDir == nullptr) { return; }

			//��ȡ�ļ�����
			SrcDir->GetAllFileOfDir(allfile);
			//�ļ�ͨ�����
			regex = dirpath.substr(pos + 1, dirpath.size() - 1);
		}
		for (auto filename : allfile)
		{
			//���ƥ��ɹ�
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
		// 3 ������̿�������ʵ���̣��ڴ浽���̣�
		if (this->CopyType == 3)
		{
			//����һ���ļ�����һ�����е��ļ�,ֱ�Ӹ���
			if (this->TargetFile.size() == 1)
			{
				string dirfile;

				//Ŀ¼
				if (FileOrDirBaseOnPath(reapath) == 2)
				{
					dirfile = reapath + "/" + this->TargetFile[0];
				}
				//�ļ�
				else
				{
					dirfile = reapath;
				}
				//�ļ����ݻ���
				char* tempcontent = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[0]])->GetContent();
				size_t size = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[0]])->GetSize();


				//д�ļ�
				ofstream fout(dirfile, ios::out | ios::binary);
				fout.write(tempcontent, size);
				fout.close();

			}
			//����ļ�
			else
			{

				//Ŀ¼
				if (FileOrDirBaseOnPath(reapath) == 2)
				{
					map<string, FileStruct*> sourcefileanddir = SrcDir->GetFileOrDir();
					char* tempcontent;
					size_t size;
					for (auto filename : this->TargetFile)
					{
						tempcontent = dynamic_cast<File*>(sourcefileanddir[filename])->GetContent();
						size = dynamic_cast<File*>(sourcefileanddir[filename])->GetSize();


						//д�ļ�
						ofstream fout(reapath + "/" + filename, ios::out | ios::binary);
						fout.write(tempcontent, size);
						fout.close();

					}
				}
				//�ļ�,ֻ�����һ���ļ�����
				else
				{
					//�ļ����ݻ���
					char* tempcontent = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[this->TargetFile.size() - 1]])->GetContent();
					size_t size = dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[this->TargetFile.size() - 1]])->GetSize();


					//д�ļ�
					ofstream fout(reapath, ios::out | ios::binary);
					fout.write(tempcontent, size);
					fout.close();

				}
			}
		}
		// 4 ������̿�����������̣��ڴ浽�ڴ棩
		else
		{
			Dir* DstDir;
			//��ȡ�������ĸ�Ŀ¼��ָ��
			if (this->TargetFile.size() == 1)
			{
				string newfilename;
				DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], false);
				if (DstDir == nullptr) { return; }
				vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[2]);
				newfilename = parapath[parapath.size() - 1];
				map<string, FileStruct*> sourcefileanddir = DstDir->GetFileOrDir();

				//������Ϊ Targetfile ������ļ���Ŀ¼
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
				//�½��ļ�
				File* newfile = new File(dynamic_cast<File*>(SrcDir->GetFileOrDir()[this->TargetFile[0]]));
				//������·��
				newfile->SetPath(DstDir->GetPath() + "/" + DstDir->GetName());
				//���ø�Ŀ¼
				newfile->SetFatherDir(DstDir);
				//�������֣���ֹ�����������û�иı�
				newfile->SetName(newfilename);
				//�޸�ʱ��
				newfile->SetModifyTime();
				//��ȡ���Ƿ�ʽ
				DstDir->FileOrDirAdd(newfilename, newfile, true);

			}
			else
			{
				DstDir = GetPathDir(virtualdisk, this->CommandOrder[2], true);
				if (DstDir == nullptr) { return; }
				//����
				map<string, FileStruct*> sourcefileanddir = SrcDir->GetFileOrDir();
				for (auto file : this->TargetFile)
				{
					//�½��ļ�
					File* newfile = new File(dynamic_cast<File*>(sourcefileanddir[file]));
					//������·��
					newfile->SetPath(DstDir->GetPath() + "/" + DstDir->GetName());
					//���ø�Ŀ¼
					newfile->SetFatherDir(DstDir);
					//�޸�ʱ��
					newfile->SetModifyTime();
					//��ȡ���Ƿ�ʽ
					DstDir->FileOrDirAdd(file, newfile, true);
				}
			}
		}




	}
}
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include<string>
#include <stdlib.h>
#include<time.h>
#include<fstream>

using namespace std;
string nowDir;

string com[15] = { "cd","dir","mkdir","more","copy","rmdir","find","attrib","del","xcopy","import","export","exit","help","showAll" }; // 建立命令表格

class Dir
{
public:
	string name; //文件夹或者文件名字
	string prename;//上一级的文件夹名称
	time_t t;//创建时间
	//struct tm *p;
	//t = time(NULL);
	//p = localtime(&t);
	//printf("%d年%02d月%02d日", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	//printf("%02d:%02d:%02d\n", p->tm_hour, p->tm_min, p->tm_sec);

	int mod;//记录文件夹 1还是文件 2
	Dir* right;//文件夹的子文件夹或者文件
	Dir* next;//链表存储每一个文件夹
	Dir* pre;//记录上一级节点
	string document;
}*head, * ND; //头指针 和 记录当前位置的指针

void set() //创建文件夹的链表
{
	//打开文档
	fstream file;
	file.open("menu.txt");

	head = new Dir;
	Dir* p = new Dir;
	Dir* p1 = new Dir;
	Dir* p2 = new Dir;
	Dir* p3 = new Dir;

	int i = 0;   //规避最后一个节点先创建但是没数据输入，成为一个空节点的情况

	head->next = p;  //头指针指向第一个节点

	while (!file.eof()) //文档指针不指向空时
	{

		//规避最后一个节点先创建但是没数据输入，成为一个空节点的情况
		if (i != 0)
		{
			p3 = new Dir;
			p->next = p3;
			p = p3;
		}
		i = 1;


		//输入文件夹的信息
		file >> p->name;
		file >> p->mod;
		file >> p->prename;

		//创建的时间  即我第一次创建的时间，这个也可以写进初始化文档里面
		p->t = 1590370409;

		p->right = p1;
		p2 = p;
		while (1)  //循环输入文件夹中的子文件
		{
			string s;
			file >> s;

			if (s == "#")  //用#判断输入是否完成
			{
				p2->right = NULL;
				break;
			}

			p1->name = s;
			file >> p1->mod;

			//创建的时间
			p1->t = 1590370409;

			if (p1->mod == 2)//文件为txt文件时，输入txt文件里的内容
			{
				int i = 0; //第一个换行符不写入
				while (1)
				{
					char d;
					file.get(d);

					if (d == '#')
						break;
					if (i == 1)
						p1->document.push_back(d);
					i = 1;
				}
			}
			p2 = p1;
			p1 = new Dir;
			p2->right = p1;
		}
	}
	p->next = NULL;

	//记录上级文件夹的位置
	p = new Dir;
	p = head->next;
	while (p)
	{
		Dir* pp = new Dir;
		pp = head->next;
		while (pp)
		{
			if (pp->name.compare(p->prename) == 0)
				p->pre = pp;
			pp = pp->next;
		}
		p = p->next;
	}

	file.close();
}

//即dir功能，展示当前文件夹中的文件
void show()
{
	Dir* p = new Dir;
	Dir* p1 = new Dir;
	Dir* p2 = new Dir;

	p = ND; //p指向当前文件夹的节点

	cout << "current folder is " << p->name << endl << "the content is following : " << endl;
	cout << "name                          type                      create_t" << endl;

	//判断是否没有子文件
	if (p->right != NULL)
		p = p->right;
	else
		return;

	//p不为空时，遍历子文件输出
	while (p != NULL)
	{
		cout << p->name << "                         ";
		if (p->mod == 2)
			cout << "TXT                ";
		else
			cout << "folder                ";
		struct tm* T;
		T = localtime(&p->t);
		printf("%d/%02d/%02d/", (1900 + T->tm_year), (1 + T->tm_mon), T->tm_mday);
		printf("%02d:%02d:%02d\n", T->tm_hour, T->tm_min, T->tm_sec);
		cout << endl;
		p = p->right;
	}
}


//展示所有文件夹的情况
void showAll()
{
	Dir* p = new Dir;
	Dir* p1 = new Dir;

	//在文件目录中，从第一个开始遍历
	p = head->next;
	while (p)
	{
		cout << "Folder named -> " << p->name << " " << ": " << endl;
		cout << "name                           type                             create_t" << endl;

		//进入子文件
		p1 = p->right;

		//当子文件不为空时，输出子文件名称
		while (p1)
		{
			cout << p1->name << "                        ";
			if (p1->mod == 2)
				cout << "TXT                  ";
			else
				cout << "folder                  ";
			struct tm* T;
			T = localtime(&p1->t);
			printf("%d/%02d/%02d/", (1900 + T->tm_year), (1 + T->tm_mon), T->tm_mday);
			printf("%02d:%02d:%02d\n", T->tm_hour, T->tm_min, T->tm_sec);

			p1 = p1->right;
		}
		p = p->next;
		cout << endl << endl;
	}
}


//移动当前位置到目标文件夹
void cd()
{

	//输入要跳转的文件夹名字
	string ad;
	cin >> ad;

	/////////////////////////////////绝对路径
	int i = 1, i2 = 0;

	string name[10];//记录路径中文件夹的名字

	if (ad[0] == '/')
	{
		int i = 1, i2 = 0;

		//  以 “ / ” 为分隔符 读入每级的文件夹名字到 name 数组中
		while (ad[i])
		{
			if (ad[i] != '/')
			{
				name[i2].push_back(ad[i]);
				i++;
			}
			if (ad[i] == '/')
			{
				i++;
				i2++;
				continue;
			}
		}


		Dir* pp = new Dir;
		Dir* pp1 = new Dir;
		pp = head;
		int i3;
		for (int j = 0; j <= i2; j++) //循环name数组里的名字，一级一级进行判断
		{
			i3 = j;
			while (pp) //循环文件目录，查找name数组中的名字
			{
				pp = pp->next;//找目录里的位置
				if (!pp) //pp不存在则跳出
					break;

				///////////////////////////////////////////////////////////////////////name中最后一个名字 即最终文件夹的名字，找到后改变ND，返回
				if (name[i3].compare(pp->name) == 0 && i3 == i2)
				{
					ND = pp;
					cout << "cd successed!" << endl << endl;
					return;
				}


				///////////////////////////////////////////////////////////////////////name中的第一个名字，即根目录
				if (name[i3].compare(pp->name) == 0 && i3 == 0)
				{
					pp1 = pp->right;//找下一个名字是否存在当前文件夹里，判断路径是否输入正确
					while (pp1)
					{
						if (pp1->name.compare(name[i3 + 1]) == 0) //找到时跳出
						{
							i3++;
							break;
						}
						pp1 = pp1->right;
					}
					if (!pp1 && i3 < i2) //找不到，说明pp1走到最末尾，输出错误
					{
						cout << "wrong path!" << endl << endl;
						return;
					}
				}


				////////////////////////////////////////////////////////////////////////name中的其他名字   文件夹名字除了相同  其上级文件夹的名字得和 name[]中的上一个名字一样
				if (name[i3].compare(pp->name) == 0 && i3 > 0 && pp->prename.compare(name[i3 - 1]) == 0)
				{
					pp1 = pp->right;//找下一个名字是否存在当前文件夹里
					while (pp1)
					{
						if (pp1->name.compare(name[i3 + 1]) == 0)
						{
							i3++;
							break;
						}
						pp1 = pp1->right;
					}
					if (!pp1 && i3 < i2)
					{
						cout << "wrong path!" << endl << endl;
						return;
					}
				}
			}
		}


		return;
	}


	/////////////////////////////////放回上一级
	if (ad == "..")
	{
		//ad = ND->prename;
		//if (ad == "NULL")
		//{
		//	cout << "该目录没有上级目录" << endl << endl;
		//	return;
		//}
		//Dir *p = new Dir;
		//p = head->next;
		//while (p)//找到文件夹的位置
		//{
		//	if (p->name.compare(ad) != 0 && p->prename != ND->name)
		//		p = p->next;
		//	else
		//		break;
		//}
		//if (!p) //输入有误
		//{
		//	cout << "没有该文件夹！请重新输入要跳转的文件夹名称！" << endl << endl;
		//	return;
		//}
		////记录文件夹的位置
		//nowDir = ad;
		//ND = p;
		if (ND->pre)
			ND = ND->pre;
		cout << "cd successed!" << endl << endl;
		return;
	}


	////////////////////////////////返回根目录
	if (ad == "!")
	{
		ND = head->next;
		cout << endl;
		return;
	}


	////////////////////////////////进入当前文件夹的子文件夹
	Dir* p = new Dir;
	p = head->next;
	while (p)//找到文件夹的位置
	{
		if (p->name.compare(ad) != 0 || p->pre != ND)
			p = p->next;
		else
			break;
	}
	if (!p) //输入有误
	{
		cout << "current folder has no such sub folder or file" << endl << endl;
		return;
	}
	//记录文件夹的位置
	nowDir = ad;
	ND = p;
	cout << "cd successed!" << endl << endl;
	return;
}


//在当前文件夹下创建一个文件夹
void mkdir()
{
	string name;
	Dir* p = new Dir;
	Dir* p1 = new Dir;
	p = ND->right;//列为当前目录的子目录
	p1 = ND->next;//列入目录列表内

	while (1)
	{
		cin >> name;
		int jg = 0; //用来判断文件夹是否重复

		///////////////////////////////////////////////判断和插入当前文件夹的子文件夹
		if (!p)//当前目录没有子文件时
		{
			p = new Dir;
			ND->right = p;
			p->mod = 1;
			p->name = name;
			p->t = time(NULL);
			p->right = NULL;
		}
		else//当前文件有子目录时
		{
			Dir* p2 = new Dir;
			p2 = ND;
			p = p2->right;
			while (p)//判断是否有重名，
			{

				if (p->name == name)
				{
					cout << "name duplicated！please re-enter a name：";

					//记录jg状态
					jg = 1;
					break;
				}
				p2 = p2->right;
				p = p->right;
			}

			//有重名则重新输入
			if (jg == 1)
				continue;

			//没有重名时，插入
			p = new Dir;
			p->name = name;
			p->t = time(NULL);
			p->mod = 1;
			p->right = NULL;
			p2->right = p;
		}

		///////////////////////////////////////////////插入子文件夹到文件夹目录中
		if (!p1)//文件夹目录中 当前问价夹的next为空时
		{
			p1 = new Dir;
			ND->next = p1;
			p1->mod = 1;
			p1->prename = ND->name;
			p1->pre = ND;
			p1->t = time(NULL);
			p1->name = name;
			p1->next = NULL;
		}
		else//文件夹目录中 当前问价夹的next不为空时
		{
			//循环到文件夹目录中的最后一个
			while (p1->next)
			{
				p1 = p1->next;
			}

			//插入
			Dir* p3 = new Dir;
			p3->name = name;
			p3->mod = 1;
			p3->prename = ND->name;
			p3->pre = ND;
			p3->t = time(NULL);
			p3->next = NULL;
			p1->next = p3;
		}
		break;
	}
	cout << "create successed!!" << endl << endl;
	return;
}


//显示当前文件夹下的文档内容
void more()
{
	//输入要显示的文档名字
	string s;
	cin >> s;

	Dir* p1 = new Dir;
	int jg = 0; //设置判断条件 0 为不存在 1为存在

	//遍历当前文件夹的子问价夹列表
	p1 = ND->right;
	while (p1)
	{
		if (p1->name.compare(s) == 0 && p1->mod == 2) //当存在时输出文档内容
		{
			cout << p1->document << endl << endl;

			//改变判断条件
			jg = 1;

			break;
		}
		p1 = p1->right;
	}
	if (jg == 0)
		cout << "flie not exist！" << endl;
}


//复制当前文件夹下的文件到目标文件夹
void copy()
{
	string cp, ad;
	cin >> cp >> ad;
	Dir* p = new Dir;
	p = ND->right;

	//记录当前文件夹，后面会改变
	Dir* p10 = new Dir;
	p10 = ND;

	while (p) //找到想要复制的文件
	{
		if (p->name.compare(cp) == 0 && p->mod == 2)
			break;
		p = p->right;
	}

	if (!p)
	{
		cout << "想要复制的文件不存在！" << endl << endl;
		return;
	}

	//p1复制文件数据
	Dir* p1 = new Dir;
	p1->document = p->document;
	p1->mod = 2;
	p1->name = p->name;
	p1->t = time(NULL);
	p->right = NULL;


	//绝对路径  找到目标文件夹 记录在ND
	int i = 1, i2 = 0;
	string name[10];//记录路径中文件夹的名字
	if (ad[0] == '/')
	{
		int i = 1, i2 = 0;
		while (ad[i])//记录路径的名字
		{
			if (ad[i] != '/')
			{
				name[i2].push_back(ad[i]);
				i++;
			}
			if (ad[i] == '/')
			{
				i++;
				i2++;
				continue;
			}
		}
		Dir* pp = new Dir;
		Dir* pp1 = new Dir;
		pp = head;
		int i3;

		for (int j = 0; j <= i2; j++)
		{
			i3 = j;
			while (pp)
			{
				pp = pp->next;//找目录里的位置
				if (!pp)
					break;

				if (name[i3].compare(pp->name) == 0 && i3 == i2) //最后一个 改变ND 跳出
				{
					ND = pp;
					break;
				}

				if (name[i3].compare(pp->name) == 0 && i3 == 0)
				{
					pp1 = pp->right;//找下一个名字是否存在当前文件夹里
					while (pp1)
					{
						if (pp1->name.compare(name[i3 + 1]) == 0)
						{
							i3++;
							break;
						}
						pp1 = pp1->right;
					}
					if (!pp1 && i3 < i2)
					{
						cout << "wrong path!" << endl << endl;
						return;
					}
				}

				if (name[i3].compare(pp->name) == 0 && i3 > 0 && pp->prename.compare(name[i3 - 1]) == 0)  //名字得相同 并且除了头文件夹	 其他文件夹的名字得和 name[]中的上一个名字一样
				{
					pp1 = pp->right;//找下一个名字是否存在当前文件夹里
					while (pp1)
					{
						if (pp1->name.compare(name[i3 + 1]) == 0)
						{
							i3++;
							break;
						}
						pp1 = pp1->right;
					}
					if (!pp1 && i3 < i2)
					{
						cout << "wrong path!" << endl << endl;
						return;
					}
				}
			}
		}

	}

	////////////////////////////////////////////////////////
	p = new Dir;
	p = ND;
	//p = head->next;
	//while (p)
	//{
	//	if (p->name.compare(ad) == 0)
	//		break;
	//	p=p->next;
	//}
	//if (!p)
	//{
	//	cout << "想要保存复制文件的目录不存在！" << endl<<endl;
	//	return;
	//}

	//判断ND的子文件夹列表中有没有重名文档
	Dir* p2 = new Dir;
	p2 = p->right;
	while (p->right) //判断是否有重名，但是最后一个没有判断
	{
		if (p->name.compare(cp) == 0 && p->mod == 2)
		{
			cout << "想要保存的目录下已经有同名文件！" << endl << endl;
			return;
		}
		p = p->right;
	}

	if (p->name.compare(cp) == 0 && p->mod == 2)  //判断最后一个是否重名
	{
		cout << "想要保存的目录下已经有同名文件！" << endl << endl;
		return;
	}

	//复制节点插入  ND变回原来的值
	p->right = p1;
	ND = p10;
	cout << endl;
}


//删除当前目录下的空文件夹
void rmdir()
{
	string s;
	cin >> s;
	Dir* p = new Dir;
	p = ND;
	while (p && p->right) //找删除的子目录
	{

		if (p->right->name.compare(s) == 0)
			break;
		p = p->right;
	}
	if (!p) //子目录不存在
	{
		cout << "没有找到该文件！" << endl;
		return;
	}


	Dir* p1 = new Dir;
	Dir* p2 = new Dir;
	p1 = head->next;
	p2 = p1;
	while (p1)      //在目录列表中删除
	{
		if (p1->name.compare(s) == 0 && p1->prename.compare(ND->name) == 0)
			break;
		p2 = p1;
		p1 = p1->next;
	}
	if (!p1->right)
	{
		p2->next = p2->next->next;
	}
	else
	{
		cout << "该子目录不为空，不能删除！！" << endl;
		return;
	}


	p->right = p->right->right; //在上级目录中删除
	cout << "删除成功！" << endl << endl;
}


//输入字符串S，查找当前文件夹中文档中是否包含S
void find()
{
	//输入要查找的字符串
	string s;
	cin >> s;

	//遍历当前文件夹的子文件列表
	Dir* p = new Dir;
	p = ND->right;
	while (p)
	{
		if (p->mod == 2) //当文件为文档时，进行查找
		{
			//赋值给 d，一个个字符进行比较
			string d = p->document;
			for (int i = 0; i < d.size(); i++)
			{
				int k = i;
				int count = 0; //计算相同字符个数
				for (int j = 0; j < s.size(); j++)
				{
					if (d[i] != s[j])
						break;
					if (d[i] == s[j])
					{
						count++;
						i++;
						continue;
					}
				}
				if (count == s.size()) //都相同时，说明包含，输出文档内容
				{
					cout << "找到该文件：" << p->name << endl;
					cout << "文件内容为：" << endl << p->document << endl << endl;
					return;
				}
				else
					i = k;
			}
		}
		p = p->right;
	}
	if (!p)
	{
		cout << "不存在含该字符串的文件" << endl << endl;
	}
}


//显示当前文件夹下文档的属性
void attrib()
{
	string s;
	cin >> s;
	Dir* p = new Dir;
	p = ND->right;
	while (p)
	{
		if (p->name.compare(s) == 0)
		{
			cout << endl << "该文件名字为 ：" << p->name << endl;
			if (p->mod == 1)
			{
				struct tm* pp;
				cout << "该文件是文件夹,创建时间为 :";
				pp = localtime(&p->t);
				printf("%d年%02d月%02d日", (1900 + pp->tm_year), (1 + pp->tm_mon), pp->tm_mday);
				printf("%02d:%02d:%02d\n\n", pp->tm_hour, pp->tm_min, pp->tm_sec);
				return;

			}
			else
			{
				struct tm* pp;
				cout << "该文件是TXT文档,创建时间为 :";
				pp = localtime(&p->t);
				printf("%d年%02d月%02d日", (1900 + pp->tm_year), (1 + pp->tm_mon), pp->tm_mday);
				printf("%02d:%02d:%02d\n", pp->tm_hour, pp->tm_min, pp->tm_sec);
				cout << "文档内容为 ：" << endl;
				cout << p->document << endl;
				return;
			}
		}
		p = p->right;
	}
	if (!p)
		cout << "没找到该文件！" << endl << endl;
}


//删除当前文件夹下的文档
void del()
{
	string s;
	cin >> s;
	Dir* p = new Dir;
	p = ND;
	while (p->right)
	{
		if (p->right->name.compare(s) == 0 && p->right->mod == 2)
		{
			p->right = p->right->right;
			cout << "删除成功！" << endl << endl;
			return;
		}
		p = p->right;
	}
	if (!p->right)
		cout << "没有找到该文件！" << endl << endl;
}


//复制文件夹及其子文件
void Xcp(Dir* p, Dir* pr) //*p 为 被复制的文件夹 
{

	//不是子文件夹的时候返回
	if (!p || p->mod == 2)
	{
		return;
	}

	//移动到文件目录的最末尾
	Dir* p1 = new Dir;
	p1 = head;
	while (p1->next)
		p1 = p1->next;

	//插入 用*p2 复制  ，然后插入到文件夹目录的最末尾
	Dir* p2 = new Dir;
	p2->mod = p->mod;
	p2->name = p->name;
	p2->t = time(NULL);
	p2->next = NULL;
	p2->pre = pr;
	p2->prename = pr->name;

	p1->next = p2;

	//用*p6 记录 *p2，准备递归
	Dir* p6 = new Dir;
	p6 = p2;

	//递归文件中的子文件
	Dir* p5 = new Dir;
	Dir* p4 = new Dir;
	while (p->right) //子文件存在时 遍历每一个文件
	{
		//复制B.a 后面的内容，接入在B.c的后面
		p5 = new Dir;
		p5->mod = p->right->mod;
		if (p->right->mod == 2)
			p5->document = p->right->document;
		p5->name = p->right->name;
		p5->t = time(NULL);

		p2->right = p5;
		p2 = p2->right;

		//找到该子文件夹在文件夹目录中的位置，进入进行复制其子文件
		p4 = new Dir;
		p4 = head->next;
		while (p4)
		{
			if (p4->name.compare(p->right->name) == 0)
				break;
			p4 = p4->next;
		}

		Xcp(p4, p6);

		p = p->right;
	}
	return;
}
void xcopy()
{
	string s, ad; // 名字  路径
	cin >> s >> ad;


	//1   A->B->C
	//2   B.a->D      B.a : B 当前文件名    .a 上一级(用指针）
	//3   C.a
	//4   D.b.a

	//把A中的B复制到C中 得到的结果

	//1   A      ->B->C
	//2   B.a    ->D
	//3   C.a    ->B
	//4   D.b.a
	//5   B.c    ->D
	//6   D.b.c


	//找到想要复制的文件     第2行 B.a->D 中的B.a （*p） ，right为该文件夹中的文件 B.a->D 的 D
	Dir* p = new Dir;
	p = head->next;
	while (p)
	{
		if (p->name.compare(s) == 0 && p->pre == ND)
			break;
		p = p->next;
	}
	if (!p)
	{
		cout << "没有找到该文件夹！" << endl << endl;
		return;
	}

	//复制文件夹  p2找到要复制的文件夹  第1行  A->B->C 中的 B  *p3进行复制，right的数据去除
	Dir* p2 = new Dir;
	p2 = ND->right;
	while (p2)
	{
		if (p2->name.compare(s) == 0)
			break;
		p2 = p2->right;
	}
	Dir* p3 = new Dir;
	p3->mod = p2->mod;
	p3->name = p2->name;
	p3->prename = ad;
	p3->right = NULL;
	p3->t = time(NULL);


	Dir* p1 = new Dir;
	p1 = head->next;


	//绝对路径  把目标文件夹赋值给 *p1

	int i = 1, i2 = 0;

	int j = 0; //用于记录本次查询使用绝对地址

	string name[10];//记录路径中文件夹的名字
	if (ad[0] == '/')
	{
		int i = 1, i2 = 0;
		while (ad[i])
		{
			if (ad[i] != '/')
			{
				name[i2].push_back(ad[i]);
				i++;
			}
			if (ad[i] == '/')
			{
				i++;
				i2++;
				continue;
			}
		}
		Dir* pp = new Dir;
		Dir* pp1 = new Dir;
		pp = head;
		int i3;

		for (j = 0; j <= i2; j++)
		{
			i3 = j;
			while (pp)
			{
				pp = pp->next;//找目录里的位置
				if (!pp)
					break;

				if (name[i3].compare(pp->name) == 0 && i3 == i2) //最后一个 改变ND 跳出
				{
					p1 = pp;
					j = 99;
					break;
				}

				if (name[i3].compare(pp->name) == 0 && i3 == 0)
				{
					pp1 = pp->right;//找下一个名字是否存在当前文件夹里
					while (pp1)
					{
						if (pp1->name.compare(name[i3 + 1]) == 0)
						{
							i3++;
							break;
						}
						pp1 = pp1->right;
					}
					if (!pp1 && i3 < i2)
					{
						cout << "wrong path!" << endl << endl;
						return;
					}
				}

				if (name[i3].compare(pp->name) == 0 && i3 > 0 && pp->prename.compare(name[i3 - 1]) == 0)  //名字得相同 并且除了头文件夹	 其他文件夹的名字得和 name[]中的上一个名字一样
				{
					pp1 = pp->right;//找下一个名字是否存在当前文件夹里
					while (pp1)
					{
						if (pp1->name.compare(name[i3 + 1]) == 0)
						{
							i3++;
							break;
						}
						pp1 = pp1->right;
					}
					if (!pp1 && i3 < i2)
					{
						cout << "wrong path!" << endl << endl;
						return;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////


	while (p1)
	{
		if (p1->name.compare(ad) == 0 || j == 100)  //找到复制的地址 进行复制 C.a     前一个条件为当前文件夹下的文件夹    后一个为绝对地址的文件夹
		{
			//移动最末尾插入
			Dir* p4 = new Dir;
			p4 = p1;
			while (p4->right)
				p4 = p4->right;
			p4->right = p3;
			/*	p3->pre = p1;*/

				//载入到文件目录中，递归复制子文件夹
			Xcp(p, p1);

			cout << "复制成功！" << endl << endl;
			return;
		}
		p1 = p1->next;
	}
	if (!p1)//找不到复制的地址
		cout << "复制文件要保存的地址不存在！" << endl << endl;

	return;
}


//导入文档
void import()
{
	//s 为路径  name为存储进系统的名称   命令格式为import  E:\\hello.txt  hello1.txt
	string s, name;
	cin >> s >> name;
	fstream file;

	file.open(s);//打开文件

	//文档不存在时
	if (!file.is_open())
	{
		cout << "找不到该文档！" << endl << endl;
		return;
	}

	//读入string 可以保存换行符
	string d;
	while (!file.eof())
	{
		char c;
		file.get(c);
		d.push_back(c);
	}

	Dir* p = new Dir;
	p = ND;
	while (p->right)//移动到最末尾
		p = p->right;


	//插入保存
	Dir* p1 = new Dir;
	p1->name = name;
	p1->document = d;
	p1->mod = 2;
	p1->right = NULL;
	p1->t = time(NULL);
	p->right = p1;
	cout << "写入成功！" << endl;
	cout << "文件内容为 ：" << endl << p1->document << endl << endl;
	file.close();
}


//导出文档
void export1()
{
	//s 为 要复制文档的名字 ad为要复制的路径
	string s, ad;
	cin >> s >> ad;
	Dir* p = new Dir;
	p = ND->right;
	while (p)
	{
		if (p->name.compare(s) == 0 && p->mod == 2)
		{
			ofstream outfile(ad);
			outfile << p->document;
			outfile.close();

			cout << "导出成功！" << endl << endl;
			return;
		}
		p = p->right;
	}
	if (!p)
		cout << "不存在该文件！" << endl << endl;
}


//提示命令用处
void help()
{
	cout << "cd                   enter folder" << endl;
	cout << "dir                  show sub folders and files" << endl;
	cout << "mkdir                creater sub folder" << endl;
	cout << "more                 show file" << endl;
	cout << "copy                 copy the file to a specified folder" << endl;
	cout << "rmdir                delete folder" << endl;
	cout << "find                 find string in file" << endl;
	cout << "attrib               check folder properties" << endl;
	cout << "del                  delete file" << endl;
	cout << "xcopy                copy file and " << endl;
	cout << "import               import files" << endl;
	cout << "export               export files" << endl;
	cout << "showAll              show all folders and files" << endl;
	cout << "exit                 exit system" << endl;
}


//退出时的保存函数
void save()
{
	ofstream f("save.txt");
	Dir* p = new Dir;

	//遍历文件目录中的每一个文件夹
	p = head->next;
	while (p)
	{
		f << p->name << '\n';
		f << p->mod << '\n';
		f << p->prename << '\n';

		Dir* p1 = new Dir;
		p1 = p->right;
		while (p1)
		{
			f << p1->name << " ";
			f << p1->mod << '\n';
			if (p1->mod == 2)
				f << p1->document << "#" << '\n';
			p1 = p1->right;
		}
		p = p->next;
		if (!p1 && p)
			f << "#" << '\n';
		if (!p1 && !p)
			f << "#";
	}
	f.close();
}


//退出系统
void exit()
{
	while (1)
	{
		cout << "do you want to save all the operation？ (y/n)   : ";
		string s;
		cin >> s;
		if (s.compare("y") == 0 || s.compare("yes") == 0 || s.compare("Y") == 0)
		{
			save();
			return;
		}
		else
			if (s.compare("n") == 0 || s.compare("no") == 0 || s.compare("N") == 0)
			{
				cout << "Have a good one!!" << endl;
				system("pause");
				return;
			}
			else
			{
				cout << "wrong input！！" << endl << endl;
				continue;
			}
	}
}


//输出当前目录的路径  递归

//void cmd(Dir *p)
//{
//	Dir *p1 = new Dir;
//
//	int i = 0; //记录 跳出循环
//	
//	if (p->prename == "NULL")  //当最上级目录时 先输出
//	{
//		cout  <<p->name << ":";
//		return;
//	}
//	else 
//	{
//		p1 = head->next;
//		while (p1)
//		{
//			if (p1->name.compare(p->prename) == 0)
//			{
//				Dir *p2 = new Dir;
//				p2 = p1->right;
//				while (p2)
//				{
//					if (p2->name.compare(p->name) == 0)
//					{
//						cmd(p1);
//						i = 1;
//						break;
//					}
//					p2 = p2->right;
//				}
//			}
//			if (i == 1)
//				break;
//			p1 = p1->next;
//		}
//	}
//	cout << " / " << p->name ;
//	return;
//}
void cmd1(Dir* p)
{
	Dir* p1 = new Dir;

	int i = 0; //记录 跳出循环

	if (p->prename == "NULL")  //当最上级目录时 先输出
	{
		cout << p->name << ":";
		return;
	}
	else
	{
		cmd1(p->pre);
	}
	cout << " / " << p->name;
	return;
}

int main()
{
	//初始化
	set();

	nowDir = head->next->name;
	ND = new Dir;
	ND = head->next;

	//显示所有的文件夹
	showAll();

	string command;
	while (1)
	{
		cout << endl;
		cmd1(ND);
		cout << " >  ";

		//输入命令
		cin >> command;
		int cm = -1;
		for (int i = 0; i < 15; i++) //与命令数组中的命令进行比较，判断是否输入正确
		{
			string s(com[i]);
			if (s.compare(command) == 0)
			{
				cm = i;
				break;
			}
		}
		switch (cm)
		{
		case 0:cd(); break;
		case 1:show(); break;
		case 2:mkdir(); break;
		case 3:more(); break;
		case 4:copy(); break;
		case 5:rmdir(); break;
		case 6:find(); break;
		case 7:attrib(); break;
		case 8:del(); break;
		case 9:xcopy(); break;
		case 10:import(); break;
		case 11:export1(); break;
		case 12:exit(); return 0; break;
		case 13:help(); break;
		case 14:showAll(); break;
		default:cout << "Wrong Command!" << endl << endl; break;
		}
	}


}


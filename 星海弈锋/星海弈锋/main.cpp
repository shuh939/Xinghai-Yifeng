#include "GameFrameWork/Application.h"
#include "GameScene.h"
#include "StartScene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "Winmm.lib")  // 链接Windows多媒体库

typedef struct Person {
	char name[50];
	char gender[10];
	char birthdate[20];
	char password[20];
	int battleCount;  // 新增：比赛场次
	struct Person* next;
} Person;

// 检查出生年月格式（YYYY-MM-DD）
int checkBirthdate(const char* date) {
	if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return 0;
	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) continue;
		if (date[i] < '0' || date[i] > '9') return 0;
	}
	return 1;
}

// 多属性查询
typedef enum { NAME, GENDER } SearchType;

Person* findByAttribute(Person* head, SearchType type, const char* value) {
	Person* temp = head;
	while (temp) {
		switch (type) {
		case NAME: if (!strcmp(temp->name, value)) return temp; break;
		case GENDER: if (!strcmp(temp->gender, value)) return temp; break;
		}
		temp = temp->next;
	}
	return NULL;
}

const char* getRank(int count) {
	if (count < 10) return "初阶·行星架构师";
	else if (count < 20) return "二阶·恒星牧星者";
	else if (count < 30) return "三阶·星链编织者";
	else if (count < 40) return "四阶·维度观测者";
	else if (count < 50) return "五阶·奇点工程师";
	else if (count < 60) return "六阶·弦域调律者";
	else if (count < 75) return "七阶·宇宙代码师";
	else if (count < 100) return "八阶·归零执政官";
	else return "九阶·超弦之影";
}

// 表头插入（提高新增效率）
Person* addPerson(Person* head) {
	Person* newPerson = (Person*)malloc(sizeof(Person));
	while (1)
	{
		printf("请输入用户名(最大50字符):");
		scanf("%49s", newPerson->name);
		Person* temp = findByAttribute(head, NAME, newPerson->name);
		if (temp) { printf("该用户名已被注册! \n"); continue; }
		printf("请输入性别(最大10字符):");
		scanf("%9s", newPerson->gender);

		do {
			printf("请输入生日(YYYY-MM-DD):");
			scanf("%19s", newPerson->birthdate);
			if (!checkBirthdate(newPerson->birthdate)) {
				printf("格式错误！请严格按照YYYY-MM-DD输入\n");
			}
		} while (!checkBirthdate(newPerson->birthdate));

		printf("请输入密码(最大20字符):");
		scanf("%19s", newPerson->password);

		// 新增初始化代码 ============================
		newPerson->battleCount = 0;  // 初始化比赛场次为0
		// ==========================================

		newPerson->next = head;  // 表头插入
		printf("人员信息添加成功（表头插入）! \n");
		return newPerson;  // 返回新头节点
	}
}

void displayPersons(Person* head) {
	if (head == NULL) {
		printf("当前没有人员信息! \n");
		return;
	}
	Person* temp = head;
	printf("\n====人员信息列表===\n");
	while (temp != NULL) {
		printf("用户名:%-10s 性别:%-6s 生日:%s 场次:%-3d 等级:%s\n",
			temp->name, temp->gender, temp->birthdate,
			temp->battleCount, getRank(temp->battleCount));
		temp = temp->next;
	}
}

Person* deletePerson(Person* head, const char* name) {
	Person* temp = head, * prev = NULL;
	while (temp && strcmp(temp->name, name)) {
		prev = temp;
		temp = temp->next;
	}
	if (!temp) { printf("未找到该人员信息! \n"); return head; }
	if (prev) prev->next = temp->next;
	else head = temp->next;
	free(temp);
	printf("人员信息删除成功! \n");
	return head;
}

void modifyPerson(Person* head, const char* name) {
	Person* temp = findByAttribute(head, NAME, name);
	if (!temp) { printf("未找到该人员信息! \n"); return; }

	printf("请输入新的性别(最大10字符):");
	scanf("%9s", temp->gender);

	do {
		printf("请输入新的生日(YYYY-MM-DD):");
		scanf("%19s", temp->birthdate);
		if (!checkBirthdate(temp->birthdate)) {
			printf("格式错误！请严格按照YYYY-MM-DD输入\n");
		}
	} while (!checkBirthdate(temp->birthdate));

	printf("请输入新的密码(最大20字符):");
	scanf("%19s", temp->password);

	printf("人员信息修改成功! \n");
}

void freeList(Person* head) {
	Person* temp;
	while (head) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// 数据持久化 - 保存到文件
void saveToFile(Person* head, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (!file) { printf("保存失败：无法打开文件！\n"); return; }

	Person* temp = head;
	while (temp) {
		fprintf(file, "%s %s %s %s %d\n",  // 添加 %d 保存场次
			temp->name, temp->gender,
			temp->birthdate, temp->password,
			temp->battleCount);
		temp = temp->next;
	}
	fclose(file);
	printf("数据已成功保存到 %s\n", filename);
}

// 数据持久化 - 从文件加载
Person* loadFromFile(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("加载失败：文件不存在！\n");
		return NULL;
	}

	Person* head = NULL, * tail = NULL, * newPerson;
	char buff[200];

	while (fgets(buff, sizeof(buff), file)) {
		newPerson = (Person*)malloc(sizeof(Person));

		// 修改点：添加 battleCount 字段读取
		int numRead = sscanf(buff, "%49s %9s %19s %19s %d",
			newPerson->name,
			newPerson->gender,
			newPerson->birthdate,
			newPerson->password,
			&newPerson->battleCount);  // 新增字段

		// 处理旧数据格式（没有 battleCount 的情况）
		if (numRead < 5) {
			newPerson->battleCount = 0;  // 默认初始化为0
		}

		newPerson->next = NULL;

		// 链表构建逻辑保持不变
		if (!head) {
			head = tail = newPerson;
		}
		else {
			tail->next = newPerson;
			tail = newPerson;
		}
	}

	fclose(file);
	printf("数据加载成功！当前记录数：");
	displayPersons(head);  // 注意：displayPersons 需要已支持显示 battleCount
	return head;
}

// 循环播放背景音乐
void bgm()
{
	// 给音乐文件设置别名，避免重复处理路径
	mciSendString("open ./Onmyway.mp3 alias bgm", NULL, 0, NULL);
	// 循环播放指令（repeat参数）
	mciSendString("play bgm repeat", NULL, 0, NULL);
}

int main(int argc, char* argv[])
{
	bgm();  // 启动背景音乐

	Person* head = NULL;
	int choice, searchType;
	char name[50], filename[50] = "data.dat";

	printf("===== 人员信息管理系统 ====\n");
	printf("支持功能：密码验证、格式校验、数据持久化\n\n");

	while (1) {
		printf("\n1. 添加人员信息（表头插入）\n");
		printf("2. 显示所有人员信息\n");
		printf("3. 修改人员信息（按用户名）\n");
		printf("4. 删除人员信息（按用户名）\n");
		printf("5. 多条件查询（用户名/性别/生日/场次/等级）\n");
		printf("6. 进入游戏（用户登录）\n");
		printf("7. 保存数据到文件\n");
		printf("8. 从文件加载数据\n");
		printf("9. 退出系统\n");
		printf("请输入选项: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			head = addPerson(head);
			break;
		case 2: {
			char admin_pass[20];
			printf("请输入管理员密码: ");
			scanf("%19s", admin_pass);
			// 这里可以设置一个默认管理员密码，比如"admin123"
			if (strcmp(admin_pass, "adminXD") == 0) {
				displayPersons(head);
			}
			else {
				printf("密码错误！\n");
			}
			break;
		}
		case 3:
			printf("请输入要修改的人员用户名: ");
			scanf("%s", name);
			modifyPerson(head, name);
			break;
		case 4:
			printf("请输入要删除的人员用户名: ");
			scanf("%s", name);
			head = deletePerson(head, name);
			break;
		case 5: {
			printf("请输入用户名: ");
			searchType = 1;
			scanf("%s", name);
			Person* found = findByAttribute(head, (SearchType)(searchType - 1), name);
			if (found) {
				char input_pass[20];
				printf("请输入该用户的密码: ");
				scanf("%19s", input_pass);
				if (strcmp(input_pass, found->password) == 0) {
					printf("\n查询结果：\n");
					printf("用户名:%s\n性别:%s\n生日:%s\n",
						found->name, found->gender, found->birthdate);
					printf("场次:%d\n等级:%s\n",
						found->battleCount, getRank(found->battleCount));
				}
				else {
					printf("密码错误，无权查看信息！\n");
				}
			}
			else {
				printf("未找到匹配的人员信息！\n");
			}
			break;
		}
		case 6: {
			printf("请输入用户名: ");
			searchType = 1;
			scanf("%s", name);
			Person* found = findByAttribute(head, (SearchType)(searchType - 1), name);
			if (found) {
				char input_pass[20];
				printf("请输入该用户的密码: ");
				scanf("%19s", input_pass);
				if (strcmp(input_pass, found->password) == 0) {
					printf("登录成功，欢迎 %s 进入游戏！\n", found->name);
					found->battleCount++;  // 比赛场次加1
					// 这里可以调用游戏的主函数
					// 初始化应用窗口（960x820）
					Application a(960, 820);

					// 启动开始页面
					a.runScene(new StartScene());

					// 进入主循环
					return a.exec();
				}
				else {
					printf("密码错误，无法进入游戏！\n");
				}
			}
			else {
				printf("未找到匹配的人员信息！\n");
			}
			break;
		}
		case 7: {
			saveToFile(head, filename);
			break;
		}
		case 8:
		{
			head = loadFromFile(filename);
			break;
		}
		case 9: {
			freeList(head);
			printf("系统已退出，数据已释放！\n");
			return 0;
		}
		default: {
			printf("无效的选项，请重新输入！\n");
		}
		}
	}

}

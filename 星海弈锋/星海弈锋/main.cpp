#include "GameFrameWork/Application.h"
#include "GameScene.h"
#include "StartScene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "Winmm.lib")  // ����Windows��ý���

typedef struct Person {
	char name[50];
	char gender[10];
	char birthdate[20];
	char password[20];
	int battleCount;  // ��������������
	struct Person* next;
} Person;

// ���������¸�ʽ��YYYY-MM-DD��
int checkBirthdate(const char* date) {
	if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return 0;
	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) continue;
		if (date[i] < '0' || date[i] > '9') return 0;
	}
	return 1;
}

const char* getRank(int count) {
	if (count < 10) return "���ס����Ǽܹ�ʦ";
	else if (count < 20) return "���ס�����������";
	else if (count < 30) return "���ס�������֯��";
	else if (count < 40) return "�Ľס�ά�ȹ۲���";
	else if (count < 50) return "��ס���㹤��ʦ";
	else if (count < 60) return "���ס����������";
	else if (count < 75) return "�߽ס��������ʦ";
	else if (count < 100) return "�˽ס�����ִ����";
	else return "�Žס�����֮Ӱ";
}

// ��ͷ���루�������Ч�ʣ�
Person* addPerson(Person* head) {
	Person* newPerson = (Person*)malloc(sizeof(Person));

	printf("�������û���(���50�ַ�):");
	scanf("%49s", newPerson->name);

	printf("�������Ա�(���10�ַ�):");
	scanf("%9s", newPerson->gender);

	do {
		printf("����������(YYYY-MM-DD):");
		scanf("%19s", newPerson->birthdate);
		if (!checkBirthdate(newPerson->birthdate)) {
			printf("��ʽ�������ϸ���YYYY-MM-DD����\n");
		}
	} while (!checkBirthdate(newPerson->birthdate));

	printf("����������(���20�ַ�):");
	scanf("%19s", newPerson->password);

	// ������ʼ������ ============================
	newPerson->battleCount = 0;  // ��ʼ����������Ϊ0
	// ==========================================

	newPerson->next = head;  // ��ͷ����
	printf("��Ա��Ϣ��ӳɹ�����ͷ���룩! \n");
	return newPerson;  // ������ͷ�ڵ�
}

void displayPersons(Person* head) {
	if (head == NULL) {
		printf("��ǰû����Ա��Ϣ! \n");
		return;
	}
	Person* temp = head;
	printf("\n====��Ա��Ϣ�б�===\n");
	while (temp != NULL) {
		printf("�û���:%-10s �Ա�:%-6s ����:%s ����:%-3d �ȼ�:%s\n",
			temp->name, temp->gender, temp->birthdate,
			temp->battleCount, getRank(temp->battleCount));
		temp = temp->next;
	}
}

// �����Բ�ѯ
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

Person* deletePerson(Person* head, const char* name) {
	Person* temp = head, * prev = NULL;
	while (temp && strcmp(temp->name, name)) {
		prev = temp;
		temp = temp->next;
	}
	if (!temp) { printf("δ�ҵ�����Ա��Ϣ! \n"); return head; }
	if (prev) prev->next = temp->next;
	else head = temp->next;
	free(temp);
	printf("��Ա��Ϣɾ���ɹ�! \n");
	return head;
}

void modifyPerson(Person* head, const char* name) {
	Person* temp = findByAttribute(head, NAME, name);
	if (!temp) { printf("δ�ҵ�����Ա��Ϣ! \n"); return; }

	printf("�������µ��Ա�(���10�ַ�):");
	scanf("%9s", temp->gender);

	do {
		printf("�������µ�����(YYYY-MM-DD):");
		scanf("%19s", temp->birthdate);
		if (!checkBirthdate(temp->birthdate)) {
			printf("��ʽ�������ϸ���YYYY-MM-DD����\n");
		}
	} while (!checkBirthdate(temp->birthdate));

	printf("�������µ�����(���20�ַ�):");
	scanf("%19s", temp->password);

	printf("��Ա��Ϣ�޸ĳɹ�! \n");
}

void freeList(Person* head) {
	Person* temp;
	while (head) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// ���ݳ־û� - ���浽�ļ�
void saveToFile(Person* head, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (!file) { printf("����ʧ�ܣ��޷����ļ���\n"); return; }

	Person* temp = head;
	while (temp) {
		fprintf(file, "%s %s %s %s %d\n",  // ��� %d ���泡��
			temp->name, temp->gender,
			temp->birthdate, temp->password,
			temp->battleCount);
		temp = temp->next;
	}
	fclose(file);
	printf("�����ѳɹ����浽 %s\n", filename);
}

// ���ݳ־û� - ���ļ�����
Person* loadFromFile(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("����ʧ�ܣ��ļ������ڣ�\n");
		return NULL;
	}

	Person* head = NULL, * tail = NULL, * newPerson;
	char buff[200];

	while (fgets(buff, sizeof(buff), file)) {
		newPerson = (Person*)malloc(sizeof(Person));

		// �޸ĵ㣺��� battleCount �ֶζ�ȡ
		int numRead = sscanf(buff, "%49s %9s %19s %19s %d",
			newPerson->name,
			newPerson->gender,
			newPerson->birthdate,
			newPerson->password,
			&newPerson->battleCount);  // �����ֶ�

		// ��������ݸ�ʽ��û�� battleCount �������
		if (numRead < 5) {
			newPerson->battleCount = 0;  // Ĭ�ϳ�ʼ��Ϊ0
		}

		newPerson->next = NULL;

		// �������߼����ֲ���
		if (!head) {
			head = tail = newPerson;
		}
		else {
			tail->next = newPerson;
			tail = newPerson;
		}
	}

	fclose(file);
	printf("���ݼ��سɹ�����ǰ��¼����");
	displayPersons(head);  // ע�⣺displayPersons ��Ҫ��֧����ʾ battleCount
	return head;
}

// ѭ�����ű�������
void bgm()
{
	// �������ļ����ñ����������ظ�����·��
	mciSendString("open ./Onmyway.mp3 alias bgm", NULL, 0, NULL);
	// ѭ������ָ�repeat������
	mciSendString("play bgm repeat", NULL, 0, NULL);
}

int main(int argc, char* argv[])
{
	bgm();  // ������������

	Person* head = NULL;
	int choice, searchType;
	char name[50], filename[50] = "data.dat";

	printf("===== ��Ա��Ϣ����ϵͳ ====\n");
	printf("֧�ֹ��ܣ�������֤����ʽУ�顢���ݳ־û�\n\n");

	while (1) {
		printf("\n1. �����Ա��Ϣ����ͷ���룩\n");
		printf("2. ��ʾ������Ա��Ϣ\n");
		printf("3. �޸���Ա��Ϣ�����û�����\n");
		printf("4. ɾ����Ա��Ϣ�����û�����\n");
		printf("5. ��������ѯ���û���/�Ա�/����/����/�ȼ���\n");
		printf("6. ������Ϸ���û���¼��\n");
		printf("7. �������ݵ��ļ�\n");
		printf("8. ���ļ���������\n");
		printf("9. �˳�ϵͳ\n");
		printf("������ѡ��: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			head = addPerson(head);
			break;
		case 2: {
			char admin_pass[20];
			printf("���������Ա����: ");
			scanf("%19s", admin_pass);
			// �����������һ��Ĭ�Ϲ���Ա���룬����"admin123"
			if (strcmp(admin_pass, "adminXD") == 0) {
				displayPersons(head);
			}
			else {
				printf("�������\n");
			}
			break;
		}
		case 3:
			printf("������Ҫ�޸ĵ���Ա�û���: ");
			scanf("%s", name);
			modifyPerson(head, name);
			break;
		case 4:
			printf("������Ҫɾ������Ա�û���: ");
			scanf("%s", name);
			head = deletePerson(head, name);
			break;
		case 5: {
			printf("�������û���: ");
			searchType = 1;
			scanf("%s", name);
			Person* found = findByAttribute(head, (SearchType)(searchType - 1), name);
			if (found) {
				char input_pass[20];
				printf("��������û�������: ");
				scanf("%19s", input_pass);
				if (strcmp(input_pass, found->password) == 0) {
					printf("\n��ѯ�����\n");
					printf("�û���:%s\n�Ա�:%s\n����:%s\n",
						found->name, found->gender, found->birthdate);
					printf("����:%d\n�ȼ�:%s\n",
						found->battleCount, getRank(found->battleCount));
				}
				else {
					printf("���������Ȩ�鿴��Ϣ��\n");
				}
			}
			else {
				printf("δ�ҵ�ƥ�����Ա��Ϣ��\n");
			}
			break;
		}
		case 6: {
			printf("�������û���: ");
			searchType = 1;
			scanf("%s", name);
			Person* found = findByAttribute(head, (SearchType)(searchType - 1), name);
			if (found) {
				char input_pass[20];
				printf("��������û�������: ");
				scanf("%19s", input_pass);
				if (strcmp(input_pass, found->password) == 0) {
					printf("��¼�ɹ�����ӭ %s ������Ϸ��\n", found->name);
					found->battleCount++;  // �������μ�1
					// ������Ե�����Ϸ��������
					// ��ʼ��Ӧ�ô��ڣ�960x820��
					Application a(960, 820);

					// ������ʼҳ��
					a.runScene(new StartScene());

					// ������ѭ��
					return a.exec();
				}
				else {
					printf("��������޷�������Ϸ��\n");
				}
			}
			else {
				printf("δ�ҵ�ƥ�����Ա��Ϣ��\n");
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
			printf("ϵͳ���˳����������ͷţ�\n");
			return 0;
		}
		default: {
			printf("��Ч��ѡ����������룡\n");
		}
		}
	}

}

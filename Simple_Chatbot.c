#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 200
#define MAX_STACK_SIZE 200
void input_data(char* buf, char* who);	//���ڿ� �Է� �ޱ�
char* search_anwser(char* input, int teach); //������ ���� ��� ã�� 
void print_answer(char* output); //��� ���
void teach_answer(char* buf); //���Ͽ� ��� �Է� (������ �н�)
void move_line(FILE* file, int line); //line��ŭ �����ٷ� �̵�
int filtering(char* input); //�弳 �� Ư�� ���� ���͸�

char* str_malloc(int len); //���ڿ� ���� �Ҵ�
char* get_str(char* buf, int max, FILE* file);//���๮�ڸ� ���� �����Է�
void textcolor(int color_number); //�۾� ������

int top = -1;

//stack ����
char stack[MAX_STACK_SIZE][SIZE];
void push(int* top, char stack[][SIZE],char* input) {
	(*top)++;    
	strcpy(stack[*top], input);
}

void main() {
	MessageBoxA(NULL, "�̸��� ���ϰ� �����̿� ä���غ�����. \n�����̸� ����ġ�� �ʹٸ� \"?? ����\" �������� ���ּ���. \n��ȭ�� �����ϰ� �ʹٸ� \"�߰�\"�� �Է��� �ּ���\n", "������", MB_OK);
	char* buf = str_malloc(SIZE);
	int teach;
	char* input;
	char* output;
	char user_name[15];
	int answer_count = 0;
	srand((unsigned)time(NULL));
	input_data(user_name, "������?");
	while (1) { // ��ȭ�� �ݺ�
		teach = 0;
		//INPUT
		input_data(buf, user_name); //buf�� �Է¹���
		input = str_malloc(strlen(buf) + 1); //input�� bufũ�⸸ŭ �����Ҵ�(�ι��� ����)
		strcpy(input, buf); //input�� buf�� ����


		//input check
		if (!strncmp(input, "?? ", 3)) { //�����̿��� ���� ����ġ�� ����� ��
			input = input + 3; //�տ� '?? '�� input���� ����
			teach = 1;
		}
		if (!strcmp("�߰�", input)) { //"�߰�"�Է½� ������ ����
			MessageBoxA(NULL, "������ ��", "������", MB_OK);
			break;
		}
		if (filtering(input) == 1) { //���͸� �ܾ� ������ input free�ϰ� continue
			free(input);
			continue;
		}
		
		// �ߺ� üũ
		if (top != -1 && strcmp(stack[top], input)==0) { // ���� ���� üũ
			if (strcmp(stack[top], "�ȳ�") == 0) { // �׷��� �λ�� ����ص� ��.
				answer_count = 0;
			}
			else if (strcmp(stack[top], "�ȳ��ϼ���") == 0) { // �׷��� �λ�� ����ص� ��.
				answer_count = 0;
			}
			else{ // �λ縻�� �ٸ� ������ 2������ ����� ¥����
				answer_count++;
				if (answer_count == 1) { // 2�������� ����
					print_answer("�� �� �����?");
				}
				else if (answer_count == 2) { //3���翡�� ȭ��
					print_answer("������ ����. �Ȱ��� ���� ��� ���� ��!");

					textcolor(11);
					printf("��  ������  ����������������������������������������������������������������������������������������\n");
					printf(" ���� ������ ���� �ƴµ� ���������� �� ���� �� ���� �����ϸ� ������ �ſ���. �����ؿ�!\n");
					printf("��������������������������������������������������������������������������������������������������������������\n\n");
					textcolor(10);
				}
				else if (answer_count == 3) {
					MessageBoxA(NULL, "�����̰� ¥�����鼭 �����Ф�\n", "������", MB_OK);
					free(input);
					return;
				}
				free(input);
				continue;
			}
			
		}

		answer_count = 0;
		push(&top, stack, input);

		//output
		strcpy(buf, search_anwser(input, teach)); //input�� �´� output�� ã�Ƽ� buf�� ����, ������ NULL
		output = str_malloc(strlen(buf) + 1);  //output�� bufũ�⸸ŭ �����Ҵ�(�ι��� ����)
		strcpy(output, buf); //output�� buf�� ����


		//print & teach
		if (strcmp(output, "")) { // ������ ���� ���������� 

			if (teach == 1) { // ����ġ�� �����϶�
				printf("�����̰� �˰� �ִ� �����̿���.\n\n");
				input = input - 3; //input�� ũ�⸦ ������� ���� ���߿� free
			}
			print_answer(output); //output��st ��� 

		}
		else { // ������ ��ġ�ϴ� ����� �������
			printf("�����̰� �𸣴� �����̳׿�. �˷��ּ���!\n\n");
			teach_answer(input); // ������ ����ġ��
			if (teach == 1) input = input - 3; // �Ҵ� ���� ��ŭ free�ϱ� ���� �ٽ� input�� ũ�⸦ ������� ����
		}

		//FREE
		free(input);
		free(output);

	}

}

//���ڿ� �Է� �ޱ� 
void  input_data(char* buf, char* who) {
	fflush(stdin);  //�Է���ġ �ʱ�ȭ (���� ���� ����)
	while (1) { 
		textcolor(10); 
		printf("%s : ", who);
		textcolor(7); 
		get_str(buf, SIZE, stdin);
		printf("\n");
		//������ SIZE��ŭ ����Ʈ stdin -> �����ġ
		if (strcmp(buf, "")) break; //�ԷµǸ� �ݺ��� Ż��
	}

}

// ������ ���� ����� ã�Ƽ� ����, ������ ""
char* search_anwser(char* input, int teach) {
	FILE* fp;
	int output_num, random, check = 0, i;
	char* buf;
	char* similar;
	buf = str_malloc(SIZE);
	similar = str_malloc(SIZE);

	if ((fp = fopen("question.txt", "r")) == NULL) return "";   //������ ������ ����

	for (i = 0; i < 2; i++) { //�� 2�� ������ Ž�� i - 0.�Ϻ��� ��ġ�ϴ°�,  1.���� ��ġ�ϴ°�
		while (!feof(fp)) {// ���� ������ Ž��
			fscanf(fp, "%d", &output_num); // ������ ���� ����� output_num�� ����
			get_str(buf, SIZE, fp); // buf�� ���� ����

			if (i == 0 && !strcmp(input, buf)) check = 1;//������ �Ϻ��� ��ġ�ϴ°�

			if (i == 1 && strlen(buf) > 3 && (strstr(input, buf) != NULL || strstr(buf, input) != NULL)) { // ������ ������ �ִ°�
				check = 1;
				if (teach == 0) printf("\n���ϴ� ����� �ƴҰ�� \"?? ����\" �� ���� �����̸� ������ �ּ���.\n\n");
			}

			if (check == 1)                        //input�� ������ ��ġ�ų� ��� �ϸ�
			{
				random = rand() % output_num;         //������ ���� ����� ���� �������� ����
				move_line(fp, random);               //����� ������� �̵�
				get_str(buf, SIZE, fp);               //����� buf�� ����
				fclose(fp);
				return buf;                        //������ ����� ����
			}
			move_line(fp, output_num + 1);            //���� ������ �ִ� ������ġ�� �̵�
		}
		if (teach == 1) break;
		rewind(fp);                              //����Ž�� ��ġ�� ó������ ����
	}

	fclose(fp);
	return "";
}

void print_answer(char* output) {
	textcolor(14);
	printf("��  ������  ������������������������������������������������\n");
	printf(" %s\n", output);
	printf("����������������������������������������������������������������������\n\n");
	textcolor(10);
}

void  teach_answer(char* input) {
	char answer[SIZE];
	int num, i;
	FILE* fp;
	fp = fopen("question.txt", "a");

	textcolor(12); 
	printf("�� ���� ����� �˷��ֽðھ��?( 0 �Է½� skip )");
	
	textcolor(7);
	scanf("%d", &num); 
	getchar();

	if (num == 0) {
		fclose(fp);
		return;
	}
	fprintf(fp, "%d%s\n", num, input);   // ����� ������ ������ ����      3�ȳ�
	for (i = 0; i < num; i++) {        // ����� ������ŭ for��           �ݰ���
		input_data(answer, "TEACH");      //TEACH : ����� �Է¹���          �ȳ�
		fprintf(fp, "%s\n", answer);      //�Է¹��� ����� ���Ͽ� ����      ����
	}
	fprintf(fp, " \n"); //���� ó��
	fclose(fp);
	textcolor(2);
	printf("��������  �н� �Ϸ�!����������������������������������������\n\n");
	textcolor(10);
}

//line��ŭ �����ٷ� �̵�
void move_line(FILE* file, int line) {
	int i;
	for (i = 0; i < line; i++) {
		while (!feof(file))
		{
			if (fgetc(file) == '\n') break;
		}
	}
}

//�弳 �� Ư���ܾ� ���͸�
int filtering(char* input) {
	if (strstr(input, "18") != NULL) {
			MessageBoxA(NULL, "���͸� �ܾ� ����, ��������.", "������", MB_OK); {
		}
		return 1;
	}
	return 0;
}

//���ڿ� �����Ҵ�
char* str_malloc(int len) {
	return (char*)malloc(sizeof(char) * len); //�׳� malloc�ص� �Ǵµ� ���� ������ �Լ�ȭ
}

//���� ���� �����Է� ������ ���͸� ���ֹ����� �� ����
char* get_str(char* buf, int max, FILE* file) {
	fgets(buf, max, file);
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

//�۾� ������
void textcolor(int color_number) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}
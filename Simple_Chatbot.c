#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 200
#define MAX_STACK_SIZE 200
void input_data(char* buf, char* who);	//문자열 입력 받기
char* search_anwser(char* input, int teach); //질문에 대한 대답 찾기 
void print_answer(char* output); //대답 출력
void teach_answer(char* buf); //파일에 대답 입력 (츤츤이 학습)
void move_line(FILE* file, int line); //line만큼 다음줄로 이동
int filtering(char* input); //욕설 및 특정 질문 필터링

char* str_malloc(int len); //문자열 동적 할당
char* get_str(char* buf, int max, FILE* file);//개행문자를 빼고 파일입력
void textcolor(int color_number); //글씨 색변경

int top = -1;

//stack 구현
char stack[MAX_STACK_SIZE][SIZE];
void push(int* top, char stack[][SIZE],char* input) {
	(*top)++;    
	strcpy(stack[*top], input);
}

void main() {
	MessageBoxA(NULL, "이름을 정하고 츤츤이와 채팅해보세요. \n츤츤이를 가르치고 싶다면 \"?? 질문\" 형식으로 써주세요. \n대화를 종료하고 싶다면 \"잘가\"를 입력해 주세요\n", "츤츤이", MB_OK);
	char* buf = str_malloc(SIZE);
	int teach;
	char* input;
	char* output;
	char user_name[15];
	int answer_count = 0;
	srand((unsigned)time(NULL));
	input_data(user_name, "누구니?");
	while (1) { // 대화의 반복
		teach = 0;
		//INPUT
		input_data(buf, user_name); //buf에 입력받음
		input = str_malloc(strlen(buf) + 1); //input을 buf크기만큼 동적할당(널문자 포함)
		strcpy(input, buf); //input에 buf를 넣음


		//input check
		if (!strncmp(input, "?? ", 3)) { //츤츤이에게 질문 가르치기 사용할 때
			input = input + 3; //앞에 '?? '를 input에서 지움
			teach = 1;
		}
		if (!strcmp("잘가", input)) { //"잘가"입력시 츤츤이 종료
			MessageBoxA(NULL, "다음에 봐", "츤츤이", MB_OK);
			break;
		}
		if (filtering(input) == 1) { //필터링 단어 나오면 input free하고 continue
			free(input);
			continue;
		}
		
		// 중복 체크
		if (top != -1 && strcmp(stack[top], input)==0) { // 연속 질문 체크
			if (strcmp(stack[top], "안녕") == 0) { // 그래도 인사는 계속해도 됨.
				answer_count = 0;
			}
			else if (strcmp(stack[top], "안녕하세요") == 0) { // 그래도 인사는 계속해도 됨.
				answer_count = 0;
			}
			else{ // 인사말고 다른 질문은 2번연속 물어보면 짜증냄
				answer_count++;
				if (answer_count == 1) { // 2번까지는 봐줌
					print_answer("왜 또 물어보냐?");
				}
				else if (answer_count == 2) { //3번재에는 화남
					print_answer("마지막 경고다. 똑같은 질문 계속 묻지 마!");

					textcolor(11);
					printf("─  관리자  ────────────────────────────────────────────\n");
					printf(" 제가 옆에서 봐서 아는데 츤츤이한테 한 번만 더 같은 질문하면 도망갈 거예요. 조심해요!\n");
					printf("───────────────────────────────────────────────────────\n\n");
					textcolor(10);
				}
				else if (answer_count == 3) {
					MessageBoxA(NULL, "츤츤이가 짜증내면서 갔어요ㅠㅠ\n", "츤츤이", MB_OK);
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
		strcpy(buf, search_anwser(input, teach)); //input에 맞는 output을 찾아서 buf에 저장, 없으면 NULL
		output = str_malloc(strlen(buf) + 1);  //output을 buf크기만큼 동적할당(널문자 포함)
		strcpy(output, buf); //output에 buf를 넣음


		//print & teach
		if (strcmp(output, "")) { // 질문에 대한 대답있을경우 

			if (teach == 1) { // 가르치기 상태일때
				printf("츤츤이가 알고 있는 질문이예요.\n\n");
				input = input - 3; //input의 크기를 원래대로 해줘 나중에 free
			}
			print_answer(output); //output을st 출력 

		}
		else { // 질문에 일치하는 대답이 없을경우
			printf("츤츤이가 모르는 질문이네요. 알려주세요!\n\n");
			teach_answer(input); // 츤츤이 가르치기
			if (teach == 1) input = input - 3; // 할당 받은 만큼 free하기 위해 다시 input의 크기를 원래대로 해줌
		}

		//FREE
		free(input);
		free(output);

	}

}

//문자열 입력 받기 
void  input_data(char* buf, char* who) {
	fflush(stdin);  //입력장치 초기화 (버퍼 비우는 느낌)
	while (1) { 
		textcolor(10); 
		printf("%s : ", who);
		textcolor(7); 
		get_str(buf, SIZE, stdin);
		printf("\n");
		//내용을 SIZE만큼 프린트 stdin -> 출력장치
		if (strcmp(buf, "")) break; //입력되면 반복문 탈출
	}

}

// 질문에 대한 대답을 찾아서 리턴, 없으면 ""
char* search_anwser(char* input, int teach) {
	FILE* fp;
	int output_num, random, check = 0, i;
	char* buf;
	char* similar;
	buf = str_malloc(SIZE);
	similar = str_malloc(SIZE);

	if ((fp = fopen("question.txt", "r")) == NULL) return "";   //파일이 없으면 리턴

	for (i = 0; i < 2; i++) { //총 2번 파일을 탐색 i - 0.완벽히 일치하는가,  1.조금 일치하는가
		while (!feof(fp)) {// 파일 끝까지 탐색
			fscanf(fp, "%d", &output_num); // 질문의 답이 몇개인지 output_num에 저장
			get_str(buf, SIZE, fp); // buf에 질문 저장

			if (i == 0 && !strcmp(input, buf)) check = 1;//질문과 완벽히 일치하는가

			if (i == 1 && strlen(buf) > 3 && (strstr(input, buf) != NULL || strstr(buf, input) != NULL)) { // 유사한 질문이 있는가
				check = 1;
				if (teach == 0) printf("\n원하는 대답이 아닐경우 \"?? 질문\" 을 통해 츤츤이를 가르쳐 주세요.\n\n");
			}

			if (check == 1)                        //input과 질문과 일치거나 비슷 하면
			{
				random = rand() % output_num;         //질문의 답중 출력할 답을 랜덤으로 설정
				move_line(fp, random);               //출력할 대답으로 이동
				get_str(buf, SIZE, fp);               //대답을 buf에 저장
				fclose(fp);
				return buf;                        //저장한 대답을 리턴
			}
			move_line(fp, output_num + 1);            //다음 질문이 있는 파일위치로 이동
		}
		if (teach == 1) break;
		rewind(fp);                              //파일탐색 위치를 처음으로 돌림
	}

	fclose(fp);
	return "";
}

void print_answer(char* output) {
	textcolor(14);
	printf("─  츤츤이  ────────────────────────\n");
	printf(" %s\n", output);
	printf("───────────────────────────────────\n\n");
	textcolor(10);
}

void  teach_answer(char* input) {
	char answer[SIZE];
	int num, i;
	FILE* fp;
	fp = fopen("question.txt", "a");

	textcolor(12); 
	printf("몇 개의 대답을 알려주시겠어요?( 0 입력시 skip )");
	
	textcolor(7);
	scanf("%d", &num); 
	getchar();

	if (num == 0) {
		fclose(fp);
		return;
	}
	fprintf(fp, "%d%s\n", num, input);   // 대답의 갯수와 질문을 저장      3안녕
	for (i = 0; i < num; i++) {        // 대답의 갯수만큼 for문           반가워
		input_data(answer, "TEACH");      //TEACH : 대답을 입력받음          안녕
		fprintf(fp, "%s\n", answer);      //입력받은 대답을 파일에 저장      ㅎㅇ
	}
	fprintf(fp, " \n"); //공백 처리
	fclose(fp);
	textcolor(2);
	printf("────  학습 완료!────────────────────\n\n");
	textcolor(10);
}

//line만큼 다음줄로 이동
void move_line(FILE* file, int line) {
	int i;
	for (i = 0; i < line; i++) {
		while (!feof(file))
		{
			if (fgetc(file) == '\n') break;
		}
	}
}

//욕설 및 특정단어 필터링
int filtering(char* input) {
	if (strstr(input, "18") != NULL) {
			MessageBoxA(NULL, "필터링 단어 감지, 욕하지마.", "츤츤이", MB_OK); {
		}
		return 1;
	}
	return 0;
}

//문자열 동적할당
char* str_malloc(int len) {
	return (char*)malloc(sizeof(char) * len); //그냥 malloc해도 되는데 자주 쓰여서 함수화
}

//엔터 빼고 파일입력 마지막 엔터를 없애버리고 널 삽입
char* get_str(char* buf, int max, FILE* file) {
	fgets(buf, max, file);
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

//글씨 색변경
void textcolor(int color_number) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}
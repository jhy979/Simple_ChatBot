# Simple_ChatBot
___
## 0. 프로그램 제작 동기
- 2002년 출시된 심심이라는 서비스가 있습니다. 심심이는 모바일 인공지능 대화 엔진으로, 텍스트 기반으로 서비스를 진행하고 있습니다. 어릴 때부터 심심이 라는 서비스를 종종 이용하곤 했었는데 이에 영감을 받아 간단한 챗봇을 만들어 보기로 했습니다. <br><br>
- 기존 심심이 서비스처럼 데이터베이스에 없는 질문에 대한 답변은 사용자가 학습시킬 수 있도록 하고 추가적으로 심심이에는 없는 저만의 특별한 컨셉을 가져오고 싶었습니다. ‘츤데레’같은 성격을 모티브로 까칠한 성격의 채팅 봇 츤츤이를 만들면 재미있지 않을까 라는 생각을 하게 되었고, 말투도 까칠하고 여러 번 같은 질문을 하면 짜증을 내면서 가버리는 컨셉의 채팅봇을 떠올리게 되었습니다.
___
## 1. 프로그램 구현 모습
> 백문이불여일견! 먼저 프로그램 데모부터 보시겠습니다 👇👇👇<br><br>
![ezgif com-gif-maker](https://user-images.githubusercontent.com/32920566/122630005-c24b1000-d0fb-11eb-8183-1056824b1fd5.gif)

___
## 2. 프로그램 설명
___
### 2-1) Flow Chart
<img src = "https://user-images.githubusercontent.com/32920566/122630441-9f6e2b00-d0fe-11eb-9542-94e841460aff.png" width = "50%">

<br>
- 위 순서도를 보겠습니다. 가장 먼저 이름을 적습니다. 그 후 츤츤이에게 질문을 하게 됩니다. 질문은 총 4가지 check를 거치고 나서야 츤츤이의 답변을 받을 수 있습니다. <br> <br>
1) teach check : 일반 질문이 아닌 츤츤이를 가르치기 위한 질문입니다. 이 경우 질문에 대한 대답을 사용자가 직접 작성할 수 있습니다.<br><br>
2) filtering check : 욕설이나 특정 문자열을 포함하는 질문을 필터링하여 경고를 줍니다. 다시 질문을 하도록 합니다.<br><br>
3) program quit check : ‘잘가’를 질문으로 입력하면 프로그램을 종료합니다.<br><br>
4) same question check : 같은 질문을 2번 반복하면 경고, 3번 반복하면 화를 내고 마지막으로 4번 반복할 경우 프로그램이 종료됩니다.<br><br>
- 위의 4가지 케이스에 해당하지 않는 질문이 들어왔다면 프로그램은 output check를 시작합니다. output check는 question.txt(츤츤이의 대답 텍스트 파일)에 질문에 100% 일치하는 문자열이 있다면 그에 해당하는 답변들 중 랜덤으로 하나 출력합니다. <br><br>
- 만약 길이가 3이상이 일치하는 문자열이 있다면 해당 답변들 중에서 랜덤으로 출력합니다. 하지만, 이 대답은 사용자가 원하는 대답이 아닐 수 있으므로 teach를 할지 말지의 기회를 부여합니다. 만약 문자열이 search가 되지 않는다고 하면 출력없이 teach의 기회를 바로 줍니다.<br><br>

___
### 2-2) 자료구조
- 이번 프로젝트에서는 기존 심심이와의 차별성이 반복된 질문에서 시작됩니다. 질문이 반복되어 들어온다면 프로그램은 그에 따라 상이한 방식으로 사용자에게 출력을 반환합니다.

- 같은 질문이 반복적으로 들어오면 화를 내거나 경고합니다.

- 인사는 반복적으로 해도 잘 받아줍니다.

- 이를 구현하기 위한 자료구조로 스택을 사용했습니다. 스택을 사용한 이유는 가장 최근의 질문이 스택에 들어간 경우 top연산을 통해서 가장 최근의 질문과 현재 들어온 질문을 비교하는데 용이하기 때문입니다. 이 LIFO의 개념을 이용해 질문의 반복성을 판단했습니다.
```c
//stack 구현
char stack[MAX_STACK_SIZE][SIZE];
void push(int* top, char stack[][SIZE],char* input) {
	(*top)++;    
	strcpy(stack[*top], input);
}
```
___
### 2-3) 주요 함수 설명
```c void input_data(char* buf, char* who); ```<br>
👉 who(누가) buf(내용)을 입력하는지에 대한 정보를 담습니다. <br>
👉 출력을 하는데 who : buf 식으로 출력해줍니다. <br><br>
```c char* search_answer(char* input, int teach); ```<br>
👉 질문에 대한 대답을 찾아줍니다. question.txt파일을 총 2번 돕니다. 처음 돌때에는 완벽하게 input과 일치하는 문자열이 있는지 찾습니다. 만약 일치하는 문자열이 있다면 대답을 랜덤하게 선택해서 반환해줍니다. 만약 100% 일치하는 파일이 없다면 rewind하여 question.txt를 한번 더 돌면서 길이가 3 이상이 일치하는 문자열이 있는지 확인합니다. 만약 있다고 한다면, 그 중에서 대답을 골라 반환합니다. 반환할 때 이 대답이 정확히 사용자의 니즈에 맞는지 알 수 없기에 teach기회를 주어 사용자가 학습을 시킬지 말지에 대한 여부를 선택하게 합니다.<br><br>
```c void print_answer(char* output); ``` <br>
👉 대답을 출력하는 함수입니다.<br><br>
```c void teach_answer(char* buf); ```<br>
👉 일명‘츤츤이’학습 함수입니다. question.txt 파일에는 츤츤이가 특정 질문에 대답할 수 있는 대답들이 모여 있습니다. 이 텍스트 파일에 대답들을 새롭게 기록하는 함수입니다.<br><br> 

```c void move_line(FILE* file, intline); ```<br> 
👉 line만큼 다음줄로 이동하는 함수입니다. question.txt 파일을 파일포인터로 읽는데 원하는 위치를 제대로 참조하기 위해서 \n를 통해 파일포인터의 위치를 옮겨 주는 역할을 하는 함수입니다.<br><br>
```c int filtering(char* input); ```<br>
👉 욕설 및 특정 단어를 필터링하는 함수입니다. 미리 설정해 놓은 특정 단어들이 input에 들어있는지 판단하고 들어있으면 1을 반환하고 클린한 input이라면 0을 반환하는 함수입니다. <br><br>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { SHIRTS, PANTS, DRESS, OUTER, PAJAMA }Type_e;
char* Type_name[] = { "shirts", "pants", "dress", "outer", "pajama" };
typedef enum {SPRING, SUMMER, FALL, WINTER }Season_e;
char* Season_name[] = {"spring", "summer", "fall", "winter" };


typedef struct {
	char name[20];
	Season_e season;
	char color[10];
	int size;
	Type_e type;
}Cloth_t;
//사이즈 공용체 필드 SML/int?
typedef struct node {
	Cloth_t cloth;
	struct node* next;
}Node_t;

void getText(Node_t* head, void (*add)(Node_t*,Cloth_t)) {
	Cloth_t getCloth = { 0 };
	FILE* fp;
	if ((fp = fopen("closet.txt", "a+")) == NULL) {
		fprintf(stderr, "closet.txt 파일을 열 수 없습니다.");
		exit(1);
	}

	while (!feof(fp)) {
		fscanf(fp, "%s %d %s %d %d\n", getCloth.name, &getCloth.season, getCloth.color, &getCloth.size, &getCloth.type);
		//연청바지 1 skyblue 27 2 //name season color size type
		if (strcmp(getCloth.name,"")==0) break;
		add(head, getCloth);
		//printf("%s %d %s %d %d\n", getCloth.name, getCloth.season, getCloth.color, getCloth.size, getCloth.type);
	}
	fclose(fp);
}

void saveText(Node_t* head) {
	Cloth_t* saveCloth = NULL;
	Node_t* cur = head->next;
	FILE* fp;
	if ((fp = fopen("closet.txt", "w")) == NULL) {
		fprintf(stderr, "closet.txt 파일을 열 수 없습니다.");
		exit(1);
	}

	while (cur != NULL) {
		saveCloth = &cur->cloth;
		fprintf(fp, "%s %d %s %d %d\n", saveCloth->name, saveCloth->season, saveCloth->color, saveCloth->size, saveCloth->type);
		//연청바지 1 skyblue 27 2 //name season color size type
		cur = cur->next;
	}
	fclose(fp);
}

void clearBuffer()
{
	while (getchar() != '\n');
}

void addNode(Node_t* head, Cloth_t cloth) {
	Node_t* newNode = (Node_t*)malloc(sizeof(Node_t));
	if (newNode == NULL) { // 반환값이 NULL인지 검사
		printf("동적 메모리 할당 오류\n");
		exit(1);
	}

	/*
	//find last node
	Node_t* last = head;
	int i = 0;
	while (last->next != NULL) {
		last = last->next;
	}
	
	//선정리
	last->next = newNode;

	newNode->cloth = cloth;
	newNode->next = NULL;
	*/
	newNode->cloth = cloth;
	Node_t* nextNode = head->next;
	Node_t* prevNode = head;
	//노드 삽입 위치 검색
	while (nextNode != NULL) {
		if (nextNode->cloth.season > newNode->cloth.season)
			break;
		prevNode = nextNode;
		nextNode = nextNode->next;
	}
	//노드 삽입
	prevNode->next = newNode;
	newNode->next = nextNode;
}

void addCloth(Node_t* head) {
	Cloth_t newCloth = { 0 };
	
	printf("옷의 이름: ");
	scanf("%s", newCloth.name);
	printf("어떤 계절 옷인가요?\n(0.spring, 1.summer, 2.fall, 3.winter): ");
	scanf("%d", &newCloth.season);
	printf("옷의 색: ");
	scanf("%s", newCloth.color);
	printf("옷의 사이즈: ");
	scanf("%d", &newCloth.size);
	printf("옷의 유형\n(0.shirts, 1.pants, 2.dress, 3.outer, 4.pajama): ");
	scanf("%d", &newCloth.type);

	addNode(head, newCloth);
}
//에러처리!!

int compare(Node_t* target, const char* word) {

	if (strcmp(target->cloth.name, word) == 0)
		return 1; 
	if (strcmp(Season_name[target->cloth.season], word) == 0) 
		return 2;
	if (strcmp(target->cloth.color, word) == 0) 
		return 3;
	if ((target->cloth.size == atoi(word))) 
		return 4; 
	if (strcmp(Type_name[target->cloth.type], word) == 0) 
		return 5;
	
	return 0;
}

void search(Node_t* head, void (*func)(Node_t*, Node_t*)) {
	Node_t* curNode = head->next;
	Node_t* prevNode = head;
	char word[20] = { 0 };

	printf("검색어를 입력하세요: ");
	scanf("%s", word);

	while (curNode != NULL) {
		if (compare(curNode, word) > 0) func(curNode,head);
		prevNode = curNode;
		curNode = curNode->next;
	}
}

void printNode(Node_t* curNode, Node_t* head) {
	printf("-----------------\n");
	printf("cloth name : %s\n", curNode->cloth.name);
	printf("cloth season : %s\n", Season_name[curNode->cloth.season]);
	printf("cloth color : %s\n", curNode->cloth.color);
	printf("cloth size : %d\n", curNode->cloth.size);
	printf("cloth type : %s\n", Type_name[curNode->cloth.type]);
	printf("-----------------\n");
}

void printList(const Node_t* head, void (*print)(Node_t*, Node_t*)) {
	Node_t* curNode = head->next;

	while (curNode != NULL) {
		print(curNode,curNode);
		curNode = curNode->next;
	}
}

void deleteNode(Node_t* targetNode, Node_t* head) {
	/*
	Node_t* target = head->next;
	Node_t* prev = head;
	while (target != NULL){
		if (strcmp(target->cloth.name, name) == 0) break;
		else {
			Cloth_t* deleteCloth = &target->cloth;
			prev = target;
			target = target->next;
		}
	}
	*/
	char option;
	Node_t* prevNode = head;
	if (targetNode == NULL) return;
	while (prevNode->next != targetNode) {
		prevNode = prevNode->next;
	}
	printNode(targetNode, head);
	printf("삭제하시겠습니까? (Y/N) : ");
	scanf(" %c", &option);
	
	if (option == 'Y') {
		//선정리
		prevNode->next = targetNode->next;

		//메모리 해제(운영체제에게 반납)
		//free(deleteCloth);
		free(targetNode);
	}
}

void MENU() {
	Node_t head;
	head.next = NULL;
	printf("======메뉴======\n");
	printf("1. 옷 추가하기\n");
	printf("2. 옷 전체 출력하기\n");
	printf("3. 옷 버리기\n");
	printf("4. 옷 검색하기\n");
	printf("5. 정보 수정하기\n");
	printf("9. 저장 후 종료\n");
	printf("================\n");
	printf("메뉴를 선택하세요: ");
}

void modify(Node_t* targetNode, Node_t* head) {
	char option;
	if (targetNode == NULL) return;
	Node_t* prevNode = head;
	while (prevNode->next != targetNode) {
		prevNode = prevNode->next;
	}
	printNode(targetNode, targetNode);
	printf("수정하시겠습니까? (Y/N) : ");
	scanf(" %c", &option);
	if (option == 'Y') {
		//선정리
		prevNode->next = targetNode->next;

		//메모리 해제(운영체제에게 반납)
		//free(deleteCloth);
		free(targetNode);
		printf("수정정보를 입력하세요\n");
		addCloth(head);
	}
}

int main() {
	Node_t head;
	head.next = NULL;
	int option;

	getText(&head, addNode);
	while (1) {
		MENU();
		scanf("%d", &option);
		switch (option) {
			case 1: addCloth(&head); break;
			case 2: printList(&head, printNode); break;
			case 3: search(&head, deleteNode); break;
			case 4: search(&head, printNode); break;
			case 5: search(&head, modify); break;
			case 9: saveText(&head); return 0;
		}
		clearBuffer();
	}
}

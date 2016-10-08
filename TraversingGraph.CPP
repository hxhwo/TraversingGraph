#include <iostream>
#include <malloc.h>
using namespace std;
#define NULL 0
#define MAX_VERTEX_NUM 30  //��󶥵���
typedef struct ArcNode{
	int adjvex;   //�ñ���ָ�Ķ����λ��
	struct ArcNode *nextarc;   //ָ����һ���ߵ�ָ��
}ArcNode;   //���Ľ��

typedef struct VNode{
	char data;        //������Ϣ�������ݵȣ�
	ArcNode *firstarc;        //ָ���һ�������ö���ıߵĻ�ָ��
}VNode, AdjList[MAX_VERTEX_NUM];   //ͷ���

typedef struct MGraph{
	AdjList vertices;
	int visited[MAX_VERTEX_NUM];    //���ʱ�־����
	int vexnum, arcnum;   //ͼ�ĵ�ǰ�������ͻ���
}MGraph;

//���ض���v�ڶ��������е�λ��
int LocateVex(MGraph &G, char v){
	int i;
	for (i = 0; v != G.vertices[i].data && i < G.vexnum; i++)
		;
	return i;
}

//���붥��
void add_vex(MGraph &G){
	cout << "��������ͼ������: " << endl;
	cin >> G.vexnum;
	for (int i = 0; i < G.vexnum; i++){
		cout << "�����" << i + 1 << "������:" << endl;
		cin >> G.vertices[i].data;   //���춥������
		G.vertices[i].firstarc = NULL;
	}
}

//�����
void add_arc(MGraph &G){
	ArcNode *s, *t, *p;
	cout << "��������ͼ����: " << endl;
	cin >> G.arcnum;
	char v1, v2;
	for (int k = 0; k < G.arcnum; k++){
		cout << "�����" << k + 1 << "���߹�������������:" << endl;
		cin >> v1 >> v2;
		int i = LocateVex(G, v1);
		int j = LocateVex(G, v2);    //ȷ��v1 , v2��G�е�λ��
		s = (ArcNode*)malloc(sizeof(ArcNode));
		t = (ArcNode*)malloc(sizeof(ArcNode));
		s->adjvex = j;   //�ñ���ָ��Ķ����λ��Ϊj
		s->nextarc = NULL;
		if (!G.vertices[i].firstarc){
			G.vertices[i].firstarc = s;
		}
		else{
			for (p = G.vertices[i].firstarc; p->nextarc; p = p->nextarc)
				;
			p->nextarc = s;
		}
		t->adjvex = i;   //�ñ���ָ��Ķ����λ��Ϊj
		t->nextarc = NULL;
		if (!G.vertices[j].firstarc){
			G.vertices[j].firstarc = t;
		}
		else{
			for (p = G.vertices[j].firstarc; p->nextarc; p = p->nextarc)
				;
			p->nextarc = t;
		}
	}
}

//�����ڽӱ�
MGraph CreateMGraph(MGraph &G){
	add_vex(G);        //���ӽڵ�
	add_arc(G);        //���ӱ�     
	cout << "����ͼ�����ɹ���" << endl;
	return G;
}

//����ڽӱ�
void PrintMGraph(MGraph &G){
	int i;
	ArcNode *p;
	cout << "���    ����    �ڵ���" << endl;
	for (i = 0; i < G.vexnum; i++){
		cout << "  " << i << "       " << G.vertices[i].data << "      ";
		for (p = G.vertices[i].firstarc; p; p = p->nextarc)
			cout << p->adjvex << "  ";
		cout << endl;
	}
}


//(�����������)
void DFS(MGraph &G, int i, int s, char path[]){
	ArcNode *p;
	G.visited[i] = 1;
	static int found = 0;
	static int rear = 0;        //·�������βָ��
	path[rear++] = G.vertices[i].data;
	path[rear] = '\0';    //ע�⽫���һλ��Ϊ'\0'
	for (p = G.vertices[i].firstarc; p && !found; p = p->nextarc)
	if (s == p->adjvex){    //�ҵ�Ŀ��ڵ�
		found = 1;    //found��Ϊ1���˳�
		path[rear++] = G.vertices[p->adjvex].data;
		path[rear] = '\0';
	}
	else if (!G.visited[p->adjvex]){
		DFS(G, p->adjvex, s, path);
	}
	if (!found)    //����ýڵ�������ڽӵ㶼����Ŀ��ڵ㣬�����ڽӵ���ڽӵ�Ҳ����Ŀ��ڵ㣬��ô���Ͳ���·���ϵġ��˳�·��
		path[--rear] = '\0';
}

typedef struct qnode{
	char data;
	struct qnode * pre;
	struct qnode * next;
}Qnode, *Queueptr;    //    ������ Qnode��struct qnode�ı�����Queueptr��struct qnode *�ı���

typedef struct{
	Queueptr front;    //��ͷָ��
	Queueptr rear;    //��βָ��
}LinkQueue;    //��������

//��ʼ������
void InitQueue(LinkQueue *Q){
	Q->front = (Queueptr)malloc(sizeof(Qnode));    //��ͷ�Ͷ�βָ��ͷ���
	if (!Q->front){
		cout << "�޿����ڴ棡" << endl;    //�洢����ʧ��
	}
	else{
		Q->front->next = NULL;
		Q->rear = Q->front;
	}
}

//��Ӻ���
void Enqueue(LinkQueue *Q, int value){
	Queueptr newp = (Queueptr)malloc(sizeof(Qnode));
	if (!newp)
		cout << "�޿����ڴ棡" << endl;    //�洢����ʧ��
	newp->data = value;
	newp->next = NULL;
	Q->rear->next = newp;    //p����ԭ��β
	Q->rear = newp;    //p��Ϊ�µĶ�β
}

//(�����������)
void BFS(MGraph &G, int i, int s, char path[]){    //�ӵ�i�����㿪ʼ����
	ArcNode *p;
	int found = 0;
	char gettop;
	int top;
	for (int x = 0; x<G.vexnum; x++)
		G.visited[x] = 0;
	LinkQueue *Q = (LinkQueue *)malloc(sizeof(LinkQueue));
	InitQueue(Q);			//��ʼ������
	G.visited[i] = 1;        //����Դ��
	Enqueue(Q, G.vertices[i].data);    //Դ�����
	Q->rear->pre = Q->front;    //��һ���ڵ��ǰ��ָ��ָ��front
	Queueptr temp = Q->front->next;        //���������е�ÿ������
	while (!found){
		gettop = temp->data;
		top = LocateVex(G, gettop);
		for (p = G.vertices[top].firstarc; p && !found; p = p->nextarc){
			if (s == p->adjvex){    //�ҵ�Ŀ���
				found = 1;    //��־��Ϊ1������У��˳�
				Enqueue(Q, G.vertices[p->adjvex].data);
				Q->rear->pre = temp;    //��������еĵ㣬ǰ��ָ��ָ������Դ��
			}
			else if (!G.visited[p->adjvex]){    //������ǣ��ͷ��ʣ����������
				G.visited[p->adjvex] = 1;
				Enqueue(Q, G.vertices[p->adjvex].data);    //���
				Q->rear->pre = temp;
			}
		}
		temp = temp->next;
	}//while
	int j = 0;
	for (temp = Q->rear; temp != Q->front; temp = temp->pre)    //���ƶ����еĶ��㵽�ַ�����ȥ
		path[j++] = temp->data;
	path[j] = '\0';
	free(Q);
}

//��ͼ�����·��
void GetV(MGraph &G, char way){
	char start, end;
	cout << "��ѡ��һ����ʼ���㣺";
	cin >> start;
	while (LocateVex(G, start) >= G.vexnum){
		cout << "û���ҵ��˿�ʼ���㣡���������룺";
		cin >> start;
	}
	cout << "��ѡ��һ���������㣺";
	cin >> end;
	while (LocateVex(G, end) >= G.vexnum){
		cout << "û���ҵ��˽������㣡������ѡ��һ�����㣺";
		cin >> end;
	}
	int i = LocateVex(G, start);
	int s = LocateVex(G, end);
	int j = 0, x = 0;
	for (j = 0; j<G.vexnum; j++)
		G.visited[j] = 0;
	if (way == 'D'){
		char *path;        //���·��
		path = (char *)malloc(MAX_VERTEX_NUM*sizeof(char));
		DFS(G, i, s, path);
		cout << "����" << start << "������" << end << "�ļ�·��Ϊ:" << endl;
		while (path[x])
			cout << path[x++];    //���·��
	}
	else{
		char *path;        //���·��
		path = (char *)malloc(MAX_VERTEX_NUM*sizeof(char));
		BFS(G, s, i, path);    //�����������
		cout << "����" << start << "������" << end << "�����·��Ϊ:" << endl;
		while (path[x]){
			cout << path[x++];    //���·��
		}
		cout << endl;
		cout << "����" << start << "������" << end << "�����·������Ϊ:" << x-1 << endl;
	}
	cout << endl;
}

void display_menu(){
	int i;
	char ch = 'y';
	char way;
	MGraph G;
	G.vexnum = 0;
	while (ch == 'y'){
		cout << "--------------------�˵�-------------------" << endl;
		cout << " " << "1  |�����ڽӱ�����ͼ" << endl;
		cout << " " << "2  |��ʾͼ���ڽӱ�" << endl;
		cout << " " << "3  |��һ���Ӷ��� i ������ s �ļ�·��" << endl;
		cout << " " << "4  |����������֮���һ��·��������̵�·��" << endl;
		cout << " " << "0  |�˳���" << endl;
		cout << "---------------------------------------------" << endl;
		cout << endl << "��ѡ��Ҫ���еĲ���(0~4)��";
		cin >> i;
		switch (i){
		case 1:
			if (G.vexnum == 0)
				CreateMGraph(G);
			else
				cout << "ͼ�Ѵ�����" << endl;
			break;
		case 2:
			if (G.vexnum != 0)
				PrintMGraph(G);
			else
				cout << "ͼΪ�գ�" << endl;
			break;
		case 3:
			if (G.vexnum == 0)
				cout << "���ȴ���ͼ��" << endl;
			else{
				way = 'D';
				GetV(G, way);
			}
			break;
		case 4:
			if (G.vexnum == 0)
				cout << "���ȴ���ͼ��" << endl;
			else{
				way = 'B';
				GetV(G, way);
			}
			break;
		case 0:
			exit(1);
		default:
			cout << "����0~4֮�����ѡ��" << endl;
		}
		cout << "�Ƿ����y/n��";
		cin >> ch;
		system("cls");
	}
}
int main()
{
display_menu();
return 0;
}
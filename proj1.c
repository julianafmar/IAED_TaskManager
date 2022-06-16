/*
 * Ficheiro: proj1.c
 * Autora: Juliana Fernandes Marcelino
 * Descricao: Este programa baseia-se no metodo kanban para organizar as
 * tarefas introduzidas por um ou mais utilizadores de modo a distribui-las
 * de forma a que todas sejam eficazmente realizadas.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Constantes para valores de verdade*/
#define VERDADEIRO 1
#define FALSO 0

/*Numero maximo de carateres no input*/
#define MAXINPUT 100000

/*Numero maximo de carateres numa descricao*/
#define DESCRICAO 50

/*Uma constante geral para vetores sem valores especificos*/
#define MAXGERAL 100

/*Numero maximo de atividades*/
#define ATIVIDADES 10

/*Numero maximo de carateres no nome de uma atividade*/
#define ATIVCARAT 21

/*Numero maximo de tarefas*/
#define TAREFAS 10000

/*Numero maximo de utilizadores*/
#define UTILIZADORES 50

/*Numero msximo de carateres no nome de utilizador*/
#define UTILIZCARAT 21

/*Estrutura que identifica uma tarefa*/
typedef struct {
	int identificador;
	char descricao[DESCRICAO];
	char utilizador[UTILIZCARAT];
	char atividade[ATIVCARAT];
	int duracao;
	int inicio;
} Tarefa;

void t(Tarefa tarefas_todas[], int duracao, char descricao[], int durinvalida);
void l(Tarefa tarefas_todas[], char input[], int res);
void n(char input[]);
void u(char input[]);
void m(Tarefa tarefas_todas[], int id, char uti[], char ativ[]);
void d(Tarefa tarefas_todas[], char input[]);
void a(char input[]);

/*O valor do tempo quando o programa e iniciado.*/
int tempo = 0;

/*Todos os utilizadores ficam guardados neste vetor.*/
char utilizadores[UTILIZADORES][UTILIZCARAT];

/*Todas as atividades ficam guardadas neste vetor.*/
char atividades[ATIVIDADES][ATIVCARAT] = {"TO DO", "IN PROGRESS", "DONE"};

/*Le o input e identifica o comando escolhido, passando para a funcao
 * correspondente.*/
int main()
{
	char duracaostr[MAXGERAL], descr[DESCRICAO];
	char input[MAXINPUT];
	int i, j;
	int dur, vazio, size, id, durinvalida = FALSO;
	char idstr[MAXGERAL], uti[UTILIZCARAT], ativ[ATIVCARAT];

	Tarefa tarefas_todas[TAREFAS];

	fgets(input, MAXINPUT, stdin);
	
	while(input[0] != 'q'){

		if (input[0] == 't'){
			for(i = 1; input[i] == ' '; i++)
				;
			j = 0;
			for(; input[i] != ' '; i++){
				if (input[i] == '.' || input[i] == '-')
					durinvalida = VERDADEIRO;
				duracaostr[j] = input[i];
				j++;
			}
			duracaostr[j++] = '\0';
			for(; input[i] == ' '; i++)
                              	;
			for(j = 0; input[i] != '\0'; i++){
				descr[j] = input[i];
				j++;
			}
			descr[j++] = '\0';

			dur = atoi(duracaostr);
			t(tarefas_todas, dur, descr, durinvalida);
		}

		else if (input[0] == 'l'){
			size = strlen(input);
			if (size == 2)
				vazio = VERDADEIRO;
			else
				vazio = FALSO;

			l(tarefas_todas, input, vazio);
		}

		else if (input[0] == 'n')
			n(input);

		else if (input[0] == 'u')
			u(input);

		else if (input[0] == 'd')
			d(tarefas_todas, input);

		else if (input[0] == 'a')
			a(input);

		else if (input[0] == 'm'){
			j = 0;
			for(i = 1; input[i] == ' '; i++)
				;
			for(; input[i] != ' '; i++)
				idstr[j++] = input[i];
			idstr[j++] = '\0';

			j = 0;
			for(; input[i] == ' '; i++)
                                ;
			for(; input[i] != ' '; i++)
                                uti[j++] = input[i];
                  	uti[j++] = '\0';
			
			j = 0;
			for(; input[i] == ' '; i++)
                                ;
                        for(; input[i] != '\0' && input[i] != EOF && input[i] != '\n'; i++)
                                ativ[j++] = input[i];
			ativ[j++] = '\0';

			id = atoi(idstr);

			m(tarefas_todas, id, uti, ativ);
		}

		fgets(input, MAXINPUT, stdin);
		durinvalida = FALSO;
	}
	return 0;
}

/*Caso o input contenha o comando t, e aqui que se cria a tarefa
 * indicada atraves de um input que contenha uma descricao e uma 
 * duracao prevista. E atribuido um identificador a tarefa, sendo
 * esse o output desta funcao.*/
void t(Tarefa tarefas_todas[], int dur, char descr[], int durinvalida)
{
	int i, erro = FALSO;

	for(i = 0; tarefas_todas[i].identificador != 0; i++)
		if(strcmp(tarefas_todas[i].descricao, descr) == 0)
			erro = 1;
	if (dur == 0)
		durinvalida = VERDADEIRO;

	if (i == TAREFAS)
		printf("too many tasks\n");

	else if (erro == VERDADEIRO)
		printf("duplicate description\n");

	else if (durinvalida == VERDADEIRO)
		printf("invalid duration\n");

	else{
		strcpy(tarefas_todas[i].atividade, "TO DO");
		tarefas_todas[i].identificador = i + 1;
		strcpy(tarefas_todas[i].descricao, descr);
		tarefas_todas[i].duracao = dur;
		tarefas_todas[i].inicio = 0;
		printf("task %d\n", tarefas_todas[i].identificador);
	}
}

/*Caso o input contenha o comando l, esta funcao lista as tarefas indicadas ou
 * todas as atividades, caso nenhuma seja especificamente indicada, por ordem
 * alfabetica da sua descricao.*/
void l(Tarefa tarefas_todas[], char input[], int vazio)
{
	int i = 3, j = 0, id, n, size, size_auxvet, right, left;
	char idstr[MAXGERAL];
	int auxvet[TAREFAS];

	size = strlen(input);

	if(vazio == FALSO){

		for(i = 1; input[i] == ' '; i++)
                                ;

		while(input[i] != ']' && i < (size - 1) && input[i] != '\0' && input[i] != '\n'){

			for(; input[i] == ' '; i++)
                                ;

			for(; input[i] != ' ' && input[i] != ']' && input[i] != '\0' && input[i] != '\n'; i++){
				idstr[j] = input[i];
				j++;
			}
			idstr[j++] = '\0';
			id = atoi(idstr);
			i++;

			j = 0;
			for(n = 0; tarefas_todas[n].identificador != id && n <= TAREFAS; n++)
				;

			if(n >= TAREFAS || id <= 0)
				printf("%d: no such task\n", id);
			
			else
				printf("%d %s #%d %s", tarefas_todas[n].identificador, tarefas_todas[n].atividade, tarefas_todas[n].duracao, tarefas_todas[n].descricao);	
		}
	}

	else{
		for(i = 0; tarefas_todas[i].identificador != 0; i++)
			auxvet[i] = tarefas_todas[i].identificador;
		size_auxvet = i;
		right = i - 1;
		left = 0;

		for(i = left; i < right; i++){
			int aux, min = i;
                        for(j = i + 1; j <= right; j++)
                                if(strcmp(tarefas_todas[auxvet[j]-1].descricao, tarefas_todas[auxvet[min]-1].descricao) < 0)
					min = j;
			aux = auxvet[i];
                	auxvet[i] = auxvet[min];
                	auxvet[min] = aux;
		}

		for(i = 0; i < size_auxvet; i++)
			printf("%d %s #%d %s", tarefas_todas[auxvet[i]-1].identificador, tarefas_todas[auxvet[i]-1].atividade, tarefas_todas[auxvet[i]-1].duracao, tarefas_todas[auxvet[i]-1].descricao);
	}
}

/*Quando o input contem n, o tempo do sistema aumenta em relacao ao argumento
 * introduzido, sendo mostrado logo em seguida.*/
void n(char input[])
{
	int i, j = 0;
	int novo_tempo = 0, erro = FALSO, size;
	char novo_tempostr[MAXGERAL];

	size = strlen(input);

	for(i = 1; input[i] == ' '; i++)
                                ;

	for(; input[i] != '\0' && input[i] != EOF && i < size - 1; i++){
		if (input[i] > '9' || input[i] < '0')
			erro = VERDADEIRO;
		novo_tempostr[j] = input[i];
		j++;
	}
	novo_tempostr[j] = '\0';


	if (erro == FALSO){
		novo_tempo = atoi(novo_tempostr);
        	tempo = tempo + novo_tempo;
	        printf("%d\n", tempo);
	}

	else
		printf("invalid time\n");

}

/*Se o comando introduzido for u, e criado um novo utilizador ou os
 * utilizadores sao listados, dependendo da existencia ou nao de argumentos,
 * respetivamente.*/
void u(char input[])
{
	int i, size, j, erro = FALSO;
	char uti[UTILIZCARAT];

	size = strlen(input);

	if(size == 2)
		for(i = 0; strcmp(utilizadores[i], "") != 0; i++)
			printf("%s\n", utilizadores[i]);

	else{
		for(i = 1; input[i] == ' ' || input[i] == '['; i++)
        		;
		for(j = 0; input[i] != '\0' && input[i] != ']' && input[i] != '\n' && i < size-1; i++)
			uti[j++] = input[i];
		uti[j++] = '\0';

		for(i = 0; strcmp(utilizadores[i], "") != 0; i++)
			if (strcmp(utilizadores[i], uti) == 0)
				erro = VERDADEIRO;

		if (erro == VERDADEIRO)
			printf("user already exists\n");
		else if (i >= UTILIZADORES)
        	        printf("too many users\n");
		else
			strcpy(utilizadores[i], uti);
	}
}

/*O comando m move uma tarefa da atividade em que se encontra para outra
 * escolhida pelo utilizador. Caso a atividade seja movida para DONE, o
 * output corresponde ao tempo gasto desde que a tarefa foi iniciada ate
 * ao final e a diferenca entreo tempo gasto e o previsto.*/
void m(Tarefa tarefas_todas[], int id, char uti[], char ativ[])
{
	int i, gasto, slack;
	int existeativ = FALSO, existeuti = FALSO, existetarefa = FALSO, comecou = FALSO;

	for (i = 0; strcmp(atividades[i], "") != 0; i++)
		if (strcmp(atividades[i], ativ) == 0)
			existeativ = VERDADEIRO;

	for (i = 0; strcmp(utilizadores[i], "") != 0; i++)
		if (strcmp(utilizadores[i], uti) == 0)
			existeuti = VERDADEIRO;

	for (i = 0; tarefas_todas[i].identificador != 0; i++)
		if (tarefas_todas[i].identificador == id)
			existetarefa = VERDADEIRO;
	
	if (strcmp(ativ, "TO DO") == 0 && strcmp(tarefas_todas[id - 1].atividade, "TO DO") != 0)
		comecou = VERDADEIRO;

	if (existetarefa == FALSO)
		printf("no such task\n");
	else if (comecou == VERDADEIRO)
		printf("task already started\n");
	else if (existeuti == FALSO)
                printf("no such user\n");
	else if (existeativ == FALSO)
                printf("no such activity\n");
	else{
		if (strcmp(tarefas_todas[id - 1].atividade, "TO DO") == 0){
			strcpy(tarefas_todas[id - 1].atividade, ativ);
			tarefas_todas[id - 1].inicio = tempo;
		}

		if (strcmp(ativ, "DONE") == 0){
			strcpy(tarefas_todas[id - 1].atividade, ativ);
			gasto = tempo - tarefas_todas[id - 1].inicio;
			slack = gasto - tarefas_todas[id - 1].duracao;
			printf("duration=%d slack=%d\n", gasto, slack);
		}

		else
			strcpy(tarefas_todas[id - 1].atividade, ativ);
	}
}

/*Ao usar o comando d, todas as tarefas numa determinada atividade dada pelo
 * input sao listadas por ordem do instante em que foram iniciadas ou, no
 * caso do instante ser o mesmo, sao organizadas por ordem alfabetica da
 * sua descricao.*/
void d(Tarefa tarefas_todas[], char input[])
{
	int size, i, size_auxvet, aux, j = 0, erro = VERDADEIRO, vazio = VERDADEIRO;
	int auxvet[TAREFAS];
	char ativ[ATIVCARAT];

	size = strlen(input);

	for(i = 1; input[i] == ' '; i++)
                ;

	for(; i < size - 1; i++){
		ativ[j++] = input[i];
	}
	ativ[j++] = '\0';

	for(i = 0; strcmp(atividades[i], "") != 0; i++)
		if (strcmp(ativ, atividades[i]) == 0)
			erro = FALSO;

	if(erro == FALSO){
		j = 0;
		for(i = 0; tarefas_todas[i].identificador != 0; i++)
			if(strcmp(tarefas_todas[i].atividade, ativ) == 0){
				auxvet[j++] = tarefas_todas[i].identificador;
				vazio = FALSO;
			}

		size_auxvet = i;

		if(vazio == FALSO){
			for(j = size_auxvet - 1; j > 0; j--){
				for(i = 0; i < j; i++){
					if(tarefas_todas[auxvet[i]-1].inicio > tarefas_todas[auxvet[i+1]-1].inicio){
						aux = auxvet[i];
						auxvet[i] = auxvet[i+1];
						auxvet[i+1] = aux;
					}
				}
			}

			for(j = 0; j <= size_auxvet; j++){
        	                for(i = j; i <= size_auxvet; i++){		
					if (tarefas_todas[auxvet[i]-1].inicio == tarefas_todas[auxvet[i+1]-1].inicio)
						if(strcmp(tarefas_todas[auxvet[i]-1].descricao, tarefas_todas[auxvet[i+1] - 1].descricao) > 0){
                                			aux = auxvet[i];
                                        		auxvet[i] = auxvet[i+1];
                                        		auxvet[i+1] = aux;
	                        	}
				}
			}

			for(i = 0; i < size_auxvet; i++)
				printf("%d %d %s", tarefas_todas[auxvet[i]-1].identificador, tarefas_todas[auxvet[i]-1].inicio, tarefas_todas[auxvet[i]-1].descricao);
		}
	}
	else
		printf("no such activity\n");
}

/*Se o comando a for utilizado, as atividades sao todas listadas caso nao
 * haja mais argumentos ou, caso seja dado um argumento, e criada uma
 * nova atividade.*/
void a(char input[])
{
	int i, j = 0, size;
	int invalido = FALSO, duplo = FALSO;
	char ativ[ATIVCARAT];

	size = strlen(input);

	for(i = 1; input[i] == ' ' || input[i] == '['; i++)
                ;

	if(size == 2)
		for (i = 0; strcmp(atividades[i], "") != 0; i++)
                        printf("%s\n", atividades[i]);

	else{
		for (; i < (size - 1) && input[i] != ']'; i++){
			if (input[i] >= 'a' && input[i] <= 'z')
				invalido = VERDADEIRO;
			ativ[j++] = input[i];
		}
		ativ[j++] = '\0';

		for (i = 0; strcmp(atividades[i], "") != 0; i++){
			if (strcmp(atividades[i], ativ) == 0)
				duplo = VERDADEIRO;
		}
		if (duplo == VERDADEIRO)
			printf("duplicate activity\n");
		else if (invalido == VERDADEIRO)
			printf("invalid description\n");
		else if (i >= ATIVIDADES)
                	printf("too many activities\n");
		else
			strcpy(atividades[i], ativ);
	}
}

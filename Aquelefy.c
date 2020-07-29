#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>
#define NOME_ARQUIVO "registro_musicas.bin"
typedef struct musica{
	int id;
	char nome_musica[80];
	char nome_artista[80];
	char estilo[80];
	int tempo;
	int dia, mes, ano;
	int ativo;
}musica;
void gravar_arquivo();
void cadastrar_musica();
void visualizar_musicas();
void pesquisar_musica();
void tempo_total();
void maior_menor();
int editar();
int excluir();
int menu();	
int cabecalho();
int ordenar();
int contador();
int imprimir_formatado();
int main(){
	gravar_arquivo();
	setlocale(LC_ALL, "Portuguese");
	menu();
	return 0;
}
//Função que imprime cabeçalho formatado
int cabecalho(){
	system("cls");
	printf("%-9s %-36s %-36s %-15s %-9s %s\n", "ID", "NOME DA MUSICA", "NOME DO ARTISTA", "ESTILO", "TEMPO", "DATA DO CADASTRO");
	printf("------------------------------------------------------------------------------------------------------------------------------\n");
	return 0;
}
//Função para imprimir formatado de acordo o cabeçalho
int imprimir_formatado(musica m){
	printf("%-9d %-36s %-36s %-15s %-9d ", m.id, m.nome_musica, m.nome_artista, m.estilo, m.tempo);
	if(m.dia < 10)
		printf("0%d/", m.dia);
	else
		printf("%d/", m.dia);
	if(m.mes < 10)
		printf("0%d/", m.mes);
	else
		printf("%d/", m.mes);
	printf("%d\n", m.ano);
	return 0;
}
//criar arquivo
void gravar_arquivo(){
	FILE *arquivo;
	arquivo = fopen(NOME_ARQUIVO, "rb");
	if (arquivo == NULL){
		arquivo = fopen(NOME_ARQUIVO, "wb");
	}
	fclose(arquivo);
}
//registrar música nova
void cadastrar_musica(){
	musica temp;
	FILE *arquivo;
	arquivo = fopen(NOME_ARQUIVO, "a");
	//ponteiro para struct que armazena data e hora  
 	struct tm *data_hora_atual;     
 	 //variável do tipo time_t para armazenar o tempo em segundos  
  	time_t segundos;
  	//obtendo o tempo em segundos  
  	time(&segundos);   
  	//para converter de segundos para o tempo local  
 	//utilizamos a função localtime  
  	data_hora_atual = localtime(&segundos); 
	system("cls");
	printf("---------------CADASTRO DE MÚSICA---------------\n");
	setbuf(stdin, NULL);
	printf("Nome da música: ");
	gets(temp.nome_musica);
	setbuf(stdin, NULL);
	printf("Nome do artista/banda: ");
	gets(temp.nome_artista);
	setbuf(stdin, NULL);
	printf("Estilo da música: ");
	gets(temp.estilo);
	do{
		setbuf(stdin, NULL);
		printf("Duracao da música (segundos): ");
		scanf("%d", &temp.tempo);
		if(temp.tempo <= 0)
			printf("DIGITE UMA DURACAO VALIDA\n");
	}while(temp.tempo <= 0);

	srand(time(NULL));
	temp.id = rand() % 1000;
	temp.dia = data_hora_atual->tm_mday;
	temp.mes = data_hora_atual->tm_mon+1;
	temp.ano = data_hora_atual->tm_year+1900;
	temp.ativo = 1;
	fwrite(&temp, sizeof(musica), 1, arquivo);
	fclose(arquivo);
	system("cls");
	printf("MÚSICA CADASTRADA COM SUCESSO!\n");
	getch();
}
//listar musica cadastrada
void visualizar_musicas(){
	FILE *arquivo;
	musica m;
	arquivo = fopen(NOME_ARQUIVO, "rb");
	cabecalho();
	while(fread(&m, sizeof(musica), 1, arquivo) == 1)
	{
		if(m.ativo)
			imprimir_formatado(m);
	}
}
//pesquisar música pelo ID
void pesquisar_musica(){
	FILE *arquivo;
	musica m;
	arquivo = fopen(NOME_ARQUIVO, "rb");
	system("cls");
	int procura_id;
	int cont = 0;
	system("cls");
	printf("Digite o ID da música desejada: ");
	scanf("%d", &procura_id);
	while(!feof(arquivo)){
		fread(&m, sizeof(musica), 1, arquivo);
		if(m.id == procura_id){
			if(m.ativo){
				cabecalho();
				imprimir_formatado(m);
				cont++;
			}
			break;
		}
		else if(feof(arquivo)){
			break;
		}
		
	}
	if (cont == 0){
		printf("ID NÃO ENCONTRADO!\n");
	}
	fclose(arquivo);
	getch();	
}
//calculo tempo  total das músicas cadastradas em minutos
void tempo_total(){
	FILE *arquivo;
	musica m;
	int soma_segundos = 0;
	arquivo = fopen(NOME_ARQUIVO, "rb");
	while(fread(&m, sizeof(musica), 1, arquivo)){
		if(m.ativo)
			soma_segundos += m.tempo;
		if(feof(arquivo)){
			break;
		}
	}
	system("cls");
	printf("TEMPO TOTAL EM MINUTOS = %.1f minutos", (float)soma_segundos / 60);
	fclose(arquivo);
	getch();
}
//exibir maior e menor músicas cadastradas
void maior_menor(){
	FILE *arquivo;
	musica temp, maior, menor;
	int cont = 0;
	arquivo = fopen(NOME_ARQUIVO, "rb");
	do{
		fread(&temp, sizeof(musica), 1, arquivo);
		maior = temp;
		menor = temp;
		if(temp.ativo)
			cont++;
	}while(fread(&temp, sizeof(musica), 1, arquivo) == 1 && !temp.ativo);

	system("cls");
	if(temp.ativo){
		while(fread(&temp, sizeof(musica), 1, arquivo) == 1){
			if (temp.tempo > maior.tempo){
				if(temp.ativo)
					maior = temp;
			}
			else{
				if (temp.tempo < menor.tempo){
					if(temp.ativo)
						menor = temp;
				}
			}
		}
		printf("Música com maior duração: %s\n", maior.nome_musica);
		printf("Música com menor duração: %s\n", menor.nome_musica);
	}
	else{
		printf("Nenhuma musica encontrada!\n");
	}
	fclose(arquivo);
	getch();
}
//editar músicas previamente cadastradas
int editar(){
	struct tm *data_hora_atual;     
  	time_t segundos;
  	time(&segundos);   
  	data_hora_atual = localtime(&segundos); 
	FILE *arquivo;
	int posicao=0, id_alterar, cont=0, editar;
	musica m;
	arquivo = fopen(NOME_ARQUIVO, "r+b");
	system("cls");
	visualizar_musicas();
	printf("\nDigite o ID da musica que deseja alterar: ");
	scanf("%d", &id_alterar);
	while(!feof(arquivo)){
		fread(&m, sizeof(musica), 1, arquivo);
		if(m.id == id_alterar){
			cont++;
			break;
		}
		else if(feof(arquivo)){
			break;
		}
		posicao++;
	}
	if (cont == 0){
		printf("ID NÃO ENCONTRADO!\n");
		getch();
		return 0;
	}
	system("cls");
	fseek(arquivo, sizeof(musica)*(posicao), SEEK_SET);
	if(ferror(arquivo))
		printf("Erro no posicionamento do cursor\n");
	else{
		fread(&m, sizeof(musica), 1, arquivo);
		if(ferror(arquivo))
			printf("Erro no posicionamento do cursor\n");
		else{
			do{
				system("cls");
				cabecalho();
				imprimir_formatado(m);
				printf("\nO que deseja alterar: \n"
			   "(0) SAIR DA ALTERACAO\n"
			   "(1) NOME DA MUSICA\n"
			   "(2) NOME DO ARTISTA\n"
			   "(3) ESTILO\n"
			   "(4) DURACAO\n"
			   "Escolha a sua opcao: ");
				scanf("%d", &editar);
				switch(editar){
					case 0:
						break;
					case 1:
						setbuf(stdin, NULL);
						printf("Nome da música: ");
						gets(m.nome_musica);
						break;
					case 2:
						setbuf(stdin, NULL);
						printf("Nome do artista/banda: ");
						gets(m.nome_artista);
						break;
					case 3:
						setbuf(stdin, NULL);
						printf("Estilo da música: ");
						gets(m.estilo);
						break;
					case 4:
						do{
							setbuf(stdin, NULL);
							printf("Duracao da música (segundos): ");
							scanf("%d", &m.tempo);
							if(m.tempo <= 0)
								printf("DIGITE UMA DURACAO VALIDA\n");
						}while(m.tempo <= 0);
						break;
					default:
						printf("DIGITE UMA OPCAO VALIDA\n");
						break;
				}
			}while(editar != 0);

		}
	}
	
	srand(time(NULL));
	m.dia = data_hora_atual->tm_mday;
	m.mes = data_hora_atual->tm_mon+1;
	m.ano = data_hora_atual->tm_year+1900;
	fseek(arquivo, sizeof(musica)*(posicao), SEEK_SET);
	fwrite(&m, sizeof(musica), 1, arquivo);
	fclose(arquivo);
	return 0;
}
//excluir registro de música
int excluir(){
	FILE *arquivo;
	int id_excluir, posicao = 0, cont = 0;
	musica m;
	char resp;
	system("cls");
	visualizar_musicas();
	printf("\nDigite o ID deseja excluir: ");
	scanf("%d", &id_excluir);
	arquivo = fopen(NOME_ARQUIVO, "r+b");
	while(!feof(arquivo)){
		fread(&m, sizeof(musica), 1, arquivo);
		if(m.id == id_excluir){
			cont++;
			break;
		}
		else if(feof(arquivo)){
			break;
		}
		posicao++;
	}
	if(cont == 0){
		printf("ID NAO ENCONTRADO!\n");
		getch();
		return 0;
	}
	system("cls");
	fseek(arquivo, sizeof(musica)*(posicao), SEEK_SET);
	if(ferror(arquivo)){
		printf("ERRO NO POSICIONAMENTO DO CURSOR\n");
		getch();
		return 0;
	}
	else{
		fread(&m, sizeof(musica), 1, arquivo);
		if(ferror(arquivo)){
			printf("ERRO NA LEITURA DO ARQUIVO\n");
			getch();
			return 0;
		}
		else{
			cabecalho();
			imprimir_formatado(m);
		}
	}
	do{
		setbuf(stdin, NULL);
		printf("Confirmar exclusao? (s/n): ");
		resp = tolower(getchar());
	}while(resp != 's' && resp != 'n');
	if(resp == 's'){
		fseek(arquivo, sizeof(musica)*(posicao), SEEK_SET);
		m.ativo = 0;
		fwrite(&m, sizeof(musica), 1, arquivo);
		if(ferror(arquivo))
			printf("ERRO NA EXLUSAO DO REGISTRO!\n");
		else
			printf("EXCLUSAO EFETUADA COM SUCESSO!\n");
	}
	else{
		printf("EXCLUSAO NAO EFETUADA\n");
	}
	fclose(arquivo);
	getch();
	return 0;
}
//exibição menu
int menu(){
	int opcao;
	system ("mode 150");
	do {
		system("cls");
		printf("--------------------------------------------------\n"
			   "                  AQUELEFY MUSIC                  \n"
			   "--------------------------------------------------\n"
		       "0 - Fechar o programa\n"
			   "1 - Cadastrar música\n"
			   "2 - Listar músicas cadastradas\n"
			   "3 - Pesquisar música\n"
			   "4 - Tempo total de músicas\n"
			   "5 - Exibir maior e menor música\n"
			   "6 - Editar musica\n"
			   "7 - Excluir musica\n"
			   "8 - Ordenar lista\n");
		printf("Opção: ");	
		scanf("%d", &opcao);
		switch(opcao){
		case 0:
			system("cls");
			printf("ENCERRANDO PROGRAMA...\n");
			Sleep(1500);
			break;
		case 1:
			cadastrar_musica();
			break;
		case 2:
			visualizar_musicas();
			getch();
			break;
		case 3:
			pesquisar_musica();
			break;
		case 4:
			tempo_total();
			break;
		case 5:
			maior_menor();
			break;
		case 6:
			editar();
			break;
		case 7:
			excluir();
			break;
		case 8:
			ordenar();
			break;
		default:
			system("cls");
			printf("OPÇÃO INVALIDA!\n");
			getch();
			break;
		}
		system("cls");   
	} while (opcao != 0);
	return 0;
}
//contar número de músicas cadastradas, usado para ordenação
int contador(){
	FILE *arquivo;
	int cont = 0;
	musica m;
	arquivo = fopen(NOME_ARQUIVO, "r+b");
	while(fread(&m, sizeof(musica), 1, arquivo) == 1)
	{
		if(m.ativo)
			cont++;
	}
	fclose(arquivo);
	return cont;
}
//coloca as músicas cadastradas em ordem crescente, de acordo o ID
int ordenar(){
	FILE *arquivo;
	musica m;
	int tamanho = contador(), i = 0, j;
	musica vetor[tamanho];
	system("cls");
	arquivo = fopen(NOME_ARQUIVO, "r+b");
	while(fread(&m, sizeof(musica), 1, arquivo) == 1){
		if(m.ativo){
			vetor[i] = m;
			i++;
		}
	}
	for(i = 1;  i< tamanho; i++){
		j = i;
		while(j > 0 && vetor[j-1].id > vetor[j].id){
			m = vetor[j];
			vetor[j] = vetor[j-1];
			vetor[j-1] = m;
			j--;
		}
	}
	fclose(arquivo);
	arquivo = fopen(NOME_ARQUIVO, "w+a");
	for(i = 0; i < tamanho; i++){
		fwrite(&vetor[i], sizeof(musica), 1, arquivo);
	}
	fclose(arquivo);
	return 0;
}

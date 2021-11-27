#include<stdio.h>
#include<string.h>
// --------------------- enum ------------------------------------
typedef enum status_do_cliente {INATIVO, ATIVO} status;
typedef enum genero_do_filme {AVENTURA, COMEDIA, DRAMA, TERROR, ACAO, ROMANCE} genero;
typedef enum classificacao_do_filme {LIVRE, MAISDEZ, DOZE, CATORZE, DEZESSEIS, DEZOITO} classif;
typedef enum meses {JAN=1, FEV, MAR, ABR, MAI, JUN, JUL, AGO, SET, OUT, NOV, DEZ} mes;
typedef enum tipo_plano {BASICO, PREMIUM} plano;

// --------------------- registros (struct) ----------------------
typedef struct s_cliente{
	
	int cpf;
	char nome_c[50];
	char email[50];
	char telefone[15];
	status status;	
} cliente;

typedef struct s_filme{
	
	int cod;	//autoincremento (comeca em 1001)
	char nome_f[50];
	genero genero;
	classif classificacao;	
} filme;

typedef struct plano_basico {
	
	int qtd_filme;
	float valor_base_b;
	float valor_excedente;	
} p_basico;

typedef struct plano_premium {
	
	float valor_base_p;	
} p_premium;

typedef struct pagam_credito {
	
	int num_cartao;	
} credito;

typedef struct pagam_debito {
	
	int agencia;
	int conta;	
} debito;

// --------------------- union -----------------------------------
typedef union tipo_pagamento {
	
	credito credito;
	debito debito;	
} pagamento;
// ----------------------------------------------------------------

typedef struct s_contrato {	// cada cliente pode ter apenas 1 contrato, portanto maximo de 10 contratos
	
	int cpf_cliente;
	plano tipo_plano;		//0 basico e 1 premium --> enum
	pagamento tipo_pagam; 	//0 debito e 1 credito --> uniao
	int dia_contrata;
	mes mes_contrata; 	
	int dia_cancela;
	mes mes_atual;		// inicializa com 1, em janeiro e só incrementa no gerar_fatura	
} contrato;

typedef struct historico_mensal {
	
	int cod_filme;
	int data;
} historico;

typedef struct filmes_assistidos {
	
	int cod;
	genero genero;
} filme_assistido;

// --------------------- prototipos ------------------------------
void cadastra_cliente(cliente *vet_cliente, int *cont);
void cadastra_filme(filme *vet_filme, int *pos_filme);
void cadastra_plano_basico(p_basico *plano_b);
void cadastra_plano_premium(p_premium *plano_p);
void cadastra_contrato(contrato *vet_contrato, int *pos_contrato, cliente *vet_cliente, int pos_cliente);
int carrega_filme(historico mat[][30], int t_max_filme, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme, contrato *vet_contrato, int pos_contrato, p_basico *plano_b);
int cancela_contrato(contrato *vet_contrato, int *pos_contrato, cliente *vet_cliente, int pos_cliente, p_basico *plano_b, p_premium *plano_p, historico mat[][30], int t_max_filme);
int gerar_fatura(contrato *vet_contrato, int *pos_contrato, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme, historico mat[][30], int tam_max, int t_max_filme, p_premium *plano_p, p_basico *plano_b);
void lista_dados_cliente(cliente *vet_cliente, int posicao);
void lista_historico_cliente(historico mat[][30], int t_max_filme, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme, contrato *vet_contrato);
void lista_cli_excede(cliente *vet_cliente, int pos_cliente, p_basico *plano_b, contrato *vet_contrato, int pos_contrato, historico mat[][30], int t_max_filme);
void frequencia_filme(filme *vet_filme, int pos_filme, historico mat[][30], int tam_max, int t_max_filme);
int recomenda_filme(historico mat[][30], int tam_max, int t_max_filme, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme);
					
void confere_tipo_plano(contrato *vet_contrato, int pos_contrato, int cont_filme, p_basico *plano_b, p_premium *plano_p);
void mostra_filmes_recomendados(filme *vet_filme, int pos_filme, filme_assistido *vet_filme_assistido, int pos_assistido, int maior_gen, int *vet_gen_igual, int pos_igual);

// ---------------------------------------------------------------
int main(){
	
	int opcao, i, j;
	const int tam_max = 10, t_max_filme = 30;
		
	cliente vet_cliente[tam_max];
	filme vet_filme[tam_max];
	p_basico plano_b;
	p_premium plano_p;
	contrato vet_contrato[tam_max];
	historico mat[tam_max][t_max_filme];
		
	int cont_cli=0, cont_filme=0, cont_contrato=0, ret=0;
	
	// inicializo na primeira posicao com 0 para verificar depois nas funcoes se já tem algum cadastro ou nao
	vet_cliente[0].cpf = 0;
	vet_contrato[0].cpf_cliente = 0;
	
	// inicializo o codigo do primeiro filme cadastrado com 1001 e depois vai incrementando
	vet_filme[0].cod = 1001;
	for(i=1; i<tam_max; i++){
		vet_filme[i].cod = vet_filme[i-1].cod + 1;
	}
	
	// inicializo matriz com 0 para verificar depois nas funções se ja tem cadastro ou não
	for(i=0; i<tam_max; i++){
		for(j=0; j<t_max_filme; j++){
			mat[i][j].cod_filme = 0;
		}
	}
	// inicializo o mes atual do contrato de todos os clientes em janeiro. (Isso incrementa só quando gerar a fatura de todos os clientes)
	for(i=0; i<tam_max; i++){
		vet_contrato[i].mes_atual = JAN;
	}
	
	do{
		// menu
		//printf("Digite sua opcao: ");
		scanf(" %d", &opcao);
		
		switch(opcao){
			
			case 0: {
				printf("Finalizando programa...");
				break;
			}
			case 1: {
				cadastra_cliente(vet_cliente, &cont_cli);
				break;
			}
			case 2: {
				cadastra_filme(vet_filme, &cont_filme);
				break;
			}
			case 3: {
				cadastra_plano_basico(&plano_b);
				break;
			}
			case 4: {
				cadastra_plano_premium(&plano_p);
				break;
			}
			case 5: {
				cadastra_contrato(vet_contrato, &cont_contrato, vet_cliente, cont_cli);
				break;
			}
			case 6: {
				ret = carrega_filme(mat, t_max_filme, vet_cliente, cont_cli, vet_filme, cont_filme, vet_contrato, cont_contrato, &plano_b);
				if(ret == 1){
					printf("Filme carregado com sucesso\n");
				}
				break;
			}
			case 7 : {
				ret = cancela_contrato(vet_contrato, &cont_contrato, vet_cliente, cont_cli, &plano_b, &plano_p, mat, t_max_filme);
				if(ret == 1){
					printf("Cancelamento feito com sucesso\n");
				}
				break;
			}
			case 8: {
				gerar_fatura(vet_contrato, &cont_contrato, vet_cliente, cont_cli, vet_filme, cont_filme, mat, tam_max, t_max_filme, &plano_p, &plano_b);
				break;
			}
			case 9: {
				lista_dados_cliente(vet_cliente, cont_cli);
				break;
			}
			case 10: {
				lista_historico_cliente(mat, t_max_filme, vet_cliente, cont_cli, vet_filme, cont_filme, vet_contrato);
				break;
			}
			case 11: {
				lista_cli_excede(vet_cliente, cont_cli, &plano_b, vet_contrato, cont_contrato, mat, t_max_filme);
				break;
			}
			case 12: {
				frequencia_filme(vet_filme, cont_filme, mat, tam_max, t_max_filme);
				break;
			}
			case 13: {
				recomenda_filme(mat, tam_max, t_max_filme, vet_cliente, cont_cli, vet_filme, cont_filme);
				break;
			}
			default: printf("ERRO: Opcao invalida\n");
		}
		
	}while(opcao != 0);	
}

// ---------------------- funcoes --------------------------------
void cadastra_cliente(cliente *vet_cliente, int *cont){
	
	int i, cpf, achei_cliente=0;
	
	if(*cont >= 10){
		printf("ERRO: Numero maximo de clientes no sistema atingido\n");
	}else{
		
		//printf("CPF: ");
		scanf(" %d", &cpf);
		
		//roda vetor para verificar se ja tem cliente cadastrado com esse cpf	
		for(i=0; i<*cont; i++){
			if(vet_cliente[i].cpf == cpf){
				achei_cliente++;
				break;
			}
		}
		
		if(achei_cliente != 0){
			printf("ERRO: CPF do cliente ja cadastrado\n");
		}else{
			
			vet_cliente[*cont].cpf = cpf;
			//printf("Nome: ");
			scanf(" %[^\n]%*c", vet_cliente[*cont].nome_c);	
			//printf("E-mail: ");
			scanf(" %s", vet_cliente[*cont].email);	
			//printf("Telefone: ");
			scanf(" %s", vet_cliente[*cont].telefone);
			
			vet_cliente[*cont].status = INATIVO; // declara cliente como inativo (0) --> sem cadastro, hora q cadastra 
			*cont = (*cont) + 1; // passa por parametro
			printf("Cliente cadastrado com sucesso\n");
		}	
	}	
}

void cadastra_filme(filme *vet_filme, int *pos_filme){
	
	int genero, classif;

	if(*pos_filme == 10){
		printf("ERRO: Numero maximo de filmes no sistema atingido\n");
	}else{
		
		//printf("Nome filme: ");
		scanf(" %[^\n]%*c", vet_filme[*pos_filme].nome_f);
		
		// verifica se genero do filme foi digitado certo
		do{
			//printf("Genero ");
			//printf("0: Aventura | 1: Comedia | 2: Drama | 3: Terror | 4: Acao | 5: Romance\n Opcao: ");
			scanf(" %u", &genero);
			
			if(genero < 0 || genero > 5){
				printf("ERRO: Genero invalido\n");
			}else{
				vet_filme[*pos_filme].genero = genero;
			}
			
		}while(genero < 0 || genero > 5);
		
		// verifica se a classificacao do filme foi digitada certa
		do{
			//printf("Classificacao ");
			//printf("0: Livre | 1: +10 | 2: +12 | 3: +14 | 4: +16 | 5: +18 \nOpcao: ");
			scanf(" %u", &classif);
			
			if(classif < 0 || classif > 5){
				printf("ERRO: Classificao invalida\n");
			}else{
				vet_filme[*pos_filme].classificacao = classif;
			}
			
		}while(classif < 0 || classif > 5);
			
		printf("Filme cadastrado com sucesso\n");
		*pos_filme = (*pos_filme) + 1;
	}
}

void cadastra_plano_basico(p_basico *plano_b){
	
	//printf("Quant max de filmes no mes: ");
	scanf(" %d", &plano_b->qtd_filme);	
	//printf("Valor Base: ");
	scanf(" %f", &plano_b->valor_base_b);	
	//printf("Valor excedente: ");
	scanf(" %f", &plano_b->valor_excedente);
	printf("Plano Basico cadastrado com sucesso\n");
}

void cadastra_plano_premium(p_premium *plano_p){
	
	//printf("Valor base: ");
	scanf(" %f", &plano_p->valor_base_p);
	printf("Plano Premium cadastrado com sucesso\n");
}

void cadastra_contrato(contrato *vet_contrato, int *pos_contrato, cliente *vet_cliente, int pos_cliente){
	
	int i=0, j, plano, tipo_pag, dia, cpf;
	int achei=0, ja_teve_contrato=0;

	if(*pos_contrato == 10){
		printf("ERRO: Numero maximo de contratos no sistema atingido\n");
	}else{
		
		if(vet_cliente[0].cpf == 0){	// inicializei no main com 0 para verificar aqui
			printf("ERRO: Nenhum cliente cadastrado no sistema\n");
		}else{
			
			//printf("CPF do cliente: ");
			scanf(" %d", &cpf);	
			
			for(i=0; i<pos_cliente; i++){		
				if(vet_cliente[i].cpf == cpf){	

					if(vet_cliente[i].status == ATIVO){		//se status = 1 = ATIVO, eh pq ja foi cadastrado um contrato para esse cliente
						printf("ERRO: Cliente com contrato existente\n");
						achei=2;
					}else{
						achei++;
						break;
					}
				}
			}
	
			if(achei == 0){
				printf("ERRO: Cliente nao cadastrado\n");
			}else if(achei == 1){
				
				// verifico se cliente com esse cpf ja teve cadastro antes. Faço isso pra quando cancela um contrato, aí atualiza os dados na mesma posicao.
				for(j=0; j<*pos_contrato; j++){
					if(vet_contrato[j].cpf_cliente == cpf){
						*pos_contrato = j;
						ja_teve_contrato++;
						break;
					}
				}
				
				if(ja_teve_contrato == 0){
					vet_contrato[*pos_contrato].cpf_cliente = cpf;
				}
								
				// verifica se usuario digita tipo de plano certo (0 e basico e 1 e premium)
				do{
					//printf("Tipo de Plano: ");
					scanf(" %d", &plano);
					
					if(plano < 0 || plano > 1){
						printf("ERRO: Tipo de plano invalido\n");
					}else{
						vet_contrato[*pos_contrato].tipo_plano = plano;
					}
					
				}while(plano < 0 || plano > 1);
				
				// verifica se usuario digita tipo de pagamento certo (0 e debito e 1 e credito)
				do{
					//printf("Tipo de Pagamento: ");
					scanf(" %d", &tipo_pag);
					
					if(tipo_pag < 0 || tipo_pag > 1){
						printf("ERRO: Tipo de pagamento invalido\n");
					}else{
						
						if(tipo_pag == 0){		// significa que usuario escolheu debito, pede agencia e conta
							//printf("Agencia: ");
							scanf(" %d", &vet_contrato[*pos_contrato].tipo_pagam.debito.agencia);	
							//printf("Conta: ");
							scanf(" %d", &vet_contrato[*pos_contrato].tipo_pagam.debito.conta);
							
						}else if(tipo_pag == 1){ 	// significa que usuario escolheu crédito, pede num da conta					
							//printf("Num da Conta: ");
							scanf(" %d", &vet_contrato[*pos_contrato].tipo_pagam.credito.num_cartao);
						}
					}
					
				}while(tipo_pag < 0 || tipo_pag > 1);
				
				// verifica se usuario digita dia do contrato certo (entre 0 e 31)
				do{
					//printf("Dia da contratacao: ");
					scanf(" %d", &dia);
					
					if(dia < 0 || dia > 31){
						printf("ERRO: Dia invalido\n");
					}else{
						vet_contrato[*pos_contrato].dia_contrata = dia;
					}				
				}while(dia < 0 || dia > 31);
				
				vet_contrato[*pos_contrato].mes_contrata = vet_contrato[0].mes_atual;
				vet_contrato[*pos_contrato].dia_cancela = 0;
				vet_cliente[i].status = ATIVO;
				
				*pos_contrato = (*pos_contrato) + 1;	
				printf("Contrato cadastrado com sucesso\n");
			} 
		} // fim algum cliente ja cadastrado no sistema
	} // fim max de contratos cadastrados
	//return 1;	
}

int carrega_filme(historico mat[][30], int t_max_filme, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme, contrato *vet_contrato, int pos_contrato, p_basico *plano_b){
	
	int i, cpf, dia, opcao_cliente, genero, classificacao, cod_filme, ind_cliente, plano, taxa_extra;
	int achei_cpf=0, achei_cod=0, quant_filme=0;
	
	if(vet_cliente[0].cpf == 0){	//inicializei no main com 0 para verificar aqui	
		printf("ERRO: Nenhum cliente cadastrado no sistema\n");
		return 0;
	}else{	
		//printf("CPF do cliente: ");
		scanf(" %d", &cpf);	
		// verifica se o cpf digitado ta cadastrado e se o status desse cliente ta ativo ou nao(se tem contrato ou nao)
		for(i=0; i<pos_cliente; i++){
				
			if(vet_cliente[i].cpf == cpf){		
				if(vet_cliente[i].status == INATIVO){	//significa q nenhum contrato foi cadastrado para esse cliente, entao nao pode cadastrar e sai 
					printf("ERRO: Cliente nao ativo\n");
					return 0;	
				}else{		// fala q achou o cpf digitado do cliente e pega o indice q esse cliente ta cadastrado no vetor	
					achei_cpf++;
					ind_cliente = i;	// vou cadastrar o filme e data do cliente na matriz na mesma posicao q esse cliente está no vetor
					break;
				}	
			}
		}
		
		if(achei_cpf == 0){
			printf("ERRO: Cliente nao cadastrado\n");
			return 0;		
		}else{
			
			// verifica se esse cliente ja assistiu o maximo de 30 filmes 
			for(i=0; i<t_max_filme; i++){
				if(mat[ind_cliente][i].cod_filme != 0){	// se não for igual a zero é pq tem codigo de algum filme que já foi carregado 
					quant_filme++;
				}
			}
			
			if(quant_filme >= 30){
				printf("ERRO: Numero maximo de filmes no sistema atingido\n");
				return 0;
			}else{
				
				// verifica se o dia digitado eh valido ate digitar certo
				do{
					//printf("Dia do carregamento do filme: ");
					scanf(" %d", &dia);
					
					if(dia < 0 || dia > 31){
						printf("ERRO: Dia invalido\n");
					}	
				}while(dia < 0 || dia > 31);
				
				// verifica se a opcao digitada eh valida ate digitar certo
				do{
					//printf("Deseja ver os filmes listados por genero (0) ou classificacao (1)? ");
					scanf(" %d", &opcao_cliente);
					
					if(opcao_cliente < 0 || opcao_cliente > 1){
						printf("ERRO: Escolha invalida\n");
					}	
				}while(opcao_cliente < 0 || opcao_cliente > 1);
				
				//mostra os filmes por genero ou classificacao q o usuario digitar
				if(opcao_cliente == 0){

					do{
						//printf("Qual genero: 0: Aventura | 1: Comedia | 2: Drama | 3: Terror | 4: Acao | 5: Romance\n Opcao: ");
						scanf(" %d", &genero);
						
						if(genero < 0 || genero > 5){
							printf("ERRO: Escolha invalida\n");
						}
						
					}while(genero < 0 || genero > 5);
					
					for(i=0; i<pos_filme; i++){
						if(vet_filme[i].genero == genero){	
							printf("Codigo: %d\n", vet_filme[i].cod);
							printf("Nome: %s\n", vet_filme[i].nome_f);
						}
					}
					
				}else if(opcao_cliente == 1){

					do{
						//printf("qual classificacao: 0: Livre | 1: +10 | 2: +12 | 3: +14 | 4: +16 | 5: +18 \nOpcao: ");
						scanf(" %d", &classificacao);
						
						if(classificacao < 0 || classificacao > 5){
							printf("ERRO: Escolha invalida\n");
						}
						
					}while(classificacao < 0 || classificacao > 5);
					
					for(i=0; i<pos_filme; i++){
						if(vet_filme[i].classificacao == classificacao){
							printf("Codigo: %d\n", vet_filme[i].cod);
							printf("Nome: %s\n", vet_filme[i].nome_f);
						}
					}							
				}
				
				// depois q usuario viu lista, pede o cod do filme que ele quer carregar 
				//printf("Codigo do filme que deseja: ");
				scanf(" %d", &cod_filme);
				
				// percorre vetor de filmes para verificar se o cod do filme escolhido eh valido senao sai
				for(i=0; i<pos_filme; i++){
					if(vet_filme[i].cod == cod_filme){
						achei_cod++;	
					}
				}
				
				if(achei_cod == 0){
					printf("ERRO: Filme nao encontrado\n");
					return 0;
				}else{
					
					//verifica o plano do cliente que possui o cpf que foi digitado para saber se pode ser carregado o filme
					//percorre vetor contrato para saber o plano do cliente do cpf digitado
					for(i=0; i<pos_contrato; i++){
						if(vet_contrato[i].cpf_cliente == cpf){
							plano = vet_contrato[i].tipo_plano;		// plano recebe 0 (plano basico) ou 1 (plano prmeium) - eh uma enumeracao
							break;
						}
					}
					
					// verifica qual o plano desse cliente
					if(plano == BASICO){
						
						if(quant_filme >= plano_b->qtd_filme){
							
							do{
								//printf("0: Nao | 1: Sim - Opcao: ");
								scanf(" %d", &taxa_extra);
								
								if(taxa_extra < 0 || taxa_extra > 1){
									printf("ERRO: Escolha invalida\n");
								}
							}while(taxa_extra < 0 || taxa_extra > 1);
							
							if(taxa_extra == 0){
								printf("ERRO: Taxa adicional negada\n");
								return 0;
							}else{
								
								// percorre vetor ate achar posicao q nao tem nada cadastrado para poder cadastrar
								for(i=0; i<t_max_filme; i++){	
									if(mat[ind_cliente][i].cod_filme == 0 && quant_filme <= 30){
										
										mat[ind_cliente][i].cod_filme = cod_filme;
										mat[ind_cliente][i].data = dia;
										break;
									}
								}
							}
							
						}else if(quant_filme <= 30){
							
							// percorre vetor de novo ate achar posicao q nao tem nada cadastrado para poder cadastrar
							for(i=0; i<t_max_filme; i++){	
								if(mat[ind_cliente][i].cod_filme == 0){
									
									mat[ind_cliente][i].cod_filme = cod_filme;
									mat[ind_cliente][i].data = dia;
									break;
								}
							}
							
						}
						
					}else if(plano == PREMIUM){
						
						// percorre vetor de novo ate achar posicao q nao tem nada cadastrado para poder cadastrar
						for(i=0; i<t_max_filme; i++){
							if(mat[ind_cliente][i].cod_filme == 0){
								
								mat[ind_cliente][i].cod_filme = cod_filme;
								mat[ind_cliente][i].data = dia;
								break;
							}
						}	
						
					} // se for plano premium	
				} // se cod_filme digitado for valido
				
			} // fim se assistiu mais de 30 filmes já	
		} // se for cliente valido
	} // se tiver algum cliente cadastrado no sistema	
	return 1;
}

int cancela_contrato(contrato *vet_contrato, int *pos_contrato, cliente *vet_cliente, int pos_cliente, p_basico *plano_b, p_premium *plano_p, historico mat[][30], int t_max_filme){
	
	int i, j, k, cpf, achei_cli=0, dia;
	int cont_filme=0;
	
	if(vet_contrato[0].cpf_cliente == 0){
		printf("ERRO: Nenhum contrato cadastrado no sistema\n");
		return 0;
	}else{
		
		//printf("Cpf do cliente que deseja cancelar o contrato: ");
		scanf(" %d", &cpf);
		
		// verificar se cpf é valido e se o cliente ta ativo
		for(i=0; i<pos_cliente; i++){
			if(vet_cliente[i].cpf == cpf){
				achei_cli++;
				if(vet_cliente[i].status == INATIVO){
					printf("ERRO: Cliente inativo\n");
					return 0;
				}
				break;
			}
		} // i sai valendo a posicao do cliente na matriz tambem
		
		if(achei_cli == 0){
			printf("ERRO: Cliente nao cadastrado\n");
			return 0;
		}else{
			
			do{
				//printf("Dia do cancelamento: ");
				scanf(" %d", &dia);
				
				if(dia < 0 || dia > 31){
					printf("ERRO: Dia invalido\n");
				}else{
					// verifica se o dia que contratou não ta antes que o dia que quer cancelar, se o contrato foi feito no mesmo mes que esta rodando o sistema 
					for(j=0; j<*pos_contrato; j++){
						if(vet_contrato[j].cpf_cliente == cpf){
							
							if(vet_contrato[j].mes_contrata == vet_contrato[0].mes_atual){	
								if(vet_contrato[j].dia_contrata > dia){
									printf("ERRO: Data de cancelamente anteiror a data de contratacao\n");
									return 0;
								}	
							}							
							break;
						}
					} // j sai valendo a posicao do contrato que esta analisando para saber o plano desse cliente em seguida
				}
				
			}while(dia < 0 || dia > 31);
			
			// verificar quantos filmes cliente assistiu para passar na funcao que confere o plano
			for(k=0; k<t_max_filme; k++){
				if(mat[i][k].cod_filme != 0){
					cont_filme++;
				}else{
					break;
				}
			}
			
			confere_tipo_plano(vet_contrato, j, cont_filme, plano_b, plano_p); //funcao que confere o tipo do plano do cliente e mostra o valor devido
					
			vet_contrato[j].dia_cancela = dia;
			vet_cliente[i].status = INATIVO;		
		}
	}
	return 1;
}

int gerar_fatura(contrato *vet_contrato, int *pos_contrato, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme, historico mat[][30], int tam_max, int t_max_filme, p_premium *plano_p, p_basico *plano_b){
	
	int i, j, k, opcao, cpf, achei_cli=0, cont_filme_b=0;
	mes mes;
	
	// usa o mes_atual para mostrar na data de carregamento do filme, pois quando incrementa ele, o historico de todos os clientes é zerado
	// por isso a data de carregamento de um filme de qualquer cliente sempre vai ser no mes atual que estiver rodando o sistema no momento
	mes = vet_contrato[0].mes_atual;
		
	do{
		//printf("0: fatura especifica | 1: fatura de todos os clientes - Opcao: ");
		scanf(" %d", &opcao);
		
		if(opcao < 0 || opcao > 1){
			printf("ERRO: Escolha invalida\n");
		}
	}while(opcao < 0 || opcao > 1);
		
	if(opcao == 0){
			
		if(vet_cliente[0].cpf == 0){
			printf("ERRO: Nenhum cliente cadastrado no sistema\n");
			return 0;
		}else{
		
			//printf("CPF do cliente que deseja gerar a fatura: ");
			scanf(" %d", &cpf);
			
			for(i=0; i<pos_cliente; i++){
				if(vet_cliente[i].cpf == cpf){
					achei_cli++;		
					if(vet_cliente[i].status == INATIVO){
						printf("ERRO: Cliente nao possui contrato\n");
						return 0;
					}
					break;
				}
			} // i sai valendo a posicao do cliente com o cpf digitado (reutiliza na matriz que vai ser a mesma posicao)
			
			if(achei_cli == 0){
				printf("ERRO: Cliente nao cadastrado\n");
				return 0;
			}else{
				
				for(j=0; j<t_max_filme; j++){
					for(k=0; k<pos_filme; k++){
						
						if(mat[i][j].cod_filme == vet_filme[k].cod){
							printf("Nome: %s\n", vet_filme[k].nome_f); //nome do filme
							printf("Data: %d/%d\n", mat[i][j].data, mes); //data do carregamento
							
							cont_filme_b++;
						}
					}
				}
				
				//percorre vetor contrato,para pegar posicao do contrato para passar na chamada da funcao que confere o plano
				for(j=0; j<*pos_contrato; j++){
					if(vet_cliente[i].cpf == vet_contrato[j].cpf_cliente){
						break;
					}
				} // j sai valendo a posicao do contrato que esta analisando para saber o plano desse cliente
				
				confere_tipo_plano(vet_contrato, j, cont_filme_b, plano_b, plano_p); //funcao que confere o tipo do plano do cliente e mostra o valor devido				
			}
		}
			
	}else if(opcao == 1){

		// mostra cpf, nome e valor devido de cada cliente
		for(i=0; i<pos_cliente; i++){		// i = posicao do cliente no vetor cliente e na matriz
			if(vet_cliente[i].status == ATIVO){
					
				printf("CPF: %d\n", vet_cliente[i].cpf);
				printf("Nome: %s\n", vet_cliente[i].nome_c); //nome do cliente
	
				// pega o plano de cada cliente para conferir o valor devido
				
				// roda matriz historico pra ver quantos filmes ele assistiu e fazer as contas
				for(j=0; j<t_max_filme; j++){	// j = posicao dos filmes na matriz
					if(mat[i][j].cod_filme != 0){
						cont_filme_b++;
					}
				}
				
				for(j=0; j<*pos_contrato; j++){	
					if(vet_cliente[i].cpf == vet_contrato[j].cpf_cliente){	
						break;
					}
				} // j sai valendo a posicao do contrato que esta analisando para saber o plano desse cliente
				
				confere_tipo_plano(vet_contrato, j, cont_filme_b, plano_b, plano_p);
			}	
		}
		//depois que mostrar de todos os clientes incrementa o mes e zera o historico de todos os clientes
		
		// zera a matriz historico para todos os clientes cadastrados
		for(i=0; i<pos_cliente; i++){
			for(j=0; j<t_max_filme; j++){
				mat[i][j].cod_filme = 0;
				mat[i][j].data = 0;
			}
		}
		// incrementa o mes
		for(i=0; i<tam_max; i++){
			vet_contrato[i].mes_atual = mes + 1;
		}
		
		printf("Mes vigente apos a fatura: %d\n", vet_contrato[0].mes_atual);
	
	} // fatura de todos os clientes	
	return 1;
}

void confere_tipo_plano(contrato *vet_contrato, int pos_contrato, int cont_filme, p_basico *plano_b, p_premium *plano_p){
	
	int qtd_filme;
	float valor_extra, valor_final;
	
	if(vet_contrato[pos_contrato].tipo_plano == PREMIUM){
		
		printf("Valor devido: %.2f\n", plano_p->valor_base_p);	
			
	}else if(vet_contrato[pos_contrato].tipo_plano == BASICO){

		if(cont_filme > plano_b->qtd_filme){
				
			qtd_filme = cont_filme - plano_b->qtd_filme;
			valor_extra = qtd_filme * plano_b->valor_excedente;
			
			valor_final = plano_b->valor_base_b + valor_extra;
			printf("Valor devido: %.2f\n", valor_final);
			
		}else{
			printf("Valor devido: %.2f\n", plano_b->valor_base_b);
		}	
	}
}

void lista_dados_cliente(cliente *vet_cliente, int posicao){
	
	int i, cpf, achei=0;
	
	if(vet_cliente[0].cpf == 0){	// inicializei no main com 0 para verificar aqui
		printf("ERRO: Nenhum cliente cadastrado no sistema\n");
	}else{
	
		//printf("CPF do cliente que quer listar: ");
		scanf(" %d", &cpf);
		
		//verifica se tem esse cpf cadastrado no sistema
		for(i=0; i<posicao; i++){	
			if(vet_cliente[i].cpf == cpf){
				achei++;
				break;
			}
		}
		
		if(achei == 0){
			printf("ERRO: Cliente nao cadastrado\n");
		}else{
			printf("Nome: %s\n", vet_cliente[i].nome_c);
			printf("Email: %s\n", vet_cliente[i].email);
			printf("Telefone: %s\n", vet_cliente[i].telefone);
			printf("Status: %d\n", vet_cliente[i].status);
		}
	}
}

void lista_historico_cliente(historico mat[][30], int t_max_filme, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme, contrato *vet_contrato){
	
	int i, j, k, cpf, achei=0;
	
	if(vet_cliente[0].cpf == 0){ // inicializei no main com 0 para verificar aqui
		printf("ERRO: Nenhum cliente cadastrado no sistema\n");
	}else{
		
		//printf("CPF do cliente que quer ver o historico: ");
		scanf(" %d", &cpf);
		
		//verifica se tem cpf digitado cadastrado correspondente a algum cliente
		for(i=0; i<pos_cliente; i++){
			if(vet_cliente[i].cpf == cpf){
				achei++;
				break;
			}
		}
		// i sai na posicao do cliente no vetor que eh a mesma posicao na matriz
		
		if(achei == 0){
			printf("ERRO: Cliente nao cadastrado\n");
		}else{
			
			if(vet_cliente[i].status == ATIVO){
				printf("Estado: Ativo\n");
			}else if(vet_cliente[i].status == INATIVO){
				printf("Estado: Inativo\n");
			}
				
			// cadastrei a posicao do cliente na matriz igual a posicao do cliente no vetor, por isso uso o i q o for parou, pois é a posicao do cliente na matriz
			// verifica se tem algum filme q o cliente carregou
			if(mat[i][0].cod_filme == 0){
				printf("ERRO: Nenhum filme assistido\n");
			}else{
				
				for(j=0; j<t_max_filme; j++){
					
					if(mat[i][j].cod_filme != 0){
						
						printf("Codigo: %d\n", mat[i][j].cod_filme);
						
						for(k=0; k<pos_filme; k++){
							if(mat[i][j].cod_filme == vet_filme[k].cod){
								
								printf("Nome: %s\n", vet_filme[k].nome_f); 
								
								switch(vet_filme[k].genero){
									case AVENTURA: {
										printf("Genero: aventura\n");
										break;
									} 
									case COMEDIA: {
										printf("Genero: comedia\n");
										break;
									}
									case DRAMA: {
										printf("Genero: drama\n");
										break;
									}
									case TERROR: {
										printf("Genero: terror\n");
										break;
									}
									case ACAO: {
										printf("Genero: acao\n");
										break;
									}
									case ROMANCE: {
										printf("Genero: romance\n");
										break;
									}
								}
								
								switch(vet_filme[k].classificacao){
									
									case LIVRE: {
										printf("Classificacao: livre\n");
										break;
									}
									case MAISDEZ: {
										printf("Classificacao: +10\n");
										break;
									}
									case DOZE: {
										printf("Classificacao: +12\n");
										break;
									}
									case CATORZE: {
										printf("Classificacao: +14\n");
										break;
									}
									case DEZESSEIS: {
										printf("Classificacao: +16\n");
										break;
									}
									case DEZOITO: {
										printf("Classificacao: +18\n");
										break;
									}
								}
 
							} 
						} // fim for vetor filme
						
						//mes eh igual pra todo mundo no carregamento pois so incrementa ele quando gera a fatura de todos e zera o historico de todos clientes
						// por isso eh so pegar o mes cadastrado no contrato na posicao 0 
						printf("Data do carregamento: %d/%d\n\n", mat[i][j].data, vet_contrato[0].mes_atual);
	
					}else{
						break;
					}	
				} // fim for j posicao_filme

			} // fim cliente assistiu algum filme	
		} // fim cpf do cliente valido
	} // fim algum cliente cadastrado
}

void lista_cli_excede(cliente *vet_cliente, int pos_cliente, p_basico *plano_b, contrato *vet_contrato, int pos_contrato, historico mat[][30], int t_max_filme){
	
	int i, j, cli_nao_excede=0, qtd_planos_basicos=0, cont_filme=0, qtd_filme;
	float valor_extra;
	
	for(i=0; i<pos_cliente; i++){
		if(vet_cliente[i].status == ATIVO){
			
			for(j=0; j<pos_contrato; j++){
				if(vet_cliente[i].cpf == vet_contrato[j].cpf_cliente){
					
					if(vet_contrato[j].tipo_plano == BASICO){
						
						qtd_planos_basicos++;
						for(j=0; j<t_max_filme; j++){	
							if(mat[i][j].cod_filme != 0){
								cont_filme++;
							}
						}
						
						if(cont_filme > plano_b->qtd_filme){
							
							qtd_filme = cont_filme - plano_b->qtd_filme;
							valor_extra = qtd_filme * plano_b->valor_excedente;

							printf("CPF: %d\n", vet_cliente[i].cpf);
							printf("Nome: %s\n",vet_cliente[i].nome_c);
							printf("Valor excedente: %.2f\n\n", valor_extra);
	
						}else{
							cli_nao_excede++;
						}
					}
					
				} // fim condicao para achar o contrato do cliente em questao
			} // fim for contrato
						
		} // fim condicao cliente ativo
	} // fim for cliente
	
	if(cli_nao_excede == qtd_planos_basicos){
		printf("ERRO: Nenhum cliente excedente\n");
	}
}

void frequencia_filme(filme *vet_filme, int pos_filme, historico mat[][30], int tam_max, int t_max_filme){
	
	int i, j, cod_filme, achei=0, filmes_carregados=0, filme_escolhido=0;
	float aux, porcentagem;
	
	if(pos_filme == 0){
		printf("ERRO: Nenhum filme cadastrado no sistema\n");
	}else{
		
		//printf("Cod Filme: ");
		scanf(" %d", &cod_filme);
		
		if(mat[0][0].cod_filme == 0){
			printf("ERRO: Nenhum filme assistido\n");
		}else{
			
			// roda vetor para saber se cod digitado eh valido
			for(i=0; i<pos_filme; i++){
				if(vet_filme[i].cod == cod_filme){
					achei++;
					break;
				}
			}
			
			if(!achei){
				printf("ERRO: Codigo invalido\n");
			}else{		
				
				// roda matriz em todas as posicoes para pegar a quant de filmes totais carregados no sistema
				for(i=0; i<tam_max; i++){
					for(j=0; j<t_max_filme; j++){
						
						if(mat[i][j].cod_filme != 0){
							filmes_carregados++;
							
							// conta quantos vezes o filme que tem o cod digitado foi assistido
							if(cod_filme == mat[i][j].cod_filme){
								filme_escolhido++;
							}
						}
						
					}
				}
				
				aux = (float)filme_escolhido * 100;
				porcentagem = aux/(float)filmes_carregados;
				printf("Frequencia: %.2f%%\n", porcentagem);
				
			} // fim condicao cod filme valido
		} // fim condicao algum filme assistido
	}
}

int recomenda_filme(historico mat[][30], int tam_max, int t_max_filme, cliente *vet_cliente, int pos_cliente, filme *vet_filme, int pos_filme){
	
	int i, j, k, cpf, achei_cli=0;
	
	// vetor que armazena todos os filmes assistidos pelo cliente
	filme_assistido vet_filme_assistido[t_max_filme];
	// contador que vai incrementando o maximo de posicoes cadastradas no vetor
	int pos_assistido=0;
	
	// variaveis que uso no for para armazenar o genero mais assistido pelo cliente
	int maior=0, maior_gen, cont=1, aux;
	
	// vetor que armazena os generos que podem empatam (maximo de 6 posicoes pq é o maximo de generos tem)
	int vet_gen_igual[6], igual=0;
	
	// verifica se tem algum cliente cadastrado
	if(pos_cliente == 0){
		printf("ERRO: Nenhum cliente cadastrado no sistema\n");
		
	}else{ // se não tiver sai
		
		//printf("Cpf cliente: ");
		scanf(" %d", &cpf);
		
		// for que verifica se o cpf digitado existe cadastrado no sistema e so o status desse cliente é ativo
		for(i=0; i<pos_cliente; i++){
			
			if(vet_cliente[i].cpf == cpf){
				achei_cli++;	
				if(vet_cliente[i].status == INATIVO){
					printf("ERRO: Cliente nao ativo\n");
					return 0;
				}
				break; // o break faz o ir sair valendo a posicao do cliente (que é a mesma posicao na matriz pra mim)
			}
		} // i sai valendo a posicao do cliente na matriz
		
		// sverifica se achou o cpf cadastrado
		if(!achei_cli){
			printf("ERRO: Cliente nao cadastrado\n");
			
		}else{ // se não achou sai 
		
			
			// verifica se cliente nessa posicao i assistiu algum filme
			if(mat[i][0].cod_filme == 0){
				printf("ERRO: Nenhum filme assistido\n");
				
			}else{ // senão sai
				
				// roda todos os filmes que o cliente assistiu e salva no vetor o codigo e o genero
				for(j=0; j<t_max_filme; j++){
					if(mat[i][j].cod_filme != 0){ // se nao for igual a zero é pq tem cod de filme cadastrado, então aramzena no vetor
						
						for(k=0; k<pos_filme; k++){ // roda esse for pra pegar o genero do codigo desse filme no vetor de filmes
							if(vet_filme[k].cod == mat[i][j].cod_filme){
								
								vet_filme_assistido[pos_assistido].cod = vet_filme[k].cod;
								vet_filme_assistido[pos_assistido].genero = vet_filme[k].genero;
								pos_assistido++;	
							}
						}
						
					}else{
						break;
					}
				}
				
				aux = pos_assistido-1;
				// roda o vetor com cod e genero dos filmes assistidos para saber qual valor mais aparece
				
				for(i=0; i<pos_assistido; i++){	 // for com a posicao atual do genero que ta analisando
				
					for(j=1; j<aux; j++){ // for rodando todas as outras posicoes que tem no vetor
					
						// isso conta quantas vezes aparece cada genero no vetor dos filmes assistidos pelo cliente
						if(vet_filme_assistido[i].genero == vet_filme_assistido[j].genero){ // se o gen (i) == gen em outra posicao do vetor ele conta
							cont++;
						}	
					}

					if(maior <= cont){ // var maior comeca com 0, vai aramzenando toda vez que o cont for maior, ou seja, armazena quantas vezes tem aquele genero
						
						if(maior == cont){ 	// pro caso de empate, se maior == cont, é pq tem outro genero que apareceu a mesma quant de vezes, entao armazena no vet
							vet_gen_igual[igual] = maior_gen;
							igual++; // incrementa a posicao desse vetor de generos iguais
						}
						
						maior = cont;  // se nao for igual ele só armazena na variavel maior o contador e armazena na var maior_genero o genero mais assistido
						maior_gen = vet_filme_assistido[i].genero;	
					}
					
					cont=1;
					aux = aux - 1;
				}
				// depois de rodar todos os generos armazenados chama essa funçao para mostrar 
				mostra_filmes_recomendados(vet_filme, pos_filme, vet_filme_assistido, pos_assistido, maior_gen, vet_gen_igual, igual);
			
			} // fim condicao cliente assistiu algum filme		
		} // fim condicao cpf valido	
	} // fim condicao algum cliente cadastrado
	return 1;
}

void mostra_filmes_recomendados(filme *vet_filme, int pos_filme, filme_assistido *vet_filme_assistido, int pos_assistido, int maior_gen, int *vet_gen_igual, int pos_igual){
	
	int i, j, k, nao_mostra, cont_filme=0;
	int cont_filme_igual=0;

	for(j=0; j<pos_igual; j++){   // ta rodando vetor dos generos que deu empate
		for(k=0; k<pos_filme; k++){		// roda o vetor de filmes para saber quais os outros filmes cadastrados do mesmo genero que é pra mostrar
			
			nao_mostra=0;
			if(vet_filme[k].genero == vet_gen_igual[j]){  //se o gen do filme q tá no vetor com todos os filmes cadastrados for igual ao gen do vetor gen igual
															// eu verifico se vai mostrar ou nao
				
				for(i=0; i<pos_assistido; i++){		// rodo o vetor com todos os filmes assistidos pelo cliente comparando se tem o cod nele
				// se o codigo desse filme que tem o genero mais assistido for igual a algum cod ja assistido pelo cliente nao mostra
					if(vet_filme[k].cod == vet_filme_assistido[i].cod){ 	
						nao_mostra++;
					}
				}
				
				if(nao_mostra == 0){
				
					cont_filme_igual++;
					
					printf("Codigo: %d\n",vet_filme[k].cod);
					printf("Nome: %s\n",vet_filme[k].nome_f);
					
					switch(vet_filme[k].genero){
						
						case 0: {
							printf("Genero: aventura\n");
							break;
						}
						case 1: {
							printf("Genero: comedia\n");
							break;
						}
						case 2: {
							printf("Genero: drama\n");
							break;
						}
						case 3: {
							printf("Genero: terror\n");
							break;
						}
						case 4: {
							printf("Genero: acao\n");
							break;
						}
						case 5: {
							printf("Genero: romance\n");
							break;
						}
					}
					
					switch(vet_filme[k].classificacao){
						
						case LIVRE: {
							printf("Classificacao: livre\n");
							break;
						}
						case MAISDEZ: {
							printf("Classificacao: +10\n");
							break;
						}
						case DOZE: {
							printf("Classificacao: +12\n");
							break;
						}
						case CATORZE: {
							printf("Classificacao: +14\n");
							break;
						}
						case DEZESSEIS: {
							printf("Classificacao: +16\n");
							break;
						}
						case DEZOITO: {
							printf("Classificacao: +18\n");
							break;
						}
					}
					
				} // fim se	é pra mostrar
				
			} // fim se genero igual
		} // fim for
	} //fim for
	
	// percorre o vetor de filmes cadastrados com o genero q mais aparece e mostra os filmes que o cliente ainda nao assistiu
	for(k=0; k<pos_filme; k++){
		
		nao_mostra=0;
		if(vet_filme[k].genero == maior_gen){
			
			for(i=0; i<pos_assistido; i++){
				if(vet_filme_assistido[i].genero == maior_gen && vet_filme[k].cod == vet_filme_assistido[i].cod){
					nao_mostra++;
				}
			}
				
			if(nao_mostra == 0){

				cont_filme++;
				
				printf("Codigo: %d\n",vet_filme[k].cod);
				printf("Nome: %s\n",vet_filme[k].nome_f);
				
				switch(vet_filme[k].genero){
					
					case 0: {
						printf("Genero: aventura\n");
						break;
					}
					case 1: {
						printf("Genero: comedia\n");
						break;
					}
					case 2: {
						printf("Genero: drama\n");
						break;
					}
					case 3: {
						printf("Genero: terror\n");
						break;
					}
					case 4: {
						printf("Genero: acao\n");
						break;
					}
					case 5: {
						printf("Genero: romance\n");
						break;
					}
				}
				
				switch(vet_filme[k].classificacao){
					
					case LIVRE: {
						printf("Classificacao: livre\n");
						break;
					}
					case MAISDEZ: {
						printf("Classificacao: +10\n");
						break;
					}
					case DOZE: {
						printf("Classificacao: +12\n");
						break;
					}
					case CATORZE: {
						printf("Classificacao: +14\n");
						break;
					}
					case DEZESSEIS: {
						printf("Classificacao: +16\n");
						break;
					}
					case DEZOITO: {
						printf("Classificacao: +18\n");
						break;
					}
				}	
			}
		} // fim se genero_filme = maior genero
	} // fim for filme
	
	if(pos_igual > 0){
		if(cont_filme == 0 && cont_filme_igual == 0){
			printf("ERRO: Todos os filmes do(s) genero(s) foram assistidos\n");
		}else if(cont_filme_igual == 0){
			printf("ERRO: Todos os filmes do(s) genero(s) foram assistidos\n");
		}
	}else{
		if(cont_filme == 0){
			printf("ERRO: Todos os filmes do(s) genero(s) foram assistidos\n");
		}
	}
}


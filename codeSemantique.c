#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

FILE* file;
char Car_Cour;
char *pchar,*pchar_temp, chaine[20],chainePR[20];
int  CUR_POS_Y=1, CUR_POS_X=0, NbrId=0;
int CMPT1=0,CMPT2=0,AD;





typedef enum {
	PROGRAM_TOKEN, NUM_TOKEN,CONST_TOKEN, VAR_TOKEN,BEGIN_TOKEN, END_TOKEN,IF_TOKEN, THEN_TOKEN,WHILE_TOKEN, DO_TOKEN,READ_TOKEN, WRITE_TOKEN,PV_TOKEN, PT_TOKEN,
  PLUS_TOKEN, MOINS_TOKEN,MULT_TOKEN, DIV_TOKEN,VIR_TOKEN, AFF_TOKEN, INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN,DIFF_TOKEN, PO_TOKEN,PF_TOKEN, FIN_TOKEN,ID_TOKEN, ERREUR_TOKEN,
  EOF_TOKEN, EG_TOKEN,COM_TOKEN,ELSE_TOKEN,REPEAT_TOKEN,UNTIL_TOKEN,FOR_TOKEN,CASE_TOKEN,INTO_TOKEN,DOWNTO_TOKEN, DP_TOKEN,OF_TOKEN, ACC_TOKEN
  } CODES_TOKENS;
  
CODES_TOKENS Sym_Cour, OP; // OP pour memoriser les operations dans la partie semantique



typedef enum{
	ERR_CAR_INC, ERR_FIC_VID, ERR_PROGRAM, ERR_ID, ERR_PV, ERR_PT, ERR_EGAL, ERR_NUM, ERR_CONST, ERR_VAR_BEGIN, ERR_CONST_VAR_BEGIN, ERR_BEGIN,
	ERR_END, ERR_AFF, ERR_IF, ERR_WHILE, ERR_DO, ERR_WRITE,  ERR_READ, ERR_THEN, ERR_PO, ERR_PF,ERR_MOINS, ERR_PLUS, ERR_MULT, ERR_DIV,
	ERR_COND, ERR_TERM, ERR_FACT, ERR_INF, ERR_SUP, ERR_SUPEG, ERR_INFEG, ERR_DIFF, ERR_ACC, ERR_EXPR,ERR_ELSE,ERR_REPEAT,ERR_UNTIL,ERR_FOR,ERR_CASE,
	ERR_INTO,ERR_DOWNTO,ERR_DP,ERR_TOKEN,ERR_OF,ERR_INST,ERR_NOTEXISTID,ERR_EXISTID,ERR_CONSTCH,ERR_PROGD, ERR_PLEIN, ERR_AFFECT_ABS
	}ERREURS;



typedef  struct  { 
  ERREURS CODE_ERR;
   char message[40];
    }ErreursTab;




typedef enum{ TPROG, TCONST, TVAR } TSYM;

typedef  struct {       
 	char NOM[20];     
    TSYM TIDF;
    int ADRESSE;
    char valeur[30];
    } T_TAB_IDF; 
    
T_TAB_IDF TAB_IDFS[100]; // tableu des identificateur ( contint le type et son adresse )
int OFFSET=-1; // ?
int MEM[1000]; // pointeur de la pile 
int SP; //sommet pile

typedef enum{ ADD, SUB, MUL, DIV, EQL, NEQ, GTR, LSS, GEQ, LEQ, PRN, INN, INT, LDI, LDA, LDV, STO, BRN, BZE, HLT 
} MNEMONIQUES ;

MNEMONIQUES CND; // cette variable pour memoriser les conditions dans la partie semantique

typedef struct{
	MNEMONIQUES MNE;
	int SUITE;
} INSTRUCTION;

INSTRUCTION PCODE[1000];// table P-CODE 
int PC=0,PCT1,PCT2;// PC :Compteur d'instruction







ErreursTab ErrTab[100]={{ERR_CAR_INC ,"caract�re inconnu." },
						{ERR_FIC_VID,"fichier est vide"},
						{ERR_PROGRAM,"Erreur dans la syntaxe PROGRAM"},
						{ERR_ID,"Erreur dans la syntaxe ID"},
						{ERR_PV," ';' manquee "},
						{ERR_PT," '.' manquee"},
						{ERR_EGAL,"Erreur dans la syntaxe '=' "},
						{ERR_NUM,"Erreur dans la syntaxe NUM"},
						{ERR_CONST,"Erreur dans la syntaxe CONST"},
						{ERR_VAR_BEGIN,"Erreur dans la syntaxe VAR BEGIN "},
						{ERR_CONST_VAR_BEGIN,"Erreur dans la syntaxe CONST VAR BEGIN"},
						{ERR_BEGIN,"Erreur dans la syntaxe  BEGIN"},
						{ERR_END,"Erreur dans la syntaxe END"},
						{ERR_AFF,"Erreur dans la syntaxe AFFECTATION"},
						{ERR_IF,"Erreur dans la syntaxe IF"},
						{ERR_WHILE,"Erreur dans la syntaxe WHILE"},
						{ERR_DO,"Erreur dans la syntaxe DO"},
						{ERR_WRITE,"Erreur dans la syntaxe WRITE"},
						{ERR_READ,"Erreur dans la syntaxe READ"},
						{ERR_THEN,"Erreur dans la syntaxe THEN"},
						{ERR_PO,"Erreur dans la syntaxe '('"},
						{ERR_PF,"Erreur dans la syntaxe ')'"},
						{ERR_MOINS,"Erreur dans la syntaxe '-'"},
						{ERR_PLUS,"Erreur dans la syntaxe '+'"},
						{ERR_MULT,"Erreur dans la syntaxe '*'"},
						{ERR_DIV,"Erreur dans la syntaxe '/'"},
						{ERR_INF,"Erreur dans la syntaxe '<'"},
						{ERR_SUP,"Erreur dans la syntaxe '>'"},
						{ERR_INFEG,"Erreur dans la syntaxe '<='"},
						{ERR_SUPEG,"Erreur dans la syntaxe '>='"},
						{ERR_COND,"Erreur dans la syntaxe COND"},
						{ERR_TERM,"Erreur dans la syntaxe TERM"},
						{ERR_FACT,"Erreur dans la syntaxe FACT"},
						{ERR_DIFF,"Erreur dans la syntaxe '<>'"},
						{ERR_ACC,"Erreur dans la syntaxe '{'"},
						{ERR_EXPR,"Erreur dans la syntaxe EXPR"},
						{ERR_REPEAT,"Erreur dans la syntaxe REPETER"},
						{ERR_UNTIL,"Erreur dans la syntaxe UNTIL"},
						{ERR_FOR,"Erreur dans la syntaxe FOR"},
						{ERR_CASE,"Erreur dans la syntaxe CASE"},
						{ERR_INTO,"Erreur dans la syntaxe INTO "},
						{ERR_DOWNTO,"Erreur dans la syntaxe DOWNTO "},
						{ERR_DP,"Erreur dans la syntaxe ':'"},
						{ERR_OF,"Erreur dans la syntaxe OF "},
						{ERR_AFF,"Erreur dans la syntaxe ':=' "},
						{ERR_ELSE,"Erreur dans la syntaxe ELSE "},
						{ERR_INST,"Erreur double ';' "},
						{ERR_NOTEXISTID,"Erreur ID inconnu"},
						{ERR_EXISTID,"Erreur ID Deja declarer "},
						{ERR_CONSTCH,"impossible de modifier un CONST type "},
						{ERR_PROGD,"impossible d'utiliser PROGRAM "},
						{ERR_PLEIN,"Erreur de PCODE "},
						{ERR_AFFECT_ABS,"manque ':='"}
					};








void erreur(ERREURS);
void sym_suiv();
void program();
void block();
void consts();
void vars();
void insts();
void inst();
void affec();
void si();
void tantque();
void ecrire();
void lire();
void cond();
void expr();
void term();
void fact();
void repeter();
void pour();
void cas();



char* testerGenerateur(MNEMONIQUES M, int A){
	char *pcode = calloc(1024,sizeof(char));
	switch(M){
		case ADD: return "ADD";break;
		case SUB: return "SUB";break;
		case MUL: return "MUL";break;
		case DIV: return "DIV";break;
		case EQL: return "EQL";break;
		case NEQ: return "NEQ";break;
		case GTR: return "GTR";break;
		case LSS: return "LSS";break;
		case GEQ: return "GEQ";break;
		case LEQ: return "LEQ";break;
		case PRN: return "PRN";break;
		case INN: return "INN";break;
		case LDI: sprintf(pcode,"LDI   %d",A);
				  return pcode;break;
		case LDA: sprintf(pcode,"LDA   %d",A);
				  return pcode;break;
		case LDV: return "LDV";break;
		case STO: return "STO";break;
		case BRN: sprintf(pcode,"BRN   %d",A);
				  return pcode;break;
		case BZE: sprintf(pcode,"BZE   %d",A);
				  return pcode;break;
		case HLT: return "HLT";break;
		default:break;
	}
}


void generer1(MNEMONIQUES M){
	if(PC == 1000) erreur(ERR_PLEIN);
	else{
		PC++;
		PCODE[PC].MNE=M;
	//	printf("%s\n",testerGenerateur(M,0));
	}
}

void generer2(MNEMONIQUES M , int A){
	if(PC==1000) erreur(ERR_PLEIN);
	else{
		PC++;
		PCODE[PC].MNE = M;
		PCODE[PC].SUITE = A; 
		//printf("%s\n",testerGenerateur(M,A));
	}
}


int  testProg(char *chaine){
	int i;
	for(i=0;i<NbrId;i++){
		if(strcmp(chaine,TAB_IDFS[i].NOM)==0){
         	if (TAB_IDFS[i].TIDF==TPROG)erreur(ERR_PROGD);
		} 
	}
}


 notConst(char *chaine){
	int i;
	for(i=0;i<NbrId;i++){
		if(strcmp(chaine,TAB_IDFS[i].NOM)==0){
         	if(TAB_IDFS[i].TIDF==TCONST) erreur(ERR_CONSTCH);
			 	} 
	}
}

int chercheTableIdent(char * chaine){
	int i;
	for(i=0;i<NbrId;i++){
		if(strcmp(chaine,TAB_IDFS[i].NOM)==0) return i;
	}
	return -1;
}

  int  exist(char* chaine){
  		int adresse = chercheTableIdent(chaine);
		if(adresse==-1) erreur(ERR_NOTEXISTID);
		return adresse;		
}
  void  notExist(char *chaine){
  		int adresse = chercheTableIdent(chaine);
		if(adresse!=-1) erreur(ERR_EXISTID);
}

int ajouteTabIden(char* chaineId ,TSYM ct){
	int adresse;
	    if(chercheTableIdent(chaineId)==-1){
			TAB_IDFS[NbrId].TIDF=ct;
			adresse = ++OFFSET;
			TAB_IDFS[NbrId].ADRESSE = adresse;
			strcpy(TAB_IDFS[NbrId].NOM,chaine);
			NbrId++;
			return adresse;
	}
	else erreur(ERR_EXISTID);
}

void test_symbole(CODES_TOKENS ct, ERREURS code_erreur){

	if(Sym_Cour==ct){
		memset (chainePR, 0, sizeof (chaine));
		strcpy(chainePR,chaine);
		sym_suiv();
	}
	else erreur(code_erreur);
	
}






//Parseur
void program(){
	test_symbole(PROGRAM_TOKEN,ERR_PROGRAM);
	ajouteTabIden(chaine,TPROG);
	test_symbole(ID_TOKEN,ERR_ID);
	test_symbole(PV_TOKEN,ERR_PV);
	block();
	test_symbole(END_TOKEN,ERR_END);
	generer1(HLT);
	test_symbole(PT_TOKEN,ERR_PT);
	}

void block(){
	consts();
	vars();
	PCODE[0].MNE = INT;
	PCODE[0].SUITE = OFFSET; 
	insts();
}

void consts(){
	switch(Sym_Cour){
		
		case CONST_TOKEN: sym_suiv();
						  testProg(chaine);
						  AD=ajouteTabIden(chaine,TCONST);
						  test_symbole(ID_TOKEN,ERR_ID);
						  generer2(LDA,TAB_IDFS[AD].ADRESSE);
						  test_symbole(EG_TOKEN,ERR_EGAL);
						  test_symbole(NUM_TOKEN,ERR_NUM);
						  generer2(LDI,atoi(chainePR));
						  test_symbole(PV_TOKEN,ERR_PV);
						  generer1(STO);
						  while(Sym_Cour==ID_TOKEN){
						    AD=ajouteTabIden(chaine,TCONST);
						  	sym_suiv();
						  	generer2(LDA,TAB_IDFS[AD].ADRESSE);
							test_symbole(EG_TOKEN,ERR_EGAL);
						  	test_symbole(NUM_TOKEN,ERR_NUM);
						  	generer2(LDI,atoi(chainePR));
						  	test_symbole(PV_TOKEN,ERR_PV);
						  	generer1(STO);
						  }break;
		
		case VAR_TOKEN:break;
		case BEGIN_TOKEN: break;
	}
	
}

void vars(){
	
	switch(Sym_Cour){
		
		case VAR_TOKEN:sym_suiv();
						testProg(chaine);
						notExist(chaine);
						ajouteTabIden(chaine,TVAR);
						test_symbole(ID_TOKEN,ERR_ID);
						while(VIR_TOKEN==Sym_Cour){
						  	sym_suiv();
						  	testProg(chaine);
							if(chercheTableIdent(chaine)==1) erreur(ERR_EXISTID);
							ajouteTabIden(chaine,TVAR);
						  	test_symbole(ID_TOKEN,ERR_ID);
						  }test_symbole(PV_TOKEN,ERR_PV);break;
						  
		case BEGIN_TOKEN: break;
	}
	
}



void insts(){
	test_symbole(BEGIN_TOKEN,ERR_BEGIN);
	inst();
	while(Sym_Cour==PV_TOKEN){
		sym_suiv();
		inst();
	}
}


void inst(){
	
	switch(Sym_Cour){
		
		case BEGIN_TOKEN: insts();break;
		case ID_TOKEN:  AD=exist(chaine);
						generer2(LDA,TAB_IDFS[AD].ADRESSE);
						if(TAB_IDFS[AD].TIDF==TCONST){
							generer1(LDV);
							CMPT1++;
							CMPT2++;
							sym_suiv();
							break;
						}
						else {
						affec();break; }
						
		case IF_TOKEN: si();break;
		case WHILE_TOKEN: tantque();break;
		case WRITE_TOKEN: ecrire();break;
		case READ_TOKEN: lire();break;
		case REPEAT_TOKEN : repeter();break;
		case FOR_TOKEN : pour();break;
		case CASE_TOKEN : cas();break; 
		//case PT_TOKEN: break;
		//case END_TOKEN: break;
		default: erreur(ERR_INST);break;
	}
}

void affec(){
	testProg(chaine);
	sym_suiv();
	printf("ff");
	switch(Sym_Cour){
		case AFF_TOKEN: notConst(chainePR);
						test_symbole(AFF_TOKEN,ERR_AFF);break;
		default: erreur(ERR_AFFECT_ABS);break;
	}
	expr();
	generer1(STO);
	CMPT1++;
	CMPT2++;
}

void si(){
	
	test_symbole(IF_TOKEN,ERR_IF);
	cond();
	generer2(BZE,0);
	PCT1=PC;
	CMPT1=1;
	sym_suiv();
	test_symbole(THEN_TOKEN,ERR_THEN);
	inst();
	PCODE[PCT1].SUITE = PCT1 + CMPT1 +1;
	printf("%s",chaine);
	switch(Sym_Cour){
			case ELSE_TOKEN : 	generer2(BRN,0);
								printf("%s",chaine);
								PCT2=PC;
								CMPT2=1;
								sym_suiv();
								inst(); 
								PCODE[PCT2].SUITE =PCT2 + CMPT2 +1;
								break;
			default : break;
		}
}


void tantque(){
	test_symbole(WHILE_TOKEN,ERR_WHILE);
	cond();
	test_symbole(DO_TOKEN,ERR_DO);
	inst();
} 

void ecrire(){
	test_symbole(WRITE_TOKEN,ERR_WRITE);
	test_symbole(PO_TOKEN,ERR_PO);
	expr();
	generer1(PRN);
	while(VIR_TOKEN==Sym_Cour){
		sym_suiv();
		expr();
		generer1(PRN);
	}
	test_symbole(PF_TOKEN,ERR_PF);
}


void lire(){
	test_symbole(READ_TOKEN,ERR_READ);
	test_symbole(PO_TOKEN,ERR_PO);
	AD=exist(chaine);
	notConst(chaine);
	testProg(chaine);
	test_symbole(ID_TOKEN,ERR_ID);
	generer2(LDA,TAB_IDFS[AD].ADRESSE);
	generer1(INN);
	while(VIR_TOKEN==Sym_Cour){
		sym_suiv();
		AD=exist(chaine);
		notConst(chaine);
		testProg(chaine);
		test_symbole(ID_TOKEN,ERR_ID);
		generer2(LDA,TAB_IDFS[AD].ADRESSE);
		generer1(INN);
	}
	test_symbole(PF_TOKEN,ERR_PF);
}

void repeter(){
	test_symbole(REPEAT_TOKEN,ERR_REPEAT);
	inst();
	test_symbole(UNTIL_TOKEN,ERR_UNTIL);
	cond();
}

void pour(){
		test_symbole(FOR_TOKEN,ERR_FOR);
		exist(chaine);
		notConst(chaine);
		testProg(chaine);
		test_symbole(ID_TOKEN,ERR_ID);
		test_symbole(AFF_TOKEN,ERR_AFF);
		test_symbole(NUM_TOKEN,ERR_NUM);
		switch(Sym_Cour){
			case INTO_TOKEN: test_symbole(INTO_TOKEN,ERR_INTO);break;
			default : DOWNTO_TOKEN : test_symbole(DOWNTO_TOKEN,ERR_DOWNTO);break;	
		}
		test_symbole(NUM_TOKEN,ERR_NUM);
		test_symbole(DO_TOKEN,ERR_DO);
		inst();
}
 void  cas(){
 	test_symbole(CASE_TOKEN,ERR_CASE);
 	exist(chaine);
 	testProg(chaine);
 	test_symbole(ID_TOKEN,ERR_ID);
 	test_symbole(OF_TOKEN,ERR_OF);
 	test_symbole(NUM_TOKEN,ERR_NUM);
	test_symbole(DP_TOKEN,ERR_DP);
 	 	inst();
 	while(Sym_Cour == NUM_TOKEN){
 		sym_suiv();
 		test_symbole(DP_TOKEN,ERR_DP);
 	 	inst();
	 }
	 switch(Sym_Cour){
	 	case ELSE_TOKEN : test_symbole(ELSE_TOKEN,ERR_ELSE);
	 					  inst();break;
	 	default: break;
	 }
	 
	 test_symbole(END_TOKEN,ERR_END);	
 	
 }

void cond(){
	expr();
	switch(Sym_Cour){
		case EG_TOKEN:     test_symbole(EG_TOKEN,ERR_EGAL);
						    CND=EQL;
							break;
		case DIFF_TOKEN:   test_symbole(DIFF_TOKEN,ERR_DIFF);
							CND=NEQ;
							break;
		case INF_TOKEN:    test_symbole(INF_TOKEN,ERR_INF);
							CND=LSS;
							break;
		case SUP_TOKEN:    test_symbole(SUP_TOKEN,ERR_SUP);
							CND=GTR;
							break;
		case INFEG_TOKEN : test_symbole(INFEG_TOKEN,ERR_INFEG);
							CND=LEQ;
							break;
		case SUPEG_TOKEN : test_symbole(SUPEG_TOKEN,ERR_SUPEG);
							CND=GEQ;
							break;
		default: erreur(ERR_COND); break;
	}
	expr();
	generer1(CND);
}	
	void expr() {  
	  term();   
	   while(Sym_Cour==PLUS_TOKEN||Sym_Cour==MOINS_TOKEN){   
	   		OP = Sym_Cour;    
		    sym_suiv();      
			term();    
			if(OP==PLUS_TOKEN)generer1(ADD);
			else generer1(SUB);
			CMPT1++;
			CMPT2++;
		}
}

void term() {
	fact(); 
	while(Sym_Cour==MULT_TOKEN||Sym_Cour==DIV_TOKEN) {
		OP = Sym_Cour;
		sym_suiv();
		fact();
		if(OP == MULT_TOKEN) generer1(MUL);
		else generer1(DIV);
		
		CMPT1++;
		CMPT2++;
	} 
}


void fact() {   
	switch(Sym_Cour) {        
		case ID_TOKEN:AD=exist(chaine);
					  testProg(chaine);
					  generer2(LDA,TAB_IDFS[AD].ADRESSE);
					  CMPT1++;
				      CMPT2++;
					  generer1(LDV);
					  CMPT1++;
				      CMPT2++;
					  sym_suiv();break;  
					  
		case NUM_TOKEN: generer2(LDI,atoi(chaine));
						CMPT1++;
					    CMPT2++;
						sym_suiv(); break;   
						 
		case PO_TOKEN: sym_suiv(); 
			expr();  
			//test_symbole(PF_TOKEN,ERR_PF);
			break;
		case PF_TOKEN: sym_suiv(); break;
   }
}










						

void erreur(ERREURS code_erreur){
		printf("(LIGNE[%d] | COLONE[%d]) ERREUR[%d]: %s \n",CUR_POS_Y,CUR_POS_X,ErrTab[code_erreur].CODE_ERR,ErrTab[code_erreur].message);
		exit(0);
}

const char* deter_Token_code(CODES_TOKENS code) {
   switch (code) 
   {
      case PROGRAM_TOKEN : return "PROGRAM_TOKEN";
      case NUM_TOKEN : return "NUM_TOKEN";
      case CONST_TOKEN : return "CONST_TOKEN";
      case VAR_TOKEN : return "VAR_TOKEN";
      case BEGIN_TOKEN : return "BEGIN_TOKEN";
      case END_TOKEN : return "END_TOKEN";
      case IF_TOKEN : return "IF_TOKEN";
      case THEN_TOKEN : return "THEN_TOKEN";
      case ELSE_TOKEN     : return "ELSE_TOKEN";
      case WHILE_TOKEN : return "WHILE_TOKEN";
      case DO_TOKEN : return "DO_TOKEN";
      case READ_TOKEN : return "READ_TOKEN";
      case WRITE_TOKEN : return "WRITE_TOKEN";
      case PV_TOKEN : return "PV_TOKEN";
      case PT_TOKEN : return "PT_TOKEN";
      case PLUS_TOKEN : return "PLUS_TOKEN";
      case MOINS_TOKEN : return "MOINS_TOKEN";
      case MULT_TOKEN : return "MULT_TOKEN";
      case DIV_TOKEN : return "DIV_TOKEN";
      case VIR_TOKEN : return "VIR_TOKEN";
      case AFF_TOKEN : return "AFF_TOKEN";
      case INF_TOKEN : return "INF_TOKEN";
      case INFEG_TOKEN : return "INFEG_TOKEN";
      case SUP_TOKEN : return "SUP_TOKEN";
      case SUPEG_TOKEN : return "SUPEG_TOKEN";
      case DIFF_TOKEN : return "DIFF_TOKEN";
      case PO_TOKEN : return "PO_TOKEN";
      case PF_TOKEN : return "PF_TOKEN";
      case FIN_TOKEN : return "FIN_TOKEN";
      case ID_TOKEN : return "ID_TOKEN";
      case ERREUR_TOKEN : return "ERREUR_TOKEN";
      case EOF_TOKEN : return "EOF_TOKEN";
      case EG_TOKEN : return "EG_TOKEN";
      case COM_TOKEN : return "COMMENTAIRE_TOKEN";
      case ACC_TOKEN : return "ACC_TOKEN";
      case DOWNTO_TOKEN : return "DOWNTO_TOKEN";
      case INTO_TOKEN : return "INTO_TOKEN";
      case UNTIL_TOKEN : return "UNTIL_TOKEN";
      case FOR_TOKEN : return "FOR_TOKEN";
      case REPEAT_TOKEN : return "REPEAT_TOKEN";
      case DP_TOKEN : return "DP_TOKEN";
      case OF_TOKEN : return "OF_TOKEN";
   }
}

void lire_nombre(){
  while(isdigit(Car_Cour)){
  	*pchar++ = Car_Cour;
    Car_Cour = fgetc(file);
    //Calculer la postion de cursour
	if(Car_Cour=='\n'){
	  	CUR_POS_Y++;
	  	CUR_POS_X=0;
	  }
	  else CUR_POS_X++;
	  
  }
  if(!isalpha(Car_Cour)){
 	Sym_Cour=NUM_TOKEN;
  }
  
}

void lire_mot(){
  while(isdigit(Car_Cour) || isalpha(Car_Cour)){
  	*pchar++ = Car_Cour;
    Car_Cour = fgetc(file);
        
	//Calculer la postion de cursour
	if(Car_Cour=='\n'){
	  	CUR_POS_Y++;
	  	CUR_POS_X=0;
	  }
	  else CUR_POS_X++;
   
  }
  if(strcmp(chaine,"PROGRAM")==0||strcmp(chaine,"program")==0) Sym_Cour = PROGRAM_TOKEN;
  else if(!strcmp(chaine,"begin")||strcmp(chaine,"BEGIN")==0) Sym_Cour = BEGIN_TOKEN;
  else if(!strcmp(chaine,"end")||strcmp(chaine,"END")==0) Sym_Cour = END_TOKEN;
  else if(!strcmp(chaine,"if")|| strcmp(chaine,"IF")==0) Sym_Cour = IF_TOKEN;
  else if(!strcmp(chaine,"then")||strcmp(chaine,"THEN")==0) Sym_Cour = THEN_TOKEN;
  else if(!strcmp(chaine,"else")||strcmp(chaine,"ELSE")==0) Sym_Cour = ELSE_TOKEN;
  else if(!strcmp(chaine,"read")||strcmp(chaine,"READ")==0) Sym_Cour = READ_TOKEN;
  else if(!strcmp(chaine,"write")||strcmp(chaine,"WRITE")==0) Sym_Cour = WRITE_TOKEN;
  else if(!strcmp(chaine,"const")||strcmp(chaine,"CONST")==0) Sym_Cour = CONST_TOKEN;
  else if(!strcmp(chaine,"var")||strcmp(chaine,"VAR")==0) Sym_Cour = VAR_TOKEN;
  else if(!strcmp(chaine,"while")||strcmp(chaine,"WHILE")==0) Sym_Cour = WHILE_TOKEN;
  else if(!strcmp(chaine,"do")||strcmp(chaine,"DO")==0) Sym_Cour = DO_TOKEN;
  else Sym_Cour = ID_TOKEN;
  
}

void sym_suiv(){
	memset (chaine, 0, sizeof (chaine));
	pchar = chaine;
  while( isspace(Car_Cour) && Car_Cour != EOF){
    Car_Cour = fgetc(file);
    
     //Calculer la postion de cursour
	if(Car_Cour=='\n'){
	  	CUR_POS_Y++;
	  	CUR_POS_X=0;
	  }
	  else CUR_POS_X++;
	  
}
	*pchar = Car_Cour;

  if(isdigit(Car_Cour))lire_nombre();
  else {
    if(isalpha(Car_Cour))lire_mot();
    else {
      switch (Car_Cour){
        case '(' : Sym_Cour = PO_TOKEN; break;
        case ')' : Sym_Cour = PF_TOKEN; break;
        case ';' : Sym_Cour = PV_TOKEN; break;
        case ',' : Sym_Cour = VIR_TOKEN; break;
        case '=' : Sym_Cour = EG_TOKEN; break;
        case '+' : Sym_Cour = PLUS_TOKEN; break;
        case '-' : Sym_Cour = MOINS_TOKEN; break;
        case '*' : Sym_Cour = MULT_TOKEN; break;
        case '/' : Sym_Cour = DIV_TOKEN; break;
        case '.' : Sym_Cour = PT_TOKEN; break;
        case ':' : Car_Cour = fgetc(file);
        
				     //Calculer la postion de cursour
					if(Car_Cour=='\n'){
					  	CUR_POS_Y++;
					  	CUR_POS_X=0;
					  }
					  else CUR_POS_X++;
	  
                  	if(Car_Cour == '=') Sym_Cour = AFF_TOKEN; 
                  	else Sym_Cour = DP_TOKEN; break;
        case '>' : Car_Cour = fgetc(file); 
        
				     //Calculer la postion de cursour
					if(Car_Cour=='\n'){
					  	CUR_POS_Y++;
					  	CUR_POS_X=0;
					  }
					  else CUR_POS_X++;
	  
                  	if(Car_Cour == '=') Sym_Cour = SUPEG_TOKEN;
                  	else Sym_Cour = SUP_TOKEN; break;
        case '<' : Car_Cour = fgetc(file); 
        
				     //Calculer la postion de cursour
					if(Car_Cour=='\n'){
					  	CUR_POS_Y++;
					  	CUR_POS_X=0;
					  }
					  else CUR_POS_X++;
	  
                  	if(Car_Cour == '=') Sym_Cour = INFEG_TOKEN;
                  	else if (Car_Cour == '>') Sym_Cour = DIFF_TOKEN;
                        else Sym_Cour = INF_TOKEN; break;
        case '{' : Car_Cour=fgetc(file);
        
				     //Calculer la postion de cursour
					if(Car_Cour=='\n'){
					  	CUR_POS_Y++;
					  	CUR_POS_X=0;
					  }
					  else CUR_POS_X++;
	
        			if(Car_Cour=='*'){
						while(Car_Cour!=EOF){
        					if(Car_Cour=='*'){
        						Car_Cour=fgetc(file);
							     //Calculer la postion de cursour
								if(Car_Cour=='\n'){
								  	CUR_POS_Y++;
								  	CUR_POS_X=0;
								  }
								  else CUR_POS_X++;
								  
        						if(Car_Cour=='}'){
        							Sym_Cour = COM_TOKEN;break;
								}
								else{Car_Cour=fgetc(file);
								     //Calculer la postion de cursour
									if(Car_Cour=='\n'){
									  	CUR_POS_Y++;
									  	CUR_POS_X=0;
									  }
									  else CUR_POS_X++;
								}
		  
							}
							else {
							Car_Cour=fgetc(file);
								     //Calculer la postion de cursour
									if(Car_Cour=='\n'){
									  	CUR_POS_Y++;
									  	CUR_POS_X=0;
									  }
									  else CUR_POS_X++;
								}
						}
					}	
					else{
						Sym_Cour = ACC_TOKEN;
					}break;
        case EOF : Sym_Cour = EOF_TOKEN;break;
        
        default:
        	erreur(0);
        	exit(0);
      }
      if(Sym_Cour != ERREUR_TOKEN && Sym_Cour != INF_TOKEN && Sym_Cour != SUP_TOKEN && Sym_Cour != EOF_TOKEN) Car_Cour = fgetc(file);
    //	printf("%s  \n",deter_Token_code(Sym_Cour));
				     //Calculer la postion de cursour
					if(Car_Cour=='\n'){
					  	CUR_POS_Y++;
					  	CUR_POS_X=0;
					  }
					  else CUR_POS_X++;
	  
    }
  }
}

int main() {
	int i;
	  file = fopen("test.p","r+");
	  Car_Cour = fgetc(file);
	  if(Car_Cour=='\n'){
	  	CUR_POS_Y++;
	  	CUR_POS_X=0;
	  }
	  else CUR_POS_X++;
	  if(Car_Cour==EOF){
	  	erreur(ERR_FIC_VID);
	        	exit(0);
	  }
	  
	    sym_suiv();
	    program();
	  printf("BRAVO: le programe est correct!! \n"); 
	  
	  for(i=1;i<=PC;i++){
	  	printf("%s\n",testerGenerateur(PCODE[i].MNE,PCODE[i].SUITE));
	  }
	  return 0;
}

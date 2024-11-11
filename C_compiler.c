#include <stdio.h>
#include <string.h>
#define MAX_TEXT_SIZE 2000
#define MAX_VARS 2000
#define MAX_VAR_NAME_LENGTH 50
#define MAX_SYMBOLS 100

FILE* fs;
FILE* fd;
char text[MAX_TEXT_SIZE] = "";
int ind = -1;
char vars[MAX_VARS][MAX_VAR_NAME_LENGTH];
char varstype[MAX_VARS][MAX_VAR_NAME_LENGTH] = { 0 };
int vz = 0;
char symbol[MAX_VAR_NAME_LENGTH];
char result[100];
char* tmp;
char* type;
int ETC = 0;
int tmpVARS[100];
int tmpi=0;
//global types
char Ftype[100]; //facteur type
char Ttype[100]; //terme type 
char EStype[100]; //expression simple type
char Etype[100];  //expression type
char EPtype[100];
char DPtype[100];
char KPtype[100];
char Itype[100];

const char* Symbols[MAX_SYMBOLS]= { "program", "var", "integer", "char", "begin", "end", "if", "then", "else", "while", "do", "read", "readln", "write", "writeln" };



void erreursymbole()
{
    printf("symbole non accepte !\n");
}

void erreursemantique()
{
    printf("erreur semantique\n");
}

void erreurlexical() {
    printf("erreur lexical\n");
}





int cree_Etiq() {
    ETC++;
    return ETC;
}



void ajouter_type(int ide, char* tp) {
    strcpy(varstype[ide], tp);
}

int compatible(char* tp1, char* tp2)
{
    if (strcmp(tp1, tp2) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

char* chercher_type(int id)
{
    return varstype[id];
}

typedef struct {
    char l[MAX_VAR_NAME_LENGTH];
    char r[MAX_VAR_NAME_LENGTH];
} Couple;

Couple createCouple(const char* lr, const char* rr) {
    Couple c;
    strncpy(c.l, lr, MAX_VAR_NAME_LENGTH - 1);
    c.l[MAX_VAR_NAME_LENGTH - 1] = '\0';  // Ensure null-termination
    strncpy(c.r, rr, MAX_VAR_NAME_LENGTH - 1);
    c.r[MAX_VAR_NAME_LENGTH - 1] = '\0';  // Ensure null-termination
    return c;
}

Couple symbolC;


char* Unilexid(const char* s, int sz) {   //if its a keyword,returns said keyword,if its a variable returns id
    for (int i = 0; i < sz; i++) {
        if (strcmp(s, Symbols[i]) == 0) {
            return s;
        }
    }
    return "id";
}


char* RangerId(const char* s, int sz) {               //returns adress of variable in the table if it exists,else returns 0
    // Check if the input string matches any of the symbols
    for (int i = 0; i < sz; i++) {
        if (strcmp(s, Symbols[i]) == 0) {
            return "0";
        }
    }

    // Check if the input string matches any of the existing variables



    for (int i = 0; i < vz; i++) {
        if (strcmp(vars[i], s) == 0) {
            sprintf(result, "%d", i+1);
            return result;
        }
    }

    // If the input string is not found, add it to the variables
    vz++;
    strncpy(vars[vz - 1], s, MAX_VAR_NAME_LENGTH - 1);
    //vars[vz - 1][MAX_VAR_NAME_LENGTH - 1] = '\0';  // Ensure null-termination
    sprintf(result, "%d", vz);

    return result;
}

char carSuivant() {
    return(fgetc(fs));
}
void reculler() {
    fseek(fs, -1, SEEK_CUR);
}




Couple AnalLex() {

    char chaine[MAX_VAR_NAME_LENGTH];  
    int etat = 0;
    char car;
    ind++;
    size_t len;
    while (1) {
        switch (etat) {
        case 0:
            chaine[0] = '\0';
            car = fgetc(fs);
            if ((car == ' ') || (car == '\t') || (car == '\n'))
            {
                etat = 0;
                ind++;
            }

            else if (car == '*') {
                etat = 1;
            }
            else if (car == '/') {
                etat = 2;
            }
            else if (car == '%') {
                etat = 3;
            }
            else if (car == '&') {
                etat = 4;
            }

            else if (car == '+') {
                etat = 6;
            }
            else if (car == '-') {
                etat = 7;
            }
            else if (car == '|') {
                etat = 8;
            }
            else if ('A' <= toupper(car) && toupper(car) <= 'Z') {
                etat = 11;
            }
            else if ('0' <= car && car <= '9') {
                etat = 13;
            }
            else if (car == '>') {
                etat = 15;
            }
            else if (car == '<') {
                etat = 19;
            }
            else if (car == '=') {
                etat = 22;
            }
            else if (car == '(') {
                etat = 24;
            }
            else if (car == ';') {
                etat = 28;
            }
            else if (car == ':') {
                etat = 29;
            }
            else if (car == ',') {
                etat = 32;
            }
            else {
                etat = 999;
            }
            break;


        case 4:
            ind++;
            car = carSuivant();
            if (car == '&') {
                etat = 5;
            }
            else {
                etat = 9;
            }
            break;

        case 8:

            ind++;
            car = carSuivant();
            if (car == '|') {
                etat = 10;
            }
            else {
                etat = 9;
            }
            break;
        case 11:
            len = strlen(chaine);
            chaine[len] = car;
            chaine[len + 1] = '\0';
            ind++;
            car = carSuivant();
            if (('0' <= car && car <= '9') || ('A' <= toupper(car) && toupper(car) <= 'Z')) {
                etat = 11;
            }
            else {
                etat = 12;
            }
            break;

        case 13:
            len = strlen(chaine);
            chaine[len] = car;
            chaine[len + 1] = '\0';
            ind++;
            car = carSuivant();
            if ('0' <= car && car <= '9') {
                etat = 13;
            }
            else {
                etat = 14;
            }
            break;
        case 15: 
            ind++;
            car = carSuivant();
            if (car == '=') {
                etat = 17;
            }
            else if (car == '<') {
                etat = 16;
            }
            else {
                etat = 18;
            }
            break;
        case 19:
            ind++;
            car = carSuivant();
            if (car == '=') {
                etat = 20;
            }
            else {
                etat = 21;
            }
            break;
        case 22:
            ind++;
            car = carSuivant();
            if (car == '=') {
                etat = 23;
            }
            else {
                etat = 9;
            }
            break;
        case 24:
            ind++;
            car = carSuivant();
            if (car == '*') {
                etat = 25;
            }
            else {
                etat = 9;
            }
            break;
        case 25:
            ind++;
            car = carSuivant();
            if (car == '*') {
                etat = 26;
            }
            else {
                etat = 25;
            }
            break;
        case 26:
            ind++;
            car = carSuivant();
            if (car == ')') {
                etat = 27;
            }
            else {
                etat = 25;
            }
            break;
        case 29:
            ind++;
            car = carSuivant();
            if (car == '=') {
                etat = 30;
            }
            else {
                etat = 31;
            }
            break;
        case 1:
            return createCouple("opmul", "mul");
        case 2:
            return createCouple("opmul", "div");
        case 3:
            return createCouple("opmul", "mod");
        case 5:
            return createCouple("opmul", "and");
        case 6:
            return createCouple("opadd", "add");
        case 7:
            return createCouple("opadd", "sub");
        case 9:
            erreurlexical();
            return createCouple("", "");
        case 10:
            return createCouple("opadd", "or");
        case 12: //unfinished
            reculler();
            return createCouple(Unilexid(chaine, 15), RangerId(chaine, 15));
        case 14: //unfinished
            reculler();
            return createCouple("nb", chaine);
        case 16:
            return createCouple("oprel", "dif");
        case 17: //unfinished
            reculler();
            return createCouple("oprel", "pre");
        case 18:
            reculler();
            return createCouple("oprel", "ppq");
        case 20:
            return createCouple("oprel", "pge");
        case 21:
            reculler();
            return createCouple("oprel", "pgq");
        case 23:
            return createCouple("oprel", "ega");
        case 27:
            return createCouple("", "");
        case 28:
            return createCouple(";", "pvr");
        case 30:
            return createCouple(":=", "aff");
        case 31:
            reculler();
            return createCouple(":", "dp");
        case 32:
            return createCouple(",", "vr");
        default:
            // Handle the default case accordingly
            return createCouple("", "");

        }



    }
}
void emettre(char* ch, char* ch2)
{
    if (ch2 != NULL) {
        fprintf(fd, ch2);
    }
    fprintf(fd, ch);
    fprintf(fd, "\n");
}



Couple nextsymbol() {
    Couple C = AnalLex();
    return C;
}

void accept(const char* s) {

    if (strcmp(s, symbol) == 0) {
        // If s is equal to symbol, update symbol with the next symbol
        // (Assuming you have a function like nextsymbol() to get the next symbol)
        symbolC = nextsymbol();
        strncpy(symbol, symbolC.l, MAX_VAR_NAME_LENGTH - 1);
        symbol[MAX_VAR_NAME_LENGTH - 1] = '\0';  // Ensure null-termination
    }
    else {
        // Handle the error
        fprintf(stderr, "Error: Symbol mismatch\n");
        // Optionally, you could exit or return an error code
        // exit(EXIT_FAILURE);
    }
}


void initialisation() {
    for (int i = 0; i < MAX_VARS; ++i) {
        strcpy(varstype[i], "erreur_de_type");
    }
}


void P();
void Dcl();
void B_prime();
void Liste_id();
void A_prime();
void Type();
void Inst_composee();
void Inst();
void Liste_inst();
void C_prime();
void I();
void K_prime();
void Exp();
void Exp_simple();
void D_prime();
void Terme();
void E_prime();
void Facteur();
void F_prime();


void P() {



    //initialisation();
    accept("program");
    accept("id");
    accept(";");
    Dcl();
    Inst_composee();
}

void Dcl() {
    B_prime();
}

void B_prime() {
    if (strcmp(symbol, "var") == 0) {
        tmpi = 0;
        accept("var");
        Liste_id();
        accept(":");
        Type();
        accept(";");
        B_prime();
    }

}

void Liste_id() {
    tmpVARS[0] = atoi(symbolC.r);
    tmpi++;
    accept("id");
    A_prime();
}

void A_prime() {
    if (strcmp(symbol, ",") == 0) {
        accept(",");
        tmpVARS[tmpi] = atoi(symbolC.r);
        tmpi++;
        accept("id");
        A_prime();
    }
}

void Type() {
    for (int j = 0; j < tmpi; j++) {
        if (strcmp(varstype[tmpVARS[j]], "") != 0) {
            erreursemantique();
            ajouter_type(tmpVARS[j], "erreur_de_type");  //case of variable declared twice
        }
        else {
            ajouter_type(tmpVARS[j], symbol);
        }
        
    }

    if (strcmp(symbol, "integer") == 0) {
        accept("integer");
    }
    else if (strcmp(symbol, "char") == 0) {
        accept("char");
    }
    else {
        fprintf(stderr, "Error: Symbol mismatch\n");
    }
}

void Inst_composee() {
    accept("begin");
    Inst();
    accept("end");
}

void Inst() {
    if ((strcmp(symbol, "while") == 0) ||(strcmp(symbol, "if") == 0) ||
        (strcmp(symbol, "id") == 0) ||(strcmp(symbol, "read") == 0) || 
        (strcmp(symbol, "readln") == 0) || (strcmp(symbol, "write") == 0) 
        || (strcmp(symbol, "writeln") == 0)) {
        Liste_inst();
    }

}

void Liste_inst() {
    I();
    C_prime();
}

void C_prime() {
    if (strcmp(symbol, ";") == 0) {
        accept(";");
        I();
        C_prime();

    }
}

void I() {
    if (strcmp(symbol, "id") == 0) {
        int IDtype=atoi(symbolC.r);
        emettre(symbolC.r, "valeurg ");
        accept("id");
        accept(":=");
        Exp_simple();
        emettre(":=",NULL);
        if (!compatible(chercher_type(IDtype), EStype)) {
            strcpy(Itype,"erreur_de_type");
            erreursemantique();
        }
        
    }
    else if (strcmp(symbol, "if") == 0) {
        accept("if");
        int test, quit;
        test = cree_Etiq();
        quit = cree_Etiq();
        char testS[100];
        char quitS[100];
        sprintf(testS, "%d", test);
        sprintf(quitS, "%d", quit);
        Exp();
        emettre(testS, "AllerSiFaux ");
        accept("then");
        I();
        emettre(quitS, "AllerA ");
        emettre(testS, "Etiq ");
        accept("else");
        I();
        emettre(quitS, "Etiq ");

    }
    else if (strcmp(symbol, "while") == 0) {
        int test,quit;
        test=cree_Etiq();
        quit= cree_Etiq();
        char testS[100];
        char quitS[100];
        sprintf(testS, "%d", test);
        sprintf(quitS, "%d", quit);
        emettre(testS, "Etiq ");
        accept("while");
        Exp();
        accept("do");
        emettre(quitS, "AllerSiFaux ");
        I();
        emettre(testS, "AllerA ");
        emettre(quitS, "Etiq ");


    }
    else if ((strcmp(symbol, "read") == 0) || (strcmp(symbol, "readln") == 0) || (strcmp(symbol, "write") == 0) || (strcmp(symbol, "writeln") == 0)) {
        accept(symbol);
        accept("(");
        accept("id");
        accept(")");
    }
}

void Exp() {
    Exp_simple();
    strcpy(Etype, EStype);
    K_prime();
    if (!compatible(KPtype, Etype)) {
        strcpy(Etype, "erreur_de_type");
        erreursemantique();
    }
}

void K_prime() {
    if (strcmp(symbol, "oprel") == 0) {
        char tmp[100];
        strcpy(tmp, symbolC.r);
        accept("oprel");
        Exp_simple();
        strcpy(KPtype, EStype);
        emettre(tmp,NULL);
    }
    else {
        strcpy(KPtype, Etype);
    }
}

void Exp_simple() {
    Terme();
    strcpy(EStype, Ttype);
    D_prime();
    if (!compatible(EStype, DPtype)) { //opadd can only be used on integers
        strcpy(EStype, "erreur_de_type");
        erreursemantique();
    }

    //EStype = Ttype;
}

void D_prime() {
    if (strcmp(symbol, "opadd") == 0) {
        char tmp[100];
        char tmpTYPE[100]; // for copying terme because it changes
        strcpy(tmp, symbolC.r);
        accept("opadd");
        Terme();
        strcpy(tmpTYPE, Ttype);
        strcpy(DPtype, Ttype);
        emettre(tmp,NULL);
        D_prime();
        if (!compatible(DPtype, tmpTYPE)||(strcmp(DPtype,"integer") != 0)) {  //opadd can only be used on integers
            strcpy(DPtype, "erreur_de_type");
            erreursemantique();
        }
    }
    else {
        strcpy(DPtype, EStype);
    }
}

void Terme() {
    Facteur();
    strcpy(Ttype, Ftype);
    E_prime();
    if (!compatible(Ttype, EPtype)) {  
        strcpy(Ttype, "erreur_de_type");
        erreursemantique();
    }
    
}

void E_prime() {
    if (strcmp(symbol, "opmul") == 0) {
        char tmp[100];
        char tmpTYPE[100];  //temporary type to copy facteur
        strcpy(tmp, symbolC.r);
        accept("opmul");
        Facteur();
        strcpy(EPtype, Ftype);
        strcpy(tmpTYPE,Ftype);
        emettre(tmp,NULL);
        E_prime();
        if (!compatible(EPtype, tmpTYPE) || (strcmp(EPtype, "integer") != 0)) {
            strcpy(EPtype, "erreur_de_type");
            erreursemantique();
        }



    }
    else {
        strcpy(EPtype, Ttype);
    }
}

void Facteur() {
    if (strcmp(symbol, "id") == 0) {
        strcpy(Ftype, chercher_type(atoi(symbolC.r)));
        emettre(symbolC.r, "valeurd ");
        accept("id");
    }
    else if (strcmp(symbol, "nb") == 0) {
        strcpy(Ftype, "integer");
        emettre(symbolC.r, "empiler ");
        accept("nb");
    }
    else {
        accept("(");
        Exp_simple();
        strcpy(Ftype,EStype);
        accept(")");
    }
}


//string Rangerid(string s,string symbols[],int sz,string )



int main()
{

    fs = fopen("input.txt", "r");
    fd = fopen("codeInterm.txt", "wt");
    if (fs == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    //strcpy(text, "program id;var id:integer;begin id:=nb;end");
    Couple C = AnalLex();
    strcpy(symbol, C.l);
    P();
    fclose(fs);
    fclose(fd);


}

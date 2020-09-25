#define TAM_VETOR 20
char fila[TAM_VETOR];
char ind_fila = 0;

void reset_fila(void)
{        
    fila[0] = ' ';
    fila[1] = 0;
    ind_fila = 0;    
}

void mod_fila(char pos, char tecla)
{
    fila[pos] = tecla;
}

char* ler_fila (void)
{
    return fila;
}

void inserir_fila(const char tecla)
{
    if(ind_fila < TAM_VETOR)
    {    
        if(tecla)
        {
            fila[ind_fila] = tecla;
            ind_fila++;
            fila[ind_fila] = 0;
        }
    }
}

char pos_fila(void)
{
    return ( ind_fila );
}

char char_fila(char i)
{    
    switch(i)
    {
        case 0:     return('0');    break;
        case 1:     return('1');    break;
        case 2:     return('2');    break;
        case 3:     return('3');    break;
        case 4:     return('4');    break;
        case 5:     return('5');    break;
        case 6:     return('6');    break;
        case 7:     return('7');    break;
        case 8:     return('8');    break;
        case 9:     return('9');    break;
        default:    return( 0 );    break;
    }            
}


union 
{ 
    struct
    {
        unsigned char    :4;
        unsigned char A  :1;
        unsigned char B  :1;
        unsigned char C  :1;
        unsigned char D  :1;      
    };  
}atuador;

void alt_estado( char tecla )
{
    switch(tecla)
    {
        case 'A':   atuador.A = atuador.A ^ 1;      break;
        case 'B':   atuador.B = atuador.B ^ 1;      break;
        case 'C':   atuador.C = atuador.C ^ 1;      break;
        case 'D':   atuador.D = atuador.D ^ 1;      break;
    }
    
}

char ler_estado(char tecla)
{
    switch(tecla)
    {
        case 'A':   return(atuador.A);      break;
        case 'B':   return(atuador.B);      break;
        case 'C':   return(atuador.C);      break;
        case 'D':   return(atuador.D);      break;  
        default:    return 0;               break;
    }
    
}

void init_atuadores(void)
{
    atuador.A = 0;
    atuador.B = 0;
    atuador.C = 0;
    atuador.D = 0;    
}




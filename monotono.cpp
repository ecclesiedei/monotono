/*
 
 ######################################################################################
 
 
            By :        Marcos Vinicius Teixeira
                                UFMS-FACOM
                                08-09-15
 
 ######################################################################################
 
 */

#include <iostream>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// Estrutura para representar um vertice
typedef struct Vertice
{
    char name[10];
    int x, y;
    Vertice* next,*prev;
    bool isLeft;
}Vertice;

void merge(Vertice a[], int m,Vertice b[], int n, Vertice sorted[]) {
  	int i, j, k;
 
  	j = k = 0;
 
  	for (i = 0; i < m + n;) {
    	if (j < m && k < n) {
      		if (a[j].y > b[k].y) {
        		sorted[i] = a[j];
        		j++;
      		}
		    else {
		        sorted[i] = b[k];
		        k++;
      			}
      		i++;
    	}
    	else if (j == m) {
	      for (; i < m + n;) {
	        sorted[i] = b[k];
	        k++;
	        i++;
      		}
    	}
    	else {
      		for (; i < m + n;) {
        		sorted[i] = a[j];
        		j++;
        		i++;
      		}
    	}
  	}

}

void invert(Vertice arr[],int size)
{
	int c = size-1;
	Vertice new_arr[1000];

	for (int i = 0; i < size; i++)
	{
		new_arr[i] = arr[c];
		c--;
	}

	for (int i = 0; i < size; ++i)
		arr[i] = new_arr[i];
}


const int STACK_CAPACITY = 1000;
typedef int StackElement;

class Stack
{
 public:
  
  int myArrayX[STACK_CAPACITY];
  int myArrayY[STACK_CAPACITY];
  char myArray_names[STACK_CAPACITY][50];
  int myTop;

 public:

  Stack(): myTop(-1)
  {}

  bool empty() const
  {
    return (myTop == -1); 
  }
  void push(int valueX,int valueY,char* value2)
  {
    if (myTop < STACK_CAPACITY - 1) //Preserve stack invariant
   { 
      myTop++;
      myArrayX[myTop] = valueX;
      myArrayY[myTop] = valueY;
      strcpy(myArray_names[myTop],value2);
   }
   else
   {
      cerr << "*** Stack full -- can't add new value ***\n"
              "Must increase value of STACK_CAPACITY in Stack.h\n";
      exit(1);
   }
  }

  int topX() const
  {
  	if ( !empty() ) 
      return (myArrayX[myTop]);
   else
   {
      cerr << "*** Stack is empty -- returning garbage value ***\n";
      int garbage=-1;
      return garbage;
   }
  }

  int topY() const
  {
    if ( !empty() ) 
      return (myArrayY[myTop]);
   else
   {
      cerr << "*** Stack is empty -- returning garbage value ***\n";
      int garbage = -1;
      return garbage;
   }
  }

  const char* top_name() const
  {
    if ( !empty() ) 
      return (myArray_names[myTop]);
   else
   {
      cerr << "*** Stack is empty -- returning garbage value ***\n";
      char* garbage = NULL;
      return garbage;
   }
  }

  void pop()
  {
    if ( !empty() )
      myTop--;
   else
      cerr << "*** Stack is empty -- can't remove a value ***\n";
  }

};                    

char diagonais[480000][20];
int counter=0;

class Monotono
{

public:	
	Monotono();
	~Monotono();

	//Primitivas
	static int Area2(Vertice a, Vertice b, Vertice c) {
		return a.x * b.y - a.y * b.x + a.y * c.x - a.x * c.y + b.x * c.y - c.x * b.y;
	}

	static bool Left(Vertice a, Vertice b, Vertice c) {
		return Area2(a, b, c) > 0; 
	}

	static bool LeftOn(Vertice a, Vertice b, Vertice c) {
		return Area2(a, b, c) >= 0; 
	}

	// Passo 1 : ordenacao
	static Vertice* sorting(Vertice arr[],int n)
	{
		int max=0,min=999999;
		Vertice max_v,min_v;

		// pegando o vertice mais alto e mais baixo
		for (int i = 0; i < n; i++)
		{
			if (arr[i].y > max){
				max = arr[i].y;
				max_v = arr[i];
			}
			if (arr[i].y <= min){
				min = arr[i].y;
				min_v = arr[i];
			}
		}

		// particionando em duas cadeias
		Vertice right[1000];
		Vertice left[1000];
		int countX=0;
		int countY=0;
		bool t_left=true;

		Vertice aux = max_v;
		while(aux.next->y != max_v.y)
		{
			// vai para direita
			if (strcmp(aux.name,min_v.name) != 0 and t_left)
			{
				left[countY] = aux;
				left[countY].isLeft = true;
				countY++; 
			}
			// vai para a esquerda
			else
			{
				right[countX] = aux;
				right[countX].isLeft = false;
				countX++; 
				t_left = false;
			}
			aux = *aux.next;
		}
		right[countX] = aux;
		right[countX].isLeft = false;
		countX++; 

		// invertendo o vetor right
		invert(right,countX);

		Vertice* result = new Vertice[1000];

		merge(left,countY,right,countX,result);

		return result;

	}

	static void findDiagonals(Vertice v[], int size)
	{
		Stack st;
		Vertice stack_sim[1000];
		int stack_sim_top = 1;
		
		Vertice base = v[0];

		st.push(v[0].x,v[0].y,v[0].name);
		st.push(v[1].x,v[1].y,v[1].name);
		stack_sim[0] = v[0];
		stack_sim[1] = v[1];
		int t;
		t=1;

		for (int i = 2; i < size; i++)
		{	
			// se v[i] e adjacente a st[mais alto] mas nao e a s[base]
			if (((strcmp(v[i].prev->name,st.top_name()) == 0) and (strcmp(v[i].next->name,base.name) != 0)) or
				((strcmp(v[i].next->name,st.top_name()) == 0) and (strcmp(v[i].prev->name,base.name) != 0)))
			{
				
				while (st.myTop >= 1 && stack_sim_top >= 1)
			  	{
			  		// Pegando os dois vertices mais acima da pilha
			  		int x1 = stack_sim[stack_sim_top].x;
			  		int y1 = stack_sim[stack_sim_top].y;
			  		char aux_[50];
					strcpy(aux_,stack_sim[stack_sim_top].name);

					int x2 = stack_sim[stack_sim_top-1].x;
			  		int y2 = stack_sim[stack_sim_top-1].y;
			  		char aux_name[50];
					strcpy(aux_name,stack_sim[stack_sim_top-1].name);
			  		Vertice v1;
			  		Vertice v2;
			  		v1.x = x1;
			  		v1.y = y1;
			  		v2.x = x2;
			  		v2.y = y2;

			  		if (!v[i].isLeft)
			  		{
			  			if(!LeftOn(v2,v1,v[i]))
						{
							// Tracando as diagonaiscom o maior vertice na frente e o menor atras
				     		if (stampSort(v[i].name) > stampSort(aux_name))
							{
								strcpy(diagonais[counter],aux_name);
								counter++;
								strcpy(diagonais[counter],v[i].name);
								counter++;
							}
							else
							{
								strcpy(diagonais[counter],v[i].name);
								counter++;
								strcpy(diagonais[counter],aux_name);
								counter++;
							}
							st.pop();
						}
			  		}

			  		else
			  		{
						if (Left(v2,v1,v[i]))
						{
							// Tracando as diagonaiscom o maior vertice na frente e o menor atras
							if (stampSort(v[i].name) > stampSort(aux_name))
							{
								strcpy(diagonais[counter],aux_name);
								counter++;
								strcpy(diagonais[counter],v[i].name);
								counter++;
							}
							else
							{
								strcpy(diagonais[counter],v[i].name);
								counter++;
								strcpy(diagonais[counter],aux_name);
								counter++;
							}

							st.pop();	
						}
			  		}
					
					stack_sim_top--;
			  	}	
			  	st.push(v[i].x,v[i].y,v[i].name);
			  	stack_sim_top = st.myTop;
			  	stack_sim[stack_sim_top] = v[i];

			}
			
			// ui e adjacente a s1, mas nao e adjacente a st:
			else if(((strcmp(v[i].prev->name,st.top_name()) != 0) and (strcmp(v[i].next->name,base.name) == 0)) or
				((strcmp(v[i].next->name,st.top_name()) != 0) and (strcmp(v[i].prev->name,base.name) == 0)))
			{
				//	Trace as diagonais ui s2 , ui s3 ,. . . ,ui st , 
				//	desempilhe todos os vertices de S e empilhe 
				//	st seguido de ui
				int aux_x = st.topX();
				int aux_y = st.topY(); 
				char aux_name[50];
				strcpy(aux_name,st.top_name());

				int aux_top = st.myTop;

				for (int r = 1; r <= aux_top; r++)
				{
					// Tracando as diagonaiscom o maior vertice na frente e o menor atras
					if(stampSort(v[i].name) > stampSort(st.myArray_names[r])){
						strcpy(diagonais[counter],st.myArray_names[r]);
						counter++;
						strcpy(diagonais[counter],v[i].name);
						counter++;
					}
					else
					{
						strcpy(diagonais[counter],v[i].name);
						counter++;
						strcpy(diagonais[counter],st.myArray_names[r]);
						counter++;
					}

					st.pop();
				}
				st.pop();
				stack_sim_top=0;
				base.x = aux_x;
				base.y = aux_y;
				strcpy(base.name,aux_name);

				// atualizando a stack_sim
				stack_sim[stack_sim_top] = base;
				stack_sim[stack_sim_top+1] = v[i];
				stack_sim_top = 1;
				
				st.push(base.x,base.y,base.name);
				st.push(v[i].x,v[i].y,v[i].name);
				
			}

			// ui e adjacente a ambos s1 e st .
			else if(((strcmp(v[i].prev->name,st.top_name()) == 0) and (strcmp(v[i].next->name,base.name) == 0)) or
				((strcmp(v[i].next->name,st.top_name()) == 0) and (strcmp(v[i].prev->name,base.name) == 0)))
			{
				//Neste caso ui = un e o polÃ„Â±gono PiÃ¢Ë†â€™1, 
				//determinado pelos vertices ui,s1,s2,...,st, 
				//possui uma unica triangulacao. 
				//Trace as diagonais uis2, uis3,...,uistÃ¢Ë†â€™1 e pare 
				//(ou volte para o for...da na mesma).
				st.pop();
				while(st.myTop > 0)
				{
					strcpy(diagonais[counter],v[i].name);
					counter++;
					strcpy(diagonais[counter],st.top_name());
					counter++;
					st.pop();					
				}
			}
		}
		
	
	}

	/*
		
		Método auxliar para obter o numero de identificação de um vertice.
		Ele simplesmente pula o primeiro caracter ( v ) e acumula em um bu-
		ffer onde depois aplicamos o metodo atoi para obter o inteiro.
	
	*/

	static int stampSort(char *v)
	{
		int k=0;
		char ex[4];
		strcpy(ex,v);

		while(k <= strlen(v))
		{
			ex[k] = ex[k+1];
			k++;
		}
		ex[k] = '\n';

		return atoi(ex);
	}

	/*

		Este é o metodo que encapsula todo processamento feito para 
		tomar um polígono e seu tamanho como entrada e encontrar as
		diagonais dele.

	*/

	static void const triangularize(Vertice P[], int n)
	{
		Vertice* res;
		res = Monotono::sorting(P,n);

		Monotono::findDiagonals(res,n);

		delete [] res;

	} 
	
};

int main(int argc, char const *argv[])
{

	int sizeSample;
	int iteration;
	Vertice V[1000];
	iteration=0;
	cin >> sizeSample;
	
	// verificando condições de parada
	if (sizeSample != 0 && sizeSample <= 1000)
	{
		while(1)
		{
			int i;
			int size = 1;

			/*
				
				Lendo o polígono
	
			*/
			cin >> V[0].name;
			cin >> V[0].x;
			cin >> V[0].y;
			V[0].next = NULL;
			V[0].prev = NULL;

			for (i = 1; i < sizeSample; i++)
			{
				cin >> V[i].name;
				cin >> V[i].x;
				cin >> V[i].y;
				V[i].next = NULL;
				V[i].prev = &V[i-1];
				V[i-1].next = &V[i];
				size++;
			}
			V[i-1].next = &V[0];
			V[0].prev = &V[i-1];
			V[0].next = &V[1];

			// chamando método responsavel pela triangulação do poligono y-monotono lido
			Monotono::triangularize(V,size);
			
			// imprimindo triangulacoes
			for (int i = 0; i < counter-1; i+=2)
				printf("%s %s \n",diagonais[i],diagonais[i+1] );
			printf("\n");


			counter=0;
			cin >> sizeSample;
			if (sizeSample == 0)// verificando condição de parada
				break;
		}
	}


    return 0;
}
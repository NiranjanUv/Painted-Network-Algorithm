#include <iostream>

using namespace std;
using std::cout;

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int N; //number of nodes
int M; //number of edges

int FindTheEndNode(int E[], int source_dest, int edge)
{
	for (int n2 = 0; n2 < N; n2++)
		if (E[n2*M+edge] == source_dest)
			return n2;
						
}

void Reset(int arr[],int size)
{
	for(int i = 0; i < size; i++)
		arr[i] = 0;
}


int main(int argc, char **argv)
{

	if  (argc < 2){
		cout << "Please input the file name\n";
		return 0;
	}

	FILE *fid;
	int absNminus; //number of nodes in N-
	int absNplus; //number of nodes in N+

	fid = fopen(argv[1],"r");
	
	if (fid == NULL)
	{
		cout <<"File doesn't exists";
		return 0;
	}

	fscanf(fid,"%d",&N);	//read number of nodes
	fscanf(fid,"%d",&M);	//read number of edges

	fscanf(fid,"%d",&absNplus);	//read number of nodes in N+
	fscanf(fid,"%d",&absNminus); //read number of nodes in N-

	int *E = new int[N*M];		// space for Incidence Matrix
	char *C = new char[M];		// Space for Color Matrix

	int *Nminus = new int[absNminus];
	int *Nplus = new int[absNplus];

	for(int i = 0; i < absNplus; i++)	//read nodes in N+
		fscanf(fid,"%d",&Nplus[i]);

	for(int i = 0; i < absNminus; i++)	//read nodes in N-
		fscanf(fid,"%d",&Nminus[i]);


	Reset(E, M*N);			//Reset the matrix entries to zero.

	int node1, node2;

	for (int j = 0; j < M; j++)	//read edges and create matrix E
	{
		fscanf(fid,"%d",&node1);
		fscanf(fid,"%d",&node2);
		fscanf(fid,"%s",&C[j]);
		E[node1*M+j] = 1;
		E[node2*M+j] = -1;
			
	}

	fclose(fid);

	int *S = new int[N];

	for (int i = 0; i < N; i++)
		S[i] = -1;

	for (int i = 0 ; i < absNplus; i++)	//initialize S with N+
		S[i] = Nplus[i];

	int n = absNplus;

	int *teta = new int[N];		//function theta: N -> A
	teta[0] = -1;
	
	//Display the Incidence Matrix
	
	for (int i = 0; i < N; i++)
	{
		for(int j = 0; j < M; j++)
			cout << E[i*M+j] << "\t";			
		cout << "\n";
	}


	int *tmp = new int[M];
	bool loop = true;

	while (loop)
	{
		//B is a submatrix of E whose nodes are from S

		int *B = new int[n*M];	
		Reset(B, n*M);
	
		//create B from E according to nodes in E

		for (int i = 0; i < n; i++)	
			for (int j = 0; j < M; j++)
				B[i*M+j] = E[S[i]*M+j];


		for (int j = 0; j < M; j++)		
		{
			tmp[j] = 0;
			for (int i = 0; i < n; i++)
				tmp[j] += abs(B[i*M+j]);

			if (tmp[j] == 2)
				for (int i = 0; i < n; i++)
					B[i*M+j] = 0;
		}

		for (int i = 0; i < n; i++)
			for (int j = 0; j < M; j++)
			{
				if (B[i*M+j] == 1)  					//if edge belongs to Q+ and it is green or white ==> update S
					if ((C[j] == 'g') || (C[j] == 'w'))
					{
						
						S[n] = FindTheEndNode(E, -1, j); 	//find the other side of edge j
						teta[S[n]] = j;
						break;
					}

				if (B[i*M+j] == -1)  				//if edge belongs to Q- and it is green or black ==> update S
					if ((C[j] == 'g') || (C[j] == 'b'))
					{
						
						S[n] = FindTheEndNode(E, 1, j);	//find the other side of edge j
						teta[S[n]] = j;
						break;
					}
			}


		// if S didn't update there is a cut
		if (S[n] == -1)
		{
			cout << "The cut is S = S[0]";
			for (int i = 1; i < n; i++)
				cout << " & " << S[i];				

			cout << "\n";
			
			loop = false;

		}

		// if S touches N- there is a path
		for (int i = 0; i < absNminus; i++)
			if (S[n] == Nminus[i])
			{
				loop = false;
				cout << "Path is:\n";

				int node_ind = n;
				bool loop2=true;

				//use function teta to find path from N+ to N-
				while(loop2)
				{
					cout << S[node_ind] <<" " <<"j"<< teta[S[node_ind]] << "\t";
					
					int tmpEdge,tmpNode;

					tmpEdge = teta[S[node_ind]];

					tmpNode = FindTheEndNode(E,(-1*E[S[node_ind]*M+tmpEdge]),tmpEdge);

					//find the Node inside S
					for (int ii = 0; ii< n;ii++)
						if (S[ii]==tmpNode)
							node_ind = ii;
					
					//if we reach N+
					for (int ii = 0; ii < absNplus; ii++)
						if (S[node_ind] == Nplus[ii])
						{
							cout << S[node_ind] ;//<< "j" <<teta[S[node_ind]] << "\t"; 
							
							loop2 = false;
						}
				}
			}

		n++;
		delete[] B;
	}
	cout << "\n";
	
	return 0;
}
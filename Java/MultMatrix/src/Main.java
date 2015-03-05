import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;


public class Main {

	public static void main(String[] args){
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String input = null;
		String[] temp = new String[20];
		int opcao, linhas, colunas;
		opcao = linhas = colunas = 0;
		try {
			while (opcao != 3)
			{
				System.out.println("1. Multiplication");
				System.out.println("2. Optimized Multiplication");
				System.out.println("3. Exit");
				System.out.println("Selection?: ");
	
				input = br.readLine();
				opcao = Integer.valueOf(input);
				
				System.out.println("Dimensions: lins cols ? ");
				input = br.readLine();
				temp = input.split(" ");
				linhas = Integer.valueOf(temp[0]);
				colunas = Integer.valueOf(temp[1]);
				if (opcao == 1)
					OnMult(linhas,colunas,1);
				else if (opcao == 2)
					OnMultOptimized(linhas,colunas,1);
			}
		} catch (IOException e) {
			System.out.println("INPUT ERROR");
			e.printStackTrace();
		}
		


	}
	
	public static void OnMult(int lin, int col, int nt)
	{
		double[][] matrix_A = new double[lin][col];
		double[][] matrix_B = new double[lin][col];
		double[][] matrix_C = new double[lin][col];
		double startTime, elapsedTime, temp = 0;
		
		for(int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				matrix_A[i][j] = (double) 1;
				matrix_B[i][j] = (double) i+1;
			}
		}
		startTime = System.currentTimeMillis();
		for(int i = 0 ; i < lin ; i++) {
			for(int j = 0 ; j < col ; j++) {
				temp = 0;
				for (int k = 0 ; k < lin; k++) {
					temp += matrix_A[i][k] * matrix_B[k][j];
				}
				matrix_C[i][j] = temp;
			}
		}
		elapsedTime = (System.currentTimeMillis() - startTime) ;
		System.out.println((elapsedTime/1000) + " seconds");
	}
	public static void OnMultOptimized(int lin, int col, int nt)
	{
		double[][] matrix_A = new double[lin][col];
		double[][] matrix_B = new double[lin][col];
		double[][] matrix_C = new double[lin][col];
		double startTime, elapsedTime;
		
		for(int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				matrix_A[i][j] = (double) 1;
				matrix_B[i][j] = (double) i+1;
			}
		}
		startTime = System.currentTimeMillis();
		for(int i = 0 ; i < lin ; i++) {
			for(int k = 0 ; k < col ; k++) {
				for (int j = 0 ; j < lin; j++) {
					matrix_C[i][j] = matrix_C[i][j] + matrix_A[i][k] * matrix_B[k][j];
				}
			}
		}
		elapsedTime = (System.currentTimeMillis() - startTime) ;
		System.out.println((elapsedTime/1000) + " seconds");
	}
}

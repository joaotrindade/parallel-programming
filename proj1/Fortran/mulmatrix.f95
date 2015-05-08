subroutine OnMult(lin, col)
	integer, intent(in)  :: lin,col
    integer :: i, j,k
    REAL, DIMENSION(lin,col) :: matrix_A,matrix_B,matrix_C
    real temp,start_time,finish_time
              
    
    do i=1,lin
           do j =1,col
              matrix_A(i,j) = 1
              matrix_B(i,j) = i+1
              matrix_C(i,j) = 0
    	end do
    end do

	call cpu_time(start_time)

    do i=1,lin
      do j=1,col

        temp = 0
        do k=1,lin
          temp = temp + matrix_A(i,k) * matrix_B(k,j)
        end do
        matrix_C(i,j) = temp
        
      end do
    end do
    
	call cpu_time(finish_time)
    print '("Time = ",f10.4," seconds.")',finish_time-start_time
    

end subroutine OnMult

subroutine OnMultOptimized(lin, col)
	integer, intent(in)  :: lin,col
    integer :: i, j,k
    REAL, DIMENSION(lin,col) :: matrix_A,matrix_B,matrix_C
    real start_time,finish_time
              
    
    do i=1,lin
           do j =1,col
              matrix_A(i,j) = 1
              matrix_B(i,j) = i+1
              matrix_C(i,j) = 0
    	end do
    end do

	call cpu_time(start_time)

    do i=1,lin
      do k=1,col

        do j=1,lin
          matrix_C(i,j) = matrix_C(i,j) + matrix_A(i,k) * matrix_B(k,j)
        end do
          
      end do
    end do
    
	call cpu_time(finish_time)
    print '("Time = ",f10.4," seconds.")',finish_time-start_time
end subroutine OnMultOptimized

subroutine OnMultIntrinsic(lin,col)
	integer, intent(in)  :: lin,col
    integer :: i, j
    REAL, DIMENSION(lin,col) :: matrix_A,matrix_B,matrix_C
    real start_time,finish_time
              
    
    do i=1,lin
           do j =1,col
              matrix_A(i,j) = 1
              matrix_B(i,j) = i+1
              matrix_C(i,j) = 0
    	end do
    end do

	call cpu_time(start_time)
		matrix_C = matmul(matrix_A,matrix_B)
	call cpu_time(finish_time)
    print '("Time = ",f10.4," seconds.")',finish_time-start_time    
end subroutine OnMultIntrinsic




program multmatrix

integer :: lins, cols, opcao
do  
	print  *, '1. Multiplication'
	print  *, '2. Optimized Multiplication'
    print  *, '3. Fortran Built-in Multiplication Function'
	print  *, '4. Exit'
	print  *, 'Selecione uma opcao'
    read   *, opcao

	if (opcao == 4) then
		exit
    end if
    
    print  *, 'Dimensions: lins cols ?'
    read   *, lins, cols
    if (opcao == 1) then
		call OnMult(lins,cols)
    end if
    if (opcao == 2) then
		call OnMultOptimized(lins,cols)
    end if
    if (opcao == 3) then
		call OnMultIntrinsic(lins,cols)
    end if
    
end do

end program multmatrix
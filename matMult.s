.globl matMult
.equ wordsize, 4


.text

#int** matsum(int** a, int** b, int num_rows, int num_cols){
#int i,j;
#int** c;
#c = (int**) malloc(num_rows * sizeof(int*));
#for(i = 0; i < num_rows; i++){
# c[i] = (int*) malloc(num_cols *sizeof(int));
# for(j = 0; j < collen; j++){
#   c[i][j] = a[i][j] + b[i][j];
# }
#}
#}

matMult: #int** matMult(int** a, int num_rows_a, int num_cols_a, int** b, int num_rows_b, int num_cols_b)
  #on entry stack looks like
  #esp + 24:num_cols_b
  #esp + 20:num_rows_b
  #esp + 16:b
  #esp + 12:num_cols_a
  #esp + 8:num_rows_a
  #esp + 4: a
  #esp: return address
  
  #prologue
  push %ebp
  movl %esp, %ebp
  
  #according to C calling convention registers EAX, ECX, and EDX will not have
  #live values in their registers. All other registers may contain live values
  #so if we want to use them we have to push their values values. 
  #The question is, where should we save these values? 
  #The answer is the stack.
  
  #esi will alternate between a and b
  #edi will hold c
  #ecx will be i and k. no push because ecx does not contain a live value
  #edx will be j. no push because edx does not contain a live value
  #eax will be used for temp storage. no push because eax does not contain a live value
  #ebx will be used for temp storage
  subl $wordsize, %esp #make space for i on the stack
  subl $wordsize, %esp #make space for k on the stack
  subl $wordsize, %esp #make space for sum on the stack
  push %esi
  push %ebx
  push %edi
  
  
  
  #first step is to make space for c
  movl 3*wordsize(%ebp), %eax # place num_rows_a in eax
  shll $2, %eax #num_rows * sizeof(int*). a pointer is 4 bytes big so we shift eax by 2 to multiply 4
  push %eax
  call malloc 
  movl %eax, %edi #the result of a function call is placed in %eax
  #so we copy that value over to edi
  addl $wordsize, %esp #clear our argument off the stack
  
  xorl %ecx, %ecx #i = 0. this is a quick way to set a register to 0
  
  #i < rowlen_a == i - rowlen < 0
  #negation i - rowlen >= 0
  i_for:
    cmpl 3*wordsize(%ebp), %ecx #this is num_rows_a
    jge end_i_for
    
    #now make the call to malloc
    movl 7*wordsize(%ebp), %eax #put num_cols_b in %eax
    shll $2, %eax #num_cols * sizeof(int*). a pointer is 4 bytes big so we shift eax by 2 to multiply 4
    push %eax #put argument to malloc on the stack
    movl %ecx, -wordsize(%ebp) #now need to make sure to save %ecx because malloc could overwrite it
    call malloc
    addl $wordsize, %esp #clear our argument off the stack
    movl -wordsize(%ebp), %ecx #restore ecx to be i again
    
    movl %eax, (%edi, %ecx, wordsize) #c[i] = (int*) malloc(num_cols *sizeof(int));
    
    xorl %edx, %edx #j = 0
    
    #j < num_cols_b == j - num_cols < 0
    #negation: j - num_cols >= 0
    j_for:
      cmpl 7*wordsize(%ebp), %edx
      jge end_j_for

      #k < num_rows_b= k - num_cols < 0
      #negation: k - num_cols >= 0
      movl $0, -2*wordsize(%ebp) #initialize k=0
      movl -2*wordsize(%ebp), %ecx #move k into %ecx
      movl $0, -3*wordsize(%ebp) #initialize sum
      k_for:
	      cmpl 6*wordsize(%ebp), %ecx #%ecx is k
	      jge end_k_for

	      movl %ecx, -2*wordsize(%ebp) #put k onto the stack

	      #bring a[i][k] into %ebx
	      movl 2*wordsize(%ebp), %esi #put a int %esi
	      movl -wordsize(%ebp), %ecx #restore ecx to be i again
	      movl (%esi, %ecx, wordsize), %esi #put a[i] into esi
	      movl %ecx, -wordsize(%ebp) #now need to make sure to save %ecx because need it for k
	      movl -2*wordsize(%ebp), %ecx #move k into %ecx
	      movl (%esi, %ecx, wordsize), %ebx #put a[i][k] into ebx
      
	      #a[i][k] * b[k][j]
	      movl 5*wordsize(%ebp), %esi #put b int %esi
	      movl (%esi, %ecx, wordsize), %esi #put b[k] into esi
	      imull (%esi, %edx, wordsize), %ebx #put a[i][k] * b[k][j] into ebx
     	      addl %ebx, -3*wordsize(%ebp) #sum

	      incl %ecx #k++
	      jmp k_for
      end_k_for:
 
      movl -wordsize(%ebp), %ecx #restore ecx to be i again
      movl -3*wordsize(%ebp), %ebx #move sum into %ebx
      #eax currently points to c[i]
      movl %ebx, (%eax, %edx, wordsize) #c[i][j] = a[i][k] * b[k][j]
      
      incl %edx #j++
      jmp j_for
      
    end_j_for:
    
    incl %ecx #i++
    jmp i_for
  end_i_for:
      
  #put the return value in eax
  movl %edi, %eax
  
  
  #restore registers
  pop %edi
  pop %ebx
  pop %esi
  
  
  #epilogue
  movl %ebp, %esp
  pop %ebp
  
  #go back to where we were called from
  ret
  

.data
oldDist:
	.rept 102
	.long 0
	.endr
curDist:
	.rept 102
	.long 0
	.endr
temp:
	.rept 10
	.long 0
	.endr
length:
	.long 102
i:
	.long 0
j:
	.long 0
string1:
	.space 101
string2:
	.space 101
string1_length:
	.long 0
string2_length:
	.long 0
.text
.globl _start
_start:
	call editDistanceOpt
check_editDistanceOpt:
	movl $length, %ecx
	movl (%ecx), %ebx
	decl %ebx
	movl $curDist, %eax
	call array_read
	movl %ecx, %eax
done:	movl $0, %ecx 	

count_string_length:
	# input: %ecx - string location, %edx - location for string length
	# output: string length put into (%edx)
	movl $0, %ebx
loop_1:
	mov (%ecx),%al
	test %al,%al
	jz end_loop1
	incl %ecx
	incl %ebx
	jmp loop_1
end_loop1:
	movl %ebx,(%edx)
	ret

# Copy array1 to array2
# inputs: $eax - address of source array, $ebx = address of destination array, $ecx - array length
# outputs: curDist changed
copy_array:
loop_2:
	movl (%eax), %edx
	movl %edx, (%ebx)
	addl $4, %eax
	addl $4, %ebx
	decl %ecx
	jnz loop_2
	ret

# Assign array location
# inputs: %eax: address of array, %ebx: array index, %ecx: value to be assigned
array_write:
loop4:
	test %ebx,%ebx
	jz loop4_end
	addl $4, %eax
	decl %ebx
	jmp loop4
loop4_end:
	movl %ecx, (%eax)
	ret

# get value from array location
# inputs: %eax: address of array, %ebx: location within array
# output: %ecx: value read from array
array_read:
loop5:
	test %ebx,%ebx
	jz loop5_end
	addl $4, %eax
	decl %ebx
	jmp loop5
loop5_end:
	movl (%eax),%ecx
	ret

# get value from string location
# inputs: %eax: address of string, %ebx: location within string
# output: %cl: value read from  string
string_read:
string_read_loop:
	test %ebx,%ebx
	jz string_read_loop_end
	incl %eax
	decl %ebx
	jmp string_read_loop
string_read_loop_end:
	mov (%eax),%cl
	ret

# Find the minimum of two numbers
# inputs: %eax, %ebx
# output: %ecx
find_min:
	cmp %eax,%ebx
	jg find_min_1
	movl %ebx,%ecx
	jmp find_min_2
find_min_1:
	movl %eax,%ecx
find_min_2:
	ret

# Find the minimum of three numbers
# inputs: %eax, %ebx, %ecx
# output: %edx
find_min_3:
	cmp %eax,%ebx
	jg find_min_3_1
	movl %ebx,%edx
	jmp find_min_3_2
find_min_3_1:
	movl %eax,%edx
find_min_3_2:
	cmp %edx,%ecx
	jg find_min_3_3
	movl %ecx,%edx
find_min_3_3:
	ret

# Initialize array to index values 
# inputs: %eax: address of array, %ebx: Number of entries to be intitialized
array_init_to_index:
	movl $0, %edx
loop6:
	test %ebx,%ebx
	jz loop6_end
	movl %edx, (%eax)
	addl $4, %eax
	decl %ebx
	incl %edx
	jmp loop6
loop6_end:
	ret

# Calulates edit distance $string1 and $string2  
# output: edit distance in %ecx
editDistanceOpt:
#	Get lengths of string1 and string2 
	movl $string1, %ecx
	movl $string1_length, %edx
	call count_string_length
	movl $string2, %ecx
	movl $string2_length, %edx
	call count_string_length
# 	Set length to string2_length + 1
	movl $string2_length, %edx
	movl (%edx), %ecx
	incl %ecx
	movl $length, %edx
	movl %ecx, (%edx)
#	Initialize oldDist
	movl $oldDist, %eax
	movl $length, %edx
	movl (%edx), %ebx
	call array_init_to_index 	
# 	copy oldDist to  curDist
	movl $oldDist, %eax
	movl $curDist, %ebx
	movl $length, %edx
	movl (%edx), %ecx
	call copy_array
#	edit outer loop
#	initialize i to 1
	movl $i, %edx
	movl $1, (%edx)
edit_i_loop:
# 	check i < len(str1)+1
	movl $string1_length, %edx
	movl (%edx), %eax
	movl $i, %edx
	movl (%edx), %ebx
	cmp %eax, %ebx
	jg edit_i_loop_end
#	start actual logic
#	curDist[0] = i
	movl $i, %edx
	movl (%edx), %ecx
	movl $curDist, %eax
	movl $0, %ebx
	call array_write	

#	edit inner loop
#	initialize j to 1
	movl $j, %edx
	movl $1, (%edx)
edit_j_loop:
#	check j < len(str2)+1
	movl $string2_length, %edx
	movl (%edx), %eax
	movl $j, %edx
	movl (%edx), %ebx
	cmp %eax, %ebx
	jg edit_j_loop_end
#	start actual logic
#	get str1[i-1] and store it in %dl
	movl $string1, %eax
	movl $i, %ecx
	movl (%ecx), %ebx
	decl %ebx
	call string_read
	mov %cl, %dl
#	get str2[j-1] and store it in %dh
	movl $string2, %eax
	movl $j, %ecx
	movl (%ecx), %ebx
	decl %ebx
	call string_read
	mov %cl, %dh	
#      	if(str1[i-1] == str2[j-1])
	cmp %dh, %dl
	jne j_loop_else 
#	get oldDist[j-1]
	movl $oldDist, %eax
	movl $j, %edx
	movl (%edx), %ebx
	decl %ebx
	call array_read
#	curDist[j] = oldDist[j-1]
	movl $curDist, %eax
	movl $j, %edx
	movl (%edx), %ebx
#	%ecx already has the value we want to set from array_read
	call array_write
	jmp j_loop_if_else_end 
j_loop_else:
#	get oldDist[j] and store in temp[0]
	movl $oldDist, %eax
	movl $j, %edx
	movl (%edx), %ebx
	call array_read
	movl $temp, %edx
	movl %ecx, (%edx) 
#	get curDist[j-1] and store in temp[1]
	movl $curDist, %eax
	movl $j, %edx
	movl (%edx), %ebx
	decl %ebx
	call array_read
	movl $temp, %edx
	addl $4, %edx
	movl %ecx, (%edx)
#	get oldDist[j-1] and store in temp[2]
	movl $oldDist, %eax
	movl $j, %edx
	movl (%edx), %ebx
	decl %ebx
	call array_read
	movl $temp, %edx
	addl $8, %edx
	movl %ecx, (%edx)
#	move values from temp to registers
	movl $temp, %edx
	movl (%edx), %eax
	addl $4, %edx
	movl (%edx), %ebx
	addl $4, %edx
	movl (%edx), %ecx
	call find_min_3
#	move minimum from %edx to temp[0]
	movl $temp, %ecx
	movl %edx, (%ecx)
#	curDist[j] = min...
	movl $curDist, %eax
	movl $j, %edx
	movl (%edx), %ebx
 	movl $temp, %edx
	movl (%edx), %ecx
	incl %ecx
	call array_write	
j_loop_if_else_end:
#	increment loop counter j
	movl $j, %edx
	movl (%edx), %ecx
	incl %ecx
	movl %ecx, (%edx)
	jmp edit_j_loop
edit_j_loop_end:	
#	oldDist = curDist.copy()
	movl $curDist, %eax
	movl $oldDist, %ebx
	movl $length, %edx
	movl (%edx), %ecx
	call copy_array
#	increment loop counter i
	movl $i, %edx
	movl (%edx), %ecx
	incl %ecx
	movl %ecx, (%edx)
	jmp edit_i_loop
edit_i_loop_end:
	ret

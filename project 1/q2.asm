.data 
 A: .word 12, 4, 5, 88, 76
 k: .word 0
 n: .word 5
 answer: .word 0
 sentence: .asciiz "Largest element in the array\n"
.text
.globl main 
main:
	#write sentence
	li $v0, 4
	la $a0, sentence
	syscall
	
	#initialize arguments and call maxInArray
	la $a0 A
	lw $a1 n
	jal maxInArray
	sw $v0 answer
	
	#write answer
	li $v0, 1
	lw $a0, answer
	syscall
	
	#exit program
	li $v0, 10
	syscall
	

.globl maxInArray
maxInArray:

	move $t0, $a1		
	move $a1, $zero
	move $a2, $t0
	addi $sp, $sp, -4	
	sw   $ra, 0($sp)  
	jal  maxInInterval	
	lw   $ra, 0($sp)	
	addi $sp, $sp, 4
	jr   $ra	
	
	
.globl maxInInterval
maxInInterval:
	addi $sp, $sp, -8 # 2 items stack
	sw $ra, 4($sp) # save the return address
	sw $a1, 0($sp) # save the k
	#access A[k]
	sll  $t1, $a1, 2			
	add  $t1, $t1, $a0 
	lw   $t3, 0($t1) # t3 is now equal to A[k]
	
	addi $t2, $a2, -1 #find n-1
	#base case
	bne $t2, $a1, recursion
	move $v0, $t3
	jr $ra
	#recursive calls
	recursion:
	addi $a1, $a1, 1	# increase k by one
	# maxInInterval(A, k+1,n)
	jal  maxInInterval	
	lw   $a1, 0($sp)
	lw   $ra, 4($sp)
	addi $sp, $sp, 8	# pop return address and k from stack
	
	sll  $t1, $a1, 2			
	add  $t1, $t1, $a0 
	lw   $t3, 0($t1) # t3 is now equal to A[k]
	
	blt  $t3, $v0, else	# if A[k] > result of previous call, return A[k] otherwise, return
	move $v0, $t3
	else:
	jr $ra		

		
		
		
		
		
		



	
	

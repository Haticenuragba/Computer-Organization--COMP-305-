.data

	message: .asciiz "Enter an integer: "
	line: .asciiz "\n"

.text
main:
#Print message to want integer
	li $v0, 4
	la $a0, message
	syscall
	#Get integer input from keyboard
	li $v0, 5
	syscall
	#Assign taken integer to t0 register.
	move $t0, $v0
	addi $t2, $zero, 7
	addi $t1, $zero, 0
	while:
		blt $t0, $t1, exit #if t1 > number, go to exit label
		addi $t1, $t1, 7 #increase t1 by 7
		blt $t0, $t1, exit
		#print value
		li $v0, 1
		move $a0, $t1
		#print \n
		syscall	
		li $v0, 4
		la $a0, line
		syscall	
		j while #go to start of the loop
	exit:
		#finish program
		li $v0, 10
		syscall
			
	
